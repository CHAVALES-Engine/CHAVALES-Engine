#include "DLLLoader.h"
#include <Debug.h>
#include <Component.h>
#include <ComponentRegister.h>

DLLLoader::DLLLoader()
{
}

DLLLoader::~DLLLoader()
{
	unLoadAll();
}

// Definimos la funcion que exporta componentes y viene de la dll
// con nombre GetComponentFn usando un puntero a una funcion
using GetComponentsFn = const core::ComponentDescriptor* (*)(size_t&);
bool DLLLoader::load(const std::string& path)
{
	// Comprobamos duplicados
	for (const auto& l : _libraries) {
		if (l.path == path) {
			core::Debug::error("DLL already loaded ", path);
			return false;
		}
	}

	HMODULE library = nullptr; // Direccion base de un modulo en memoria
	// Windows busca una dll y la carga en la memoria del programa
	if ((library = LoadLibraryA(path.c_str())) == nullptr) {
		core::Debug::error("LoadLibrary failed: ", path, " err=", GetLastError());
		return false;
	}
	// TODO: register components
	// Obtenemos la direccion de memoria de la funcion exportada "getPluginComponents"
	GetComponentsFn getComponents = (GetComponentsFn)GetProcAddress(library, "getPluginComponents");
	// Si no se ha devuelto nada lanzamos error y salimos
	if (!getComponents) {
		core::Debug::error("The export components function \"getPluginComponents not\" found in ", path);
		FreeLibrary(library);
		return false;
	}
	// Cogemos los componentDescriptor de todos los componentes en la dll
	size_t count = 0;
	const core::ComponentDescriptor* descriptors = getComponents(count);
	// Registramos los componentes cargados en el registro del engine
	for (size_t i = 0; i < count; ++i) {
		ComponentRegister::instance().registComponent(
			descriptors[i].name,
			descriptors[i].factory
		);
	}

	// Mete la libreria a un vector
	_libraries.push_back({ library, path });

	return true;
}

void DLLLoader::unLoadAll()
{
	for (auto& library : _libraries) {
		// Windows descarga la libreria
		FreeLibrary(library.handle);
	}

	_libraries.clear();
}

bool DLLLoader::unload(const std::string& path)
{
	// TODO for hot-reloading
	return true;
}
