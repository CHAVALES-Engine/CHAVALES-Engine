#include "AudioSource.h"
#include "Engine.h"
#include "Transform.h"
#include "Entity.h"
#include <Debug.h>
#include <PluginSDK.h>


REGISTER_COMPONENT(AudioSource);

AudioSource::AudioSource(): _tr(nullptr), _lastPosition(0.0f, 0.0f, 0.0f), _id(), _mute(false), _is3D(false), _loop(false),
_isStream(false), _soundVolume(0.0f),_chanelsID()
{
}

AudioSource::~AudioSource()
{
}

bool AudioSource::init(const Properties& p)
{
	_id = getProperty<std::string>(p, "soundID");
	_mute = getProperty<bool>(p, "mute");
	_is3D = getProperty<bool>(p, "is3D");
	_loop = getProperty<bool>(p, "loop");
	_isStream = getProperty<bool>(p, "isStream");
	_soundVolume = getProperty<float>(p, "soundVolume");

	return true;
}

void AudioSource::ready()
{
	assert(entity->hasComponent<Transform>());
	_tr = entity->getComponent<Transform>();
	_lastPosition = _tr->getGlobalPosition();
}

void AudioSource::update(uint64_t deltaTime)
{
	core::Vector3<> velocity = (_tr->getGlobalPosition() - _lastPosition) / deltaTime;
	_lastPosition = _tr->getGlobalPosition();

	for (int channelID : _chanelsID) {

		Engine::instance()->setSourcePosition(channelID, _tr->getGlobalPosition(), velocity);
	}
	for (auto it = _chanelsID.begin(); it != _chanelsID.end(); )
	{
		if (!Engine::instance()->isChannelPlaying(*it))
		{
			it = _chanelsID.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void AudioSource::playSound(std::string id, float soundVolume, bool loop)
{
	int looping = 0;
	_loop = loop;
	if (_loop) looping = -1;
	_soundVolume = soundVolume; 
	int chanelID = Engine::instance()->playSound(id, _tr->getGlobalPosition(), _soundVolume, looping);
	if (chanelID != -1) {
		_chanelsID.push_back(chanelID);
	}
}

