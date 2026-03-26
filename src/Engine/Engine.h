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
	* @brief Establecer el cono de luz (ángulo interno, ángulo externo y suavidad de degradado)
	*/
	void setLightSpotRange(const lightID& id, float inner, float outer, float falloff);


#pragma endregion


#pragma region audio

	//Metodos del modulo de audio
	/*
	* @brief Crear un sonido en el módulo de audio.
	Recibe un path y un id, además de parámetros de configuración, como si es stream (sonido corto) o no (música), si tiene loop o si es 3D.
	*/
	void loadSound(std::string path, std::string id, bool soundStream = true, bool soundLooping = false, bool sound3D = true);
	/*
	* @brief Descarga un sonido del módulo de audio recibiendo su id.
	*/
	void unloadSound(std::string id);
	/*
	* @brief reproduce un sonido del módulo de audio recibiendo su id.
	*/
	int playSound(std::string id, float soundVolume = 0.0f, int looping = 0, const core::Vector3<> pos3 = {0.0f, 0.0f,0.0f}, const core::Vector3<> vel3 = {0.0f,0.0f,0.0f});
	void setListener(core::Vector3<> pos, core::Vector3<> forward, core::Vector3<> up, core::Vector3<> vel = { 0.0,0.0,0.0 });
	void setSourcePosition(int chID, core::Vector3<> pos, core::Vector3<> vel);
	void setChannelVolume(int chID, float newVolume = 0.0f);
	void setDelay(int chID, unsigned long long start, unsigned long long end, bool stopChannel);
	int getLooping(int chID) const;
	bool stopPlaying(int chID);
	bool pauseChannel(int chID, bool pause);
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
#pragma endregion
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
	std::function<void(std::string)> _addAndSetScene;
};