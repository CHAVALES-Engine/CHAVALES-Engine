#include "Scene.h"

#include <cstdint>

#include "EngineAPI.h"
#include "Entity.h"

class Entity;

core::Scene::Scene(sceneName name) :
	_name(name), _nextEntityID(0)
{
	_active = false;
}

core::Scene::~Scene()
{
	_entities.clear();
}

void core::Scene::onEnable()
{
	_active = true;

	if (!_entities.empty())
	{
		for (auto e : _entities)
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
		for (auto e : _entities)
		{
			e->ready();
		}
	}
}

void core::Scene::fixedUpdate()
{
	if (!_entities.empty())
	{
		for (auto e : _entities)
		{
			e->fixedUpdate();
		}
	}
}

void core::Scene::update(uint64_t dT)
{
	if (!_entities.empty())
	{
		for (auto e : _entities)
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
		for (auto e : _entities)
		{
			e->disable();
		}
	}
}

void core::Scene::onDestroy()
{
	if (!_entities.empty())
	{
		for (auto e : _entities)
		{
			// solo se destruye si debe
			if (!e->getDontDestoroyOnLoad())
			{
				e->destroy();
			}
		}
	}
}

void core::Scene::endGame()
{
	if (!_entities.empty())
	{
		for (auto e : _entities)
		{
			e->destroy();
		}
	}
}

void core::Scene::addEntity(Entity* e)
{
	e->setEntityID(_nextEntityID++);
	_entities.push_back(e);
}

std::vector<core::Entity*> core::Scene::getEntities() const
{
	return _entities;
}


