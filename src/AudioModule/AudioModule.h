#pragma once
#include <unordered_map>
#include <string>

namespace FMOD
{
	class System;
	class Sound;
}

class AudioModule
{
public:

	AudioModule();
	~AudioModule();

	bool Init();

	bool addSound(const char* path, std::string id);
	bool addStream(const char* path, std::string id);

	bool playSound(std::string id);

private:
	FMOD::System* _system = nullptr;

	std::unordered_map<std::string, FMOD::Sound*> _soundMap;
};