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
#include "Quaternion.h"
#include "Color.h"

namespace Ogre
{
    class SceneNode;
    class ImGuiOverlay;
    class Camera;
}

using entityID = uint64_t;
using transformID = uint64_t;
using cameraID = uint64_t;

struct EngineNode
{
    Ogre::SceneNode* sceneNode;
    entityID nodeID;

    EngineNode(Ogre::SceneNode* node, entityID id) : sceneNode(node), nodeID(id) {}
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
    * @brief Anadir nodo.
    */
    transformID addNode(const entityID& entityID, const core::Vector3<float>& pos = core::Vector3<float>(0.0f, 0.0f, 0.0f), const core::Quaternion<float>& rot = core::Quaternion<float>(0.0f, 0.0f, 0.0f, 1.0f));
    /*
    * @brief Leer posicion del nodo.
    */
    core::Vector3<float> getNodePosition(const transformID& id);
    /*
    * @brief Establecer posicion del nodo.
    */
    void setNodePosition(const transformID& id, const core::Vector3<float>& pos);
    /*
    * @brief Leer orientacion del nodo. Relativo a world space.
    */
    core::Quaternion<float> getNodeRotation(const transformID& id);
    /*
    * @brief Establecer orientacion del nodo. Relativo a world space.
    */
    void setNodeRotation(const transformID& id, const core::Quaternion<float>& rot);
    /*
    * @brief Leer escala del nodo. Relativo a world space.
    */
    core::Vector3<float> getNodeScale(const transformID& id);
    /*
    * @brief Establecer escala del nodo. Relativo a world space.
    */
    void setNodeScale(const transformID& id, const core::Vector3<float>& scale);

    //Metodos viewport
    /*
    * @brief Cambiar color de fondo.
    */
    void setViewportBGColor(core::Color color);

    //Metodos camaras
    /*
    * @brief Camara nueva. Se asigna un id por orden de creacion. Main Camera id 0 y añadidas manualmente 1 en adelante.
    */
    cameraID addCamera(const entityID& entityID, const float& FOVy, const float& nearClipDistance, const float& farClipDistance, const float& focalLength, const core::Color& bgColor);
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
    void cleanLights();


    //Metodos entidades
    void addEntity();
    void deleteEntity();
    void setEntityActive();
    void cleanEntities();


    //Getter UI
    ImGuiManager getUI();

    void shutdown();
private:
    std::vector<EngineNode> _engineNodes;
    std::vector<Ogre::Camera*> _cameras;
    ImGuiManager* _ui;

    transformID _nextTransformID;
    cameraID _nextCameraID;
};