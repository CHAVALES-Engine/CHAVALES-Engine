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
#include <iostream>

//#include <OgreImGuiOverlay.h>
// RenderModule.cpp : Defines the functions for the static library.
//

static Ogre::Root* _root = nullptr;
static Ogre::RenderWindow* _window = nullptr;
static Ogre::SceneManager* _sceneMgr = nullptr;
static Ogre::Viewport* _vp = nullptr;


/*void ImGuiManager::AddElement(UIElement element)
{
    _elements.push_back(element);
}

void ImGuiManager::Clear()
{
    _elements.clear();
}

void ImGuiManager::Draw()
{
    Ogre::ImGuiOverlay 
    ImGui::Begin("Canvas");

    for (auto& element : _elements)
    {
        element();
    }

    ImGui::End();
}*/

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
        _cameras.push_back(cameraNode);
        _cameras[0]->setPosition(Ogre::Vector3(0, 0, 80));
        _cameras[0]->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
        mainCamera->setNearClipDistance(0.1f);
        mainCamera->setAutoAspectRatio(true);

        Ogre::Viewport* vp = _window->addViewport(mainCamera);
        vp->setBackgroundColour(Ogre::ColourValue(1.0f, 1.0f, 0.0f));

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
    }
}

void RenderModule::shutdown()
{
    delete _root;
    _root = nullptr;
    _window = nullptr;
    _sceneMgr = nullptr;
}