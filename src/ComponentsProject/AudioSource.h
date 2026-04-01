#pragma once
#include <Component.h>
#include <Vector3.h>
#include <vector>
#include <unordered_set>
#include <EngineAPI.h>


class Transform;

/*
 * +--------------+
 * | AUDIO SOURCE |
 * +--------------+
 *
 * --- Ejemplo de uso en lua ---
 * AudioSource = {
 *		soundPath = string,
 *		soundID = int,
 *		mute = bool,
 *		is3D = bool,
 *		loop = bool,
 *		isStream = bool,
 *		soundVolume = float
 * }
 *
 * --- Ejemplo de inicializacion ---
 * En bool init(const Properties& p):
 *		# Ej1, asignacion:
 * component = getProperty<tipo>(properties, "atributo1");
 *		# Ej2, setter:
 * return setProperty(properties, "atributo1", component);
 *
*/

class ENGINE_API AudioSource: public core::Component
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
	bool _playOnReady;
	float _soundVolume;
	int _channelID;

public:
	AudioSource();
	~AudioSource();

	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t deltaTime) override;

	int getLooping() const;
	void setLooping(int& loop) const;
	void setLooping(int&& loop) const;
	float getVolume() const;
	void setVolume(float& newVolume);
	void setVolume(float&& newVolume);
	void stopSound() const;
	void pauseSound(bool pause) const;
	void setDelay(double start, double end, bool stopChannel) const;
	std::string getSoundName() const;
	void playSound();
private:

};

