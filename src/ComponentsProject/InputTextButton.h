#pragma once
#include <Vector3.h>
#include <Color.h>

#include "UIButton.h"

class InputTextButton : public UIButton
{
public:
	// Constructor por defecto.
	InputTextButton() = default;
	// Destructor, no olvidar borrar memoria.
	~InputTextButton();

	// Llamado una vez al entrar en la escena.
	void ready() override;

	// Llamado cada frame, con el deltaTime desde el ultimo frame.
	void update(uint64_t deltaTime) override;


	std::string getTextInfo() const { return _info; }
	bool getIsWriteable() const { return _isWriteable; }

private:
	//  Variables miembro del componente 

	bool _isWriteable = false;
	std::string _info;
	void _changeWritteability();
};
