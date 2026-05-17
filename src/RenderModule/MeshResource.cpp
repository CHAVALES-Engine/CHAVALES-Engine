#include "MeshResource.h"
#include "Debug.h"
#include <filesystem>

MeshResource::MeshResource(const std::string& id, const std::string& path) :
	Resource(id, path, MESH) {
}

bool MeshResource::load()
{
	if (_state != UNLOADED || _path.empty() || !Resource::load()) return false;

	try {
		_meshPtr = Ogre::MeshManager::getSingleton().getByName(_id, _path);
		if (!_meshPtr)
			_meshPtr = Ogre::MeshManager::getSingleton().getByName(_id);
		if (!_meshPtr) {
			try {
				_meshPtr = Ogre::MeshManager::getSingleton().load(_id, _path);
			}
			catch (const std::exception& e) {
				Debug::error("[RenderModule] Error precargando ", _id, ": ", e.what());
				return false;
			}
		}
		_state = LOADED;
		return true;
	}
	catch (std::exception e) {
		Debug::error("[MeshResource] Error cargando ", _path, ": ", e.what());
		_state = LOAD_ERROR;
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
void MeshResource::setMeshPtr(Ogre::MeshPtr ptr)
{
	_meshPtr = ptr;
}
