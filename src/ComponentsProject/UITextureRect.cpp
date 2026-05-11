#include "UITextureRect.h"
#include "Entity.h"

#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "UIButton.h"
#include <UITransform.h>
#include <UIPanel.h>
#include "RenderModule.h"

#include "checkMLNew.h"

REGISTER_COMPONENT(UITextureRect);

UITextureRect::UITextureRect()
{
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
	registerMethod("setTexture", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setTexture(std::any_cast<std::string>(args[0]));
		}
		});
}

UITextureRect::~UITextureRect(){}

bool UITextureRect::init(const Properties& p)
{
	_textureName = getProperty<std::string>(p, "textureName");
	_opacity = getProperty<float>(p, "opacity");

	return true;
}

void UITextureRect::awake()
{
	auto uiT = getEntity()->getComponent<UITransform>();
	if (!uiT) {
		Debug::error("[UITextureRect] - No transform, no se crea UITextureRect");
		_textureRectID = UINT64_MAX;
		return;
	}
	auto panel = uiT->getComponentInParents<UIPanel>();
	if (!panel) {
		Debug::error("[UITextureRect] - No UIPanel en padres, no se crea UITextureRect");
		_textureRectID = UINT64_MAX;
		return;
	}
	uiPanelID  panelID = panel->getPanelID();
	if (panelID == UINT64_MAX) {
		Debug::error("[UITextureRect] - panelID, no se crea UITextureRect");
		_textureRectID = UINT64_MAX;
		return;
	}
	auto texture = Engine::instance()->getAssetSourceFolder(_textureName);
	_textureRectID = render()->addUITextureRect(panelID,getEntity()->getEntityID(), texture.second, texture.first, _opacity);
}

void UITextureRect::setTexture(const std::string& texture)
{
	_textureName = texture;
	if (_textureRectID == UINT64_MAX)return;
	auto text = Engine::instance()->getAssetSourceFolder(_textureName);
	render()->setUITextureRectTexture(_textureRectID, text.second, text.first);
}

void UITextureRect::setVisible(bool visible) {
	if (_textureRectID == UINT64_MAX)return;
	render()->setUITextureRectVisible(_textureRectID, visible);
}
void UITextureRect::setOpacity(float opacity)
{
	_opacity = opacity;
	if (_textureRectID == UINT64_MAX)return;
	render()->setUITextureRectOpacity(_textureRectID, opacity);
}

void UITextureRect::destroy()
{
	if (_textureRectID == UINT64_MAX)return;
	render()->deleteUITextureRect(_textureRectID);
}
