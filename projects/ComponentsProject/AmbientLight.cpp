#include "AmbientLight.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"

REGISTER_COMPONENT(AmbientLight);

bool AmbientLight::init(const Properties& p)
{
	core::Color color;
	if (!setProperty(p, "color", color))
		color = core::Color(0.0f, 0.0f, 0.0f, 0.0f);

	setAmbientLight(color);

	return true;
}

void AmbientLight::setAmbientLight(const core::Color& color)
{
	Engine::instance()->setAmbientLight(color);
}

void AmbientLight::clearAmbientLight()
{
	Engine::instance()->setAmbientLight(core::Color(0.0f, 0.0f, 0.0f, 0.0f));
}
