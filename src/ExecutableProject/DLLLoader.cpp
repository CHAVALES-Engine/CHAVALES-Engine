#include "DLLLoader.h"
#include <Debug.h>

DLLLoader::DLLLoader()
{
}

DLLLoader::~DLLLoader()
{
	unLoadAll();
}

bool DLLLoader::load(const std::string& path)
{
	// Comprobamos duplicados
	for (const auto& l : _libraries) {
		if (l.path == path) {
			core::Debug::error("DLL already loaded ", path);
			return false;
		}
	}

	HMODULE library = nullptr;// Base direction of module in memory
	// Windows search a library and loads on program memory
	if ((library = LoadLibraryA(path.c_str())) == nullptr) {
		core::Debug::error("LoadLibrary failde: ", path, " err=", GetLastError());
		return false;
	}
	// TODO: register components

	// Adds library handler to vector
	_libraries.push_back({ library, path });

	return true;
}

void DLLLoader::unLoadAll()
{
	for (auto& library : _libraries) {
		// Windows unloads library
		FreeLibrary(library.handle);
	}

	_libraries.clear();
}

bool DLLLoader::unload(const std::string& path)
{
	// TODO for hot-reloading
	return true;
}
