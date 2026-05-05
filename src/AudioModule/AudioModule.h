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

	bool init();
	void update();
	void shutdown();

	/// <summary>
	/// Loads sound via the path of the resource and the name of the sound, both by strings
	/// </summary>
	/// <param name="path">URL/Path of the resource</param>
	/// <param name="id">Name of sound</param>
	/// <param name="sound3D">If the sound is 3D or not</param>
	/// <param name="soundLooping">If the sound loops infinetly or not</param>
	/// <param name="soundStream">If the sound is Stream or CompressedSound</param>
	bool loadSound(std::string path, std::string id, bool soundStream = true, bool soundLooping = false, bool sound3D = true);

	/// <summary>
	/// If the sound isn´t playing anymore it unloads it
	/// </summary>
	/// <param name="id">Id of the sound</param>
	bool unloadSound(std::string id);

	/// <summary>
	/// It searches for a sound, if it is found the sound is associated to a channel. It then starts playing.
	/// The method returns the channel.
	/// </summary>
	/// <param name="id">Id of the sound to play</param>
	/// <param name="vec3">Position of the sound that is going to be played</param>
	/// <param name="soundVolume">Volume of the sound</param>
	/// <returns></returns>
	int playSound(std::string id, float soundVolume, int looping = 0, const core::Vector3<> pos3 = { 0.0f, 0.0f,0.0f }, const core::Vector3<> vel3 = { 0.0f,0.0f,0.0f });

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

	/// <summary>
	/// Method to set the loop configuration of a channel. 
	/// </summary>
	/// <param name="chID"></param>
	/// <param name="typeOfLooping">(0: no loop, 1: only plays one time, -1: loops infinently)</param>
	/// <returns></returns>
	bool setLooping(int chID, int typeOfLooping);

	/// <summary>
	/// Stops the disire sound
	/// </summary>
	/// <param name="chID">ID of the sound we are looking for</param>
	/// <returns></returns>
	bool stopPlaying(int chID);

	/// <summary>
	/// Method to pause a sound
	/// </summary>
	/// <param name="chID">ID of the sound we are looking</param>
	/// <param name="pause">True: it is paused, Flase: it is active</param>
	/// <returns>Return if it has been paused</returns>
	bool pauseChannel(int chID, bool pause);

	/// <summary>
	/// Updates de parameters of FMOD audio listener
	/// </summary>
	/// <param name="pos">Position of the Listener</param>
	/// <param name="forward">Forward vector of the Listener</param>
	/// <param name="up">Up vector of the Listenter</param>
	/// <param name="vel">(optional) Velocity of the Listener, for Doppler effect</param>
	void setListener(core::Vector3<> pos, core::Vector3<> forward, core::Vector3<> up, core::Vector3<> vel = { 0.0,0.0,0.0 });

	/// <summary>
	/// Method to mute all chanels
	/// </summary>
	void muteEverything();

	/// <summary>
	/// Method to stop all chanels
	/// </summary>
	void stopEverything();

	/// <summary>
	/// Method to unmute all chanels
	/// </summary>
	void unMuteEverything();

	/// <summary>
	/// Method to set the position of the desire sound
	/// </summary>
	/// <param name="chID">Id of the desire sound</param>
	/// <param name="pos">New position of the sound</param>
	/// <param name="vel">New velocity of propagation of the sound</param>
	bool setAudioPos(int chID, core::Vector3<> pos, core::Vector3<> vel);

	/// <summary>
	/// Method to se the minimum and maximus radius of a 3D audio
	/// </summary>
	/// <param name="chID"></param>
	/// <param name="min"></param>
	/// <param name="max"></param>
	bool setMinMaxRadius(int chID, float min, float max);

	/// <summary>
	/// Method to observe if the channel is playing 
	/// </summary>
	/// <param name="chID">Id of the desire channel</param>
	/// <returns></returns>
	bool isChannelPlaying(int chID);

	/// <summary>
	/// Sets a sample accurate start (and/or stop) time relative
	/// </summary>
	/// <param name="chID">Id of the desire channel</param>
	/// <param name="start">Parameter to start playing sound at</param>
	/// <param name="end">Parameter to end playing sound at</param>
	/// <param name="stopChannel">
	/// True: When dspclock_end is reached, behaves like ChannelControl::stop has been called.
	///	False: When dspclock_end is reached, behaves like ChannelControl::setPaused has been called.
	/// </param>
	bool setDelay(int chID, double start, double end, bool stopChannel);

	bool getVolume(int chID, float& volume);

	bool isValidChannel(int chID);

private:
	FMOD::System* _system = nullptr;
	int nativeRate; //Rate of audio refresh, taken from device

	int _nextChannelID;
	SoundMap _soundMap; //Map of sounds
	ChannelMap _channelSound; //Map of channels
};