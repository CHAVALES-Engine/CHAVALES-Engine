#pragma once
#include <cstdint>

class Scene
{
public:
	Scene();
	~Scene();

	virtual void onCreate();
	virtual void onEnable();
	virtual void init();

	virtual void fixedUpdate();
	virtual void update(uint64_t dt);
	virtual void render();

	virtual void onDisable();
	virtual void onDestroy();

private:

};

