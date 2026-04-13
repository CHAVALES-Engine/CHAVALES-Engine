#pragma once
#include <functional>
#include <string>
#include "EngineAPI.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Quaternion.h"
#include "Color.h"
#include "PhysicsTypes.h"


#include "InputDefs.h"
#include "guid.h"
#include <CommonEnums.h>
/*
 * @file Engine.h
 * @brief Defines the functions for the EngineAPI static library.
 */

class PlatformModule;
class RenderModule;
class AudioModule;
class PhysicsModule;
class ComponentDLLLoader;
class StateMachine;
class InputFacade;
class ResourcesModule;
class ResourcesFacade;


namespace core 
{
	class Scene;
	//class Vector3<>;
}
 
using entityID = ChavalesGUID;
using transformID = uint64_t;
using cameraID = uint64_t;
using modelID = uint64_t;
using animationID = uint64_t;
using lightID = uint64_t;
using particleGenID = uint64_t;
using uiPanelID = uint64_t;
using uiLabelID = uint64_t;
using uiButtonID = uint64_t;
using uiTextureRectID = uint64_t;

class ENGINE_API Engine
{
	~Engine() = default;
public:
	void update(float dt);
	/*
	* @brief Inicializacion del motor
	* @return bool - True si se ha inicializado correctamente
	*/
	static bool init();
	/*
	* @brief Devuelve la instancia del motor
	* @return Engine* - Instancia del motor
	*/
	static Engine* instance();
	/*
	* @brief Cierre limpio de los modulos del motor
	*/
	static void release();
	/*
	* @brief Lanza el bucle de juego
	*/
	void startLoop() const;
	/*
	* @brief Metodo que sincroniza los eventos de input.
	* @return bool - Booleano para saber si se ha cerrado la ventana.
	*/
	bool pollEvents() const;
	/**
	 *
	 */
	const void addAndSetScene(std::string n) const;

	//Metodos del modulo de render
#pragma region Render
	//Metodos generales
#pragma region general
	/*
	* @brief Renderizar frame.
	*/
	void renderFrame();
	/*
	* @brief Limpiar escena.
	*/
	void cleanScene();
#pragma endregion

	//Metodos viewport
#pragma region viewport
	/*
	* @brief Cambiar color de fondo.
	*/
	void setViewportBGColor(core::Color color);
#pragma endregion

	//Metodos transform
#pragma region transform
	/*
	* @brief Anadir nodo.
	*/
	transformID addTransform(const entityID& entityID, const core::Vector3<float>& pos = core::Vector3<float>(0.0f, 0.0f, 0.0f), const core::Quaternion<float>& rot = core::Quaternion<float>(0.0f, 0.0f, 0.0f, 1.0f), const core::Vector3<float>& scale = core::Vector3<float>(1.0f, 1.0f, 1.0f), const TransformType type = TransformType::WORLD);
	/*
	* @brief Establecer posicion del nodo.
	*/
	void setTransformPosition(const transformID& id, const core::Vector3<float>& pos);
	/*
	* @brief Establecer orientacion del nodo. Relativo a world space.
	*/
	void setTransformRotation(const transformID& id, const core::Quaternion<float>& rot);
	/*
	* @brief Establecer escala del nodo. Relativo a world space.
	*/
	void setTransformScale(const transformID& id, const core::Vector3<float>& scale);
#pragma endregion

	//Metodos camaras
#pragma region camera
	/*
	* @brief Camara nueva. Se asigna un id por orden de creacion. Main Camera id 0 y a�adidas manualmente 1 en adelante.
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
#pragma endregion

	//Metodos modelos
#pragma region model
	/*
	* @brief anade un modelo a la escena.
	*/
	modelID addModel(const entityID& entityID, const std::string& modelName);
	/*
	* @brief Borra un modelo de la escena.
	*/
	void deleteModel(const modelID& id);
	/*
	* @brief Borra un modelo de la escena.
	*/
	void setSubmeshDiffuse(const modelID& id, const std::string& textureName, const int& submesh);
	/*
	* @brief Establecer tinte de material.
	*/
	void setSubmeshTint(const modelID& id, const core::Color& tint, const int& submesh);
	/*
	* @brief Establecer si el modelo es visible.
	*/
	void setModelVisible(const modelID& id, const bool& visible);
#pragma endregion

	//Metodos animaciones
#pragma region animation
	/*
	* @brief Anadir animator.
	*/
	void addAnimator(const entityID& entityID, modelID& modelID);
	/*
	* @brief Registrar animacion de esqueleto.
	*/
	animationID registerSkeletonAnim(const modelID& modelID, const std::string& animationName, const bool& loop);
	/*
	* @brief Crear animacion de transform.
	*/
	animationID createTransformAnimation(const entityID& entityID, const std::string& animationName, const bool& loop, const float& totalDuration);
	/*
	* @brief Anadir keyframe a animacion de transform. Time pos en segundos.
	*/
	void addTransformKeyFrame(const animationID& animationID,
							  const float& timePos, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale);
	/*
	* @brief Anadir keyframe a animacion de transform con rotacion sencilla. Time pos en segundos.
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
#pragma endregion

	//Metodos luces
#pragma region light
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
	* @brief Establecer el tipo de luz
	*/
	void setLightType(const lightID& id, const int& type);
	/*
	* @brief Establecer el color de la luz
	*/
	void setLightColor(const lightID& id, const core::Color& color);
	/*
	* @brief Establecer la intensidad de luz
	*/
	void setLightIntensity(const lightID& id, const float& intensity);
	/*
	* @brief Establecer el cono de luz (ángulo interno, ángulo externo y suavidad de degradado)
	*/
	void setLightSpotRange(const lightID& id, const float& inner, const float& outer, const float& falloff);
#pragma endregion

	//Metodos particulas
#pragma region particle
	//Metodos particulas
	/*
	* @brief Anadir generador de particulas.
	*/
	particleGenID addParticleGen(const entityID& entityID, const std::string& textureName);
	/*
	* @brief Borrar generador de particulas.
	*/
	void deleteParticleGen(const particleGenID& id);
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
#pragma endregion
#pragma region UI
#pragma region UI-Panels
	/*
	* @brief Anadir panel de UI.
	*/
	uiPanelID addUIPanel(const entityID& entityID, const std::string& title);
	/*
	* @brief Establecer visibilidad del panel de UI.
	*/
	void setUIPanelVisible(const uiPanelID& id, bool visible);
#pragma endregion
#pragma region UI-Labels
	/*
	* @brief Anadir letrero al panel.
	*/
	uiLabelID addUILabel(const std::string& panelName, const entityID& entityID, const std::string& text, const  float opacity, const  core::Vector2<float> size, const core::Color textColor, const core::Color bgColor, const float fontSize, const TextAlign textAlign, const std::string fontName);
	/*
	* @brief Establecer el texto del letrero.
	*/
	void setUILabelText(const uiLabelID& uiLabelID, const std::string& text);
	/*
	* @brief Establecer visibilidad del letrero.
	*/
	void setUILabelVisible(const uiLabelID& uiLabelID, bool visible);
	/*
	* @brief Establecer la opacidad  del letrero
	*/
	void setUILabelOpacity(const uiLabelID& labelID, float opacity);
	/*
  * @brief Establecer las dimensiones  del letrero 
  */
	void setUILabelDimension(const uiLabelID& labelID, core::Vector2<float> dimension);
	/*
	* @brief Establecer el color del texto  del letrero
   */
	void setUILabelTextColor(const uiLabelID labelID, core::Color color);
	/*
	* @brief Establecer el color del fondo  del letrero
	*/
	void setUILabelBackGroundColor(const uiLabelID labelID, core::Color color);
	/*
	* @brief Establecer el alineado  del letrero
	*/
	void setUILabelAlign(const uiLabelID labelID, const std::string& align);
	/*
	* @brief Establecer la tipografia  del letrero
	*/
	//void setUILabelFont(const uiLabelID id, ImFont* font);
#pragma endregion
#pragma region UI-Buttons
	/*
	* @brief Anadir boton al panel.
	*/
	uiButtonID addUIButton(const std::string& panelName, const entityID& entityID, const std::string& text, core::Vector2<float> size);

	/*
	* @brief Anadir ImageBoton al panel.
	*/
	uiButtonID addUIImageButton(const std::string& panelName, const entityID& entityID, const std::string& text, const std::string& textureName, core::Vector2<float> size);
	/*
	* @brief Establecer el texto del boton.
	*/
	void setUIButtonText(const uiButtonID& id, const std::string& text);
	/*
	* @brief Establecer visibilidad del boton.
	*/
	void setUIButtonVisible(const uiButtonID& id, bool visible);
	/*
	* @brief Establecer textura del boton.
	*/
	void setUIButtonTexture(const uiButtonID& id,const std::string& texture);
	/*
	* @brief Establecer dimensiones del boton.
	*/
	void setUIButtonDimension(const uiButtonID& id,core::Vector2<float> dimension);
	/*
   * @brief Establecer la opacidad  del boton
   */
	void  setUIButtonOpacity(const uiButtonID& buttonID, float opacity);
	/*
	* @brief Establecer callback del boton.
	*/
	void setUIButtonCallback(const uiButtonID& id, std::function<void()> callback);

#pragma endregion
#pragma region UI-TextureRect
	/*
	* @brief Anadir textureRect al panel.
	*/
	uiTextureRectID addUITextureRect(const std::string& panelName, const entityID& entityID,  const std::string& textureName, core::Vector2<float> size);
	/*
	* @brief Establecer textura del textureRect.
	*/
	void setUITextureRectTexture(const uiTextureRectID& id, const std::string& texture);
	/*
	* @brief Establecer dimensiones del textureRect.
	*/
	void setUITextureRectDimension(const uiTextureRectID& id, core::Vector2<float> dimension);
	/*
	* @brief Establecer visibilidad del textureRect.
	*/
	void setUITextureRectVisible(const uiTextureRectID& id, bool visible);
	/*
	* @brief Establecer la opacidad  del textureRect
	*/
	void  setUITextureRectOpacity(const uiTextureRectID& textureRectID, float opacity);

#pragma endregion
#pragma endregion
#pragma endregion

	//Metodos audio
#pragma region audio

	//Metodos del modulo de audio
	/*
	* @brief Crea un sonido en el módulo de audio.
	Recibe un path y un id, además de parámetros de configuración, como si es stream (sonido corto) o no (música), si tiene loop o si es 3D.
	*/
	void loadSound(std::string path, std::string id, bool soundStream = true, bool soundLooping = false, bool sound3D = true);
	/*
	* @brief Descarga un sonido del módulo de audio recibiendo su id.
	*/
	void unloadSound(std::string id);
	/*
	* @brief Reproduce un sonido del módulo de audio recibiendo su id y su configuración: volumen, loop (si creado con looping: -1 = indef, 0 = one time, 1 = loop once), posición y velocidad (para audio 3D)
	*/
	int playSound(std::string id, float soundVolume, int looping = 0, const core::Vector3<> pos3 = {0.0f, 0.0f,0.0f}, const core::Vector3<> vel3 = {0.0f,0.0f,0.0f});
	/*
	* @brief Configura en el módulo de audio el listener de la escena, recibiendo su posicion, forward y up, y adicionalmente la velocidad para el audio 3D (efecto Doppler)
	*/
	void setListener(core::Vector3<> pos, core::Vector3<> forward, core::Vector3<> up, core::Vector3<> vel = { 0.0,0.0,0.0 });
	/*
	* @brief Actualiza la posición y velocidad de un audio 3D
	*/
	void setSourcePosition(int chID, core::Vector3<> pos, core::Vector3<> vel);
	/*
	* @brief Actualiza el volumen de un canal
	*/
	void setChannelVolume(int chID, float newVolume = 0.0f);
	/*
	* @brief Devuelve la configuracion de loopeo que tiene un audio
	*/
	int getLooping(int chID) const;
	/*
	* @brief Detiene y libera un canal
	*/
	bool stopPlaying(int chID);
	/*
	* @brief Pausa o reanuda un canal y el audio que reproduce
	*/
	bool pauseChannel(int chID, bool pause);
	/*
	* @brief Configura el milisegundo de inicio y de final del audio que se reproduciran
	*/
	void setDelay(int chID, double start, double end, bool stopChannel);
	/*
	* @brief Devuelve si un canal esta pausado (false) o en reproduccion (true)
	*/
	bool isChannelPlaying(int chID);
	void setLooping(int chID,int typeOfLooping);
	float getVolume(int chID);

#pragma endregion


#pragma region Physics

	
	/*
	* @brief Devuelve el id de la entidad que tiene el boxcollider y lo crea
	*/
	uint32_t createBoxCollider(const core::Vector3<>& size, const core::Vector3<>& pos, bool isDynamic,  bool isTrigger);
	/*
	* @brief Setea la posicion fisica de la entidad
	*/
	void setPhysicsPosition( uint32_t id, const core::Vector3<>& pos);
	/*
	* @brief Coge la posicion del transform de la entidad en cuando a physx para poder mover el transform de la entidad y que se vea en nuestra escena
	*/
	core::Vector3<> getPhysicsPosition(uint32_t id);
	/*
	* @brief Devuelve el id de la entidad que tiene el capsulecollider y lo crea
	*/
	uint32_t createCapsuleCollider(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, bool isDynamic, bool isTrigger);

	std::vector<PhysicsEvent> getPhysicsEvents(ComponentID id);
	void clearPhysicsEvents();

	ComponentID attachBoxShapeToRigidBody(ComponentID bodyID, const core::Vector3<> size, const core::Vector3<>& center, bool isTrigger);

	ComponentID attachCapsuleShapeToRigidBody(ComponentID bodyID, float radius, float height, const core::Vector3<>& center, bool isTrigger);

	void setPhysicsTransform(ComponentID id, const core::Vector3<>& pos, const core::Quaternion<>& rot);

	uint32_t createRigidBody(core::Vector3<> pos, float mass, bool useGravity, bool isKinematic);

	core::Vector3<> getLinearVelocity(uint32_t id);

	void setLinearVelocity(uint32_t id, core::Vector3<> vel);

	void setMass(uint32_t id, float mass);
	float getMass(uint32_t id);

	void setLinearDamping(uint32_t id, float damping);
	float getLinearDamping(uint32_t id);

	void addForce(uint32_t id, core::Vector3<> force, char mode);
	void clearForce(uint32_t id, char mode);

	uint32_t createMaterial(float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);

	void updateMaterial(uint32_t id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);

	bool rayCast(const core::Vector3<>& origin,
		const core::Vector3<>& direction,
		float maxDistance);

#pragma endregion

#pragma region Resources

	ResourcesFacade* resources() const;

#pragma endregion

	/**
	* @brief Devuelve anchura de la ventana
	*/
	int getWindowWidth() const;
	/**
	* @brief Devuelve altura de la ventana
	*/
	int getWindowHeight() const;

	InputFacade* input() const;

private:
	/*
	* @brief
	*	Inicializador privado de modulos
	*/
	bool _initPriv();
	/*
	* @brief
	*	Instancia estatica de la clase
	*/
	static Engine* _instance;
	/*
	* @brief
	*	Referencia al modulo de platform
	*/
	PlatformModule* _platformModule = nullptr;
	/**
	 * @brief Referencia a la api publica del input
	 */
	InputFacade* _input;
	/*
	* @brief
	*	Referencia al modulo de render
	*/
	RenderModule* _renderModule = nullptr;
	/*
	* @brief
	*	Referencia al modulo de audio
	*/
	AudioModule* _audioModule = nullptr;
	/*
	* @brief
	*	Referencia al modulo de fisica
	*/
	PhysicsModule* _physicsModule = nullptr;
	/*
	* @brief
	*	Referencia al modulo de recursos
	*/
	ResourcesModule* _resourcesModule = nullptr;
	ResourcesFacade* _resources;
	/*
	* @brief
	*	Referencia a la maquina de estados
	*/
	StateMachine* _stateMachine = nullptr;
};