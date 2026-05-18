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
		auto& mgr = Ogre::TextureManager::getSingleton();

		// Intentar obtener si ya existe
		_texturePtr = mgr.getByName(_id, _path);

		if (!_texturePtr)
			_texturePtr = mgr.getByName(_id);

		// Si aun no existe cargar del archivo
		if (!_texturePtr)
			_texturePtr = mgr.load(_id, _path, Ogre::TEX_TYPE_2D, 0);

		if (!_texturePtr) {
			Debug::error("[TextureResource] No se pudo cargar: ", _id);
			_state = LOAD_ERROR;
			return false;
		}

		_path = _texturePtr->getGroup();
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
		auto& mgr = Ogre::TextureManager::getSingleton();
		std::string textureName = _texturePtr->getName();
		std::string groupName = _path;

		if (mgr.resourceExists(textureName, groupName)) {
			mgr.remove(textureName, groupName);
		}
		_texturePtr.reset();
		_state = UNLOADED;
		return true;
	}
	catch (const Ogre::RuntimeAssertionException& e) {
		Debug::warning("[TextureResource] Textura ya fue descargada: ",
			_texturePtr ? _texturePtr->getName() : "desconocida");
		_texturePtr.reset();
		_state = UNLOADED;
		return true;
	}
	catch (const std::exception& e) {
		Debug::error("[TextureResource] Error descargando: ", e.what());
		_state = LOAD_ERROR;
		return false;
	}
	return false;
}

Ogre::TexturePtr TextureResource::getTexturePtr() const
{
	return _texturePtr;
}