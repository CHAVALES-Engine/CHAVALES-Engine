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
#include "TimerManager.h"

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
	// recursos
	delete _instance->_resourcesModule;
	_instance->_resourcesModule = nullptr;
	// resto de modulos
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
	catch (const std::exception& e)
	{
		Debug::error(e.what());
	}
	catch (...)
	{
		Debug::error("Error desconocido liberando RenderModule.");
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

void Engine::cleanScene()
{
	if (!_physicsModule || !_renderModule || !_audioModule) return;
	_physicsModule->ReloadPhysics();
	_renderModule->cleanScene(false);
	_audioModule->stopEverything();
}

#pragma region RENDER
bool Engine::renderFrame()
{
	if (!_renderModule) return false;;
	return _renderModule->renderFrame();
}


void Engine::setViewportBGColor(const core::Color& color)
{
	if (_renderModule == nullptr) return;
	_renderModule->setViewportBGColor(color);
}

bool Engine::getViewportRect(int& x, int& y, int& w, int& h) const
{
	if (_renderModule == nullptr) return false;
	return _renderModule->getViewportRectPixels(x, y, w, h);
}

core::Vector2<> Engine::getLogicResolution() const
{
	if (_renderModule == nullptr) return { -1, -1 };
	return _renderModule->getResolution();
}

core::Vector2<> Engine::windowToLogicCoords(const core::Vector2<>& windowPos) const
{
	if (_renderModule == nullptr) return { -1.0f, -1.0f };
	return _renderModule->windowToLogicCoords(windowPos);
}

void Engine::setGizmos(bool gizmos)
{
	_gizmos = gizmos;
}
#pragma endregion

#pragma region PHYSICS
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
#pragma endregion

#pragma region RESOURCES
std::string Engine::getAssetSourceFolder(const std::string& assetName) const
{
	return _resourcesModule->getAssetPath(assetName);
}

bool Engine::preload(const std::string& path)
{
	return _resourcesModule->load(path, true);
}

bool Engine::preloadAll()
{
	return _resourcesModule->preloadAllAssets();
}
#pragma endregion

#pragma region PLATFORM
int Engine::getWindowWidth() const
{
	return _platformModule->getWindowWidth();
}

int Engine::getWindowHeight() const
{
	return _platformModule->getWindowHeight();
}

void Engine::setWindowResizable(bool enabled) const
{
	if (_platformModule == nullptr) return;
	_platformModule->setWindowResizable(enabled);
}

void Engine::setWindowMaximizable(bool enabled) const
{
	if (_platformModule == nullptr) return;
	_platformModule->setWindowMaximizable(enabled);
}

bool Engine::setFullscreen(bool enabled) const
{
	if (_platformModule == nullptr) return false;
	return _platformModule->setFullscreen(enabled);
}

bool Engine::isFullscreen() const
{
	if (_platformModule == nullptr) return false;
	return _platformModule->isFullscreen();
}
#pragma endregion

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
	if (!_renderModule->Init(_platformModule->getSDLWindow(), _platformModule->getWindowHandle(), _platformModule->getWindowWidth(), _platformModule->getWindowHeight())) {
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
		[this](const std::string& id, const std::string& path, bool preload)
		{
			return _renderModule->loadMesh(id, path, preload);
		});
	_resourcesModule->addFactory(core::Resource::Type::TEXTURE,
		[this](const std::string& id, const std::string& path, bool preload)
		{
			return _renderModule->loadTexture(id, path, preload);
		});
	_resourcesModule->addFactory(core::Resource::Type::FONT,
		[this](const std::string& id, const std::string& path, bool preload)
		{
			// imgui precarga automaticamente
			return _renderModule->loadFont(id, path);
		});
	/*_resourcesModule->addFactory(core::Resource::Type::SOUND, TODO
		[this](const std::string& id, const std::string& path, bool preload) {
		});*/
	ComponentDLLLoader::instance().preloadResources();
	// imgui necesita tener precargadas todas las fonts
	_resourcesModule->loadAllOfType(core::Resource::FONT);
	_renderModule->buildFontAtlas();


	// Facades publicas
	_input = new InputFacade(_platformModule);

	_stateMachine = new StateMachine();
	// Manager de scripts
	ScriptsManager::instance().init();
	_registerScriptBindings();

	return true;
}

void Engine::_registerScriptBindings() const
{
	auto& sm = ScriptsManager::instance();

	sm.bindMethodImpl("Engine", "requestSceneChange",
		[](void* o, const std::vector<Property>& a) -> Property {
			static_cast<Engine*>(o)->requestSceneChange(ScriptsManager::instance().getArg<std::string>(a[0]));
			return Property(0);
		});

	sm.bindMethodImpl("Engine", "quitGame",
		[](void* o, const std::vector<Property>&) -> Property {
			static_cast<Engine*>(o)->quitGame();
			return Property(0);
		});

	sm.bindMethodImpl("Engine", "instantiatePrefab",
		[](void* o, const std::vector<Property>& a) -> Property {
			return Property(static_cast<Engine*>(o)->instantiatePrefab(
				ScriptsManager::instance().getArg<std::string>(a[0])));
		});

	sm.bindMethodImpl("Engine", "getScene",
		[](void* o, const std::vector<Property>&) -> Property {
			return Property(static_cast<Engine*>(o)->getScene());
		});

	// ===== render =====
	sm.bindMethodImpl("Engine", "setViewportBGColor",
		[](void* o, const std::vector<Property>& a) -> Property {
			static_cast<Engine*>(o)->setViewportBGColor(ScriptsManager::instance().getArg<core::Color>(a[0]));
			return Property(0);
		});

	// ===== physics =====
	sm.bindMethodImpl("Engine", "rayCast",
		[](void* o, const std::vector<Property>& a) -> Property {
			RayInfo info;
			bool hit = static_cast<Engine*>(o)->rayCast(
				ScriptsManager::instance().getArg<core::Vector3<>>(a[0]),
				ScriptsManager::instance().getArg<core::Vector3<>>(a[1]),
				ScriptsManager::instance().getArg<float>(a[2]),
				info);
			if (!hit) info = RayInfo{};
			return Property(info);
		});

	sm.bindMethodImpl("Engine", "setGravity",
		[](void* o, const std::vector<Property>& a) -> Property {
			static_cast<Engine*>(o)->SetGravity(ScriptsManager::instance().getArg<core::Vector3<>>(a[0]));
			return Property(0);
		});

	sm.bindMethodImpl("Engine", "setGizmos",
		[](void* o, const std::vector<Property>& a) -> Property {
			static_cast<Engine*>(o)->setGizmos(ScriptsManager::instance().getArg<bool>(a[0]));
			return Property(0);
		});

	// ===== resources =====
	sm.bindMethodImpl("Engine", "getAssetSourceFolder",
		[](void* o, const std::vector<Property>& a) -> Property {
			return Property(static_cast<Engine*>(o)->getAssetSourceFolder(
				ScriptsManager::instance().getArg<std::string>(a[0])));
		});

	sm.bindMethodImpl("Engine", "preload",
		[](void* o, const std::vector<Property>& a) -> Property {
			return Property(static_cast<Engine*>(o)->preload(ScriptsManager::instance().getArg<std::string>(a[0])));
		});

	sm.bindMethodImpl("Engine", "preloadAll",
		[](void* o, const std::vector<Property>&) -> Property {
			return Property(static_cast<Engine*>(o)->preloadAll());
		});

	// ===== platform =====
	sm.bindMethodImpl("Engine", "getWindowWidth",
		[](void* o, const std::vector<Property>&) -> Property {
			return Property(static_cast<Engine*>(o)->getWindowWidth());
		});

	sm.bindMethodImpl("Engine", "getWindowHeight",
		[](void* o, const std::vector<Property>&) -> Property {
			return Property(static_cast<Engine*>(o)->getWindowHeight());
		});

	sm.bindMethodImpl("Engine", "setFullscreen",
		[](void* o, const std::vector<Property>& a) -> Property {
			return Property(static_cast<Engine*>(o)->setFullscreen(ScriptsManager::instance().getArg<bool>(a[0])));
		});

	sm.bindMethodImpl("Engine", "isFullscreen",
		[](void* o, const std::vector<Property>&) -> Property {
			return Property(static_cast<Engine*>(o)->isFullscreen());
		});

	sm.bindGlobalImpl("engine", "Engine", const_cast<Engine*>(this));

	// ===== input =====
	_input->_registerScriptBindings();
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
