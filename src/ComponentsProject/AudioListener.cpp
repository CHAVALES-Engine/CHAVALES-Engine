#include "AudioListener.h"
#include "Transform.h"
#include "Entity.h"
#include "Engine.h"

#include "PluginSDK.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(AudioListener);

AudioListener::AudioListener() :
	_transform(nullptr),
	_lastPos()
{
}

AudioListener::~AudioListener()
{
	_transform = nullptr;
}

bool AudioListener::init(const Properties& p)
{
	return true;
}

void AudioListener::ready()
{
	_transform = entity->getComponent<Transform>();
	_lastPos = _transform->getGlobalPosition();
}

void AudioListener::update(uint64_t deltaTime)
{
	if (!_transform)
		_transform = getEntity()->getComponent<Transform>();
	if (!_transform) return;
	core::Vector3<> vel = (_transform->getGlobalPosition() - _lastPos) / deltaTime;
	_lastPos = _transform->getGlobalPosition();
	Engine::instance()->setListener(_transform->getGlobalPosition(), _transform->forward(), _transform->up(), vel);
}
