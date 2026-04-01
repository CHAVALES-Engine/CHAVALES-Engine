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
class InputFacade;

namespace core
{
	class Scene;
	//class Vector3<>;
}

using entityID = uint64_t;
using transformID = uint64_t;
using cameraID = uint64_t;
using modelID = uint64_t;
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

	//Metodos modelos
#pragma region model
	/*
	* @brief anade un modelo a la escena.
	*/
	void addModel(const entityID& entityID, const std::string& modelFolder, const std::string& modelFile);
	/*
	* @brief Borra un modelo de la escena.
	*/
	void deleteModel(const modelID& id);
	/*
	* @brief Borra un modelo de la escena.
	*/
	void setSubmeshDiffuse(const modelID& id, const std::string& textureFolder, const std::string& textureFile, const int& submesh);
	/*
	* @brief Establecer tinte de material.
	*/
	void setSubmeshTint(const modelID& id, const core::Color& tint, const int& submesh);
	/*
	* @brief Establecer si el modelo es visible.
	*/
	void setModelVisible(const modelID& id, const bool& visible);
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
	*	Referencia a la maquina de estados
	*/
	StateMachine* _stateMachine = nullptr;
};