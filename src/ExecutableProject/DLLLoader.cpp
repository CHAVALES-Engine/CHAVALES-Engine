#include "DLLLoader.h"
#include <Windows.h>

DLLLoader::DLLLoader()
{
}

DLLLoader::~DLLLoader()
{
	unLoadAll();
}

bool DLLLoader::load(const std::string& path)
{
	HMODULE library = nullptr;// Base direction of module in memory

	// Windows search a library and loads on program memory
	if ((library = LoadLibraryA(path.c_str())) == nullptr) return false;

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
