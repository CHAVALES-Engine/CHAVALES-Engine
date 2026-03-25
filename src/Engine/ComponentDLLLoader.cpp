#include "ComponentDLLLoader.h"
#include <Debug.h>
#include <Component.h>
#include <ComponentRegister.h>
#include <filesystem>

std::vector<ComponentDLLLoader::LoadedLibrary> ComponentDLLLoader::_libraries;

ComponentDLLLoader::~ComponentDLLLoader()
{
	unLoadAll();
}

// Definimos la funcion que exporta componentes y viene de la dll
// con nombre GetComponentFn usando un puntero a una funcion.
using GetComponentsFn = const core::ComponentDescriptor* (*)(size_t&);
bool ComponentDLLLoader::load(const std::string& path)
{
	// Entrada libreria a cargar.
	LoadedLibrary entry;
	// Comprobamos duplicados.
	for (const auto& l : _libraries) {
		if (l.path == path) {
			Debug::error("DLL already loaded ", path);
			unload(path);
			entry = l;
			break;
		}
	}
	if (entry.path != path)
	{
		entry.path = path;
		entry.tempPath = _makeTempPath(path);
	}

	// copiamos la libreria en un archivo temporal
	try
	{
		std::filesystem::copy_file(entry.path, entry.tempPath, std::filesystem::copy_options::overwrite_existing);
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		Debug::error("LoadLibrary failed: CopyLibrary: [", path,"] ", e.what());
		return false;
	}

	// Windows busca una dll y la carga en la memoria del programa.
	if ((entry.handle = LoadLibraryA(path.c_str())) == nullptr) {
		Debug::error("LoadLibrary failed: ", path, " err=", GetLastError());
		return false;
	}

	// Obtenemos la direccion de memoria de la funcion exportada "getPluginComponents".
	GetComponentsFn getComponents = (GetComponentsFn)GetProcAddress(entry.handle, "getPluginComponents");
	// Si no se ha devuelto nada lanzamos error y salimos
	if (!getComponents) {
		Debug::error("The export components function \"getPluginComponents not\" found in ", path);
		FreeLibrary(entry.handle);
		return false;
	}

	// Cogemos los componentDescriptor de todos los componentes en la dll.
	size_t count = 0;
	const core::ComponentDescriptor* descriptors = getComponents(count);

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
		// Windows descarga la libreria.
		FreeLibrary(library.handle);
	}

	_libraries.clear();
}

bool ComponentDLLLoader::unload(const std::string& path)
{
	// Entrada libreria a cargar.
	LoadedLibrary entry;
	// Comprobamos duplicados.
	for (auto& l : _libraries) {
		if (l.path == path) {
			_unload(l);
			break;
		}
	}
	return true;
}

bool ComponentDLLLoader::checkReload()
{
	bool reload = false;
	for (auto& entry : _libraries) {
		FILETIME current = _getFileWriteTime(entry.path);

		// CompareFileTime devuelve 1 si current > lastWriteTime
		if (CompareFileTime(&current, &entry.lastWriteTime) > 0) {
			_reload(entry);
		}
	}
	return reload;
}

void ComponentDLLLoader::setReloadCallback(ReloadCallback const& cb)
{
	_reloadCallback = cb;
}

void ComponentDLLLoader::_unload(LoadedLibrary& entry)
{
	Debug::warning("Unloading[", entry.path, "]");
	FreeLibrary(entry.handle);
}

void ComponentDLLLoader::_reload(LoadedLibrary& library)
{
	_unload(library);
	if (!load(library.path))
		Debug::error("Cannot unload[" + library.tempPath + "]");
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
