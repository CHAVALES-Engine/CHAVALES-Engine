#include "Scene.h"

#include <cstdint>
#include "Entity.h"
#include "checkMLNew.h"
namespace core {
	core::Scene::Scene(sceneName name) :
		_name(name)
	{
		_active = false;
	}

	core::Scene::~Scene()
	{
		_entities.clear();
		_entitiesNames.clear();
	}

	void core::Scene::onEnable()
	{
		_active = true;

		if (!_entities.empty())
		{
			for (auto& [guid, e] : _entities)
			{
				e->enable();
			}
		}
	}

	//void core::Scene::init()
	//{
	//	if (!_entities.empty())

	//	{
	//		for (auto e : _entities)
	//		{
	//			e->init();
	//		}
	//	}
	//}

	void core::Scene::ready()
	{
		if (_entities.empty()) return;

		std::vector<entityID> guids;
		for (const auto& [guid, _] : _entities)
			guids.push_back(guid);

		for (const auto& guid : guids)
		{
			auto it = _entities.find(guid);
			if (it != _entities.end() && it->second)
				it->second->ready();
		}
	}

	void core::Scene::fixedUpdate()
	{
		if (_entities.empty()) return;
		// Copiar los guids para iterar de forma segura
		// pos si se anyade o quitan entidades en runtime
		std::vector<entityID> guids;
		for (const auto& [guid, _] : _entities)
			guids.push_back(guid);

		for (auto& guid : guids)
		{
			auto it = _entities.find(guid);
			if (it != _entities.end() && it->second)
				it->second->fixedUpdate();
		}

	}

	void core::Scene::update(uint64_t dT)
	{
		if (_entities.empty()) return;

		// Copiar los guids para iterar de forma segura
		// pos si se anyade o quitan entidades en runtime
		std::vector<entityID> guids;
		for (const auto& [guid, _] : _entities)
			guids.push_back(guid);

		for (auto& guid : guids)
		{
			auto it = _entities.find(guid);
			if (it != _entities.end() && it->second)
				it->second->update(dT);
		}
	}

	//void core::Scene::render()
	//{
	//	if (!_entities.empty())
	//	{
	//		for (auto e : _entities)
	//		{
	//			e->render();
	//		}
	//	}
	//}

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

	void core::Scene::onDestroy() // elimina al completo
	{
		if (!_entities.empty())
		{
			for (auto& [guid, e] : _entities)
			{
				delete e;
			}
			_entities.clear();
		}
		if (!_entitiesNames.empty())
		{
			_entitiesNames.clear();
		}
	}

	void core::Scene::clearScene() // comprueba dont destroy on load
	{
		for (auto it = _entities.begin(); it != _entities.end(); )
		{
			auto& [guid, e] = *it;

			if (!e->getDontDestroyOnLoad())
			{
				delete e;
				it = _entities.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void core::Scene::addEntity(Entity* e)
	{
		entityID guid = ChavalesGUID::generate();
		e->setEntityID(guid);

		std::string originalName = e->getName();
		std::string finalName = originalName;
		int counter = 1;

		// iterar hasta encontrar un nombre valido.
		while (_entitiesNames.find(finalName) != _entitiesNames.end())
		{
			Debug::warning("SCENE: Nombre duplicado. ", finalName);
			finalName = originalName + "_" + std::to_string(counter);
			counter++;
		}
		// Si el nombre es diferente actualiza el de la entidad.
		if (finalName != originalName)
		{
			e->setName(finalName);
			Debug::warning("SCENE: Nombre duplicado. Renombrado de '", originalName, "' a '", finalName, "'");
		}

		_entities[guid] = e;
		_entitiesNames[finalName] = guid;
		e->setScene(this);
	}

	std::unordered_map<ChavalesGUID, core::Entity*> core::Scene::getEntities() const
	{
		return _entities;
	}

	Entity* core::Scene::findEntityByName(std::string const& name) const
	{
		auto namIt = _entitiesNames.find(name);
		if (namIt == _entitiesNames.end()) return nullptr;
		auto entIt = _entities.find(namIt->second);
		if (entIt != _entities.end()) return entIt->second;
		return nullptr;
	}
}
