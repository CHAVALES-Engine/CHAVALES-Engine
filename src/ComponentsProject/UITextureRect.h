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
	std::string _textureName;
	float _opacity;

public:
	UITextureRect();
	~UITextureRect();

	bool init(const Properties& p) override;
	void awake() override;
	void setTexture(const std::string& texture);
	void setVisible(bool visible);
	void setOpacity(float opacity);
	void destroy() override;
};