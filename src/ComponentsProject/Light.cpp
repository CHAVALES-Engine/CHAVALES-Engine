#include "Light.h"

#include "RenderModule.h"
#include "Engine.h"
#include "Entity.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"

REGISTER_COMPONENT(Light);

Light::Light() : _type(Type::POINT), _color(1, 1, 1, 1), _intensity(1.0f), _innerAngle(15.0f), _outerAngle(45.0f), _falloff(1.0f)
{
	registerMethod("setType", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setType(std::any_cast<Type>(args[0]));
		}
		});

	registerMethod("setColor", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setColor(std::any_cast<core::Color>(args[0]));
		}
		});

	registerMethod("setIntensity", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setIntensity(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setSpotRange", [this](const std::vector<std::any>& args) {
		if (args.size() >= 3) {
			setSpotRange(std::any_cast<float>(args[0]), std::any_cast<float>(args[1]), std::any_cast<float>(args[2]));
		}
		});

	registerMethod("getType", [this](const std::vector<std::any>& args) {
		return getType();
		});

	registerMethod("getColor", [this](const std::vector<std::any>& args) {
		return getColor();
		});

	registerMethod("getIntensity", [this](const std::vector<std::any>& args) {
		return getIntensity();
		});

	registerMethod("getInnerAngle", [this](const std::vector<std::any>& args) {
		return getInnerAngle();
		});

	registerMethod("getOuterAngle", [this](const std::vector<std::any>& args) {
		return getOuterAngle();
		});

	registerMethod("getFallOff", [this](const std::vector<std::any>& args) {
		return getFallOff();
		});
}

Light::~Light()
{
}
bool Light::init(const Properties& p)
{
	_typeInt = getProperty<int>(p, "type");
	_type = static_cast<Light::Type>(_typeInt);

	_color = getProperty<core::Color>(p, "color");
	_intensity = getProperty<float>(p, "intensity");
	_innerAngle = getProperty<float> (p, "inner");
	_outerAngle = getProperty<float>(p, "outer");
	_falloff = getProperty<float>(p, "falloff");
	return true;
}
void Light::ready()
{
	_lightID = render()->addLight(getEntity()->getEntityID(), _typeInt, _color, _intensity);
}
void Light::setType(const Type& type)
{
	_type = type;
}

void Light::setColor(const core::Color& color)
{
	_color = color;
}

void Light::setIntensity(const float& intensity)
{
	_intensity = intensity;
}

void Light::setSpotRange(const float& inner, const float& outer, const float& falloff) {
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
	render()->deleteLight(_lightID);
}