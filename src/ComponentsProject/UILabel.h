/**
 * @file
 * @brief Componente que gestiona los letreros de la UI
 */
#pragma once
#include "EngineComponent.h"
#include <CommonEnums.h>
#include <EngineAPI.h>


using labelID = uint64_t;
/**
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
class ENGINE_API UILabel : public EngineComponent
{
private:
	labelID _labelID;
	std::string _text;
	core::Color _bgColor;
	core::Color _textColor;
	float _opacity;
	float _fontSize;
	TextAlign _align = TextAlign::LEFT;
	std::string _fontName;
public:
	UILabel();
	~UILabel();

	bool init(const Properties& p) override;
	void awake() override;
	void destroy() override;
	void setText(const std::string& text);
	void setVisible(bool visible);
	void setOpacity(float opacity);
	void setBackgroudColor(const core::Color& color);
	void setTextColor(const core::Color& color);
	void setAlign(TextAlign align);
};