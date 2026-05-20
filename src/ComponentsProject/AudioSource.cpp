#include "AudioSource.h"
#include <Debug.h>
#include <Entity.h>
#include <PluginSDK.h>

#include "AudioModule.h"
#include "ResourcesModule.h"

#include "Engine.h"
#include "Transform.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(AudioSource);

AudioSource::AudioSource() : _tr(nullptr), _lastPosition(0.0f, 0.0f, 0.0f), _path(), _id(), _is3D(false), _loop(false),
_isStream(), _playOnReady(), _soundVolume(0.0f), _minRadius(1.0f), _maxRadius(100.0f), _channelID(-1), isPlaying(false)
{
	registerMethod("playSound", [this](const std::vector<std::any>& args) {
		playSound();
		});

	registerMethod("stopSound", [this](const std::vector<std::any>& args) {
		stopSound();
		});

	registerMethod("pauseSound", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			pauseSound(std::any_cast<bool>(args[0]));
		}
		});

	registerMethod("isPaused", [this](const std::vector<std::any>& args) {
		return isPaused();
		});

	registerMethod("setVolume", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setVolume(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("getVolume", [this](const std::vector<std::any>& args) {
		return getVolume();
		});

	registerMethod("setLooping", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setLooping(std::any_cast<int>(args[0]));
		}
		});

	registerMethod("getLooping", [this](const std::vector<std::any>& args) {
		return getLooping();
		});

	registerMethod("setDelay", [this](const std::vector<std::any>& args) {
		if (args.size() >= 3) {
			setDelay(std::any_cast<double>(args[0]), std::any_cast<double>(args[1]), std::any_cast<bool>(args[2]));
		}
		});

	registerMethod("setMinRadius", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setMinRadius(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setMaxRadius", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setMaxRadius(std::any_cast<float>(args[0]));
		}
		});
}

AudioSource::~AudioSource()
{
}

bool AudioSource::init(const Properties& p)
{
	_path = getProperty<std::string>(p, "soundPath");
	if (_path.empty()) {
		Debug::warning("[AudioSource] soundPath vacio");
		return false;
	}
	_is3D = getProperty<bool>(p, "is3D");
	_loop = getProperty<bool>(p, "loop");
	_isStream = getProperty<bool>(p, "isStream");
	_playOnReady = getProperty<bool>(p, "playOnReady");
	_soundVolume = getProperty<float>(p, "soundVolume");
	_minRadius = getProperty<float>(p, "minRadius");
	_maxRadius = getProperty<float>(p, "maxRadius");
	return entity->hasComponent<Transform>();
}

void AudioSource::ready()
{
	if (!entity || !entity->hasComponent<Transform>()) {
		Debug::warning("[AudioSource] Entity sin Transform");
		return;
	}
	_tr = entity->getComponent<Transform>();
	if (!_tr) {
		Debug::warning("[AudioSource] No se pudo obtener Transform");
		return;
	}
	_lastPosition = _tr->getGlobalPosition();

	resources()->getOrLoadAsset(_path);

	// Validar path
	if (_path.empty()) {
		Debug::warning("[AudioSource] soundPath vacío, no se cargará audio");
		return;
	}

	// Carga el modelo en fmod y se guarda una referencia a el
	core::ResourcePtr res = resources()->getOrLoadAsset(_path);
	if (!res || !res->isValid()) {
		Debug::error("[AudioSource] Modelo no encontrado: ", _id);
		return;
	}
	_id = res->getName();
	audio()->configureSound(_id, _isStream, _loop, _is3D);

	if (_playOnReady)
	{
		playSound();
	}

}

void AudioSource::update(uint64_t deltaTime)
{
	// Validar que tenemos lo necesario
	if (!_tr || _id.empty() || _channelID == -1) {
		return;
	}

	float dt = deltaTime / 1000.0f;

	isPlaying = audio()->isChannelPlaying(_channelID);

	if (_tr && _is3D && dt > 0 && isPlaying) {
		auto currentPos = _tr->getGlobalPosition();
		auto velocity = (currentPos - _lastPosition) / dt;

		audio()->setAudioPos(_channelID, currentPos, velocity);

		_lastPosition = currentPos;
	}
	if (!isPlaying) {
		_channelID = -1;
	}
}

void AudioSource::disable()
{
	pauseSound(true);
}
void AudioSource::destroy()
{
	if (isPlaying) stopSound();
}
void AudioSource::enable()
{
	if (isPlaying && audio()->isPaused(_channelID)) pauseSound(false);
}

void AudioSource::playSound()
{
	if (_id.empty()) {
		Debug::warning("[AudioSource] Audio no disponible: ", _path);
		return;
	}

	if (!_tr) {
		Debug::warning("[AudioSource] Transform no disponible para reproducir audio");
		_tr = entity ? entity->getComponent<Transform>() : nullptr;
		if (!_tr && _is3D) {
			Debug::error("[AudioSource] Audio 3D requiere Transform");
			return;
		}
	}

	if (isPlaying && _channelID != -1) {
		stopSound();
	}

	// Reproducir
	int looping = _loop ? -1 : 0;
	core::Vector3<> pos = _tr ? _tr->getGlobalPosition() : core::Vector3<>(0.0f, 0.0f, 0.0f);

	_channelID = audio()->playSound(_id, _soundVolume, looping, pos);
	isPlaying = (_channelID != -1);

	if (!isPlaying) {
		Debug::warning("[AudioSource] No se pudo reproducir: ", _id);
		return;
	}

	// Configurar radio 3D si aplica
	if (_is3D && _channelID != -1) {
		audio()->setMinMaxRadius(_channelID, _minRadius, _maxRadius);
	}
}

int AudioSource::getLooping() const
{
	if (!isPlaying) return false;
	int looping = 0;
	audio()->getLooping(_channelID, &looping);
	return looping;
}

void AudioSource::setLooping(int& loop) const
{
	if (!isPlaying) return;
	audio()->setLooping(_channelID, loop);
}
void AudioSource::setLooping(int&& loop) const
{
	if (!isPlaying) return;
	audio()->setLooping(_channelID, loop);
}

float AudioSource::getVolume() const
{
	if (!isPlaying) return -1.0;
	float volume = 0.0f;
	audio()->getVolume(_channelID, volume);
	return volume;
}

void AudioSource::setVolume(float& newVolume)
{
	if (!isPlaying) return;
	_soundVolume = newVolume;
	audio()->setChannelVolume(_channelID, _soundVolume);
}

void AudioSource::setVolume(float&& newVolume)
{
	if (!isPlaying) return;
	_soundVolume = newVolume;
	audio()->setChannelVolume(_channelID, _soundVolume);
}

void AudioSource::setMinRadius(float& newRadius)
{
	if (!isPlaying) return;
	_minRadius = newRadius;
	audio()->setMinMaxRadius(_channelID, _minRadius, _maxRadius);
}

void AudioSource::setMinRadius(float&& newRadius)
{
	if (!isPlaying) return;
	_minRadius = newRadius;
	audio()->setMinMaxRadius(_channelID, _minRadius, _maxRadius);
}

float AudioSource::getMinRadius() const
{
	return _minRadius;
}

void AudioSource::setMaxRadius(float& newRadius)
{
	if (!isPlaying) return;
	_maxRadius = newRadius;
	audio()->setMinMaxRadius(_channelID, _minRadius, _maxRadius);
}

void AudioSource::setMaxRadius(float&& newRadius)
{
	if (!isPlaying) return;
	_maxRadius = newRadius;
	audio()->setMinMaxRadius(_channelID, _minRadius, _maxRadius);
}

float AudioSource::getMaxRadius() const
{
	return _maxRadius;
}

void AudioSource::stopSound()
{
	if (!isPlaying) return;

	audio()->stopPlaying(_channelID);
	_channelID = -1;
	isPlaying = false;
}

void AudioSource::pauseSound(bool pause) const
{
	if (!isPlaying) return;
	audio()->pauseChannel(_channelID, pause);
}
bool AudioSource::isPaused() const
{
	if (!isPlaying) return false;
	return audio()->isPaused(_channelID);
}

void AudioSource::setDelay(double start, double end, bool stopChannel) const
{
	if (!isPlaying) return;
	audio()->setDelay(_channelID, start, end, stopChannel);
}

const std::string& AudioSource::getSoundName() const
{
	return _id;
}