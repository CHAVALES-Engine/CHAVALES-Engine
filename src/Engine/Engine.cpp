
#include "Engine.h"

#include "PlatformModule.h"
#include "RenderModule.h"
#include "AudioModule.h"
#include "PhysicsModule.h"

Engine* Engine::_instance = nullptr;

bool Engine::init()
{
	// utilizamos este tipo de inicializacion para tener mas control
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
		delete _instance->_physicsModule;
		delete _instance;
		_instance = nullptr;
	}
}

PlatformModule* Engine::getPlatform()
{
	return _instance->_platformModule;
}

const bool Engine::syncronize()
{
	return _platformModule->syncronize();
}

bool Engine::_initPriv()
{

	_platformModule = new PlatformModule();

	//si falla el ultimo el resto se crean pero no se limpian nunca???

	if (!_platformModule->Init())
	{
		delete _platformModule;
		return false;
	}

	_renderModule = new RenderModule();

	if (!_renderModule->Init(_platformModule->getWindowHandle(), _platformModule->getWindowWidth(), _platformModule->getWindowHeight()))
	{
		delete _renderModule;
		return false;
	}

	_audioModule = new AudioModule();

	if (!_audioModule->Init())
	{
		delete _audioModule;
		return false;
	}

	_physicsModule = new PhysicsModule();

	if (!_physicsModule->Init())
	{
		delete _physicsModule;
		return false;
	}
	return true;
}
