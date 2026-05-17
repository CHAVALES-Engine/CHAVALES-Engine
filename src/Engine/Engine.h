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

#include "EngineComponent.h"

/**
 * @file
 * @brief Define las funciones de la libreria estatica EngineAPI.
 */

class AudioFacade;
class PlatformModule;
class RenderModule;
class AudioModule;
class PhysicsModule;
class ComponentDLLLoader;
class StateMachine;
class InputFacade;
class RenderFacade;
class UIFacade;
class PhysicsFacade;
class ResourcesModule;

namespace core
{
	class Entity;
	class Scene;
}

using entityID = ChavalesGUID;

class ENGINE_API Engine
{
	Engine() = default;
	~Engine() = default;

public:
	bool update(uint64_t dt) const;
	void fixedUpdate(float dt) const;

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

	// ---------- STATE MACHINE
	/*
	* @brief Lanza el bucle de juego
	*/
	void startLoop() const;

	/**
	 * @brief Solicita un cambio de escena a la maquina de estados.
	 * @param n - path de la escena.
	 */
	void requestSceneChange(std::string const& n) const;

	/**
	 * @brief Acaba y cierra el juego.
	 */
	void quitGame() const;

	/**
	 * @brief Instancia un prefab en la escena.
	 * @param pref - path del prefab a instanciar.
	 */
	core::Entity* instantiatePrefab(std::string const& pref) const;

	/**
	 * @brief Devuelve la escena actual.
	 * @return std::shared_ptr <const core::Scene> - Puntero inteligente a la escena actual.
	 */
	std::shared_ptr <core::Scene> getScene() const;

	// ---------- RENDER
	/*
	* @brief Renderizar frame.
	*/
	void renderFrame();
	/*
	* @brief Limpiar escena.
	*/
	void cleanScene();

	// Metodos viewport
	/*
	* @brief Cambiar color de fondo.
	*/
	void setViewportBGColor(const core::Color& color);

	// ---------- PHYSICS
	bool rayCast(const core::Vector3<>& origin,
		const core::Vector3<>& direction,
		float maxDistance,
		RayInfo& rayInfo) const;
	std::vector<ShapeRenderData> GetPhysicsRenderData();

	void SetGravity(const core::Vector3<>& gravity = { 0.0f, -9.8f,0.0f }) const;
	/*
	* @brief Setea los gizmos para debuggear physx
	*/
	void setGizmos(bool gizmos);

	// ---------- RESOURCES
	std::pair<std::string, std::string> getAssetSourceFolder(const std::string& assetName) const;
	std::vector<std::pair<std::string, std::string>> getAllAssets() const;

	// ---------- PLATFORM
	/**
	* @brief Devuelve anchura de la ventana
	*/
	int getWindowWidth() const;

	/**
	* @brief Devuelve altura de la ventana
	*/
	int getWindowHeight() const;

	// ---------- INPUT
	const InputFacade* getInput() const { return _input; }

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

	// gizmos
	bool _gizmos = false;

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
	*	Referencia al modulo de recursos
	*/
	ResourcesModule* _resourcesModule = nullptr;

	InputFacade* _input = nullptr;

	/*
	* @brief
	*	Referencia a la maquina de estados
	*/
	StateMachine* _stateMachine;

	friend EngineComponent;
};

static const InputFacade* Input() { return Engine::instance()->getInput(); }
