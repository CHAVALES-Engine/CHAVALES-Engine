/*
 * @file
 * @brief Componente que gestiona una camara.
 */
#pragma once
#include <Component.h>
#include <Vector3.h>
#include <Color.h>

using particleGenID = uint64_t;

/*
 * +--------------------+
 * | PARTICLE GENERATOR |
 * +--------------------+
 *
 * --- Ejemplo de uso en lua ---
 * ParticleGen = {
 *		FOVy = float,
 *		near clip distance = float,
 *		far clip distance = float,
 *		focal length = float,
 *		background color = Color
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
class ParticleGen : public core::Component
{
	particleGenID _particleGenID;

	std::string _textureName;

public:
	ParticleGen();
	~ParticleGen();

	bool init(const Properties& p) override;
	void ready() override;
	void enable() override;
	void disable() override;

	/*
	* @brief Establecer emision de generador de particulas.
	*/
	void setEmitting(const bool& emitting);
	/*
	* @brief Establecer cantidad total de particulas del generador.
	*/
	void setQuota(const float& quota);
	/*
	* @brief Establecer ratio de emision de generador de particulas.
	*/
	void setEmissionRate(const float& rate);
	/*
	* @brief Establecer tiempo de emision del generador de particulas.
	*/
	void setDuration(const float& duration);
	/*
	* @brief Establecer tiempo de vida de particulas del generador.
	*/
	void setTimeToLive(const float& time);
	/*
	* @brief Establecer velocidad de particulas del generador.
	*/
	void setVelocity(const float& velocity);
	/*
	* @brief Establecer velocidad minima de particulas del generador.
	*/
	void setMinVelocity(const float& velocity);
	/*
	* @brief Establecer velocidad maxima de particulas del generador.
	*/
	void setMaxVelocity(const float& velocity);
	/*
	* @brief Establecer direccion de particulas del generador.
	*/
	void setDirection(const core::Vector3<float>& direction);
	/*
	* @brief Establecer angulo de dipsersion de particulas del generador.
	*/
	void setAngle(const float& angle);
	/*
	* @brief Establecer ancho de particulas del generador.
	*/
	void setPartWidth(const float& width);
	/*
	* @brief Establecer alto de particulas del generador.
	*/
	void setPartHeight(const float& height);
	/*
	* @brief Establecer color de particulas del generador.
	*/
	void setPartColor(const core::Color& color);
};
