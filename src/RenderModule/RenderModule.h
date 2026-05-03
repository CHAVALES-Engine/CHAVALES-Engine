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
#include <unordered_set>
#include <string>
#include <functional>
#include "Vector3.h"
#include "Vector2.h"
#include "Quaternion.h"
#include "Color.h"
#include "CommonEnums.h"
#include "guid.h"
#include "PhysicsTypes.h"

namespace Ogre
{
	class SceneNode;
	class ImGuiOverlay;
	class Camera;
	class Entity;
	class Light;
	class AnimationState;
	class ParticleSystem;
	class ImGui;
}

struct SDL_Window;
struct ImFont;
using entityID = ChavalesGUID;
using transformID = uint64_t;
using UITransformID = uint64_t;
using cameraID = uint64_t;
using modelID = uint64_t;
using animationID = uint64_t;
using subMeshID = uint64_t;
using lightID = uint64_t;
using particleGenID = uint64_t;
using uiPanelID = uint64_t;
using uiLabelID = uint64_t;
using uiButtonID = uint64_t;
using uiTextureRectID = uint64_t;
using ImTextureID = uint64_t;
using FontName = std::string;
using FontPath = std::string;

struct EngineNode
{
	Ogre::SceneNode* sceneNode;
	entityID nodeID;

	EngineNode(Ogre::SceneNode* node, entityID id) : sceneNode(node), nodeID(id) {}
};

struct UILabelData 
{
	entityID entity;
	std::string text;
	bool visible;
	float opacity = 1.0f;
	core::Color textColor = core::Color(1, 1, 1, 1);
	core::Color bgColor = core::Color(0, 0, 0, 0);
	float fontSize = 16.0f;
	TextAlign align = TextAlign::LEFT;
	ImFont* font;
	bool alive = true;
};

struct UIButtonData 
{
	entityID entity;
	std::string text;
	bool visible;
	std::string textureFolder;
	std::string textureFile;
	ImTextureID textureID;
	bool buttonImage;
	float opacity = 1.0f;
	std::function<void()> onClick;
	ImFont* font;
	float fontSize = 16.0f;
	core::Color textColor = core::Color(1, 1, 1, 1);
	core::Color bgColor = core::Color(0, 0, 0, 1);
	core::Color hvColor = core::Color(0, 0, 0, 1);
	core::Color psColor = core::Color(0, 0, 0, 1);
	bool alive = true;
};

struct UITextureRectData 
{
	entityID entity;
	std::string textureFolder;
	std::string textureFile;
	bool visible;
	float opacity = 1.0f;

	ImTextureID textureID;
	bool alive = true;
};

struct UIPanelData 
{
	entityID entity;
	std::string title;
	bool visible;

	std::vector<UILabelData> labels;
	std::vector<UIButtonData> buttons;
	std::vector<UITextureRectData> textureRects;
	bool alive = true;
};

struct UITransform
{
	entityID entity;
	core::Vector2<float> position;
	core::Vector2<float> dimension;
	float rotation = 0.0f;
	int zBuffer = 0;
};

union SDL_Event;

class RenderModule
{
public:
	~RenderModule();
	bool Init( SDL_Window* sdlWindow,const HWND handle, const int width, const int height, const std::vector<std::pair<FontName, FontPath>> fonts);

	/*
	* @brief Renderizar frame.
	*/
	void renderFrame();

	/*
	* @brief Borrar todos los elementos de la escena.
	*/
	void cleanScene(const bool& end);

	using EventCallback = bool(*)(const SDL_Event* event);
	EventCallback getImguiInputCallback();

	// --- Metodos transform
	/*
	* @brief Anadir nodo.
	*/
	transformID addNode(const entityID& entityID, const core::Vector3<float>& pos = core::Vector3<float>(0.0f, 0.0f, 0.0f), const core::Quaternion<float>& rot = core::Quaternion<float>(0.0f, 0.0f, 0.0f, 1.0f), const core::Vector3<float> scale = core::Vector3<float>(1.0f, 1.0f, 1.0f), const bool& fromTransform = false);

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

	// --- Metodos transform UI

	/*
	* @brief Anadir nodoUI
	*/
	UITransformID addUITransform(const entityID& entityID, const core::Vector2<float>& pos = core::Vector2<float>(1.0f, 0.0f), const int& zBuffer = 0, const core::Vector2<float>& dimension = core::Vector2<float>(0.0f, 0.0f), const float& rotation = 0.0f);

	/*
	* @brief Establecer posicion del nodo.
	*/
	void setUITransformDimension(const UITransformID& id, const core::Vector2<float>& dim);

	/*
	* @brief Establecer dimension del nodo.
	*/
	void setUITransformPos(const UITransformID& id, const core::Vector2<float>& pos);

	/*
	* @brief Establecer rotacion del nodo.
	*/
	void setUITransformRotation(const UITransformID& id, const float& pos);
	
	/*
	* @brief Establecer zBuffer del nodo.
	*/
	void setUITransformZBuffer(const UITransformID& id, const int& pos);
	 
	/*
	* @brief Getter de nodoUI. Devuelve -1 si no existe.
	*/
	UITransformID getTransformUI(const entityID& entityID);
	
	/*
	* @brief Leer posicion del componente de la UI.
	*/
	core::Vector2<float> getUIPosition(const UITransformID& id);

	// --- Metodos viewport
	/*
	* @brief Cambiar color de fondo.
	*/
	void setViewportBGColor(core::Color color);

	// --- Metodos camaras
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


	// --- Metodos modelos
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

	// --- Metodos animaciones
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
		const float& timePos, const core::Vector3<float>& pos, 
		const core::Quaternion<float>& rot, const core::Vector3<float>& scale);

	/*
	* @brief Anadir keyframe a animacion de transform con rotacion sencilla.
	*/
	void addTransformKeyFrame(const animationID& animationID,
		const float& timePos, const core::Vector3<float>& pos, 
		const float& rot, const int& axis, const core::Vector3<float>& scale);

	/*
	* @brief Establecer animacion activa.
	*/
	void setAnimEnabled(const animationID& animationID, const bool& active);

	/*
	* @brief Reanudar animacíon a partir de cierto instante de tiempo.
	*/
	void setAnimTimePos(const animationID& animationID, const float& timePos);

	/*
	* @brief Ajustar velocidad animacion.
	*/
	void setAnimSpeed(const animationID& animationID, const float& speed);

	/*
	* @brief Actualizar animacion.
	*/
	void updateAnimation(const animationID& animationID, const uint64_t& deltaTime);

	// --- Metodos luces
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

	/*
	 * @brief Cambiar luz ambiente de la escena.
	 */
	void setAmbientLight(const core::Color& color);

	// --- Metodos particulas
	/*
	* @brief Anadir generador de particulas.
	*/
	particleGenID addParticleGen(const entityID& entityID, const std::string& textureFolder, const std::string& textureFile);

	/*
	* @brief Borrar generador de particulas.
	*/
	void deleteParticleGen(const particleGenID& id);

	/*
	* @brief Limpiar generadores de particulas.
	*/
	void cleanParticleGens();

	/*
	* @brief Establecer actividad de generador de particulas.
	*/
	void setParticleGenEnabled(const particleGenID& id, const bool& enabled);

	/*
	* @brief Establecer emision de generador de particulas.
	*/
	void setParticleGenEmitting(const particleGenID& id, const bool& emitting);

	/*
	* @brief Establecer cantidad total de particulas del generador.
	*/
	void setParticleGenQuota(const particleGenID& id, const float& quota);

	/*
	* @brief Establecer ratio de emision de generador de particulas.
	*/
	void setParticleGenEmissionRate(const particleGenID& id, const float& rate);

	/*
	* @brief Establecer tiempo de emision del generador de particulas.
	*/
	void setParticleGenDuration(const particleGenID& id, const float& duration);

	/*
	* @brief Establecer tiempo de vida de particulas del generador.
	*/
	void setParticleGenTimeToLive(const particleGenID& id, const float& time);

	/*
	* @brief Establecer velocidad de particulas del generador.
	*/
	void setParticleGenVelocity(const particleGenID& id, const float& velocity);

	/*
	* @brief Establecer velocidad minima de particulas del generador.
	*/
	void setParticleGenMinVelocity(const particleGenID& id, const float& velocity);

	/*
	* @brief Establecer velocidad maxima de particulas del generador.
	*/
	void setParticleGenMaxVelocity(const particleGenID& id, const float& velocity);

	/*
	* @brief Establecer direccion de particulas del generador.
	*/
	void setParticleGenDirection(const particleGenID& id, const core::Vector3<float>& direction);

	/*
	* @brief Establecer angulo de dipsersion de particulas del generador.
	*/
	void setParticleGenAngle(const particleGenID& id, const float& angle);

	/*
	* @brief Establecer ancho de particulas del generador.
	*/
	void setParticleGenPartWidth(const particleGenID& id, const float& width);

	/*
	* @brief Establecer alto de particulas del generador.
	*/
	void setParticleGenPartHeight(const particleGenID& id, const float& height);

	/*
	* @brief Establecer color de particulas del generador.
	*/
	void setParticleGenPartColor(const particleGenID& id, const core::Color& color);

    // --- Metodos skydome
    /*
    * @brief Establecer skydome
    */
    void setSkydome(const std::string& textureFolder, const std::string& textureFile, const float& curvature, const float& tiling, const float& distance, const bool& drawFirst);

    /*
    * @brief Quitar skydome
    */
    void setSkydomeNull();

	// --- Metodos UI
	/*
	* @brief Añadir un panel
	*/
	uiPanelID addUIPanel(const entityID& entityID, const std::string& title);

	/*
	* @brief Establecer si el panel es visible
	*/
	void setUIPanelVisible(const uiPanelID& id, bool visible);
	/*
	* @brief borrar panel de UI.
	*/
	void deleteUIPanel(const uiPanelID& id);
	/*
	 * @brief Añadir un letrero al panel
	 */
	uiLabelID addUILabel(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const  float opacity, const core::Color textColor, const core::Color bgColor, const float fontSize, const TextAlign textAlign, const std::string fontName);
	/*
	* @brief borrar label de UI.
	*/
	void deleteUILabel(const uiLabelID& id);
	/*
	* @brief Establecer si el letrero es visible
	*/
	void setUILabelVisible(const uiLabelID& labelID, bool visible);

	/*
	* @brief Establecer si el texto del letrero
	*/
	void setUILabelText(const uiLabelID& labelID, const std::string& text);

	/*
	* @brief Establecer la opacidad del letrero
	*/
	void  setUILabelOpacity(const uiLabelID& labelID, float opacity);

	/*
	* @brief Establecer el color del texto del letrero
	*/
	void setUILabelTextColor(const uiLabelID& labelID, core::Color color);

	/*
	 * @brief Establecer el color del fondo del letrero
	*/
	void setUILabelBackGroundColor(const uiLabelID& labelID, core::Color color);

	/*
	* @brief Establecer el alineado del letrero
	*/
	void setUILabelAlign(const uiLabelID& labelID, const TextAlign& align);

	/*
	 * @brief Añadir un boton al panel
	 */
	uiButtonID addUIButton(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const float& fontSize, const std::string& fontName, const core::Color& bgColor, const core::Color& txColor, const core::Color& hvColor, const core::Color& psColor, const float& opacity);
	/*
	* @brief borrar boton de UI.
	*/
	void deleteUIButton(const uiButtonID& id);
	/*
	 * @brief Añadir un ImageBoton al panel
	 */
	uiButtonID addUIImageButton(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const std::string& textureFolder, const std::string& textureFile, const core::Color& bgColor, const core::Color& hvColor, const core::Color& psColor, const float& opacity);

	/*
	* @brief Establecer si el boton es visible
	*/
	void setUIButtonVisible(const uiButtonID& buttonID, bool& visible);

	/*
	* @brief Establecer el texto del boton
	*/
	void setUIButtonText(const uiButtonID& buttonID, const std::string& text);

	/*
	* @brief Establecer la textura del boton
	*/
	void  setUIButtonTexture(const uiButtonID& buttonID, const std::string& texture);

	/*
	* @brief Establecer la opacidad  del boton
	*/
	void  setUIButtonOpacity(const uiButtonID& buttonID, float& opacity);

	/*
	* @brief Establecer el color de fondo del boton
	*/
	void  setUIButtonBackgroundColor(const uiButtonID& buttonID, core::Color& bgColor);

	/*
	* @brief Establecer el color del texto del boton
	*/
	void  setUIButtonTextColor(const uiButtonID& buttonID, core::Color& txColor);

	/*
	* @brief Establecer el color del hover del boton
	*/
	void  setUIButtonHoverColor(const uiButtonID& buttonID, core::Color& hvColor);

	/*
	* @brief Establecer el color del pulsado del boton
	*/
	void  setUIButtonPressColor(const uiButtonID& buttonID, core::Color& psColor);

	/*
	* @brief Establecer el callback del boton
	*/
	void setUIButtonCallback(const uiButtonID& buttonID, std::function<void()> callback);

	/*
	 * @brief Anadir textureRect al panel.
	 */
	uiTextureRectID addUITextureRect(const uiPanelID& panelID, const entityID& entityID, const std::string& textureFolder, const std::string& textureFile);
	/*
	* @brief borrar textureRect de UI.
	*/
	void deleteUITextureRect(const uiTextureRectID& id);
	/*
	* @brief Establecer la textura del textureRect
	*/
	void setUITextureRectTexture(const uiTextureRectID& textureRectID, const std::string& texture);

	/*
	* @brief Establecer si el textureRect es visible
	*/
	void setUITextureRectVisible(const uiTextureRectID& textureRectID, bool& visible);

	/*
	* @brief Establecer la opacidad  del textureRect
	*/
	void setUITextureRectOpacity(const uiTextureRectID& textureRectID, float& opacity);

	void renderUI();
	void cleanUI();

	/*
	* @brief Se encarga de resetear los colliders dibujados en escen
	*/
	void cleanDebug();

	/*
	* @brief Se encarga de dibujar el debug de todos los colliders en escena
	*/
	void RenderPhysics(const std::vector<ShapeRenderData>& physicsShapes);

	/*
	* @brief Dibuja el box collider para debug
	*/
	void DrawBox(const ShapeRenderData& data);

	/*
	* @brief Dibuja el capsule collider para debug
	*/
	void DrawCapsule(const ShapeRenderData& data);

	/*
	* @brief Dibuja el box sphere para debug
	*/
	void DrawSphere(const ShapeRenderData& data);

	void shutdown();

private:
	std::vector<EngineNode> _engineNodes;
	std::vector<Ogre::Camera*> _cameras;
	std::vector<Ogre::Entity*> _models;
	//Animation State y speed
	std::vector<std::pair<Ogre::AnimationState*, float>> _animations;
	std::vector<Ogre::Light*> _lights;
	std::vector<Ogre::ParticleSystem*> _particleGens;
	std::vector<UIPanelData> _uiPanels;
	std::unordered_map<uiLabelID, std::pair< uiPanelID, int>> _labelToPanel;
	std::unordered_map<uiButtonID, std::pair<uiPanelID, int>> _buttonToPanel;
	std::unordered_map<uiTextureRectID, std::pair<uiPanelID, int>> _textureToPanel;
	std::unordered_map<std::string, ImFont*> _fonts;
	std::vector<UITransform> _uiTransforms;
	std::vector<std::string> _createdMaterials;
	TextAlign stringToAlign(const std::string& align);

    transformID _nextTransformID;
    UITransformID _nextUITransformID;
    cameraID _nextCameraID;
    modelID _nextModelID;
    animationID _nextAnimationID;
    lightID _nextLightID;
    particleGenID _nextParticleGenID;
    uiPanelID _nextPanelID;
    uiLabelID _nextLabelID;
    uiButtonID _nextButtonID;
    uiTextureRectID _nextTextureRectID;
    Ogre::ImGuiOverlay* _overlay;
    std::unordered_set<std::string> _resourceGroups;

	bool _imguiSDLInitialized = false;
};