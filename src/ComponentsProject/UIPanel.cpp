#include "UIPanel.h"

#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"

REGISTER_COMPONENT(UIPanel);

UIPanel::UIPanel() : _title("Panel")
{
}

UIPanel::~UIPanel()
{
}

bool UIPanel::init(const Properties& p)
{
    _title = getProperty<std::string>(p, "name");

    _panelID = Engine::instance()->addUIPanel(getEntity()->getEntityID(),_title);

    return true;
}

void UIPanel::setVisible(bool visible)
{
    Engine::instance()->setUIPanelVisible(_panelID, visible);

}

