#pragma once
/*
 * @file PlatformModule.h
 * @brief Define las funciones del Modulo de Render
 */

#if _WIN64
#include <Windows.h>
#include <windef.h>
#endif // _WIN64

namespace Ogre
{
    class Root;
    class RenderWindow;
}

class RenderModule
{
public:

    bool Init(const HWND* handle, int width, int height);
    void renderFrame();
    //void resize(int width, int height);
    void shutdown();
private:

    Ogre::Root* _root = nullptr;
    Ogre::RenderWindow* _window = nullptr;
};