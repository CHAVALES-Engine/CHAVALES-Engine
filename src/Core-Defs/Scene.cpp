#include "Scene.h"

#include <cstdint>
#include "Entity.h"
namespace core {
	core::Scene::Scene(sceneName name) :
		_name(name)//, _nextEntityID(ChavalesGUID::invalid())
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
		if (!_entities.empty())
		{
			for (auto& [guid, e] : _entities)
			{
				e->ready();
			}
		}
	}

	void core::Scene::fixedUpdate()
	{
		if (!_entities.empty())
		{
			for (auto& [guid, e] : _entities)
			{
				e->fixedUpdate();
			}
		}
	}

	void core::Scene::update(uint64_t dT)
	{
		if (!_entities.empty())
		{
			for (auto& [guid, e] : _entities)
			{
				e->update(dT);
			}
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

	void core::Scene::onDestroy()
	{
		if (!_entities.empty())
		{
			for (auto& [guid, e] : _entities)
			{
				// solo se destruye si debe
				if (!e->getDontDestoroyOnLoad())
				{
					e->destroy();
				}
			}
		}

		_entities.clear();
	}

	void core::Scene::clearScene()
	{
		if (!_entities.empty())
		{
			for (auto& [guid, e] : _entities)
			{
				//e->destroy();
				delete e;
			}

			_entities.clear();
		}
	}

	void core::Scene::addEntity(Entity* e)
	{
		entityID guid = ChavalesGUID::generate();
		e->setEntityID(guid);
		_entities[guid] = e;
		_entitiesNames[e->getName()] = guid;
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