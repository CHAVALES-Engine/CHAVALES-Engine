#include "Scene.h"

#include <cstdint>
#include "Entity.h"
#include "checkMLNew.h"
namespace core {
	core::Scene::Scene(sceneName name) :
		_name(std::move(name))
	{
		_active = false; Debug::out("SCENE: Escena '", _name, "' creada.");
	}

	core::Scene::~Scene()
	{
		Debug::out("SCENE: Destruyendo escena '", _name, "'. Limpiando contenedores.");
		destroy();
	}

	void core::Scene::onEnable()
	{
		_active = true; Debug::out("SCENE: Escena '", _name, "' habilitada (onEnable).");

		if (!_entities.empty())
		{
			for (auto& [guid, e] : _entities)
			{
				if (e) e->enable();
				else Debug::error("SCENE: Puntero nulo detectado en onEnable para la entidad ", guid);
			}
		}
		else {
			Debug::warning("SCENE: onEnable ejecutado pero la escena '", _name, "' no tiene entidades.");
		}
	}

	void core::Scene::awake()
	{
		Debug::out("SCENE: Llamando a awake() en escena '", _name, "'.");
		if (_entities.empty()) {
			Debug::warning("SCENE: awake() - No hay entidades en la escena.");
			return;
		}

		for (auto [guid, ent] : _entities)
		{
			if (ent && ent->isAlive() && !ent->isInitialized())
				ent->awake();
		}
	}

	void core::Scene::ready()
	{
		Debug::out("SCENE: Llamando a ready() en escena '", _name, "'.");
		if (_entities.empty()) return;

		for (auto [guid, ent] : _entities)
		{
			if (ent && ent->isAlive() && !ent->isInitialized())
				ent->ready();
			ent->setInitialized(true);
		}
	}

	void core::Scene::fixedUpdate()
	{
		if (_entities.empty()) return;
		for (auto [guid, ent] : _entities)
		{
			if (ent && ent->isAlive())
				ent->fixedUpdate();
		}
	}

	void core::Scene::update(uint64_t dT)
	{
		if (_entities.empty()) return;

		for (auto [guid, ent] : _entities)
		{
			if (ent && ent->isAlive())
				ent->update(dT);
		}
	}

	void core::Scene::lateUpdate(uint64_t dT)
	{
		if (_entities.empty()) return;
		for (auto [guid, ent] : _entities)
		{
			if (ent && ent->isAlive())
				ent->lateUpdate(dT);
		}
	}

	void core::Scene::onDisable()
	{
		_active = false;

		if (!_entities.empty())
		{
			for (auto& [guid, e] : _entities)
			{
				e->disable();
			}
		}
	}

	void core::Scene::destroy() // elimina al completo
	{
		Debug::out("SCENE: Destruyendo todas las entidades de la escena '", _name, "'.");
		for (auto [id, e] : _entities)
			destroyEntity(e);
		destroyDeadEntities();
		while (!_entitiesToAdd.empty())
		{
			Entity* e = _entitiesToAdd.back();
			_entitiesToAdd.pop_back();
			e->setScene(nullptr);
			delete e;
		}
		_entities.clear();
		_entitiesNames.clear();
		_entitiesToAdd.clear();
	}

	void core::Scene::clearScene() // comprueba dont destroy on load
	{
		Debug::out("SCENE: Limpiando escena (respetando DontDestroyOnLoad).");
		for (auto it = _entities.begin(); it != _entities.end(); )
		{
			auto& [guid, e] = *it;

			if (e && !e->getDontDestroyOnLoad())
			{
				e->setScene(nullptr);
				delete e;
				it = _entities.erase(it);
			}
			else
			{
				if (!e) Debug::error("SCENE: Puntero nulo encontrado durante clearScene en GUID ", guid);
				++it;
			}
		}
		_entitiesNames.clear();
	}

	std::vector<Entity*> core::Scene::getDDOLEntities() const
	{
		std::vector<Entity*> persistentEntities;

		for (const auto& [guid, e] : _entities)
			if (e->getDontDestroyOnLoad())
				persistentEntities.push_back(e);

		return persistentEntities;
	}

	void core::Scene::addEntity(Entity* e)
	{
		if (!e) {
			Debug::error("SCENE: Intento de anyadir una entidad nula a la escena '", _name, "'");
			return;
		}
		_entitiesToAdd.push_back(e);
	}

	void Scene::destroyEntity(core::Entity* e)
	{
		if (!e) {
			Debug::error("SCENE: Intento de anyadir una entidad nula a la escena '", _name, "'");
			return;
		}
		_entitiesToDelete.push_back(e->getEntityID());
	}

	void core::Scene::destroyDeadEntities()
	{ 
		for (const auto& guid : _entitiesToDelete)
		{
			auto it = _entities.find(guid);
			if (it == _entities.end()) continue;

			Entity* e = it->second;
			// Limpiar del mapa de nombres
			_entitiesNames.erase(e->getName());
			// Destruir y liberar
			e->setScene(nullptr);
			delete e;
			_entities.erase(it);
		}
		_entitiesToDelete.clear();
	}

	void Scene::addListedEntities()
	{
		for (auto* e : _entitiesToAdd)
			_addEntity(e);

		_entitiesToAdd.clear();
	}

	const std::unordered_map<ChavalesGUID, core::Entity*>& core::Scene::getEntities() const
	{
		return _entities;
	}

	Entity* core::Scene::findEntityByName(std::string const& name) const
	{
		auto namIt = _entitiesNames.find(name);
		if (namIt == _entitiesNames.end()) {
			Debug::warning("SCENE: No se pudo encontrar la entidad con nombre '", name, "'");
			return nullptr;
		}
		auto entIt = _entities.find(namIt->second);
		if (entIt != _entities.end()) return entIt->second;

		Debug::error("SCENE: Inconsistencia detectada. Nombre '", name, "' existe pero su GUID no tiene entidad asociada.");
		return nullptr;
	}

	void Scene::_addEntity(core::Entity* e)
	{
		if (!e) {
			Debug::error("SCENE: Intento de anyadir una entidad nula a la escena '", _name, "'");
			return;
		}

		entityID guid = ChavalesGUID::generate();
		e->setEntityID(guid);

		std::string originalName = e->getName();
		std::string finalName = originalName;
		int counter = 1;

		// iterar hasta encontrar un nombre valido.
		while (_entitiesNames.find(finalName) != _entitiesNames.end())
		{
			finalName = originalName + "_" + std::to_string(counter);
			counter++;
		}
		// Si el nombre es diferente actualiza el de la entidad.
		if (finalName != originalName)
		{
			e->setName(finalName);
		}

		_entities[guid] = e;
		_entitiesNames[finalName] = guid;
		e->setScene(this);
		Debug::out("SCENE: Entidad '", finalName, "' [ID: ", guid, "] anyadida con exito.");
	}
}