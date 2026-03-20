#include "GameLoader.h"

#include <fstream>
#include <sol.hpp>
#include <filesystem>

#include "Defs.h"
#include "Debug.h"
#include "Entity.h"
#include "Component.h"
#include "ComponentRegister.h"
#include "GameConfigurator.h"

namespace fs = std::filesystem;

#define SOL_ALL_SAFETIES_ON 1

void GameLoader::parseObject(const sol::object& obj, const std::string& clave, Properties& props)
{
	switch (obj.get_type())
	{
	case sol::type::number:
		{
			if (obj.is<int>())
			{
				props[clave] = obj.as<int>();
			}
			else
			{
				props[clave] = obj.as<float>();
			}
			break;
		}
	case sol::type::boolean:
		{
			props[clave] = obj.as<bool>();
			break;
		}
	case sol::type::string:
		{
			props[clave] = obj.as<std::string>();
			break;
		}
	case sol::type::table:
		{
			if (obj.is<std::vector<float>>())
			{
				props[clave] = obj.as<std::vector<float>>();
			}
			else if (obj.is<std::vector<int>>())
			{
				props[clave] = obj.as<std::vector<int>>();
			}
			else if (obj.is<std::vector<std::string>>())
			{
				props[clave] = obj.as<std::vector<std::string>>();
			}
			else if (obj.is<std::vector<bool>>())
			{
				props[clave] = obj.as<std::vector<bool>>();
			}
			else if (obj.is<std::vector<sol::table>>())
			{
				//parseObject()
			}
		}
	case sol::type::userdata:
		{
			if (obj.is<core::Vector2<>>())
			{
				Debug::out("GAMELOADER: vector2");
				props[clave] = obj.as<core::Vector2<>>();
			}
			else if (obj.is<core::Vector3<>>())
			{
				Debug::out("GAMELOADER: vector3");
				props[clave] = obj.as<core::Vector3<>>();
			}
			else if (obj.is<core::Vector4<>>())
			{
				Debug::out("GAMELOADER: vector4");
				props[clave] = obj.as<core::Vector4<>>();
			}
			else if (obj.is<core::Quaternion<>>())
			{
				Debug::out("GAMELOADER: quaternion");
				props[clave] = obj.as<core::Quaternion<>>();
			}
			else if (obj.is<core::Color>())
			{
				Debug::out("GAMELOADER: color");
				props[clave] = obj.as<core::Color>();
			}
			else
			{
				Debug::error("GAMELOADER: El tipo del parametro de ", clave, " no esta definido.");
			}
			break;
		}
	default:
		{
			Debug::error("GAMELOADER: El tipo del parametro de ", clave, " no es valido.");
			break;
		}
	}
}

void GameLoader::parseComponent(core::Entity* e, std::pair<sol::object, sol::object>& componenteObj)
{
	// encontramos su nombre e intentamos instanciar un componente con una clase con tal nombre
	std::string componenteName = componenteObj.first.as<std::string>();
	std::shared_ptr<core::Component> component = ComponentRegister::instance().create(componenteName);

	if (component != nullptr)
	{
		// si este tipo de componente estaba registrado y se ha creado
		Properties properties;

		sol::table propertiesTable = componenteObj.second;
		for (auto& p : propertiesTable)
		{
			// nombre del parametro
			std::string nombreParametro = p.first.as<std::string>();

			// declarar variant
			auto objetoParametro = p.second;

			// traducir objeto a propiedad
			parseObject(objetoParametro, nombreParametro, properties);
		}

		// --- a este nivel va el init:
		// inicializacion de los parametros de un componente a traves de los datos de lua
		component->init(properties);

		// --- mete el componente a la entidad creada
		e->addComponent(std::move(component));
		Debug::out("GAMELOADER: Componente ", componenteName, " cargado para la entidad ", e->getName(), ".");
	}
	else
	{
		// si no se ha conseguido crear el componente porque no estaba bien registrado
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
	{
		// --- para cada componente de la tabla de componentes de la entidad
		parseComponent(e, componenteObj);
	}
}

void GameLoader::defineUserTypes(sol::state& lua)
{
	lua.new_usertype<core::Vector2<>>(
		"Vector2",
		sol::constructors<
			core::Vector2<>(),
			core::Vector2<>(float),
			core::Vector2<>(float, float)>(),
		"x", &core::Vector2<>::getX,
		"y", &core::Vector2<>::getY);

	lua.new_usertype<core::Vector3<>>(
		"Vector3",
		sol::constructors<
			core::Vector3<>(),
			core::Vector3<>(float),
			core::Vector3<>(float, float, float)>(),
		"x", &core::Vector3<>::getX,
		"y", &core::Vector3<>::getY,
		"z", &core::Vector3<>::getZ);

	lua.new_usertype<core::Vector4<>>(
		"Vector4",
		sol::constructors<
			core::Vector4<>(),
			core::Vector4<>(float),
			core::Vector4<>(float, float, float, float)>(),
		"x", &core::Vector4<>::getX,
		"y", &core::Vector4<>::getY,
		"z", &core::Vector4<>::getZ,
		"w", &core::Vector4<>::getW);

	lua.new_usertype<core::Quaternion<>>(
		"Quaternion",
		sol::constructors<
			core::Quaternion<>(),
			core::Quaternion<>(float, float, float, float)>(),
		"x", &core::Quaternion<>::getX,
		"y", &core::Quaternion<>::getY,
		"z", &core::Quaternion<>::getZ,
		"w", &core::Quaternion<>::getW);

	lua.new_usertype<core::Color>(
		"Color",
		sol::constructors<core::Color(float, float, float, float)>(),
		"r", &core::Color::getRed,
		"g", &core::Color::getGreen,
		"b", &core::Color::getBlue,
		"a", &core::Color::getAlpha);
}

void GameLoader::loadLua(
	std::shared_ptr<core::Scene>& s,
	const sceneName& n,
	const std::string& p)
{
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	std::string path = p + n + ".lua";
	_path = path;

	defineUserTypes(lua);

	try
	{
		// intenta leer archivo
		lua.safe_script_file(path);
	}
	catch (const sol::error& e)
	{
		// si no lo consigue saca error
		Debug::error("GAMELOADER: Error cargando escena: ", path);
		Debug::error("Lua exception: ", e.what());
		s = nullptr;
		return;
	}

	// --- lectura lua
	sol::table scene = lua["scene"];
	Debug::out("GAMELOADER: Escena ", n, " cargada.");

	for (auto& entidadObj : scene)
	{
		// --- para cada entidad leida
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
	_firstReload = true;
	return s;
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
	GameConfigurator gameConfig("scene_prueba", "..\..\..\..\..\2526-Grupo03-ChavalesEngine\bin\game\scenes");
	std::string sceneName = gameConfig.getSceneName();

	std::cout << "Buscando escena " << sceneName << ".lua" << std::endl;

	std::string root = gameConfig.getRoot();

	if (!fs::exists(root) || !fs::is_directory(root))
	{
		Debug::error("La ruta indicada no es un directorio valido: ", root);
		return nullptr;
	}

	std::string path = findSceneFile(sceneName, root);

	if (path.empty())
	{
		Debug::error("No se encontró la escena ", sceneName);
		return nullptr;
	}

	Debug::out("Escena encontrada en ", path);

	std::filesystem::path dir(path);
	std::filesystem::path file = dir / (sceneName + ".lua");

	if (!fs::exists(file))
	{
		Debug::error("El archivo de escena no existe: ", file.string());
		return nullptr;
	}

	std::shared_ptr<core::Scene> scene = std::make_shared<core::Scene>(sceneName);

	loadLua(scene, sceneName, dir.string() + "/");

	if (!scene)
	{
		Debug::error("Error cargando la escena ", sceneName);
		return nullptr;
	}

	return scene;
}

bool GameLoader::reloadLua()
{
	try
	{
		std::filesystem::file_time_type ftime = std::filesystem::last_write_time(_path);
		uintmax_t fsize = std::filesystem::file_size(_path);

		if (!_firstReload)
		{
			if (ftime > _lastTime && // para saber la ultima modificacion en tiempo
				_lastSize < fsize) // si se ha modificado el archivo de verdad
			{
				Debug::out("GAMELOADER: Recargando escena");

				_lastTime = ftime;
				_lastSize = fsize;

				return true;
			}
		}
		else
		{
			_lastTime = ftime;
			_lastSize = fsize;
			_firstReload = false;
		}

		return false;
	}
	// si lo borras a mitad que limpie la memoria de esa escena y que vuelva a 
	// preguntar que escena quieres cargar a continuacion, dando margen de recuperar la escena
	// por si la has borrado sin querer
	catch (...)
	{
		Debug::error("GAMELOADER: Error de hot reloading");
		return false;
	}
}
