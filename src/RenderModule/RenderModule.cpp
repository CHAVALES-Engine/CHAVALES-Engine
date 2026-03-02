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

#include <OgreImGuiOverlay.h>
#include <imgui.h>
// RenderModule.cpp : Defines the functions for the static library.
//

static Ogre::Root* _root = nullptr;
static Ogre::RenderWindow* _window = nullptr;
static Ogre::SceneManager* _sceneMgr = nullptr;
static Ogre::Viewport* _vp = nullptr;


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

        Ogre::Camera* mainCamera = _sceneMgr->createCamera("MainCamera");
        Ogre::SceneNode* cameraNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
        _cameraNodes.push_back(cameraNode);
        _cameraNodes[0]->setPosition(Ogre::Vector3(0, 0, 80));
        _cameraNodes[0]->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
        mainCamera->setNearClipDistance(0.1f);
        mainCamera->setAutoAspectRatio(true);

        _vp = _window->addViewport(mainCamera);
        _vp->setBackgroundColour(Ogre::ColourValue(1.0f, 1.0f, 0.0f));

        _ui = new ImGuiManager();

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

    _ui->Clear();
}

void RenderModule::setViewportBGColor(core::Color color)
{
    _vp->setBackgroundColour(Ogre::ColourValue(color.getX(), color.getY(), color.getZ()));
}

void RenderModule::addCamera(core::Vector3<float> pos, core::Vector3<float> lookAt)
{
    if (!_sceneMgr) return;

    int id = _cameraNodes.size();
    std::string camName = "Camera" + std::to_string(id);
    Ogre::Camera* cam = _sceneMgr->createCamera(camName);

    Ogre::SceneNode* node = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(cam);

    node->setPosition(Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()));
    node->lookAt(Ogre::Vector3(lookAt.getX(), lookAt.getY(), lookAt.getZ()), Ogre::Node::TS_WORLD);

    _cameraNodes.push_back(node);
    _cameras.push_back(cam);
}

void RenderModule::deleteCamera(int id)
{
    if (id <= 0 || id >= (int)_cameraNodes.size()) return;

    Ogre::SceneNode* node = _cameraNodes[id];
    Ogre::Camera* cam = _cameras[id];

    if (!node || !cam) return;

    node->detachObject(cam);
    _sceneMgr->destroyCamera(cam);
    _sceneMgr->destroySceneNode(node);

    _cameraNodes.erase(_cameraNodes.begin() + id);
    _cameras.erase(_cameras.begin() + id);
}

void RenderModule::setActiveCamera(int id)
{
    if (!_vp || id < 0 || id >= (int)_cameraNodes.size()) return;

    Ogre::Camera* cam = _cameras[id];
    if (!cam) return;

    _vp->setCamera(cam);
}

core::Vector3<float> RenderModule::getCameraPosition(int id)
{
    if (id < 0 || id >= (int)_cameraNodes.size()) return { 0.0, 0.0, 0.0 };
    Ogre::Vector3 pos = _cameraNodes[id]->getPosition();
    return { pos.x, pos.y, pos.z };
}

void RenderModule::setCameraPosition(int id, core::Vector3<float> pos)
{
    if (id < 0 || id >= (int)_cameraNodes.size()) return;
    _cameraNodes[id]->setPosition(Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ()));
}

core::Vector3<float> RenderModule::getCameraLookAt(int id)
{
    if (id < 0 || id >= (int)_cameraNodes.size()) return { 0.0, 0.0, 0.0 };

    Ogre::SceneNode* node = _cameraNodes[id];
    Ogre::Vector3 dir = node->_getDerivedOrientation() * Ogre::Vector3::UNIT_Z;
    Ogre::Vector3 pos = node->getPosition();
    Ogre::Vector3 lookAt = pos + dir;
    return { lookAt.x, lookAt.y, lookAt.z };
}

void RenderModule::setCameraLookAt(int id, core::Vector3<float> lookAt)
{
    if (id < 0 || id >= (int)_cameraNodes.size()) return;
    _cameraNodes[id]->lookAt(Ogre::Vector3(lookAt.getX(), lookAt.getY(), lookAt.getZ()), Ogre::Node::TS_WORLD);
}

void RenderModule::cleanCameras()
{
    if (_cameraNodes.empty()) return;

    Ogre::SceneNode* mainNode = _cameraNodes[0];
    Ogre::Camera* mainCam = _cameras[0];

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
    mainNode->setOrientation(Ogre::Quaternion::IDENTITY);
}

void RenderModule::shutdown()
{
    delete _root;
    _root = nullptr;
    _window = nullptr;
    _sceneMgr = nullptr;
}