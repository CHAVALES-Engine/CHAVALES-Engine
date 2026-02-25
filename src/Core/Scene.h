#pragma once

class Scene
{
public:
	Scene();
	~Scene();

	virtual void onCreate();
	virtual void onEnable();
	virtual void onInit();

	virtual void fixedUpdate();
	virtual void update();
	virtual void render();

	virtual void onDisable();
	virtual void onDestroy();

private:

};

