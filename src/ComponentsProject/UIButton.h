/*
 * @file
 * @brief Componente que gestiona los botones de la UI
 */


#pragma once
#include <Component.h>
#include <EngineAPI.h>

using buttonID = uint64_t;
/*
 * +----------+
 * | UIBUTTON |
 * +----------+
 *
 * --- Ejemplo de uso en lua ---
 * UiPanel = {
 *		panelName = string
 *  	text = string
 *		textureFolder = string
 *  	texturePath = string
 *
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
class ENGINE_API UIButton : public core::Component
{
private:
	buttonID _buttonID;
	std::string _text;
	std::string _textureName;
	float _opacity;
	float _fontSize;
	std::string _fontName;
	core::Color _bgColor;
	core::Color _textColor;
	core::Color _hoverColor;
	core::Color _pressColor;

public:
	UIButton();
	~UIButton();

	bool init(const Properties& p) override;
	void ready() override;
	void setText(const std::string& text);
	void setVisible(bool visible);
	void setTexture(const std::string& texture);
	void setOnClick(std::function<void()>callback);
	void setOpacity(float opacity);
	void setBackgroudColor(core::Color color);
	void setTextColor(core::Color color);
	void setHoverColor(core::Color color);
	void setPressColor(core::Color color);
};

