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
#include "GameLoader.h"

#include "InputFacade.h"
#include <iostream>
#include <checkMLNew.h>

#include "ScriptsManager.h"

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
		_instance->_platformModule = nullptr;
		delete _instance->_input;
		_instance->_input = nullptr;

		delete _instance->_audioModule;
		_instance->_audioModule = nullptr;
		delete _instance->_physicsModule;
		_instance->_physicsModule = nullptr;
		try {
			delete _instance->_renderModule;
			_instance->_renderModule = nullptr;
		}
		catch (exception e)
		{
			Debug::error(e.what());
		}
		delete _instance->_resourcesModule;
		_instance->_resourcesModule = nullptr;
		delete _instance->_stateMachine;
		_instance->_stateMachine = nullptr;
		// desca
		ComponentDLLLoader::instance().unLoadAll();
		delete _instance;
		_instance = nullptr;
	}
	// Cierra archivo .log
	Debug::close();
}

void Engine::startLoop() const
{
	if (!_stateMachine) return;
	// Bucle de juego
	_stateMachine->requestSceneChange(core::GameConfigurator::instance()._firstScene); // carga la primera escena
	if (_stateMachine->getCurrentScnPtr() != nullptr)
		_stateMachine->gameLoop();
}

bool Engine::pollEvents() const
{
	return _platformModule->syncronize();
}

void Engine::requestSceneChange(std::string const& n) const
{
	_stateMachine->requestSceneChange(n);
}

void Engine::quitGame() const
{
	_stateMachine->endGame();
}

core::Entity* Engine::instantiatePrefab(std::string const& pref) const
{
	// Leer la escena prefab y parsear a lista de entidades
	return GameLoader::loadPrefab(core::GameConfigurator::instance()._root + pref);
}

std::shared_ptr<core::Scene> Engine::getScene() const
{
	return _stateMachine->getCurrentScnPtr();
}

void Engine::renderFrame()
{
	_renderModule->renderFrame();
}

void Engine::cleanScene()
{
	_physicsModule->ReloadPhysics();
	_renderModule->cleanScene(false);
}

void Engine::setViewportBGColor(core::Color color)
{
	_renderModule->setViewportBGColor(color);
}

transformID Engine::addTransform(const entityID& entityID, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale)
{
	return _renderModule->addNode(entityID, pos, rot, scale, true);
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

UITransformID Engine::addUITransform(const entityID& entityID, const core::Vector2<float>& pos,const int& zBuffer, const core::Vector2<float>& dimension, const float& rotation)
{
	return _renderModule->addUITransform(entityID, pos,zBuffer, dimension, rotation);
}

void Engine::setUITransformDimension(const UITransformID& id, const core::Vector2<float>& dim)
{
	return _renderModule->setUITransformDimension(id, dim);
}

void Engine::setUITransformPos(const UITransformID& id, const core::Vector2<float>& pos)
{
	return _renderModule->setUITransformPos(id, pos);
}

void Engine::setUITransformRotation(const UITransformID& id, const float& r)
{
	return _renderModule->setUITransformRotation(id, r);
}

void Engine::setUITransformZBuffer(const UITransformID& id, const int& zBuff)
{
	return _renderModule->setUITransformZBuffer(id, zBuff);
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

modelID Engine::addModel(const entityID& entityID, const std::string& modelName)
{
	auto model = _resourcesModule->getAssetSourceFolder(modelName);
	return _renderModule->addModel(entityID, model.second,  model.first);
}

void Engine::deleteModel(const modelID& id)
{
	_renderModule->deleteModel(id);
}

void Engine::setSubmeshDiffuse(const modelID& id, const std::string& textureName, const int& submesh)
{
	auto texture = getAssetSourceFolder(textureName);
	_renderModule->setDiffuse(id, submesh, texture.second, texture.first);
}

void Engine::setSubmeshTint(const modelID& id, const core::Color& tint, const int& submesh)
{
	_renderModule->setTint(id, submesh, tint);
}

void Engine::setModelVisible(const modelID& id, const bool& visible)
{
	_renderModule->setModelVisible(id, visible);
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

void Engine::setAnimSpeed(const animationID& animationID, const float& speed)
{
	_renderModule->setAnimSpeed(animationID, speed);
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

void Engine::setAmbientLight(const core::Color& color)
{
	_renderModule->setAmbientLight(color);
}

particleGenID Engine::addParticleGen(const entityID& entityID, const std::string& textureName)
{
	auto particle = getAssetSourceFolder(textureName);
	return _renderModule->addParticleGen(entityID, particle.second, particle.first);
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

void Engine::setSkydome(const std::string& textureName, const float& curvature, const float& tiling, const float& distance, const bool& drawFirst)
{
	auto skydome = getAssetSourceFolder(textureName);
	_renderModule->setSkydome(skydome.second, textureName, curvature, tiling, distance, drawFirst);
}

void Engine::setSkydomeNull()
{
	_renderModule->setSkydomeNull();
}

uiPanelID Engine::addUIPanel(const entityID& entityID, const std::string& title)
{
	return _renderModule->addUIPanel(entityID, title);
}

void Engine::setUIPanelVisible(const uiPanelID& id, bool visible)
{
	_renderModule->setUIPanelVisible(id, visible);
}
void Engine::deleteUIPanel(const uiPanelID& id)
{
	_renderModule->deleteUIPanel(id);
}
uiLabelID  Engine::addUILabel(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const  float opacity,  const core::Color textColor, const core::Color bgColor, const float fontSize, const TextAlign textAlign, const std::string fontName) {
	
	return _renderModule->addUILabel(panelID, entityID, text, opacity, textColor, bgColor, fontSize, textAlign, fontName);
}
void Engine::deleteUILabel(const uiLabelID& id)
{
	_renderModule->deleteUILabel(id);
}
void  Engine::setUILabelText(const uiLabelID& uiLabelID, const std::string& text) {
	_renderModule->setUILabelText(uiLabelID, text);
}
void  Engine::setUILabelVisible(const uiLabelID& uiLabelID, bool visible) {
	_renderModule->setUILabelVisible(uiLabelID, visible);
}
void Engine::setUILabelOpacity(const uiLabelID& labelID, float opacity) {
	_renderModule->setUILabelOpacity(labelID, opacity);

}
void  Engine::setUILabelTextColor(const uiLabelID& labelID, core::Color color) {
	_renderModule->setUILabelTextColor(labelID, color);
}
void  Engine::setUILabelBackGroundColor(const uiLabelID& labelID, core::Color color) {
	_renderModule->setUILabelBackGroundColor(labelID, color);
}
void  Engine::setUILabelAlign(const uiLabelID& labelID, const TextAlign& align) {
	_renderModule->setUILabelAlign(labelID, align);
}

uiButtonID  Engine::addUIButton(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const float& fontSize, const std::string& fontName, const core::Color& bgColor, const core::Color& txColor, const core::Color& hvColor, const core::Color& psColor,const float& opacity) {
	return _renderModule->addUIButton(panelID, entityID, text, fontSize, fontName, bgColor,txColor, hvColor, psColor,opacity);

}

void Engine::deleteUIButton(const uiButtonID& id)
{
	_renderModule->deleteUIButton(id);

}

uiButtonID Engine::addUIImageButton(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const std::string& textureName, const core::Color& bgColor, const core::Color& hvColor, const core::Color& psColor, const float& opacity) {

	auto texture = getAssetSourceFolder(textureName);

	return _renderModule->addUIImageButton(panelID,entityID, text, texture.second, texture.first,bgColor, hvColor, psColor, opacity);
}
void Engine::setUIButtonText(const uiButtonID& buttonID, const std::string& text) {
	_renderModule->setUIButtonText(buttonID, text);
}
void Engine::setUIButtonVisible(const uiButtonID& buttonID, bool& visible) {
	_renderModule->setUIButtonVisible(buttonID, visible);
}
void  Engine::setUIButtonTexture(const uiButtonID& buttonID, const std::string& texture) {
	_renderModule->setUIButtonTexture(buttonID, texture);
}
void  Engine::setUIButtonOpacity(const uiButtonID& buttonID, float& opacity) {
	_renderModule->setUIButtonOpacity(buttonID, opacity);
}
void Engine::setUIButtonBackgroundColor(const uiButtonID& buttonID, core::Color& bgColor)
{
	_renderModule->setUIButtonBackgroundColor(buttonID, bgColor);
}
void Engine::setUIButtonTextColor(const uiButtonID& buttonID, core::Color& txColor)
{
	_renderModule->setUIButtonTextColor(buttonID, txColor);
}
void Engine::setUIButtonHoverColor(const uiButtonID& buttonID, core::Color& hvColor)
{
	_renderModule->setUIButtonHoverColor(buttonID, hvColor);
}
void Engine::setUIButtonPressColor(const uiButtonID& buttonID, core::Color& psColor)
{
	_renderModule->setUIButtonPressColor(buttonID, psColor);
}
void Engine::setUIButtonDisable(const uiButtonID& buttonID, bool disable) {
	_renderModule->setUIButtonDisable(buttonID, disable);
}
void Engine::setUIButtonCallback(const uiButtonID& id, std::function<void()> callback) {
	_renderModule->setUIButtonCallback(id, callback);
}
uiTextureRectID Engine::addUITextureRect(const uiPanelID& panelID, const entityID& entityID, const std::string& textureName) {
	auto texture = getAssetSourceFolder(textureName);
	return _renderModule->addUITextureRect(panelID, entityID, texture.second, texture.first);
}
void Engine::deleteUITextureRect(const uiTextureRectID& id)
{
	_renderModule->deleteUITextureRect(id);
}
void Engine::setUITextureRectTexture(const uiTextureRectID& textureRectID, const std::string& texture) {
	_renderModule->setUITextureRectTexture(textureRectID, texture);
}
void Engine::setUITextureRectVisible(const uiTextureRectID& textureRectID, bool& visible) {
	_renderModule->setUITextureRectVisible(textureRectID, visible);
}
void Engine::setUITextureRectOpacity(const uiTextureRectID& textureRectID, float& opacity) {
	_renderModule->setUITextureRectOpacity(textureRectID, opacity);

}


bool Engine::loadSound(std::string path, std::string id, bool soundStream, bool soundLooping, bool sound3D)
{
	return _audioModule->loadSound(path, id, soundStream, soundLooping, sound3D);
}
bool Engine::unloadSound(std::string id)
{
	return _audioModule->unloadSound(id);
}
int Engine::playSound(std::string id, float soundVolume, int looping, const core::Vector3<> vec3, const core::Vector3<> vel3)
{
	return _audioModule->playSound(id, soundVolume, looping, vec3, vel3);
}
bool Engine::setChannelVolume(int chID, float newVolume)
{
	return _audioModule->setChannelVolume(chID, newVolume);
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
bool Engine::setSourcePosition(int chID, core::Vector3<> pos, core::Vector3<> vel)
{
	return _audioModule->setAudioPos(chID, pos, vel);
}
bool Engine::setMinMaxRadius(int chID, float min, float max)
{
	return _audioModule->setMinMaxRadius(chID, min, max);
}

bool Engine::setDelay(int chID, double start, double end, bool stopChannel)
{
	return _audioModule->setDelay(chID, start, end, stopChannel);
}

bool Engine::isChannelPlaying(int chID)
{
	return _audioModule->isChannelPlaying(chID);
}

void Engine::registerActorEntity(ComponentID physicsID, core::Entity* entity)
{
	_physicsModule->setActorEntity(physicsID, entity);
}

bool Engine::setLooping(int chID, int typeOfLooping)
{
	return _audioModule->setLooping(chID, typeOfLooping);
}

float Engine::getVolume(int chID)
{
	float volume;
	_audioModule->getVolume(chID, volume);
	return volume;
}

#pragma region Physics

uint32_t Engine::createBoxCollider(const core::Vector3<>& size, const core::Vector3<>& center, const core::Vector3<>& pos, const core::Quaternion<> rotGlob, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger)
{
	return _physicsModule->CreateBoxShape(size, center, pos, rotGlob, rotationLoc, isDynamic, isTrigger);
}

void Engine::setPhysicsPosition(uint32_t id, const core::Vector3<>& pos)
{
	_physicsModule->SetPhysicsPosition(id, pos);
}

void Engine::setPhysicsRotation(uint32_t id, const core::Quaternion<>& rot)
{
	_physicsModule->SetPhysicsRotation(id, rot);
}

core::Vector3<> Engine::getPhysicsPosition(uint32_t id)
{
	return _physicsModule->GetPhysicsPosition(id);
}

core::Quaternion<> Engine::getPhysicsRotation(uint32_t id)
{
	return _physicsModule->GetPhysicsRotation(id);
}

uint32_t Engine::createCapsuleCollider(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, const core::Quaternion<> rotGlob, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger)
{
	if (!_physicsModule) return 0;
	return _physicsModule->CreateCapsuleShape(radius, height, center, worldPos, rotGlob, rotationLoc, isDynamic, isTrigger);
}

std::vector<PhysicsEvent> Engine::getPhysicsEvents(ComponentID id)
{
	return _physicsModule->getEventsFor(id);
}

void Engine::clearPhysicsEvents()
{
	_physicsModule->clearEvents();
}
///
ComponentID Engine::attachBoxShapeToRigidBody(ComponentID bodyID, const core::Vector3<> size, const core::Vector3<>& center, const core::Quaternion<> rotation, bool isTrigger)
{
	if (!_physicsModule) return 0;
	_physicsModule->AttachBoxShape(bodyID, size, center, rotation, isTrigger);
	return bodyID; //devuelve el ID del RigidBody al que se unio
}

ComponentID Engine::attachCapsuleShapeToRigidBody(ComponentID bodyID, float radius, float height, const core::Vector3<>& center, const core::Quaternion<> rotation, bool isTrigger)
{
	if (!_physicsModule) return 0;
	_physicsModule->AttachCapsuleShape(bodyID, radius, height, center, rotation, isTrigger);
	return bodyID;
}

void Engine::setPhysicsTransform(ComponentID id, const core::Vector3<>& pos, const core::Quaternion<>& rot)
{
	if (!_physicsModule) return;

	_physicsModule->setPhysicsTransform(id, pos, rot);
}

std::vector<PhysicsEvent> Engine::consumeEvents(ComponentID id)
{
	return _physicsModule->consumeEventsFor(id);
}

///
uint32_t Engine::createRigidBody(core::Vector3<> pos, float mass, bool useGravity, bool isKinematic)
{
	return _physicsModule->CreateRigidBody(pos, mass, useGravity, isKinematic);
}

core::Vector3<> Engine::getLinearVelocity(uint32_t id)
{
	return _physicsModule->GetLinearVelocity(id);
}

void Engine::setLinearVelocity(uint32_t id, core::Vector3<> vel)
{
	_physicsModule->SetLinearVelocity(id, vel);
}

void Engine::setMass(uint32_t id, float mass)
{
	_physicsModule->SetMass(id, mass);
}

float Engine::getMass(uint32_t id)
{
	return _physicsModule->GetMass(id);
}

void Engine::setLinearDamping(uint32_t id, float damping)
{
	_physicsModule->SetLinearDamping(id, damping);
}

float Engine::getLinearDamping(uint32_t id)
{
	return _physicsModule->GetLinearDamping(id);
}


void Engine::addForce(uint32_t id, core::Vector3<> force, char mode)
{
	_physicsModule->AddForce(id, force, mode);
}

void Engine::clearForce(uint32_t id, char mode)
{
	_physicsModule->ClearForce(id, mode);
}

void Engine::blockAxes(uint32_t id, bool x, bool y, bool z)
{
	_physicsModule->BlockAxes(id, x, y, z);
}

void Engine::blockAngles(uint32_t id, bool x, bool y, bool z)
{
	_physicsModule->BlockAngles(id, x, y, z);
}

uint32_t Engine::createMaterial(ComponentID id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine)
{
	return _physicsModule->CreateMaterial(id, staticF, dynamicF, restitution, frictionCombine, bounceCombine);
}

void Engine::updateMaterial(uint32_t id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine)
{
	_physicsModule->UpdateMaterial(id, staticF, dynamicF, restitution, frictionCombine, bounceCombine);
}

void Engine::destroyMaterial(uint32_t id)
{
	if (_physicsModule == nullptr) return;
	_physicsModule->DestroyMaterial(id);
}

bool Engine::rayCast(const core::Vector3<>& origin,
	const core::Vector3<>& direction,
	float maxDistance,
	RayInfo& rayInfo) const
{
	return _physicsModule->rayCast({ origin.getX(), origin.getY(), origin.getZ() },
		{ direction.getX(), direction.getY(), direction.getZ() },
		maxDistance, rayInfo);
}
std::vector<ShapeRenderData> Engine::GetPhysicsRenderData()
{
	if (!_physicsModule) return {};
	return _physicsModule->GetRenderData();
}

void Engine::SetGravity(const core::Vector3<>& gravity) const
{
	_physicsModule->SetGravity(gravity);
}

void Engine::setGizmos(bool gizmos)
{
	_gizmos = gizmos;
}
void Engine::deletePhysicsComponent(ComponentID id)
{
	_physicsModule->DestroyBody(id);
}
void Engine::deletePhysicsMaterial(ComponentID id)
{
	_physicsModule->DestroyMaterial(id);
}
void Engine::setActorEnabled(ComponentID id, bool enabled, bool isTrigger)
{
	_physicsModule->SetActorEnabled(id, enabled, isTrigger);
}
std::pair<std::string, std::string> Engine::getAssetSourceFolder(std::string assetName) 
{
	return _resourcesModule->getAssetSourceFolder(assetName); 
}
std::vector<std::pair<std::string, std::string>> Engine::getAllAssets()
{
	return _resourcesModule->getAllAssets();
}
#pragma endregion

//------Metodo de PlatformModule:
int Engine::getWindowWidth() const
{
	return _platformModule->getWindowWidth();
}

int Engine::getWindowHeight() const
{
	return _platformModule->getWindowHeight();
}

InputFacade* Engine::input()
{
	return instance()->_input;
}


bool Engine::_initPriv()
{
	// Abre archivo .log
	Debug::open();

	//cargamos dlls
	if (!ComponentDLLLoader::instance().loadAll(DLLs_PATH))
		return false;
#if _DEBUG
	std::string basecompPath = "./ComponentsProject_d.dll";
#else 
	std::string basecompPath = "./ComponentsProject_r.dll";
#endif
	if (!ComponentDLLLoader::instance().load(basecompPath))
		return false;

	//Platform
	_platformModule = new PlatformModule();
	if (!_platformModule->Init()) {
		delete _platformModule;
		_platformModule = nullptr;
		return false;
	}
	//Resources
	_resourcesModule = new ResourcesModule();
	if (!_resourcesModule->Init()) {
		delete _resourcesModule;
		_resourcesModule = nullptr;
		return false;
	}

	_input = new InputFacade(_platformModule);
	//Render
	_renderModule = new RenderModule();
	if (!_renderModule->Init(_platformModule->getSDLWindow(),_platformModule->getWindowHandle(), _platformModule->getWindowWidth(), _platformModule->getWindowHeight(), _resourcesModule->getAllFonts())) {
		delete _renderModule;
		_renderModule = nullptr;
		return false;
	}
	_platformModule->registerEventObserver(_renderModule->getImguiInputCallback());
	//Audio
	_audioModule = new AudioModule();
	if (!_audioModule->init()) {
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

	_stateMachine = new StateMachine();
	// manager de scripts
	ScriptsManager::instance().init();

	//
	//#if _DEBUG
	//	ComponentDLLLoader::instance().load("./game/DLL-Test.dll");
	//#else
	//	std::string path = "./game/" + core::GameConfigurator::instance()._gameDLL + ".dll";
	//	ComponentDLLLoader::instance().load(path);
	//#endif


	return true;
}

void Engine::update(float dt)
{
	if (_physicsModule)
	{
		_physicsModule->Update(dt);
		auto physicsShapes = _physicsModule->GetRenderData();

		if ((_renderModule != nullptr) && _gizmos)
			_renderModule->RenderPhysics(physicsShapes);//debbug colliders
	}
	if (_audioModule)
	{
		_audioModule->update();
	}
}

