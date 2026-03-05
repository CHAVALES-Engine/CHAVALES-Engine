#include "GameLoader.h"
#include <fstream>
#include <sol.hpp>
#include "Debug.h"
#include "Entity.h"
#include "Component.h"
#include <filesystem>
#include "ComponentRegister.h"
#include "EngineAPI.h"
#include "Vector3.h"
namespace fs = std::filesystem;


void GameLoader::parseObject(const sol::object& obj, const std::string& clave, Properties& props)
{
	//switch (obj.get_type())
	//{
	//case obj.is<int>(): break;
	//case obj.is<int>(): break;
	//}

	// parsear y comprobar variant
	if (obj.is<int>())
	{
		props[clave] = obj.as<int>();
		//Debug::out("GAMELOADER: Parametro ", nombreParametro, " valido de tipo int: ", objetoParametro.as<int>());
	}
	else if (obj.is<float>())
	{
		props[clave] = obj.as<float>();
		//Debug::out("GAMELOADER: Parametro ", nombreParametro, " valido de tipo float: ", objetoParametro.as<float>());
	}
	else if (obj.is<bool>())
	{
		props[clave] = obj.as<bool>();
		//Debug::out("GAMELOADER: Parametro ", nombreParametro, " valido de tipo bool: ", objetoParametro.as<bool>());
	}
	else if (obj.is<std::string>())
	{
		props[clave] = obj.as<std::string>();
		//Debug::out("GAMELOADER: Parametro ", nombreParametro, " valido de tipo string: ", objetoParametro.as<std::string>());
	}
	else
	{
		Debug::error("GAMELOADER: El tipo del parametro de ", clave, " no es valido.");
	}
}

void GameLoader::parseComponent(core::Entity* e, std::pair<sol::object, sol::object>& componenteObj)
{
	// encontramos su nombre e intentamos instanciar un componente con una clase con tal nombre
	std::string componenteName = componenteObj.first.as<std::string>();
	std::shared_ptr<core::Component> component = ComponentRegister::instance().create(componenteName);

	if (component != nullptr)
	{ // si este tipo de componente estaba registrado y se ha creado
		Properties properties;

		sol::table propertiesTable = componenteObj.second;
		for (auto& p : propertiesTable)
		{
			// nombre del parametro
			std::string nombreParametro = p.first.as<std::string>();

			// declarar variant
			auto objetoParametro = p.second;

			// traducir objeto a propiedad
			if (!objetoParametro.is<sol::table>())
			{
				parseObject(objetoParametro, nombreParametro, properties);
			}
			else
			{
				// ...
			}
		}

		// --- a este nivel va el init:
		// inicializacion de los parametros de un componente a traves de los datos de lua
		component->init(properties);

		// --- mete el componente a la entidad creada
		e->addComponent(std::move(component));
		Debug::out("GAMELOADER: Componente ", componenteName, " cargado para la entidad ", e->getName(), ".");
	}
	else
	{ // si no se ha conseguido crear el componente porque no estaba bien registrado
		Debug::warning("GAMELOADER: Componente ", componenteName, " no registrado.");
	}
}

void GameLoader::parseEntity(core::Entity* e, std::pair<sol::object, sol::object>& entidadObj)
{
	// nombre de la entidad
	std::string entidadName = entidadObj.first.as<std::string>();
	Debug::out("GAMELOADER: Entidad ", entidadName, " cargada.");

	// crea la entidad
	e->setName(entidadName);

	// dentro de la entidad, accedo a la tabla de componentes
	sol::table partes = entidadObj.second;
	sol::table componentes = partes["components"];

	for (auto& componenteObj : componentes)
	{ // --- para cada componente de la tabla de componentes de la entidad
		parseComponent(e, componenteObj);
	}
}

void GameLoader::defineUserTypes(sol::state& lua)
{
	lua.new_usertype<core::Vector3<>>(

		// --- EJEMPLO (struct)
		//"ship", // the name of the class, as you want it
		// to be used in lua List the member
		// functions you wish to bind:
		// "name_of_item",
		// &class_name::function_or_variable
		//"shoot",
		//&ship::shoot,
		//"hurt",
		//&ship::hurt,
		//// bind variable types, too
		//"life",
		//&ship::life,
		//// names in lua don't have to be the same as C++,
		//// but it probably helps if they're kept the same,
		//// here we change it just to show its possible
		//"bullet_count",
		//&ship::bullets

		"Vector3",
		"x",
		&core::Vector3<>::getX,
		"y",
		&core::Vector3<>::getY,
		"z",
		&core::Vector3<>::getZ
	);
}

void GameLoader::loadLua(
	std::shared_ptr<core::Scene>& s, 
	const sceneName& n,  
	const std::string& p)
{
	sol::state lua;
	lua.open_libraries(sol::lib::base);
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

	defineUserTypes(lua);

	// --- lectura lua
	sol::table scene = lua["scene"];
	Debug::out("GAMELOADER: Escena ", n, " cargada.");

	for (auto& entidadObj : scene)
	{ // --- para cada entidad leida
		core::Entity* e = new core::Entity();

		parseEntity(e, entidadObj);

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

std::string GameLoader::askSceneName()
{
	std::string sceneName;

	std::cout << "Introduce el nombre de la escena: ";
	std::cin >> sceneName;

	return sceneName;
}

std::string GameLoader::askRootName()
{
	std::string rootName;

	std::cout << "Introduce la ruta del directorio principal: ";
	std::cin >> rootName;

	return rootName;
}

std::string GameLoader::findSceneFile(const std::string& sceneName, const std::string& root)
{
	std::string target = sceneName + ".lua";

	for (const auto& entry : fs::recursive_directory_iterator(root))
	{
		if (!entry.is_regular_file())
			continue;

		if (entry.path().filename() == target)
		{
			return entry.path().parent_path().string();
		}
	}

	return "";
}

std::shared_ptr<core::Scene> GameLoader::loadSceneFromSearch()
{
	std::string sceneName = askSceneName();

	std::cout << "Buscando escena " << sceneName << ".lua" << std::endl;

	std::string root = askRootName();

	std::string path = findSceneFile(sceneName, root);

	if (path.empty())
	{
		Debug::error("No se encontró la escena ", sceneName);
		return nullptr;
	}

	Debug::out("Escena encontrada en ", path);

	std::shared_ptr<core::Scene> scene = std::make_shared<core::Scene>(sceneName);

	loadLua(scene, sceneName, path + "/");

	return scene;
}