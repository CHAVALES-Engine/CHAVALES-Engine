/*
 * @file
 * @brief Componente que gestiona las texturas de la UI
 */
#pragma once
#include <Component.h>
#include <Vector2.h>
#pragma once
using textureRectID = uint64_t;
/*
 * +----------+
 * | UIBUTTON |
 * +----------+
 *
 * --- Ejemplo de uso en lua ---
 * UiPanel = {
 *		textureFolder = string
 *  	texturePath = string
 *		panelName = string
 *		dimension = vector2<float>
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
class UITextureRect : public core::Component
{
private :
	textureRectID _textureRectID;
	std::string  _panelName;
	std::string _textureName;
	core::Vector2<float> _dimension;
	float _opacity;

public:
	UITextureRect();
	~UITextureRect();

	bool init(const Properties& p) override;
	void setTexture(const std::string& texture);
	void setDimension(core::Vector2<float> dimension);
	void setVisible(bool visible);
	void setOpacity(float opacity);

};

