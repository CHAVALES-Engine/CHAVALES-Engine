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
	PROPERTY(std::string, text);
	PROPERTY(core::Color, bgColor);
	PROPERTY(core::Color, textColor);
	PROPERTY(float, opacity);
	PROPERTY(float, fontSize);
	PROPERTY(std::string, fontName);
	TextAlign align = TextAlign::LEFT;
public:
	UILabel();
	~UILabel();

	bool init(const Properties& p) override;
	void ready() override;
	void destroy() override;
	void setText(const std::string& _text);
	void setVisible(bool visible);
	void setOpacity(float _opacity);
	void setBackgroudColor(const core::Color& color);
	void setTextColor(const core::Color& color);
	void setAlign(TextAlign _align);
};