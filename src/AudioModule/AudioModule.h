/**
* @file
*/

#pragma once
#include <unordered_map>
#include <string>
#include "Vector3.h"
#include "EngineAPI.h"

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

typedef std::unordered_map<std::string, FMOD::Sound*> SoundMap;
typedef std::unordered_map<int, FMOD::Channel*> ChannelMap;

class ENGINE_API AudioModule
{
public:
	AudioModule();
	~AudioModule();

	bool init();
	void update();
	void shutdown();

	/**
	 * @brief Carga un audio.
	 * @param path - Ruta del audio.
	 * @param id - ID del audio.
	 * @param soundStream - 
	 * @param soundLooping - Si debe ciclar infinitamente.
	 * @param sound3D - Si el audio es 3D.
	 */
	bool loadSound(const std::string& path, const std::string& id, bool soundStream = true, bool soundLooping = false, bool sound3D = true);

	/**
	 * @brief Retira un audio.
	 * @param id - ID del audio.
	 */
	bool unloadSound(const std::string& id);

	/**
	 * @brief Busca un sonido, si lo encuentra lo asocia a un canal. Luego comienza a reproducirlo. El metodo devuelve el canal.
	 * @param id - ID del audio.
	 * @param soundVolume -
	 * @param looping - 
	 * @param pos3 -
	 * @param vel3 -
	 */
	int playSound(const std::string& id, float soundVolume, int looping = 0, const core::Vector3<>& pos3 = { 0.0f, 0.0f,0.0f }, const core::Vector3<>& vel3 = { 0.0f,0.0f,0.0f });

	/**
	 * @brief Metodo para establecer un volumen a un canal.
	 * @param chID - ID del canal.
	 * @param newVolume - Volumen.
	 */
	bool setChannelVolume(int chID, float newVolume = 0.0f);

	/**
	 * @brief Devuelve si el canal se esta reproduciendo en bucle o no 
	 * (0: no se reproduce en bucle, 1: se reproduce solo una vez, -1: se reproduce en bucle indefinidamente).
	 * @param chID - ID del canal.
	 * @param typeOfLooping - Tipo de loop.
	 */
	bool getLooping(int chID, int* typeOfLooping);

	/**
	 * @brief Establece el tipo de loop de un canal
	 * (0: no se reproduce en bucle, 1: se reproduce solo una vez, -1: se reproduce en bucle indefinidamente).
	 * @param chID - ID del canal.
	 * @param typeOfLooping - Tipo de loop.
	 */
	bool setLooping(int chID, int typeOfLooping);

	/**
	 * @brief Para el audio de un canal
	 * @param chID - ID del canal.
	 */
	bool stopPlaying(int chID);

	/**
	 * @brief Pausa el audio de un canal
	 * @param chID - ID del canal.
	 * @param pause - 
	 */
	bool pauseChannel(int chID, bool pause);

	/**
	 * @brief Si el canal esta pausado.
	 * @param chID - ID del canal.
	 */
	bool isPaused(int chID) const;

	/**
	 * @brief Actualiza los parametros del audio listener de FMOD.
	 * @param pos - posicion del listener.
	 * @param forward - vector forward del listener.
	 * @param up - vector up del listener.
	 * @param vel - velocidad del audio listener, para el efecto Doppler.
	 */
	void setListener(const core::Vector3<>& pos, const core::Vector3<>& forward, const core::Vector3<>& up, const core::Vector3<>& vel = { 0.0,0.0,0.0 });

	/**
	 * @brief Silencia todos los canales.
	 */
	void muteEverything();

	/**
	 * @brief Para todos los canales.
	 */
	void stopEverything();

	/**
	 * @brief Desilencia todos los canales.
	 */
	void unMuteEverything();

	/**
	 * @brief Establece la posicion del sonido.
	 * @param chID - ID del canal.
	 * @param pos - nueva posicion del sonido.
	 * @param vel - nueva velocidad de propagacion del sonido.
	 */
	bool setAudioPos(int chID, const core::Vector3<>& pos, const core::Vector3<>& vel);

	/**
	 * @brief Metodo para determinar el radio mínimo y máximo de un audio 3D.
	 * @param chID - ID del canal.
	 * @param min -
	 * @param max - 
	 */
	bool setMinMaxRadius(int chID, float min, float max);

	/**
	 * @brief Metodo para comprobar si el canal se esta reproduciendo. Devuelve falso solo si se ha detenido.
	 * @param chID - ID del canal.
	 */
	bool isChannelPlaying(int chID) const;

	/**
	 * @brief Establece un tiempo de inicio (y/o parada)
	 * @param chID - ID del canal.
	 * @param start - Parametro en el que empezar a reproducir sonido.
	 * @param end - Parametro en el que dejar de reproducir sonido.
	 * @param stopChannel - 
	 *  True: When dspclock_end is reached, behaves like ChannelControl::stop has been called.
	 *	False: When dspclock_end is reached, behaves like ChannelControl::setPaused has been called.
	 */
	bool setDelay(int chID, double start, double end, bool stopChannel);

	/**
	 * @brief Obtiene el volumen del canal.
	 * @param chID - ID del canal.
	 * @param volume - volumen.
	 */
	bool getVolume(int chID, float& volume) const;

	/**
	 * @brief Comprueba validez del canal.
	 * @param chID - ID del canal.
	 */
	bool isValidChannel(int chID) const;

private:
	FMOD::System* _system = nullptr;
	int nativeRate; // Frecuencia de actualizacion de audio

	int _nextChannelID;
	SoundMap _soundMap; // mapa de audios
	ChannelMap _channelSound; // mapa de canales
};