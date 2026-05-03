#include "AudioModule.h"
#include "Debug.h"

#include <iostream>
#include <fmod.hpp>
#include "checkMLNew.h"

using namespace std;

AudioModule::AudioModule() : _nextChannelID(0),nativeRate()
{

}
AudioModule::~AudioModule()
{
	shutdown();
}

bool AudioModule::init()
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
	_system->set3DSettings(1.0f, 1.0f, 1.0f);
	_system->getSoftwareFormat(&nativeRate, 0, 0);
	return true;
}

void AudioModule::update()
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
		it->second->stop(); 
		_channelSound.erase(it);
	}
	_system->update();
}

void AudioModule::shutdown()
{
	//Destructora

	for (auto& s : _soundMap)
	{
		s.second->release();
	}
	_soundMap.clear();

	for (auto& ch : _channelSound)
	{
		ch.second->stop();
	}
	_channelSound.clear();

	_system->close();
	_system->release();
	_system = nullptr;
}

bool AudioModule::loadSound(string path, string id, bool soundStream, bool soundLooping, bool sound3D)
{
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound != _soundMap.end())
	{
		Debug::error("Sound creation error: Couldn't create sound, there is already a sound with this id: " + id);
		return false;
	}
	//Depends in the parameters of the method
	FMOD_MODE eMode = FMOD_DEFAULT;
	if (sound3D) {
		eMode |= FMOD_3D | FMOD_3D_LINEARROLLOFF;
	}
	else {
		eMode |= FMOD_2D;
	}
	eMode |= soundLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= soundStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	//Result helps to identifie exceptions
	FMOD_RESULT result;
	FMOD::Sound* sound = nullptr;

	result = _system->createSound(path.c_str(), eMode, nullptr, &sound);

	if (result == FMOD_OK)
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
		Debug::error("[unloadSound] Sound unloading error: Couldn't find sound, there isn't a loaded sound with this id: " + id);
		return false;
	}
	itSoundFound->second->release();
	_soundMap.erase(itSoundFound);
	return true;
}

int AudioModule::playSound(std::string id, float soundVolume, int looping, const core::Vector3<> pos3, const core::Vector3<> vel3)
{
	int nextChID = _nextChannelID++;
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound == _soundMap.end())
	{
		Debug::error("[playSound] Sound not found: Couldn't find sound, there isn't a loaded sound with this id: " + id);
		return -1;
	}
	FMOD::Channel* channel = nullptr;
	FMOD_RESULT result = _system->playSound(itSoundFound->second, nullptr, true, &channel);
	//If channel has been correctly created, then registers it
	if (result == FMOD_OK) {
		FMOD_VECTOR pos = { pos3.getX(),pos3.getY(),pos3.getZ() };
		FMOD_VECTOR vel = { vel3.getX(),vel3.getY(),vel3.getZ() };
		channel->set3DAttributes(&pos, &vel);
		channel->setVolume(soundVolume);
		channel->setPaused(false);
		if (channel == nullptr) {
			Debug::error("Channel is null after playSound");
		}
		channel->setLoopCount(looping);
		_channelSound[nextChID] = channel;
	}
	else 
	{
		Debug::error("[playSOund] The sound hasn't been genrated correctly");
		return -1;
	}
	return nextChID;
}

bool AudioModule::setChannelVolume(int chID, float newVolume)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[setChannelVolume] Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	auto res = itChFound->second->setVolume(newVolume);
	if(res == FMOD_OK) return true;
	else {
		Debug::error("[setChannelVolume] Unexpected Error: Couldn't set the volume of the channel with this id: " + to_string(chID));
		return false;
	}
}
bool AudioModule::getLooping(int chID, int* typeOfLooping)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[getLooping] Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	auto res = itChFound->second->getLoopCount(typeOfLooping);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[getLooping] Unexpected Error: Couldn't get the loop configuration of the channel with this id: " + to_string(chID));
		return false;
	}
}

bool AudioModule::setLooping(int chID, int typeOfLooping)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[setLooping] Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	auto res = itChFound->second->setLoopCount(typeOfLooping);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[setLooping] Unexpected Error: Couldn't set the loop configuration of the channel with this id: " + to_string(chID));
		return false;
	}
}

bool AudioModule::stopPlaying(int chID)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[stopPlaying] Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	auto res = itChFound->second->stop();
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[stopPlaying] Unexpected Error: Couldn't stop the channel with this id: " + to_string(chID));
		return false;
	}
}

bool AudioModule::pauseChannel(int chID, bool pause)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[pauseChannel] Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	auto res =  itChFound->second->setPaused(pause);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[pauseChannel] Unexpected Error: Couldn't pause/resume the channel with this id: " + to_string(chID));
		return false;
	}
}

void AudioModule::setListener(core::Vector3<> pos, core::Vector3<> forward, core::Vector3<> up, core::Vector3<> vel)
{
	FMOD_VECTOR _pos = { pos.getX(), pos.getY(), pos.getZ() };
	FMOD_VECTOR _vel = { vel.getX(), vel.getY(), vel.getZ() };
	FMOD_VECTOR _forward = { forward.getX(), forward.getY(), forward.getZ() };
	FMOD_VECTOR _up = { up.getX(), up.getY(), up.getZ() };
	_system->set3DListenerAttributes(0, &_pos, &_vel, &_forward, &_up);
}

void AudioModule::muteEverything()
{
	for (auto it = _channelSound.begin(); it != _channelSound.end(); ++it) {
		bool isPlaying = false;
		it->second->isPlaying(&isPlaying);
		if (isPlaying) {
			it->second->setPaused(true);
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

bool AudioModule::setAudioPos(int chID, core::Vector3<> pos, core::Vector3<> vel)
{
	auto itCH = _channelSound.find(chID);
	if (itCH == _channelSound.end())
	{
		Debug::error("[setAudioPos] Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	FMOD_VECTOR position = { pos.getX(),pos.getY(),pos.getZ() };
	FMOD_VECTOR velocity = { vel.getX(),vel.getY(),vel.getZ() };
	auto res = itCH->second->set3DAttributes(&position, &velocity);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[setAudiPos] Unexpected Error: Couldn't set the position/velocity of the sound in channel with this id: " + to_string(chID));
		return false;
	}
}

bool AudioModule::setMinMaxRadius(int chID, float min, float max)
{
	auto itCH = _channelSound.find(chID);
	if (itCH == _channelSound.end())
	{
		Debug::error("[setMinMaxRadius] Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	itCH->second->set3DMinMaxDistance(min, max);
	FMOD::Sound* sound = nullptr;
	auto res = itCH->second->getCurrentSound(&sound);
	if (sound != nullptr)
		sound->set3DMinMaxDistance(min, max);
	else
	{
		Debug::warning("[setMinMaxRadius] Careful, there isn't a sound assocaited with this channel id: " + to_string(chID) + ". The Min/Max Radius parametres may override to the default settigs (1,10000)");
		return false;
	}

	if (res == FMOD_OK) return true;
	else
	{
		Debug::error("[setMinMaxRadius] Unexpected Error: Couldn't set the min/max radius of the sound in channel with this id: " + to_string(chID));
		return false;
	}
}

bool AudioModule::isChannelPlaying(int chID)
{
	auto itChFound = _channelSound.find(chID);

	if (itChFound == _channelSound.end())
	{
		Debug::error("[isChannelPlaying] Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}

	bool isPlaying = false;
	itChFound->second->isPlaying(&isPlaying);

	return isPlaying;
}

bool AudioModule::setDelay(int chID, double start, double end, bool stopChannel)
{
	unsigned long long sampleStart = (start / 1000) * nativeRate;
	unsigned long long sampleEnd = (end / 1000) * nativeRate;
	auto itChFound = _channelSound.find(chID);

	if (itChFound == _channelSound.end())
	{
		Debug::error("[setDelay] Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	auto res = itChFound->second->setDelay(sampleStart, sampleEnd, stopChannel);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[setDelay] Unexpected Error: Couldn't set the delay in channel with this id: " + to_string(chID));
		return false;
	}
}

bool AudioModule::getVolume(int chID, float& volume)
{
	auto itChFound = _channelSound.find(chID);

	if (itChFound == _channelSound.end())
	{
		Debug::error("[setDelay] Channel not found: Couldn't find channel, there isn't an existing channel with this id: " + to_string(chID));
		return false;
	}
	auto res = itChFound->second->getVolume(&volume);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[getVolume] Unexpected Error: Couldn't get the volume of the channel with this id: " + to_string(chID));
		return false;
	}
}

bool AudioModule::isValidChannel(int chID)
{
	return _channelSound.find(chID) != _channelSound.end();
}
