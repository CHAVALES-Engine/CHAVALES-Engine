#include "AudioResource.h"
#include "AudioModule.h"
#include "checkMLNew.h"

bool AudioResource::load()
{
	if (_state != UNLOADED || _path.empty() || !Resource::load()) return false;

	try {
		std::string fullPath = _path + _id;
		if (_audioModule->loadSound(fullPath, _id)) {
			_state = LOADED;
			return true;
		}
		_state = LOAD_ERROR;
		return false;
	}
	catch (const std::exception& e) {
		Debug::error("[AudioResource] Error cargando ", _id, ": ", e.what());
		_state = LOAD_ERROR;
		return false;
	}
}

bool AudioResource::unLoad()
{
	if (!Resource::unLoad()) return false;
	_audioModule->unloadSound(_id);
	_state = UNLOADED;
	return true;
}