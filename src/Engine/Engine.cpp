#include "Engine.h"

#include <memory>

#include "PlatformModule.h"
#include "RenderModule.h"
#include "AudioModule.h"
#include "ComponentDLLLoader.h"
#include "PhysicsModule.h"
#include "InputMapper.h"
#include "StateMachine.h"

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
	// Cierra archivo .log
	Debug::close();
	if (_instance) {
		delete _instance->_platformModule;
		delete _instance->_audioModule;
		delete _instance->_physicsModule;
		delete _instance->_renderModule;
		delete _instance->_componentDLLLoader;
		delete _instance->_stateMachine;
		delete _instance;
		_instance = nullptr;
	}
	// Cierra archivo .log
	Debug::close();
}

void Engine::startLoop()
{
	// Bucle de juego
	_stateMachine->addAndSetScene("scene1");
	_stateMachine->gameLoop();
}

bool Engine::syncronize() const
{
	return _platformModule->syncronize();
}

const void Engine::addAndSetScene(std::string n) const
{
	_addAndSetScene(n);
}

const void Engine::setAddAndSetScene(std::function<void(std::string)> func) {
	_addAndSetScene = func;
}

void Engine::renderFrame()
{
	_renderModule->renderFrame();
}

void Engine::cleanScene()
{
	_renderModule->cleanScene();
}

void Engine::setViewportBGColor(core::Color color)
{
	_renderModule->setViewportBGColor(color);
}

transformID Engine::addTransform(const entityID& entityID, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale)
{
	return _renderModule->addNode(entityID, pos, rot, scale);
}

void Engine::setTransformPosition(const transformID& id, const core::Vector3<float>& pos)
{
	_renderModule->setNodePosition(id, pos);
}

void Engine::setTransformRotation(const transformID& id, const core::Quaternion<float>& rot)
{
	_renderModule->setNodeRotation(id, rot);
}

void Engine::setTransformScale(const transformID& id, const core::Vector3<float>& scale)
{
	_renderModule->setNodeScale(id, scale);
}

cameraID Engine::addCamera(const entityID& entityID, const float& FOVy, const float& nearClipDistance, const float& farClipDistance, const float& focalLength, const core::Color& bgColor)
{
	return _renderModule->addCamera(entityID, FOVy, nearClipDistance, farClipDistance, focalLength, bgColor);
}

void Engine::deleteCamera(const cameraID& id)
{
	_renderModule->deleteCamera(id);
}

void Engine::setAsActiveCamera(const cameraID& id)
{
	_renderModule->setAsActiveCamera(id);
}

void Engine::setCameraFOVy(const cameraID& id, const float& FOVy)
{
	_renderModule->setCameraFOVy(id, FOVy);
}

void Engine::setCameraNearClipDistance(const cameraID& id, const float& nearClipDistance)
{
	_renderModule->setCameraNearClipDistance(id, nearClipDistance);
}

void Engine::setCameraFarClipDistance(const cameraID& id, const float& farClipDistance)
{
	_renderModule->setCameraFarClipDistance(id, farClipDistance);
}

void Engine::setCameraFocalLength(const cameraID& id, const float& focalLength)
{
	_renderModule->setCameraFocalLength(id, focalLength);
}

lightID Engine::addLight(const entityID& entityID, int type, const core::Color& color, float intensity)
{
	return _renderModule->addLight(entityID, type,color, intensity);
}

void Engine::deleteLight(const lightID& id)
{
	_renderModule->deleteLight(id);
}

void Engine::setLightActive(const lightID& id, bool active)
{
	_renderModule->setLightActive(id, active);
}

void Engine::cleanLights()
{
	_renderModule->cleanLights();
}

void Engine::setLightType(const lightID& id, int type)
{
	_renderModule->setLightType(id, type);
}

void Engine::setLightColor(const lightID& id, const core::Color& color)
{
	_renderModule->setLightColor(id, color);
}

void Engine::setLightIntensity(const lightID& id, float intensity)
{
	_renderModule->setLightIntensity(id, intensity);
}

void Engine::setLightDirection(const lightID& id, const core::Vector3<float>& dir)
{
	_renderModule->setLightDirection(id, dir);
}

void Engine::setLightSpotRange(const lightID& id, float inner, float outer, float falloff)
{
	_renderModule->setLightSpotRange(id, inner, outer, falloff);
}

void Engine::loadSound(std::string path, std::string id, bool soundStream, bool soundLooping, bool sound3D)
{
	_audioModule->loadSound(path, id, soundStream, soundLooping, sound3D );
}

void Engine::unloadSound(std::string id)
{
	_audioModule->unloadSound(id);
}
int Engine::playSound(std::string id, float soundVolume, int looping, const core::Vector3<> vec3, const core::Vector3<> vel3)
{
	return _audioModule->playSound(id,soundVolume, looping, vec3, vel3);
}
void Engine::setChannelVolume(int chID, float newVolume)
{
	_audioModule->setChannelVolume(chID, newVolume);
}

int Engine::getLooping(int chID) const
{
	int looping = 0;
	_audioModule->getLooping(chID, &looping);
	return looping;
}
void Engine::setListener(core::Vector3<> pos, core::Vector3<> forward, core::Vector3<> up, core::Vector3<> vel)
{
	_audioModule->setListener(pos, forward, up, vel);
}
bool Engine::stopPlaying(int chID)
{
	return _audioModule->stopPlaying(chID);
}

bool Engine::pauseChannel(int chID, bool pause)
{
	return _audioModule->pauseChannel(chID, pause);
}

void Engine::setSourcePosition(int chID, core::Vector3<> pos, core::Vector3<> vel)
{
	_audioModule->setAudioPos(chID, pos, vel);
}

void Engine::setDelay(int chID, unsigned long long start, unsigned long long end, bool stopChannel)
{
	_audioModule->setDelay(chID, start, end, stopChannel);
}

bool Engine::isChannelPlaying(int chID)
{
	return _audioModule->isChannelPlaying(chID);
}

void Engine::setLooping(int chID, int typeOfLooping)
{
	_audioModule->setLooping(chID, typeOfLooping);
}

float Engine::getVolume(int chID)
{
	float volume;
	_audioModule->getVolume(chID, volume);
	return volume;
}

#pragma region Platform

//------Metodo de PlatformModule:
int Engine::getWindowWidth() const
{
	return _platformModule->getWindowWidth();
}

int Engine::getWindowHeight() const
{
	return _platformModule->getWindowHeight();
}

bool Engine::isKeyPressed(input::InputEvent inputAction, input::DeviceID device) const
{
	return _platformModule->isKeyPressed(inputAction, device);
}

bool Engine::isKeyReleased(input::InputEvent inputAction, input::DeviceID device) const
{
	return _platformModule->isKeyReleased(inputAction, device);
}

float Engine::getAxis(input::InputEvent inputAction, input::DeviceID device) const
{
	return _platformModule->getAxis(inputAction, device);
}

bool Engine::isActionPressed(const std::string& actionName, input::DeviceID device) const
{
	return _platformModule->isActionPressed(actionName, device);
}

bool Engine::isActionReleased(const std::string& actionName, input::DeviceID device) const
{
	return _platformModule->isActionReleased(actionName, device);
}

void Engine::startTextInput() const
{
	_platformModule->startTextInput();
}

void Engine::stopTextInput() const
{
	_platformModule->stopTextInput();
}

std::string Engine::getTextInput(input::DeviceID device) const
{
	return _platformModule->getTextInput(device);
}

//------Metodos de InputMapper:
void Engine::addEvent(const std::string& actionName, input::InputEvent inputEvent, input::DeviceID id)
{
	_platformModule->getInputMapper()->addEvent(actionName, inputEvent, id);
}

void Engine::removeEvent(const std::string& actionName, input::InputEvent inputEvent, input::DeviceID id)
{
	_platformModule->getInputMapper()->removeEvent(actionName, inputEvent, id);
}

void Engine::removeEvents(const std::string& actionName)
{
	_platformModule->getInputMapper()->removeEvents(actionName);
}

void Engine::removeEventsFromID(const std::string& actionName, input::DeviceID id)
{
	_platformModule->getInputMapper()->removeEventsFromID(actionName, id);
}

std::vector<input::InputEvent> Engine::getInputEvents(const std::string& actionName, input::DeviceID id)
{
	return _platformModule->getInputMapper()->getInputEvents(actionName, id);
}

std::vector<std::string> Engine::getActions()
{
	return _platformModule->getInputMapper()->getActions();
}

bool Engine::hasAction(const std::string& actionName) const
{
	return _platformModule->getInputMapper()->hasAction(actionName);
}

#pragma endregion

bool Engine::_initPriv()
{
	// Abre archivo .log
	Debug::open();
	//Platform
	_platformModule = new PlatformModule();
	if (!_platformModule->Init()) return false;
	//Render
	_renderModule = new RenderModule();
	if (!_renderModule->Init(_platformModule->getWindowHandle(), _platformModule->getWindowWidth(), _platformModule->getWindowHeight()))
		return false;
	//Audio
	_audioModule = new AudioModule();
	if (!_audioModule->Init()) return false;
	//Fisicas
	_physicsModule = new PhysicsModule();
	if (!_physicsModule->Init()) return false;

	// Abre archivo .log
	Debug::open();

	_componentDLLLoader = new ComponentDLLLoader;
#if _DEBUG
	_componentDLLLoader->load("./ComponentsProject_d.dll");
#else 
	_componentDLLLoader.load("./ComponentsProject_r.dll");
#endif
	_componentDLLLoader->load("./game/DLL-Test.dll");

	_stateMachine = new StateMachine;
		
	return true;
}
