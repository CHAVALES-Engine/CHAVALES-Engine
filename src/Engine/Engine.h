#pragma once
#include "NetworkDefs.h"
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
class NetworkModule;
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

	/**
	* @brief Inicializacion del motor
	* @return bool - True si se ha inicializado correctamente
	*/
	static bool init();

	/**
	* @brief Devuelve la instancia del motor
	* @return Engine* - Instancia del motor
	*/
	static Engine* instance();

	/**
	* @brief Cierre limpio de los modulos del motor
	*/
	static void release();

	// ---------- STATE MACHINE
	/**
	* @brief Lanza el bucle de juego
	*/
	void startLoop() const;

	/**
	 * @brief Solicita un cambio de escena a la maquina de estados.
	 * @param n - path de la escena.
	 */
	void requestSceneChange(std::string const& n, const bool loadingScreen = false) const;

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
	/**
	* @brief Renderizar frame.
	*/
	bool renderFrame();

	/**
	* @brief Establece nuevo progreso para la pantalla de carga.
	*/
	bool setLoadingScreenProcedures(const int& n);

	/**
	* @brief Establece barra de progreso a 0 si existe pantalla de carga. Devuelve false en caso negativo.
	*/
	bool initLoadingScreen();

	/**
	* @brief Actualiza la barra de progreso.
	*/
	void increaseLoadingScreen(const int& n);

	/**
	* @brief Renderizar pantalla de carga.
	*/
	bool renderLoadingScreen();

	/**
	* @brief Limpiar escena.
	*/
	void cleanScene();

	// Metodos viewport
	/**
	* @brief Cambiar color de fondo.
	*/
	void setViewportBGColor(const core::Color& color);

	/**
	* @brief Devuelve por parametros el rectangulo real del viewport en pixeles de ventana, con la escala de la ventana aplicada.
	* @param x - Posicion X del viewport.
	* @param y - Posicion Y del viewport.
	* @param w - Ancho del viewport.
	* @param h - Alto del viewport.
	* @return bool - true si los datos son validos.
	*/
	bool getViewportRect(int& x, int& y, int& w, int& h) const;
	/**
	* @brief Devuelve la resolucion de referencia lógica (base) del render.
	*/
	core::Vector2<> getLogicResolution() const;
	/**
	* @brief Convierte coordenadas de ventana (pixeles reales) a coordenadas de render base lógicas.
	* @param windowPos - posicion en la ventana real de SDL, no logica (y lo que devuelve Input).
	* @return renderPos - salida en coordenadas de render base, coordenadas logicas, con el viewport a escala 1.
	*/
	core::Vector2<> windowToLogicCoords(const core::Vector2<>& windowPos) const;

	// ---------- PHYSICS
	bool rayCast(const core::Vector3<>& origin,
		const core::Vector3<>& direction,
		float maxDistance,
		RayInfo& rayInfo) const;
	std::vector<ShapeRenderData> GetPhysicsRenderData();

	void SetGravity(const core::Vector3<>& gravity) const;
	/**
	* @brief Setea los gizmos para debuggear physx
	*/
	void setGizmos(bool gizmos);

	// ---------- RESOURCES
	std::string getAssetSourceFolder(const std::string& assetName) const;
	std::vector<std::pair<std::string, std::string>> getAllAssets() const;
	bool preload(const std::string& path);
	bool unload(const std::string& path);
	bool preloadAll();


	// ---------- PLATFORM
	/**
	* @brief Devuelve anchura de la ventana
	*/
	int getWindowWidth() const;

	/**
	* @brief Devuelve altura de la ventana
	*/
	int getWindowHeight() const;
	/**
	* @brief Activa o desactiva que la ventana sea redimensionable.
	*/
	void setWindowResizable(bool enabled) const;
	/**
	* @brief Activa o desactiva que la ventana sea maximizable.
	*/
	void setWindowMaximizable(bool enabled) const;
	/**
	* @brief Activa o desactiva pantalla completa
	* @param enabled - true para fullscreen, false para modo ventana
	* @return bool - true si se aplicó correctamente
	*/
	bool setFullscreen(bool enabled) const;
	/**
	* @brief Devuelve si la ventana está en pantalla completa
	*/
	bool isFullscreen() const;

	// ---------- INPUT
	InputFacade* getInput() const { return _input; }


	// -- NETWORK

	/**
	* @brief Inicializa la conexion.
	*
	* @returns bool - Si se ha podido inicializar o no.
	*/
	bool networkInit();

	/**
	* @brief Cierre de la conexion.
	*/
	void networkShutdown();

	/**
	* @brief Hostear una sesion.
	*
	* @param port - Puerto de la sesion.
	*
	* @returns bool - Si se ha podido hostear o no.
	*/
	bool networkHost(uint16_t port);

	/**
	* @brief Unirse a una sesion.
	*
	* @param ip - IP de la sesion.
	* @param port - Puerto de la sesion.
	*/
	void networkJoin(const std::string& ip, uint16_t port);

	/**
	* @brief Desconectarse de la sesion.
	*/
	void networkDisconnect();

	/**
	* @brief Manda un mensaje.
	*
	* @param type - Tipo dado por el desarrollador para guardar su observador al que se le quiere mandar el mensaje.
	* @param payload - Mensaje.
	*/
	template<typename T>
	void networkSend(uint8_t type, const T& payload)
	{
		_networkSendRaw(type, &payload, static_cast<int>(sizeof(T)));
	}

	/**
	* @brief Mete un observador nuevo con una funcion asociada.
	*
	* @param type - Tipo dado por el desarrollador para guardar su observador.
	* @param cb - Funcion.
	*
	* @returns NetworkObserverID - ID asociado automaticamente al nuevo observador.
	*/
	NetworkObserverID networkAddObserver(uint8_t type, PacketCallback cb);

	/**
	* @brief Desuscribe un observador.
	*
	* @param type - Tipo dado por el desarrollador para guardar su observador.
	* @param id - ID del observador a desuscribir.
	*/
	void networkUnsubscribe(uint8_t type, NetworkObserverID id);

	/**
	* @brief Vacia el mapa de observadores.
	*/
	void networkClearObservers();

	/**
	* @brief Devuelve el estado de la conexion.
	*
	* @returns NetworkState - Estado de la conexion: IDLE, WAITING, CONNECTED o FAILED.
	*/
	NetworkState networkGetNetworkState();

	/**
	* @brief Devuelve si la conexion esta establecida o no.
	*
	* @returns bool - Si esta establecida (NetworkState::CONNECTED) o no.
	*/
	bool networkIsConnected();

	/**
	* @brief Devuelve la IP local.
	*
	* @returns std::string - IP.
	*/
	std::string networkGetLocalIp();

	/**
	* @brief Devuelve el rol de quien lo llama.
	*
	* @returns NetworkRole - Rol.
	*/
	NetworkRole networkGetRole();

	// -- Fin Network.

private:
	/**
	* @brief
	*	Inicializador privado de modulos
	*/
	bool _initPriv();
	/**
	 * @brief Registra los metodos del engine en el sistema de script
	 */
	void _registerScriptBindings() const;

	/**
	 * @brief Metodo helper que manda un mensaje a traves de networkmodule sin concer su implementacion.
	 */
	void _networkSendRaw(uint8_t type, const void* data, int size);

	/**
	* @brief
	*	Instancia estatica de la clase
	*/
	static Engine* _instance;

	// gizmos
	bool _gizmos = false;

	/**
	* @brief
	*	Referencia al modulo de platform
	*/
	PlatformModule* _platformModule = nullptr;

	/**
	* @brief
	*	Referencia al modulo de render
	*/
	RenderModule* _renderModule = nullptr;

	/**
	* @brief
	*	Referencia al modulo de audio
	*/
	AudioModule* _audioModule = nullptr;

	/**
	* @brief
	*	Referencia al modulo de fisica
	*/
	PhysicsModule* _physicsModule = nullptr;

	/**
	* @brief
	*	Referencia al modulo de recursos
	*/
	ResourcesModule* _resourcesModule = nullptr;

	InputFacade* _input = nullptr;

	/**
	* @brief
	*	Referencia a la maquina de estados
	*/
	StateMachine* _stateMachine = nullptr;
	
	/**
	* @brief
	*	Referencia a network.
	*/
	NetworkModule* _networkModule = nullptr;

	friend EngineComponent;
};

static InputFacade* Input() { return Engine::instance()->getInput(); }
