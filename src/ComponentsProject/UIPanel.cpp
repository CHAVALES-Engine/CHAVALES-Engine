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
    _panelID = UINT64_MAX;
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
        Debug::error("[UIPanel] - No transform, no se crea UIPanel");
        return false;
    }
    return true;
}
panelID UIPanel::getPanelID() const
{
    return _panelID;
}
void UIPanel::awake()
{
    _panelID = render()->addUIPanel(getEntity()->getEntityID(), name);
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
