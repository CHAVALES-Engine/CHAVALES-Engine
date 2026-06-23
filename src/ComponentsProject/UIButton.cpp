#include "UIButton.h"
#include "Entity.h"
#include <Debug.h>
#include <PluginSDK.h>

#include "RenderModule.h"
#include "ResourcesModule.h"
#include "Engine.h"

#include <UITransform.h>
#include <UIPanel.h>
#include "checkMLNew.h"


REGISTER_COMPONENT(UIButton);

UIButton::UIButton()
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
//bool UIButton::init(const Properties& p)
//{
//	textureName = getProperty<std::string>(p, "textureName");
//	text = getProperty<std::string>(p, "text");
//	opacity = getProperty<float>(p, "opacity");
//	fontSize = getProperty<float>(p, "fontSize");
//	fontName = getProperty<std::string>(p, "fontName");
//	bgColor = getProperty<core::Color>(p, "bgColor");
//	textColor = getProperty<core::Color>(p, "textColor");
//	hoverColor = getProperty<core::Color>(p, "hoverColor");
//	pressColor = getProperty<core::Color>(p, "pressColor");
//	return true;
//}

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
	if (textureName->empty()) {
		_buttonID = render()->addUIButton(panelID, getEntity()->getEntityID(), text, fontSize, fontName, bgColor, textColor, hoverColor, pressColor, opacity);

	}
	else {
		core::ResourcePtr res = resources()->getOrLoadAsset(textureName);
		if (!res || !res->isValid()) {
			Debug::error("[UIButton] Textura no encontrada: ", textureName);
			_buttonID = UINT64_MAX;
			return;
		}

		_buttonID = render()->addUIImageButton(panelID, getEntity()->getEntityID(), text, res->getPath(), res->getName(), bgColor, hoverColor, pressColor, opacity);
	}
}

void UIButton::destroy()
{
	if (_buttonID == UINT64_MAX)return;
	render()->deleteUIButton(_buttonID);
	_buttonID = UINT64_MAX;
}

void UIButton::setText(const std::string& _text) {
	text = _text;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonText(_buttonID, text);
}

void UIButton::setVisible(bool visible) {
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonVisible(_buttonID, visible);
}
void UIButton::setTexture(const std::string& texture)
{
	textureName = texture;
	if (_buttonID == UINT64_MAX)return;

	core::ResourcePtr res = resources()->getOrLoadAsset(textureName);
	if (!res || !res->isValid()) {
		Debug::error("[UIButton] Textura no encontrada: ", textureName);
		return;
	}

	render()->setUIButtonTexture(_buttonID, res->getPath(), res->getName());
}

void UIButton::setOpacity(float opacity)
{
	opacity = opacity;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonOpacity(_buttonID, opacity);
}
void UIButton::setBackgroudColor(const core::Color& color) {
	bgColor = color;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonBackgroundColor(_buttonID, bgColor);
}
void UIButton::setTextColor(const core::Color& color) {
	textColor = color;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonTextColor(_buttonID, textColor);
}
void UIButton::setHoverColor(const core::Color& color) {
	hoverColor = color;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonHoverColor(_buttonID, hoverColor);
}
void UIButton::setPressColor(const core::Color& color) {
	pressColor = color;
	if (_buttonID == UINT64_MAX)return;
	render()->setUIButtonPressColor(_buttonID, pressColor);
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