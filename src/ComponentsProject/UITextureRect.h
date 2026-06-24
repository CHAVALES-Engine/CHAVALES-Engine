/**
 * @file
 * @brief Componente que gestiona las texturas de la UI
 */

#pragma once
#include "EngineComponent.h"
#include <Vector2.h>
#include <EngineAPI.h>

#pragma once
using textureRectID = uint64_t;
/**
 * +----------+
 * | UIBUTTON |
 * +----------+
 *
 * --- Ejemplo de uso en lua ---
 * UiPanel = {
 *		textureFolder = string
 *  	texturePath = string
 *		panelName = string
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

class ENGINE_API UITextureRect : public EngineComponent
{
private :
	textureRectID _textureRectID;
	PROPERTY(std::string, textureName);
	PROPERTY(float, opacity);

public:
	UITextureRect();
	~UITextureRect();

	void ready() override;
	void setTexture(const std::string& texture);
	void setVisible(bool visible);
	void setOpacity(float _opacity);
	void destroy() override;
};