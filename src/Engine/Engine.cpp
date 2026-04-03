#include "Engine.h"

#include <memory>

#include <PlatformModule.h>
#include <RenderModule.h>
#include <AudioModule.h>
#include <PhysicsModule.h>
#include <ResourcesModule.h>

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
	if (_instance) {
		delete _instance->_platformModule;
		delete _instance->_input;

		delete _instance->_audioModule;
		delete _instance->_physicsModule;
		try {
			delete _instance->_renderModule;
		}
		catch (exception e)
		{
			Debug::error(e.what());
		}
		delete _instance->_resourecesModule;

		delete _instance->_stateMachine;
		delete _instance;
		_instance = nullptr;
	}
	// Cierra archivo .log
	Debug::close();
}

void Engine::startLoop()
{
	if (!_stateMachine) return;
	// Bucle de juego
	_stateMachine->addAndSetScene(core::GameConfigurator::_firstScene); // carga la primera escena
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

void Engine::addAnimator(const entityID& entityID, modelID& modelID)
{
	_renderModule->addAnimator(entityID, modelID);
}

animationID Engine::registerSkeletonAnim(const modelID& modelID, const std::string& animationName, const bool& loop)
{
	return _renderModule->registerSkeletonAnim(modelID, animationName, loop);
}

animationID Engine::createTransformAnimation(const entityID& entityID, const std::string& animationName, const bool& loop, const float& totalDuration)
{
	return _renderModule->createTransformAnimation(entityID, animationName, loop, totalDuration);
}

void Engine::addTransformKeyFrame(const animationID& animationID, const float& timePos, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale)
{
	_renderModule->addTransformKeyFrame(animationID, timePos, pos, rot, scale);
}

void Engine::addTransformKeyFrame(const animationID& animationID, const float& timePos, const core::Vector3<float>& pos, const float& rot, const int& axis, const core::Vector3<float>& scale)
{
	_renderModule->addTransformKeyFrame(animationID, timePos, pos, rot, axis, scale);
}

void Engine::setAnimEnabled(const animationID& animationID, const bool& active)
{
	_renderModule->setAnimEnabled(animationID, active);
}

void Engine::setAnimTimePos(const animationID& animationID, const float& timePos)
{
	_renderModule->setAnimTimePos(animationID, timePos);
}

void Engine::updateAnimation(const animationID& animationID, const uint64_t& deltaTime)
{
	_renderModule->updateAnimation(animationID, deltaTime);
}

lightID Engine::addLight(const entityID& entityID, const int& type, const core::Color& color, const float& intensity)
{
	return _renderModule->addLight(entityID, type, color, intensity);
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

particleGenID Engine::addParticleGen(const entityID& entityID, const std::string& textureFolder, const std::string& textureFile)
{
	return _renderModule->addParticleGen(entityID, textureFolder, textureFile);
}

void Engine::deleteParticleGen(const particleGenID& id)
{
	_renderModule->deleteParticleGen(id);
}

void Engine::setParticleGenEnabled(const particleGenID& id, const bool& enabled)
{
	_renderModule->setParticleGenEnabled(id, enabled);
}

void Engine::setParticleGenEmitting(const particleGenID& id, const bool& emitting)
{
	_renderModule->setParticleGenEmitting(id, emitting);
}

void Engine::setParticleGenQuota(const particleGenID& id, const float& quota)
{
	_renderModule->setParticleGenQuota(id, quota);
}

void Engine::setParticleGenEmissionRate(const particleGenID& id, const float& rate)
{
	_renderModule->setParticleGenEmissionRate(id, rate);
}

void Engine::setParticleGenDuration(const particleGenID& id, const float& duration)
{
	_renderModule->setParticleGenDuration(id, duration);
}

void Engine::setParticleGenTimeToLive(const particleGenID& id, const float& time)
{
	_renderModule->setParticleGenTimeToLive(id, time);
}

void Engine::setParticleGenVelocity(const particleGenID& id, const float& velocity)
{
	_renderModule->setParticleGenVelocity(id, velocity);
}

void Engine::setParticleGenMinVelocity(const particleGenID& id, const float& velocity)
{
	_renderModule->setParticleGenMinVelocity(id, velocity);
}

void Engine::setParticleGenMaxVelocity(const particleGenID& id, const float& velocity)
{
	_renderModule->setParticleGenMaxVelocity(id, velocity);
}

void Engine::setParticleGenDirection(const particleGenID& id, const core::Vector3<float>& direction)
{
	_renderModule->setParticleGenDirection(id, direction);
}

void Engine::setParticleGenAngle(const particleGenID& id, const float& angle)
{
	_renderModule->setParticleGenAngle(id, angle);
}

void Engine::setParticleGenPartWidth(const particleGenID& id, const float& width)
{
	_renderModule->setParticleGenPartWidth(id, width);
}

void Engine::setParticleGenPartHeight(const particleGenID& id, const float& height)
{
	_renderModule->setParticleGenPartHeight(id, height);
}

void Engine::setParticleGenPartColor(const particleGenID& id, const core::Color& color)
{
	_renderModule->setParticleGenPartColor(id, color);
}

void Engine::loadSound(std::string path, std::string id, bool soundStream, bool soundLooping, bool sound3D)
{
	_audioModule->loadSound(path, id, soundStream, soundLooping, sound3D);
}

void Engine::unloadSound(std::string id)
{
	_audioModule->unloadSound(id);
}
int Engine::playSound(std::string id, float soundVolume, int looping, const core::Vector3<> vec3, const core::Vector3<> vel3)
{
	return _audioModule->playSound(id, soundVolume, looping, vec3, vel3);
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

std::string Engine::getAudioByName(const std::string& name)
{
	return _resourecesModule->getAudio(name);
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
	if (!_platformModule->Init()) {
		delete _platformModule;
		_platformModule = nullptr;
		return false;
	}
	_input = new InputFacade(_platformModule);
	//Render
	_renderModule = new RenderModule();
	if (!_renderModule->Init(_platformModule->getWindowHandle(), _platformModule->getWindowWidth(), _platformModule->getWindowHeight())) {
		delete _renderModule;
		_renderModule = nullptr;
		return false;
	}
	//Audio
	_audioModule = new AudioModule();
	if (!_audioModule->Init()) {
		delete _audioModule;
		_audioModule = nullptr;
		return false;
	}
	//Fisicas
	_physicsModule = new PhysicsModule();
	if (!_physicsModule->Init()) {
		delete _physicsModule;
		_physicsModule = nullptr;
		return false;
	}

	//Resources
	_resourecesModule = new ResourcesModule();
	if (!_resourecesModule->Init()) {
		delete _resourecesModule;
		_resourecesModule = nullptr;
		return false;
	}
	_stateMachine = new StateMachine();

#if _DEBUG
	ComponentDLLLoader::instance().load("./ComponentsProject_d.dll");
#else 
	ComponentDLLLoader::instance().load("./ComponentsProject_r.dll");
#endif

#if _DEBUG
	ComponentDLLLoader::instance().load("./game/DLL-Test.dll");
#else
	std::string path = "./game/" + core::GameConfigurator::_gameDLL + ".dll";
	ComponentDLLLoader::instance().load(path);
#endif


	return true;
}
