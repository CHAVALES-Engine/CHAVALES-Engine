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
#include <iostream>
#include <OgreTechnique.h>
#include <OgreSubEntity.h>
#include <OgreGpuProgramManager.h>
#include <OgreRTShaderSystem.h>
#include <OgreShaderGenerator.h>

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
    /*if (!_sceneMgr)
        return;

    //Limpiar entidades
    for (auto* node : _entities)
    {
        if (!node)
        {
            // Destruir objetos adjuntos
            while (node->numAttachedObjects() > 0)
            {
                Ogre::MovableObject* obj = node->getAttachedObject(0);
                node->detachObject(obj);

                if (obj->getMovableType() == "Entity")
                    _sceneMgr->destroyEntity(static_cast<Ogre::Entity*>(obj));
                else
                    _sceneMgr->destroyMovableObject(obj);
            }

            _sceneMgr->destroySceneNode(node);
        }
    }
    _entities.clear();

    //limpiar luces
    for (auto* node : _lights)
    {
        if (!node)
        {
            while (node->numAttachedObjects() > 0)
            {
                Ogre::MovableObject* obj = node->getAttachedObject(0);
                node->detachObject(obj);

                if (obj->getMovableType() == "Light")
                    _sceneMgr->destroyLight(static_cast<Ogre::Light*>(obj));
                else
                    _sceneMgr->destroyMovableObject(obj);
            }

            _sceneMgr->destroySceneNode(node);
        }
    }
    _lights.clear();

    //Limpiar camaras
    cleanCameras();

    _ui->Clear();*/
}

void RenderModule::setViewportBGColor(core::Color color)
{
    _vp->setBackgroundColour(Ogre::ColourValue(color.getX(), color.getY(), color.getZ()));
}

void RenderModule::addCamera(const entityID& entityID, const float& FOVy, const float& nearClipDistance, const float& farClipDistance, const float& focalLength, const core::Color& bgColor)
{
    /*Ogre::Camera* camera = _sceneMgr->createCamera("camera" + );
    EngineNode aux = _engineNodes.back();

    if (aux.nodeID = entityID)
    {
        aux.sceneNode->
    }

    else
    {

    }*/
}

void RenderModule::deleteCamera(const cameraID& id)
{
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
    /*if (!_vp || id < 0 || id >= (int)_cameraNodes.size()) return;

    Ogre::Camera* cam = _cameras[id];
    if (!cam) return;

    _vp->setCamera(cam);*/
}

void RenderModule::cleanCameras()
{
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
    _cameras[id]->setFOVy(Ogre::Radian(FOVy));
}

void RenderModule::setCameraNearClipDistance(const cameraID& id, const float& nearClipDistance)
{
    _cameras[id]->setNearClipDistance(nearClipDistance);
}

void RenderModule::setCameraFarClipDistance(const cameraID& id, const float& farClipDistance)
{
    _cameras[id]->setFarClipDistance(farClipDistance);
}

void RenderModule::setCameraFocalLength(const cameraID& id, const float& focalLength)
{
    _cameras[id]->setFocalLength(focalLength);
}
