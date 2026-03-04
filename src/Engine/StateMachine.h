/*
 * @file
 * @brief
 */
#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace core
{
	class Scene;
	class Clock;
}

using sceneID = uint64_t;
using sceneName = std::string;
using scenePtr = std::shared_ptr<core::Scene>;

struct scene 
{
	sceneID id = 0;
	scenePtr ptr = nullptr;
	sceneName name = " ";
};

using stateMachine = std::unordered_map<sceneID, scenePtr>;
using nameToID = std::unordered_map<sceneName, sceneID>;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	/**
	 * @brief Bucle principal.
	 */
	void gameLoop();

	/**
	 * @brief Inserta una escena nueva en la maquina de estados.
	 * @param n - Nombre de la escena a insertar.
	 * @param s - Puntero a la escena a insertar.
	 */
	void addScene(sceneName n, scenePtr s); // DEPRECATED

	/**
	 * @brief Setea la escena n como escena activa actualmente.
	 * @param n - Nombre de la escena a activar.
	 */
	void setScene(sceneName n); // DEPRECATED

	/**
	 * @brief Inserta una escena nueva en la maquina de estados y la setea como escena activa actualmente.
	 * @param n - Nombre de la escena a insertar.
	 * @param s - Puntero a la escena a insertar.
	 */
	void addAndSetScene(const sceneName& n); // <-

	/**
	 * @brief Inserta la escena n de la maquina de estados.
	 * @param n - Nombre de la escena a eliminar.
	 */
	void deleteScene(sceneName n);

	/**
	 * @brief Devuelve el identificador de la escena activa actualmente.
	 */
	inline sceneID getCurrentScnID() const {
		return _currentScene.id;
	}

	/**
	 * @brief Devuelve el nombre de la escena activa actualmente.
	 */
	inline sceneName getCurrentScnName() const {
		return _currentScene.name;
	}

	/**
	 * @brief Devuelve un puntero a la escena activa actualmente.
	 */
	inline scenePtr getCurrentScnPtr() const {
		return _currentScene.ptr;
	}

	/**
	 * @brief Devuelve la escena activa actualmente.
	 */
	inline scene getCurrentScn() const {
		return _currentScene;
	}

private:
	/**
	 * @brief Devuelve el ID interno asociado a la clave n.
	 * @param n - Nombre de la escena a traducir.
	 */
	uint64_t _parseNameToID(std::string n);

	/**
	 * @brief Tiempo desde la ultima actualizacion.
	 */
	uint64_t _deltaTime;

	/**
	 * @brief Mapa de escenas.
	 */
	stateMachine _stateMachine;

	/**
	 * @brief Mapa de nombres de escenas e ID.
	 */
	nameToID _nameToID;

	/**
	 * @brief ID y referencia de la escena activa actualmente.
	 */
	scene _currentScene;

	/**
	 * @brief Booleano de control de ejecucion del bucle de juego.
	 */
	bool _endGame = false;

	/**
	 * @brief Booleano de control de ejecucion del bucle de juego.
	 */
	static uint64_t _nextId;

	/**
	 * @brief Genera nuevos identificadores.
	 */
	static uint64_t _getNextId() { return ++_nextId; }
};

