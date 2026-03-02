#include "GameLoader.h"

#include <fstream>
#include <sol.hpp>
#include "Debug.h"
#include "Entity.h"
#include <filesystem>

#include "ComponentRegister.h"
namespace fs = std::filesystem;

std::shared_ptr<core::Scene> GameLoader::loadScene(std::string n)
{
	std::shared_ptr<core::Scene> scn = nullptr;

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


	sol::state lua;
	lua.script_file("./scenes/" + n + ".lua");
	sol::table scene = lua["scene"];
	core::Scene* s = new core::Scene(n);

	for (auto& entidad : scene)
	{
		// nombre de la entidad
		auto entidadName = entidad.first;
		std::cout << "ENTIDAD: " << entidadName.as<std::string>() << std::endl;

		// crea la entidad
		core::Entity* e = new core::Entity();

		// 
		sol::table partes = entidad.second;
		sol::table componentes = partes["components"];
		for (auto& componente : componentes)
		{
			auto componenteObj = componente.first;
			std::string componenteName = componenteObj.as<std::string>();
			std::cout << "COMPONENTE: " << componenteName << std::endl;
			std::unique_ptr<core::Component> component = ComponentRegister::instance().create(componenteName);

			if (component != nullptr)
			{
				// mete el componente a la entidad creada
				e->addComponent(std::move(component));
			}
			else
			{
				core::Debug::warning("Componente ", componenteName, " no registrado\n");
			}
		}

		// mete la entidad en la escena
		s->addEntity(e);
	}

	return scn;
}

std::shared_ptr<core::Scene> GameLoader::loadScene(sceneName n, const std::string& path)
{
	std::shared_ptr<core::Scene> scn = nullptr;

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
		sol::state lua;
		lua.script_file(path);
		//bool isfullscreen = lua["config"]["fullscreen"]; // can get nested variables
		//sol::table config = lua["config"];
		//assert(!isfullscreen)
	}

	return scn;
}

bool GameLoader::load(std::string& path)
{
	// recorrer todos los archivos de lua del directorio scenes
	path = "./scenes";
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
		sol::state lua;
		lua.script_file(luaFile);
		//bool isfullscreen = lua["config"]["fullscreen"]; // can get nested variables
		//sol::table config = lua["config"];
		//assert(!isfullscreen)
	}

    return true;
}