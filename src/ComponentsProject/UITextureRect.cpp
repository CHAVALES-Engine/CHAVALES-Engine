#include "UITextureRect.h"
#include <Debug.h>
#include "Entity.h"
#include <PluginSDK.h>

#include "RenderModule.h"
#include "ResourcesModule.h"
#include "Engine.h"

#include <UITransform.h>
#include <UIPanel.h>
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
	core::ResourcePtr res = resources()->getOrLoadAsset(_textureName);
	if (!res || !res->isValid()) {
		Debug::error("[UITextureRect] Textura no encontrada: ", _textureName);
		_textureRectID = UINT64_MAX;
		return;
	}

	_textureRectID = render()->addUITextureRect(panelID, getEntity()->getEntityID(), res->getPath(), res->getName(), _opacity);
}

void UITextureRect::setTexture(const std::string& texture)
{
	_textureName = texture;
	if (_textureRectID == UINT64_MAX)return;

	core::ResourcePtr res = resources()->getOrLoadAsset(_textureName);
	if (!res || !res->isValid()) {
		Debug::error("[UITextureRect] Textura no encontrada: ", _textureName);
		return;
	}

	render()->setUITextureRectTexture(_textureRectID, res->getPath(), res->getName());
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
	_textureRectID = UINT64_MAX;
}
