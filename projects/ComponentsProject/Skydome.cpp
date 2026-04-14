#include "Skydome.h"
#include "Engine.h"

Skydome::Skydome()
{
}

Skydome::~Skydome()
{
}

bool Skydome::init(const Properties& p)
{
	_enabledOnStart = getProperty<bool>(p, "enabled on start");
	_textureName = getProperty<std::string>(p, "texture");

	_curvature = getProperty<float>(p, "curvature");
	_tiling = getProperty<float>(p, "tiling");
	_distance = getProperty<float>(p, "distance");
	_drawFirst = getProperty<bool>(p, "draw first");
	return true;
}

void Skydome::ready()
{
	if (_enabledOnStart)
	{
		setSkydome();
	}
}

void Skydome::disable()
{
	setSkydomeNull();
}

void Skydome::setSkydome()
{
	Engine::instance()->setSkydome(_textureName, _curvature, _tiling, _distance, _drawFirst);
}

void Skydome::setSkydomeNull()
{
	Engine::instance()->setSkydomeNull();
}




