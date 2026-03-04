#pragma once

#include <memory>
#include <unordered_map>

#include <EngineAPI.h>
#include <sol.hpp>

#include "Scene.h"

using sceneName = std::string;

class ENGINE_API GameLoader
{
public:
	GameLoader() = default;
	~GameLoader() = default;

	static void parseObject(const sol::object& obj, const std::string& clave, Properties& props);

	static void parseComponent(core::Entity* e, std::pair<sol::object, sol::object>& componenteObj);

	static void parseEntity(core::Entity* e, std::pair<sol::object, sol::object>& entidadObj);


	static void loadLua(std::shared_ptr<core::Scene>& s, const sceneName& n, const std::string& p = "./game/scenes/");

	static std::shared_ptr<core::Scene> loadScene(const sceneName& n);

	// carga una escena dada
	static std::shared_ptr<core::Scene> loadScene(const sceneName& n, const std::string& path);

	// carga ficheros de lua
	// para cargar todas las escenas de una vez
	bool load(std::string& path);

	/*
	Entity e
	e.addComponent(c)

	Scene s
	s.addEntity(e)

	StateMachine
	sm.addScene(s)
	*/

private:
	// mapa de nombre de escena - contenidos de la escena
	// como guardar los contenidos? 
	//std::unordered_map<sceneName, > _scenesContent;
};