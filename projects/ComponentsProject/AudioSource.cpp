#include "AudioSource.h"
#include "Engine.h"
#include <Debug.h>
#include <PluginSDK.h>


REGISTER_COMPONENT(AudioSource);

AudioSource::AudioSource(Engine* eng): _eng(eng)
{
}

AudioSource::~AudioSource()
{
}

bool AudioSource::init(const Properties& p)
{
	_position = getProperty<core::Vector3<>>(p, "position");
	return true;
}

void AudioSource::loadSound(const char* path, std::string id, bool sound3D, bool soundLooping, bool soundStream)
{
	_eng->loadSound(path, id, sound3D, soundLooping, soundStream);
}

void AudioSource::unloadSound(std::string id)
{
	_eng->unloadSound(id);
}

void AudioSource::playSound(std::string id, const core::Vector3<> vec3, float soundVolume, int looping)
{
	_eng->playSound(id, vec3, soundVolume, looping);
}

void AudioSource::setChannelVolume(int chID, float newVolume)
{
	_eng->setChannelVolume(chID, newVolume);
}
