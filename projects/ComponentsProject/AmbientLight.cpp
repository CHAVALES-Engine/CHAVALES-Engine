#include "AmbientLight.h"
#include <Debug.h>
#include <PluginSDK.h>
#include "RenderModule.h"
#include "Engine.h"

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
	render()->setAmbientLight(color);
}

void AmbientLight::clearAmbientLight()
{
	render()->setAmbientLight(core::Color(0.0f, 0.0f, 0.0f, 0.0f));
}
