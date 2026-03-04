#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "EngineAPI.h"

class Entity;

namespace core
{
	class Entity;
	using sceneName = std::string;
	using sceneID = uint64_t;

	class ENGINE_API Scene
	{
	public:
		Scene(sceneName name);
		~Scene();

		virtual void onEnable();
		virtual void init();
		//virtual void ready();

		virtual void fixedUpdate();
		virtual void update(uint64_t dT);
		virtual void render();

		virtual void onDisable();
		virtual void onDestroy();

		void addEntity(core::Entity* e);

		inline sceneName getName()
		{
			return _name;
		}

		inline void setID(sceneID id)
		{
			_id = id;
		}

	private:
		sceneName _name;
		sceneID _id;

		bool _active = false;

		std::vector<core::Entity*> _entities;
	};
}
