#include "AudioModule.h"
#include "Debug.h"

#include <iostream>
#include <fmod.hpp>

using namespace std;

AudioModule::AudioModule() : _nextChannelID(0)
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
		Debug::error("FMOD error: Couldn't create system");
		return false;
	}
	//If Creation went OK, the system is initialized
	result = _system->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		Debug::error("FMOD error: Couldn't initialize system");
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

void AudioModule::ShutDown()
{
	//Destructora
	_system->release();
	delete _system;	
}

bool AudioModule::loadSound(const char* path, std::string id, bool sound3D, bool soundLooping, bool soundStream)
{
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound != _soundMap.end())
	{
		Debug::error("Sound creation error: Couldn't create sound, there is already a sound with this id: " + id);
		return false;
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
		return true;
	}
	else
	{
		Debug::error("FMOD error: Couldn't locate sound");
		return false;
	}
}

bool AudioModule::unloadSound(std::string id)
{
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound == _soundMap.end())
	{
		Debug::error("Sound unloading error: Couldn't find sound, there isn't a loaded sound with this id: " + id);
		return false;
	}
	itSoundFound->second->release();
	_soundMap.erase(itSoundFound);
	return true;
}

int AudioModule::playSound(std::string id, const core::Vector3<> vec3, float soundVolume, int looping)
{
	int nextChID = _nextChannelID++;
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound == _soundMap.end())
	{
		Debug::error("Sound not found: Couldn't find sound, there isn't a loaded sound with this id: " + id);
		return -1;
	}
	FMOD::Channel* channel = nullptr;
	_system->playSound(itSoundFound->second, nullptr, true, &channel);
	//If channel has been correctly created, then registers it
	if (channel) {
		FMOD_VECTOR pos = { vec3.getX(),vec3.getY(),vec3.getZ() };
		channel->set3DAttributes(&pos, nullptr);
		channel->setVolume(soundVolume);
		channel->setPaused(false);
		channel->setLoopCount(looping);
		_channelSound[nextChID] = channel;
	}
	return nextChID;
}

bool AudioModule::setChannelVolume(int chID, float newVolume)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	itChFound->second->setVolume(newVolume);
	return true;
}
bool AudioModule::getLooping(int chID, int* typeOfLooping)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	itChFound->second->getLoopCount(typeOfLooping);
	return true;
}

bool AudioModule::stopPlaying(int chID)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	itChFound->second->stop();
	return true;
}

bool AudioModule::pauseChannel(int chID, bool pause)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	return itChFound->second->setPaused(pause);
}

void AudioModule::muteEverything()
{
	for (auto it = _channelSound.begin(); it != _channelSound.end(); ++it) {
		bool isPlaying = false;
		it->second->isPlaying(&isPlaying);
		if (isPlaying) {
			it->second->setPaused(false);
		}
	}
}

void AudioModule::unMuteEverything()
{
	for (auto it = _channelSound.begin(); it != _channelSound.end(); ++it) {
		bool isPlaying = false;
		it->second->isPlaying(&isPlaying);
		if (!isPlaying) {
			it->second->setPaused(false);
		}
	}
}