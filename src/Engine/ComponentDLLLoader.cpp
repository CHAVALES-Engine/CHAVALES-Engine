#include "ComponentDLLLoader.h"
#include <Debug.h>
#include <Component.h>
#include <ComponentRegister.h>
#include <filesystem>

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
		Debug::error("File not found: ", path, " err=", GetLastError());
		return false;
	}

	// Entrada libreria a cargar.
	LoadedLibrary entry;
	entry.path = path;
	//Debug::out("Checking if library [", path, "] already exists");
	// Comprobamos duplicados.
	for (const auto& l : _libraries) {
		if (l.path == path) {
			Debug::error("load: Library already loaded [", path, "]");
			break;
		}
	}
	Debug::warning("loading [", entry.path, "]");
	entry.tempPath = _makeTempPath(entry.path); // Path temporal para leerlo y dejando libre al anterior.
	// Copia la libreria a una temporal.
	if (!CopyFileA(entry.path.c_str(), entry.tempPath.c_str(), FALSE)) {
		Debug::error("CopyFile failed for ", entry.path, " err=", GetLastError());
		return false;
	}
	entry.lastWriteTime = _getFileWriteTime(entry.path); // Momento en el que ha sido modificado el fichero.
	// Windows busca una dll en el path y la carga en la memoria del programa.
	if ((entry.handle = LoadLibraryA(entry.tempPath.c_str())) == nullptr) {
		Debug::error("LoadLibrary failed: ", entry.path, " err=", GetLastError());
		return false;
	}
	// Obtenemos la direccion de memoria de la funcion exportada "getPluginComponents".
	GetComponentsFn getComponents = (GetComponentsFn)GetProcAddress(entry.handle, "getPluginComponents");
	// Si no se ha devuelto nada lanzamos error y salimos.
	if (!getComponents) {
		Debug::error("The export components function \"getPluginComponents not\" found in ", entry.path);
		FreeLibrary(entry.handle);
		return false;
	}
	// Cogemos los componentDescriptor de todos los componentes en la dll.
	size_t count = 0;
	const core::ComponentDescriptor* descriptors = getComponents(count);
	Debug::out("Registering ", std::to_string(count), " components on [", entry.path, "]");
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

void ComponentDLLLoader::unLoadAll()
{
	for (auto& library : _libraries) {
		_unload(library);
	}
	_libraries.clear();
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

void ComponentDLLLoader::setReloadCallback(ReloadCallback const& cb)
{
	_reloadCallback = cb;
}

bool ComponentDLLLoader::_unload(LoadedLibrary& library)
{
	Debug::warning("Unloading[", library.path, "]");
	if (!FreeLibrary(library.handle))return false;
	library.handle = nullptr;
	return DeleteFileA(library.tempPath.c_str());
}

void ComponentDLLLoader::_reload(LoadedLibrary& library)
{
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

	// Descarga y recarga de libreria.
	std::string	path = library.path;
	if (!unload(path)) {
		Debug::error("Reload: Something went wrong with unload library");
		return;
	}
	if (!load(path)) {
		Debug::error("Reload: Something went wrong with load library");
		return;
	}
	if (_reloadCallback) _reloadCallback(path);

	Debug::out("Reload OK: ", path);
}

std::string ComponentDLLLoader::_makeTempPath(const std::string& originalPath) {
	// "plugins/game.dll" -> "plugins/game_hot.dll"
	auto dotPos = originalPath.rfind('.');
	std::string base = (dotPos != std::string::npos) // valor por defecto de size_t
		? originalPath.substr(0, dotPos)
		: originalPath;
	std::string ext = (dotPos != std::string::npos)
		? originalPath.substr(dotPos)
		: "";
	return base + "_hot" + ext;
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