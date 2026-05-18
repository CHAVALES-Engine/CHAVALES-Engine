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
#include <checkMLNew.h>

#include "MessagesManager.h"
#include "ScriptsManager.h"
#include "TimeManager.h"

Engine* Engine::_instance = nullptr;

bool Engine::init()
{
	assert(!_instance);
	// utilizamos este tipo de inicializacion para tener mas control
	if (_instance == nullptr) _instance = new Engine();
	return _instance->_initPriv();
}

Engine* Engine::instance()
{
	assert(_instance);
	return _instance;
}

void Engine::release()
{
	assert(_instance);
	// facades
	delete _instance->_input;
	_instance->_input = nullptr;
	delete _instance->_platformModule;
	_instance->_platformModule = nullptr;
	delete _instance->_audioModule;
	_instance->_audioModule = nullptr;
	delete _instance->_physicsModule;
	_instance->_physicsModule = nullptr;
	try {
		delete _instance->_renderModule;
		_instance->_renderModule = nullptr;
	}
	catch (std::exception e)
	{
		Debug::error(e.what());
	}
	delete _instance->_resourcesModule;
	_instance->_resourcesModule = nullptr;
	delete _instance->_stateMachine;
	_instance->_stateMachine = nullptr;
	// Descarga dlls
	ComponentDLLLoader::instance().unLoadAll();
	// Cierra sistemas core del motor
	core::MessagesManager::instance().shutdown();
	delete _instance;
	_instance = nullptr;

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
	_audioModule->stopEverything();
}

void Engine::setViewportBGColor(const core::Color& color)
{
	_renderModule->setViewportBGColor(color);
}

bool Engine::rayCast(const core::Vector3<>& origin, const core::Vector3<>& direction, float maxDistance,
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

std::string Engine::getAssetSourceFolder(const std::string& assetName) const
{
	return _resourcesModule->getAssetPath(assetName);
}

bool Engine::preload(const std::string& path)
{
	return _resourcesModule->preload(path);
}

bool Engine::preloadAll()
{
	return _resourcesModule->preloadAllAssets();
}
#pragma endregion

int Engine::getWindowWidth() const
{
	return _platformModule->getWindowWidth();
}

int Engine::getWindowHeight() const
{
	return _platformModule->getWindowHeight();
}

bool Engine::_initPriv()
{
	// Abre archivo .log
	Debug::open();

	// Carga de dlls
	if (!ComponentDLLLoader::instance().loadAll(DLLs_PATH))
		return false;
#if _DEBUG
	std::string basecompPath = "./ComponentsProject_d.dll";
#else 
	std::string basecompPath = "./ComponentsProject_r.dll";
#endif
	if (!ComponentDLLLoader::instance().load(basecompPath))
		return false;
	// Resources
	_resourcesModule = new ResourcesModule();
	if (!_resourcesModule->Init()) {
		delete _resourcesModule;
		_resourcesModule = nullptr;
		return false;
	}
	// Platform
	_platformModule = new PlatformModule();
	if (!_platformModule->Init()) {
		delete _platformModule;
		_platformModule = nullptr;
		return false;
	}
	// Render
	_renderModule = new RenderModule();
	if (!_renderModule->Init(_platformModule->getSDLWindow(), _platformModule->getWindowHandle(), _platformModule->getWindowWidth(), _platformModule->getWindowHeight(), _resourcesModule->getAllFonts())) {
		delete _renderModule;
		_renderModule = nullptr;
		return false;
	}
	_platformModule->registerEventObserver(_renderModule->getImguiInputCallback());
	// Audio
	_audioModule = new AudioModule();
	if (!_audioModule->init()) {
		delete _audioModule;
		_audioModule = nullptr;
		return false;
	}
	// Fisicas
	_physicsModule = new PhysicsModule();
	if (!_physicsModule->Init()) {
		delete _physicsModule;
		_physicsModule = nullptr;
		return false;
	}
	// Precarga de recursos
	_resourcesModule->addFactory(core::Resource::Type::MESH,
		[this](const std::string& id, const std::string& path)
		{
			return _renderModule->preloadMesh(id, path);
		});
	ComponentDLLLoader::instance().preloadResources();
	preloadAll();
	// Facades publicas
	_input = new InputFacade(_platformModule);

	_stateMachine = new StateMachine();
	// Manager de scripts
	ScriptsManager::instance().init();

	return true;
}

bool Engine::update(uint64_t dt) const
{
	core::TimerManager::instance().update();
	if (_physicsModule)
	{
		auto physicsShapes = _physicsModule->GetRenderData();

		if ((_renderModule != nullptr) && _gizmos)
			_renderModule->RenderPhysics(physicsShapes);//debug colliders
	}
	if (_audioModule)
	{
		_audioModule->update();
	}
	if (_platformModule)
		return _platformModule->pollEvents();
	return false;
}

void Engine::fixedUpdate(float dt) const
{
	if (_physicsModule)
		_physicsModule->fixedUpdate(dt);
}
