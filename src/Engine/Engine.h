#pragma once
#include <functional>
#include <string>
#include "EngineAPI.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Color.h"
/*
 * @file Engine.h
 * @brief Defines the functions for the EngineAPI static library.
 */

class PlatformModule;
class RenderModule;
class AudioModule;
class PhysicsModule;

namespace core
{
	class Scene;
}

using entityID = uint64_t;
using transformID = uint64_t;
using cameraID = uint64_t;

class ENGINE_API Engine
{
	~Engine() = default;
public:
	/*
	* @brief
	*	Inicializacion de modulos
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
	* @brief Metodo que sincroniza los modulos con el juego
	*/
	const bool syncronize();
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
#pragma endregion
#pragma endregion

	//Metodos del modulo de audio
	void loadSound(const char* path, std::string id, bool sound3D = true, bool soundLooping = false, bool soundStream = false);
	void unloadSound(std::string id);
	int playSound(std::string id, const core::Vector3<> vec3 = { 0.0f,0.0f,0.0f }, float soundVolume = 0.0f, int looping = 0);
	void setChannelVolume(int chID, float newVolume = 0.0f);
	void getLooping(int chID, int* typeOfLooping);
	void setListener(core::Vector3<> pos, core::Vector3<> forward, core::Vector3<> up, core::Vector3<> vel = { 0.0,0.0,0.0 });

	bool stopPlaying(int chID);

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
	std::function<void(std::string)> _addAndSetScene;
};