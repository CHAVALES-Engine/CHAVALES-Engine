#pragma once
#include <functional>
#include <string>
#include "EngineAPI.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Color.h"
#include <Vector3.h>

#include "InputDefs.h"
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
namespace core
{
	class Scene;
	//class Vector3<>;
}

using entityID = uint64_t;
using transformID = uint64_t;
using cameraID = uint64_t;
using lightID = uint64_t;

class ENGINE_API Engine
{
	~Engine() = default;
public:
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
	void startLoop();
	/*
	* @brief Metodo que sincroniza los modulos con el juego
	*/
	bool syncronize() const;
	/**
	 *
	 */
	const void addAndSetScene(std::string n) const;
	const void setAddAndSetScene(std::function<void(std::string)> func);



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
	transformID addTransform(const entityID& entityID, const core::Vector3<float>& pos = core::Vector3<float>(0.0f, 0.0f, 0.0f), const core::Quaternion<float>& rot = core::Quaternion<float>(0.0f, 0.0f, 0.0f, 1.0f), const core::Vector3<float>& scale = core::Vector3<float>(1.0f, 1.0f, 1.0f));
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

#pragma region light
	/*
	* @brief Luz nueva. Se asigna un id por orden de creacion. Main Luz id 0 y añadidas manualmente 1 en adelante.
	*/
	lightID addLight(const entityID& entityID, int type, const core::Color& color, float intensity);
	/*
	* @brief Borrar luz por id. A las luces creadas posteriormente se les resta el id en 1.
	*/
	void deleteLight(const lightID& id);
	/*
	* @brief activar/descativar camara
	*/
	void setLightActive(const lightID& id, bool active);
	/*
	* @brief activar/descativar camara
	*/
	void cleanLights();
	/*
	* @brief Establecer el tipo de luz
	*/
	void setLightType(const lightID& id, int type);
	/*
	* @brief Establecer el color de la luz
	*/
	void setLightColor(const lightID& id, const core::Color& color);
	/*
	* @brief Establecer la intensidad de luz
	*/
	void setLightIntensity(const lightID& id, float intensity);
	/*
	* @brief Establecer la direccion de luz
	*/
	void setLightDirection(const lightID& id, const core::Vector3<float>& dir);
	/*
	* @brief Establecer el cono de luz (ángulo interno, ángulo externo y suavidad de degradado)
	*/
	void setLightSpotRange(const lightID& id, float inner, float outer, float falloff);


#pragma endregion


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

#pragma endregion

#pragma region Platform

	//------Metodos de PlatformModule:

	/**
	* @brief Devuelve anchura de la ventana
	*/
	int getWindowWidth() const;
	/**
	* @brief Devuelve altura de la ventana
	*/
	int getWindowHeight() const;
	/*
	* @brief Devuelve si una tecla esta pulsada
	* @param inputAction - InputEvent a comprobar
	* @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	*/
	bool isKeyPressed(input::InputEvent inputAction, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	* @brief Devuelve si se ha dejado de pulsar una tecla
	* @param inputAction - InputEvent a comprobar
	* @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	*/
	bool isKeyReleased(input::InputEvent inputAction, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	* @brief Devuelve cuanto de accionado esta la accion a comprobar
	* @param inputAction - InputEvent a comprobar
	* @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	* @return float - Devuelve de -1 a 1
	*/
	float getAxis(input::InputEvent inputAction, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	* @brief Devuelve si se ha pulsado una accion
	* @param actionName - accion a comprobar
	* @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	*/
	bool isActionPressed(const std::string& actionName, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	* @brief Devuelve si se ha dejado de pulsar una accion
	* @param actionName - accion a comprobar
	* @param device - id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	*/
	bool isActionReleased(const std::string& actionName, input::DeviceID device = input::ANY_DEVICE) const;
	/*
	* @brief Indica a la ventana que tome input de texto.
	*/
	void startTextInput() const;
	/*
	* @brief Indica a la ventana que deje de tomar input de texto.
	*/
	void stopTextInput() const;
	/*
	* @brief Devuelve el texto introducido por el dispositivo
	* @param device - id del dispositivo a comprobar. ANY_DEVICE por defecto => la suma del input de todos los dispositivos.
	*/
	std::string getTextInput(input::DeviceID device = input::ANY_DEVICE) const;

	//------Metodos de InputMapper:

	/**
	* @brief Mete un evento asociada a un nombre de accion.
	*
	* @param actionName - Nombre de la accion.
	* @param InputEvent - Input que lanza el evento.
	* @param id - Id del dispositivo a comprobar. -1 por defecto => el primero positivo que encuentre.
	*/
	void addEvent(const std::string& actionName, input::InputEvent inputEvent, input::DeviceID id = input::ANY_DEVICE);

	/**
	* @brief Quita una evento asociado a una accion.
	*
	* @param actionName - Accion de la que eliminar un input.
	* @param InputEvent - Evento que quitar del mapa.
	* @param id - Id del dispositivo a comprobar. -1 por defecto => elimina todos los eventos del tipo dado.
	*/
	void removeEvent(const std::string& actionName, input::InputEvent inputEvent, input::DeviceID id = input::ANY_DEVICE);
	/**
	* @brief Elimina todos los eventos asociados a una accion.
	*
	* @param actionName - Accion cuyos eventos hay que eliminar.
	*/
	void removeEvents(const std::string& actionName);
	/**
	* @brief Elimina todos los eventos asociados a una accion y a un id.
	*
	* @param actionName - Accion cuyos eventos hay que eliminar.
	* @param id - Id del dispositivo a comprobar. -1 por defecto => elimina todos los eventos de la accion (llama a removeEvents(actionName)).
	*/
	void removeEventsFromID(const std::string& actionName, input::DeviceID id = input::ANY_DEVICE);

	/**
	* @brief Devuelve todos los eventos correspondientes a una accion.
	*
	* @param actionName - Accion a consultar.
	* @param id - Id del dispositivo a comprobar. -1 por defecto => Devuelve todos los eventos de esa accion.
	*
	* @return std::vector<InputAction> - Vector de InputActions correspondientes.
	*/
	std::vector<input::InputEvent> getInputEvents(const std::string& actionName, input::DeviceID id = input::ANY_DEVICE);
	/**
	* @brief Devuelve todas las acciones.
	*
	* @return std::vector<std::string> - Nombres de acciones registradas.
	*/
	std::vector<std::string> getActions();

	/**
	* @brief Devuelve si tiene un nombre de accion registrada.
	*
	* @param actionName - Nombre de la accion a consultar.
	*
	* @return bool - Devuelve true si esta mapeada.
	*/
	bool hasAction(const std::string& actionName) const;

#pragma endregion

#pragma endregion

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
	*	Referencia al cargador de dlls
	*/
	ComponentDLLLoader* _componentDLLLoader = nullptr;
	/*
	* @brief
	*	Referencia a la maquina de estados
	*/
	StateMachine* _stateMachine = nullptr;
	std::function<void(std::string)> _addAndSetScene;
};