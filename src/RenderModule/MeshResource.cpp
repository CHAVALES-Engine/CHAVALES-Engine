#include "MeshResource.h"
#include "Debug.h"
#include <filesystem>
#include "checkMLNew.h"

MeshResource::MeshResource(const std::string& id, const std::string& path) :
	Resource(id, path, MESH), _meshPtr(nullptr)
{
}

bool MeshResource::load()
{
	if (_state != UNLOADED || _path.empty() || !Resource::load()) return false;

	try {
		auto& mgr = Ogre::MeshManager::getSingleton();

		_meshPtr = mgr.load(_id, _path);

		if (!_meshPtr) {
			Debug::error("[MeshResource] No se pudo cargar: ", _id);
			_state = LOAD_ERROR;
			return false;
		}
		_path = _meshPtr->getGroup();

		_state = LOADED;
		return true;
	}
	catch (std::exception e) {
		Debug::error("[MeshResource] Error cargando ", _path, ": ", e.what());
		_state = LOAD_ERROR;
		return false;
	}

}

bool MeshResource::unLoad()
{
	// El metodo padre funciona como programacion defensiva.
	if (!Resource::unLoad()) return false;
	if (!_meshPtr) {
		_state = UNLOADED;
		return true;
	}

	try {
		if (_meshPtr) {
			auto& mgr = Ogre::MeshManager::getSingleton();
			std::string meshName = _meshPtr->getName();
			std::string groupName = _path;

			// Descargamos mesh de ogre.
			if (mgr.resourceExists(meshName, groupName)) {
				mgr.remove(meshName, groupName);
			}
			// Limpiamos el recurso.
			_meshPtr.reset();
			_state = UNLOADED;
			return true;
		}
	}
	catch (const Ogre::RuntimeAssertionException& e) {
		Debug::warning("[MeshResource] Mesh ya fue descargada: ",
			_meshPtr ? _meshPtr->getName() : "desconocida");
		_meshPtr.reset();
		_state = UNLOADED;
		return true;
	}
	catch (const std::exception& e) {
		Debug::error("[MeshResource] Error descargando: ", e.what());
		_state = LOAD_ERROR;
		return false;
	}

	return false;
}

Ogre::MeshPtr MeshResource::getMeshPtr() const
{
	return _meshPtr;
}