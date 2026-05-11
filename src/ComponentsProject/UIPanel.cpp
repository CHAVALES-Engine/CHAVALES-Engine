#include "UIPanel.h"

#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include "checkMLNew.h"
#include <UITransform.h>

REGISTER_COMPONENT(UIPanel);

UIPanel::UIPanel() : _title("Panel")
{
    registerMethod("setVisible", [this](const std::vector<std::any>& args) {
        if (args.size() >= 1) {
            setVisible(std::any_cast<bool>(args[0]));
        }
        });
}

UIPanel::~UIPanel(){}

bool UIPanel::init(const Properties& p)
{
    auto uiT = getEntity()->getComponent<UITransform>();
    if (!uiT) {
        Debug::error("[UIButton] - No transform, no se crea UIButton");
        _panelID = UINT64_MAX;
        return false;
    }
    _title = getProperty<std::string>(p, "name");
    _panelID = Engine::instance()->addUIPanel(getEntity()->getEntityID(), _title);
    return true;
}
panelID UIPanel::getPanelID() const
{
    return _panelID;
}
void UIPanel::ready()
{
   
}
void UIPanel::setVisible(bool visible)
{
    if (_panelID == UINT64_MAX)return;
    Engine::instance()->setUIPanelVisible(_panelID, visible);

}

void UIPanel::destroy()
{
    if (_panelID == UINT64_MAX)return;
    Engine::instance()->deleteUIPanel(_panelID);
}

