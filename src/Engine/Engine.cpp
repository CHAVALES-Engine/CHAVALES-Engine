
#include "Engine.h"

#include "PlatformModule.h"
#include "RenderModule.h"
#include "AudioModule.h"
#include "PhysicsModule.h"
using namespace std;
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
		delete _instance->_renderModule;
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

const void Engine::addAndSetScene(std::string n) const
{
	_addAndSetScene(n);
}

const void Engine::setAddAndSetScene(std::function<void(std::string)> func){
	_addAndSetScene = func;
}

bool Engine::_initPriv()
{
	//Uso unique_ptr entonces no hace falta delete porque se maneja solo
	
	//Platform
	unique_ptr<PlatformModule> platform(new PlatformModule());
	if (!platform->Init()) return false;
	//Render
	unique_ptr<RenderModule> render(new RenderModule());
	if (!render->Init(platform->getWindowHandle(), platform->getWindowWidth(), platform->getWindowHeight()))
		return false;
	//Audio
	unique_ptr<AudioModule> audio(new AudioModule());
	if (!audio->Init()) return false;
	//Fisicas
	unique_ptr<PhysicsModule> physics(new PhysicsModule());
	if (!physics->Init()) return false;

	//paso propiedades al engine
	_platformModule = platform.release();
	_renderModule = render.release();
	_audioModule = audio.release();
	_physicsModule = physics.release();

	return true;
}
