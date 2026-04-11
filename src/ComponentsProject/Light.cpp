#include "Light.h"

#include "Engine.h"
#include "Entity.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"

REGISTER_COMPONENT(Light);

Light::Light() : _type(Type::POINT), _color(1, 1, 1, 1), _intensity(1.0f), _innerAngle(15.0f), _outerAngle(45.0f), _falloff(1.0f)
{
}

Light::~Light()
{
	Engine::instance()->deleteLight(_lightID);
}
bool Light::init(const Properties& p)
{
	int typeInt = getProperty<int>(p, "type");
	_type = static_cast<Light::Type>(typeInt);

	_color = getProperty<core::Color>(p, "color");
	_intensity = getProperty<float>(p, "intensity");
	_innerAngle = getProperty<float> (p, "inner");
	_outerAngle = getProperty<float>(p, "outer");
	_falloff = getProperty<float>(p, "falloff");

	Engine::instance()->addLight(getEntity()->getEntityID(), typeInt, _color, _intensity);
	return true;
}
void Light::setType(Type type)
{
	_type = type;
}

void Light::setColor(const core::Color& color)
{
	_color = color;
}

void Light::setIntensity(float intensity)
{
	_intensity = intensity;
}

void Light::setSpotRange(float inner, float outer, float falloff) {
	_innerAngle = inner;
	_outerAngle = outer;
	_falloff = falloff;
}


Light::Type Light::getType() const {
	return _type;
}
core::Color Light::getColor() const {
	return _color;
}
float Light::getIntensity() const {
	return _intensity;
}

float Light::getInnerAngle() const {
	return _innerAngle;
}

float Light::getOuterAngle() const {
	return _outerAngle;
}

float Light::getFallOff() const {
	return _falloff;
}

void Light::destroy()
{
	Engine::instance()->deleteLight(_lightID);
}