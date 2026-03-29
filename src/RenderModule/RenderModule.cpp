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
#include <OgreLogManager.h>
#include <iostream>
#include <OgreImGuiOverlay.h>
#include <OgreOverlaySystem.h>
#include <imgui.h>
#include <assimp/postprocess.h>
#include <OgreOverlayManager.h>

// RenderModule.cpp : Defines the functions for the static library.
//

static Ogre::Root* _root = nullptr;
static Ogre::OverlaySystem* _overlaySystem = nullptr;
static Ogre::RenderWindow* _window = nullptr;
static Ogre::SceneManager* _sceneMgr = nullptr;
static Ogre::Viewport* _vp = nullptr;
static Ogre::RTShader::ShaderGenerator* _shaderGen;
static Ogre::ResourceGroupManager* _rgm;


void ImGuiManager::AddElement(UIElement element)
{
    _uiElements.push_back(element);
}

void ImGuiManager::Clear()
{
    _uiElements.clear();
}

void ImGuiManager::Draw()
{
    Ogre::ImGuiOverlay::NewFrame();

    for (auto& element : _uiElements)
    {
        if (element) element();
    }

    ImGui::ShowDemoWindow();

    ImGui::Render();
}

RenderModule::~RenderModule()
{
    shutdown();
}
void ImGuiManager::Init() {
    _vp->setOverlaysEnabled(true); 
    IMGUI_CHECKVERSION(); 
    ImGui::CreateContext(); 
    ImGui::StyleColorsDark(); 
    ImGuiIO& io = ImGui::GetIO(); 
    io.Fonts->AddFontDefault(); 
    io.Fonts->Build(); 
    overlay = static_cast<Ogre::ImGuiOverlay*>(
        Ogre::OverlayManager::getSingleton().create("ImGuiOverlay")
        );
        overlay->setZOrder(500);
        overlay->show();
}

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

        addNode(0, { 0.0f, 5.0f, 15.0f }, { -20.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
        addCamera(0, 45.0f, 0.1f, 1000.0f, 1.0f, { 0.0f, 0.0f, 0.0f, 1.0f });

        _vp = _window->addViewport(_cameras[0]);
        _vp->setBackgroundColour(Ogre::ColourValue(0.02f, 0.22f, 0.11f));

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

        _rgm->addResourceLocation("../dependencies/ogre/src/ogre/Samples/Media/packs/metroid-floating/source", "FileSystem", "Scene");
        _rgm->addResourceLocation("../dependencies/ogre/src/ogre/Samples/Media/packs/dragon.zip", "Zip", "Scene");
        _rgm->addResourceLocation("../dependencies/ogre/src/ogre/Samples/Media/packs/metroid-floating/sourceimages/membrane", "FileSystem", "membrane");
        _rgm->addResourceLocation("../dependencies/ogre/src/ogre/Samples/Media/packs/metroid-floating/sourceimages/body", "FileSystem", "body");
        _rgm->addResourceLocation("../dependencies/ogre/src/ogre/Samples/Media/packs/metroid-floating/sourceimages/nuclei", "FileSystem", "nuclei");
        _rgm->addResourceLocation("../dependencies/ogre/src/ogre/Samples/Media/packs/metroid-floating/sourceimages/mandibles", "FileSystem", "mandibles");
        _rgm->initialiseAllResourceGroups();

        Ogre::MaterialPtr whiteMat = Ogre::MaterialManager::getSingleton().getByName("BaseWhite");

        //Ogre::MaterialPtr mat = base->clone("BaseWhiteRTSS");

        whiteMat->load();

        std::cout << "Total techniques: " << whiteMat->getNumTechniques() << std::endl;

        _shaderGen->createShaderBasedTechnique(
            *whiteMat,
            Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true
        );

        _shaderGen->validateMaterial(
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
            whiteMat->getName()
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

        Ogre::Entity* cube = _sceneMgr->createEntity("metroid_final.fbx");

        /*for (unsigned int i = 0; i < cube->getNumSubEntities(); ++i)
        {
            Ogre::SubEntity* sub = cube->getSubEntity(i);
            Ogre::MaterialPtr mat = sub->getMaterial();

            std::cout << "SubEntity " << i << ": material = " << mat->getName() << std::endl;

            for (unsigned int t = 0; t < mat->getNumTechniques(); ++t)
            {
                Ogre::Technique* tech = mat->getTechnique(t);
                for (unsigned int p = 0; p < tech->getNumPasses(); ++p)
                {
                    Ogre::Pass* pass = tech->getPass(p);
                    for (unsigned int tu = 0; tu < pass->getNumTextureUnitStates(); ++tu)
                    {
                        Ogre::TextureUnitState* tus = pass->getTextureUnitState(tu);
                        std::cout << "   TextureUnitState " << tu
                            << " = " << tus->getTextureName() << std::endl;
                    }
                }
            }
        }*/

        for (unsigned int i = 0; i < cube->getNumSubEntities(); ++i)
        {
            Ogre::SubEntity* sub = cube->getSubEntity(i);
            Ogre::MaterialPtr mat = sub->getMaterial();

            mat->load();

            // Generar tecnica RTSS sobre el material ya cargado
            _shaderGen->createShaderBasedTechnique(
                *mat,
                Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
                Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                true
            );

            _shaderGen->validateMaterial(
                Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                mat->getName()
            );
        }

        //Materiales
        //////////////////////////////////////////////////////
        Ogre::SubEntity* membraneSub = cube->getSubEntity(0);

        Ogre::MaterialPtr membraneMat = membraneSub->getMaterial();
        membraneSub->setMaterial(membraneMat);

        if (membraneMat->getNumTechniques() == 0)
            membraneMat->createTechnique();
        Ogre::Technique* membraneTech = membraneMat->getTechnique(0);
        if (membraneTech->getNumPasses() == 0)
            membraneTech->createPass();
        Ogre::Pass* membranePass = membraneTech->getPass(0);

        membranePass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        membranePass->setDepthWriteEnabled(false);

        Ogre::TexturePtr membraneTex = Ogre::TextureManager::getSingleton().load(
            "Base_Color.jpeg", "membrane", Ogre::TEX_TYPE_2D, 0
        );

        Ogre::TextureUnitState* membraneTus = membranePass->createTextureUnitState();
        membraneTus->setTexture(membraneTex);
        membraneTus->setColourOperation(Ogre::LBO_MODULATE);
        membranePass->setDiffuse(1.0f, 1.0f, 1.0f, 0.7f);

        /*Ogre::TexturePtr membraneTexOp = Ogre::TextureManager::getSingleton().load(
            "Opacity.jpg", "membrane", Ogre::TEX_TYPE_2D, 0
        );

        Ogre::TextureUnitState* membraneTusOp = membranePass->createTextureUnitState();
        membraneTusOp->setTexture(membraneTexOp);

        membraneTusOp->setColourOperationEx(
            Ogre::LBX_SOURCE1,
            Ogre::LBS_CURRENT,
            Ogre::LBS_CURRENT
        );

        membraneTusOp->setAlphaOperation(
            Ogre::LBX_SOURCE1,
            Ogre::LBS_TEXTURE,
            Ogre::LBS_TEXTURE
        );*/
        //////////////////////////////////////////////////////
        Ogre::SubEntity* bodySub = cube->getSubEntity(1);

        Ogre::MaterialPtr bodyMat = bodySub->getMaterial();
        bodySub->setMaterial(bodyMat);

        if (bodyMat->getNumTechniques() == 0)
            bodyMat->createTechnique();
        Ogre::Technique* bodyTech = bodyMat->getTechnique(0);
        if (bodyTech->getNumPasses() == 0)
            bodyTech->createPass();
        Ogre::Pass* bodyPass = bodyTech->getPass(0);

        Ogre::TexturePtr bodyText = Ogre::TextureManager::getSingleton().load(
            "Base_Color.jpeg", "body", Ogre::TEX_TYPE_2D, 0
        );

        Ogre::TextureUnitState* bodyTus = bodyPass->createTextureUnitState();
        bodyTus->setTexture(bodyText);
        bodyTus->setColourOperation(Ogre::LBO_MODULATE);
        //////////////////////////////////////////////////////
        Ogre::SubEntity* nucleiSub = cube->getSubEntity(2);

        Ogre::MaterialPtr nucleiMat = nucleiSub->getMaterial();
        nucleiSub->setMaterial(nucleiMat);

        if (nucleiMat->getNumTechniques() == 0)
            nucleiMat->createTechnique();
        Ogre::Technique* nucleiTech = nucleiMat->getTechnique(0);
        if (nucleiTech->getNumPasses() == 0)
            nucleiTech->createPass();
        Ogre::Pass* nucleiPass = nucleiTech->getPass(0);

        Ogre::TexturePtr nucleiTex = Ogre::TextureManager::getSingleton().load(
            "Base_Color.jpeg", "nuclei", Ogre::TEX_TYPE_2D, 0
        );

        Ogre::TextureUnitState* nucleiTus = nucleiPass->createTextureUnitState();
        nucleiTus->setTexture(nucleiTex);
        nucleiTus->setColourOperation(Ogre::LBO_MODULATE);
        //////////////////////////////////////////////////////
        Ogre::SubEntity* neuronsSub = cube->getSubEntity(10);

        Ogre::MaterialPtr neuronsMat = neuronsSub->getMaterial();
        neuronsSub->setMaterial(neuronsMat);

        if (neuronsMat->getNumTechniques() == 0)
            neuronsMat->createTechnique();
        Ogre::Technique* neuronsTech = neuronsMat->getTechnique(0);
        if (neuronsTech->getNumPasses() == 0)
            neuronsTech->createPass();
        Ogre::Pass* neuronsPass = neuronsTech->getPass(0);

        Ogre::TexturePtr neuronsTex = Ogre::TextureManager::getSingleton().load(
            "Base_Color.jpeg", "nuclei", Ogre::TEX_TYPE_2D, 0
        );

        Ogre::TextureUnitState* neuronsTus = neuronsPass->createTextureUnitState();
        neuronsTus->setTexture(neuronsTex);
        neuronsTus->setColourOperation(Ogre::LBO_MODULATE);
        //////////////////////////////////////////////////////
        Ogre::SubEntity* mandiblesSub = cube->getSubEntity(6);

        Ogre::MaterialPtr mandiblesMat = mandiblesSub->getMaterial();
        mandiblesSub->setMaterial(mandiblesMat);

        if (mandiblesMat->getNumTechniques() == 0)
            mandiblesMat->createTechnique();
        Ogre::Technique* mandiblesTech = mandiblesMat->getTechnique(0);
        if (mandiblesTech->getNumPasses() == 0)
            mandiblesTech->createPass();
        Ogre::Pass* mandiblesPass = mandiblesTech->getPass(0);

        Ogre::TexturePtr mandiblesTex = Ogre::TextureManager::getSingleton().load(
            "Base_Color.jpeg", "mandibles", Ogre::TEX_TYPE_2D, 0
        );

        Ogre::TextureUnitState* mandiblesTus = mandiblesPass->createTextureUnitState();
        mandiblesTus->setTexture(mandiblesTex);
        mandiblesTus->setColourOperation(Ogre::LBO_MODULATE);
        //////////////////////////////////////////////////////

        Ogre::SceneNode* cubeNode =
            _sceneMgr->getRootSceneNode()->createChildSceneNode();

        cubeNode->setPosition(Ogre::Vector3(-2, 5, 11));
        cubeNode->yaw(Ogre::Degree(130));
        cubeNode->setScale(0.1f, 0.1f, 0.1f);
        cubeNode->attachObject(cube);

        //for (unsigned int i = 0; i < cube->getNumSubEntities(); ++i) { cube->getSubEntity(i)->setMaterialName("BaseWhiteRTSS"); }

        _engineNodes.push_back({ cubeNode, 1 });

        Ogre::Light* light = _sceneMgr->createLight();

        light->setType(Ogre::Light::LT_DIRECTIONAL);

        Ogre::SceneNode* lightNode =
            _sceneMgr->getRootSceneNode()->createChildSceneNode();

        lightNode->attachObject(light);

        _engineNodes.push_back({ lightNode, 2 });
        //ZONA DEMO FINAL
        _overlaySystem = new Ogre::OverlaySystem();
        _sceneMgr->addRenderQueueListener(_overlaySystem);
    

        _ui = new ImGuiManager();
        _ui->Init();
        _ui->AddElement([]() {
            ImGui::ShowDemoWindow();
            ImGui::Begin("Test");
            ImGui::Text("FUNCIONA");
            ImGui::End();
            });
        _nextTransformID = 0;
        _nextCameraID = 0;
        _nextModelID = 0;
        _nextLightID = 0;

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
    if (_root)
    {

        if (_ui)
        {
            _ui->Draw();
        }
     
        
        _root->renderOneFrame();

    }
}

void RenderModule::cleanScene()
{
    if (!_sceneMgr)
        return;

    //Limpiar camaras
   // cleanCameras();

    //Limpiar luces
    cleanLights();

    //Limpiar modelos
    cleanModels();

    //Limpiar nodos
    for (const EngineNode& engineNode : _engineNodes)
    {
        Ogre::SceneNode* sceneNode = engineNode.sceneNode;
        if (sceneNode != nullptr)
        {
            _sceneMgr->destroySceneNode(sceneNode);
        }
    }
    _engineNodes.clear();
    _nextTransformID = 0;

    //_ui->Clear();

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
}



transformID RenderModule::addNode(const entityID& entityID, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float> scale)
{
    if (_engineNodes.empty() || _engineNodes.back().nodeID != entityID)
    {
        EngineNode& aux = _engineNodes.emplace_back(_sceneMgr->getRootSceneNode()->createChildSceneNode(), entityID);
        aux.sceneNode->setPosition(Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()));
        aux.sceneNode->setOrientation(Ogre::Quaternion(rot.getX(), rot.getY(), rot.getZ(), rot.getW()));
        aux.sceneNode->setScale(Ogre::Vector3(scale.getX(), scale.getY(), scale.getZ()));
        return _nextTransformID++;
    }
    return _nextTransformID;
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
        _engineNodes[id].sceneNode->setOrientation(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
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



void RenderModule::setViewportBGColor(core::Color color)
{
    _vp->setBackgroundColour(Ogre::ColourValue(color.getRed(), color.getGreen(), color.getBlue()));
}

cameraID RenderModule::addCamera(const entityID& entityID, const float& FOVy, const float& nearClipDistance, const float& farClipDistance, const float& focalLength, const core::Color& bgColor)
{
    //Si no existe un nodo con este entityID lo creamos
    addNode(entityID);

    Ogre::Camera* camera = _cameras.emplace_back(_sceneMgr->createCamera("camera" + std::to_string(_nextCameraID++)));
    camera->setAutoAspectRatio(true);
    _engineNodes.back().sceneNode->attachObject(camera);

    camera->setFOVy(Ogre::Radian(FOVy));
    camera->setNearClipDistance(nearClipDistance);
    camera->setFarClipDistance(farClipDistance);
    camera->setFocalLength(focalLength);

    //Si es la primera se convierte automaticamente en la activa
    if (_nextCameraID == 0)
    {
        setAsActiveCamera(_nextCameraID);
        _vp->setBackgroundColour(Ogre::ColourValue(bgColor.getRed(), bgColor.getGreen(), bgColor.getBlue()));
    }
    return _nextCameraID;
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
    if (id >= 0 && id < _cameras.size() && _cameras[id] != nullptr) _vp->setCamera(_cameras[id]);
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
    _vp->setCamera(nullptr);
    _cameras.clear();
    _nextCameraID = 0;
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



modelID RenderModule::addModel(const entityID& entityID, std::string modelFolder, std::string modelFile)
{
    addNode(entityID);
    if (!_rgm->resourceGroupExists(modelFolder))
    {
        _rgm->addResourceLocation("carpetilla resources textures" + modelFolder, "FileSystem", modelFolder);
        _rgm->loadResourceGroup(modelFolder);
    }
    Ogre::Entity* model = _models.emplace_back(_sceneMgr->createEntity(modelFile + std::to_string(_nextModelID++), modelFile));
    _engineNodes.back().sceneNode->attachObject(model);

    for (unsigned int i = 0; i < model->getNumSubEntities(); ++i)
    {
        Ogre::SubEntity* sub = model->getSubEntity(i);
        Ogre::MaterialPtr mat = sub->getMaterial();

        mat->load();

        // Generar tecnica RTSS sobre el material ya cargado
        _shaderGen->createShaderBasedTechnique(
            *mat,
            Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
            true
        );

        _shaderGen->validateMaterial(
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
            mat->getName()
        );
    }

    return _nextModelID;
}

void RenderModule::deleteModel(const modelID& id)
{
    if (id >= 0 && id < _models.size() && _models[id] != nullptr)
    {
        Ogre::Entity* model = _models[id];
        Ogre::SceneNode* parent = model->getParentSceneNode();
        parent->detachObject(model);
        _sceneMgr->destroyEntity(model);
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

void RenderModule::setDiffuse(const modelID& id, const subMeshID& subID, std::string textureFolder, std::string textureFile)
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
            _rgm->addResourceLocation("carpetilla resources textures" + textureFolder, "FileSystem", textureFolder);
            _rgm->loadResourceGroup(textureFolder);
        }
        Ogre::TexturePtr text = Ogre::TextureManager::getSingleton().load(textureFile, "diffuse_" + textureFile, Ogre::TEX_TYPE_2D, 0);

        Ogre::TextureUnitState* tus = pass->createTextureUnitState();
        tus->setTexture(text);
        tus->setColourOperation(Ogre::LBO_MODULATE);
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



lightID RenderModule::addLight(const entityID& entityID, int type, const core::Color& color, float intensity) {
    //Si no existe un nodo con este entityID lo creamos
    addNode(entityID);

    Ogre::Light* light = _sceneMgr->createLight("light" + std::to_string(_nextLightID++));
    
    switch (type) {
        case 0: light->setType(Ogre::Light::LT_POINT); break;
        case 1: light->setType(Ogre::Light::LT_DIRECTIONAL); break;
        case 2: light->setType(Ogre::Light::LT_SPOTLIGHT); break;
        case 3: light->setType(Ogre::Light::LT_RECTLIGHT); break;
    }
    
    light->setDiffuseColour(color.getRed(), color.getGreen(), color.getBlue());
    light->setSpecularColour(color.getRed(), color.getGreen(), color.getBlue());

    light->setPowerScale(intensity);
    
    _engineNodes.back().sceneNode->attachObject(light);

    _lights.push_back(light);
    
    return _nextLightID;
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

void RenderModule::setLightActive(const lightID& id, bool active) {
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

void RenderModule::setLightType(const lightID& id, int type) {
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

void RenderModule::setLightIntensity(const lightID& id, float intensity) {
    if (id >= 0 && id < _lights.size() && _lights[id] != nullptr) _lights[id]->setPowerScale(intensity);
}

void RenderModule::setLightSpotRange(const lightID& id, float inner, float outer, float falloff) {
    if (id >= 0 && id < _lights.size() && _lights[id] != nullptr) _lights[id]->setSpotlightRange(Ogre::Degree(inner), Ogre::Degree(outer), falloff);
}

void RenderModule::shutdown()
{
    cleanScene();
    if (_overlaySystem) {
        delete _overlaySystem;
        _overlaySystem = nullptr;
    }
    delete _root;
    _root = nullptr;
    _window = nullptr;
    _sceneMgr = nullptr;
}