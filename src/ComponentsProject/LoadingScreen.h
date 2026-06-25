/**
 * @file
 * @brief Componente que permite modificar la luz ambiente de una escena.
 */
#pragma once
#include <Color.h>
#include <EngineAPI.h>
#include "Enginecomponent.h"

 /*
  * +----------------+
  * | LOADING SCREEN |
  * +----------------+
  *
  * --- Ejemplo de uso en lua ---
  * LoadingScreen = {
  *		bgImage = string,
  *		barColor = Color.new(float...),
  *		fontName = string
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

using labelID = uint64_t;
using textureRectID = uint64_t;
using panelID = uint64_t;


class ENGINE_API LoadingScreen : public EngineComponent
{
public:
	LoadingScreen() = default;

	bool init(const Properties& p) override;
};