#include "UITextureRect.h"
#include "Entity.h"

#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "UIButton.h"
#include "checkMLNew.h"
#include <UITransform.h>
#include <UIPanel.h>

REGISTER_COMPONENT(UITextureRect);

UITextureRect::UITextureRect()
{
}

UITextureRect::~UITextureRect()
{
}

bool UITextureRect::init(const Properties& p)
{
	_textureName = getProperty<std::string>(p, "textureName");
	_opacity = getProperty<float>(p, "opacity");

	return true;
}

void UITextureRect::ready()
{
	auto panel = getEntity()->getComponent<UITransform>()->getComponentInParents<UIPanel>();


	uiPanelID  panelID = panel->getPanelID();
	_textureRectID = Engine::instance()->addUITextureRect(panelID,getEntity()->getEntityID(), _textureName);
}

void UITextureRect::setTexture(const std::string& texture)
{
	_textureName = texture;
	Engine::instance()->setUITextureRectTexture(_textureRectID, _textureName);
}

void UITextureRect::setVisible(bool visible) {
	Engine::instance()->setUITextureRectVisible(_textureRectID, visible);

}
void UITextureRect::setOpacity(float opacity)
{
	_opacity = opacity;
	Engine::instance()->setUITextureRectOpacity(_textureRectID, opacity);

}
