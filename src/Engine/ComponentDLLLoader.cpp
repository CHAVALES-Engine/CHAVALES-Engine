#include "ComponentDLLLoader.h"
#include <Debug.h>
#include <Component.h>
#include <ComponentRegister.h>
#include <filesystem>

#include "GameConfigurator.h"
#include "checkMLNew.h"
#include "Engine.h"

ComponentDLLLoader::~ComponentDLLLoader()
{
	unLoadAll();
}

ComponentDLLLoader& ComponentDLLLoader::instance()
{
	static ComponentDLLLoader instance;
	return instance;
}

// Definimos la funcion que exporta componentes y viene de la dll
// con nombre GetComponentFn usando un puntero a una funcion.
using GetComponentsFn = const core::ComponentDescriptor* (*)(size_t&);
bool ComponentDLLLoader::load(const std::string& path)
{
	// Comprueba si existe el fichero
	WIN32_FILE_ATTRIBUTE_DATA attr;
	if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &attr)) {
		Debug::error("COMPONENT DLL LOADER: Archivo no encontrado : ", path, " err=", GetLastError());
		return false;
	}

	// Entrada libreria a cargar.
	LoadedLibrary entry;
	entry.path = path;
	// Comprobamos duplicados.
	for (const auto& l : _libraries) {
		if (l.path == path) {
			Debug::error("COMPONENT DLL LOADER: Library already loaded [", path, "]");
			break;
		}
	}
	Debug::warning("COMPONENT DLL LOADER: loading [", entry.path, "]");
	entry.tempPath = _makeTempPath(entry.path); // Path temporal para leerlo y dejando libre al anterior.
	// Copia la libreria a una temporal.
	if (!CopyFileA(entry.path.c_str(), entry.tempPath.c_str(), FALSE)) {
		Debug::error("COMPONENT DLL LOADER: CopyFile failed for ", entry.path, " err=", GetLastError());
		return false;
	}

	entry.lastWriteTime = _getFileWriteTime(entry.path); // Momento en el que ha sido modificado el fichero.
	// Windows busca una dll en el path y la carga en la memoria del programa.
	if ((entry.handle = LoadLibraryA(entry.tempPath.c_str())) == nullptr) {
		Debug::error("COMPONENT DLL LOADER: LoadLibrary failed: ", entry.path, " err=", GetLastError());
		return false;
	}

	// ==== CONFIGURACION DEL JUEGO ====
	// Si no se especifica si cargar de argumentos o de toml, se busca la funcion configuradora en la dll
	if (core::GameConfigurator::instance()._configType.empty())
	{
		// Busca si tiene una funcion de configuracion para configurar el juego
		using ConfigFunc = void(*)();
		ConfigFunc confFunc = (void (*)())GetProcAddress(entry.handle, "configureGame");
		if (confFunc) confFunc();
	}

	// ==== COMPONENTES ====
	// Obtenemos la direccion de memoria de la funcion exportada "getPluginComponents".
	GetComponentsFn getComponents = (GetComponentsFn)GetProcAddress(entry.handle, "getPluginComponents");
	// Si no se ha devuelto nada lanzamos error y salimos.
	if (!getComponents) {
		Debug::error("COMPONENT DLL LOADER: La funcion de exportacion de componentes \"getPluginComponents\" no encontrada en ", entry.path);
		FreeLibrary(entry.handle);
		return false;
	}
	// Cogemos los componentDescriptor de todos los componentes en la dll.
	size_t count = 0;
	const core::ComponentDescriptor* descriptors = getComponents(count);
	Debug::out("COMPONENT DLL LOADER: Registering ", std::to_string(count), " components on [", entry.path, "]");
	// Registramos los componentes cargados en el registro del engine,
	for (size_t i = 0; i < count; ++i) {
		ComponentRegister::instance().registComponent(
			descriptors[i].name,
			descriptors[i].ComponentConstructor
		);
	}

	// Mete la libreria a un vector.
	_libraries.push_back(entry);

	return true;
}

bool ComponentDLLLoader::loadAll(const std::string& path)
{
	Debug::out("[Component DLLLoader] Loading all dlls from: ", path);
	if (!std::filesystem::exists(path))
	{
		Debug::error("COMPONENT DLL LOADER: Carpeta no encontrada: ", path);
		return false;
	}

	std::vector<std::string> pendingLibraries;
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.path().extension() != ".dll") continue;
		std::string stem = entry.path().stem().string();

#if _DEBUG
		// Comprobar si el stem termina en _d
		if (stem.size() < 2 || stem.substr(stem.size() - 2) != "_d")
			continue;
#else
		// En release ignorar los que terminen en _d
		if (stem.size() >= 2 && stem.substr(stem.size() - 2) == "_d")
			continue;
#endif

		pendingLibraries.push_back(stem + ".dll");
	}
	for (std::string& lib : pendingLibraries)
	{
		bool ok = load(path + lib);
		if (!ok) {
			unLoadAll();
			return false;
		}
	}
	return true;
}

void ComponentDLLLoader::unLoadAll()
{
	for (auto& library : _libraries)
	{
		_unload(library);
	}
	_libraries.clear();

	std::filesystem::remove_all(_getProcessTempRoute());
}

bool ComponentDLLLoader::unload(const std::string& path)
{
	bool ok = true;
	for (auto it = _libraries.begin(); it != _libraries.end(); ++it) {
		if (it->path == path) {
			ok = _unload(*it);
			_libraries.erase(it); // eliminar del vector
			return ok;
		}
	}
	return ok;
}

#ifdef _DEBUG

bool ComponentDLLLoader::checkReload()
{
	bool reloaded = false;
	// Copia los paths a recargar.
	std::vector<std::string> toReload;
	toReload.reserve(_libraries.size());

	for (auto& entry : _libraries) {
		FILETIME current = _getFileWriteTime(entry.path);

		if (CompareFileTime(&current, &entry.lastWriteTime) > 0) {
			toReload.push_back(entry.path);
		}
	}
	// Recargamos solo si el path no esta siendo modificado
	for (const auto& path : toReload) {
		for (auto& entry : _libraries) {
			if (entry.path == path && _isFileFree(entry.path)) {
				_reload(entry);
				reloaded = true;
				break;
			}
		}
	}
	return reloaded;
}

#endif

using GetResourcesFn = const std::string* (*)(size_t&);
void ComponentDLLLoader::preloadResources() const
{
	for (auto entry : _libraries)
	{
		// ==== PRECARGA DE RECURSOS ====
		// Obtenemos la direccion de memoria de la funcion exportada "getPluginPreloadResources".
		GetResourcesFn getResources = (GetResourcesFn)GetProcAddress(entry.handle, "getPluginPreloadResources");
		// Si no se ha devuelto nada lanzamos error y salimos.
		if (!getResources) {
			Debug::error("COMPONENT DLL LOADER: La funcion de exportacion de resources \"getPluginPreloadResources\" no encontrada en  ", entry.path);
			FreeLibrary(entry.handle);
			continue;
		}
		// Cogemos los paths de los recursos marcados en la dll.
		size_t count = 0;
		const std::string* resPaths = getResources(count);
		// Precargamos los recursos
		for (size_t i = 0; i < count; ++i)
			Engine::instance()->preload(resPaths[i]);
	}
}

bool ComponentDLLLoader::_unload(LoadedLibrary& library)
{
	Debug::warning("Unloading[", library.path, "]");
	// Guardar componentes a desregistrar
	std::vector<std::string> toUnregister;
	GetComponentsFn getComponents =
		(GetComponentsFn)GetProcAddress(library.handle, "getPluginComponents");
	if (getComponents) {
		size_t count = 0;
		const core::ComponentDescriptor* descs = getComponents(count);
		for (size_t i = 0; i < count; ++i)
			toUnregister.push_back(descs[i].name);
	}
	for (const auto& name : toUnregister)
		ComponentRegister::instance().unregisterComponent(name);

	//Descargar assets en hotReload
	GetResourcesFn getResources = (GetResourcesFn)GetProcAddress(library.handle, "getPluginPreloadResources");
	// Si no se ha devuelto nada lanzamos error y salimos.
	if (!getResources) {
		Debug::error("COMPONENT DLL LOADER: La funcion de exportacion de resources \"getPluginPreloadResources\" no encontrada en  ", library.path);
	}
	else {
		// Cogemos los paths de los recursos marcados en la dll.
		size_t count = 0;
		const std::string* resPaths = getResources(count);
		// Precargamos los recursos
		for (size_t i = 0; i < count; ++i)
			Engine::instance()->unload(resPaths[i]);
	}
	if (!FreeLibrary(library.handle))return false;
	library.handle = nullptr;
	//return true;
	return DeleteFileA(library.tempPath.c_str());
}

void ComponentDLLLoader::_reload(LoadedLibrary& library)
{
	// Descarga y recarga de libreria.
	std::string	path = library.path;
	std::string newTempPath = _makeTempPath(path);

	if (!_isFileFree(path) || !_isFileFree(newTempPath))
	{
		Debug::warning("[COMPONENTDLLLOADER] Libreria ", path, " en uso. Se seguira usando la copia.");
		return;
	}
	if (!unload(path)) {
		Debug::error("[COMPONENT DLL LOADER] Reload: Something went wrong with unload library");
		return;
	}
	if (!load(path)) {
		Debug::error("[COMPONENT DLL LOADER] Reload: Something went wrong with load library");
		return;
	}

	Debug::out("Reload OK: ", path);
}

std::string ComponentDLLLoader::_makeTempPath(const std::string& originalPath)
{
	std::string basePath = _getProcessTempRoute();

	std::filesystem::create_directories(basePath);

	std::filesystem::path dir(basePath);
	std::filesystem::path original(originalPath);
	return (dir / (original.stem().string() + "_hot" + original.extension().string())).string();
}

FILETIME ComponentDLLLoader::_getFileWriteTime(const std::string& path)
{
	FILETIME ft = {};
	WIN32_FILE_ATTRIBUTE_DATA data;
	if (GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &data))
		ft = data.ftLastWriteTime;
	return ft;
}

bool ComponentDLLLoader::_isFileFree(const std::string& path)
{
	// Para comprobar si el fichero esta libre, intentamos abrirlo con shareMode = 0 (haces que nadie mas pueda abrirlo).
	// Si ya estaba abierto -> HANDLE = INVALID_HANDLE_VALUE.
	HANDLE h = CreateFileA(
		path.c_str(),   // ruta del fichero.
		GENERIC_READ,   // solo lectura.
		0,              // dwShareMode = 0 acceso exclusivo, nadie mas puede abrirlo.
		nullptr,        // seguridad por defecto.
		OPEN_EXISTING,  // solo abrir si ya existe, no crear.
		0,              // atributos normales.
		nullptr         // sin plantilla.
	);
	if (h == INVALID_HANDLE_VALUE) return false;
	CloseHandle(h);
	return true;
}

std::string ComponentDLLLoader::_getProcessTempRoute()
{
	// "plugins/game.dll" -> "<LOCALAPPDATA>/ChavalesEngineReload/HotReload/<PID>/game_hot.dll"
	char* buffer = nullptr;
	size_t len = 0;

	std::filesystem::path basePath;

	if (_dupenv_s(&buffer, &len, "LOCALAPPDATA") == 0 && buffer)
	{
		basePath = std::filesystem::path(buffer);
		free(buffer);
	}
	else
	{
		basePath = std::filesystem::temp_directory_path();
	}

	basePath /= "ChavalesEngineReload";
	basePath /= "HotReload";
	basePath /= std::to_string(GetCurrentProcessId()); // Carpeta por el PID del proceso.

	return basePath.string();
}
