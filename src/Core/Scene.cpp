#include "Scene.h"

#include <cstdint>

#include "Entity.h"

class Entity;

core::Scene::Scene(sceneName name) :
	_name(name)
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

	for (auto e: _entities)
	{
		e->enable();
	}
}

void core::Scene::init()
{
	for (auto e : _entities)
	{
		e->init();
	}
}

void core::Scene::fixedUpdate()
{
	for (auto e : _entities)
	{
		e->fixedUpdate();
	}
}

void core::Scene::update(uint64_t dT)
{
	for (auto e : _entities)
	{
		e->update(dT);
	}
}

void core::Scene::render()
{
	for (auto e : _entities)
	{
		e->render();
	}
}

void core::Scene::onDisable()
{
	_active = false;

	for (auto e : _entities)
	{
		e->disable();
	}
}

void core::Scene::onDestroy()
{
	for (auto e : _entities)
	{
		e->destroy();
	}
}

void core::Scene::addEntity(Entity* e)
{
	_entities.push_back(e);
}
