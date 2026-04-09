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
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreLogManager.h>
#include <iostream>
#include <OgreImGuiOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlaySystem.h>
#include <imgui.h>
#include <assimp/postprocess.h>
#include <OgreGL3PlusTexture.h>
#include <guid.h>

#include "GameConfigurator.h"

// RenderModule.cpp : Defines the functions for the static library.
//

static Ogre::Root* _root = nullptr;
static Ogre::OverlaySystem* _overlaySystem = nullptr;
static Ogre::RenderWindow* _window = nullptr;
static Ogre::SceneManager* _sceneMgr = nullptr;
static Ogre::Viewport* _vp = nullptr;
static Ogre::RTShader::ShaderGenerator* _shaderGen;
static Ogre::ResourceGroupManager* _rgm;
static entityID _mainCameraID;

/*void ImGuiManager::AddElement(UIElement element)
{
	_uiElements.push_back(element);
}
    _uiElements.push_back(element);
}*/

/*void ImGuiManager::Clear()
{
    _uiElements.clear();
}*/

/*void ImGuiManager::Draw()
{
    _overlay->NewFrame();
   // ImGui::ShowDemoWindow();
    for (auto e : _uiElements) {
        e();
    }
    ImDrawData* draw_data = ImGui::GetDrawData();
    if (!draw_data || draw_data->CmdListsCount == 0)
        std::cout << "No draw commands generated\n";
    else
        std::cout << "Draw commands generated: " << draw_data->CmdListsCount << "\n";
}*/

RenderModule::~RenderModule()
{
	shutdown();
}

//void ImGuiManager::Init()
//{
   
   // std::cout << "Context: " << ImGui::GetCurrentContext() << std::endl;
    
   // std::cout << "DisplaySize: " << io.DisplaySize.x << ", " << io.DisplaySize.y << std::endl;
//}

bool RenderModule::Init(const HWND handle, const int width, const int height)
{
	try
	{
		_root = new Ogre::Root("", "", "ogre.log");

		Ogre::GL3PlusPlugin* gl3Plugin = new Ogre::GL3PlusPlugin();
		_root->installPlugin(gl3Plugin);

		Ogre::AssimpPlugin* assimpPlugin = new Ogre::AssimpPlugin();
		_root->installPlugin(assimpPlugin);

		Ogre::Codec::registerCodec(new Ogre::STBIImageCodec("jpg"));
		Ogre::Codec::registerCodec(new Ogre::STBIImageCodec("jpeg"));
		Ogre::Codec::registerCodec(new Ogre::STBIImageCodec("png"));
		Ogre::Codec::registerCodec(new Ogre::STBIImageCodec("tga"));
		Ogre::Codec::registerCodec(new Ogre::STBIImageCodec("bmp"));

		Ogre::LogManager::getSingleton().getDefaultLog()->setDebugOutputEnabled(false);

		const Ogre::RenderSystemList& renderers = _root->getAvailableRenderers();
		if (renderers.empty())
			std::cerr << "No hay RenderSystems disponibles" << std::endl;
		Ogre::RenderSystem* rs = renderers[0];
		_root->setRenderSystem(rs);

		_root->initialise(false); // No ventana automatica

		// Crear ventana basica
		Ogre::NameValuePairList params;
		params["externalWindowHandle"] = std::to_string((size_t)handle); // SDL maneja la ventana, solo usamos contexto
		params["FSAA"] = "0";
		params["vsync"] = "true";

		_window = _root->createRenderWindow("OgreWindow", width, height, false, &params);

		//Crear escena con main camera
		_sceneMgr = _root->createSceneManager();

		//entityID zero = ChavalesGUID::generate();
		//addNode(zero, { 0.0f, 5.0f, 15.0f }, { 0.0f, 0.0f, 0.0f, -20.0f }, { 1.0f, 1.0f, 1.0f });
		//addCamera(zero, 45.0f, 0.1f, 1000.0f, 1.0f, { 0.0f, 0.0f, 0.0f, 1.0f });

		//_vp->setBackgroundColour(Ogre::ColourValue(0.02f, 0.22f, 0.11f));

		_nextTransformID = 0;
		_nextUITransformID = 0;
		_nextCameraID = 0;
		_nextModelID = 0;
		_nextAnimationID = 0;
		_nextLightID = 0;

		//Se crea una camara auxiliar para crear el viewport. En el momento que se cree una camara manualmente esta pasara automaticamente a ser la activa.
		_mainCameraID = ChavalesGUID::generate();
		addCamera(_mainCameraID, 45.0f, 0.1f, 1000.0f, 1.0f, { 0.0f, 0.0f, 0.0f, 1.0f });

		//ZONA DEMO INICIO
		_rgm = &Ogre::ResourceGroupManager::getSingleton();

		_rgm->addResourceLocation("../dependencies/ogre/src/ogre/Media/Main", "FileSystem", "Scene");
		_rgm->addResourceLocation("../dependencies/ogre/src/ogre/Media/RTShaderLib", "FileSystem", "Scene");
		
		Ogre::RTShader::ShaderGenerator::initialize();

		_shaderGen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

		_shaderGen->addSceneManager(_sceneMgr);
		_shaderGen->setTargetLanguage("glsl");

		Ogre::MaterialManager::getSingleton().setActiveScheme(
			Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME
		);

		Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

		/*Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual("metroid", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		Ogre::SkeletonPtr skeleton;

		Ogre::AssimpLoader loader;
		Ogre::AssimpLoader::Options opts;
		opts.postProcessSteps =
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_GenNormals;

		bool ok = loader.load("C:/Users/danie/Documents/2526-Grupo03-ChavalesEngine/dependencies/ogre/src/ogre/Samples/Media/packs/metroid-floating/source/metroid_final.fbx", mesh.get(), skeleton, opts);

		if (!ok)
			std::cout << "Error cargando mesh con AssimpLoader\n";

		Ogre::Entity* cube = _sceneMgr->createEntity("metroid", mesh);*/

		/*entityID one = ChavalesGUID::generate();
		addNode(one, core::Vector3(-2.0f, 5.0f, 11.0f), core::Quaternion(0.0f, -0.906f, 0.0f, -0.423f), core::Vector3(0.1f, 0.1f, 0.1f));
		addModel(one, "metroid-floating/source", "metroid_final.fbx");
		setDiffuse(0, 0, "metroid-floating/sourceimages/membrane", "Base_Color.jpeg");
		setTint(0, 0, core::Color(1.0f, 1.0f, 1.0f, 0.7f));
		setDiffuse(0, 1, "metroid-floating/sourceimages/body", "Base_Color.jpeg");
		setDiffuse(0, 2, "metroid-floating/sourceimages/nuclei", "Base_Color.jpeg");
		setDiffuse(0, 10, "metroid-floating/sourceimages/nuclei", "Base_Color.jpeg");
		setDiffuse(0, 6, "metroid-floating/sourceimages/mandibles", "Base_Color.jpeg");*/

		/*Ogre::AnimationStateSet* animSet = _models[0]->getAllAnimationStates();

		if (animSet)
		{
			Ogre::AnimationStateIterator it = animSet->getAnimationStateIterator();
			int i = 0;

			while (it.hasMoreElements())
			{
				Ogre::AnimationState* anim = it.getNext();
				anim = _models[0]->getAnimationState(anim->getAnimationName());
				anim->setEnabled(true);
				anim->setLoop(true);
			}
		}
		else
		{
			std::cout << "No hay animaciones" << std::endl;
		}*/

		//entityID two = ChavalesGUID::generate();
		//addLight(two, 1, core::Color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);

		_overlaySystem = new Ogre::OverlaySystem();
		_sceneMgr->addRenderQueueListener(_overlaySystem);

		_overlay = new Ogre::ImGuiOverlay();
		Ogre::OverlayManager::getSingleton().addOverlay(_overlay);
		_overlay->show();


		Ogre::MaterialPtr materialUI = Ogre::MaterialManager::getSingleton().getByName("ImGui/material");
		_shaderGen->createShaderBasedTechnique(*materialUI, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true);

		_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, materialUI->getName());

		_vp->setOverlaysEnabled(true);

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontDefault();
		io.Fonts->Build();

		io.DisplaySize = ImVec2(
			(float)_vp->getActualWidth(),
			(float)_vp->getActualHeight()
		);

		//_ui->Clear();
	   // _ui->AddElement([]() {
	   //     static bool open = true;
	   //     ImGui::Begin("Test", &open);
	   //     ImGui::Text("SI VES ESTO FUNCIONA");
	   //     ImGui::End();
	   //     });

		/*_ui = new ImGuiManager();
		_ui->Init();
		_ui->Clear();
		_ui->AddElement([]() {
			static bool open = true;
			ImGui::Begin("Test", &open);
			ImGui::Text("SI VES ESTO FUNCIONA");
			ImGui::End();
			});*/

        //renderFrame();

		return true;
	}
	catch (...)
	{
		std::cerr << "Error iniciando OGRE" << std::endl;
		return false;
	}
}

void RenderModule::renderFrame()
{
	// if (_ui)
	 //    _ui->Draw();

	renderUI();
	_root->renderOneFrame();
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

	//Limpiar luces
	cleanLights();

	//Limpiar camaras
	cleanCameras();

	//Limpiar nodos
	/*for (const EngineNode& engineNode : _engineNodes)
	{
		Ogre::SceneNode* sceneNode = engineNode.sceneNode;
		if (sceneNode != nullptr)
		{
			_sceneMgr->destroySceneNode(sceneNode);
		}
	}*/
	// limpia toda la escena de Ogre de golpe
	_sceneMgr->clearScene();
	_engineNodes.clear();
	_nextTransformID = 0;
	_nextUITransformID = 0;
	//_ui->Clear();

	// Esto filtra los grupos que se borran para que no se borren los grupos basicos de ogre y que no pete
	static const std::vector<std::string> internalGroups = {
		"Scene",
	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	   Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME,
		Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME
	};


	Ogre::StringVector groups = _rgm->getResourceGroups();

	for (const std::string& groupName : groups)
	{
		bool isInternal = false;
		for (const auto& ig : internalGroups)
			if (groupName == ig) { isInternal = true; break; }

		if (!isInternal)
		{
			// Liberar modelos y textruas
			_rgm->unloadResourceGroup(groupName);

			// Limpiar lista
			_rgm->clearResourceGroup(groupName);

			// Borrar grupo
			_rgm->destroyResourceGroup(groupName);
		}
	}

	_shaderGen->removeAllShaderBasedTechniques();
	_shaderGen->flushShaderCache();

	//Si se va a crear una escena nueva dejamos una camara de seguridad. Volvemos a anadir rtss a imgui.
	if (!end)
	{
		addCamera(_mainCameraID, 45.0f, 0.1f, 1000.0f, 1.0f, { 0.0f, 0.0f, 0.0f, 1.0f });
		Ogre::MaterialPtr materialUI = Ogre::MaterialManager::getSingleton().getByName("ImGui/material");
		_shaderGen->createShaderBasedTechnique(*materialUI, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true);

		_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, materialUI->getName());
	}
}



transformID RenderModule::addNode(const entityID& entityID, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float> scale, const bool& fromTransform,const TransformType type)
{
	if (type == TransformType::WORLD) {
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
				return i; //Ya existe.
			}
		}

		//bool newNode = false;
		//EngineNode* aux = nullptr;
		// Crear nuevo nodo
		EngineNode& aux = _engineNodes.emplace_back(_sceneMgr->getRootSceneNode()->createChildSceneNode(), entityID);
		aux.sceneNode->setPosition(Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()));
		aux.sceneNode->setOrientation(Ogre::Quaternion(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));
		aux.sceneNode->setScale(Ogre::Vector3(scale.getX(), scale.getY(), scale.getZ()));
		return _nextTransformID++;
	}
	else {
		for (int i = 0; i < (int)_uiTransforms.size(); i++) {
			if (_uiTransforms[i].entity == entityID) {
				_uiTransforms[i].position = { pos.getX(), pos.getY() };
				return i;
			}
		}
		UITransform uiT;
		uiT.entity = entityID;
		uiT.position = { pos.getX(), pos.getY() };
		_uiTransforms.push_back(uiT);
		return _nextUITransformID++;
		

	}
	
}

transformID RenderModule::addNode(const entityID& entityID, const TransformType type)
{
	return addNode(entityID, core::Vector3<float>(0.0f, 0.0f, 0.0f), core::Quaternion<float>(0.0f, 0.0f, 0.0f, 1.0f), core::Vector3<float>(1.0f, 1.0f, 1.0f), false, type);
}

transformID RenderModule::getNode(const entityID& entityID)
{
	for (int i = _engineNodes.size() - 1; i >= 0; i--)
	{
		if (_engineNodes[i].nodeID == entityID) return i;
	}
	return -1;
}

core::Vector3<float> RenderModule::getNodePosition(const transformID& id)
{
	if (id >= 0 && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		Ogre::Vector3 pos = _engineNodes[id].sceneNode->getPosition();
		return core::Vector3<float>(pos.x, pos.y, pos.z);
	}
	else return core::Vector3<float>(0.0f, 0.0f, 0.0f);
}

void RenderModule::setNodePosition(const transformID& id, const core::Vector3<float>& pos)
{
	if (id >= 0 && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		_engineNodes[id].sceneNode->setPosition(pos.getX(), pos.getY(), pos.getZ());
	}
}

core::Quaternion<float> RenderModule::getNodeRotation(const transformID& id)
{
	if (id >= 0 && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		Ogre::Quaternion rot = _engineNodes[id].sceneNode->getOrientation();
		return core::Quaternion<float>(rot.x, rot.y, rot.z, rot.w);
	}
	else return core::Quaternion<float>(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderModule::setNodeRotation(const transformID& id, const core::Quaternion<float>& rot)
{
	if (id >= 0 && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		_engineNodes[id].sceneNode->setOrientation(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
	}
}

core::Vector3<float> RenderModule::getNodeScale(const transformID& id)
{
	if (id >= 0 && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		Ogre::Vector3 pos = _engineNodes[id].sceneNode->getScale();
		return core::Vector3<float>(pos.x, pos.y, pos.z);
	}
	else return core::Vector3<float>(0.0f, 0.0f, 0.0f);
}

void RenderModule::setNodeScale(const transformID& id, const core::Vector3<float>& scale)
{
	if (id >= 0 && id < _engineNodes.size() && _engineNodes[id].sceneNode != nullptr)
	{
		_engineNodes[id].sceneNode->setPosition(scale.getX(), scale.getY(), scale.getZ());
	}
}

UITransformID RenderModule::getTransformUI(const entityID& entityID)
{
	for (int i = _uiTransforms.size() - 1; i >= 0; i--) {
		if (_uiTransforms[i].entity == entityID) return i;

	}
	return -1;
}

core::Vector2<float> RenderModule::getUIPosition(const transformID& id)
{
	if (id >= 0 && id < _uiTransforms.size()) {
		return _uiTransforms[id].position;
	}
	return core::Vector2<float>(0,0);
}

void RenderModule::setUIPosition(const transformID& id, const core::Vector2<float>& pos)
{
	if (id >= 0 && id < _uiTransforms.size()) {
		 _uiTransforms[id].position = pos;
	}
}



void RenderModule::setViewportBGColor(core::Color color)
{
	_vp->setBackgroundColour(Ogre::ColourValue(color.getRed(), color.getGreen(), color.getBlue()));
}

cameraID RenderModule::addCamera(const entityID& entityID, const float& FOVy, const float& nearClipDistance, const float& farClipDistance, const float& focalLength, const core::Color& bgColor)
{
	//Si no existe un nodo con este entityID lo creamos
	transformID nodeID = addNode(entityID, TransformType::WORLD);
	Ogre::Camera* camera = _cameras.emplace_back(_sceneMgr->createCamera("camera" + entityID.toString()));
	camera->setAutoAspectRatio(true);
	_engineNodes[nodeID].sceneNode->attachObject(camera);

	camera->setFOVy(Ogre::Radian(FOVy));
	camera->setNearClipDistance(nearClipDistance);
	camera->setFarClipDistance(farClipDistance);
	camera->setFocalLength(focalLength);

	//Si es la main camera auxiliar o es la primera camara manual se convierte automaticamente en la activa
	if (_nextCameraID <= 1)
	{
		setAsActiveCamera(_nextCameraID);
		_vp->setBackgroundColour(Ogre::ColourValue(bgColor.getRed(), bgColor.getGreen(), bgColor.getBlue()));
	}
	return _nextCameraID++;
}

void RenderModule::deleteCamera(const cameraID& id)
{
	if (id >= 0 && id < _cameras.size() && _cameras[id] != nullptr)
	{
		Ogre::Camera* cam = _cameras[id];
		//Desvinculamos del viewport en caso de actividad
		if (_vp->getCamera() == cam) _vp->setCamera(nullptr);
		Ogre::SceneNode* parent = cam->getParentSceneNode();
		if (parent) parent->detachObject(cam);
		_sceneMgr->destroyCamera(cam);
		_cameras.erase(_cameras.begin() + id);
	}
	/*if (id <= 0 || id >= (int)_cameraNodes.size()) return;

	Ogre::SceneNode* node = _cameraNodes[id];
	Ogre::Camera* cam = _cameras[id];

	if (!node || !cam) return;

	node->detachObject(cam);
	_sceneMgr->destroyCamera(cam);
	_sceneMgr->destroySceneNode(node);

	_cameraNodes.erase(_cameraNodes.begin() + id);
	_cameras.erase(_cameras.begin() + id);*/
}

void RenderModule::setAsActiveCamera(const cameraID& id)
{
	if (id >= 0 && id < _cameras.size() && _cameras[id] != nullptr)
	{
		if (_vp == nullptr)
		{
			_vp = _window->addViewport(_cameras[id]);
		}
		else
		{
			_vp->setCamera(_cameras[id]);
		}
	}
}

void RenderModule::cleanCameras()
{
	for (Ogre::Camera* cam : _cameras)
	{
		if (cam != nullptr)
		{
			Ogre::SceneNode* parent = cam->getParentSceneNode();
			if (parent)
				parent->detachObject(cam);

			_sceneMgr->destroyCamera(cam);
		}
	}
	_cameras.clear();
	_nextCameraID = 0;
	if (_vp)
		_vp->setCamera(nullptr);
	/*if (_cameraNodes.empty()) return;

	Ogre::SceneNode* mainNode = _cameraNodes[0];
	Ogre::Camera* mainCam = _cameras[0];
	mainCam->setf

	for (size_t i = 1; i < _cameraNodes.size(); ++i)
	{
		Ogre::SceneNode* node = _cameraNodes[i];
		Ogre::Camera* cam = _cameras[i];

		if (!node || !cam) continue;

		node->detachObject(cam);
		_sceneMgr->destroyCamera(cam);
		_sceneMgr->destroySceneNode(node);
	}

	_cameraNodes.clear();
	_cameras.clear();

	_cameraNodes.push_back(mainNode);
	_cameras.push_back(mainCam);

	//Reset camara principal
	mainNode->setPosition(0, 0, 80);
	mainNode->setOrientation(Ogre::Quaternion::IDENTITY);*/
}

void RenderModule::setCameraFOVy(const cameraID& id, const float& FOVy)
{
	if (id >= 0 && id < _cameras.size() && _cameras[id] != nullptr) _cameras[id]->setFOVy(Ogre::Radian(FOVy));
}

void RenderModule::setCameraNearClipDistance(const cameraID& id, const float& nearClipDistance)
{
	if (id >= 0 && id < _cameras.size() && _cameras[id] != nullptr) _cameras[id]->setNearClipDistance(nearClipDistance);
}

void RenderModule::setCameraFarClipDistance(const cameraID& id, const float& farClipDistance)
{
	if (id >= 0 && id < _cameras.size() && _cameras[id] != nullptr) _cameras[id]->setFarClipDistance(farClipDistance);
}

void RenderModule::setCameraFocalLength(const cameraID& id, const float& focalLength)
{
	if (id >= 0 && id < _cameras.size() && _cameras[id] != nullptr) _cameras[id]->setFocalLength(focalLength);
}



modelID RenderModule::addModel(const entityID& entityID, const std::string& modelFolder, const std::string& modelFile)
{
	transformID nodeID = addNode(entityID, TransformType::WORLD);

	if (!_rgm->resourceGroupExists(modelFolder))
	{
		_rgm->addResourceLocation(modelFolder, "FileSystem", modelFolder);
		_rgm->loadResourceGroup(modelFolder);
	}
	Ogre::Entity* model = _models.emplace_back(_sceneMgr->createEntity(modelFile + std::to_string(_nextModelID), modelFile));
	_engineNodes[nodeID].sceneNode->attachObject(model);

	for (unsigned int i = 0; i < model->getNumSubEntities(); ++i)
	{
		Ogre::SubEntity* sub = model->getSubEntity(i);
		Ogre::MaterialPtr mat = sub->getMaterial();

		mat->load();

		// Generar tecnica RTSS sobre el material ya cargado
		_shaderGen->createShaderBasedTechnique(*mat, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true);

		_shaderGen->invalidateMaterial(
			Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
			mat->getName(), mat->getGroup());

		if (!_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, mat->getName()))
			Debug::error("[RenderModule] validateMaterial");
	}

	return _nextModelID++;
}

void RenderModule::deleteModel(const modelID& id)
{
	if (id >= 0 && id < _models.size() && _models[id] != nullptr)
	{
		Ogre::Entity* model = _models[id];
		Ogre::SceneNode* parent = model->getParentSceneNode();
		parent->detachObject(model);
		_sceneMgr->destroyEntity(model);
		//_sceneMgr->destroySceneNode(parent);
		_models.erase(_models.begin() + id);
	}
}

void RenderModule::cleanModels()
{
	for (Ogre::Entity* model : _models)
	{
		if (model != nullptr)
		{
			Ogre::SceneNode* parent = model->getParentSceneNode();
			if (parent)
				parent->detachObject(model);

			_sceneMgr->destroyEntity(model);
		}
	}

	_models.clear();
	_nextModelID = 0;
}

void RenderModule::setDiffuse(const modelID& id, const subMeshID& subID, const std::string& textureFolder, const std::string& textureFile)
{
	if (id >= 0 && id < _models.size() && _models[id] != nullptr)
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
		}
		Ogre::TexturePtr text = Ogre::TextureManager::getSingleton().load(textureFile, textureFolder, Ogre::TEX_TYPE_2D, 0);

		Ogre::TextureUnitState* tus = pass->createTextureUnitState();
		tus->setTexture(text);
		tus->setColourOperation(Ogre::LBO_MODULATE);
	}
}

void RenderModule::setTint(const modelID& id, const subMeshID& subID, const core::Color& tint)
{
	if (id >= 0 && id < _models.size() && _models[id] != nullptr)
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

		pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
		pass->setDepthWriteEnabled(false);

		pass->setDiffuse(tint.getRed(), tint.getGreen(), tint.getBlue(), tint.getAlpha());
	}
}

void RenderModule::setModelVisible(const modelID& id, const bool& visible)
{
	if (id >= 0 && id < _models.size() && _models[id] != nullptr)
	{
		Ogre::Entity* model = _models[id];
		Ogre::SceneNode* node = model->getParentSceneNode();
		if (node)
			node->setVisible(visible, false);
	}

}



void RenderModule::addAnimator(const entityID& entityID, modelID& modelID)
{
	transformID nodeID = addNode(entityID, TransformType::WORLD);
	modelID = -1;
	auto& node = _engineNodes[nodeID].sceneNode;
	for (unsigned int i = 0; i < node->numAttachedObjects(); ++i)
	{
		Ogre::MovableObject* obj = node->getAttachedObject(i);
		Ogre::Entity* ent = dynamic_cast<Ogre::Entity*>(obj);
		if (ent)
		{
			modelID = _nextModelID - 1;
			return;
		}
	}
}

void RenderModule::cleanAnimations()
{
	for (Ogre::AnimationState* state : _animations)
	{
		if (state != nullptr)
		{
			Ogre::String name = state->getAnimationName();
			state->setEnabled(false);

			//Destruir solo si es transform animation. Las de esqueleto se borran junto a la entidad.
			if (_sceneMgr->hasAnimation(name))
			{
				_sceneMgr->destroyAnimationState(name);
				_sceneMgr->destroyAnimation(name);
			}
		}
	}

	_animations.clear();
	_nextAnimationID = 0;
}

animationID RenderModule::registerSkeletonAnim(const modelID& modelID, const std::string& animationName, const bool& loop)
{
	if (modelID >= 0 && modelID < _models.size() && _models[modelID] != nullptr)
	{
		auto anim = _animations.emplace_back(_models[modelID]->getAnimationState(animationName));
		if (anim == nullptr)
			return -1;
		_animations.back()->setLoop(loop);
		return _nextAnimationID++;
	}
	return -1;
}

animationID RenderModule::createTransformAnimation(const entityID& entityID, const std::string& animationName, const bool& loop, const float& totalDuration)
{
	transformID nodeID = getNode(entityID);
	if (nodeID != -1 && _engineNodes[nodeID].sceneNode != nullptr)
	{
		Ogre::Animation* animation = _sceneMgr->createAnimation(animationName + std::to_string(_nextAnimationID), totalDuration);
		animation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
		animation->createNodeTrack(0, _engineNodes[nodeID].sceneNode);
		_animations.emplace_back(_sceneMgr->createAnimationState(animationName + std::to_string(_nextAnimationID)));
		_animations.back()->setLoop(loop);
		return _nextAnimationID++;
	}
	return -1;
}

void RenderModule::addTransformKeyFrame(const animationID& animationID, const float& timePos, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale)
{
	if (animationID >= 0 && animationID < _animations.size() && _animations[animationID] != nullptr)
	{
		Ogre::Animation* anim = _sceneMgr->getAnimation(_animations[animationID]->getAnimationName());

		Ogre::NodeAnimationTrack* track = anim->getNodeTrack(0);

		Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(timePos);
		kf->setTranslate(Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()));
		kf->setRotation(Ogre::Quaternion(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));
		kf->setScale(Ogre::Vector3(scale.getX(), scale.getY(), scale.getZ()));
	}
}

void RenderModule::addTransformKeyFrame(const animationID& animationID, const float& timePos, const core::Vector3<float>& pos, const float& rot, const int& axis, const core::Vector3<float>& scale)
{
	if (animationID >= 0 && animationID < _animations.size() && _animations[animationID] != nullptr)
	{
		Ogre::Animation* anim = _sceneMgr->getAnimation(_animations[animationID]->getAnimationName());

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
	if (animationID >= 0 && animationID < _animations.size() && _animations[animationID] != nullptr)
	{
		_animations[animationID]->setEnabled(active);
	}
}

void RenderModule::setAnimTimePos(const animationID& animationID, const float& timePos)
{
	if (animationID >= 0 && animationID < _animations.size() && _animations[animationID] != nullptr)
	{
		_animations[animationID]->setTimePosition(timePos);
	}
}

void RenderModule::updateAnimation(const animationID& animationID, const uint64_t& deltaTime)
{
	if (animationID >= 0 && animationID < _animations.size() && _animations[animationID] != nullptr)
	{
		_animations[animationID]->addTime((float)deltaTime / 1000.0f);
	}
}



lightID RenderModule::addLight(const entityID& entityID, const int& type, const core::Color& color, const float& intensity) {
	//Si no existe un nodo con este entityID lo creamos
	transformID nodeID = addNode(entityID, TransformType::WORLD);

	Ogre::Light* light = _sceneMgr->createLight("light" + std::to_string(_nextLightID));

	switch (type) {
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
void  RenderModule::deleteLight(const lightID& id) {
	if (id >= 0 && id < _lights.size() && _lights[id] != nullptr)
	{
		Ogre::Light* light = _lights[id];
		Ogre::SceneNode* parent = light->getParentSceneNode();
		if (parent) parent->detachObject(light);
		_sceneMgr->destroyLight(light);
		_lights.erase(_lights.begin() + id);
	}
}

void RenderModule::setLightActive(const lightID& id, const bool& active) {
	if (id >= 0 && id < _lights.size() && _lights[id] != nullptr) _lights[id]->setVisible(active);
}

void RenderModule::cleanLights() {
	for (Ogre::Light* light : _lights)
	{
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

void RenderModule::setLightType(const lightID& id, const int& type) {
	if (id >= 0 && id < _lights.size() && _lights[id] != nullptr)
	{
		Ogre::Light* light = _lights[id];
		switch (type) {
		case 0: light->setType(Ogre::Light::LT_POINT); break;
		case 1: light->setType(Ogre::Light::LT_DIRECTIONAL); break;
		case 2: light->setType(Ogre::Light::LT_SPOTLIGHT); break;
		case 3: light->setType(Ogre::Light::LT_RECTLIGHT); break;
		}
	}
}

void RenderModule::setLightColor(const lightID& id, const core::Color& color) {
	if (id >= 0 && id < _lights.size() && _lights[id] != nullptr) _lights[id]->setDiffuseColour(color.getRed(), color.getGreen(), color.getBlue());
}

void RenderModule::setLightIntensity(const lightID& id, const float& intensity) {
	if (id >= 0 && id < _lights.size() && _lights[id] != nullptr) _lights[id]->setPowerScale(intensity);
}

void RenderModule::setLightSpotRange(const lightID& id, const float& inner, const float& outer, const float& falloff) {
	if (id >= 0 && id < _lights.size() && _lights[id] != nullptr) _lights[id]->setSpotlightRange(Ogre::Degree(inner), Ogre::Degree(outer), falloff);
}



particleGenID RenderModule::addParticleGen(const entityID& entityID, const std::string& textureFolder, const std::string& textureFile)
{
	addNode(entityID, TransformType::WORLD);

	std::string matName = "ParticleMat_" + std::to_string(_nextParticleGenID);

	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(matName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	mat->setReceiveShadows(false);

	Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
	pass->setLightingEnabled(false);
	pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	pass->setDepthWriteEnabled(false);

	if (!_rgm->resourceGroupExists(textureFolder))
	{
		_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
		_rgm->loadResourceGroup(textureFolder);
	}

	Ogre::TexturePtr text = Ogre::TextureManager::getSingleton().load(textureFile, textureFolder, Ogre::TEX_TYPE_2D, 0);

	Ogre::TextureUnitState* tus = pass->createTextureUnitState();
	tus->setTexture(text);
	tus->setColourOperation(Ogre::LBO_MODULATE);

	mat->load();

	//Asignar RTSS
	_shaderGen->createShaderBasedTechnique(*mat, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true);
	_shaderGen->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, mat->getName());

	Ogre::ParticleSystem* ps = _particleGens.emplace_back(_sceneMgr->createParticleSystem("ParticleGen_" + std::to_string(_nextParticleGenID)));

	ps->setMaterialName(matName);
	ps->addEmitter("Point");
	_engineNodes.back().sceneNode->attachObject(ps);

	return _nextParticleGenID++;
}

void RenderModule::deleteParticleGen(const particleGenID& id)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
	{
		Ogre::ParticleSystem* ps = _particleGens[id];

		Ogre::SceneNode* parent = ps->getParentSceneNode();
		if (parent)
			parent->detachObject(ps);

		_sceneMgr->destroyParticleSystem(ps);

		_particleGens.erase(_particleGens.begin() + id);
	}
}

void RenderModule::cleanParticleGens()
{
	for (Ogre::ParticleSystem* ps : _particleGens)
	{
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
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setEnabled(enabled);
}

void RenderModule::setParticleGenEmitting(const particleGenID& id, const bool& emitting)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setEnabled(emitting);
}

void RenderModule::setParticleGenQuota(const particleGenID& id, const float& quota)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->setParticleQuota(static_cast<size_t>(quota));
}

void RenderModule::setParticleGenEmissionRate(const particleGenID& id, const float& rate)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setEmissionRate(rate);
}

void RenderModule::setParticleGenDuration(const particleGenID& id, const float& duration)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setDuration(duration);
}

void RenderModule::setParticleGenTimeToLive(const particleGenID& id, const float& time)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setTimeToLive(time);
}

void RenderModule::setParticleGenVelocity(const particleGenID& id, const float& velocity)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setParticleVelocity(velocity);
}

void RenderModule::setParticleGenMinVelocity(const particleGenID& id, const float& velocity)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setMinParticleVelocity(velocity);
}

void RenderModule::setParticleGenMaxVelocity(const particleGenID& id, const float& velocity)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setMaxParticleVelocity(velocity);
}

void RenderModule::setParticleGenDirection(const particleGenID& id, const core::Vector3<float>& direction)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setDirection(Ogre::Vector3(direction.getX(), direction.getY(), direction.getZ())
		);
}

void RenderModule::setParticleGenAngle(const particleGenID& id, const float& angle)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setAngle(Ogre::Degree(angle));
}

void RenderModule::setParticleGenPartWidth(const particleGenID& id, const float& width)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->setDefaultWidth(width);
}

void RenderModule::setParticleGenPartHeight(const particleGenID& id, const float& height)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->setDefaultHeight(height);
}

void RenderModule::setParticleGenPartColor(const particleGenID& id, const core::Color& color)
{
	if (id >= 0 && id < _particleGens.size() && _particleGens[id] != nullptr)
		_particleGens[id]->getEmitter(0)->setColour(Ogre::ColourValue(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));
}




uiPanelID RenderModule::addUIPanel(const entityID& entityID, const std::string& title) {

	UIPanelData panel;
	panel.entity = entityID;
	panel.title = title;
	panel.visible = true;

	_uiPanels.push_back(panel);

	return _nextPanelID++;
}
void RenderModule::setUIPanelVisible(const uiPanelID& id, bool visible) {
	_uiPanels[id].visible = visible;

}
uiLabelID RenderModule::addUILabel(const std::string& panelName, const entityID& entityID, const std::string& text,const  float opacity,const  core::Vector2<float> size, const core::Color textColor,const core::Color bgColor,const float fontSize,const TextAlign textAlign, const std::string fontFolder, const std::string fontFile) {
	addNode(entityID, TransformType::UI);

	uiPanelID panelID = getOrSetPanel(panelName);
	UILabelData label;
	label.entity = entityID;
	label.text = text;
	label.visible = true;
	label.opacity = opacity;
	label.size = size;
	label.textColor = textColor;
	label.bgColor = bgColor;
	label.fontSize = fontSize;
	label.align = textAlign;
	auto& io{ ImGui::GetIO() };

	ImFont* fontAux = io.Fonts->AddFontFromFileTTF((fontFolder + fontFile).c_str(), fontSize);
	io.Fonts->Build();
	label.font = fontAux;
	
	_uiPanels[panelID].labels.push_back(label);

	uiLabelID id = _nextLabelID++;
	int  labelIndex = _uiPanels[panelID].labels.size() - 1;
	_labelToPanel[id] = { panelID, labelIndex };
	return id;
		

}



void RenderModule::setUILabelVisible(const uiLabelID& labelID, bool visible) {
	auto [panelID, labelIndex] = _labelToPanel[labelID];
	_uiPanels[panelID].labels[labelIndex].visible = visible;
}


void RenderModule::setUILabelText(const uiLabelID& labelID, const std::string& text) {
	auto [panelID, labelIndex] = _labelToPanel[labelID];
	_uiPanels[panelID].labels[labelIndex].text = text;
}
void  RenderModule::setUILabelOpacity(const uiLabelID& labelID, float opacity) {
	auto [panelID, labelIndex] = _labelToPanel[labelID];
	_uiPanels[panelID].labels[labelIndex].opacity = opacity;
}

void RenderModule::setUILabelDimension(const uiLabelID& labelID, core::Vector2<float> dimension) {
	auto [panelID, labelIndex] = _labelToPanel[labelID];
	_uiPanels[panelID].labels[labelIndex].size = dimension;
}


void RenderModule::setUILabelTextColor(const uiLabelID labelID, core::Color color) {
	auto [panelID, labelIndex] = _labelToPanel[labelID];
	_uiPanels[panelID].labels[labelIndex].textColor = color;
}
void RenderModule::setUILabelBackGroundColor(const uiLabelID labelID, core::Color color) {
	auto [panelID, labelIndex] = _labelToPanel[labelID];
	_uiPanels[panelID].labels[labelIndex].bgColor = color;
}
void RenderModule::setUILabelAlign(const uiLabelID labelID, const std::string& align) {
	auto [panelID, labelIndex] = _labelToPanel[labelID];
	_uiPanels[panelID].labels[labelIndex].align = stringToAlign(align);
}
//void RenderModule::setUILabelFont(const uiLabelID id, ImFont* font) {
//
//}



uiButtonID RenderModule::addUIButton(const std::string& panelName, const entityID& entityID, const std::string& text, const std::string& textureFolder, const std::string& textureFile, core::Vector2<float> size)
{
	addNode(entityID, TransformType::UI);

	uiPanelID panelID = getOrSetPanel(panelName);
	UIButtonData button;
	button.entity = entityID;
	button.text = text;
	button.visible = true;
	button.size = size;
	if (!textureFile.empty()) {
		button.buttonImage = true;
		if (!_rgm->resourceGroupExists(textureFolder))
		{
			_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
			_rgm->loadResourceGroup(textureFolder);
		}
		Ogre::TexturePtr ogreTexture = Ogre::TextureManager::getSingleton().load(textureFile, textureFolder);
		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = ogreTexture->getBuffer();
		pixelBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
		Ogre::GL3PlusTexture* glTexture = static_cast<Ogre::GL3PlusTexture*>(ogreTexture.get());
		GLuint texID = glTexture->getGLID();
		pixelBuffer->unlock();
		button.textureID = (ImTextureID)(uintptr_t)texID;
	}
	else {
		button.buttonImage = false;

	}

	_uiPanels[panelID].buttons.push_back(button);

	uiButtonID id = _nextButtonID++;
	int  buttonIndex = _uiPanels[panelID].buttons.size() - 1;
	_buttonToPanel[id] = { panelID, buttonIndex };
	return id;
		
}

void RenderModule::setUIButtonVisible(const uiButtonID& buttonID, bool visible)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].visible = visible;

}

void RenderModule::setUIButtonText(const uiButtonID& buttonID, const std::string& text)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].text = text;
}
void  RenderModule::setUIButtonTexture(const uiButtonID& buttonID, const std::string& texture) {
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].textureFile = texture;
}

void  RenderModule::setUIButtonDimension(const uiButtonID& buttonID, core::Vector2<float> dimension) {
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].size = dimension;
}
void  RenderModule::setUIButtonOpacity(const uiButtonID& buttonID, float opacity) {
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].opacity = opacity;
}

void RenderModule::setUIButtonCallback(const uiButtonID& buttonID, std::function<void()> callback)
{
	auto [panelID, buttonIndex] = _buttonToPanel[buttonID];
	_uiPanels[panelID].buttons[buttonIndex].onClick = callback;
}



uiTextureRectID RenderModule::addUITextureRect(const std::string& panelName, const entityID& entityID, const std::string& textureFolder, const std::string& textureFile, core::Vector2<float> size) {
	addNode(entityID, TransformType::UI);
	uiPanelID panelID = getOrSetPanel(panelName);
	UITextureRectData tex;
	tex.entity = entityID;
	tex.textureFolder = textureFolder;
	tex.textureFile = textureFile;
	tex.visible = true;
	tex.size = size;
	if (!_rgm->resourceGroupExists(textureFolder))
	{
		_rgm->addResourceLocation(textureFolder, "FileSystem", textureFolder);
		_rgm->loadResourceGroup(textureFolder);
	}

	_uiPanels[panelID].textureRects.push_back(tex);

	uiTextureRectID id = _nextTextureRectID++;
	int index = _uiPanels[panelID].textureRects.size() - 1;

	_textureToPanel[id] = { panelID, index };

	return id;	
}
void  RenderModule::setUITextureRectTexture(const uiTextureRectID& textureRectID, const std::string& texture) {
	auto [panelID, textureRectIndex] = _textureToPanel[textureRectID];
	_uiPanels[panelID].textureRects[textureRectIndex].textureFile = texture;
}
void  RenderModule::setUITextureRectDimension(const uiTextureRectID& textureRectID, core::Vector2<float> dimension) {
	auto [panelID, textureRectIndex] = _textureToPanel[textureRectID];
	_uiPanels[panelID].textureRects[textureRectIndex].size = dimension;
}
void  RenderModule::setUITextureRectVisible(const uiTextureRectID& textureRectID, bool visible) {
	auto [panelID, textureRectIndex] = _textureToPanel[textureRectID];
	_uiPanels[panelID].textureRects[textureRectIndex].visible = visible;
}
void  RenderModule::setUITextureRectOpacity(const uiTextureRectID& textureRectID, float opacity) {
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

uiPanelID RenderModule::getOrSetPanel(const std::string& panelName)
{
	for (int i = 0; i < _uiPanels.size(); i++) {
		if (_uiPanels[i].title == panelName) {
			return i;
		}
	}
	UIPanelData panel;
	panel.title = panelName;
	panel.visible = true;
	_uiPanels.push_back(panel);
	return _uiPanels.size() -1;
}


void RenderModule::renderUI() {
	_overlay->NewFrame();
	for (UIPanelData& panel : _uiPanels) {

		if (!panel.visible) {
			continue;
		}
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::Begin(panel.title.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

		for (UITextureRectData& tex : panel.textureRects) {
			if (!tex.visible) {
				continue;
			}
			int tID = getTransformUI(tex.entity);
			auto pos = _uiTransforms[tID].position;
			ImGui::SetCursorPos(ImVec2(pos.getX(), pos.getY()));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tex.opacity);

			const ImVec2 aux = { tex.size.getX(), tex.size.getY() };
			ImGui::Image((ImTextureID)tex.textureID, aux);
			ImGui::PopStyleVar();

		}

		for (UILabelData& label : panel.labels) {
			if (!label.visible) {
				continue;
			}

			const ImVec2 aux = { label.size.getX(), label.size.getY() };
			int tID = getTransformUI(label.entity);
			core::Vector2<> pos = _uiTransforms[tID].position;
			const ImVec2 auxPos = { pos.getX(), pos.getY() };
			std::string labelName = "label_" + label.entity.toString();
			ImGui::InvisibleButton(labelName.c_str(), aux);
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			float auxOpacity = label.bgColor.getAlpha() * label.opacity;
			drawList->AddRectFilled(auxPos, ImVec2(auxPos.x + aux.x, auxPos.y + aux.y), IM_COL32(label.bgColor.getRed() * 255, label.bgColor.getGreen() * 255, label.bgColor.getBlue() * 255, auxOpacity * 255));
			ImGui::PushFont(label.font);
			ImVec2 textSize = ImGui::CalcTextSize(label.text.c_str());

			float posTextX;
			float posTextY = auxPos.y + (aux.y - textSize.y) * 0.5f;

			switch (label.align) {
			case TextAlign::LEFT:
				posTextX = auxPos.x + 5.0f;
				break;
			case TextAlign::CENTER:
				posTextX = auxPos.x + (aux.x - textSize.x) * 0.5f;
				break;
			case TextAlign::RIGHT:
				posTextX = auxPos.x + aux.x - textSize.x - 5.0f;
			}
			drawList->AddText(ImVec2(posTextX, posTextY), IM_COL32(label.textColor.getRed() * 255, label.textColor.getGreen() * 255, label.textColor.getBlue() * 255, auxOpacity * 255), label.text.c_str());

			ImGui::PopFont();

		}
		for (UIButtonData& button : panel.buttons) {
			if (!button.visible) {
				continue;
			}
			int tID = getTransformUI(button.entity);
			auto pos = _uiTransforms[tID].position;
			ImGui::SetCursorPos(ImVec2(pos.getX(), pos.getY()));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, button.opacity);
			const ImVec2 aux = { button.size.getX(), button.size.getY() };

			if (button.buttonImage) {
				std::string idButton = button.textureFile + "_" + button.entity.toString();

				if (ImGui::ImageButton(idButton.c_str(), button.textureID, aux)) {
					if (button.onClick) {
						button.onClick();
					}
				}
			}
			else {
				if (ImGui::Button(button.text.c_str(),aux)) {
					if (button.onClick) {
						button.onClick();
					}
				}
			}
			ImGui::PopStyleVar();
		}
		ImGui::End();

	}
	ImGui::Render();
}




void RenderModule::shutdown()
{
	cleanScene(true);
	if (_overlaySystem) {
		delete _overlaySystem;
		_overlaySystem = nullptr;
	}
	delete _root;
	_root = nullptr;
	_window = nullptr;
	_sceneMgr = nullptr;
}

