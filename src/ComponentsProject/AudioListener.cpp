#include "AudioListener.h"
#include "AudioModule.h"
#include "Entity.h"
#include "Engine.h"
#include "Transform.h"

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
	const core::Vector3<> currentPos = _transform->getGlobalPosition();
	core::Vector3<> vel(0.0f, 0.0f, 0.0f);
	if (deltaTime > 0)
	{
		// deltaTime llega en ms -> FMOD espera unidades por segundo
		const float dtSeconds = deltaTime / 1000.0f;
		vel = (currentPos - _lastPos) / dtSeconds;
	}
	_lastPos = currentPos;
	audio()->setListener(currentPos, _transform->forward(), _transform->up(), vel);
}