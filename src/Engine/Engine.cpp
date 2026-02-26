
#include "Engine.h"

#include "PlatformModule.h"
#include "AudioModule.h"

Engine* Engine::_instance = nullptr;

bool Engine::init()
{
	if (_instance == nullptr) _instance = new Engine();

	return _instance->_initPriv();
}

Engine* Engine::instance()
{
	return _instance;
}

void Engine::release()
{
	if (_instance) {
		delete _instance->_platformModule;
		delete _instance->_audioModule;
		delete _instance;
		_instance = nullptr;
	}
}

PlatformModule* Engine::getPlatform()
{
	return _instance->_platformModule;
}

void Engine::setDeltaTime(uint64_t const deltaTime)
{
}

const uint64_t Engine::DeltaTime()
{
	return _instance->_deltaTime;
}

const uint64_t Engine::getSecSinceStart() const
{
	return _platformModule->getSecSinceStart();
}

bool Engine::_initPriv()
{

	_platformModule = new PlatformModule();

	if (!_platformModule->Init())
	{
		delete _platformModule;
		return false;
	}

	_audioModule = new AudioModule();

	if (!_audioModule->Init())
	{
		delete _audioModule;
		return false;
	}
	return true;
}
