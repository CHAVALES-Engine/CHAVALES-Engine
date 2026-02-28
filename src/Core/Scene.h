#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Entity;

namespace core
{
	class Entity;
	using sceneName = std::string;

	class Scene
	{
	public:
		Scene(sceneName name);
		~Scene();

		virtual void onEnable();
		virtual void init();
		//virtual void onCreate();

		virtual void fixedUpdate();
		virtual void update(uint64_t dT);
		virtual void render();

		virtual void onDisable();
		virtual void onDestroy();

		void addEntity(core::Entity* e);

	private:
		sceneName _name;
		bool _active = false;

		std::vector<core::Entity*> _entities;
	};
}
