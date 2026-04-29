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
 *		soundID = string,
 *		is3D = bool,
 *		loop = bool,
 *		isStream = bool,
 *		playOnReady = bool,
 *		soundVolume = float,
 *		minRadius = float,
 *		maxRadius = float
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

class ENGINE_API AudioSource : public core::Component
{
private:
	/*
	* @brief Referencia al Tranform de la entidad para el audio 3D
	*/
	std::shared_ptr<Transform> _tr;
	/*
	* @brief Vector que guarda la última posición regristrada para el cálculo de velocidad
	*/
	core::Vector3<float> _lastPosition;
	/*
	* @brief Identificador del audio para la carga del recurso
	*/
	std::string _id;
	/*
	* @brief Bool que configura si el audio es 3D (true) o 2D (false)
	*/
	bool _is3D;
	/*
	* @brief Bool que configura si el audio se repite infinitamente (true) o se reproduce solo una vez (false)
	*/
	bool _loop;
	/*
	* @brief Bool que configura si el audio es Stream, conigurado para audios largos (true) o nom, pensado para audios cortos o efectos de sonido (false)
	*/
	bool _isStream;
	/*
	* @brief Bool que configura si el audio empieza a sonar al activarse por primera vez la entidad en Ready (true) o no (false)
	*/
	bool _playOnReady;
	/*
	* @brief Float que determina el volumen del sonido
	*/
	float _soundVolume;
	/*
	* @brief Para el audio 3D, radio de la esfera en la que se escucha el audio al máximo volumen
	*/
	float _minRadius;
	/*
	* @brief Para el audio 3D, radio de la esfera en la que se escucha el audio.
		Entre el radio minimo y el maximo el audio se disipa linealmente. Más alla del máximo el audio no se escucha
	*/
	float _maxRadius;
	/*
	* @brief Identificador del canal que reproduce el sonido
	*/
	int _channelID;

public:
	AudioSource();
	~AudioSource();

	bool init(const Properties& p) override;
	/*
	* @brief Se carga el sonido con los parámetros configurados y, si tiene activado _playOnReady empieza a reproducirlo
	*/
	virtual void ready() override;
	/*
	* @brief En cada frame se actualiza la posición del Source y su velocidad si este es 3D
	*/
	virtual void update(uint64_t deltaTime) override;
	/*
	* @brief Al desactivar el componente se pausa el audio
	*/
	virtual void disable() override;
	/*
	* @brief Al destruir el componente se detiene el audio
	*/
	virtual void destroy() override;
	/*
	* @brief Al activar el componente se reanuda el audio
	*/
	virtual void enable() override;

	/*
	* @brief Configura el modo loop del audio (-1 = indef, 0 = one time, 1 = loop once)
	*/
	void setLooping(int& loop) const;
	void setLooping(int&& loop) const;
	/*
	* @brief Devuelve el modo actual del loop del audio
	*/
	int getLooping() const;
	/*
	* @brief Configura el volumen del audio
	*/
	void setVolume(float& newVolume);
	void setVolume(float&& newVolume);
	/*
	* @brief Devuelve el volumen del audio
	*/
	float getVolume() const;
	/*
	* @brief Reprouce el sonido en un nuevo canal
	*/
	void playSound();
	/*
	* @brief Detiene por completo el sonido
	*/
	void stopSound() const;
	/*
	* @brief Pausea (true) o reanuda (false) el canal
	*/
	void pauseSound(bool pause) const;
	/*
	* @brief Establece, en milisegundos, el principio y el fin que se reproduce del audio
	* stopChannel sirve para indicar si se quiere detener el audio al llegar a end
	*/
	void setDelay(double start, double end, bool stopChannel) const;
	/*
	* @brief Devuelve el ID del sonido
	*/
	std::string getSoundName() const;
	/*
	* @brief En 3D, configura el radio Minimo del la esfera de sonido
	*/
	void setMinRadius(float& newRadius);
	void setMinRadius(float&& newRadius);
	/*
	* @brief En 3D, devuelve el radio minimo del sonido
	*/
	float getMinRadius() const;
	/*
	* @brief En 3D, configura el radio Maxino del la esfera de sonido
	*/
	void setMaxRadius(float& newRadius);
	void setMaxRadius(float&& newRadius);
	/*
	* @brief En 3D, devuelve el radio maximo del sonido
	*/
	float getMaxRadius() const;
private:

};

