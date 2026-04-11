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






