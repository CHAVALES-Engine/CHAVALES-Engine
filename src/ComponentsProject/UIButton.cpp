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
	registerMethod("setText", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setText(std::any_cast<std::string>(args[0]));
		}
		});
	registerMethod("setVisible", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setVisible(std::any_cast<bool>(args[0]));
		}
		});
	registerMethod("setOpacity", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setOpacity(std::any_cast<float>(args[0]));
		}
		});
	registerMethod("setBackgroudColor", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setBackgroudColor(std::any_cast<core::Color>(args[0]));
		}
		});
	registerMethod("setTextColor", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setTextColor(std::any_cast<core::Color>(args[0]));
		}
		});
	registerMethod("setTexture", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setTexture(std::any_cast<std::string>(args[0]));
		}
		});
	registerMethod("setHoverColor", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setHoverColor(std::any_cast<core::Color>(args[0]));
		}
		});
	registerMethod("setPressColor", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setPressColor(std::any_cast<core::Color>(args[0]));
		}
		});
	registerMethod("setOnClick", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setOnClick(std::any_cast<std::function<void()>>(args[0]));
		}
		});
	
}

UIButton::~UIButton()
{
	Engine::instance()->deleteUIButton(_buttonID);
}
bool UIButton::init(const Properties& p)
{
	_textureName = getProperty<std::string>(p, "textureName");
	_text = getProperty<std::string>(p, "text");
	_opacity = getProperty<float>(p, "opacity");
	_fontSize = getProperty<float>(p, "fontSize");
	_fontName = getProperty<std::string>(p, "fontName");
	_bgColor = getProperty<core::Color>(p, "bgColor");
	_textColor = getProperty<core::Color>(p, "textColor");
	_hoverColor = getProperty<core::Color>(p, "hoverColor");
	_pressColor = getProperty<core::Color>(p, "pressColor");
	return true;
}

void UIButton::awake()
{
	auto panel = getEntity()->getComponent<UITransform>()->getComponentInParents<UIPanel>();

	uiPanelID  panelID = panel->getPanelID();

	if (_textureName.empty()) {
		_buttonID = Engine::instance()->addUIButton(panelID, getEntity()->getEntityID(), _text, _fontSize,_fontName,_bgColor,_textColor,_hoverColor,_pressColor, _opacity);

	}
	else {
		_buttonID = Engine::instance()->addUIImageButton(panelID, getEntity()->getEntityID(), _text, _textureName,_bgColor,_hoverColor,_pressColor, _opacity);
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
	Engine::instance()->setUIButtonOpacity(_buttonID, opacity);

}
void UIButton::setBackgroudColor(core::Color color) {
	_bgColor = color;
	Engine::instance()->setUIButtonBackgroundColor(_buttonID, _bgColor);
}
void UIButton::setTextColor(core::Color color) {
	_textColor = color;
	Engine::instance()->setUIButtonTextColor(_buttonID, _textColor);

}
void UIButton::setHoverColor(core::Color color) {
	_hoverColor = color;
	Engine::instance()->setUIButtonHoverColor(_buttonID, _hoverColor);

}
void UIButton::setPressColor(core::Color color) {
	_pressColor = color;
	Engine::instance()->setUIButtonPressColor(_buttonID, _pressColor);
}
void UIButton::setOnClick(std::function<void()> callback)
{
	Engine::instance()->setUIButtonCallback(_buttonID, callback);
}
