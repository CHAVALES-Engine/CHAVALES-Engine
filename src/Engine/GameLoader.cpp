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

template<typename T>
bool GameLoader::isVectorOf(const sol::table& table)
{
	int expectedIndex = 1;

	for (auto& kv : table)
	{
		if (!kv.first.is<int>() || kv.first.as<int>() != expectedIndex)
			return false;

		if (!kv.second.is<T>())
			return false;

		expectedIndex++;
	}

	return true;
}

template<typename T>
std::vector<T> GameLoader::parseVector(const sol::table& table)
{
	std::vector<T> result;

	for (auto& kv : table)
	{
		result.push_back(kv.second.as<T>());
	}

	return result;
}

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
		sol::table t = obj;

		if (isVectorOf<int>(t))
		{
			props[clave] = parseVector<int>(t);
		}
		else if (isVectorOf<float>(t))
		{
			props[clave] = parseVector<float>(t);
		}
		else if (isVectorOf<std::string>(t))
		{
			props[clave] = parseVector<std::string>(t);
		}
		else if (isVectorOf<bool>(t))
		{
			props[clave] = parseVector<bool>(t);
		}
		else if (isVectorOf<core::Vector2<>>(t))
		{
			props[clave] = parseVector<core::Vector2<>>(t);
		}
		else if (isVectorOf<core::Vector3<>>(t))
		{
			props[clave] = parseVector<core::Vector3<>>(t);
		}
		else if (isVectorOf<core::Vector4<>>(t))
		{
			props[clave] = parseVector<core::Vector4<>>(t);
		}
		else if (isVectorOf<core::Quaternion<>>(t))
		{
			props[clave] = parseVector<core::Quaternion<>>(t);
		}
		else if (isVectorOf<core::Color>(t))
		{
			props[clave] = parseVector<core::Color>(t);
		}
		else
		{
			Debug::error("GAMELOADER: Parametro tabla no compatible en ", clave);
		}

		break;
	}
	case sol::type::userdata:
	{
		if (obj.is<core::Vector2<>>())
		{
			props[clave] = obj.as<core::Vector2<>>();
		}
		else if (obj.is<core::Vector3<>>())
		{
			props[clave] = obj.as<core::Vector3<>>();
		}
		else if (obj.is<core::Vector4<>>())
		{
			props[clave] = obj.as<core::Vector4<>>();
		}
		else if (obj.is<core::Quaternion<>>())
		{
			props[clave] = obj.as<core::Quaternion<>>();
		}
		else if (obj.is<core::Color>())
		{
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
	std::shared_ptr<core::Component> component = e->getComponent(componenteName);

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
		bool init = component->init(properties);

		if (init)
		{
			Debug::out("GAMELOADER: Componente ", componenteName, " cargado para la entidad ", e->getName(), ".");
		}
		else
		{
			// --- quita el componente a la entidad
			e->removeComponent(componenteName);
			Debug::warning("GAMELOADER: Error al cargar componente ", componenteName, ": no se pudo inicializar correctamente.");
		}
	}
	else
	{
		// si no se ha conseguido crear el componente porque no estaba bien registrado
		Debug::warning("GAMELOADER: Componente ", componenteName, " no registrado en la entidad ", e->getName(), ".");
	}
}

void GameLoader::instanceEntity(core::Entity* e, std::pair<sol::object, sol::object>& entidadObj)
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
		// --- instancia cada componente de la tabla de componentes de la entidad
		std::string componenteName = componenteObj.first.as<std::string>();
		std::shared_ptr<core::Component> component = ComponentRegister::instance().create(componenteName);
		// anyade el componente a la entidad si existe el componente
		if (component != nullptr){
			component->setName(componenteName);
			e->addComponent(std::move(component)); // anyade sin inicializar
		}
		else
			Debug::warning("GAMELOADER: Componente ", componenteName, " no registrado.");
	}
}

void GameLoader::initializeEntity(core::Entity* e, std::pair<sol::object, sol::object>& entidadObj)
{
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

		instanceEntity(e, entidadObj);

		// --- a este nivel se llamaria al awake:
		// metodo de logica de un componente sin garantizar que el resto de componentes y entidades esten inicializados
		//for (auto& c : e->getComponents())
		//{
		//	//c->awake();
		//}

		// --- mete la entidad en la escena
		s->addEntity(e);
	}

	for (auto& entidadObj : scene)
	{
		// --- para cada entidad leida
		std::string name = entidadObj.first.as<std::string>();
		core::Entity* e = s->findEntityByName(name);
		if (!e) continue;
		// inicializamos los componentes
		initializeEntity(e, entidadObj);
	}

	// --- a este nivel se llama al ready:
	// garantizamos que en el ready el resto de entidades y sus componentes estan inicializados
	s->ready();
}

std::shared_ptr<core::Scene> GameLoader::loadScene(const sceneName& n)
{
	std::shared_ptr<core::Scene> s = std::make_shared<core::Scene>(n);

#if _DEBUG
	loadLua(s, n, "./game/scenes/");
#else
	loadLua(s, n, core::GameConfigurator::instance()._scenesRoot);
#endif

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

std::shared_ptr<core::Scene> GameLoader::loadSceneFromSearch(const std::string& sceneName)
{
	std::string root = core::GameConfigurator::instance()._scenesRoot;

	std::cout << "Buscando escena " << sceneName << ".lua" << std::endl;

	if (!fs::exists(root) || 
		!fs::is_directory(root))
	{
		Debug::error("La ruta indicada no es un directorio valido: ", root);
		return nullptr;
	}

	std::string path = findSceneFile(sceneName, root);

	if (path.empty())
	{
		Debug::error("No se encontro la escena ", sceneName);
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
				_lastSize != fsize) // si se ha modificado el archivo de verdad
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
