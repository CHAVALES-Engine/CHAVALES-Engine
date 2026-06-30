#include "RenderModule.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreGL3PlusRenderSystem.h>
#include <OgreGL3PlusPlugin.h>
#include <OgreMeshManager.h>
#include <OgreTextureManager.h>
#include <OgreAssimpLoader.h>
#include <OgreSTBICodec.h>
#include <OgreStringConverter.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreEntity.h>
#include <OgreKeyFrame.h>
#include <OgreLight.h>
#include <OgreRay.h>
#include <OgreSceneNode.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreTechnique.h>
#include <OgreSubEntity.h>
#include <OgreGpuProgramManager.h>
#include <OgreRTShaderSystem.h>
#include <OgreShaderGenerator.h>
#include <OgreParticleFXPlugin.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreLogManager.h>
#include <iostream>
#include <OgreImGuiOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlaySystem.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <assimp/postprocess.h>
#include <OgreGL3PlusTexture.h>
#include <guid.h>
#include "GameConfigurator.h"
#include <checkMLNew.h>
#include <filesystem>
#include <stdexcept>
#include <Vector2.h>
#include <algorithm>

#include "MeshResource.h"
#include "TextureResource.h"

#include "Debug.h"

static Ogre::Root* _root = nullptr;
static Ogre::GL3PlusPlugin* _gl3Plugin = nullptr;
static Ogre::AssimpPlugin* _assimpPlugin = nullptr;
static Ogre::ParticleFXPlugin* _particlePlugin = nullptr;
static Ogre::OverlaySystem* _overlaySystem = nullptr;
static Ogre::RenderWindow* _window = nullptr;
static Ogre::SceneManager* _sceneMgr = nullptr;
static Ogre::Viewport* _vp = nullptr;
static Ogre::RTShader::ShaderGenerator* _shaderGen;
static Ogre::ResourceGroupManager* _rgm;


static Ogre::STBIImageCodec* _jpgCodec;
static Ogre::STBIImageCodec* _jpegCodec;
static Ogre::STBIImageCodec* _pngCodec;
static Ogre::STBIImageCodec* _tgaCodec;
static Ogre::STBIImageCodec* _bmpCodec;

static Ogre::ManualObject* _debugDraw = nullptr;
static Ogre::SceneNode* _debugNode = nullptr;

RenderModule::~RenderModule()
{
	shutdown();
}

bool RenderModule::Init(SDL_Window* sdlWindow, const HWND handle, const int width, const int height)
{
	try
	{
		_root = new Ogre::Root("", "", "ogre.log");

		_gl3Plugin = new Ogre::GL3PlusPlugin();
		_root->installPlugin(_gl3Plugin);

		_assimpPlugin = new Ogre::AssimpPlugin();
		_root->installPlugin(_assimpPlugin);

		_particlePlugin = new Ogre::ParticleFXPlugin();
		_root->installPlugin(_particlePlugin);

		_jpgCodec = new Ogre::STBIImageCodec("jpg");
		_jpegCodec = new Ogre::STBIImageCodec("jpeg");
		_pngCodec = new Ogre::STBIImageCodec("png");
		_tgaCodec = new Ogre::STBIImageCodec("tga");
		_bmpCodec = new Ogre::STBIImageCodec("bmp");

		Ogre::Codec::registerCodec(_jpgCodec);
		Ogre::Codec::registerCodec(_jpegCodec);
		Ogre::Codec::registerCodec(_pngCodec);
		Ogre::Codec::registerCodec(_tgaCodec);
		Ogre::Codec::registerCodec(_bmpCodec);

		Ogre::LogManager::getSingleton().getDefaultLog()->setDebugOutputEnabled(false);

		const Ogre::RenderSystemList& renderers = _root->getAvailableRenderers();
		if (renderers.empty())
			std::cerr << "No hay RenderSystems disponibles" << std::endl;
		Ogre::RenderSystem* rs = renderers[0];
		_root->setRenderSystem(rs);

		_root->initialise();
		_windowWidth = width;
		_windowHeight = height;

		// Crear ventana basica
		Ogre::NameValuePairList params;
		params["externalWindowHandle"] = std::to_string((size_t)handle); // SDL maneja la ventana, solo usamos contexto
		params["FSAA"] = "0";
		params["vsync"] = "true";

		_window = _root->createRenderWindow("OgreWindow", width, height, false, &params);

		//Crear escena con main camera
		_sceneMgr = _root->createSceneManager();

		_nextTransformID = 0;
		_nextUITransformID = 0;
		_nextModelID = 0;
		_nextAnimationID = 0;
		_nextLightID = 0;
		_nextParticleGenID = 0;
		_nextSkydomeID = 0;

		_rgm = &Ogre::ResourceGroupManager::getSingleton();

		const std::filesystem::path ogreMainPath("./game/assets/ogre/Media/Main");
		const std::filesystem::path ogreRtShaderPath("./game/assets/ogre/Media/RTShaderLib");
		
		if (!std::filesystem::exists(ogreMainPath) || !std::filesystem::is_directory(ogreMainPath))
			throw std::runtime_error("[RENDER MODULE] No existe ./game/assets/ogre/Media/Main");
		if (!std::filesystem::exists(ogreRtShaderPath) || !std::filesystem::is_directory(ogreRtShaderPath))
			throw std::runtime_error("[RENDER MODULE] No existe ./game/assets/ogre/Media/RTShaderLib");

		_rgm->addResourceLocation(ogreMainPath.string(), "FileSystem", "Scene");
		_rgm->addResourceLocation(ogreRtShaderPath.string(), "FileSystem", "Scene");

		Ogre::RTShader::ShaderGenerator::initialize();
		_shaderGen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		if (!_shaderGen)
		{
			Debug::error("[RenderModule] No se pudo inicializar RTShader::ShaderGenerator");
			return false;
		}
		_shaderGen->addSceneManager(_sceneMgr);
		_shaderGen->setTargetLanguage("glsl");

		Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

		_overlaySystem = new Ogre::OverlaySystem();
		_sceneMgr->addRenderQueueListener(_overlaySystem);
		_overlay = new Ogre::ImGuiOverlay();
		if (ImGui_ImplSDL3_InitForOther(sdlWindow))
			_imguiSDLInitialized = true;
		ImGuiIO& io = ImGui::GetIO();
		_fonts["default"] = io.Fonts->AddFontDefault();

		io.DisplaySize = ImVec2(
			_windowWidth, _windowHeight
		);

		_ls = LoadingScreenData();

		/*Ogre::MaterialPtr materialUI = Ogre::MaterialManager::getSingleton().getByName("ImGui/material");
		_shaderGen->createShaderBasedTechnique(*materialUI, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true);

		_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, materialUI->getName());*/

		Ogre::MaterialManager& matMgr = Ogre::MaterialManager::getSingleton();

		//---------------debug colliders--------------
		std::string debugMatName = "Debug/PhysicsLines";
		//compruebo si existe por si acaso

		Ogre::MaterialPtr mat = matMgr.getByName(debugMatName);
		if (!mat)
		{
			mat = matMgr.create(debugMatName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			mat->setReceiveShadows(false);
			mat->setDepthCheckEnabled(true);
			mat->setDepthWriteEnabled(false);

			Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
			pass->setLightingEnabled(false);

			//para el color
			pass->setVertexColourTracking(Ogre::TVC_DIFFUSE);
			pass->setDepthCheckEnabled(false);//ignora si hay objetos delante
			pass->setDepthWriteEnabled(false);
			pass->setLightingEnabled(false);
			pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
			pass->setLineWidth(3.0f);//grosor linea
			mat->load();

			_shaderGen->createShaderBasedTechnique(*mat, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true);
			//_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, mat->getName(), mat->getGroup());
		}
		_debugDraw = _sceneMgr->createManualObject("DebugDraw");
		_debugDraw->setDynamic(true);
		_debugDraw->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
		_debugDraw->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
		_debugNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
		_debugNode->attachObject(_debugDraw);
		//---------------------------------------------------------

		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error iniciando OGRE: " << e.what() << std::endl;
		shutdown();
		return false;
	}
	catch (...)
	{
		std::cerr << "Error iniciando OGRE: excepcion desconocida" << std::endl;
		shutdown();
		return false;
	}
}

void RenderModule::buildFontAtlas()
{
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Build();
	io.DisplaySize = ImVec2(
		_windowWidth, _windowHeight
	);

	// Mostrar overlay despues de construir el atlas
	Ogre::OverlayManager::getSingleton().addOverlay(_overlay);
	_overlay->show();

	// Recrear shader para ImGui con el nuevo atlas
	Ogre::MaterialPtr materialUI = Ogre::MaterialManager::getSingleton().getByName("ImGui/material");
	_shaderGen->createShaderBasedTechnique(*materialUI, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true);
	_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, materialUI->getName());
}

core::ResourcePtr RenderModule::loadMesh(const std::string& id, const std::string& path, bool preload)
{
	/*std::string meshName = std::filesystem::path(path).filename().string();
	std::string folder = std::filesystem::path(path).parent_path().string() + "/";*/
	// Verificar si ya esta cargado
	if (Ogre::MeshManager::getSingleton().resourceExists(id)) {
		Debug::warning("[RenderModule] Mesh ya cargada: ", id);
		return nullptr;
	}
	// Copiado de addModel
	if (!Ogre::MeshManager::getSingleton().resourceExists(id)
		&& !_rgm->resourceGroupExists(path))
	{
		_rgm->addResourceLocation(path, "FileSystem", path);
		_rgm->loadResourceGroup(path);
		if (preload)
			_preloadedGroups.insert(path);
		else
			_resourceGroups.insert(path);
	}

	// Crea y carga el recurso.
	auto res = std::make_shared<MeshResource>(id, path);
	res->load();
	if (res->isValid())
		return res;
	// devuelve invalido
	return nullptr;
}

core::ResourcePtr RenderModule::loadTexture(const std::string& id, const std::string& path, bool preload)
{
	/*auto p = std::filesystem::path(path);
	std::string textureName = p.filename().string();
	std::string folder = p.parent_path().string() + "/";*/

	if (Ogre::TextureManager::getSingleton().resourceExists(id)) {
		Debug::warning("[RenderModule] Textura ya cargada: ", id);
		return nullptr;
	}

	if (!_rgm->resourceGroupExists(path))
	{
		_rgm->addResourceLocation(path, "FileSystem", path);
		_rgm->loadResourceGroup(path);
		if (preload)
			_preloadedGroups.insert(path);
		else
			_resourceGroups.insert(path);
	}

	auto res = std::make_shared<TextureResource>(id, path);
	res->load();
	if (res->isValid())
		return res;
	return nullptr;
}

core::ResourcePtr RenderModule::loadFont(const std::string& name, const std::string& path)
{
	ImGuiIO& io = ImGui::GetIO();
	std::vector<float> sizes = { 16.0f, 32.0f, 64.0f };

	std::string fullPath = path + name;
	std::string fontName = std::filesystem::path(name).stem().string();

	for (float size : sizes) {
		ImFont* f = io.Fonts->AddFontFromFileTTF(fullPath.c_str(), size);
		if (f) {
			_fonts[fontName + "_" + std::to_string((int)size)] = f;
		}
	}

	auto res = std::make_shared<core::Resource>(name, path, core::Resource::FONT);
	res->load();
	if (res->isValid())
		return res;
	return nullptr;
}

bool RenderModule::renderFrame()
{
	if (_vp != nullptr && _vp->getCamera() != nullptr)
	{
		scaleViewportToWindow();
		renderUI();
		_root->renderOneFrame();
		return true;
	}
	else
	{
		return false;
	}
}

bool RenderModule::renderLoadingScreen()
{
	if (_ls.exists && _vp != nullptr)
	{
		if (_vp->getCamera() == nullptr && _ls.cam != nullptr)
		{
			_vp->setCamera(_ls.cam);
		}
		if (_vp->getCamera() != nullptr)
		{
			scaleViewportToWindow();
			renderUI(true);
			//_root->renderOneFrame();
			return true;
		}
		else
		{
			return false;
		}
		
	}
	else
	{
		return false;
	}
}

void RenderModule::_calculateViewportRect(
	float windowWidth, float windowHeight, 
	float& vpLeft, float& vpTop, 
	float& vpWidth, float& vpHeight) const
{
	const float baseW = _windowWidth;
	const float baseH = _windowHeight;
	const float sx = windowWidth / baseW;
	const float sy = windowHeight / baseH;
	const float s = std::min(sx, sy);

	const float vpWpx = baseW * s;
	const float vpHpx = baseH * s;
	const float xPx = (windowWidth - vpWpx) * 0.5f;
	const float yPx = (windowHeight - vpHpx) * 0.5f;

	vpLeft = xPx / windowWidth;
	vpTop = yPx / windowHeight;
	vpWidth = vpWpx / windowWidth;
	vpHeight = vpHpx / windowHeight;
}

void RenderModule::scaleViewportToWindow()
{
	if (_vp == nullptr || _window == nullptr) return;
	_window->windowMovedOrResized();

	const float windowWidth = _window->getWidth();
	const float windowHeight = _window->getHeight();
	if (windowWidth <= 0.0f || windowHeight <= 0.0f) return;

	float vpLeft, vpTop, vpWidth, vpHeight;
	_calculateViewportRect(windowWidth, windowHeight, vpLeft, vpTop, vpWidth, vpHeight);
	_vp->setDimensions(vpLeft, vpTop, vpWidth, vpHeight);
}

bool RenderModule::getViewportRectPixels(int& x, int& y, int& w, int& h) const
{
	if (_window == nullptr) return false;

	if (_vp != nullptr)
	{
		x = _vp->getActualLeft();
		y = _vp->getActualTop();
		w = _vp->getActualWidth();
		h = _vp->getActualHeight();
	}
	else
	{
		x = 0;
		y = 0;
		w = _window->getWidth();
		h = _window->getHeight();
	}
	return (w > 0 && h > 0);
}

core::Vector2<> RenderModule::getResolution() const
{
	return { _windowWidth, _windowHeight };
}

core::Vector2<> RenderModule::windowToLogicCoords(const core::Vector2<>& windowPos) const
{
	int vpX = 0, vpY = 0, vpW = 0, vpH = 0;
	if (!getViewportRectPixels(vpX, vpY, vpW, vpH)) return { -1.0f, -1.0f };

	const auto ref = getResolution();
	if (vpW <= 0 || vpH <= 0 || ref.getX() <= 0 || ref.getY() <= 0) return { -1.0f, -1.0f };

	const float localX = std::clamp(windowPos.getX() - static_cast<float>(vpX), 0.0f, static_cast<float>(vpW));
	const float localY = std::clamp(windowPos.getY() - static_cast<float>(vpY), 0.0f, static_cast<float>(vpH));
	return {
		localX / static_cast<float>(vpW) * ref.getX(),
		localY / static_cast<float>(vpH) * ref.getY()
	};
}

void RenderModule::cleanScene(const bool& end)
{
	if (!_sceneMgr)
		return;

	//Limpiar camaras
	cleanCameras();

	//Limpiar luces
	cleanLights();

	//Limpiar animaciones
	cleanAnimations();

	//Limpiar modelos
	cleanModels();

	//Limpiar particulas
	cleanParticleGens();

	//Limpiar UI
	cleanUI();

	//limpiar debug
	cleanDebug();

	_engineNodes.clear();
	_nextTransformID = 0;
	_nextSkydomeID = 0;

	Ogre::MaterialManager& matMgr = Ogre::MaterialManager::getSingleton();

	for (auto& m : _createdMaterials)
	{
		if (matMgr.resourceExists(m))
		{
			matMgr.remove(m);
		}
	}

	//Si se va a crear una escena nueva dejamos una camara de seguridad. Volvemos a anadir rtss a imgui.
	if (!end)
	{
		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
		//Limpiamos solo recursos del juego
		for (auto resourceGroup : _resourceGroups)
		{
			// Si es un grupo precargado no lo libera.
			if (_preloadedGroups.find(resourceGroup) != _preloadedGroups.end())
				continue;

			// Liberar modelos y textruas
			_rgm->unloadResourceGroup(resourceGroup);

			// Limpiar lista
			_rgm->clearResourceGroup(resourceGroup);
		}
		_resourceGroups.clear();

		//recreacion de debug draw
		_debugDraw = _sceneMgr->createManualObject("DebugDraw");
		_debugDraw->setDynamic(true);
		_debugDraw->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
		_debugDraw->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);

		_debugNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
		_debugNode->attachObject(_debugDraw);

		//Recrear loading screen
		if (_ls.exists)
		{
			createLoadingScreenScene(_ls.bgImageFolder, _ls.bgImageName, _ls.barFill, _ls.fontName);
		}
	}
	else
	{
		if (_ls.cam != nullptr)
		{
			Ogre::SceneNode* parent = _ls.cam->getParentSceneNode();
			if (parent)
				parent->detachObject(_ls.cam);

			_sceneMgr->destroyCamera(_ls.cam);
			_ls.cam = nullptr;
		}

		_sceneMgr->clearScene();

		Ogre::StringVector groups = _rgm->getResourceGroups();
		for (const std::string& groupName : groups)
		{
			// Liberar modelos y textruas
			_rgm->unloadResourceGroup(groupName);

			// Limpiar lista
			_rgm->clearResourceGroup(groupName);

			// Borrar grupo
			_rgm->destroyResourceGroup(groupName);
		}
		_resourceGroups.clear();
		_preloadedGroups.clear();
	}
}

RenderModule::EventCallback RenderModule::getImguiInputCallback()
{
	return ImGui_ImplSDL3_ProcessEvent;
}

transformID RenderModule::addNode(const entityID& entityID, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale, const bool& fromTransform)
{
	for (int i = 0; i < (int)_engineNodes.size(); i++)
	{
		if (_engineNodes[i].nodeID == entityID)
		{
			if (fromTransform)
			{
				_engineNodes[i].sceneNode->setPosition(Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()));
				_engineNodes[i].sceneNode->setOrientation(Ogre::Quaternion(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));
				_engineNodes[i].sceneNode->setScale(Ogre::Vector3(scale.getX(), scale.getY(), scale.getZ()));
			}
			return i; //Ya existe
		}
	}

	// Crear nuevo nodo
	EngineNode& aux = _engineNodes.emplace_back(_sceneMgr->getRootSceneNode()->createChildSceneNode(), entityID);
	aux.sceneNode->setPosition(Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()));
	aux.sceneNode->setOrientation(Ogre::Quaternion(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));
	aux.sceneNode->setScale(Ogre::Vector3(scale.getX(), scale.getY(), scale.getZ()));
	return _nextTransformID++;

}

transformID RenderModule::getNode(const entityID& entityID)
{
	for (int i = _engineNodes.size() - 1; i >= 0; i--)
	{
		if (_engineNodes[i].nodeID == entityID) return i;
	}
	return UINT64_MAX;
}

core::Vector3<float> RenderModule::getNodePosition(const transformID& id)
{
	if (id != UINT64_MAX && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		Ogre::Vector3 pos = _engineNodes[id].sceneNode->getPosition();
		return core::Vector3<float>(pos.x, pos.y, pos.z);
	}
	else return core::Vector3<float>(0.0f, 0.0f, 0.0f);
}

void RenderModule::setNodePosition(const transformID& id, const core::Vector3<float>& pos)
{
	if (id != UINT64_MAX && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		_engineNodes[id].sceneNode->setPosition(pos.getX(), pos.getY(), pos.getZ());
	}
}

core::Quaternion<float> RenderModule::getNodeRotation(const transformID& id)
{
	if (id != UINT64_MAX && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		Ogre::Quaternion rot = _engineNodes[id].sceneNode->getOrientation();
		return core::Quaternion<float>(rot.x, rot.y, rot.z, rot.w);
	}
	else return core::Quaternion<float>(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderModule::setNodeRotation(const transformID& id, const core::Quaternion<float>& rot)
{
	if (id != UINT64_MAX && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		_engineNodes[id].sceneNode->setOrientation(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
	}
}

core::Vector3<float> RenderModule::getNodeScale(const transformID& id)
{
	if (id != UINT64_MAX && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		Ogre::Vector3 pos = _engineNodes[id].sceneNode->getScale();
		return core::Vector3<float>(pos.x, pos.y, pos.z);
	}
	else return core::Vector3<float>(0.0f, 0.0f, 0.0f);
}

void RenderModule::setNodeScale(const transformID& id, const core::Vector3<float>& scale)
{
	if (id != UINT64_MAX && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		_engineNodes[id].sceneNode->setScale(scale.getX(), scale.getY(), scale.getZ());
	}
}
UITransformID RenderModule::addUITransform(const entityID& entityID, const core::Vector2<float>& pos, const int& dLayer, const core::Vector2<float>& dimension, const float& rotation)
{
	if (dLayer < 0 || dLayer > 32) {
		Debug::error("depth layer fuera de rango [0-32]");
		return -1;
	}
	for (int i = 0; i < (int)_uiTransforms.size(); i++)
	{
		if (_uiTransforms[i].entity == entityID)
		{
			return i; //Ya existe
		}
	}

	UITransformData uiT;
	uiT.entity = entityID;
	uiT.position = pos;
	uiT.dimension = dimension;
	uiT.rotation = rotation;
	uiT.depthLayer = dLayer;
	_uiTransforms.emplace_back(uiT);
	return _uiTransforms.size() - 1;
}

void RenderModule::setUITransformDimension(const UITransformID& id, const core::Vector2<float>& dim) {
	if (id != UINT64_MAX && id < _uiTransforms.size()) {
		_uiTransforms[id].dimension = { std::max(0.1f,dim.getX()), std::max(0.1f,dim.getY()) };
	}
}

void RenderModule::setUITransformPos(const UITransformID& id, const core::Vector2<float>& pos) {
	if (id != UINT64_MAX && id < _uiTransforms.size()) {
		_uiTransforms[id].position = pos;
	}
}

void RenderModule::setUITransformRotation(const UITransformID& id, const float& r) {
	if (id != UINT64_MAX && id < _uiTransforms.size()) {
		_uiTransforms[id].rotation = r;
	}
}

void RenderModule::setUITransformDepthLayer(const UITransformID& id, const int& dLayer) {
	if (id != UINT64_MAX && id < _uiTransforms.size()) {
		_uiTransforms[id].depthLayer = dLayer;
	}
}
UITransformID RenderModule::getTransformUI(const entityID& entityID)
{
	for (int i = _uiTransforms.size() - 1; i >= 0; i--) {
		if (_uiTransforms[i].entity == entityID) return i;

	}
	return UINT64_MAX;
}

void RenderModule::setViewportBGColor(const core::Color& color)
{
	_vp->setBackgroundColour(Ogre::ColourValue(color.getRed(), color.getGreen(), color.getBlue()));
}

cameraID RenderModule::addCamera(const entityID& entityID, const float& FOVy, const float& nearClipDistance, const float& farClipDistance, const float& focalLength, const core::Color& bgColor)
{
	//Si no existe un nodo con este entityID lo creamos
	transformID nodeID = addNode(entityID);
	Ogre::Camera* camera = _cameras.emplace_back(_sceneMgr->createCamera("camera" + entityID.toString()));
	cameraID createdCameraID = _cameras.size() - 1;
	camera->setAutoAspectRatio(true);
	_engineNodes[nodeID].sceneNode->attachObject(camera);

	camera->setFOVy(Ogre::Radian(FOVy));
	camera->setNearClipDistance(nearClipDistance);
	camera->setFarClipDistance(farClipDistance);
	camera->setFocalLength(focalLength);

	//Si es la primera camara manual se convierte automaticamente en la activa
	if (_vp == nullptr || _vp->getCamera() == nullptr || (_cameras.size() == 1))
	{
		setAsActiveCamera(createdCameraID);
		_vp->setBackgroundColour(Ogre::ColourValue(bgColor.getRed(), bgColor.getGreen(), bgColor.getBlue()));
	}
	return createdCameraID;
}

void RenderModule::deleteCamera(const cameraID& id)
{
	if (id != UINT64_MAX && id < _cameras.size() && _cameras[id] != nullptr && _vp->getCamera() != _cameras[id])
	{
		Ogre::Camera* cam = _cameras[id];
		_cameras[id] = nullptr;
		Ogre::SceneNode* parent = cam->getParentSceneNode();
		if (parent) parent->detachObject(cam);
		_sceneMgr->destroyCamera(cam);
	}
}

void RenderModule::setAsActiveCamera(const cameraID& id)
{
	if (id != UINT64_MAX && id < _cameras.size() && _cameras[id] != nullptr)
	{
		if (_vp == nullptr)
		{
			_vp = _window->addViewport(_cameras[id]);
		}
		else
		{
			_vp->setCamera(_cameras[id]);
		}
		_vp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
		_vp->setOverlaysEnabled(true);
	}
}

void RenderModule::cleanCameras()
{
	while (!_cameras.empty())
	{
		Ogre::Camera* cam = _cameras.back();
		_cameras.pop_back();
		if (cam != nullptr)
		{
			Ogre::SceneNode* parent = cam->getParentSceneNode();
			if (parent)
				parent->detachObject(cam);

			_sceneMgr->destroyCamera(cam);
		}
	}
	_cameras.clear();
	if (_vp)
		_vp->setCamera(nullptr);
}

void RenderModule::setCameraFOVy(const cameraID& id, const float& FOVy)
{
	if (id != UINT64_MAX && id < _cameras.size() && _cameras[id] != nullptr) _cameras[id]->setFOVy(Ogre::Radian(FOVy));
}

void RenderModule::setCameraNearClipDistance(const cameraID& id, const float& nearClipDistance)
{
	if (id != UINT64_MAX && id < _cameras.size() && _cameras[id] != nullptr) _cameras[id]->setNearClipDistance(nearClipDistance);
}

void RenderModule::setCameraFarClipDistance(const cameraID& id, const float& farClipDistance)
{
	if (id != UINT64_MAX && id < _cameras.size() && _cameras[id] != nullptr) _cameras[id]->setFarClipDistance(farClipDistance);
}

void RenderModule::setCameraFocalLength(const cameraID& id, const float& focalLength)
{
	if (id != UINT64_MAX && id < _cameras.size() && _cameras[id] != nullptr) _cameras[id]->setFocalLength(focalLength);
}

modelID RenderModule::addModel(const entityID& entityID, const std::string& modelFolder, const std::string& modelFile)
{
	transformID nodeID = addNode(entityID);

	// si el recurso no esta cargado y el grupo no existe lo registra
	if (!Ogre::MeshManager::getSingleton().resourceExists(modelFile) && !_rgm->resourceGroupExists(modelFolder))
	{
		_rgm->addResourceLocation(modelFolder, "FileSystem", modelFolder);
		_rgm->loadResourceGroup(modelFolder);
		_resourceGroups.insert(modelFolder);
	}

	Ogre::Entity* model = _models.emplace_back(
		_sceneMgr->createEntity(modelFile + std::to_string(_nextModelID), modelFile));

	_engineNodes[nodeID].sceneNode->attachObject(model);

	for (unsigned int i = 0; i < model->getNumSubEntities(); ++i)
	{
		Ogre::SubEntity* sub = model->getSubEntity(i);
		Ogre::MaterialPtr mat = sub->getMaterial();
		Ogre::MaterialManager& matMgr = Ogre::MaterialManager::getSingleton();

		Ogre::String baseMatName = "BaseMatChavales_" + std::to_string(_nextModelID) + "_" + std::to_string(i);
		Ogre::MaterialPtr baseMat = matMgr.getByName(baseMatName);
		if (!baseMat)
		{
			// Obtener BaseWhite
			Ogre::MaterialPtr baseWhite = matMgr.getByName("BaseWhite");

			if (!baseWhite)
			{
				Debug::error("[RenderModule] BaseWhite no encontrado");
			}
			else
			{
				baseMat = baseWhite->clone(baseMatName);
				baseMat->load();

				// RTSS
				_shaderGen->createShaderBasedTechnique(*baseMat, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true);

				_shaderGen->invalidateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, baseMat->getName(), baseMat->getGroup());

				if (!_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, baseMat->getName()))
				{
					Debug::error("[RenderModule] BaseMat validateMaterial");
				}
			}
		}

		_createdMaterials.push_back(baseMatName);
		sub->setMaterial(Ogre::MaterialManager::getSingleton().getByName(baseMatName));
	}

	return _nextModelID++;
}

void RenderModule::deleteModel(const modelID& id)
{
	if (id != UINT64_MAX && id < _models.size() && _models[id] != nullptr)
	{
		Ogre::Entity* model = _models[id];
		_models[id] = nullptr;

		Ogre::SceneNode* parent = model->getParentSceneNode();

		Ogre::MaterialManager& matMgr = Ogre::MaterialManager::getSingleton();

		for (unsigned int i = 0; i < model->getNumSubEntities(); ++i)
		{
			Ogre::SubEntity* sub = model->getSubEntity(i);
			Ogre::MaterialPtr mat = sub->getMaterial();

			if (mat)
			{
				Ogre::String matName = mat->getName();

				for (unsigned short t = 0; t < mat->getNumTechniques(); ++t)
				{
					Ogre::Technique* tech = mat->getTechnique(t);
					if (_shaderGen && tech)
					{
						try
						{
							_shaderGen->removeShaderBasedTechnique(tech, Ogre::MaterialManager::DEFAULT_SCHEME_NAME);
						}
						catch (...)
						{
							Debug::error("[RENDER MODULE]: Error borrando ShaderBasedTechnique.");
						}
					}
				}
				if (matMgr.resourceExists(matName))
				{
					matMgr.remove(matName);
				}
			}
		}

		if (parent)
			parent->detachObject(model);

		_sceneMgr->destroyEntity(model);
	}
}

void RenderModule::cleanModels()
{
	// itera al reves para evitar problemas de 
	// que intente eliminarse algo ya eliminado
	while (!_models.empty())
	{
		Ogre::Entity* model = _models.back();
		_models.pop_back(); // Elimina de la lista mientras iteras
		if (model != nullptr)
		{
			Ogre::SceneNode* parent = model->getParentSceneNode();

			Ogre::MaterialManager& matMgr = Ogre::MaterialManager::getSingleton();

			for (unsigned int i = 0; i < model->getNumSubEntities(); ++i)
			{
				Ogre::SubEntity* sub = model->getSubEntity(i);
				Ogre::MaterialPtr mat = sub->getMaterial();

				if (mat)
				{
					Ogre::String matName = mat->getName();

					for (unsigned short t = 0; t < mat->getNumTechniques(); ++t)
					{
						Ogre::Technique* tech = mat->getTechnique(t);
						if (_shaderGen && tech)
						{
							try
							{
								_shaderGen->removeShaderBasedTechnique(tech, Ogre::MaterialManager::DEFAULT_SCHEME_NAME);
							}
							catch (...)
							{
								Debug::error("[RENDER MODULE]: Error borrando ShaderBasedTechnique.");
							}
						}
					}
					if (matMgr.resourceExists(matName))
					{
						matMgr.remove(matName);
					}
				}
			}

			if (parent)
				parent->detachObject(model);

			_sceneMgr->destroyEntity(model);
		}
	}

	_nextModelID = 0;
}

void RenderModule::setDiffuse(const modelID& id, const subMeshID& subID, const std::string& textureFolder, const std::string& textureFile)
{
	if (id != UINT64_MAX && id < _models.size() && _models[id] != nullptr)
	{
		Ogre::Entity* model = _models[id];
		Ogre::SubEntity* sub = model->getSubEntity(subID);

		Ogre::MaterialPtr mat = sub->getMaterial();
		sub->setMaterial(mat);

		if (mat->getNumTechniques() == 0)
			mat->createTechnique();
		Ogre::Technique* tech = mat->getTechnique(0);
		if (tech->getNumPasses() == 0)
			tech->createPass();
		Ogre::Pass* pass = tech->getPass(0);

		if (!_rgm->resourceGroupExists(textureFolder))
		{
			_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
			_rgm->loadResourceGroup(textureFolder);
			// Solo meter a _resourceGroups si no es precargado
			if (_preloadedGroups.find(textureFolder) == _preloadedGroups.end())
				_resourceGroups.insert(textureFolder);
		}
		Ogre::TexturePtr text = Ogre::TextureManager::getSingleton().load(textureFile, textureFolder, Ogre::TEX_TYPE_2D, 0);

		Ogre::TextureUnitState* tus;
		if (pass->getNumTextureUnitStates() == 0)
		{
			tus = pass->createTextureUnitState();
		}
		else
		{
			tus = pass->getTextureUnitState(0);
		}
		tus->setTexture(text);
		tus->setColourOperation(Ogre::LBO_MODULATE);
	}
}

void RenderModule::setTint(const modelID& id, const subMeshID& subID, const core::Color& tint)
{
	if (id != UINT64_MAX && id < _models.size() && _models[id] != nullptr)
	{
		Ogre::Entity* model = _models[id];
		Ogre::SubEntity* sub = model->getSubEntity(subID);

		Ogre::MaterialPtr mat = sub->getMaterial();

		if (mat->getNumTechniques() == 0)
			mat->createTechnique();
		Ogre::Technique* tech = mat->getTechnique(0);
		if (tech->getNumPasses() == 0)
			tech->createPass();
		Ogre::Pass* pass = tech->getPass(0);

		pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
		pass->setDepthWriteEnabled(false);

		pass->setDiffuse(tint.getRed(), tint.getGreen(), tint.getBlue(), tint.getAlpha());

		mat->reload();
		sub->setMaterial(mat);

		_shaderGen->invalidateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, mat->getName(), mat->getGroup());

		_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, mat->getName());
	}
}

core::Color RenderModule::getTint(const modelID& id, const subMeshID& subID)
{
	if (id != UINT64_MAX && id < _models.size() && _models[id] != nullptr)
	{
		Ogre::Entity* model = _models[id];
		Ogre::SubEntity* sub = model->getSubEntity(subID);

		Ogre::MaterialPtr mat = sub->getMaterial();

		if (mat->getNumTechniques() > 0)
		{
			Ogre::Technique* tech = mat->getTechnique(0);
			if (tech->getNumPasses() > 0)
			{
				Ogre::Pass* pass = tech->getPass(0);
				Ogre::ColourValue color = pass->getDiffuse();
				return core::Color(color.r, color.g, color.b, color.a);;
			}
		}
		return core::Color(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void RenderModule::setModelVisible(const modelID& id, const bool& visible)
{
	if (id != UINT64_MAX && id < _models.size() && _models[id] != nullptr)
	{
		Ogre::Entity* model = _models[id];
		Ogre::SceneNode* node = model->getParentSceneNode();
		if (node)
			node->setVisible(visible, false);
	}
}

void RenderModule::cleanAnimations()
{
	for (auto& anim : _sceneAnims)
	{
		_sceneMgr->destroyAnimationState(anim);
		_sceneMgr->destroyAnimation(anim);
	}
	_sceneAnims.clear();
	_animations.clear();
	_nextAnimationID = 0;
}

animationID RenderModule::registerSkeletonAnim(const modelID& modelID, const std::string& animationName, const bool& loop)
{
	if (modelID != UINT64_MAX && modelID < _models.size() && _models[modelID] != nullptr)
	{
		try
		{
			auto anim = _animations.emplace_back(std::make_pair(_models[modelID]->getAnimationState(animationName), 1.0f));
			if (anim.first == nullptr)
				return UINT64_MAX;
			_animations.back().first->setLoop(loop);
			return _nextAnimationID++;
		}
		catch (const std::exception& e)
		{
			Debug::error("[RenderModule] registerSkeletonAnim fallo para '", animationName, "': ", e.what());
			return UINT64_MAX;
		}
		catch (...)
		{
			Debug::error("[RenderModule] registerSkeletonAnim fallo para '", animationName, "': excepcion desconocida");
			return UINT64_MAX;
		}
	}
	return UINT64_MAX;
}

animationID RenderModule::createTransformAnimation(const entityID& entityID, const std::string& animationName, const bool& loop, const float& totalDuration)
{
	transformID nodeID = getNode(entityID);
	if (nodeID != UINT64_MAX && _engineNodes[nodeID].sceneNode != nullptr)
	{
		Ogre::Animation* animation = _sceneMgr->createAnimation(animationName + std::to_string(_nextAnimationID), totalDuration);
		_sceneAnims.push_back(animationName + std::to_string(_nextAnimationID));
		animation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
		animation->createNodeTrack(0, _engineNodes[nodeID].sceneNode);
		_animations.emplace_back(std::make_pair(_sceneMgr->createAnimationState(animationName + std::to_string(_nextAnimationID)), 1.0f));
		_animations.back().first->setLoop(loop);
		return _nextAnimationID++;
	}
	return UINT64_MAX;
}

void RenderModule::addTransformKeyFrame(const animationID& animationID, const float& timePos, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale)
{
	if (animationID != UINT64_MAX && animationID < _animations.size() && _animations[animationID].first != nullptr)
	{
		Ogre::Animation* anim = _sceneMgr->getAnimation(_animations[animationID].first->getAnimationName());

		Ogre::NodeAnimationTrack* track = anim->getNodeTrack(0);

		Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(timePos);
		kf->setTranslate(Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()));
		kf->setRotation(Ogre::Quaternion(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));
		kf->setScale(Ogre::Vector3(scale.getX(), scale.getY(), scale.getZ()));
	}
}

void RenderModule::addTransformKeyFrame(const animationID& animationID, const float& timePos, const core::Vector3<float>& pos, const float& rot, const int& axis, const core::Vector3<float>& scale)
{
	if (animationID != UINT64_MAX && animationID < _animations.size() && _animations[animationID].first != nullptr)
	{
		Ogre::Animation* anim = _sceneMgr->getAnimation(_animations[animationID].first->getAnimationName());

		Ogre::NodeAnimationTrack* track = anim->getNodeTrack(0);

		Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(timePos);
		kf->setTranslate(Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()));
		Ogre::Vector3 ogreAxis;
		switch (axis)
		{
		case 0:
			ogreAxis = Ogre::Vector3::UNIT_X;
			break;
		case 1:
			ogreAxis = Ogre::Vector3::UNIT_Y;
			break;
		case 2:
			ogreAxis = Ogre::Vector3::UNIT_Z;
			break;
		}
		kf->setRotation(Ogre::Quaternion(Ogre::Degree(rot), ogreAxis));
		kf->setScale(Ogre::Vector3(scale.getX(), scale.getY(), scale.getZ()));
	}
}

void RenderModule::setAnimEnabled(const animationID& animationID, const bool& active)
{
	if (animationID != UINT64_MAX && animationID < _animations.size() && _animations[animationID].first != nullptr)
	{
		try
		{
			_animations[animationID].first->setEnabled(active);
		}
		catch (const std::exception& e)
		{
			Debug::error("[RenderModule] setAnimEnabled fallo (id=", std::to_string(animationID), "): ", e.what());
		}
		catch (...)
		{
			Debug::error("[RenderModule] setAnimEnabled fallo (id=", std::to_string(animationID), "): excepcion desconocida");
		}
	}
}

void RenderModule::setAnimTimePos(const animationID& animationID, const float& timePos)
{
	if (animationID != UINT64_MAX && animationID < _animations.size() && _animations[animationID].first != nullptr)
	{
		try
		{
			_animations[animationID].first->setTimePosition(timePos);
		}
		catch (const std::exception& e)
		{
			Debug::error("[RenderModule] setAnimTimePos fallo (id=", std::to_string(animationID), "): ", e.what());
		}
		catch (...)
		{
			Debug::error("[RenderModule] setAnimTimePos fallo (id=", std::to_string(animationID), "): excepcion desconocida");
		}
	}
}

void RenderModule::setAnimSpeed(const animationID& animationID, const float& speed)
{
	if (animationID != UINT64_MAX && animationID < _animations.size() && _animations[animationID].first != nullptr)
	{
		_animations[animationID].second = speed;
	}
}

void RenderModule::updateAnimation(const animationID& animationID, const uint64_t& deltaTime)
{
	if (animationID != UINT64_MAX && animationID < _animations.size() && _animations[animationID].first != nullptr)
	{
		auto& anim = _animations[animationID];
		try
		{
			anim.first->addTime(((float)deltaTime / 1000.0f) * anim.second);
		}
		catch (const std::exception& e)
		{
			Debug::error("[RenderModule] updateAnimation fallo (id=", std::to_string(animationID), "): ", e.what());
			anim.first = nullptr;
		}
		catch (...)
		{
			Debug::error("[RenderModule] updateAnimation fallo (id=", std::to_string(animationID), "): excepcion desconocida");
			anim.first = nullptr;
		}
	}
}

lightID RenderModule::addLight(const entityID& entityID, const int& type, const core::Color& color, const float& intensity)
{
	//Si no existe un nodo con este entityID lo creamos
	transformID nodeID = addNode(entityID);

	Ogre::Light* light = _sceneMgr->createLight("light" + std::to_string(_nextLightID));

	switch (type)
	{
	case 0: light->setType(Ogre::Light::LT_POINT); break;
	case 1: light->setType(Ogre::Light::LT_DIRECTIONAL); break;
	case 2: light->setType(Ogre::Light::LT_SPOTLIGHT); break;
	case 3: light->setType(Ogre::Light::LT_RECTLIGHT); break;
	}
	light->setDiffuseColour(color.getRed(), color.getGreen(), color.getBlue());
	light->setSpecularColour(color.getRed(), color.getGreen(), color.getBlue());

	light->setPowerScale(intensity);

	_engineNodes[nodeID].sceneNode->attachObject(light);

	_lights.push_back(light);

	return _nextLightID++;
}

void  RenderModule::deleteLight(const lightID& id)
{
	if (id != UINT64_MAX && id < _lights.size() && _lights[id] != nullptr)
	{
		Ogre::Light* light = _lights[id];
		_lights[id] = nullptr;
		Ogre::SceneNode* parent = light->getParentSceneNode();
		if (parent) parent->detachObject(light);
		_sceneMgr->destroyLight(light);
	}
}

void RenderModule::setLightActive(const lightID& id, const bool& active)
{
	if (id != UINT64_MAX && id < _lights.size() && _lights[id] != nullptr) _lights[id]->setVisible(active);
}

void RenderModule::cleanLights()
{
	while (!_lights.empty())
	{
		Ogre::Light* light = _lights.back();
		_lights.pop_back();
		if (light != nullptr)
		{
			Ogre::SceneNode* parent = light->getParentSceneNode();
			if (parent)
				parent->detachObject(light);
			_sceneMgr->destroyLight(light);
		}
	}

	_lights.clear();
	_nextLightID = 0;
}

void RenderModule::setLightType(const lightID& id, const int& type)
{
	if (id != UINT64_MAX && id < _lights.size() && _lights[id] != nullptr)
	{
		Ogre::Light* light = _lights[id];
		switch (type)
		{
		case 0:
			light->setType(Ogre::Light::LT_POINT);
			break;
		case 1:
			light->setType(Ogre::Light::LT_DIRECTIONAL);
			break;
		case 2:
			light->setType(Ogre::Light::LT_SPOTLIGHT);
			break;
		case 3:
			light->setType(Ogre::Light::LT_RECTLIGHT);
			break;
		}
	}
}

void RenderModule::setLightColor(const lightID& id, const core::Color& color)
{
	if (id != UINT64_MAX && id < _lights.size() && _lights[id] != nullptr) _lights[id]->setDiffuseColour(color.getRed(), color.getGreen(), color.getBlue());
}

void RenderModule::setLightIntensity(const lightID& id, const float& intensity)
{
	if (id != UINT64_MAX && id < _lights.size() && _lights[id] != nullptr) _lights[id]->setPowerScale(intensity);
}

void RenderModule::setLightSpotRange(const lightID& id, const float& inner, const float& outer, const float& falloff)
{
	if (id != UINT64_MAX && id < _lights.size() && _lights[id] != nullptr) _lights[id]->setSpotlightRange(Ogre::Degree(inner), Ogre::Degree(outer), falloff);
}

void RenderModule::setAmbientLight(const core::Color& color)
{
	_sceneMgr->setAmbientLight(Ogre::ColourValue(color.getRed(), color.getGreen(), color.getBlue()));
}

particleGenID RenderModule::addParticleGen(const entityID& entityID, const std::string& textureFolder, const std::string& textureFile)
{
	if (textureFolder.empty() || textureFile.empty())
	{
		Debug::error("[ParticleGen] Texture folder/file vacio. Carpeta: ", textureFolder, ", Archivo: ", textureFile, ".");
		return UINT64_MAX;
	}

	transformID nodeID = addNode(entityID);

	std::string matName = "ParticleMat_" + std::to_string(_nextParticleGenID);

	try
	{
		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(matName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		_createdMaterials.push_back(matName);
		mat->setReceiveShadows(false);

		Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
		pass->setLightingEnabled(false);
		pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
		pass->setDepthWriteEnabled(false);

		if (!_rgm->resourceGroupExists(textureFolder))
		{
			_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
			_rgm->loadResourceGroup(textureFolder);
			_resourceGroups.insert(textureFolder);
		}

		if (!Ogre::ResourceGroupManager::getSingleton().resourceExists(textureFolder, textureFile))
		{
			Debug::error("[ParticleGen] No existe textura ", textureFile, " en ", textureFolder, ".");
			return UINT64_MAX;
		}

		Ogre::TexturePtr text = Ogre::TextureManager::getSingleton().load(textureFile, textureFolder, Ogre::TEX_TYPE_2D, 0);

		Ogre::TextureUnitState* tus = pass->createTextureUnitState();
		tus->setTexture(text);
		tus->setColourOperation(Ogre::LBO_MODULATE);

		mat->load();

		//Asignar RTSS
		if (_shaderGen)
		{
			_shaderGen->createShaderBasedTechnique(*mat, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true);
			if (!_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, mat->getName()))
				Debug::error("[ParticleGen] No se valido el material ", mat->getName()," correctamente.");
		}

		Ogre::ParticleSystem* ps = _particleGens.emplace_back(_sceneMgr->createParticleSystem("ParticleGen_" + std::to_string(_nextParticleGenID)));
		ps->setMaterialName(matName);
		ps->addEmitter("Point");
		_engineNodes[nodeID].sceneNode->attachObject(ps);

		return _nextParticleGenID++;
	}
	catch (const std::exception& e)
	{
		Debug::error("[ParticleGen] Error creando sistema de particulas: ", e.what());
		return UINT64_MAX;
	}
	catch (...)
	{
		Debug::error("[ParticleGen] Error desconocido creando sistema de particulas.");
		return UINT64_MAX;
	}
}

void RenderModule::deleteParticleGen(const particleGenID& id)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
	{
		Ogre::ParticleSystem* ps = _particleGens[id];
		Ogre::String mn = ps->getMaterialName();
		Ogre::SceneNode* parent = ps->getParentSceneNode();
		if (parent)
			parent->detachObject(ps);

		_sceneMgr->destroyParticleSystem(ps);

		_particleGens[id] = nullptr;

		if (!mn.empty())
			Ogre::MaterialManager::getSingleton().remove(mn);
	}
}

void RenderModule::cleanParticleGens()
{
	while (!_particleGens.empty())
	{
		Ogre::ParticleSystem* ps = _particleGens.back();
		_particleGens.pop_back();
		if (ps != nullptr)
		{
			Ogre::SceneNode* parent = ps->getParentSceneNode();
			if (parent)
				parent->detachObject(ps);

			_sceneMgr->destroyParticleSystem(ps);
		}
	}

	_particleGens.clear();
	_nextParticleGenID = 0;
}

void RenderModule::setParticleGenEnabled(const particleGenID& id, const bool& enabled)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setEnabled(enabled);
}

void RenderModule::setParticleGenEmitting(const particleGenID& id, const bool& emitting)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setEnabled(emitting);
}

void RenderModule::setParticleGenQuota(const particleGenID& id, const float& quota)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->setParticleQuota(static_cast<size_t>(quota));
}

void RenderModule::setParticleGenEmissionRate(const particleGenID& id, const float& rate)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setEmissionRate(rate);
}

void RenderModule::setParticleGenDuration(const particleGenID& id, const float& duration)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setDuration(duration);
}

void RenderModule::setParticleGenTimeToLive(const particleGenID& id, const float& time)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setTimeToLive(time);
}

void RenderModule::setParticleGenVelocity(const particleGenID& id, const float& velocity)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setParticleVelocity(velocity);
}

void RenderModule::setParticleGenMinVelocity(const particleGenID& id, const float& velocity)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setMinParticleVelocity(velocity);
}

void RenderModule::setParticleGenMaxVelocity(const particleGenID& id, const float& velocity)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setMaxParticleVelocity(velocity);
}

void RenderModule::setParticleGenDirection(const particleGenID& id, const core::Vector3<float>& direction)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setDirection(Ogre::Vector3(direction.getX(), direction.getY(), direction.getZ()));
}

void RenderModule::setParticleGenAngle(const particleGenID& id, const float& angle)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setAngle(Ogre::Degree(angle));
}

void RenderModule::setParticleGenPartWidth(const particleGenID& id, const float& width)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->setDefaultWidth(width);
}

void RenderModule::setParticleGenPartHeight(const particleGenID& id, const float& height)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->setDefaultHeight(height);
}

void RenderModule::setParticleGenPartColor(const particleGenID& id, const core::Color& color)
{
	if (id != UINT64_MAX && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setColour(Ogre::ColourValue(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));
}

void RenderModule::setSkydome(const std::string& textureFolder, const std::string& textureFile, const float& curvature, const float& tiling, const float& distance, const bool& drawFirst)
{
	if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(textureFolder))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(textureFolder, "FileSystem", textureFolder);
		Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(textureFolder);
		_resourceGroups.insert(textureFolder);
	}

	std::string matName = "SkydomeMat_" + std::to_string(_nextSkydomeID++);

	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(matName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	_createdMaterials.push_back(matName);
	Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
	pass->setLightingEnabled(false);
	pass->setDepthWriteEnabled(false);

	if (!_rgm->resourceGroupExists(textureFolder))
	{
		_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
		_rgm->loadResourceGroup(textureFolder);
		_resourceGroups.insert(textureFolder);
	}

	Ogre::TexturePtr text = Ogre::TextureManager::getSingleton().load(textureFile, textureFolder, Ogre::TEX_TYPE_2D, 0);

	Ogre::TextureUnitState* tus = pass->createTextureUnitState();
	tus->setTexture(text);
	tus->setColourOperation(Ogre::LBO_MODULATE);

	mat->load();

	//Asignar RTSS
	_shaderGen->createShaderBasedTechnique(*mat, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true);
	_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, mat->getName());

	_sceneMgr->setSkyDome(true, matName, curvature, tiling, distance, drawFirst);
}

void RenderModule::setSkydomeNull()
{
	_sceneMgr->setSkyDome(false, "");
}

uiPanelID RenderModule::addUIPanel(const entityID& entityID, const std::string& title)
{
	addUITransform(entityID);
	UIPanelData panel;
	panel.entity = entityID;
	panel.title = title;
	panel.visible = true;

	_uiPanels.push_back(panel);

	return _nextPanelID++;
}

void RenderModule::setUIPanelVisible(const uiPanelID& id, bool visible)
{
	_uiPanels[id].visible = visible;
}
void  RenderModule::deleteUIPanel(const uiPanelID& id) {
	auto& panel = _uiPanels[id];
	panel.alive = false;
	panel.visible = false;

	for (auto& button : panel.buttons) {
		button.alive = false;
		button.onClick = nullptr;
	}
	for (auto& label : panel.labels) {
		label.alive = false;
	}
	for (auto& texRect : panel.textureRects) {
		texRect.alive = false;
	}
}


uiLabelID RenderModule::addUILabel(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const  float opacity, const core::Color& textColor, const core::Color& bgColor, const float fontSize, const TextAlign textAlign, const std::string& fontName)
{
	addUITransform(entityID);

	UILabelData label;
	label.entity = entityID;
	label.text = text;
	label.visible = true;
	label.opacity = opacity;
	label.textColor = textColor;
	label.bgColor = bgColor;
	label.fontSize = fontSize;
	label.align = textAlign;

	//std::string auxFontName = fontName + "_" + std::to_string((int)fontSize);
	std::string auxFontName = std::filesystem::path(fontName).stem().string() + "_" + std::to_string((int)fontSize);
	auto it = _fonts.find(auxFontName);
	if (it != _fonts.end())
	{
		label.font = it->second;
	}
	else
	{
		label.font = _fonts["default"];
	}
	_uiPanels[panelID].labels.push_back(label);

	uiLabelID id = _nextLabelID++;
	int  labelIndex = _uiPanels[panelID].labels.size() - 1;
	_labelToPanel[id] = { panelID, labelIndex };
	return id;
}

void RenderModule::deleteUILabel(const uiLabelID& id)
{
	const auto it = _labelToPanel.find(id);
	if (it == _labelToPanel.end()) return;

	auto [panelID, labelIndex] = it->second;
	if (panelID >= _uiPanels.size()) return;
	if (labelIndex < 0 || labelIndex >= _uiPanels[panelID].labels.size()) return;

	auto& label = _uiPanels[panelID].labels[labelIndex];
	label.alive = false;
}

void RenderModule::setUILabelVisible(const uiLabelID& labelID, bool visible)
{
	const auto it = _labelToPanel.find(labelID);
	if (it == _labelToPanel.end()) return;

	auto [panelID, labelIndex] = it->second;
	if (panelID >= _uiPanels.size()) return;
	if (labelIndex < 0 || labelIndex >= _uiPanels[panelID].labels.size()) return;

	_uiPanels[panelID].labels[labelIndex].visible = visible;
}

void RenderModule::setUILabelText(const uiLabelID& labelID, const std::string& text)
{
	const auto it = _labelToPanel.find(labelID);
	if (it == _labelToPanel.end()) return;

	auto [panelID, labelIndex] = it->second;
	if (panelID >= _uiPanels.size()) return;
	if (labelIndex < 0 || labelIndex >= _uiPanels[panelID].labels.size()) return;

	_uiPanels[panelID].labels[labelIndex].text = text;
}

void RenderModule::setUILabelOpacity(const uiLabelID& labelID, float opacity)
{
	const auto it = _labelToPanel.find(labelID);
	if (it == _labelToPanel.end()) return;

	auto [panelID, labelIndex] = it->second;
	if (panelID >= _uiPanels.size()) return;
	if (labelIndex < 0 || labelIndex >= _uiPanels[panelID].labels.size()) return;

	_uiPanels[panelID].labels[labelIndex].opacity = opacity;
}

void RenderModule::setUILabelTextColor(const uiLabelID& labelID, const core::Color& color)
{
	const auto it = _labelToPanel.find(labelID);
	if (it == _labelToPanel.end()) return;

	auto [panelID, labelIndex] = it->second;
	if (panelID >= _uiPanels.size()) return;
	if (labelIndex < 0 || labelIndex >= _uiPanels[panelID].labels.size()) return;

	_uiPanels[panelID].labels[labelIndex].textColor = color;
}

void RenderModule::setUILabelBackGroundColor(const uiLabelID& labelID, const core::Color& color)
{
	const auto it = _labelToPanel.find(labelID);
	if (it == _labelToPanel.end()) return;

	auto [panelID, labelIndex] = it->second;
	if (panelID >= _uiPanels.size()) return;
	if (labelIndex < 0 || labelIndex >= _uiPanels[panelID].labels.size()) return;

	_uiPanels[panelID].labels[labelIndex].bgColor = color;
}

void RenderModule::setUILabelAlign(const uiLabelID& labelID, const TextAlign& align)
{
	const auto it = _labelToPanel.find(labelID);
	if (it == _labelToPanel.end()) return;

	auto [panelID, labelIndex] = it->second;
	if (panelID >= _uiPanels.size()) return;
	if (labelIndex < 0 || labelIndex >= _uiPanels[panelID].labels.size()) return;

	_uiPanels[panelID].labels[labelIndex].align = align;
}

uiButtonID RenderModule::addUIImageButton(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const std::string& textureFolder, const std::string& textureFile, const core::Color& bgColor, const core::Color& hvColor, const core::Color& psColor, const float& opacity)
{
	addUITransform(entityID);

	UIButtonData button;
	button.entity = entityID;
	button.text = text;
	button.visible = true;
	button.textureFolder = textureFolder;
	button.textureFile = textureFile;
	button.hvColor = hvColor;
	button.psColor = psColor;
	button.opacity = opacity;
	button.bgColor = bgColor;

	button.buttonImage = true;
	/*if (!_rgm->resourceGroupExists(textureFolder))
	{
		_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
		_rgm->loadResourceGroup(textureFolder);
		_resourceGroups.insert(textureFolder);
	}*/
	if (!textureFile.empty()) {
		// Obtener la textura ya cargada
		Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName(textureFile, textureFolder);

		// Si no existe, registrar grupo y cargar
		if (!tex) {
			if (!_rgm->resourceGroupExists(textureFolder))
			{
				_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
				_rgm->loadResourceGroup(textureFolder);
				// Solo meter a _resourceGroups si no es precargado
				if (_preloadedGroups.find(textureFolder) == _preloadedGroups.end())
					_resourceGroups.insert(textureFolder);
			}
			tex = Ogre::TextureManager::getSingleton().load(textureFile, textureFolder, Ogre::TEX_TYPE_2D, 0);
		}

		button.textureID = tex ? (ImTextureID)tex->getHandle() : UINT64_MAX;
		if (tex)
		{
			button.texWidth = static_cast<int>(tex->getWidth());
			button.texHeight = static_cast<int>(tex->getHeight());
		}
	}
	else {
		button.textureID = UINT64_MAX;
		button.texWidth = 0;
		button.texHeight = 0;
	}

	_uiPanels[panelID].buttons.push_back(button);

	uiButtonID id = _nextButtonID++;
	int  buttonIndex = _uiPanels[panelID].buttons.size() - 1;
	_buttonToPanel[id] = { panelID, buttonIndex };
	return id;
}

uiButtonID RenderModule::addUIButton(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const float& fontSize, const std::string& fontName, const core::Color& bgColor, const core::Color& txColor, const core::Color& hvColor, const core::Color& psColor, const float& opacity)
{
	addUITransform(entityID);

	UIButtonData button;
	button.entity = entityID;
	button.text = text;
	button.visible = true;
	button.buttonImage = false;
	button.textColor = txColor;
	button.hvColor = hvColor;
	button.bgColor = bgColor;
	button.psColor = psColor;
	button.opacity = opacity;

	//std::string auxFontName = fontName + "_" + std::to_string((int)fontSize);
	std::string auxFontName = std::filesystem::path(fontName).stem().string() + "_" + std::to_string((int)fontSize);
	auto it = _fonts.find(auxFontName);

	if (it != _fonts.end())
	{
		button.font = it->second;
	}
	else
	{
		button.font = _fonts["default"];
	}
	_uiPanels[panelID].buttons.push_back(button);

	uiButtonID id = _nextButtonID++;
	int  buttonIndex = _uiPanels[panelID].buttons.size() - 1;
	_buttonToPanel[id] = { panelID, buttonIndex };
	return id;
}

void RenderModule::deleteUIButton(const uiButtonID& id) {
	auto [panelID, buttonIndex] = _buttonToPanel[id];
	auto& button = _uiPanels[panelID].buttons[buttonIndex];
	button.alive = false;
	button.onClick = nullptr;
}

void RenderModule::setUIButtonVisible(const uiButtonID& buttonID, bool& visible)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].visible = visible;
}

void RenderModule::setUIButtonText(const uiButtonID& buttonID, const std::string& text)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].text = text;
}

void RenderModule::setUIButtonTexture(const uiButtonID& buttonID, const std::string& textureFolder, const std::string& textureFile)
{
	if (textureFile.empty() || textureFolder.empty()) {
		Debug::error("[UIButton] TextureFile/textureFolder vacio");
		return;
	}
	if (!_rgm->resourceGroupExists(textureFolder))
	{
		_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
		_rgm->loadResourceGroup(textureFolder);
		_resourceGroups.insert(textureFolder);
	}
	if (!Ogre::ResourceGroupManager::getSingleton().resourceExists(textureFolder, textureFile))
	{
		Debug::error("[UIButton] Textura no existe");
		return;
	}
	Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().load(textureFile, textureFolder, Ogre::TEX_TYPE_2D, 0);
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].textureFile = textureFile;
	_uiPanels[panelID].buttons[buttonIndex].textureFolder = textureFolder;
	_uiPanels[panelID].buttons[buttonIndex].textureID = (ImTextureID)tex->getHandle();
	_uiPanels[panelID].buttons[buttonIndex].texWidth = static_cast<int>(tex->getWidth());
	_uiPanels[panelID].buttons[buttonIndex].texHeight = static_cast<int>(tex->getHeight());
}

void RenderModule::setUIButtonOpacity(const uiButtonID& buttonID, float& opacity)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].opacity = opacity;
}

void RenderModule::setUIButtonBackgroundColor(const uiButtonID& buttonID, core::Color& bgColor)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].bgColor = bgColor;
}

void RenderModule::setUIButtonTextColor(const uiButtonID& buttonID, core::Color& txColor)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].textColor = txColor;
}

void  RenderModule::setUIButtonHoverColor(const uiButtonID& buttonID, core::Color& hvColor)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].hvColor = hvColor;
}

void  RenderModule::setUIButtonPressColor(const uiButtonID& buttonID, core::Color& psColor)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].psColor = psColor;
}

void RenderModule::setUIButtonDisable(const uiButtonID& buttonID, bool disable)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].disable = disable;
}

void RenderModule::setUIButtonCallback(const uiButtonID& buttonID, std::function<void()> callback)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].onClick = callback;
}

uiTextureRectID RenderModule::addUITextureRect(const uiPanelID& panelID, const entityID& entityID, const std::string& textureFolder, const std::string& textureFile, float& opacity)
{
	addUITransform(entityID);
	UITextureRectData tex;
	tex.entity = entityID;
	tex.textureFolder = textureFolder;
	tex.textureFile = textureFile;
	tex.visible = true;
	tex.opacity = opacity;

	if (!_rgm->resourceGroupExists(textureFolder))
	{
		_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
		_rgm->loadResourceGroup(textureFolder);
		_resourceGroups.insert(textureFolder);
	}
	if (!textureFile.empty()) {
		// Obtener la textura ya cargada
		Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(textureFile, textureFolder);

		// Si no existe, registrar grupo y cargar
		if (!texture) {
			if (!_rgm->resourceGroupExists(textureFolder))
			{
				_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
				_rgm->loadResourceGroup(textureFolder);
				// Solo meter a _resourceGroups si no es precargado
				if (_preloadedGroups.find(textureFolder) == _preloadedGroups.end())
					_resourceGroups.insert(textureFolder);
			}
			texture = Ogre::TextureManager::getSingleton().load(textureFile, textureFolder, Ogre::TEX_TYPE_2D, 0);
		}

		Debug::error("[UIButton] Textura no existe");
		tex.textureID = texture ? (ImTextureID)texture->getHandle() : UINT64_MAX;
		if (texture)
		{
			tex.texWidth = static_cast<int>(texture->getWidth());
			tex.texHeight = static_cast<int>(texture->getHeight());
		}
	}
	else {
		tex.textureID = UINT64_MAX;
		tex.texWidth = 0;
		tex.texHeight = 0;
	}

	_uiPanels[panelID].textureRects.push_back(tex);

	uiTextureRectID id = _nextTextureRectID++;
	int index = _uiPanels[panelID].textureRects.size() - 1;

	_textureToPanel[id] = { panelID, index };

	return id;
}

void RenderModule::deleteUITextureRect(const uiTextureRectID& id) {
	auto [panelID, textureRectIndex] = _textureToPanel[id];
	auto& textureRect = _uiPanels[panelID].textureRects[textureRectIndex];
	textureRect.alive = false;
}

void  RenderModule::setUITextureRectTexture(const uiTextureRectID& textureRectID, const std::string& textureFolder, const std::string& textureFile)
{
	if (textureFile.empty() || textureFolder.empty()) {
		Debug::error("[UITextureRect] TextureFile/textureFolder vacio");
		return;
	}
	if (!_rgm->resourceGroupExists(textureFolder))
	{
		_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
		_rgm->loadResourceGroup(textureFolder);
		_resourceGroups.insert(textureFolder);
	}
	if (!Ogre::ResourceGroupManager::getSingleton().resourceExists(textureFolder, textureFile))
	{
		Debug::error("[UITextureRect] Textura no existe");
		return;
	}
	Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().load(textureFile, textureFolder, Ogre::TEX_TYPE_2D, 0);
	auto [panelID, textureRectIndex] = _textureToPanel[textureRectID];
	_uiPanels[panelID].textureRects[textureRectIndex].textureFile = textureFile;
	_uiPanels[panelID].textureRects[textureRectIndex].textureFolder = textureFolder;
	_uiPanels[panelID].textureRects[textureRectIndex].textureID = (ImTextureID)tex->getHandle();
	_uiPanels[panelID].textureRects[textureRectIndex].texWidth = static_cast<int>(tex->getWidth());
	_uiPanels[panelID].textureRects[textureRectIndex].texHeight = static_cast<int>(tex->getHeight());
}

void RenderModule::setUITextureRectVisible(const uiTextureRectID& textureRectID, bool& visible)
{
	auto [panelID, textureRectIndex] = _textureToPanel[textureRectID];
	_uiPanels[panelID].textureRects[textureRectIndex].visible = visible;
}

void RenderModule::setUITextureRectOpacity(const uiTextureRectID& textureRectID, float& opacity)
{
	auto [panelID, textureRectIndex] = _textureToPanel[textureRectID];
	_uiPanels[panelID].textureRects[textureRectIndex].opacity = opacity;
}

TextAlign RenderModule::stringToAlign(const std::string& align)
{
	if (align == "right") {
		return TextAlign::RIGHT;
	}
	else if (align == "center") {
		return TextAlign::CENTER;
	}
	else {
		return TextAlign::LEFT;
	}
}

void RenderModule::renderUI(const bool& loadingScreen)
{
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	const float baseW = _windowWidth > 0.0f ? _windowWidth : 1.0f;
	const float baseH = _windowHeight > 0.0f ? _windowHeight : 1.0f;
	const float vpW = _vp != nullptr ? _vp->getActualWidth() : baseW;
	const float vpH = _vp != nullptr ? _vp->getActualHeight() : baseH;
	const float vpLeft = _vp != nullptr ? _vp->getActualLeft() : 0.0f;
	const float vpTop = _vp != nullptr ? _vp->getActualTop() : 0.0f;
	const float uiScale = std::min(vpW / baseW, vpH / baseH);
	const float safeScale = uiScale > 0.0f ? uiScale : 1.0f;

	for (UIPanelData& panel : _uiPanels)
	{
		if (!panel.visible || !panel.alive) continue;

		bool isLSPanel = (panel.title == "LSPanel");
		//Descartamos el panel de la pantalla de carga para render normal
		if (!loadingScreen && isLSPanel) continue;
		//Si estamos renderizando pantalla de carga solo queremos su panel
		if (loadingScreen && !isLSPanel) continue;
		

		int tID = getTransformUI(panel.entity);
		const ImVec2 auxDim = { _uiTransforms[tID].dimension.getX() * safeScale, _uiTransforms[tID].dimension.getY() * safeScale };
		const ImVec2 auxPos = { vpLeft + _uiTransforms[tID].position.getX() * safeScale,  vpTop + _uiTransforms[tID].position.getY() * safeScale };
		ImGui::SetNextWindowPos(ImVec2(auxPos));
		ImGui::SetNextWindowSize(ImVec2(auxDim));
		ImGui::Begin(panel.title.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowFontScale(safeScale);

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImDrawListSplitter splitter;
		splitter.Split(drawList, 32);
		splitter.SetCurrentChannel(drawList, _uiTransforms[tID].depthLayer);

		for (UILabelData& label : panel.labels)
		{
			if (!label.visible || !label.alive)
			{
				continue;
			}
			int tID = getTransformUI(label.entity);
			splitter.SetCurrentChannel(drawList, _uiTransforms[tID].depthLayer);

			const ImVec2 auxDim = { _uiTransforms[tID].dimension.getX() * safeScale, _uiTransforms[tID].dimension.getY() * safeScale };
			const ImVec2 auxPos = { vpLeft + _uiTransforms[tID].position.getX() * safeScale,  vpTop + _uiTransforms[tID].position.getY() * safeScale };
			std::string labelName = "label_" + label.entity.toString();
			drawList->AddRectFilled(auxPos, ImVec2(auxPos.x + auxDim.x, auxPos.y + auxDim.y), IM_COL32(label.bgColor.getRed() * 255, label.bgColor.getGreen() * 255, label.bgColor.getBlue() * 255, label.bgColor.getAlpha() * label.opacity * 255));
			ImGui::PushFont(label.font);
			ImVec2 textSize = ImGui::CalcTextSize(label.text.c_str());

			float posTextX;
			float posTextY = auxPos.y + (auxDim.y - textSize.y) * 0.5f;

			switch (label.align)
			{
			case TextAlign::LEFT:
				posTextX = auxPos.x + 5.0f * safeScale;
				break;
			case TextAlign::CENTER:
				posTextX = auxPos.x + (auxDim.x - textSize.x) * 0.5f;
				break;
			case TextAlign::RIGHT:
				posTextX = auxPos.x + auxDim.x - textSize.x - 5.0f * safeScale;
			}

			drawList->AddText(ImVec2(posTextX, posTextY), IM_COL32(label.textColor.getRed() * 255, label.textColor.getGreen() * 255, label.textColor.getBlue() * 255, label.textColor.getAlpha() * label.opacity * 255), label.text.c_str());

			ImGui::PopFont();
		}

		for (UITextureRectData& tex : panel.textureRects)
		{
			if (!tex.visible || !tex.alive || tex.textureID == UINT64_MAX)
			{
				continue;
			}

			int tID = getTransformUI(tex.entity);
			splitter.SetCurrentChannel(drawList, _uiTransforms[tID].depthLayer);

			auto pos = _uiTransforms[tID].position;
			ImGui::SetCursorPos(ImVec2(pos.getX() * safeScale, pos.getY() * safeScale));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tex.opacity);

			const ImVec2 aux = {
				std::max(1.0f, std::round((_uiTransforms[tID].dimension.getX() * safeScale))),
				std::max(1.0f, std::round((_uiTransforms[tID].dimension.getY() * safeScale)))
			};
			ImVec2 uv0(0.0f, 0.0f);
			ImVec2 uv1(1.0f, 1.0f);
			if (tex.texWidth > 1 && tex.texHeight > 1)
			{
				const float uHalfTexel = 0.5f / static_cast<float>(tex.texWidth);
				const float vHalfTexel = 0.5f / static_cast<float>(tex.texHeight);
				uv0 = ImVec2(uHalfTexel, vHalfTexel);
				uv1 = ImVec2(1.0f - uHalfTexel, 1.0f - vHalfTexel);
			}
			ImGui::Image((ImTextureID)tex.textureID, aux, uv0, uv1);
			ImGui::PopStyleVar();
		}

		for (UIButtonData& button : panel.buttons)
		{
			if (!button.visible || !button.alive)
			{
				continue;
			}

			int tID = getTransformUI(button.entity);
			splitter.SetCurrentChannel(drawList, _uiTransforms[tID].depthLayer);

			auto pos = _uiTransforms[tID].position;
			ImGui::SetCursorPos(ImVec2(pos.getX() * safeScale, pos.getY() * safeScale));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, button.opacity);
			const ImVec2 aux = { _uiTransforms[tID].dimension.getX() * safeScale, _uiTransforms[tID].dimension.getY() * safeScale };
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(button.textColor.getRed() * 255, button.textColor.getGreen() * 255, button.textColor.getBlue() * 255, button.textColor.getAlpha() * button.opacity * 255));
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(button.bgColor.getRed() * 255, button.bgColor.getGreen() * 255, button.bgColor.getBlue() * 255, button.bgColor.getAlpha() * button.opacity * 255));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(button.hvColor.getRed() * 255, button.hvColor.getGreen() * 255, button.hvColor.getBlue() * 255, button.hvColor.getAlpha() * button.opacity * 255));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(button.psColor.getRed() * 255, button.psColor.getGreen() * 255, button.psColor.getBlue() * 255, button.psColor.getAlpha() * button.opacity * 255));
			bool click = false;
			if (button.buttonImage)
			{
				if (button.textureID != UINT64_MAX) {
					std::string idButton = button.textureFile + "_" + button.entity.toString();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
					ImVec2 uv0(0.0f, 0.0f);
					ImVec2 uv1(1.0f, 1.0f);
					if (button.texWidth > 1 && button.texHeight > 1)
					{
						const float uHalfTexel = 0.5f / static_cast<float>(button.texWidth);
						const float vHalfTexel = 0.5f / static_cast<float>(button.texHeight);
						uv0 = ImVec2(uHalfTexel, vHalfTexel);
						uv1 = ImVec2(1.0f - uHalfTexel, 1.0f - vHalfTexel);
					}
					click = ImGui::ImageButton(idButton.c_str(), (ImTextureID)(uintptr_t)button.textureID, aux, uv0, uv1);
					ImGui::PopStyleVar();
				}
				else {
					std::string idButton = button.textureFile + "_" + button.entity.toString();
					ImGui::InvisibleButton(idButton.c_str(), ImVec2(1, 1));
				}
			}
			else
			{
				ImGui::PushFont(button.font);
				std::string textID = button.text + "##" + button.entity.toString();
				click = ImGui::Button(textID.c_str(), aux);
				ImGui::PopFont();
			}
			bool hover = ImGui::IsItemHovered();
			bool active = ImGui::IsItemActive();
			ImVec2 min = ImGui::GetItemRectMin();
			ImVec2 max = ImGui::GetItemRectMax();
			if (active) {
				drawList->AddRectFilled(min, max, IM_COL32(button.psColor.getRed() * 255, button.psColor.getGreen() * 255, button.psColor.getBlue() * 255, button.psColor.getAlpha() * button.opacity * 255));
			}
			else if (hover) {
				drawList->AddRectFilled(min, max, IM_COL32(button.hvColor.getRed() * 255, button.hvColor.getGreen() * 255, button.hvColor.getBlue() * 255, button.hvColor.getAlpha() * button.opacity * 255));

			}
			if (click && !button.disable && button.onClick) {
				button.onClick();
			}
			ImGui::PopStyleColor(4);
			ImGui::PopStyleVar();
		}
		splitter.Merge(drawList);

		ImGui::End();
	}

	ImGui::Render();
}

void RenderModule::cleanUI()
{
	_nextUITransformID = 0;
	_nextLabelID = 0;
	_nextButtonID = 0;
	_nextTextureRectID = 0;
	_nextPanelID = 0;
	while (!_uiPanels.empty())
	{
		auto uiT = _uiPanels.back();
		_uiPanels.pop_back();
		uiT.labels.clear();
		uiT.buttons.clear();
		uiT.textureRects.clear();
	}
	_uiTransforms.clear();
	_uiPanels.clear();
	_labelToPanel.clear();
	_buttonToPanel.clear();
	_textureToPanel.clear();
}

void RenderModule::cleanDebug()
{
	if (_debugDraw)
	{
		_debugDraw->clear();
	}

	if (_debugNode && _debugDraw)
	{
		_debugNode->detachObject(_debugDraw);
		_sceneMgr->destroyManualObject(_debugDraw);
		_debugDraw = nullptr;
	}

	if (_debugNode)
	{
		_sceneMgr->destroySceneNode(_debugNode);
		_debugNode = nullptr;
	}
}

bool RenderModule::createLoadingScreenScene(const std::string& bgImageFolder, const std::string& bgImageName, const core::Color& barFill, const std::string& fontName)
{
	try
	{
		if (!_ls.exists)
		{
			//Camara auxiliar
			Ogre::SceneNode* camNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
			_ls.cam = _sceneMgr->createCamera("CameraLS");
			camNode->attachObject(_ls.cam);
			_ls.cam->setAutoAspectRatio(true);
			_ls.cam->setNearClipDistance(0.1f);
			_ls.cam->setFarClipDistance(1000.0f);
		}
		entityID eidPanel = entityID::generate();
		entityID eidBG = entityID::generate();
		entityID eidBarFrame = entityID::generate();
		entityID eidBarFill = entityID::generate();
		entityID eidText = entityID::generate();

		// Panel
		_ls.panelID = addUIPanel(eidPanel, "LSPanel");
		UITransformID tPanel = getTransformUI(eidPanel);
		setUITransformPos(tPanel, { 0.0f, 0.0f });
		setUITransformDimension(tPanel, { 1280.0f, 720.0f });

		//Imagen fondo
		float opacity = 1.0f;
		_ls.bgID = addUITextureRect(_ls.panelID, eidBG, bgImageFolder, bgImageName, opacity);
		UITransformID tBG = getTransformUI(eidBG);
		setUITransformPos(tBG, { 0.0f, 0.0f });
		setUITransformDimension(tBG, { 1280.0f, 720.0f });
		setUITransformDepthLayer(tBG, 1);

		//Barra marco
		_ls.barFrameID = addUILabel(_ls.panelID, eidBarFrame, "", opacity, core::Color(1.0f, 1.0f, 1.0f, 1.0f), core::Color(0.1f, 0.1f, 0.1f, 1.0f), 16.0f, TextAlign::LEFT, "");
		UITransformID tFrame = getTransformUI(eidBarFrame);
		setUITransformPos(tFrame, { 693.0f, 550.0f });
		setUITransformDimension(tFrame, { 400.0f, 27.0f });
		setUITransformDepthLayer(tFrame, 2);

		//Barra relleno
		_ls.barFillID = addUILabel(_ls.panelID, eidBarFill, "", opacity, core::Color(1.0f, 1.0f, 1.0f, 1.0f), barFill, 16.0f, TextAlign::LEFT, "");
		_ls.barFillTID = getTransformUI(eidBarFill);
		setUITransformPos(_ls.barFillTID, { 707.0f, 557.0f });
		setUITransformDimension(_ls.barFillTID, { 0.0f, 13.0f });
		setUITransformDepthLayer(_ls.barFillTID, 3);

		//Label porcentaje
		_ls.textID = addUILabel(_ls.panelID, eidText, "0", opacity, core::Color(1.0f, 1.0f, 1.0f, 1.0f), core::Color(0.0f, 0.0f, 0.0f, 0.0f), 16.0f, TextAlign::RIGHT, fontName);
		UITransformID tPct = getTransformUI(eidText);
		setUITransformPos(tPct, { 1093.0f, 553.0f });
		setUITransformDimension(tPct, { 80.0f, 20.0f });
		setUITransformDepthLayer(tPct, 3);

		_ls.exists = true;
		_ls.bgImageFolder = bgImageFolder;
		_ls.bgImageName = bgImageName;
		_ls.barFill = barFill;
		_ls.fontName = fontName;

		return true;
	}
	catch (const std::exception& e)
	{
		Debug::error("[RenderModule] Error al crear escena de pantalla de carga: ", e.what());
		return false;
	}
}

bool RenderModule::initLoadingScreen()
{
	if (!_ls.exists) return false;
	_ls.currProcedures = 0;
	setUITransformDimension(_ls.barFillTID, { 0.0f, 20.0f });
	return true;
}

bool RenderModule::setLoadingScreenProcedures(const int& n)
{
	if (!_ls.exists) return false;
	if (_ls.currProcedures > 0 && _ls.nProcedures > 0)
	{
		_ls.currProcedures = (_ls.currProcedures * n) / _ls.nProcedures;
	}
	_ls.nProcedures = n;
	return true;
}

void RenderModule::increaseLoadingScreen(const int& n)
{
	if (_ls.exists)
	{
		_ls.currProcedures += n;

		float newPer = static_cast<float>(_ls.currProcedures) / static_cast<float>(_ls.nProcedures);

		setUITransformDimension(_ls.barFillTID, { std::min(373.0f, 373.0f * newPer), 13.0f });
		int finalPer = std::min(100, static_cast<int>(newPer * 100));
		setUILabelText(_ls.textID, std::to_string(finalPer));
	}
}

void RenderModule::shutdown()
{
	if (!_root) return;

	if (_imguiSDLInitialized)
	{
		ImGui_ImplSDL3_Shutdown();
		_imguiSDLInitialized = false;
	}

	if (_sceneMgr && _overlaySystem)
	{
		_sceneMgr->removeRenderQueueListener(_overlaySystem);
		delete _overlaySystem;
		_overlaySystem = nullptr;
		_overlay = nullptr;
	}

	if (_debugDraw)
	{
		if (_debugNode)
		{
			_debugNode->detachObject(_debugDraw);
		}

		_sceneMgr->destroyManualObject(_debugDraw);
		_debugDraw = nullptr;
	}

	if (_debugNode)
	{
		_sceneMgr->destroySceneNode(_debugNode);
		_debugNode = nullptr;
	}

	if (Ogre::RTShader::ShaderGenerator::getSingletonPtr())
	{
		Ogre::RTShader::ShaderGenerator::getSingleton()
			.removeAllShaderBasedTechniques();
		if (_sceneMgr)
			Ogre::RTShader::ShaderGenerator::getSingleton()
			.removeSceneManager(_sceneMgr);
		Ogre::RTShader::ShaderGenerator::destroy();
	}

	cleanScene(true);

	if (_jpgCodec) Ogre::Codec::unregisterCodec(_jpgCodec);
	if (_jpegCodec) Ogre::Codec::unregisterCodec(_jpegCodec);
	if (_pngCodec) Ogre::Codec::unregisterCodec(_pngCodec);
	if (_tgaCodec) Ogre::Codec::unregisterCodec(_tgaCodec);
	if (_bmpCodec) Ogre::Codec::unregisterCodec(_bmpCodec);
	delete _jpgCodec; _jpgCodec = nullptr;
	delete _jpegCodec; _jpegCodec = nullptr;
	delete _pngCodec; _pngCodec = nullptr;
	delete _tgaCodec; _tgaCodec = nullptr;
	delete _bmpCodec; _bmpCodec = nullptr;

	delete _root; _root = nullptr;

	_window = nullptr;
	_sceneMgr = nullptr;

	delete _gl3Plugin; _gl3Plugin = nullptr;
	delete _assimpPlugin; _assimpPlugin = nullptr;
	delete _particlePlugin; _particlePlugin = nullptr;
}

void RenderModule::RenderPhysics(const std::vector<ShapeRenderData>& shapes)
{
	_debugDraw->clear();
	_debugDraw->begin("Debug/PhysicsLines", Ogre::RenderOperation::OT_LINE_LIST);
	Ogre::ColourValue debugColor(1.0f, 0.0f, 0.0f, 0.5f);//rojo
	_debugDraw->colour(debugColor);

	for (const auto& s : shapes)
	{
		switch (s.type)
		{
		case ShapeType::BOX:
			DrawBox(s);
			break;

		case ShapeType::CAPSULE:
			if (s.halfHeight <= 0.0f)
				DrawSphere(s);
			else
				DrawCapsule(s);
			break;
		}
	}
	_debugDraw->end();
}

void RenderModule::DrawBox(const ShapeRenderData& data)
{
	//tam
	Ogre::Vector3 halfSize(data.size.getX(), data.size.getY(), data.size.getZ());
	halfSize *= 0.5f;
	//offset
	Ogre::Vector3 center(data.position.getX(), data.position.getY(), data.position.getZ());
	//rot
	Ogre::Quaternion q(data.rotation.getW(), data.rotation.getX(), data.rotation.getY(), data.rotation.getZ());

	auto transformPoint = [&](const Ogre::Vector3& p)
		{
			return center + (q * p);
		};

	Ogre::Vector3 v[8] = {
		{-halfSize.x, -halfSize.y, -halfSize.z},
		{ halfSize.x, -halfSize.y, -halfSize.z},
		{ halfSize.x, -halfSize.y,  halfSize.z},
		{-halfSize.x, -halfSize.y,  halfSize.z},

		{-halfSize.x,  halfSize.y, -halfSize.z},
		{ halfSize.x,  halfSize.y, -halfSize.z},
		{ halfSize.x,  halfSize.y,  halfSize.z},
		{-halfSize.x,  halfSize.y,  halfSize.z}
	};

	auto line = [&](int a, int b) {
		_debugDraw->position(transformPoint(v[a]));
		_debugDraw->position(transformPoint(v[b]));
		};

	//bottom
	line(0, 1);
	line(1, 2);
	line(2, 3);
	line(3, 0);
	//top
	line(4, 5);
	line(5, 6);
	line(6, 7);
	line(7, 4);
	//lados
	line(0, 4);
	line(1, 5);
	line(2, 6);
	line(3, 7);
}

void RenderModule::DrawCapsule(const ShapeRenderData& data)
{
	const int segments = 32;
	const int rings = 4;

	float r = data.radius;
	float hh = data.halfHeight;

	Ogre::Vector3 center(data.position.getX(), data.position.getY(), data.position.getZ());
	Ogre::Quaternion q(data.rotation.getW(), data.rotation.getX(), data.rotation.getY(), data.rotation.getZ());

	Ogre::Vector3 U = q * Ogre::Vector3::UNIT_Y;
	Ogre::Vector3 V = q * Ogre::Vector3::UNIT_Z;
	Ogre::Vector3 W = q * Ogre::Vector3::UNIT_X;

	auto drawCircle = [&](const Ogre::Vector3& C)
		{
			for (int i = 0; i < segments; i++)
			{
				float a0 = Ogre::Math::TWO_PI * i / segments;
				float a1 = Ogre::Math::TWO_PI * (i + 1) / segments;

				Ogre::Vector3 p0 = C + (cos(a0) * U + sin(a0) * V) * r;
				Ogre::Vector3 p1 = C + (cos(a1) * U + sin(a1) * V) * r;

				_debugDraw->position(p0);
				_debugDraw->position(p1);
			}
		};

	// distribucion de aros por el tronco
	float totalHeight = 2.0f * hh;
	float step = totalHeight / rings;

	for (int i = 0; i <= rings; i++)
	{
		float y = -hh + step * i;
		Ogre::Vector3 C = center + W * y;
		drawCircle(C);
	}

	// para marcar la altura
	Ogre::Vector3 bottom(0, data.position.getY() - r - totalHeight * 0.5f, 0);
	Ogre::Vector3 top(0, data.position.getY() + r + totalHeight * 0.5f, 0);
	_debugDraw->position(bottom);
	_debugDraw->position(top);
}

void RenderModule::DrawSphere(const ShapeRenderData& data)
{
	const int segments = 24; // si tira mucho del ordenador poner 16
	float radius = data.radius;
	Ogre::Vector3 center(data.position.getX(), data.position.getY(), data.position.getZ());
	Ogre::Quaternion q(data.rotation.getW(), data.rotation.getX(), data.rotation.getY(), data.rotation.getZ());

	auto drawCircle = [&](Ogre::Vector3 axis1, Ogre::Vector3 axis2)
		{
			for (int i = 0; i < segments; i++)
			{
				float a0 = Ogre::Math::TWO_PI * i / segments;
				float a1 = Ogre::Math::TWO_PI * (i + 1) / segments;

				Ogre::Vector3 p0 = axis1 * cos(a0) * radius + axis2 * sin(a0) * radius;
				Ogre::Vector3 p1 = axis1 * cos(a1) * radius + axis2 * sin(a1) * radius;

				p0 = center + (q * p0);
				p1 = center + (q * p1);

				_debugDraw->position(p0);
				_debugDraw->position(p1);
			}
		};

	drawCircle(Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y);
	drawCircle(Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Z);
	drawCircle(Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z);
}
