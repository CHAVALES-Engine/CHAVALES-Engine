#include "UIButton.h"
#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "RenderModule.h"
#include <UITransform.h>
#include <UIPanel.h>
#include "checkMLNew.h"


REGISTER_COMPONENT(UIButton);

UIButton::UIButton() : _text("Button")
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
	registerMethod("disable", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			disable(std::any_cast<bool>(args[0]));
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
	auto uiT = getEntity()->getComponent<UITransform>();
	if (!uiT) {
		Debug::error("[UIButton] - No transform, no se crea UIButton");
		_buttonID = UINT64_MAX;
		return;
	}
	auto panel = uiT->getComponentInParents<UIPanel>();
	if (!panel) {
		Debug::error("[UIButton] - No UIPanel en padres, no se crea UIButton");
		_buttonID = UINT64_MAX;
		return;
	}
	uiPanelID  panelID = panel->getPanelID();
	if (panelID == UINT64_MAX) {
		Debug::error("[UIButton] - panelID, no se crea UIButton");
		_buttonID = UINT64_MAX;
		return;
	}
	if (_textureName.empty()) {
		_buttonID = render()->addUIButton(panelID, getEntity()->getEntityID(), _text, _fontSize, _fontName, _bgColor, _textColor, _hoverColor, _pressColor, _opacity);

	}
	else {
		auto texture = Engine::instance()->getAssetSourceFolder(_textureName);
		_buttonID = render()->addUIImageButton(panelID, getEntity()->getEntityID(), _text, texture.second, texture.first, _bgColor, _hoverColor, _pressColor, _opacity);
	}
}

void UIButton::destroy()
{
	if (_buttonID == UINT64_MAX)return;
	render()->deleteUIButton(_buttonID);
	_buttonID = UINT64_MAX;
}

void UIButton::setText(const std::string& text) {
	_text = text;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonText(_buttonID, _text);
}

void UIButton::setVisible(bool visible) {
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonVisible(_buttonID, visible);
}
void UIButton::setTexture(const std::string& texture)
{
	_textureName = texture;
	if (_buttonID == UINT64_MAX)return;
	auto text= Engine::instance()->getAssetSourceFolder(_textureName);
	render()->setUIButtonTexture(_buttonID, text.second, text.first);
}

void UIButton::setOpacity(float opacity)
{
	_opacity = opacity;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonOpacity(_buttonID, opacity);
}
void UIButton::setBackgroudColor(const core::Color& color) {
	_bgColor = color;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonBackgroundColor(_buttonID, _bgColor);
}
void UIButton::setTextColor(const core::Color& color) {
	_textColor = color;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonTextColor(_buttonID, _textColor);
}
void UIButton::setHoverColor(const core::Color& color) {
	_hoverColor = color;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonHoverColor(_buttonID, _hoverColor);
}
void UIButton::setPressColor(const core::Color& color) {
	_pressColor = color;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonPressColor(_buttonID, _pressColor);
}
void UIButton::disable(bool disable)
{
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonDisable(_buttonID, disable);
}
void UIButton::setOnClick(std::function<void()> callback)
{
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonCallback(_buttonID, callback);
}