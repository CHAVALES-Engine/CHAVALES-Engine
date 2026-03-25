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

	// -- Traducciones de Lua
	/**
	 * @brief Traduce un objeto de sol a variant para guardarlo en el mapa de propiedades de un componente.
	 *
	 * @param obj - Objeto de sol a traducir.
	 * @param clave - Nombre con el que se guardara la propiedad en el mapa de propiedades.
	 * @param props - Mapa de propiedades del componente.
	 */
	static void parseObject(const sol::object& obj, const std::string& clave, Properties& props);

	/**
	 * @brief Traduce un objeto de sol a variant para guardarlo en el mapa de propiedades de un componente.
	 *
	 * @param e - Entidad a la que pertenecera el componente.
	 * @param componenteObj - Par nombre, objeto de sol a traducir.
	 */
	static void parseComponent(core::Entity* e, std::pair<sol::object, sol::object>& componenteObj);

	/**
	 * @brief Traduce un objeto de sol a variant para guardarlo en el mapa de propiedades de un componente.
	 *
	 * @param e - Entidad a la que pertenecera el componente.
	 * @param componenteObj - Par nombre, objeto de sol a traducir.
	 */
	static void parseEntity(core::Entity* e, std::pair<sol::object, sol::object>& entidadObj);

	/**
	 * @brief Para definir tipos de clases propias que poder traducir desde lua.
	 *
	 * @param lua - Estado de lua donde definir los tipos.
	 */
	static void defineUserTypes(sol::state& lua);

	/**
	 * @brief Para definir tipos de clases propias que poder traducir desde lua.
	 *
	 * @param lua - Estado de lua donde definir los tipos.
	 */
	static void loadLua(std::shared_ptr<core::Scene>& s, const sceneName& n, const std::string& p = "./game/scenes/");

	/**
	 * @brief Carga una escena dada de lua.
	 *
	 * @param n - Nombre de la escena a cargar, debe coincidir con el .lua que la define.
	 */
	static std::shared_ptr<core::Scene> loadScene(const sceneName& n);

	/**
	 * @brief Busca en la ruta el nombre de la escena y devuelve la ruta completa hasta el .lua.
	 *
	 * @param sceneName - Nombre de la escena a cargar.
	 * @param root - Ruta donde buscar el nombre de la escena.
	 */
	static std::string findSceneFile(const std::string& sceneName, const std::string& root);

	/**
	 * @brief LLama a preguntar por el nombre de la escena y usa findSceneFile para cargar el archivo .lua con loadLua.
	 *
	 */
	static std::shared_ptr<core::Scene> loadSceneFromSearch();

	/**
	 * @brief Control de recargado de la escena desde el archivo .lua.
	 */
	static bool reloadLua();

	/**
	 * @brief Comprueba si una tabla sol es un vector de tipo especificado en la plantilla
	 * @param table - tabla leida de sol
	 * @returns Si la tabla es traducible a un vector de plantilla
	 */
	template<typename T>
	static bool isVectorOf(const sol::table& table);

	/**
	 * @brief Traduce una tabla sol a vector de tipo especificado en el template
	 * @param table - tabla leida de sol
	 * @returns vector de tipo T con el contenido de la tabla
	 */
	template<typename T>
	static std::vector<T> parseVector(const sol::table& table);

private:
	static inline std::string _path = "";
	static inline std::filesystem::file_time_type _lastTime;
	static inline uintmax_t _lastSize;

	static inline bool _firstReload = true;
};