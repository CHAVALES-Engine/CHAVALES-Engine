#include "ParticleGen.h"
#include "Entity.h"
#include "PluginSDK.h"

#include "Engine.h"
#include "RenderModule.h"
#include "ResourcesModule.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(ParticleGen);

ParticleGen::ParticleGen()
{
	registerMethod("setEmitting", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setEmitting(std::any_cast<bool>(args[0]));
		}
		});

	registerMethod("setQuota", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setQuota(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setEmissionRate", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setEmissionRate(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setDuration", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setDuration(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setVelocity", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setVelocity(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setMinVelocity", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setMinVelocity(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setMaxVelocity", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setMaxVelocity(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setDirection", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setDirection(std::any_cast<core::Vector3<float>>(args[0]));
		}
		});

	registerMethod("setEmissionRate", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setEmissionRate(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setAngle", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setAngle(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setPartWidth", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setPartWidth(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setPartHeight", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setPartHeight(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setPartColor", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setPartColor(std::any_cast<core::Color>(args[0]));
		}
		});
}

ParticleGen::~ParticleGen()
{
}

bool ParticleGen::init(const Properties& p)
{
	_textureName = getProperty<std::string>(p, "texture name");
	setProperty(p, "particle width", _partWidth);
	setProperty(p, "particle height", _partHeight);
	setProperty(p, "emission rate", _emissionRate);
	setProperty(p, "min velocity", _minVelocity);
	setProperty(p, "max velocity", _maxVelocity);
	setProperty(p, "direction", _direction);
	setProperty(p, "angle", _angle);
	return true;
}

void ParticleGen::enable()
{
	render()->setParticleGenEnabled(_particleGenID, true);
}

void ParticleGen::disable()
{
	render()->setParticleGenEnabled(_particleGenID, false);
}

void ParticleGen::destroy()
{
	render()->deleteParticleGen(_particleGenID);
}

void ParticleGen::ready()
{

	core::ResourcePtr res = resources()->getOrLoadAsset(_textureName);
	if (!res || !res->isValid()) {
		Debug::error("[ParticleGen] Textura no encontrada: ", _textureName);
	}
	else
	{
		_particleGenID = render()->addParticleGen(getEntity()->getEntityID(), res->getPath(), res->getName());
	}
	render()->setParticleGenPartWidth(_particleGenID, _partWidth);
	render()->setParticleGenPartHeight(_particleGenID, _partHeight);
	render()->setParticleGenEmissionRate(_particleGenID, _emissionRate);
	render()->setParticleGenMinVelocity(_particleGenID, _minVelocity);
	render()->setParticleGenMaxVelocity(_particleGenID, _maxVelocity);
	render()->setParticleGenDirection(_particleGenID, _direction);
	render()->setParticleGenAngle(_particleGenID, _angle);
}

void ParticleGen::setEmitting(const bool& emitting)
{
	render()->setParticleGenEmitting(_particleGenID, emitting);
}

void ParticleGen::setQuota(const float& quota)
{
	render()->setParticleGenQuota(_particleGenID, quota);
}

void ParticleGen::setEmissionRate(const float& rate)
{
	render()->setParticleGenEmissionRate(_particleGenID, rate);
}

void ParticleGen::setDuration(const float& duration)
{
	render()->setParticleGenDuration(_particleGenID, duration);
}

void ParticleGen::setTimeToLive(const float& time)
{
	render()->setParticleGenTimeToLive(_particleGenID, time);
}

void ParticleGen::setVelocity(const float& velocity)
{
	render()->setParticleGenVelocity(_particleGenID, velocity);
}

void ParticleGen::setMinVelocity(const float& velocity)
{
	render()->setParticleGenMinVelocity(_particleGenID, velocity);
}

void ParticleGen::setMaxVelocity(const float& velocity)
{
	render()->setParticleGenMaxVelocity(_particleGenID, velocity);
}

void ParticleGen::setDirection(const core::Vector3<float>& direction)
{
	render()->setParticleGenDirection(_particleGenID, direction);
}

void ParticleGen::setAngle(const float& angle)
{
	render()->setParticleGenAngle(_particleGenID, angle);
}

void ParticleGen::setPartWidth(const float& width)
{
	render()->setParticleGenPartWidth(_particleGenID, width);
}

void ParticleGen::setPartHeight(const float& height)
{
	render()->setParticleGenPartHeight(_particleGenID, height);
}

void ParticleGen::setPartColor(const core::Color& color)
{
	render()->setParticleGenPartColor(_particleGenID, color);
}