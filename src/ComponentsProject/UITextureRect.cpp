#include "UITextureRect.h"
#include "Entity.h"

#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>

REGISTER_COMPONENT(UITextureRect);

UITextureRect::UITextureRect()
{
}

UITextureRect::~UITextureRect()
{
}

bool UITextureRect::init(const Properties& p)
{
	_textureFolder = getProperty<std::string>(p, "textureFolder");
	_textureFile = getProperty<std::string>(p, "texturePath");
	_panelName = getProperty<std::string>(p, "panelName");
	_dimension = getProperty<core::Vector2<float>>(p, "dimension");

	_textureRectID = Engine::instance()->addUITextureRect(_panelName, getEntity()->getEntityID(), _textureFolder,_textureFile, _dimension);
	return true;
}

void UITextureRect::setTexture(const std::string& textture)
{
}
void UITextureRect::setVisible(bool visible) {

}
