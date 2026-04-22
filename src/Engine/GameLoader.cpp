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
#include <checkMLNew.h>

#include "Engine.h"

namespace fs = std::filesystem;

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

void GameLoader::parseObject(const sol::object& obj, const std::string& clave, Properties& props, const std::string& componentName)
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
			Debug::error("GAMELOADER: Parametro tabla no compatible en ", clave, " para el componente ", componentName, ".");
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
			Debug::error("GAMELOADER: El tipo del parametro de ", clave, " no esta definido para el componente ", componentName, ".");
		}
		break;
	}
	default:
	{
		Debug::error("GAMELOADER: El tipo del parametro de ", clave, " para el componente ", componentName, " no es valido.");
		break;
	}
	}
}

void GameLoader::parseComponent(core::Entity* e, std::pair<sol::object, sol::object>& componenteObj)
{
	// encontramos su nombre e intentamos instanciar un componente con una clase con tal nombre
	std::string componenteName = componenteObj.first.as<std::string>();
	std::shared_ptr<core::Component> component = e->getComponent(componenteName);

	if (component != nullptr && componenteObj.first.is<std::string>())
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
			parseObject(objetoParametro, nombreParametro, properties, componenteName);
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
			//e->removeComponent(componenteName);
			Debug::warning("GAMELOADER: Error al cargar componente ", componenteName, ": no se pudo inicializar correctamente.");
		}
	}
	else
	{
		// si no se ha conseguido crear el componente porque no estaba bien registrado
		Debug::warning("GAMELOADER: Componente ", componenteName, " no registrado en la entidad ", e->getName(), ".");
	}
}

void GameLoader::instanceEntity(core::Entity* e, std::pair<sol::object, sol::object> const& entidadObj)
{
	// nombre de la entidad
	std::string entidadName = entidadObj.first.as<std::string>();
	Debug::out("GAMELOADER: Cargando entidad ", entidadName, ".");

	// crea la entidad
	e->setName(entidadName);

	sol::table partes = entidadObj.second;

	// dentro de la entidad, accedo al dontdestroyonload
	sol::object ddol = partes["ddol"];

	if (ddol.is<bool>())
	{
		bool _ddol_B = ddol.as<bool>();
		e->setDontDestroyOnLoad(_ddol_B);

		std::string _ddol_S;
		_ddol_B ? _ddol_S = "true" : _ddol_S = "false";
		Debug::out("GAMELOADER: DontDestroyOnLoad de la entidad ", e->getName(), " cargado a ", _ddol_S, ".");
	}
	else
	{
		Debug::warning("GAMELOADER: No se ha leido el atributo DontDestroyOnLoad de la entidad ", e->getName(), ", cargado a false por defecto.");
	}

	// dentro de la entidad, accedo a la tabla de componentes
	sol::object object = partes["components"];
	if (object.get_type() != sol::type::table)
	{
		Debug::error("GAMELOADER: 'components' no existe o no es una tabla en la entidad ", entidadName, ", creando entidad vacia.");
		return;
	}
	sol::table componentes = object;

	for (auto& componenteObj : componentes)
	{
		// --- instancia cada componente de la tabla de componentes de la entidad
		std::string componenteName = componenteObj.first.as<std::string>();
		std::shared_ptr<core::Component> component = ComponentRegister::instance().create(componenteName);
		// anyade el componente a la entidad si existe el componente
		if (component != nullptr) {
			component->setName(componenteName);
			e->addComponent(std::move(component)); // anyade sin inicializar
		}
		else
			Debug::warning("GAMELOADER: No existe el componente ", componenteName, " en ningun registro, no se cargara el componente en ", entidadName, ".");
	}
}

void GameLoader::initializeEntity(core::Entity* e, std::pair<sol::object, sol::object> const& entidadObj)
{
	// dentro de la entidad, accedo a la tabla de componentes
	sol::table partes = entidadObj.second;
	sol::object object = partes["components"];
	if (object.get_type() != sol::type::table)
	{
		Debug::error("GAMELOADER: 'components' no existe o no es una tabla en la entidad ", e->getName(), ", creando entidad vacia.");
		return;
	}
	sol::table componentes = object;

	for (auto& componenteObj : componentes)
	{
		// --- para cada componente de la tabla de componentes de la entidad
		parseComponent(e, componenteObj);
	}
	Debug::out("GAMELOADER: Entidad ", e->getName(), " cargada.");
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

void GameLoader::defineFunc(sol::state& lua, const std::string& p)
{
	sol::load_result script = lua.load_file(p);

	if (!script.valid())
	{
		sol::error err = script;
		Debug::error("GAMELOADER: Error cargando el script de carga de prefabs", err.what());
		return;
	}

	sol::protected_function func = script;

	try
	{
		sol::protected_function_result result = func("game/scenes/cube.lua");

		if (result.valid())
		{
			Debug::out("Resultado de func() valido");
			sol::table root = result;

			if (root.get_type() == sol::type::table)
			{
				Debug::warning("Resultado de func() SI es una tabla");
			}
			else
			{
				Debug::error("Resultado de func() NO es una tabla");
				return;
			}

			sol::object cube = root["cube"];
			if (cube.valid())
			{
				Debug::warning("Cube ES valido");
				if (cube.get_type() == sol::type::table)
				{
					Debug::warning("Cube ES una tabla");
				}
				else
				{
					Debug::error("Cube NO es una tabla");
				}
			}
			else
			{
				Debug::error("Cube NO es valido");
			}
		}
		else
		{
			sol::error err = result;
			Debug::error("Resultado de func() invalido: ", err.what());
		}
	}
	catch (const sol::error& e)
	{
		// si no lo consigue saca error
		Debug::error("Lua exception: ", e.what());
	}
}

void GameLoader::loadLua(
	std::shared_ptr<core::Scene>& s,
	const sceneName& n,
	const std::string& p)
{
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::io);
	std::string path = p + n + ".lua";
	_path = path;

	defineUserTypes(lua);

	std::string pathFunc = p + "luaFunc.lua";
	defineFunc(lua, pathFunc);

	try
	{
		// intenta leer archivo
		lua.safe_script_file(path);
	}
	catch (const sol::error& e)
	{
		// si no lo consigue saca error
		Debug::error("GAMELOADER: Error abriendo escena: ", path);
		Debug::error("Lua exception: ", e.what());
		s = nullptr;
		return;
	}

	// --- lectura lua
	// - Escena
	sol::object object = lua["scene"];
	if (!object.valid() || object.get_type() != sol::type::table)
	{
		Debug::error("GAMELOADER: 'scene' no existe o no es una tabla en ", path);
		s = nullptr;
		return;
	}
	sol::table scene = object;
	Debug::out("GAMELOADER: Cargando escena ", n, ".");

	// - Ajustes escena
	// Gizmos
	// dentro de la entidad, accedo al dontdestroyonload
	sol::object gizmos = scene["gizmos"];

	if (gizmos.valid() && gizmos.is<bool>())
	{
		bool _gizmos_B = gizmos.as<bool>();
		std::string _gizmos_S;

		Engine::instance()->setGizmos(_gizmos_B);

		_gizmos_B ? _gizmos_S = "true" : _gizmos_S = "false";
		Debug::out("GAMELOADER: Render de gizmos inicializado a ", _gizmos_S, ".");
	}
	else
	{
		Debug::warning("GAMELOADER: No se ha leido la configuracion de render de gizmos, inicializado a false por defecto.");
	}

	object = scene["entities"];
	if (!object.valid() || object.get_type() != sol::type::table)
	{
		Debug::error("GAMELOADER: 'entities' no existe o no es una tabla en ", path);
		s = nullptr;
		return;
	}
	sol::table entities = object;

	for (auto& entidadObj : entities)
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

	for (auto& entidadObj : entities)
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

	Debug::out("GAMELOADER: Escena ", n, " cargada.");
}

core::Entity* GameLoader::loadLua(const std::shared_ptr<core::Scene>& s, std::string const& p)
{
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::io);
	// tipos de usuario
	defineUserTypes(lua);

	std::string path = p + ".lua";
	Debug::warning("GAMELOADER: cargando prefab: ", path);

	/*std::string pathFunc = p + "luaFunc.lua";
	defineFunc(lua, pathFunc);*/

	try
	{
		// intenta leer archivo
		// - Prefab
		sol::object object = lua.safe_script_file(path);
		if (!object.valid() || object.get_type() != sol::type::table) {
			Debug::error("GAMELOADER: 'prefab' no existe o no es una tabla en ", path);
			return nullptr;
		}
		// ESTO POR SI UN PREFAB TIENE MAS DE UNA ENTIDAD

		//// - Lee las entradas de la tabla de lua para meterla en la escena y a sus hijos.
		sol::table table = object;
		//for (auto& table : entities)	{
		//	// --- para cada entidad leida
		//	core::Entity* e = new core::Entity();
		//	instanceEntity(e, table);
		//	// --- mete la entidad en la escena
		//	s->addEntity(e);
		//}
		//for (auto& table : entities)	{
		//	std::string name = table.first.as<std::string>();
		//	core::Entity* e = s->findEntityByName(name);
		//	if (!e) continue;
		//	initializeEntity(e, table);
		//}
		//s->ready();

		//Debug::out("GAMELOADER: Prefab ", p, " cargado.");

		// Prefab de 1 entidad.
		core::Entity* e = new core::Entity();
		std::string prefabName = p.substr(p.find_last_of("/\\") + 1);
		Debug::out("GAMELOADER: Configurando entidad ", prefabName, ".");
		e->setName(prefabName);

		// DDOL
		sol::object ddol = table["ddol"];
		if (ddol.is<bool>())
		{
			bool ddolValue = ddol.as<bool>();
			e->setDontDestroyOnLoad(ddolValue);
			Debug::out("GAMELOADER: DontDestroyOnLoad = ", (ddolValue ? "true" : "false"));
		}
		else
		{
			Debug::warning("GAMELOADER: DontDestroyOnLoad no especificado, usando false.");
		}

		// Componentes
		sol::object componentsObj = table["components"];
		if (!componentsObj.valid() || componentsObj.get_type() != sol::type::table)
		{
			Debug::error("GAMELOADER: 'components' no existe en ", prefabName);
			return nullptr;
		}

		sol::table componentes = componentsObj;
		for (auto& kv : componentes)
		{
			// Filtrar solo claves string
			if (!kv.first.is<std::string>())
				continue;

			std::string componenteName = kv.first.as<std::string>();
			std::shared_ptr<core::Component> component = ComponentRegister::instance().create(componenteName);

			if (component != nullptr)
			{
				component->setName(componenteName);
				e->addComponent(std::move(component));
			}
			else
			{
				Debug::warning("GAMELOADER: Componente ", componenteName, " no registrado.");
			}
		}


		s->addEntity(e);

		// inicializacion de componentes.
		for (auto& kv : componentes)
		{
			if (!kv.first.is<std::string>())
				continue;

			parseComponent(e, kv);
		}

		e->ready();

		return e;
	}
	catch (const sol::error& e)
	{
		// si no lo consigue saca error
		Debug::error("GAMELOADER: Error abriendo escena: ", path);
		Debug::error("Lua exception: ", e.what());
		return nullptr;
	}
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

std::shared_ptr<core::Scene> GameLoader::loadScene(const sceneName& n)
{
	std::string root = core::GameConfigurator::instance()._scenesRoot;

	if (!fs::exists(root) ||
		!fs::is_directory(root))
	{
		Debug::error("La ruta indicada no es un directorio valido: ", root);
		return nullptr;
	}

	std::string path = findSceneFile(n, root);

	if (path.empty())
	{
		Debug::error("No se encontro la escena ", n);
		return nullptr;
	}

	Debug::out("Escena encontrada en ", path);

	std::filesystem::path dir(path);
	std::filesystem::path file = dir / (n + ".lua");

	if (!fs::exists(file))
	{
		Debug::error("El archivo de escena no existe: ", file.string());
		return nullptr;
	}

	std::shared_ptr<core::Scene> s = std::make_shared<core::Scene>(n);

	try
	{
		loadLua(s, n, root);

	}
	catch (...)
	{
		Debug::error("[GAMELOADER] Error critico leyendo escena, borrando memoria creada a partir de ella");
		s->clearScene();
		return nullptr;
	}

	if (!s)
	{
		Debug::error("Error cargando la escena ", n);
		return nullptr;
	}

	_firstReload = true;
	return s;
}

core::Entity* GameLoader::loadPrefab(std::string const& n)
{
	return loadLua(Engine::instance()->getScene(), n);
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
