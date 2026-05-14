//#pragma once
//#include "Vector3.h"
//
//namespace core
//{
//	class EngineComponent;
//}
//
//class Engine;
//class AudioModule;
///**
// * @brief API publica para audio del motor.
// * Expone solo los metodos seguros para componentes.
// * Para metodos internos, ver AudioModule.
// */
//class AudioFacade
//{
//private:
//	/**
//	 * @brief Clases amigas.
//	 */
//	friend Engine;
//	friend core::EngineComponent;
//	/**
//	 * @brief Referencia al modulo que envuelve.
//	 */
//	AudioModule* _audio = nullptr;
//	AudioFacade(AudioModule* audio);
//
//	//Metodos del modulo de audio
//	/*
//	* @brief Crea un sonido en el modulo de audio.
//	* Recibe un path y un id, ademas de parametros de configuracion, como si es stream (sonido corto) o no (musica), si tiene loop o si es 3D.
//	*/
//	bool _loadSound(std::string path, std::string id, bool soundStream = true, bool soundLooping = false, bool sound3D = true);
//	/*
//	* @brief Descarga un sonido del modulo de audio recibiendo su id.
//	*/
//	bool _unloadSound(std::string id);
//	/*
//	* @brief Reproduce un sonido del modulo de audio recibiendo su id y su configuracion: volumen, loop (si creado con looping: -1 = indef, 0 = one time, 1 = loop once), posicion y velocidad (para audio 3D)
//	*/
//	int _playSound(std::string id, float soundVolume, int looping = 0, const core::Vector3<> pos3 = { 0.0f, 0.0f,0.0f }, const core::Vector3<> vel3 = { 0.0f,0.0f,0.0f });
//	/*
//	* @brief Configura en el modulo de audio el listener de la escena, recibiendo su posicion, forward y up, y adicionalmente la velocidad para el audio 3D (efecto Doppler)
//	*/
//	void _setListener(core::Vector3<> pos, core::Vector3<> forward, core::Vector3<> up, core::Vector3<> vel = { 0.0,0.0,0.0 });
//	/*
//	* @brief Actualiza la posicion y velocidad de un audio 3D
//	*/
//	bool _setSourcePosition(int chID, core::Vector3<> pos, core::Vector3<> vel);
//	/*
//	* @brief Cambia el radio minimo y maximo de difusion de un audio 3D
//	*/
//	bool _setMinMaxRadius(int chID, float min, float max);
//	/*
//	* @brief Actualiza el volumen de un canal
//	*/
//	bool _setChannelVolume(int chID, float newVolume = 0.0f);
//	/*
//	* @brief Devuelve el volumen de un canal
//	*/
//	float _getVolume(int chID);
//	/*
//	* @brief Configura el modo de loopeo de un canal: -1 = indef, 0 = one time, 1 = loop once
//	*/
//	bool _setLooping(int chID, int typeOfLooping);
//	/*
//	* @brief Devuelve la configuracion de loopeo que tiene un audio
//	*/
//	int _getLooping(int chID) const;
//	/*
//	* @brief Detiene y libera un canal
//	*/
//	bool _stopPlaying(int chID);
//	/*
//	* @brief Pausa o reanuda un canal y el audio que reproduce
//	*/
//	bool _pauseChannel(int chID, bool pause);
//	/*
//	* @brief Devuelve si un canal esta pausado
//	*/
//	bool _isPaused(int chID);
//	/*
//	* @brief Configura el milisegundo de inicio y de final del audio que se reproduciran
//	*/
//	bool _setDelay(int chID, double start, double end, bool stopChannel);
//	/*
//	* @brief Devuelve si un canal esta pausado (false) o en reproduccion (true)
//	*/
//	bool _isChannelPlaying(int chID);
//
//public:
//	/*
//	* @brief Pausa todos los canales
//	*/
//	void muteEverything();
//
//	/*
//	* @brief Para y elimina todos los canales
//	*/
//	void stopEverything();
//
//	/*
//	* @brief Reanuda todos los canales
//	*/
//	void unMuteEverything();
//};
//
