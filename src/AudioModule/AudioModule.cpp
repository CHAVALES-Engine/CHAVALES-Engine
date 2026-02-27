#include "AudioModule.h"

#include <iostream>
#include <fmod.hpp>

using namespace std;

AudioModule::AudioModule()
{

}
AudioModule::~AudioModule()
{
	if (_system != nullptr)
	{
		_system->release();
	}
}

bool AudioModule::Init()
{
	FMOD_RESULT result;
	//Creates an FMOD System
	result = FMOD::System_Create(&_system);
	if (result != FMOD_OK)
	{
		//throw("FMOD error: Couldn't create system");
		return false;
	}
	//If Creation went OK, the system is initialized
	result = _system->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		//throw("FMOD error: Couldn't initialize system");
		return false;
	}
	return true;
}

void AudioModule::Update()
{
	vector<ChannelMap::iterator> vecStoppedChannel;
	for (auto i = _channelSound.begin(); i != _channelSound.end(); ++i)
	{
		bool soundPlaying = false;
		i->second->isPlaying(&soundPlaying);
		if (!soundPlaying)
		{
			vecStoppedChannel.push_back(i);
		}
	}
	for (auto& it : vecStoppedChannel)
	{
		_channelSound.erase(it);
	}
	_system->update();
}

void AudioModule::loadSound(const char* path, std::string id, bool sound3D, bool soundLooping, bool soundStream)
{
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound != _soundMap.end())
	{
		//throw("Sound cretion error: Couldn't create sound, there is already a sound with this id: " + id);
		return;
	}
	//Depends in the parameters of the method
	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= sound3D ? FMOD_2D : FMOD_3D;
	eMode |= soundLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= soundStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	//Result helps to identifie exceptions
	FMOD_RESULT result;
	FMOD::Sound* sound = nullptr;
	result = _system->createSound(path, eMode, nullptr, &sound);

	if (sound && result == FMOD_OK)
	{
		_soundMap[id] = sound;
	}
	else
	{
		//throw("FMOD error: Couldn't locate sound");
	}
}

void AudioModule::unloadSound(std::string id)
{
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound == _soundMap.end())
	{
		//throw("Sound unloading error: Couldn't find sound, there isn't a loaded sound with this id: " + id);
		return;
	}
	itSoundFound->second->release();
	_soundMap.erase(itSoundFound);
}

int AudioModule::playSound(std::string id, const Vector3<> vec3, float soundVolume)
{
	int nextChID = _nextChannelID++;
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound == _soundMap.end())
	{
		//throw("Sound not found: Couldn't find sound, there isn't a loaded sound with this id: " + id);
		return nextChID;
	}
	FMOD::Channel* channel = nullptr;
	_system->playSound(itSoundFound->second, nullptr, true, &channel);
	//If channel has been correctly created, then registers it
	if (channel) {
		FMOD_VECTOR pos = { vec3.getX(),vec3.getY(),vec3.getZ() };
		channel->set3DAttributes(&pos, nullptr);
		channel->setVolume(soundVolume);
		channel->setPaused(false);
		_channelSound[nextChID] = channel;
	}
	return nextChID;
}
