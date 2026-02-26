/*
 * @file
 * @brief
 */
#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class Scene;
class Timing;

using sceneID = uint64_t;
using sceneName = std::string;
using scenePtr = std::shared_ptr<Scene>;
using scene = std::pair<sceneID, scenePtr>;

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

	void addScene(sceneName n, scenePtr s);
	void setScene(sceneID s);

	void deleteScene(sceneID s);

	/**
	 * @brief Devuelve el identificador de la escena activa actualmente.
	 */
	inline sceneID getCurrentScnID() const {
		return _currentScene.first;
	}

	/**
	 * @brief Devuelve un puntero a la escena activa actualmente.
	 */
	inline scenePtr getCurrentScnPtr() const {
		return _currentScene.second;
	}

	/**
	 * @brief Devuelve el pair<id,ptr> de la escena activa actualmente.
	 */
	inline scene getCurrentScn() const {
		return _currentScene;
	}

private:
	/**
	 * @brief Devuelve el ID interno asociado a la clave n.
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

	static uint64_t _getNextId() { return ++_nextId; }
};

