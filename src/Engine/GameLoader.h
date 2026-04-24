#pragma once

#include <memory>
#include <EngineAPI.h>
#include <sol.hpp>
#include <string>

#include "Defs.h"
#include "Scene.h"

#include <filesystem>

using sceneName = std::string;

/**
 * @brief GameLoader.
 *
 *		Clase que implementa la integracion y carga de escenas con Lua.
 */
class ENGINE_API GameLoader
{
public:
	GameLoader() = default;
	~GameLoader() = default;

	/**
	 * @brief Carga una escena dada de lua.
	 *
	 * @param n - Nombre de la escena a cargar, debe coincidir con el .lua que la define.
	 */
	static std::shared_ptr<core::Scene> loadScene(const sceneName& n);

	/**
	 * @brief Carga un prefab e instancia las entidades.
	 * @param n - path del prefab a cargar.
	 * @return 
	 */
	static core::Entity* loadPrefab(std::string const& n);

	/**
	 * @brief Control de recargado de la escena desde el archivo .lua.
	 */
	static bool reloadLua();

private:
	// -- Traducciones de Lua
	/**
	 * @brief Traduce un objeto de sol a variant para guardarlo en el mapa de propiedades de un componente.
	 *
	 * @param obj - Objeto de sol a traducir.
	 * @param clave - Nombre con el que se guardara la propiedad en el mapa de propiedades.
	 * @param props - Mapa de propiedades del componente.
	 */
	static void _parseObject(const sol::object& obj, const std::string& clave, Properties& props, const std::string& componentName);

	/**
	 * @brief Traduce un objeto de sol a variant para guardarlo en el mapa de propiedades de un componente.
	 *
	 * @param e - Entidad a la que pertenecera el componente.
	 * @param componenteObj - Par nombre, objeto de sol a traducir.
	 */
	static void _parseComponent(core::Entity* e, std::pair<sol::object, sol::object>& componenteObj);

	/**
	 * @brief Traduce un objeto de sol a variant para guardarlo en el mapa de propiedades de un componente.
	 *
	 * @param e - Entidad a la que pertenecera el componente.
	 * @param entidadObj - Par nombre, objeto de sol a traducir.
	 */
	static void _instanceEntity(core::Entity* e, std::pair<sol::object, sol::object> const& entidadObj);
	static void _initializeEntity(core::Entity* e, std::pair<sol::object, sol::object> const& entidadObj);

	/**
	 * @brief Para definir tipos de clases propias que poder traducir desde lua.
	 *
	 * @param lua - Estado de lua donde definir los tipos.
	 */
	static void _defineUserTypes(sol::state& lua);

	/**
	 * @brief Funcion que ejecuta un lua a nivel de motor para hidratar los prefabs de una escena.
	 
	 * @param lua - Estado de lua.
	 * @param fp - Ruta al archivo lua de funciones.
	 * @param sp - Ruta a la escena a hidratar.
	 * @param st - Tabla sol resultante de la ejecucion de lua al hidratar los prefabs.
	 * @returns Si el lua se ha podido ejecutar correctamente y ha devuelto una tabla valida.
	 */
	static bool _defineFunc(sol::state& lua, const std::string& fp, const std::string& sp, sol::table& st);

	/**
	 * @brief Para definir tipos de clases propias que poder traducir desde lua.
	 *
	 * @param lua - Estado de lua donde definir los tipos.
	 */
	static void _loadLua(std::shared_ptr<core::Scene>& s, const sceneName& n, const std::string& p);

	/**
	 * Carga un archivo de lua y lo mete en la escena activa.
	 * @param s - Escena a la que anyadir.
	 * @param p - path del fichero a cargar.
	 */
	static core::Entity* _loadLua(const std::shared_ptr<core::Scene>& s, std::string const& p);

	/**
	 * @brief Busca en la ruta el nombre de la escena y devuelve la ruta completa hasta el .lua.
	 *
	 * @param sceneName - Nombre de la escena a cargar.
	 * @param root - Ruta donde buscar el nombre de la escena.
	 */
	static std::string _findSceneFile(const std::string& sceneName, const std::string& root);

	/**
	 * @brief Comprueba si una tabla sol es un vector de tipo especificado en la plantilla
	 * @param table - tabla leida de sol
	 * @returns Si la tabla es traducible a un vector de plantilla
	 */
	template<typename T>
	static bool _isVectorOf(const sol::table& table);

	/**
	 * @brief Traduce una tabla sol a vector de tipo especificado en el template
	 * @param table - tabla leida de sol
	 * @returns vector de tipo T con el contenido de la tabla
	 */
	template<typename T>
	static std::vector<T> _parseVector(const sol::table& table);

	static inline std::string _path = "";
	static inline std::filesystem::file_time_type _lastTime;
	static inline uintmax_t _lastSize;

	static inline bool _firstReload = true;
};