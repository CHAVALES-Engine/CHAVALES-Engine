
#include "EngineAPI.h"

#include "PlatformModule.h"
#include "RenderModule.h"

EngineAPI* EngineAPI::_instance = nullptr;

bool EngineAPI::init()
{
	if (_instance == nullptr) _instance = new EngineAPI();

	return _instance->_initPriv();
}

EngineAPI* EngineAPI::instance()
{
	return _instance;
}

void EngineAPI::release()
{
	if (_instance) {
		delete _instance->_platformModule;
		delete _instance;
		_instance = nullptr;
	}
}

PlatformModule* EngineAPI::getPlatform()
{
	return _instance->_platformModule;
}

EngineAPI::EngineAPI()
{

}

EngineAPI::~EngineAPI()
{

}

bool EngineAPI::_initPriv()
{

	_platformModule = new PlatformModule();

	if (!_platformModule->Init() || !_renderModule->Init(_platformModule->getWindowHandle(), 1280, 720))
	{
		delete _platformModule;
		return false;
	}
	return true;
}
