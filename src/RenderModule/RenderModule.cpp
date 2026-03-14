#include "RenderModule.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreGL3PlusRenderSystem.h>
#include <OgreGL3PlusPlugin.h>
#include <OgreStringConverter.h>

#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreEntity.h>
#include <OgreLight.h>
#include <OgreSceneNode.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreTechnique.h>
#include <OgreSubEntity.h>
#include <OgreGpuProgramManager.h>
#include <OgreRTShaderSystem.h>
#include <OgreShaderGenerator.h>
#include <iostream>

#include <OgreImGuiOverlay.h>
#include <imgui.h>

// RenderModule.cpp : Defines the functions for the static library.
//

static Ogre::Root* _root = nullptr;
static Ogre::RenderWindow* _window = nullptr;
static Ogre::SceneManager* _sceneMgr = nullptr;
static Ogre::Viewport* _vp = nullptr;
static Ogre::RTShader::ShaderGenerator* _shaderGen;


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
    ImGui::Begin("Canvas");

    for (auto& element : _uiElements)
    {
        element();
    }

    ImGui::End();
}

bool RenderModule::Init(const HWND handle, const int width, const int height)
{
    try
    {
        _root = new Ogre::Root("", "", "ogre.log");

        Ogre::GL3PlusPlugin* gl3Plugin = new Ogre::GL3PlusPlugin();
        _root->installPlugin(gl3Plugin);

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

        /*Ogre::Camera* mainCamera = _sceneMgr->createCamera("MainCamera");
        Ogre::SceneNode* cameraNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
        _cameraNodes.push_back(cameraNode);
        _cameraNodes[0]->setPosition(Ogre::Vector3(0, 5, 15));
        _cameraNodes[0]->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
        mainCamera->setNearClipDistance(0.1f);
        mainCamera->setFarClipDistance(1000.0f);
        mainCamera->setAutoAspectRatio(true);

        _vp = _window->addViewport(mainCamera);
        _vp->setBackgroundColour(Ogre::ColourValue(0.8f, 0.0f, 0.0f));*/

        //ZONA DEMO INICIO
        Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();

        rgm.addResourceLocation("../dependencies/ogre/src/ogre/Media/Main", "FileSystem", "General");
        rgm.addResourceLocation("../dependencies/ogre/src/ogre/Media/RTShaderLib", "FileSystem", "General");


        Ogre::RTShader::ShaderGenerator::initialize();

        _shaderGen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

        _shaderGen->addSceneManager(_sceneMgr);
        _shaderGen->setTargetLanguage("glsl");

        Ogre::MaterialManager::getSingleton().setActiveScheme(
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME
        );

        rgm.addResourceLocation("../dependencies/ogre/src/ogre/Samples/Media/packs/dragon.zip", "Zip", "General");
        rgm.initialiseAllResourceGroups();
        rgm.loadResourceGroup("General");

        Ogre::MaterialPtr mat =
            Ogre::MaterialManager::getSingleton().getByName("BaseWhite");

        //Ogre::MaterialPtr mat = base->clone("BaseWhiteRTSS");

        mat->load();

        std::cout << "Total techniques: " << mat->getNumTechniques() << std::endl;

        _shaderGen->createShaderBasedTechnique(
            *mat,
            Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, true
        );

        _shaderGen->validateMaterial(
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
            mat->getName()
        );

        Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

        Ogre::Entity* cube = _sceneMgr->createEntity("dragon.mesh");

        Ogre::SceneNode* cubeNode =
            _sceneMgr->getRootSceneNode()->createChildSceneNode();

        cubeNode->setPosition(Ogre::Vector3(-50, 0, -200));
        cubeNode->yaw(Ogre::Degree(130));
        cubeNode->attachObject(cube);

        //for (unsigned int i = 0; i < cube->getNumSubEntities(); ++i) { cube->getSubEntity(i)->setMaterialName("BaseWhiteRTSS"); }

        /*_entities.push_back(cubeNode);

        Ogre::Light* light = _sceneMgr->createLight();

        light->setType(Ogre::Light::LT_DIRECTIONAL);

        Ogre::SceneNode* lightNode =
            _sceneMgr->getRootSceneNode()->createChildSceneNode();

        lightNode->attachObject(light);

        _entities.push_back(lightNode);*/
        //ZONA DEMO FINAL

        //_ui = new ImGuiManager();

        _nextTransformID = 0;
        _nextCameraID = 0;

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
        _root->renderOneFrame();
        if (_ui)
        {
            _ui->Draw();
        }
    }
}

void RenderModule::cleanScene()
{
    if (!_sceneMgr)
        return;

    //Limpiar camaras
    cleanCameras();

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

    //_ui->Clear();
}

transformID RenderModule::addNode(const entityID& entityID, const core::Vector3<float>& pos, const core::Quaternion<float>& rot)
{
    EngineNode aux = _engineNodes.back();

    if (aux.nodeID != entityID)
    {
        aux = _engineNodes.emplace_back(_sceneMgr->getRootSceneNode()->createChildSceneNode(), entityID);
        aux.sceneNode->setPosition(Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()));
        aux.sceneNode->setOrientation(Ogre::Quaternion(rot.getX(), rot.getY(), rot.getZ(), rot.getW()));
        return _nextTransformID++;
    }
    return _nextTransformID;
}

core::Vector3<float> RenderModule::getNodePosition(const transformID& id)
{
    if (id >= 0 && id < _engineNodes.size())
    {
        Ogre::Vector3 pos = _engineNodes[id].sceneNode->getPosition();
        return core::Vector3<float>(pos.x, pos.y, pos.z);
    }
    else return core::Vector3<float>(0.0f, 0.0f, 0.0f);
}

void RenderModule::setNodePosition(const transformID& id, const core::Vector3<float>& pos)
{
    if (id >= 0 && id < _engineNodes.size())
    {
        _engineNodes[id].sceneNode->setPosition(pos.getX(), pos.getY(), pos.getZ());
    }
}

core::Quaternion<float> RenderModule::getNodeRotation(const transformID& id)
{
    if (id >= 0 && id < _engineNodes.size())
    {
        Ogre::Quaternion rot = _engineNodes[id].sceneNode->getOrientation();
        return core::Quaternion<float>(rot.x, rot.y, rot.z, rot.w);
    }
    else return core::Quaternion<float>(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderModule::setNodeRotation(const transformID& id, const core::Quaternion<float>& rot)
{
    if (id >= 0 && id < _engineNodes.size())
    {
        _engineNodes[id].sceneNode->setOrientation(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
    }
}

core::Vector3<float> RenderModule::getNodeScale(const transformID& id)
{
    if (id >= 0 && id < _engineNodes.size())
    {
        Ogre::Vector3 pos = _engineNodes[id].sceneNode->getScale();
        return core::Vector3<float>(pos.x, pos.y, pos.z);
    }
    else return core::Vector3<float>(0.0f, 0.0f, 0.0f);
}

void RenderModule::setNodeScale(const transformID& id, const core::Vector3<float>& scale)
{
    if (id >= 0 && id < _engineNodes.size())
    {
        _engineNodes[id].sceneNode->setPosition(scale.getX(), scale.getY(), scale.getZ());
    }
}

void RenderModule::setViewportBGColor(core::Color color)
{
    _vp->setBackgroundColour(Ogre::ColourValue(color.getX(), color.getY(), color.getZ()));
}

cameraID RenderModule::addCamera(const entityID& entityID, const float& FOVy, const float& nearClipDistance, const float& farClipDistance, const float& focalLength, const core::Color& bgColor)
{
    EngineNode aux = _engineNodes.back();

    //Si no existe un nodo con este entityID lo creamos
    if (aux.nodeID != entityID)
    {
        addNode(entityID);
        aux = _engineNodes.back();
    }

    Ogre::Camera* camera = _cameras.emplace_back(_sceneMgr->createCamera("camera" + std::to_string(_nextCameraID++)));
    aux.sceneNode->attachObject(camera);

    //Si es la primera se convierte automaticamente en la activa
    if (_nextCameraID == 0) setAsActiveCamera(_nextCameraID);
    return _nextCameraID;
}

void RenderModule::deleteCamera(const cameraID& id)
{
    if (id >= 0 && id < _cameras.size())
    {
        Ogre::Camera* cam = _cameras[id];
        Ogre::Viewport* vp = _window->getViewport(0);
        //Desvinculamos del viewport en caso de actividad
        if (vp->getCamera() == cam) vp->setCamera(nullptr);
        Ogre::SceneNode* parent = cam->getParentSceneNode();
        parent->detachObject(cam);
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
    if (id >= 0 && id < _cameras.size()) _vp->setCamera(_cameras[id]);
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

void RenderModule::shutdown()
{
    delete _root;
    _root = nullptr;
    _window = nullptr;
    _sceneMgr = nullptr;
}

void RenderModule::setCameraFOVy(const cameraID& id, const float& FOVy)
{
    if (id >= 0 && id < _cameras.size()) _cameras[id]->setFOVy(Ogre::Radian(FOVy));
}

void RenderModule::setCameraNearClipDistance(const cameraID& id, const float& nearClipDistance)
{
    if (id >= 0 && id < _cameras.size()) _cameras[id]->setNearClipDistance(nearClipDistance);
}

void RenderModule::setCameraFarClipDistance(const cameraID& id, const float& farClipDistance)
{
    if (id >= 0 && id < _cameras.size()) _cameras[id]->setFarClipDistance(farClipDistance);
}

void RenderModule::setCameraFocalLength(const cameraID& id, const float& focalLength)
{
    if (id >= 0 && id < _cameras.size()) _cameras[id]->setFocalLength(focalLength);
}
