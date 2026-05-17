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
		Debug::error("[AudioModule]: No se ha creado sistema");
		return false;
	}
	//If Creation went OK, the system is initialized
	result = _system->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		Debug::error("[AudioModule]: No se ha inicializado sistema");
		if (_system)
		{
			_system->release();
			_system = nullptr;
		}
		return false;
	}
	_system->set3DSettings(1.0f, 1.0f, 1.0f);
	_system->getSoftwareFormat(&nativeRate, 0, 0);
	return true;
}

void AudioModule::update()
{
	_system->update();
	for (auto it = _channelSound.begin(); it != _channelSound.end(); )
	{
		bool playing = false;
		it->second->isPlaying(&playing);

		if (!playing)
			it = _channelSound.erase(it);
		else
			++it;
	}
}

void AudioModule::shutdown()
{
	//Destructora
	if (_system == nullptr)
		return;

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

bool AudioModule::loadSound(const string& path, const string& id, bool soundStream, bool soundLooping, bool sound3D)
{
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound == _soundMap.end())
	{
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
			Debug::error("[AudioModule]: No se ha encontrado audio");
			return false;
		}
	}
	return true;
}

bool AudioModule::unloadSound(const std::string& id)
{
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound == _soundMap.end())
	{
		Debug::error("[AudioModule]: No se ha encontrado audio ", id);
		return false;
	}
	itSoundFound->second->release();
	_soundMap.erase(itSoundFound);
	return true;
}

int AudioModule::playSound(const std::string& id, float soundVolume, int looping, const core::Vector3<>& pos3, const core::Vector3<>& vel3)
{
	int nextChID = _nextChannelID++;
	auto itSoundFound = _soundMap.find(id);
	if (itSoundFound == _soundMap.end())
	{
		Debug::error("[AudioModule]: No se ha encontrado audio ", id);
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
		channel->setLoopCount(looping);
		_channelSound[nextChID] = channel;
	}
	else 
	{
		Debug::error("[AudioModule]: El audio no se ha generado correctamente");
		return -1;
	}
	return nextChID;
}

bool AudioModule::setChannelVolume(int chID, float newVolume)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
	auto res = itChFound->second->setVolume(newVolume);
	if(res == FMOD_OK) return true;
	else {
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
}
bool AudioModule::getLooping(int chID, int* typeOfLooping)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
	auto res = itChFound->second->getLoopCount(typeOfLooping);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[AudioModule]: Configuracion de loop del canal no encontrada, ID: ", to_string(chID));
		return false;
	}
}

bool AudioModule::setLooping(int chID, int typeOfLooping)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
	auto res = itChFound->second->setLoopCount(typeOfLooping);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[AudioModule]: No se pudo configurar el loop del canal con ID ", to_string(chID));
		return false;
	}
}

bool AudioModule::stopPlaying(int chID)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
	auto res = itChFound->second->stop();
	_channelSound.erase(itChFound);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[AudioModule]: No se pudo parar el canal con ID: ", to_string(chID));
		return false;
	}
}

bool AudioModule::pauseChannel(int chID, bool pause)
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
	auto res =  itChFound->second->setPaused(pause);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[AudioModule]: No se pudo pausar/reanudar el canal con ID: ", to_string(chID));
		return false;
	}
}

bool AudioModule::isPaused(int chID) const
{
	auto itChFound = _channelSound.find(chID);
	if (itChFound == _channelSound.end())
	{
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
	bool paused = true;
	auto res = itChFound->second->getPaused(&paused);
	if (res == FMOD_OK) return paused;
	else {
		Debug::error("[AudioModule]: Configuracion de pausa del canal no encontrada, ID: ", to_string(chID));
		return false;
	}
}

void AudioModule::setListener(const core::Vector3<>& pos, const core::Vector3<>& forward, const core::Vector3<>& up, const core::Vector3<>& vel)
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
		it->second->getPaused(&isPlaying);
		if (isPlaying) {
			it->second->setPaused(true);
		}
	}
}
void AudioModule::stopEverything()
{
	for (auto it = _channelSound.begin(); it != _channelSound.end(); ++it) {
		stopPlaying(it->first);
	}
	_nextChannelID = 0;
}

void AudioModule::unMuteEverything()
{
	for (auto it = _channelSound.begin(); it != _channelSound.end(); ++it) {
		bool isPlaying = false;
		it->second->getPaused(&isPlaying);
		if (isPlaying) {
			it->second->setPaused(false);
		}
	}
}

bool AudioModule::setAudioPos(int chID, const core::Vector3<>& pos, const core::Vector3<>& vel)
{
	auto itCH = _channelSound.find(chID);
	if (itCH == _channelSound.end())
	{
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
	FMOD_VECTOR position = { pos.getX(),pos.getY(),pos.getZ() };
	FMOD_VECTOR velocity = { vel.getX(),vel.getY(),vel.getZ() };
	auto res = itCH->second->set3DAttributes(&position, &velocity);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[AudioModule]: La configuracion de position/velocity no se ha podido establecer en el audio del canal ", to_string(chID));
		return false;
	}
}

bool AudioModule::setMinMaxRadius(int chID, float min, float max)
{
	auto itCH = _channelSound.find(chID);
	if (itCH == _channelSound.end())
	{
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
	itCH->second->set3DMinMaxDistance(min, max);
	FMOD::Sound* sound = nullptr;
	auto res = itCH->second->getCurrentSound(&sound);
	if (sound != nullptr)
		sound->set3DMinMaxDistance(min, max);
	else
	{
		Debug::warning("[AudioModule] No hay audio asociado al canal con ID : " + to_string(chID) + ". El los parametros Min/Max del radio se pueden sobreescribir con los parametros por defecto (1,10000)");
		return false;
	}

	if (res == FMOD_OK) return true;
	else
	{
		Debug::error("[AudioModule]: La configuracion de parametros Min/Max del radio no se han podido establecer en el canal ", to_string(chID));
		return false;
	}
}

bool AudioModule::isChannelPlaying(int chID) const
{
	auto itChFound = _channelSound.find(chID);

	if (itChFound == _channelSound.end())
	{
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
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
	auto res = itChFound->second->setDelay(sampleStart, sampleEnd, stopChannel);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[AudioModule]: La configuracion de delay no se ha podido establecer en el canal ", to_string(chID));
		return false;
	}
}

bool AudioModule::getVolume(int chID, float& volume) const
{
	auto itChFound = _channelSound.find(chID);

	if (itChFound == _channelSound.end())
	{
		Debug::error("[AudioModule]: Canal no encontrado, ID: ", to_string(chID));
		return false;
	}
	auto res = itChFound->second->getVolume(&volume);
	if (res == FMOD_OK) return true;
	else {
		Debug::error("[AudioModule]: Configuracion de volumen del canal no encontrada, ID: ", to_string(chID));
		return false;
	}
}

bool AudioModule::isValidChannel(int chID) const
{
	return _channelSound.find(chID) != _channelSound.end();
}
