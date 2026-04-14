#include "ParticleGen.h"

#include "Entity.h"
#include "Engine.h"
#include "checkMLNew.h"

ParticleGen::ParticleGen() {}

ParticleGen::~ParticleGen()
{
	Engine::instance()->deleteParticleGen(_particleGenID);
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

void ParticleGen::ready()
{
	_particleGenID = Engine::instance()->addParticleGen(getEntity()->getEntityID(), _textureName);
	Engine::instance()->setParticleGenPartWidth(_particleGenID, _partWidth);
	Engine::instance()->setParticleGenPartWidth(_particleGenID, _partHeight);
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






