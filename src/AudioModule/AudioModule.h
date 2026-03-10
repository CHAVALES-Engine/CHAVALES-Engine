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
	bool loadSound(const char* path, std::string id, bool sound3D = true, bool soundLooping = false, bool soundStream = false);

	/// <summary>
	/// If the sound isn´t playing anymore it unloads it
	/// </summary>
	/// <param name="id">Id of the sound</param>
	bool unloadSound(std::string id);

	/// <summary>
	/// It searches for a sound, if it is found yhe sound is associated to a channel,then it plays
	/// </summary>
	/// <param name="id">Id of the sound to play</param>
	/// <param name="vec3">Position of the sound that is going to be played</param>
	/// <param name="soundVolume">Volume of the sound</param>
	/// <returns></returns>
	int playSound(std::string id, const core::Vector3<> vec3 = { 0.0f,0.0f,0.0f }, float soundVolume = 0.0f, int looping = 0);

	/// <summary>
	/// Method to set a Volume, the ID is of the channel of the sound that is going to be found
	/// </summary>
	/// <param name="chID">ID of the sound we are looking for</param>
	/// <param name="newVolume">New volume the sound is going to get</param>
	bool setChannelVolume(int chID, float newVolume = 0.0f);

	/// <summary>
	/// Return if the channels is looping or not(0: no loop, 1: only plays one time, -1: loops infinently)
	/// </summary>
	/// <param name="chID">ID of the sound we are looking for</param>
	/// <param name="typeOfLooping">Retrives the type of looping</param>
	bool getLooping(int chID, int* typeOfLooping);

	bool stopPlaying(int chID);

	/// <summary>
	/// Method to pause a sound
	/// </summary>
	/// <param name="chID">ID of the sound we are looking</param>
	/// <param name="pause">True: it is paused, Flase: it is active</param>
	/// <returns>Return if it has been paused</returns>
	bool pauseChannel(int chID, bool pause);

	/// <summary>
	/// Method to mute all chanels
	/// </summary>
	void muteEverything();

	/// <summary>
	/// Method to unmute all chanels
	/// </summary>
	void unMuteEverything();

private:
	FMOD::System* _system = nullptr;

	int _nextChannelID;
	SoundMap _soundMap; //Map of sounds
	ChannelMap _channelSound; //Map of channels
};