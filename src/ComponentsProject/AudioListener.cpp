#include "AudioListener.h"
#include "Transform.h"
#include "Entity.h"
#include "Engine.h"

#include "PluginSDK.h"

REGISTER_COMPONENT(AudioListener);

AudioListener::AudioListener() :
	_transform(nullptr),
	_eng(nullptr),
	_lastPos()
{
}

AudioListener::~AudioListener()
{
	_transform = nullptr;
	_eng = nullptr;
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
	core::Vector3<> vel = (_transform->getGlobalPosition() - _lastPos) / deltaTime;
	_lastPos = _transform->getGlobalPosition();
	_eng->setListener(_transform->getGlobalPosition(), _transform->forward(), _transform->up(), vel);
}
