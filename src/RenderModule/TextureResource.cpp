#include "TextureResource.h"
#include <Ogre.h>
#include "checkMLNew.h"

TextureResource::TextureResource(const std::string& id, const std::string& path) :
	Resource(id, path, TEXTURE), _texturePtr(nullptr)
{
}

TextureResource::~TextureResource()
{
	if (_texturePtr) {
		unLoad();
	}

	_texturePtr.reset();
	_state = LOAD_ERROR;
}

bool TextureResource::load()
{
	if (_state != UNLOADED || _path.empty() || !Resource::load()) return false;

	try {
		_texturePtr = Ogre::TextureManager::getSingleton().getByName(_id, _path);
		if (!_texturePtr) {
			_texturePtr = Ogre::TextureManager::getSingleton().load(
				_id, _path, Ogre::TEX_TYPE_2D, 0);
		}

		if (!_texturePtr) {
			Debug::error("[TextureResource] No se pudo cargar: ", _id);
			_state = LOAD_ERROR;
			return false;
		}

		_state = LOADED;
		return true;
	}
	catch (const std::exception& e) {
		Debug::error("[TextureResource] Error cargando ", _id, ": ", e.what());
		_state = LOAD_ERROR;
		return false;
	}
}

bool TextureResource::unLoad()
{
	if (!Resource::unLoad()) return false;

	try {
		if (_texturePtr) {
			auto& mgr = Ogre::TextureManager::getSingleton();
			if (mgr.resourceExists(_texturePtr->getName(), _texturePtr->getGroup())) {
				mgr.unload(_texturePtr->getHandle());
			}
			_texturePtr.reset();
			_state = UNLOADED;
			return true;
		}
	}
	catch (const std::exception& e) {
		Debug::error("[TextureResource] Error descargando: ", e.what());
	}
	return false;
}

Ogre::TexturePtr TextureResource::getTexturePtr() const
{
	return _texturePtr;
}