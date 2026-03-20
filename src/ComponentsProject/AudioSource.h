#pragma once
#include <Component.h>
#include <Vector3.h>
#include <vector>
#include <unordered_set>


class Transform;

class AudioSource: public core::Component
{
private:
	Transform* _tr;

	core::Vector3<float> _lastPosition;

	std::string _path;
	std::string _id;
	bool _mute;
	bool _is3D;
	bool _loop;
	bool _isStream;
	float _soundVolume;
	int _channelID;

public:
	AudioSource();
	~AudioSource();

	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t deltaTime) override;

	void playSound();
	int getLooping() const;
	void setLooping(int loop);
	void setVolume(float& newVolume);
	void setVolume(float&& newVolume);
	void stopSound() const;
	void pauseSound(bool pause) const;
	void setDelay(unsigned long long start, unsigned long long end, bool stopChannel) const;
	std::string getSoundName() const;
};

