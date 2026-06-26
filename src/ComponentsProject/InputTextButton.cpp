#include "InputTextButton.h"

#include <Engine.h>
#include <PluginSDK.h>

#include "Debug.h"
#include "InputFacade.h"

#include "checkMLNew.h"


#include "Entity.h"

REGISTER_COMPONENT(InputTextButton);

InputTextButton::~InputTextButton()
{

}

void InputTextButton::ready()
{
	UIButton::ready();
	
	setHoverColor({ 0.933f, 0.808f, 0.251f, 0.5f });
	setPressColor({ 0.925f, 0.71f, 0.929f, .7f });
	setOnClick([this] { _changeWritteability(); }); // Callback.
}

void InputTextButton::update(uint64_t deltaTime)
{
	// Llamado cada frame, con el deltaTime desde el ultimo frame.

	// Cambiar si es escribibile por el ENTER tambien.
	if (Input()->isJustPressed(input::KEY_ENTER) && _isWriteable)
	{
		_changeWritteability();
	}

	if (_isWriteable)
	{
		_info = Input()->getTextInput();

		setText(_info);
	}
}

void InputTextButton::_changeWritteability()
{
	_isWriteable = !_isWriteable;

	Debug::warning("[InputText] Cambio a: ", _isWriteable ? "SI escribible" : "NO escribible");
	if (_isWriteable)
	{
		_info = "";
		Input()->startTextInput();
	}
	else
	{
		Input()->stopTextInput();
		Input()->clearTextInput();
	}
}