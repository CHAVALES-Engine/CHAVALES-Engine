#include "UIPanel.h"

#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>
#include <UITransform.h>
#include "RenderModule.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(UIPanel);

UIPanel::UIPanel()
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
    if (!Component::init(p)) return false;
    auto uiT = getEntity()->getComponent<UITransform>();
    if (!uiT) {
        Debug::error("[UIButton] - No transform, no se crea UIButton");
        _panelID = UINT64_MAX;
        return false;
    }
    name = getProperty<std::string>(p, "name");
    _panelID = render()->addUIPanel(getEntity()->getEntityID(), name);
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
    render()->setUIPanelVisible(_panelID, visible);

}

void UIPanel::destroy()
{
	if (_panelID == UINT64_MAX)return;
	render()->deleteUIPanel(_panelID);
	_panelID = UINT64_MAX;
}
