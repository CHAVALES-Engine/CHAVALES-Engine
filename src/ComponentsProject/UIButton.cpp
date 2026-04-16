#include "UIButton.h"
#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"
#include <UITransform.h>
#include <UIPanel.h>

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
	_text = getProperty<std::string>(p, "text");
	_opacity = getProperty<float>(p, "opacity");

	return true;
}

void UIButton::ready()
{
	auto panel = getEntity()->getComponent<UITransform>()->getComponentInParents<UIPanel>();

	uiPanelID  panelID = panel->getPanelID();

	if (_textureName.empty()) {
		_buttonID = Engine::instance()->addUIButton(panelID, getEntity()->getEntityID(), _text);

	}
	else {
		_buttonID = Engine::instance()->addUIImageButton(panelID, getEntity()->getEntityID(), _text, _textureName);
	}
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

void UIButton::setOpacity(float opacity)
{
	_opacity = opacity;
	Engine::instance()->setUILabelOpacity(_buttonID, opacity);

}
void UIButton::setOnClick(std::function<void()> callback)
{
	Engine::instance()->setUIButtonCallback(_buttonID, callback);
}
