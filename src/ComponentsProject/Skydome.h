/*
 * @file
 * @brief Componente que gestiona un skydome.
 */
#pragma once
#include "EngineComponent.h"
#include <EngineAPI.h>


using particleGenID = uint64_t;

/*
 * +---------+
 * | SKYDOME |
 * +---------+
 *
 * --- Ejemplo de uso en lua ---
 * Skydome = {
 *		enabled on start = string,
 *		texture = string,
 *		curvature = float,
 *		tiling = float,
 *		distance = float,
 *		draw first = bool,
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
class ENGINE_API Skydome : public EngineComponent
{
	bool _enabledOnStart;
	std::string _textureName;

	float _curvature;
	float _tiling;
	float _distance;
	bool _drawFirst;

public:
	Skydome();
	~Skydome();

	bool init(const Properties& p) override;
	void ready() override;
	void disable() override;

	/*
	* @brief Establecer este skybox como el actual.
	*/
	void setSkydome();
	/*
	* @brief Quitar cualquier skybox de la escena.
	*/
	void setSkydomeNull();
};
