#include "UILabel.h"

#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>

REGISTER_COMPONENT(UILabel);

UILabel::UILabel() : _text("Label")
{
}

UILabel::~UILabel()
{
}

bool UILabel::init(const Properties& p)
{
	_text = getProperty<std::string>(p, "text");
	_panelName = getProperty<std::string>(p, "panelName");

	_labelID = Engine::instance()->addUILabel(_panelName, getEntity()->getEntityID(), _text);

	return true;
}

void UILabel::setText(const std::string& text)
{
	_text = text;
	Engine::instance()->setUILabelText(_labelID, _text);

}

void UILabel::setVisible(bool visible)
{
	Engine::instance()->setUILabelVisible(_labelID, visible);
}
