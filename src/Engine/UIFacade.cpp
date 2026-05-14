//#include "UIFacade.h"
//#include "RenderModule.h"
//
//UIFacade::UIFacade(RenderModule* render): _render(render)
//{}
//
//UITransformID UIFacade::addUITransform(const entityID& entityID, const core::Vector2<float>& pos, const int& zBuffer, const core::Vector2<float>& dimension, const float& rotation)
//{
//	return _render->addUITransform(entityID, pos, zBuffer, dimension, rotation);
//}
//
//void UIFacade::setUITransformDimension(const UITransformID& id, const core::Vector2<float>& dim)
//{
//	return _render->setUITransformDimension(id, dim);
//}
//
//void UIFacade::setUITransformPos(const UITransformID& id, const core::Vector2<float>& pos)
//{
//	return _render->setUITransformPos(id, pos);
//}
//
//void UIFacade::setUITransformRotation(const UITransformID& id, const float& r)
//{
//	return _render->setUITransformRotation(id, r);
//}
//
//void UIFacade::setUITransformZBuffer(const UITransformID& id, const int& zBuff)
//{
//	return _render->setUITransformZBuffer(id, zBuff);
//}
//
//uiPanelID UIFacade::addUIPanel(const entityID& entityID, const std::string& title)
//{
//	return _render->addUIPanel(entityID, title);
//}
//
//void UIFacade::setUIPanelVisible(const uiPanelID& id, bool visible)
//{
//	_render->setUIPanelVisible(id, visible);
//}
//void UIFacade::deleteUIPanel(const uiPanelID& id)
//{
//	_render->deleteUIPanel(id);
//}
//uiLabelID  UIFacade::addUILabel(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const  float opacity, const core::Color textColor, const core::Color bgColor, const float fontSize, const TextAlign textAlign, const std::string fontName) {
//
//	return _render->addUILabel(panelID, entityID, text, opacity, textColor, bgColor, fontSize, textAlign, fontName);
//}
//void UIFacade::deleteUILabel(const uiLabelID& id)
//{
//	_render->deleteUILabel(id);
//}
//void  UIFacade::setUILabelText(const uiLabelID& uiLabelID, const std::string& text) {
//	_render->setUILabelText(uiLabelID, text);
//}
//void  UIFacade::setUILabelVisible(const uiLabelID& uiLabelID, bool visible) {
//	_render->setUILabelVisible(uiLabelID, visible);
//}
//void UIFacade::setUILabelOpacity(const uiLabelID& labelID, float opacity) {
//	_render->setUILabelOpacity(labelID, opacity);
//
//}
//void  UIFacade::setUILabelTextColor(const uiLabelID& labelID, core::Color color) {
//	_render->setUILabelTextColor(labelID, color);
//}
//void  UIFacade::setUILabelBackGroundColor(const uiLabelID& labelID, core::Color color) {
//	_render->setUILabelBackGroundColor(labelID, color);
//}
//void  UIFacade::setUILabelAlign(const uiLabelID& labelID, const TextAlign& align) {
//	_render->setUILabelAlign(labelID, align);
//}
//
//uiButtonID  UIFacade::addUIButton(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const float& fontSize, const std::string& fontName, const core::Color& bgColor, const core::Color& txColor, const core::Color& hvColor, const core::Color& psColor, const float& opacity) {
//	return _render->addUIButton(panelID, entityID, text, fontSize, fontName, bgColor, txColor, hvColor, psColor, opacity);
//
//}
//
//void UIFacade::deleteUIButton(const uiButtonID& id)
//{
//	_render->deleteUIButton(id);
//
//}
//
//uiButtonID UIFacade::addUIImageButton(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const std::string& textureName, const core::Color& bgColor, const core::Color& hvColor, const core::Color& psColor, const float& opacity) {
//
//	auto texture = getAssetSourceFolder(textureName);
//
//	return _render->addUIImageButton(panelID, entityID, text, texture.second, texture.first, bgColor, hvColor, psColor, opacity);
//}
//void UIFacade::setUIButtonText(const uiButtonID& buttonID, const std::string& text) {
//	_render->setUIButtonText(buttonID, text);
//}
//void UIFacade::setUIButtonVisible(const uiButtonID& buttonID, bool& visible) {
//	_render->setUIButtonVisible(buttonID, visible);
//}
//void UIFacade::setUIButtonTexture(const uiButtonID& buttonID, std::string& textureName)
//{
//	auto texture = getAssetSourceFolder(textureName);
//	return _render->setUIButtonTexture(buttonID, texture.second, texture.first);
//}
//void  UIFacade::setUIButtonOpacity(const uiButtonID& buttonID, float& opacity) {
//	_render->setUIButtonOpacity(buttonID, opacity);
//}
//void UIFacade::setUIButtonBackgroundColor(const uiButtonID& buttonID, core::Color& bgColor)
//{
//	_render->setUIButtonBackgroundColor(buttonID, bgColor);
//}
//void UIFacade::setUIButtonTextColor(const uiButtonID& buttonID, core::Color& txColor)
//{
//	_render->setUIButtonTextColor(buttonID, txColor);
//}
//void UIFacade::setUIButtonHoverColor(const uiButtonID& buttonID, core::Color& hvColor)
//{
//	_render->setUIButtonHoverColor(buttonID, hvColor);
//}
//void UIFacade::setUIButtonPressColor(const uiButtonID& buttonID, core::Color& psColor)
//{
//	_render->setUIButtonPressColor(buttonID, psColor);
//}
//void UIFacade::setUIButtonDisable(const uiButtonID& buttonID, bool disable) {
//	_render->setUIButtonDisable(buttonID, disable);
//}
//void UIFacade::setUIButtonCallback(const uiButtonID& id, std::function<void()> callback) {
//	_render->setUIButtonCallback(id, callback);
//}
//uiTextureRectID UIFacade::addUITextureRect(const uiPanelID& panelID, const entityID& entityID, const std::string& textureName, float& opacity) {
//	auto texture = getAssetSourceFolder(textureName);
//	return _render->addUITextureRect(panelID, entityID, texture.second, texture.first, opacity);
//}
//void UIFacade::deleteUITextureRect(const uiTextureRectID& id)
//{
//	_render->deleteUITextureRect(id);
//}
//void UIFacade::setUITextureRectTexture(const uiTextureRectID& textureRectID, std::string& textureName) {
//	auto texture = getAssetSourceFolder(textureName);
//	_render->setUITextureRectTexture(textureRectID, texture.second, texture.first);
//}
//void UIFacade::setUITextureRectVisible(const uiTextureRectID& textureRectID, bool& visible) {
//	_render->setUITextureRectVisible(textureRectID, visible);
//}
//void UIFacade::setUITextureRectOpacity(const uiTextureRectID& textureRectID, float& opacity) {
//	_render->setUITextureRectOpacity(textureRectID, opacity);
//
//}