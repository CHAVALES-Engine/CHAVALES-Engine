/*
 * @file
 * @brief Componente que gestiona los letreros de la UI
 */
#pragma once
#include <Component.h>
#include <CommonEnums.h>

using labelID = uint64_t;
/*
 * +---------+
 * | UILABEL |
 * +---------+
 *
 * --- Ejemplo de uso en lua ---
 * UiPanel = {
 *		panelName = string
 *  	text = string
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
class UILabel : public core::Component
{
private:
	labelID _labelID;
	std::string _text;
	std::string _panelName;
	core::Vector2<float> _dimension;
	core::Color _bgColor;
	core::Color _textColor;
	float _opacity;
	float _fontSize;
	TextAlign _align = TextAlign::LEFT;
	std::string _fontFilePath;
	std::string _fotntFolderPath;
public:
	UILabel();
	~UILabel();

	bool init(const Properties& p) override;
	void setText(const std::string& text);
	void setVisible(bool visible);
	void setOpacity(float opacity);
	void setDimension(core::Vector2<float> dimension);
	void setBackgroudColor(core::Color color);
	void setTextColor(core::Color color);
	void setAlign(TextAlign align);
	void setFont(std::string font);
};

