#include "MeshResource.h"
#include "Debug.h"
#include "Ogre.h"

MeshResource::MeshResource()
{
}

bool MeshResource::load()
{
	if (_state != UNLOAD && _path.empty()) return false;

	try {
		_ptr = Ogre::MeshManager::load(_id, _path);
		_state = UNLOAD;
		return true;
	}
	catch (std::exception e) {
		Debug::error(e.what());
	}

	return true;
}

bool MeshResource::unLoad()
{
	try {
		if (_ptr) {
			Ogre::MeshManager::unload(_id, _path);
			_ptr = nullptr;
			_state = UNLOAD;
			return true;
		}
	}
	catch (std::exception e) {
		Debug::error(e.what());
	}

	_ptr = nullptr;
	_state = ERROR;
	return false;
}

Ogre::MeshPtr MeshResource::getMeshPtr() const
{
	try
	{
		if (_ptr && _state == LOAD)
		{

		}
	}
	catch (std::exception e) {
		Debug::error(e.what());
	}
	return nullptr;
}
