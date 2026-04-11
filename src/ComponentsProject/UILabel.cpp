#include "UILabel.h"

#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"

REGISTER_COMPONENT(UILabel);

UILabel::UILabel() : _text("Label")
{
}

UILabel::~UILabel()
{
}

bool UILabel::init(const Properties& p)
{
	_text = getProperty<std::string>(p, "text");
	_panelName = getProperty<std::string>(p, "panelName");
	_opacity = getProperty<float>(p, "opacity");
	_dimension = getProperty<core::Vector2<float>>(p, "dimension");
	_bgColor = getProperty<core::Color>(p, "bgColor");
	_textColor = getProperty<core::Color>(p, "textColor");
	std::string auxAlign = getProperty<std::string>(p, "align");
	if (auxAlign == "center") {
		_align = TextAlign::CENTER;
	}
	else if(auxAlign == "right"){
		_align = TextAlign::RIGHT;
	}
	else {
		_align = TextAlign::LEFT;

	}
	_fontSize = getProperty<float>(p, "fontSize");
	_fontName = getProperty<std::string>(p, "fontName");
	_labelID = Engine::instance()->addUILabel(_panelName, getEntity()->getEntityID(), _text,_opacity,_dimension,_textColor,_bgColor,_fontSize, _align,_fontName);
	return true;
}

void UILabel::setText(const std::string& text)
{
	_text = text;
	Engine::instance()->setUILabelText(_labelID, _text);

}

void UILabel::setVisible(bool visible)
{
	Engine::instance()->setUILabelVisible(_labelID, visible);
}

void UILabel::setOpacity(float opacity)
{
	_opacity = opacity;
	Engine::instance()->setUILabelOpacity(_labelID, _opacity);

}

void UILabel::setDimension(core::Vector2<float> dimension)
{
	_dimension = dimension;
	Engine::instance()->setUILabelDimension(_labelID, _dimension);

}

void UILabel::setBackgroudColor(core::Color color)
{
	_bgColor = color;
	Engine::instance()->setUILabelBackGroundColor(_labelID, _bgColor);
}

void UILabel::setTextColor(core::Color color){
	_textColor = color;
	Engine::instance()->setUILabelTextColor(_labelID, _textColor);

}

void UILabel::setAlign(TextAlign align)
{
	_align = align;
}

void UILabel::setFont(std::string font)
{
	_fontName = font;
}
