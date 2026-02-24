#include "RenderModule.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreStringConverter.h>
#include <iostream>
// RenderModule.cpp : Defines the functions for the static library.
//

bool RenderModule::Init(const HWND* handle, int width, int height)
{
    try
    {
        _root = new Ogre::Root("", "", "ogre.log");

        _root->loadPlugin("RenderSystem_GL3Plus");

        Ogre::RenderSystem* rs = _root->getRenderSystemByName("OpenGL 3+ Rendering Subsystem");

        if (!rs)
        {
            std::cerr << "No se encontro GL3Plus" << std::endl;
            return false;
        }

        _root->setRenderSystem(rs);
        _root->initialise(false); // No ventana automatica

        // Crear ventana basica

        Ogre::NameValuePairList params;
        params["externalWindowHandle"] = std::to_string((size_t)handle); // SDL maneja la ventana, solo usamos contexto
        /*params["FSAA"] = "0";
        params["vsync"] = "true";*/

        _window = _root->createRenderWindow("OgreWindow", width, height, false, &params);

        return true;
    }
    catch (...)
    {
        std::cerr << "Error inicializando OGRE" << std::endl;
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

