#pragma once
#include <Component.h>
#include <Vector3.h>

class Engine;
class AudioSource: public core::Component
{
private:
	core::Vector3<> _position;
	Engine* _eng;
public:
	AudioSource(Engine* eng);
	~AudioSource() override;

	bool init(const Properties& p) override;

	void loadSound(const char* path, std::string id, bool sound3D = true, bool soundLooping = false, bool soundStream = false);
	void unloadSound(std::string id);
	void playSound(std::string id, const core::Vector3<> vec3 = { 0.0f,0.0f,0.0f }, float soundVolume = 0.0f, int looping = 0);
	void setChannelVolume(int chID, float newVolume = 0.0f);
};

