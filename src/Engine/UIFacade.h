//#pragma once
//#include <functional>
//
//#include "Color.h"
//#include "CommonEnums.h"
//#include "guid.h"
//#include "Vector2.h"
//
//
//class RenderModule;
//class Engine;
//using entityID = ChavalesGUID;
//using UITransformID = uint64_t;
//using uiPanelID = uint64_t;
//using uiLabelID = uint64_t;
//using uiButtonID = uint64_t;
//using uiTextureRectID = uint64_t;
//
///**
// * @brief API publica para UI del motor.
// * Expone solo los metodos seguros para componentes.
// * Para metodos internos, ver RenderModule.
// */
//class UIFacade
//{
//private:
//	/**
//	 * @brief Clases amigas.
//	 */
//	friend Engine;
//	friend core::EngineComponent;
//	/**
//	 * @brief Referencia al modulo que envuelve.
//	 */
//	RenderModule* _render = nullptr;
//	UIFacade(RenderModule* platform);
//
//	//Metodos UItransform
//#pragma region UItransform
//	/*
//	* @brief Anadir nodoUI
//	*/
//	UITransformID addUITransform(const entityID& entityID, const core::Vector2<float>& pos, const int& zBuffer, const core::Vector2<float>& dimension, const float& rotation);
//	/*
//	* @brief Establecer posicion del nodo.
//	*/
//	void setUITransformDimension(const UITransformID& id, const core::Vector2<float>& dim);
//	/*
//	* @brief Establecer dimension del nodo.
//	*/
//	void setUITransformPos(const UITransformID& id, const core::Vector2<float>& pos);
//	/*
//	* @brief Establecer rotacion del nodo.
//	*/
//	void setUITransformRotation(const UITransformID& id, const float& r);
//	/*
//	* @brief Establecer zBuffer del nodo.
//	*/
//	void setUITransformZBuffer(const UITransformID& id, const int& zBuff);
//#pragma endregion
//
//#pragma region UI-Panels
//	/*
//	* @brief Anadir panel de UI.
//	*/
//	uiPanelID addUIPanel(const entityID& entityID, const std::string& title);
//	/*
//	* @brief Establecer visibilidad del panel de UI.
//	*/
//	void setUIPanelVisible(const uiPanelID& id, bool visible);
//	/*
//	* @brief borrar panel de UI.
//	*/
//	void deleteUIPanel(const uiPanelID& id);
//#pragma endregion
//#pragma region UI-Labels
//	/*
//	* @brief Anadir letrero al panel.
//	*/
//	uiLabelID addUILabel(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const  float opacity, const core::Color textColor, const core::Color bgColor, const float fontSize, const TextAlign textAlign, const std::string fontName);
//	/*
//	* @brief borrar label de UI.
//	*/
//	void deleteUILabel(const uiLabelID& id);
//	/*
//	* @brief Establecer el texto del letrero.
//	*/
//	void setUILabelText(const uiLabelID& uiLabelID, const std::string& text);
//	/*
//	* @brief Establecer visibilidad del letrero.
//	*/
//	void setUILabelVisible(const uiLabelID& uiLabelID, bool visible);
//	/*
//	* @brief Establecer la opacidad  del letrero
//	*/
//	void setUILabelOpacity(const uiLabelID& labelID, float opacity);
//	/*
//	* @brief Establecer el color del texto  del letrero
//   */
//	void setUILabelTextColor(const uiLabelID& labelID, core::Color color);
//	/*
//	* @brief Establecer el color del fondo  del letrero
//	*/
//	void setUILabelBackGroundColor(const uiLabelID& labelID, core::Color color);
//	/*
//	* @brief Establecer el alineado  del letrero
//	*/
//	void setUILabelAlign(const uiLabelID& labelID, const TextAlign& align);
//
//#pragma endregion
//#pragma region UI-Buttons
//	/*
//	* @brief Anadir boton al panel.
//	*/
//	uiButtonID addUIButton(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const float& fontSize, const std::string& fontName, const core::Color& bgColor, const core::Color& txColor, const core::Color& hvColor, const core::Color& psColor, const float& opacity);
//	/*
//	* @brief borrar boton de UI.
//	*/
//	void deleteUIButton(const uiButtonID& id);
//	/*
//	* @brief Anadir ImageBoton al panel.
//	*/
//	uiButtonID addUIImageButton(const uiPanelID& panelID, const entityID& entityID, const std::string& text, const std::string& textureName, const core::Color& bgColor, const core::Color& hvColor, const core::Color& psColor, const float& opacity);
//	/*
//	* @brief Establecer el texto del boton.
//	*/
//	void setUIButtonText(const uiButtonID& buttonID, const std::string& text);
//	/*
//	* @brief Establecer visibilidad del boton.
//	*/
//	void setUIButtonVisible(const uiButtonID& buttonID, bool& visible);
//	/*
//	* @brief Establecer textura del boton.
//	*/
//	void  setUIButtonTexture(const uiButtonID& buttonID, std::string& textureName);
//	/*
//	* @brief Establecer la opacidad  del boton
//	*/
//	void  setUIButtonOpacity(const uiButtonID& buttonID, float& opacity);
//	/*
//	* @brief Establecer el color de fondo del boton
//	*/
//	void  setUIButtonBackgroundColor(const uiButtonID& buttonID, core::Color& bgColor);
//	/*
//	* @brief Establecer el color del texto del boton
//	*/
//	void  setUIButtonTextColor(const uiButtonID& buttonID, core::Color& txColor);
//	/*
//	* @brief Establecer el color del hover del boton
//	*/
//	void  setUIButtonHoverColor(const uiButtonID& buttonID, core::Color& hvColor);
//	/*
//	* @brief Establecer el color del pulsado del boton
//	*/
//	void  setUIButtonPressColor(const uiButtonID& buttonID, core::Color& psColor);
//	/*
//	* @brief Establecer el disable  del boton
//	*/
//	void setUIButtonDisable(const uiButtonID& buttonID, bool disable);
//	/*
//	* @brief Establecer callback del boton.
//	*/
//	void setUIButtonCallback(const uiButtonID& id, std::function<void()> callback);
//
//#pragma endregion
//#pragma region UI-TextureRect
//	/*
//	* @brief Anadir textureRect al panel.
//	*/
//	uiTextureRectID addUITextureRect(const uiPanelID& panelID, const entityID& entityID, const std::string& textureName, float& opacity);
//	/*
//	* @brief borrar textureRect de UI.
//	*/
//	void deleteUITextureRect(const uiTextureRectID& id);
//	/*
//	* @brief Establecer textura del textureRect.
//	*/
//	void setUITextureRectTexture(const uiTextureRectID& textureRectID, std::string& textureName);
//
//	/*
//	* @brief Establecer visibilidad del textureRect.
//	*/
//	void setUITextureRectVisible(const uiTextureRectID& id, bool& visible);
//	/*
//	* @brief Establecer la opacidad  del textureRect
//	*/
//	void  setUITextureRectOpacity(const uiTextureRectID& textureRectID, float& opacity);
//
//#pragma endregion
//};
//
