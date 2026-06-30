#include "InputTextButton.h"

#include <Engine.h>
#include <PluginSDK.h>

#include "Debug.h"
#include "InputFacade.h"

#include "checkMLNew.h"


#include "Entity.h"

REGISTER_COMPONENT(InputTextButton);

static InputTextButton* s_activeField = nullptr;
InputTextButton::~InputTextButton()
{
	if (s_activeField == this) s_activeField = nullptr;
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
	if (!_isWriteable) return;
	// Cambiar si es escribibile por el ENTER tambien.
	if (Input()->isJustPressed(input::KEY_ENTER))
	{
		_changeWritteability();
		return;
	}

	// Borrado con Backspace.
	if (Input()->isJustPressed(input::KEY_BACKSPACE))
		Input()->backspaceTextInput();

	// Leer el buffer (ya reducido si hubo backspace) y actualizar solo si cambia.
	std::string current = Input()->getTextInput();
	if (current != _info)
	{
		_info = current;
		setText(_info);
	}
}

void InputTextButton::_changeWritteability()
{
	_isWriteable = !_isWriteable;

	Debug::warning("[InputText] Cambio a: ", _isWriteable ? "SI escribible" : "NO escribible");
	if (_isWriteable)
	{
		// Desactivar el campo que estuviera activo antes.
		if (s_activeField && s_activeField != this)
		{
			s_activeField->_isWriteable = false;
		}
		s_activeField = this;

		_info = "";
		Input()->clearTextInput();
		Input()->startTextInput();
	}
	else
	{
		if (s_activeField == this) s_activeField = nullptr;
		Input()->stopTextInput();
		Input()->clearTextInput();
	}
}
