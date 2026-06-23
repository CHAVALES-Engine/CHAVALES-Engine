#include "Light.h"

#include "RenderModule.h"
#include "Engine.h"
#include "Entity.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"

REGISTER_COMPONENT(Light);

Light::Light() : _type(Type::POINT)
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
	if (!Component::init(p)) return false;
	if (type < 0 || type > 3) {
		Debug::error("[LIGHT] TIPO INCOMPATIBLE!!");
		return false;
	}
	_type = static_cast<Light::Type>(type.get());

	return true;
}
void Light::ready()
{
	_lightID = render()->addLight(getEntity()->getEntityID(), type, color, intensity);
}
void Light::setType(const Type& type)
{
	_type = type;
}

void Light::setColor(const core::Color& _color)
{
	color = _color;
}

void Light::setIntensity(const float& _intensity)
{
	intensity = _intensity;
}

void Light::setSpotRange(const float& _inner, const float& _outer, const float& _falloff) {
	innerAngle = _inner;
	outerAngle = _outer;
	falloff = _falloff;
}


Light::Type Light::getType() const {
	return _type;
}
core::Color Light::getColor() const {
	return color;
}
float Light::getIntensity() const {
	return intensity;
}

float Light::getInnerAngle() const {
	return innerAngle;
}

float Light::getOuterAngle() const {
	return outerAngle;
}

float Light::getFallOff() const {
	return falloff;
}

void Light::destroy()
{
	render()->deleteLight(_lightID);
}