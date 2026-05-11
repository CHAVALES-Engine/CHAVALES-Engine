/*
 * @file
 * @brief Componente que permite modificar la luz ambiente de una escena.
 */
#pragma once
#include <Color.h>
#include <EngineAPI.h>
#include "Enginecomponent.h"

/*
 * +---------------+
 * | AMBIENT LIGHT |
 * +---------------+
 *
 * --- Ejemplo de uso en lua ---
 * AmbientLight = {
 *		color = Color.new(float...),
 * }
 *
 *
 * --- Ejemplo de inicializacion ---
 * En bool init(const Properties& p):
 *		# Ej1, asignacion:
 * component = getProperty<tipo>(properties, "atributo1");
 *		# Ej2, setter:
 * return setProperty(properties, "atributo1", component);
 *
*/
;
class ENGINE_API AmbientLight : public EngineComponent
{
public:
	AmbientLight() = default;

	bool init(const Properties& p) override;

	/**
	 * @brief Cambiar luz ambiente de la escena
	 */
	void setAmbientLight(const core::Color& color);
	/**
	 * @brief Quitar luz ambiente de la escena
	 */
	void clearAmbientLight();
};
