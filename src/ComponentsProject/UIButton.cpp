#include "UIButton.h"
#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>

REGISTER_COMPONENT(UIButton);

UIButton::UIButton(): _text("Button")
{
}

UIButton::~UIButton()
{
}
bool UIButton::init(const Properties& p)
{
	_text = getProperty<std::string>(p, "text");
	_panelName = getProperty<std::string>(p, "panelName");

	_buttonID = Engine::instance()->addUILabel(_panelName, getEntity()->getEntityID(), _text);

	return true;
}

void UIButton::setText(const std::string& text) {
	_text = text;
	Engine::instance()->setUIButtonText(_buttonID, _text);

}

void UIButton::setVisible(bool visible) {
	Engine::instance()->setUIButtonVisible(_buttonID, visible);

}


void UIButton::setOnClick(std::function<void()> callback)
{
	Engine::instance()->setUIButtonCallback(_buttonID, callback);
}
