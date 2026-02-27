#pragma once
#include <unordered_map>
#include <string>
#include "Vector3.h"

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}


typedef std::unordered_map<std::string, FMOD::Sound*> SoundMap;
typedef std::unordered_map<int, FMOD::Channel*> ChannelMap;

class AudioModule
{
public:

	AudioModule();
	~AudioModule();

	bool Init();
	void Update();
	void ShutDown();

	/// <summary>
	/// Loads sound via the path of the resource and the name of the sound, both by strings
	/// </summary>
	/// <param name="path">URL/Path of the resource</param>
	/// <param name="id">Name of sound</param>
	/// <param name="sound3D">If the sound is 3D or not</param>
	/// <param name="soundLooping">If the sound loops infinetly or not</param>
	/// <param name="soundStream">If the sound is Stream or CompressedSound</param>
	void loadSound(const char* path, std::string id, bool sound3D = true, bool soundLooping = false, bool soundStream = false);

	void unloadSound(std::string id);

	int playSound(std::string id, const Vector3<> vec3 = { 0.0f,0.0f,0.0f }, float soundVolume = 0.0f);

private:
	FMOD::System* _system = nullptr;

	int _nextChannelID;
	SoundMap _soundMap; //Map of sounds
	ChannelMap _channelSound; //Map of channels
};