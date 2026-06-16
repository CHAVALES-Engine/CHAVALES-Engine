/**
 * @file ScriptsManager.h
 * @brief Gestor de scripts en Lua integrado con el motor.
 */
#pragma once
#include <any>
#include <functional>
#include <memory>
#include <string>

#include "Defs.h"
#include "EngineAPI.h"
#include "guid.h"
#include "Debug.h"
#include "Entity.h"

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
private:
	ScriptsManager();
	~ScriptsManager();
	using MethodWrapper = std::function<Property(void*, const std::vector<Property>&)>;
	std::function<void(const std::string&)> _onEditableScript;
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
	void shutdown();

	/**
	 * @brief Registro en el luastate de core automaticamente
	 */
	void registerBindings() const;

	/**
	 * @brief Helper que coge de manera segura un elemento del tipo T del variant Property.
	 * @param p - Property del que sacar el tipo.
	 * @return T - Elemento sacado.
	 */
	template<typename T>
	static T getArg(const Property& p)
	{
		return std::get<std::remove_cv_t<std::remove_reference_t<T>>>(p);
	}
	/**
	 * @brief Carga y compila un script Lua desde una ruta de archivo.
	 * @param path - Ruta al archivo .lua.
	 * @return ScriptHandle - El identificador unico del script. Devuelve NULL_SCRIPT si falla.
	 */
	ScriptHandle loadScript(const std::string& path) const;
	/**
	 * @brief Inyecta entity en el entorno del script para que pueda usar.
	 * @param h - Identificador del script.
	 * @param entity - Entidad a inyectar.
	 */
	void setScriptEntity(ScriptHandle h, core::Entity* entity) const;
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
	 * @brief Ejecuta una funcion especifica dentro de un script cargado.
	 * @param h - Identificador del script.
	 * @param fn - Nombre de la funcion en Lua.
	 * @param args - Lista de Propierties (Argumentos de la funcion).
	 * @return bool - true si la ejecucion fue exitosa, false en caso de error o si la funcion no existe.
	 */
	bool execute(ScriptHandle h, const std::string& fn, ExecuteArgs args) const;
	/**
	 * @brief Ejecuta una funcion pasando las Properties como tabla Lua
	 * @param h - Identificador del script.
	 * @param fn - Nombre de la funcion en Lua.
	 * @param props Properties de argumentos.
	 * @return bool - true si la ejecucion fue exitosa.
	 */
	bool executeWithProps(ScriptHandle h, const std::string& fn, const Properties& props) const;
	/**
	 * @brief Bindea un metodo y usertype (Si no lo estaba antes) al luastate.
	 * @param typeName - Usertype (Lo crea si no estaba antes).
	 * @param methodName - Nombre del metodo a bindear.
	 * @param wrapper - Envoltorio del metodo a bindear.
	 */
	void bindMethodImpl(const std::string& typeName, const std::string& methodName, MethodWrapper wrapper) const;
	/**
	 * @brief Bindea un usertype (Si no lo estaba antes) como global de lua.
	 * @param globalName - Nombre global del usertype
	 * @param typeName - Usertype (Lo crea si no estaba antes).
	 * @param instance - Instancia estatica de la clase en c++ usada.
	 */
	void bindGlobalImpl(const std::string& globalName, const std::string& typeName, void* instance) const;

	void setEditableScriptCallback(std::function<void(const std::string&)> cb)
	{
		_onEditableScript = std::move(cb);
	}

	void notifyEditableScript(const std::string& path)
	{
		if (_onEditableScript)
			_onEditableScript(path);
	}
	void notifyLuaChanged()
	{
		_luaDirty = true;
	}
	bool isLuaDirty() const
	{
		return _luaDirty;
	}

	void clearLuaDirty()
	{
		_luaDirty = false;
	}
private:
	/**
	 * @brief Helper que declara un usertype.
	 * @param typeName - Nombre del usertype.
	 */
	void _declareTypeImpl(const std::string& typeName) const;

	// @brief Estructura interna de implementacion (PIMPL).
	struct Impl;
	// @brief Puntero inteligente a la implementacion interna.
	std::unique_ptr<Impl> pImpl;
	bool _luaDirty = false;
};
