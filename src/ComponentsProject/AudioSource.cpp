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

AudioSource::AudioSource() : tr(nullptr), lastPosition(0.0f, 0.0f, 0.0f), id(), channelID(-1), isPlaying(false)
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

//bool AudioSource::init(const Properties& p)
//{
//	path = getProperty<std::string>(p, "soundPath");
//	is3D = getProperty<bool>(p, "is3D");
//	loop = getProperty<bool>(p, "loop");
//	isStream = getProperty<bool>(p, "isStream");
//	playOnReady = getProperty<bool>(p, "playOnReady");
//	soundVolume = getProperty<float>(p, "soundVolume");
//	minRadius = getProperty<float>(p, "minRadius");
//	maxRadius = getProperty<float>(p, "maxRadius");
//	return entity->hasComponent<Transform>();
//}

void AudioSource::ready()
{
	if (!entity || !entity->hasComponent<Transform>()) {
		Debug::warning("[AudioSource] Entity sin Transform");
		return;
	}
	tr = entity->getComponent<Transform>();
	if (!tr) {
		Debug::warning("[AudioSource] No se pudo obtener Transform");
		return;
	}
	lastPosition = tr->getGlobalPosition();

	// Validar path
	if (path->empty()) {
		Debug::warning("[AudioSource] soundPath vacío, no se cargará audio");
		return;
	}

	// Carga el modelo en fmod y se guarda una referencia a el
	core::ResourcePtr res = resources()->getOrLoadAsset(path);
	if (!res || !res->isValid()) {
		Debug::error("[AudioSource] Audio no encontrado: ", id);
		return;
	}
	id = res->getName();
	audio()->configureSound(id, isStream, loop, is3D);

	if (playOnReady)
	{
		playSound();
	}

}

void AudioSource::update(uint64_t deltaTime)
{
	// Validar que tenemos lo necesario
	if (!tr || id.empty() || channelID == -1) {
		return;
	}

	float dt = deltaTime / 1000.0f;

	isPlaying = audio()->isChannelPlaying(channelID);

	if (tr && is3D && dt > 0 && isPlaying) {
		auto currentPos = tr->getGlobalPosition();
		auto velocity = (currentPos - lastPosition) / dt;

		audio()->setAudioPos(channelID, currentPos, velocity);

		lastPosition = currentPos;
	}
	if (!isPlaying) {
		channelID = -1;
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
	if (isPlaying && audio()->isPaused(channelID)) pauseSound(false);
}

void AudioSource::playSound()
{
	if (id.empty()) {
		Debug::warning("[AudioSource] Audio no disponible: ", path);
		return;
	}

	if (!tr) {
		Debug::warning("[AudioSource] Transform no disponible para reproducir audio");
		tr = entity ? entity->getComponent<Transform>() : nullptr;
		if (!tr && is3D) {
			Debug::error("[AudioSource] Audio 3D requiere Transform");
			return;
		}
	}

	if (isPlaying && channelID != -1) {
		stopSound();
	}

	// Reproducir
	int looping = loop ? -1 : 0;
	core::Vector3<> pos = tr ? tr->getGlobalPosition() : core::Vector3<>(0.0f, 0.0f, 0.0f);

	channelID = audio()->playSound(id, soundVolume, looping, pos);
	isPlaying = (channelID != -1);

	if (!isPlaying) {
		Debug::warning("[AudioSource] No se pudo reproducir: ", id);
		return;
	}

	// Configurar radio 3D si aplica
	if (is3D && channelID != -1) {
		audio()->setMinMaxRadius(channelID, minRadius, maxRadius);
	}
}

int AudioSource::getLooping() const
{
	if (!isPlaying) return false;
	int looping = 0;
	audio()->getLooping(channelID, &looping);
	return looping;
}

void AudioSource::setLooping(int& loop) const
{
	if (!isPlaying) return;
	audio()->setLooping(channelID, loop);
}
void AudioSource::setLooping(int&& loop) const
{
	if (!isPlaying) return;
	audio()->setLooping(channelID, loop);
}

float AudioSource::getVolume() const
{
	if (!isPlaying) return -1.0;
	float volume = 0.0f;
	audio()->getVolume(channelID, volume);
	return volume;
}

void AudioSource::setVolume(float& newVolume)
{
	if (!isPlaying) return;
	soundVolume = newVolume;
	audio()->setChannelVolume(channelID, soundVolume);
}

void AudioSource::setVolume(float&& newVolume)
{
	if (!isPlaying) return;
	soundVolume = newVolume;
	audio()->setChannelVolume(channelID, soundVolume);
}

void AudioSource::setMinRadius(float& newRadius)
{
	if (!isPlaying) return;
	minRadius = newRadius;
	audio()->setMinMaxRadius(channelID, minRadius, maxRadius);
}

void AudioSource::setMinRadius(float&& newRadius)
{
	if (!isPlaying) return;
	minRadius = newRadius;
	audio()->setMinMaxRadius(channelID, minRadius, maxRadius);
}

float AudioSource::getMinRadius() const
{
	return minRadius;
}

void AudioSource::setMaxRadius(float& newRadius)
{
	if (!isPlaying) return;
	maxRadius = newRadius;
	audio()->setMinMaxRadius(channelID, minRadius, maxRadius);
}

void AudioSource::setMaxRadius(float&& newRadius)
{
	if (!isPlaying) return;
	maxRadius = newRadius;
	audio()->setMinMaxRadius(channelID, minRadius, maxRadius);
}

float AudioSource::getMaxRadius() const
{
	return maxRadius;
}

void AudioSource::stopSound()
{
	if (!isPlaying) return;

	audio()->stopPlaying(channelID);
	channelID = -1;
	isPlaying = false;
}

void AudioSource::pauseSound(bool pause) const
{
	if (!isPlaying) return;
	audio()->pauseChannel(channelID, pause);
}
bool AudioSource::isPaused() const
{
	if (!isPlaying) return false;
	return audio()->isPaused(channelID);
}

void AudioSource::setDelay(double start, double end, bool stopChannel) const
{
	if (!isPlaying) return;
	audio()->setDelay(channelID, start, end, stopChannel);
}

const std::string& AudioSource::getSoundName() const
{
	return id;
}