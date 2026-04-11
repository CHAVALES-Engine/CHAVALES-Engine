#include "UITextureRect.h"
#include "Entity.h"

#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "UIButton.h"
#include "checkMLNew.h"

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
	_panelName = getProperty<std::string>(p, "panelName");
	_dimension = getProperty<core::Vector2<float>>(p, "dimension");
	_opacity = getProperty<float>(p, "opacity");

	_textureRectID = Engine::instance()->addUITextureRect(_panelName, getEntity()->getEntityID(), _textureName, _dimension);
	return true;
}

void UITextureRect::setTexture(const std::string& texture)
{
	_textureName = texture;
	Engine::instance()->setUITextureRectTexture(_textureRectID, _textureName);
}
void UITextureRect::setDimension(core::Vector2<float> dimension)
{
	_dimension = dimension;
	Engine::instance()->setUITextureRectDimension(_textureRectID, _dimension);
}
void UITextureRect::setVisible(bool visible) {
	Engine::instance()->setUITextureRectVisible(_textureRectID, visible);

}
void UITextureRect::setOpacity(float opacity)
{
	_opacity = opacity;
	Engine::instance()->setUITextureRectOpacity(_textureRectID, opacity);

}
