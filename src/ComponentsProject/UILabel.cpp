#include "UILabel.h"

#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "RenderModule.h"
#include <UITransform.h>
#include <UIPanel.h>
#include "checkMLNew.h"

REGISTER_COMPONENT(UILabel);

UILabel::UILabel()
{
	_labelID = UINT64_MAX;

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
	registerMethod("setAlign", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setAlign(std::any_cast<TextAlign>(args[0]));
		}
		});
}

UILabel::~UILabel(){}

bool UILabel::init(const Properties& p)
{
	if (!Component::init(p)) return false;
	std::string auxAlign;
	if(!setProperty<std::string>(p, "align", auxAlign)) return false;
	if (auxAlign == "center") {
		align = TextAlign::CENTER;
	}
	else if(auxAlign == "right"){
		align = TextAlign::RIGHT;
	}
	else if (auxAlign == "left") {
		align = TextAlign::LEFT;
	}
	else {
		Debug::error("[UILABEL] Align incompatible. Debe ser 'center', 'right' o 'left'");
		return false;
	}
	return true;
}

void UILabel::awake()
{
	auto uiT = getEntity()->getComponent<UITransform>();
	if (!uiT) {
		Debug::error("[UILabel] - No transform, no se crea UILabel");
		_labelID = UINT64_MAX;
		return;
	}
	auto panel = getEntity()->getComponent<UITransform>()->getComponentInParents<UIPanel>();
	if (!panel) {
		Debug::error("[UILabel] - No UIPanel en padres, no se crea UILabel");
		_labelID = UINT64_MAX;
		return;
	}
	uiPanelID  panelID = panel->getPanelID();
	if (panelID == UINT64_MAX) {
		Debug::error("[UILabel] - panelID, no se crea UILabel");
		_labelID = UINT64_MAX;
		return;
	}
	_labelID = render()->addUILabel(panelID, getEntity()->getEntityID(), text, opacity, textColor, bgColor, fontSize, align, fontName);
}

void UILabel::destroy()
{
	if (_labelID == UINT64_MAX)return;
	render()->deleteUILabel(_labelID);
	_labelID = UINT64_MAX;
}

void UILabel::setText(const std::string& _text)
{
	text = _text;
	if (_labelID == UINT64_MAX)return;
	render()->setUILabelText(_labelID, text);

}

void UILabel::setVisible(bool visible)
{
	if (_labelID == UINT64_MAX)return;
	render()->setUILabelVisible(_labelID, visible);
}

void UILabel::setOpacity(float _opacity)
{
	opacity = _opacity;
	if (_labelID == UINT64_MAX)return;
	render()->setUILabelOpacity(_labelID, opacity);

}

void UILabel::setBackgroudColor(const core::Color& color)
{
	bgColor = color;
	if (_labelID == UINT64_MAX)return;
	render()->setUILabelBackGroundColor(_labelID, bgColor);
}

void UILabel::setTextColor(const core::Color& color){
	textColor = color;
	if (_labelID == UINT64_MAX)return;
	render()->setUILabelTextColor(_labelID, textColor);

}

void UILabel::setAlign(TextAlign _align)
{
	align = _align;
	if (_labelID == UINT64_MAX)return;
	render()->setUILabelAlign(_labelID, align);
}