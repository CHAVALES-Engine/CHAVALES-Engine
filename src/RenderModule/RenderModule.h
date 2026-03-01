#pragma once
/*
 * @file PlatformModule.h
 * @brief Define las funciones del Modulo de Render
 */

#if _WIN64
#include <Windows.h>
#include <windef.h>
#endif // _WIN64

#include <vector>
#include <functional>

namespace Ogre
{
    class SceneNode;
    class ImGuiOverlay;
}

/*class ImGuiManager
{
public:
    using UIElement = std::function<void()>;

    void Init();
    void AddElement(UIElement element);
    void Clear();
    void Draw();

private:
    Ogre::ImGuiOverlay _ui;
    std::vector<UIElement> _elements;
};*/

class RenderModule
{
public:

    bool Init(const HWND handle, const int width, const int height);
    //void update();
    void renderFrame();
    //void resize(int width, int height);
    void shutdown();

private:
    std::vector<Ogre::SceneNode*> _cameras;
    std::vector<Ogre::SceneNode*> _entitiesNodes;
    //ImGuiManager _ui;
};