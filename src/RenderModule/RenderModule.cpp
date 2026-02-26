#include "RenderModule.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreGL3PlusRenderSystem.h>
#include <OgreGL3PlusPlugin.h>
#include <OgreStringConverter.h>
#include <iostream>
// RenderModule.cpp : Defines the functions for the static library.
//

static Ogre::Root* _root = nullptr;
static Ogre::RenderWindow* _window = nullptr;

bool RenderModule::Init(const HWND handle, int width, int height)
{
    try
    {
        _root = new Ogre::Root("", "", "ogre.log");

        Ogre::GL3PlusPlugin* gl3Plugin = new Ogre::GL3PlusPlugin();
        _root->installPlugin(gl3Plugin);

        const Ogre::RenderSystemList& renderers = _root->getAvailableRenderers();
        if (renderers.empty())
            throw std::runtime_error("No renderers available!");
        Ogre::RenderSystem* rs = renderers[0];
        _root->setRenderSystem(rs);

        _root->initialise(false); // No ventana automatica

        // Crear ventana basica

        Ogre::NameValuePairList params;
        params["externalWindowHandle"] = std::to_string((size_t)handle); // SDL maneja la ventana, solo usamos contexto
        params["FSAA"] = "0";
        params["vsync"] = "true";

        _window = _root->createRenderWindow("OgreWindow", width, height, false, &params);

        return true;
    }
    catch (...)
    {
        std::cerr << "Error initializing OGRE" << std::endl;
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
}