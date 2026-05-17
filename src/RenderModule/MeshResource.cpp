#include "MeshResource.h"
#include "Debug.h"

MeshResource::MeshResource(const std::string& id, const std::string& path) :
	Resource(id, path, MESH) {
}

bool MeshResource::load()
{
	if (_state != UNLOADED || _path.empty()) return false;

	try {
		// Carga el recurso
		_meshPtr = Ogre::MeshManager::getSingletonPtr()->load(
			_path,"General");
		_state = LOADED;
		return true;
	}
	catch (std::exception e) {
		Debug::error(e.what());
	}

	return true;
}

bool MeshResource::unLoad()
{
	// El metodo padre funciona como programacion defensiva.
	if (!Resource::unLoad()) return false;
	try {
		if (_meshPtr) {
			// Descargamos mesh de ogre.
			Ogre::MeshManager::getSingleton().unload(_path);
			// Limpiamos el recurso.
			_meshPtr.reset();
			_state = UNLOADED;
			return true;
		}
	}
	catch (std::exception e) {
		Debug::error(e.what());
	}

	return false;
}

Ogre::MeshPtr MeshResource::getMeshPtr() const
{
	return _meshPtr;
}
