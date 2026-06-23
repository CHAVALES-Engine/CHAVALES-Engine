/**
 * @file
 * @brief Componente que gestiona los botones de la UI
 */

#pragma once
#include "EngineComponent.h"
#include <EngineAPI.h>

using buttonID = uint64_t;
/**
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
class ENGINE_API UIButton : public EngineComponent
{
private:
	buttonID _buttonID;
	PROPERTY(std::string, text);
	PROPERTY(std::string, textureName);
	PROPERTY(float, opacity);
	PROPERTY(float, fontSize);
	PROPERTY(std::string, fontName);
	PROPERTY(core::Color, bgColor);
	PROPERTY(core::Color, textColor);
	PROPERTY(core::Color, hoverColor);
	PROPERTY(core::Color, pressColor);

public:
	UIButton();
	~UIButton();

	//bool init(const Properties& p) override;
	void awake() override;
	void destroy() override;
	void setText(const std::string& _text);
	void setVisible(bool visible);
	void setTexture(const std::string& texture);
	void setOnClick(std::function<void()>callback);
	void setOpacity(float opacity);
	void setBackgroudColor(const core::Color& color);
	void setTextColor(const core::Color& color);
	void setHoverColor(const core::Color& color);
	void setPressColor(const core::Color& color);
	void disable(bool disable);
};