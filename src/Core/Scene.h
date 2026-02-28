#pragma once
#include <cstdint>
#include <string>

namespace core
{
	using sceneName = std::string;

	class Scene
	{
	public:
		Scene();
		~Scene();

		virtual void onEnable();
		virtual void init();
		//virtual void onCreate(); // igual que init?

		virtual void fixedUpdate();
		virtual void update(uint64_t dt);
		virtual void render();

		virtual void onDisable();
		virtual void onDestroy();

	private:
		sceneName name;
	};
}
