#include "AudioModule.h"

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

bool AudioModule::addSound(const char* path, std::string id)
{
	if (_soundMap.count(id))
	{
		//throw("Sound cretion error: Couldn't create sound, there is already a sound with this id: " + id);
		return false;
	}

	FMOD_RESULT result;
	//Creates an FMOD System
	FMOD::Sound* sound;
	result = _system->createSound(path, FMOD_NONBLOCKING, 0, &sound);
	if (result != FMOD_OK)
	{
		//throw("FMOD error: Couldn't locate sound");
		return false;
	}
	_soundMap.insert({ id, sound });
	return true;
}

bool AudioModule::addStream(const char* path, std::string id)
{
	if (_soundMap.count(id))
	{
		//throw("Sound cretion error: Couldn't create sound, there is already a sound with this id: " + id);
		return false;
	}

	FMOD_RESULT result;
	//Creates an FMOD System
	FMOD::Sound* sound;
	result = _system->createStream(path, FMOD_NONBLOCKING, 0, &sound);
	if (result != FMOD_OK)
	{
		//throw("FMOD error: Couldn't locate stream");
		return false;
	}
	_soundMap.insert({ id, sound });
	return true;
}

bool AudioModule::playSound(std::string id)
{
	return false;
}
