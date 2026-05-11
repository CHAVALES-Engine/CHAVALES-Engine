/**
 * @file ScriptsManager.h
 * @brief Gestor de scripts en Lua integrado con el motor.
 */
#pragma once
#include <memory>
#include <string>

#include "Defs.h"
#include "EngineAPI.h"
#include "guid.h"

/**
 * @brief Alias que identifica una lista de argumentos para ejecutar una funcion.
 *		usamos un initializer:list porque es mas practico escribit como argumento {42.0f, true, std::string("fire")} 
 *		en lugar de tener que construir una estructura tipo vector o array.
 */
using ExecuteArgs = std::initializer_list<Property>;
 /**
  * @brief Alias para el identificador unico de un script cargado.
  */
using ScriptHandle = ChavalesGUID;
/**
 * @brief Valor constante que representa un manejador de script nulo o invalido.
 */
static constexpr ScriptHandle NULL_SCRIPT = ChavalesGUID::invalid();

/**
 * @class ScriptsManager
 * @brief Clase Singleton encargada de la carga, ejecucion y gestion del ciclo de vida de scripts Lua.
 *		Utiliza el patron PIMPL para ocultar las dependencias de Sol2 y Lua en el header.
 * Cada script cargado se ejecuta en su propio entorno para evitar colisiones de nombres.
 */
class ENGINE_API ScriptsManager
{
public:
	// Eliminar copia y movimiento
	ScriptsManager(const ScriptsManager&) = delete;
	ScriptsManager& operator=(const ScriptsManager&) = delete;
	ScriptsManager(ScriptsManager&&) = delete;
	ScriptsManager& operator=(ScriptsManager&&) = delete;
	/**
	 * @brief Obtiene la instancia unica del gestor de scripts.
	 * @return ScriptsManager& - Referencia a la instancia de ScriptsManager.
	 */
	static ScriptsManager& instance();
	/**
	 * @brief Inicializa la maquina virtual de Lua y carga las librerias base.
	 *		Debe llamarse antes de intentar cargar cualquier script.
	 */
	void init() const;
	/**
	 * @brief Registra las funciones y clases de C++ que estaran disponibles en Lua.
	 */
	void registerBindings() const;
	/**
     * @brief Carga y compila un script Lua desde una ruta de archivo.
     * @param path - Ruta al archivo .lua.
     * @return ScriptHandle - El identificador unico del script. Devuelve NULL_SCRIPT si falla.
     */
	ScriptHandle loadScript(const std::string& path) const;
	/**
	 * @brief Libera la memoria y el entorno asociado a un script.
	 * @param handle - El identificador del script a liberar.
	 */
	void releaseScript(ScriptHandle handle) const;
	/**
	 * @brief Ejecuta una funcion especifica dentro de un script cargado.
	 * @param h - Identificador del script.
	 * @param fn - Nombre de la funcion en Lua.
	 * @return bool - true si la ejecucion fue exitosa, false en caso de error o si la funcion no existe.
	 */
	bool execute(ScriptHandle h, const std::string& fn) const;
	/**
	 * @brief Ejecuta una funcion especifica pasando argumentos.
	 *		Ej: execute(h, "onDamage", {42.0f, true, std::string("fire")});
	 * @param h - Identificador del script.
	 * @param fn - Nombre de la funcion en Lua.
	 * @param args - initializer_list<Property> de argumentos.
	 * @return bool - true si la ejecucion fue exitosa.
	 */
	bool execute(ScriptHandle h, const std::string& fn, ExecuteArgs args) const;
private:
	ScriptsManager();
	~ScriptsManager();
	// @brief Estructura interna de implementacion (PIMPL).
	struct Impl;
	// @brief Puntero inteligente a la implementacion interna.
	std::unique_ptr<Impl> pImpl;
};