#include "AudioSource.h"
#include <Debug.h>
#include <Entity.h>
#include <PluginSDK.h>

#include "AudioModule.h"
#include "ResourcesModule.h"

#include "Engine.h"
#include "Transform.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(AudioSource);

AudioSource::AudioSource() : _tr(nullptr), _lastPosition(0.0f, 0.0f, 0.0f), _path(), _id(), _is3D(false), _loop(false),
_isStream(), _playOnReady(), _soundVolume(0.0f), _minRadius(1.0f), _maxRadius(100.0f), _channelID(-1), isPlaying(false)
{
}

AudioSource::~AudioSource()
{
}

bool AudioSource::init(const Properties& p)
{
	_path = getProperty<std::string>(p, "soundPath");
	_id = getProperty<std::string>(p, "soundID");
	_is3D = getProperty<bool>(p, "is3D");
	_loop = getProperty<bool>(p, "loop");
	_isStream = getProperty<bool>(p, "isStream");
	_playOnReady = getProperty<bool>(p, "playOnReady");
	_soundVolume = getProperty<float>(p, "soundVolume");
	_minRadius = getProperty<float>(p, "minRadius");
	_maxRadius = getProperty<float>(p, "maxRadius");
	return true;
}

void AudioSource::ready()
{
	assert(entity->hasComponent<Transform>());
	_tr = entity->getComponent<Transform>();
	_lastPosition = _tr->getGlobalPosition();
	std::string audioPath = resources()->getAssetPath(_path);
	if (audioPath.empty()) {
		Debug::error("[AudioSource] Audio no encontrado: ", _path);
	}

	audio()->loadSound(audioPath, _id, _isStream, _loop, _is3D);
	if (_playOnReady)
	{
		playSound();
	}

}

void AudioSource::update(uint64_t deltaTime)
{
	float dt = deltaTime / 1000.0f;

	isPlaying = audio()->isChannelPlaying(_channelID);

	if (_is3D && dt > 0 && isPlaying) {
		auto currentPos = _tr->getGlobalPosition();
		auto velocity = (currentPos - _lastPosition) / dt;

		audio()->setAudioPos(_channelID, currentPos, velocity);

		_lastPosition = currentPos;
	}
	if (!isPlaying) {
		_channelID = -1;
	}
}

void AudioSource::disable()
{
	pauseSound(true);
}
void AudioSource::destroy()
{
	if (isPlaying) stopSound();
}
void AudioSource::enable()
{
	if (isPlaying && audio()->isPaused(_channelID)) pauseSound(false);
}

void AudioSource::playSound()
{
	int looping = 0;
	if (_loop) looping = -1;

	if (isPlaying)
	{
		stopSound();
	}
	_channelID = audio()->playSound(_id, _soundVolume, looping, _tr->getGlobalPosition());
	isPlaying = _channelID != -1;

	if (_is3D)
	{
		audio()->setMinMaxRadius(_channelID, _minRadius, _maxRadius);
	}
}

int AudioSource::getLooping() const
{
	if (!isPlaying) return false;
	int looping = 0;
	audio()->getLooping(_channelID, &looping);
	return looping;
}

void AudioSource::setLooping(int& loop) const
{
	if (!isPlaying) return;
	audio()->setLooping(_channelID, loop);
}
void AudioSource::setLooping(int&& loop) const
{
	if (!isPlaying) return;
	audio()->setLooping(_channelID, loop);
}

float AudioSource::getVolume() const
{
	if (!isPlaying) return -1.0;
	float volume = 0.0f;
	audio()->getVolume(_channelID, volume);
	return volume;
}

void AudioSource::setVolume(float& newVolume)
{
	if (!isPlaying) return;
	_soundVolume = newVolume;
	audio()->setChannelVolume(_channelID, _soundVolume);
}

void AudioSource::setVolume(float&& newVolume)
{
	if (!isPlaying) return;
	_soundVolume = newVolume;
	audio()->setChannelVolume(_channelID, _soundVolume);
}

void AudioSource::setMinRadius(float& newRadius)
{
	if (!isPlaying) return;
	_minRadius = newRadius;
	audio()->setMinMaxRadius(_channelID, _minRadius, _maxRadius);
}

void AudioSource::setMinRadius(float&& newRadius)
{
	if (!isPlaying) return;
	_minRadius = newRadius;
	audio()->setMinMaxRadius(_channelID, _minRadius, _maxRadius);
}

float AudioSource::getMinRadius() const
{
	return _minRadius;
}

void AudioSource::setMaxRadius(float& newRadius)
{
	if (!isPlaying) return;
	_maxRadius = newRadius;
	audio()->setMinMaxRadius(_channelID, _minRadius, _maxRadius);
}

void AudioSource::setMaxRadius(float&& newRadius)
{
	if (!isPlaying) return;
	_maxRadius = newRadius;
	audio()->setMinMaxRadius(_channelID, _minRadius, _maxRadius);
}

float AudioSource::getMaxRadius() const
{
	return _maxRadius;
}

void AudioSource::stopSound()
{
	if (!isPlaying) return;

	audio()->stopPlaying(_channelID);
	_channelID = -1;
	isPlaying = false;
}

void AudioSource::pauseSound(bool pause) const
{
	if (!isPlaying) return;
	audio()->pauseChannel(_channelID, pause);
}
bool AudioSource::isPaused() const
{
	if (!isPlaying) return false;
	return audio()->isPaused(_channelID);
}

void AudioSource::setDelay(double start, double end, bool stopChannel) const
{
	if (!isPlaying) return;
	audio()->setDelay(_channelID, start, end, stopChannel);
}

const std::string& AudioSource::getSoundName() const
{
	return _id;
}