#include "UILabel.h"

#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>

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
	_bgColor = getProperty<core::Color>(p, "bgColor");
	_textColor = getProperty<core::Color>(p, "textColor");
	_align = getProperty<std::string>(p, "align");
	_fotntFolderPath = getProperty<std::string>(p, "fontFolder");
	_fontFilePath = getProperty<std::string>(p, "fontPath");
	_labelID = Engine::instance()->addUILabel(_panelName, getEntity()->getEntityID(), _text);
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

void UILabel::setAlign(std::string align)
{
	_align = align;
}

void UILabel::setFont(std::string font)
{
	_fontFilePath = font;
}
