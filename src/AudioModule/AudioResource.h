#pragma once
#include "Resource.h"

class AudioModule;

class AudioResource : public core::Resource
{
public:
	AudioResource(const std::string& id, const std::string& path, AudioModule* audioModule)
		: Resource(id, path, SOUND), _audioModule(audioModule) {
	}
	bool load();
	bool unLoad();

private:
	AudioModule* _audioModule;
};

