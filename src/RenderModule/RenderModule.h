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
#include "Vector3.h"
#include "Color.h"

namespace Ogre
{
    class SceneNode;
    class ImGuiOverlay;
    class Camera;
}

struct EngineNode
{
    Ogre::SceneNode* sceneNode;
    uint64_t nodeID;
};

class ImGuiManager
{
public:
    using UIElement = std::function<void()>;

    void Init();
    void AddElement(UIElement element);
    void Clear();
    void Draw();

private:
    Ogre::ImGuiOverlay* _ui;
    std::vector<UIElement> _uiElements;
};

using entityID = uint64_t;
using cameraID = uint64_t;

class RenderModule
{
public:

    bool Init(const HWND handle, const int width, const int height);
    //void update();
    void renderFrame();
    //void resize(int width, int height);

    //Limpiar escena
    /*
    * @brief Limpiar escena.
    */
    void cleanScene();

    //Metodos transform
    /*
    * @brief Leer posicion del nodo.
    */
    core::Vector3<float> getNodePosition(uint64_t id);
    /*
    * @brief Establecer posicion del nodo.
    */
    void setNodePosition(int id, core::Vector3<float> pos);
    /*
    * @brief Leer orientacion del nodo. Relativo a world space.
    */
    core::Vector3<float> getNodeRotation(uint64_t id);
    /*
    * @brief Establecer orientacion del nodo. Relativo a world space.
    */
    void setNodeRotation(uint64_t id, core::Vector3<float> lookAt);

    //Metodos viewport
    /*
    * @brief Cambiar color de fondo.
    */
    void setViewportBGColor(core::Color color);

    //Metodos camaras
    /*
    * @brief Camara nueva. Se asigna un id por orden de creacion. Main Camera id 0 y añadidas manualmente 1 en adelante.
    */
    void addCamera(const entityID& entityID, const float& FOVy, const float& nearClipDistance, const float& farClipDistance, const float& focalLength, const core::Color& bgColor);
    /*
    * @brief Borrar camara por id. A las camaras creadas posteriormente se les resta el id en 1.
    */
    void deleteCamera(const cameraID& id);
    /*
    * @brief El viewport mostrara la vista de esta camara.
    */
    void setAsActiveCamera(const cameraID& id);
    /*
    * @brief Establecer FOVy.
    */
    void setCameraFOVy(const cameraID& id, const float& FOVy);
    /*
    * @brief Establecer distancia del plano cercano.
    */
    void setCameraNearClipDistance(const cameraID& id, const float& nearClipDistance);
    /*
    * @brief Establecer distancia del plano lejano.
    */
    void setCameraFarClipDistance(const cameraID& id, const float& farClipDistance);
    /*
    * @brief Establecer distancia focal.
    */
    void setCameraFocalLength(const cameraID& id, const float& focalLength);
    /*
    * @brief Limpiar camaras. Deja solo la main camera en posicion inicial.
    */
    void cleanCameras();



    //Metodos luces
    void addLight();
    void deleteLight();
    void setLightActive();


    //Metodos entidades
    void addEntity();
    void deleteEntity();
    void setEntityActive();


    //Getter UI
    ImGuiManager getUI();

    void shutdown();
private:
    std::vector<EngineNode> _engineNodes;
    std::vector<Ogre::Camera*> _cameras;
    ImGuiManager* _ui;
};