#include "AudioSource.h"
#include "Engine.h"
#include "Transform.h"
#include "Entity.h"
#include <Debug.h>
#include <PluginSDK.h>


REGISTER_COMPONENT(AudioSource);

AudioSource::AudioSource() : _tr(nullptr), _lastPosition(0.0f, 0.0f, 0.0f), _id(), _mute(false), _is3D(false), _loop(false),
_isStream(),_playOnReady(), _soundVolume(0.0f), _channelID()
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
	_playOnReady = getProperty<bool>(p, "playOnReady");
	_soundVolume = getProperty<float>(p, "soundVolume");
	return true;
}

void AudioSource::ready()
{
	assert(entity->hasComponent<Transform>());
	_tr = entity->getComponent<Transform>();
	_lastPosition = _tr->getGlobalPosition();
	Engine::instance()->loadSound(Engine::instance()->getAudioByName(_id), _id, _is3D, _loop, _isStream);
	if (_playOnReady)
		playSound();
}

void AudioSource::update(uint64_t deltaTime)
{
	if (_is3D) {
		core::Vector3<> velocity = (_tr->getGlobalPosition() - _lastPosition) / deltaTime;
		_lastPosition = _tr->getGlobalPosition();
		Engine::instance()->setSourcePosition(_channelID, _tr->getGlobalPosition(), velocity);
	}
}

void AudioSource::playSound()
{
	int looping = 0;
	if (_loop) looping = -1;
	_channelID = Engine::instance()->playSound(_id, _soundVolume, looping, _tr->getGlobalPosition());
}

int AudioSource::getLooping() const
{
	return Engine::instance()->getLooping(_channelID);
}

void AudioSource::setLooping(int& loop) const
{
	Engine::instance()->setLooping(_channelID, loop);
}
void AudioSource::setLooping(int&& loop) const
{
	Engine::instance()->setLooping(_channelID, loop);
}

float AudioSource::getVolume() const
{
	return Engine::instance()->getVolume(_channelID);
}

void AudioSource::setVolume(float& newVolume)
{
	_soundVolume = newVolume;
	Engine::instance()->setChannelVolume(_channelID, _soundVolume);
}

void AudioSource::setVolume(float&& newVolume)
{
	_soundVolume = newVolume;
	Engine::instance()->setChannelVolume(_channelID, _soundVolume);
}

void AudioSource::stopSound() const
{
	Engine::instance()->stopPlaying(_channelID);
}

void AudioSource::pauseSound(bool pause) const
{
	Engine::instance()->pauseChannel(_channelID, pause);
}

void AudioSource::setDelay(double start, double end, bool stopChannel) const
{
	Engine::instance()->setDelay(_channelID, start, end, stopChannel);
}

std::string AudioSource::getSoundName() const
{
	return _id;
}
