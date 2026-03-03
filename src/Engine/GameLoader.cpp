#include "GameLoader.h"

#include <fstream>
#include <sol.hpp>
#include "Debug.h"
#include "Entity.h"
#include "Component.h"
#include <filesystem>

#include "ComponentRegister.h"
namespace fs = std::filesystem;

void GameLoader::loadLua(
	std::shared_ptr<core::Scene>& s, 
	const sceneName& n,  
	const std::string& p)
{
	sol::state lua;
	std::string path = p + n + ".lua";

	try
	{
		lua.safe_script_file(path);
	}
	catch (const sol::error& e)
	{
		core::Debug::error("GAMELOADER: Error cargando escena: ", path);
		core::Debug::error("Lua exception: ", e.what());
		return;
	}

	sol::table scene = lua["scene"];
	core::Debug::out("GAMELOADER: Escena ", n, " cargada.");

	for (auto& entidad : scene)
	{
		// nombre de la entidad
		std::string entidadName = entidad.first.as<std::string>();
		core::Debug::out("GAMELOADER: Entidad ", entidadName, " cargada.");
		// crea la entidad
		core::Entity* e = new core::Entity();
		e->setName(entidadName);

		// 
		sol::table partes = entidad.second;
		sol::table componentes = partes["components"];
		for (auto& componente : componentes)
		{
			auto componenteObj = componente.first;
			std::string componenteName = componenteObj.as<std::string>();
			std::shared_ptr<core::Component> component = ComponentRegister::instance().create(componenteName);

			if (component != nullptr)
			{
				// mete el componente a la entidad creada
				e->addComponent(std::move(component));
				core::Debug::out("GAMELOADER: Componente ", componenteName, " cargado para la entidad ", entidadName, ".");
			}
			else
			{
				core::Debug::warning("GAMELOADER: Componente ", componenteName, " no registrado.");
			}
		}

		// mete la entidad en la escena
		s->addEntity(e);
	}
}

std::shared_ptr<core::Scene> GameLoader::loadScene(const sceneName& n)
{
	std::shared_ptr<core::Scene> s = std::make_shared<core::Scene>(n);

	// comprobar que no haya una escena con ese nombre ya
	/*
	auto itS = _scenesContent.find(n);
	if (itS == _scenesContent.end())
	{
		core::Debug::error("No existe escena con nombre ", n);
	}
	else
	{
		scn = new core::Scene(n);

		for (auto e : _scenesContent)
		{
			
		}
	}
	*/
	
	loadLua(s, n);

	return s;
}

std::shared_ptr<core::Scene> GameLoader::loadScene(const sceneName& n, const std::string& path)
{
	std::shared_ptr<core::Scene> scn = std::make_shared<core::Scene>(n);

	std::ifstream entrada(path);

	if (!entrada.is_open())
	{
		core::Debug::error("Error abriendo archivo ", path);
	}
	else
	{
		//auto cinbuf = std::cin.rdbuf(entrada.rdbuf());
		//std::string entidadName;
		//std::cin >> entidadName;
		//std::cin.rdbuf(cinbuf);

		// the type "sol::state" behaves exactly like a table!
		//sol::state lua;
		//lua.script_file(path);
		//bool isfullscreen = lua["config"]["fullscreen"]; // can get nested variables
		//sol::table config = lua["config"];
		//assert(!isfullscreen)

		loadLua(scn, n, path);
	}

	return scn;
}

bool GameLoader::load(std::string& path)
{
	// recorrer todos los archivos de lua del directorio scenes
	path = "./game/scenes";
	std::string luaFile;
	for (const auto& entry : fs::directory_iterator(path))
	{
		luaFile = entry.path().generic_string();

		std::ifstream entrada(luaFile);

		if (!entrada.is_open())
		{
			core::Debug::error("Error abriendo archivo ", path);
			return false;
		}

		//auto cinbuf = std::cin.rdbuf(entrada.rdbuf());
		//std::string entidadName;
		//std::cin >> entidadName;
		//std::cin.rdbuf(cinbuf);

		// the type "sol::state" behaves exactly like a table!
		//sol::state lua;
		//lua.script_file(luaFile);
		//bool isfullscreen = lua["config"]["fullscreen"]; // can get nested variables
		//sol::table config = lua["config"];
		//assert(!isfullscreen)


	}

    return true;
}