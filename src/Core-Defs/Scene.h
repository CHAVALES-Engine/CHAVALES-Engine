/**
 * @file
 * @brief
 */

#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "guid.h"

namespace core
{
	class Entity;
	using sceneName = std::string;
	using entityID = ChavalesGUID;

	/**
	 * @brief Escena.
	 *
	 *		Clase que implementa una escena, almacena sus entidades en un mapa y 
	 *		llama a los metodos de estas cuando corresponda segun la maquina de estados.
	 */
	class Scene
	{
	public:
		/**
		 * @brief Constructor.
		 *
		 * @param name - nombre de la escena a crear.
		 */
		Scene(sceneName name);
		~Scene();

		/**
		 * @brief Parte del ciclo de escena. Se llama cuando esta se habilita.
		 *
		 */
		void onEnable();
		//virtual void init();

		/**
		 * @brief Parte del ciclo de escena. Se llama cuando todas las entidades y componentes ya se han inicializado.
		 *
		 */
		void awake(const bool& loadingScreen = false);

		/**
		 * @brief Parte del ciclo de escena. Se llama despues de la inicializacion.
		 *
		 */
		void ready(const bool& loadingScreen = false);

		/**
		 * @brief Parte del ciclo de escena. Se llama una vez cada intervalo fijo de tiempo.
		 *
		 */
		void fixedUpdate();

		/**
		 * @brief Parte del ciclo de escena. Se llama una vez cada frame.
		 * 
		 * @param dT - deltaTime.
		 *
		 */
		void update(uint64_t dT);
		
		/**
		 * @brief Parte del ciclo de escena. Se llama una vez cada frame despues de update.
		 *
		 * @param dT - deltaTime.
		 *
		 */
		void lateUpdate(uint64_t dT);

		/**
		 * @brief Parte del ciclo de escena. Se llama cuando esta se deshabilita.
		 *
		 */
		void onDisable();

		/**
		 * @brief Parte del ciclo de escena. Se llama cuando esta se destruye.
		 *
		 */
		void destroy();

		/**
		 * @brief Parte del ciclo de escena. Se llama cuando se carga una escena nueva.
		 *
		 */
		void clearScene();

		/**
		 * @brief Extrae las entidades marcadas como DontDestroyOnLoad de la escena.
		 */
		std::vector<Entity*> getDDOLEntities() const;

		/**
		 * @brief Marca una entidad para insertar.
		 *
		 * @param e - entidad a insertar.
		 */
		void addEntity(core::Entity* e);
		/**
		 * @brief Marca una entidad a eliminar.
		 *
		 * @param e - entidad a eliminar.
		 */
		void destroyEntity(core::Entity* e);

		/**
		 * @brief Elimina las entidades marcadas.
		 */
		void destroyDeadEntities();

		/**
		 * @brief Aniade las entidades marcadas.
		 */
		void addListedEntities(const bool& loadingScreen = false);

		/**
		 * @brief Devuelve el mapa de entidades de la escena.
		 *
		 */
		const std::unordered_map<entityID, core::Entity*>& getEntities() const;

		/**
		 * @brief Busca una entidad a partir de su nombre.
		 * @param name - nombre de la entidad a buscar.
		 */
		Entity* findEntityByName(std::string const& name)const;

		/**
		 * @brief Devuelve el nombre de la escena.
		 *
		 */
		inline const sceneName& getName() const
		{
			return _name;
		}
		/**
		 * @brief Settea el nombre de la escena.
		 *
		 */
		inline void setName(const sceneName& n)
		{
			_name = n;
		}

		/**
		 * @brief Establecer metodo para renderizar pantalla de carga
		 *
		 */
		void setLoadingScreenRenderCallback(std::function<void()> cb) { _loadingScreenRender = cb; }
		/**
		 * @brief Establecer metodo para incrementar progreso pantalla de carga
		 *
		 */
		void setLoadingScreenIncreaseCallback(std::function<void()> cb) { _loadingScreenIncrease = cb; }

	private:
		sceneName _name;
		bool _active = false;
		/**
		 * @brief Aniade entidad al mapa.
		 * @param e 
		 */
		void _addEntity(core::Entity* e);
		/**
		 * @brief Unordered map de entidades en la escena actualmente.
		 *
		 */
		std::unordered_map<entityID, core::Entity*> _entities;
		/**
		 * @brief Vector de entidades en la escena a eliminar.
		 *
		 */
		std::vector<entityID> _entitiesToDelete;
		/**
		 * @brief Vector de entidades en la escena a aniadir.
		 *
		 */
		std::vector<core::Entity*> _entitiesToAdd;
		/**
		 * @brief Unordered map de nombres de entidades y su guid para buscar rapidamente.
		 *
		 */
		std::unordered_map<std::string, entityID> _entitiesNames;


		std::function<void()> _loadingScreenRender;
		std::function<void()> _loadingScreenIncrease;
	};
}