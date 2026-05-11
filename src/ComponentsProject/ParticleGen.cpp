#include "ParticleGen.h"

#include "Entity.h"
#include "Engine.h"
#include "checkMLNew.h"
#include "PluginSDK.h"

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
	//Engine::instance()->deleteParticleGen(_particleGenID);
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
	Engine::instance()->setParticleGenEnabled(_particleGenID, true);
}

void ParticleGen::disable()
{
	Engine::instance()->setParticleGenEnabled(_particleGenID, false);
}

void ParticleGen::destroy()
{
	Engine::instance()->deleteParticleGen(_particleGenID);
}

void ParticleGen::ready()
{
	_particleGenID = Engine::instance()->addParticleGen(getEntity()->getEntityID(), _textureName);
	Engine::instance()->setParticleGenPartWidth(_particleGenID, _partWidth);
	Engine::instance()->setParticleGenPartHeight(_particleGenID, _partHeight);
	Engine::instance()->setParticleGenEmissionRate(_particleGenID, _emissionRate);
	Engine::instance()->setParticleGenMinVelocity(_particleGenID, _minVelocity);
	Engine::instance()->setParticleGenMaxVelocity(_particleGenID, _maxVelocity);
	Engine::instance()->setParticleGenDirection(_particleGenID, _direction);
	Engine::instance()->setParticleGenAngle(_particleGenID, _angle);
}

void ParticleGen::setEmitting(const bool& emitting)
{
	Engine::instance()->setParticleGenEmitting(_particleGenID, emitting);
}

void ParticleGen::setQuota(const float& quota)
{
	Engine::instance()->setParticleGenQuota(_particleGenID, quota);
}

void ParticleGen::setEmissionRate(const float& rate)
{
	Engine::instance()->setParticleGenEmissionRate(_particleGenID, rate);
}

void ParticleGen::setDuration(const float& duration)
{
	Engine::instance()->setParticleGenDuration(_particleGenID, duration);
}

void ParticleGen::setTimeToLive(const float& time)
{
	Engine::instance()->setParticleGenTimeToLive(_particleGenID, time);
}

void ParticleGen::setVelocity(const float& velocity)
{
	Engine::instance()->setParticleGenVelocity(_particleGenID, velocity);
}

void ParticleGen::setMinVelocity(const float& velocity)
{
	Engine::instance()->setParticleGenMinVelocity(_particleGenID, velocity);
}

void ParticleGen::setMaxVelocity(const float& velocity)
{
	Engine::instance()->setParticleGenMaxVelocity(_particleGenID, velocity);
}

void ParticleGen::setDirection(const core::Vector3<float>& direction)
{
	Engine::instance()->setParticleGenDirection(_particleGenID, direction);
}

void ParticleGen::setAngle(const float& angle)
{
	Engine::instance()->setParticleGenAngle(_particleGenID, angle);
}

void ParticleGen::setPartWidth(const float& width)
{
	Engine::instance()->setParticleGenPartWidth(_particleGenID, width);
}

void ParticleGen::setPartHeight(const float& height)
{
	Engine::instance()->setParticleGenPartHeight(_particleGenID, height);
}

void ParticleGen::setPartColor(const core::Color& color)
{
	Engine::instance()->setParticleGenPartColor(_particleGenID, color);
}