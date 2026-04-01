#include "Engine.h"

#include <memory>

#include <PlatformModule.h>
#include <RenderModule.h>
#include <AudioModule.h>
#include <PhysicsModule.h>

#include <InputMapper.h>

#include "ComponentDLLLoader.h"
#include "GameConfigurator.h"
#include "StateMachine.h"
#include "InputFacade.h"

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
		delete _instance->_input;

		delete _instance->_audioModule;
		delete _instance->_physicsModule;
		delete _instance->_renderModule;

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
	//_stateMachine->addAndSetScene(core::GameConfigurator::_firstScene); // carga la primera escena
	_stateMachine->addAndSetScene("scene1"); // carga la primera escena
	_stateMachine->gameLoop();
}

bool Engine::pollEvents() const
{
	return _platformModule->syncronize();
}

const void Engine::addAndSetScene(std::string n) const
{
	_stateMachine->addAndSetScene(n);
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

void Engine::addModel(const entityID& entityID, const std::string& modelFolder, const std::string& modelFile)
{
	_renderModule->addModel(entityID, modelFolder, modelFile);
}

void Engine::deleteModel(const modelID& id)
{
	_renderModule->deleteModel(id);
}

void Engine::setSubmeshDiffuse(const modelID& id, const std::string& textureFolder, const std::string& textureFile, const int& submesh)
{
	_renderModule->setDiffuse(id, submesh, textureFolder, textureFile);
}

void Engine::setSubmeshTint(const modelID& id, const core::Color& tint, const int& submesh)
{
	_renderModule->setTint(id, submesh, tint);
}

void Engine::setModelVisible(const modelID& id, const bool& visible)
{
	_renderModule->setModelVisible(id, visible);
}

lightID Engine::addLight(const entityID& entityID, const int& type, const core::Color& color, const float& intensity)
{
	return _renderModule->addLight(entityID, type,color, intensity);
}

void Engine::deleteLight(const lightID& id)
{
	_renderModule->deleteLight(id);
}

void Engine::setLightActive(const lightID& id, const bool& active)
{
	_renderModule->setLightActive(id, active);
}

void Engine::setLightType(const lightID& id, const int& type)
{
	_renderModule->setLightType(id, type);
}

void Engine::setLightColor(const lightID& id, const core::Color& color)
{
	_renderModule->setLightColor(id, color);
}

void Engine::setLightIntensity(const lightID& id, const float& intensity)
{
	_renderModule->setLightIntensity(id, intensity);
}

void Engine::setLightSpotRange(const lightID& id, const float& inner, const float& outer, const float& falloff)
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

void Engine::setDelay(int chID, double start, double end, bool stopChannel)
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


//------Metodo de PlatformModule:
int Engine::getWindowWidth() const
{
	return _platformModule->getWindowWidth();
}

int Engine::getWindowHeight() const
{
	return _platformModule->getWindowHeight();
}

InputFacade* Engine::input() const
{
	return _input;
}


bool Engine::_initPriv()
{
	// Abre archivo .log
	Debug::open();
	//Platform
	_platformModule = new PlatformModule();
	if (!_platformModule->Init()) return false;
	_input = new InputFacade(_platformModule);
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

#if _DEBUG
	ComponentDLLLoader::instance().load("./ComponentsProject_d.dll");
#else 
	ComponentDLLLoader::instance().load("./ComponentsProject_r.dll");
#endif
	ComponentDLLLoader::instance().load("./game/DLL-Test.dll");		
	
	_stateMachine = new StateMachine;
	
	return true;
}
