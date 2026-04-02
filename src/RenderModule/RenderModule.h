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
#include <string>
#include <functional>
#include "Vector3.h"
#include "Quaternion.h"
#include "Color.h"

#include "guid.h"

namespace Ogre
{
    class SceneNode;
    class ImGuiOverlay;
    class Camera;
    class Entity;
    class Light;
    class AnimationState;
}

using entityID = ChavalesGUID;
using transformID = uint64_t;
using cameraID = uint64_t;
using modelID = uint64_t;
using animationID = uint64_t;
using subMeshID = uint64_t;
using lightID = uint64_t;

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
    Ogre::ImGuiOverlay* _overlay;
private:
   
    std::vector<UIElement> _uiElements;
};

class RenderModule
{
public:
    ~RenderModule();
    bool Init(const HWND handle, const int width, const int height);
    //void update();
    /*
    * @brief Renderizar frame.
    */
    void renderFrame();
    //void resize(int width, int height);

    /*
    * @brief Borrar todos los elementos de la escena.
    */
    void cleanScene();



    //Metodos transform
    /*
    * @brief Anadir nodo.
    */
    transformID addNode(const entityID& entityID, const core::Vector3<float>& pos = core::Vector3<float>(0.0f, 0.0f, 0.0f), const core::Quaternion<float>& rot = core::Quaternion<float>(0.0f, 0.0f, 0.0f, 1.0f), const core::Vector3<float> scale = core::Vector3<float>(1.0f, 1.0f, 1.0f));
    /*
    * @brief Getter de nodo. Devuelve -1 si no existe.
    */
    transformID getNode(const entityID& entityID);
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


    //Metodos modelos
    /*
    * @brief Modelo nuevo. Se asigna un id por orden de creacion. Main Luz id 0 y añadidas manualmente 1 en adelante. 
    */
    modelID addModel(const entityID& entityID, const std::string& modelFolder, const std::string& modelFile);
    /*
    * @brief Borrar modelo.
    */
    void deleteModel(const modelID& id);
    /*
    * @brief Limpiar modelos
    */
    void cleanModels();
    /*
    * @brief Asignar color base a submesh.
    */
    void setDiffuse(const modelID& id, const subMeshID& subID, const std::string& textureFolder, const std::string& textureFile);
    /*
    * @brief Tintar la textura del material. Incluye canal alpha para transparencia.
    */
    void setTint(const modelID& id, const subMeshID& subID, const core::Color& tint);
    /*
    * @brief Establecer si el modelo es visible
    */
    void setModelVisible(const modelID& id, const bool& visible);


    //Metodos animaciones
    /*
    * @brief Anadir animator.
    */
    void addAnimator(const entityID& entityID, modelID& modelID);
    /*
    * @brief Limpiar animaciones
    */
    void cleanAnimations();
    /*
    * @brief Registrar animacion de esqueleto.
    */
    animationID registerSkeletonAnim(const modelID& modelID, const std::string& animationName, const bool& loop);
    /*
    * @brief Crear animacion de transform.
    */
    animationID createTransformAnimation(const entityID& entityID, const std::string& animationName, const bool& loop, const float& totalDuration);
    /*
    * @brief Anadir keyframe a animacion de transform.
    */
    void addTransformKeyFrame(const animationID& animationID,
                              const float& timePos, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale);
    /*
    * @brief Anadir keyframe a animacion de transform con rotacion sencilla.
    */
    void addTransformKeyFrame(const animationID& animationID,
                              const float& timePos, const core::Vector3<float>& pos, const float& rot, const int& axis, const core::Vector3<float>& scale);
    /*
    * @brief Establecer animacion activa.
    */
    void setAnimEnabled(const animationID& animationID, const bool& active);
    /*
    * @brief Reanudar animacíon a partir de cierto instante de tiempo.
    */
    void setAnimTimePos(const animationID& animationID, const float& timePos);
    /*
    * @brief Actualizar animacion.
    */
    void updateAnimation(const animationID& animationID, const uint64_t& deltaTime);


    //Metodos luces
    /*
    * @brief Luz nueva. Se asigna un id por orden de creacion. Main Luz id 0 y añadidas manualmente 1 en adelante.
    */
    lightID addLight(const entityID& entityID, const int& type, const core::Color& color, const float& intensity);
    /*
    * @brief Borrar luz por id. A las luces creadas posteriormente se les resta el id en 1.
    */
    void deleteLight(const lightID& id);
    /*
   * @brief Establecer actividad de luz.
   */
    void setLightActive(const lightID& id, const bool& active);
    /*
    * @brief Limpiar luces.
    */
    void cleanLights();
    /*
    * @brief Establecer el tipo de luz.
    */
    void setLightType(const lightID& id, const int& type);
    /*
    * @brief Establecer el color de la luz.
    */
    void setLightColor(const lightID& id, const core::Color& color);
    /*
    * @brief Establecer la intensidad de luz.
    */
    void setLightIntensity(const lightID& id, const float& intensity);
   /*
    * @brief Establecer el cono de luz (angulo interno, angulo externo y suavidad de degradado).
    */
    void setLightSpotRange(const lightID& id, const float& inner, const float& outer, const float& falloff);


    //Getter UI
    ImGuiManager* getUI() { return _ui; }

    void shutdown();
private:
    std::vector<EngineNode> _engineNodes;
    std::vector<Ogre::Camera*> _cameras;
    std::vector<Ogre::Entity*> _models;
    std::vector<Ogre::AnimationState*> _animations;
    std::vector<Ogre::Light*> _lights;
    ImGuiManager* _ui;

    transformID _nextTransformID;
    cameraID _nextCameraID;
    modelID _nextModelID;
    animationID _nextAnimationID;
    lightID _nextLightID;
};