//#include "AudioFacade.h"
//#include "AudioModule.h"
//#include "Entity.h"
//
//AudioFacade::AudioFacade(AudioModule* audio):_audio(audio)
//{}
//
//bool AudioFacade::_loadSound(std::string path, std::string id, bool soundStream, bool soundLooping, bool sound3D)
//{
//	return _audio->loadSound(path, id, soundStream, soundLooping, sound3D);
//}
//bool AudioFacade::_unloadSound(std::string id)
//{
//	return _audio->unloadSound(id);
//}
//int AudioFacade::_playSound(std::string id, float soundVolume, int looping, const core::Vector3<> vec3, const core::Vector3<> vel3)
//{
//	return _audio->playSound(id, soundVolume, looping, vec3, vel3);
//}
//bool AudioFacade::_setChannelVolume(int chID, float newVolume)
//{
//	return _audio->setChannelVolume(chID, newVolume);
//}
//int AudioFacade::_getLooping(int chID) const
//{
//	int looping = 0;
//	_audio->getLooping(chID, &looping);
//	return looping;
//}
//void AudioFacade::_setListener(core::Vector3<> pos, core::Vector3<> forward, core::Vector3<> up, core::Vector3<> vel)
//{
//	_audio->setListener(pos, forward, up, vel);
//}
//bool AudioFacade::_stopPlaying(int chID)
//{
//	return _audio->stopPlaying(chID);
//}
//bool AudioFacade::_pauseChannel(int chID, bool pause)
//{
//	return _audio->pauseChannel(chID, pause);
//}
//bool AudioFacade::_isPaused(int chID)
//{
//	return _audio->isPaused(chID);
//}
//bool AudioFacade::_setSourcePosition(int chID, core::Vector3<> pos, core::Vector3<> vel)
//{
//	return _audio->setAudioPos(chID, pos, vel);
//}
//bool AudioFacade::_setMinMaxRadius(int chID, float min, float max)
//{
//	return _audio->setMinMaxRadius(chID, min, max);
//}
//
//bool AudioFacade::_setDelay(int chID, double start, double end, bool stopChannel)
//{
//	return _audio->setDelay(chID, start, end, stopChannel);
//}
//
//bool AudioFacade::_isChannelPlaying(int chID)
//{
//	return _audio->isChannelPlaying(chID);
//}
//
//void  AudioFacade::muteEverything()
//{
//	_audio->muteEverything();
//}
//void  AudioFacade::stopEverything()
//{
//	_audio->stopEverything();
//}
//void  AudioFacade::unMuteEverything()
//{
//	_audio->unMuteEverything();
//}
//
//bool AudioFacade::_setLooping(int chID, int typeOfLooping)
//{
//	return _audio->setLooping(chID, typeOfLooping);
//}
//
//float AudioFacade::_getVolume(int chID)
//{
//	float volume;
//	_audio->getVolume(chID, volume);
//	return volume;
//}
