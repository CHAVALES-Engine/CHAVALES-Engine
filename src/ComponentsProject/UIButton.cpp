#include "UIButton.h"
#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"

REGISTER_COMPONENT(UIButton);

UIButton::UIButton(): _text("Button")
{
}

UIButton::~UIButton()
{
}
bool UIButton::init(const Properties& p)
{
	_textureName = getProperty<std::string>(p, "textureName");
	_dimension = getProperty<core::Vector2<float>>(p, "dimension");
	_text = getProperty<std::string>(p, "text");
	_panelName = getProperty<std::string>(p, "panelName");
	_opacity = getProperty<float>(p, "opacity");

	_buttonID = Engine::instance()->addUIButton(_panelName, getEntity()->getEntityID(), _text, _textureName, _dimension);

	return true;
}

void UIButton::setText(const std::string& text) {
	_text = text;
	Engine::instance()->setUIButtonText(_buttonID, _text);

}

void UIButton::setVisible(bool visible) {
	Engine::instance()->setUIButtonVisible(_buttonID, visible);

}
void UIButton::setTexture(const std::string& texture)
{
	_textureName = texture;
	Engine::instance()->setUIButtonTexture(_buttonID, _textureName);
}
void UIButton::setDimension(core::Vector2<float> dimension)
{
	_dimension = dimension;
	Engine::instance()->setUIButtonDimension(_buttonID, dimension);

}
void UIButton::setOpacity(float opacity)
{
	_opacity = opacity;
	Engine::instance()->setUILabelOpacity(_buttonID, opacity);

}
void UIButton::setOnClick(std::function<void()> callback)
{
	Engine::instance()->setUIButtonCallback(_buttonID, callback);
}
