
#include "Engine.h"

#include <memory>

#include "PlatformModule.h"
#include "RenderModule.h"
#include "AudioModule.h"
#include "PhysicsModule.h"
#include "InputMapper.h"

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

void Engine::loadSound(const char* path, std::string id, bool sound3D, bool soundLooping, bool soundStream)
{
	_audioModule->loadSound(path, id, sound3D, soundLooping, soundStream);
}

void Engine::unloadSound(std::string id)
{
	_audioModule->unloadSound(id);
}
int Engine::playSound(std::string id, const core::Vector3<> vec3, float soundVolume, int looping)
{
	return _audioModule->playSound(id,vec3,soundVolume,looping);
}
void Engine::setChannelVolume(int chID, float newVolume) 
{
	_audioModule->setChannelVolume(chID, newVolume);
}
void Engine::getLooping(int chID, int* typeOfLooping) 
{
	_audioModule->getLooping(chID, typeOfLooping);
}
void Engine::setListener(core::Vector3<> pos, core::Vector3<> forward, core::Vector3<> up, core::Vector3<> vel)
{
	_audioModule->setListener(pos, forward, up, vel);
}
bool Engine::stopPlaying(int chID)
{
	return _audioModule->stopPlaying(chID);
}

void Engine::setSourcePosition(int chID, core::Vector3<> pos, core::Vector3<> vel)
{
	_audioModule->setAudioPos(chID, pos, vel);
}

bool Engine::isChannelPlaying(int chID)
{
	return _audioModule->isChannelPlaying(chID);
}


bool Engine::_initPriv()
{
	//Uso unique_ptr entonces no hace falta delete porque se maneja solo
	
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

	return true;
}
