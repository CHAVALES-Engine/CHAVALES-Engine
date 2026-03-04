#include "GameLoader.h"
#include <fstream>
#include <sol.hpp>
#include "Debug.h"
#include "Entity.h"
#include "Component.h"
#include <filesystem>
#include "ComponentRegister.h"
namespace fs = std::filesystem;

using Property = std::variant<
	int,
	float,
	bool,
	std::string
	//Vector3
	//Vector4
	//Quaternion
	//Color
	//...
>;
using Properties = std::unordered_map<std::string, Property>;

void GameLoader::loadLua(
	std::shared_ptr<core::Scene>& s, 
	const sceneName& n,  
	const std::string& p)
{
	sol::state lua;
	std::string path = p + n + ".lua";

	try
	{ // intenta leer archivo
		lua.safe_script_file(path);
	}
	catch (const sol::error& e)
	{ // si no lo consigue saca error
		Debug::error("GAMELOADER: Error cargando escena: ", path);
		Debug::error("Lua exception: ", e.what());
		s = nullptr;
		return;
	}

	// --- lectura lua
	sol::table scene = lua["scene"];
	Debug::out("GAMELOADER: Escena ", n, " cargada.");

	for (auto& entidad : scene)
	{ // --- para cada entidad leida
		// nombre de la entidad
		std::string entidadName = entidad.first.as<std::string>();
		Debug::out("GAMELOADER: Entidad ", entidadName, " cargada.");

		// crea la entidad
		core::Entity* e = new core::Entity();
		e->setName(entidadName);

		// dentro de la entidad, accedo a la tabla de componentes
		sol::table partes = entidad.second;
		sol::table componentes = partes["components"];

		for (auto& componente : componentes)
		{ // --- para cada componente de la tabla de componentes de la entidad
			// encontramos su nombre e intentamos instanciar un componente con una clase con tal nombre
			std::string componenteName = componente.first.as<std::string>();
			std::shared_ptr<core::Component> component = ComponentRegister::instance().create(componenteName);

			if (component != nullptr)
			{ // si este tipo de componente estaba registrado y se ha creado
				// --- a este nivel va el init:
				// inicializacion de los parametros de un componente a traves de los datos de lua
				//component->init(...);

				// --- mete el componente a la entidad creada
				e->addComponent(std::move(component));
				Debug::out("GAMELOADER: Componente ", componenteName, " cargado para la entidad ", entidadName, ".");
			}
			else
			{ // si no se ha conseguido crear el componente porque no estaba bien registrado
				Debug::warning("GAMELOADER: Componente ", componenteName, " no registrado.");
			}
		}

		// --- a este nivel se llamaria al awake:
		// metodo de logica de un componente sin garantizar que el resto de componentes y entidades esten inicializados
		//for (auto& c : e->getComponents())
		//{
		//	//c->awake();
		//}

		// --- mete la entidad en la escena
		s->addEntity(e);
	}

	// --- a este nivel se llama al ready:
	// garantizamos que en el ready el resto de entidades y sus componentes estan inicializados
	s->ready();
}

std::shared_ptr<core::Scene> GameLoader::loadScene(const sceneName& n)
{
	std::shared_ptr<core::Scene> s = std::make_shared<core::Scene>(n);
	
	loadLua(s, n);

	return s;
}

std::shared_ptr<core::Scene> GameLoader::loadScene(const sceneName& n, const std::string& path)
{
	std::shared_ptr<core::Scene> scn = std::make_shared<core::Scene>(n);

	std::ifstream entrada(path);

	if (!entrada.is_open())
	{
		Debug::error("Error abriendo archivo ", path);
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
			Debug::error("Error abriendo archivo ", path);
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