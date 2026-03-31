// InputTest.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>

#include <Debug.h>
#include <Engine.h>
#include <InputFacade.h>


int main()
{
	// Inicializa el Engine
	Engine::init();
	Engine::instance()->input()->addEventToAction("Jump", input::KEY_SPACE, input::KEYBOARD_ID);
	Engine::instance()->input()->addEventToAction("Left", input::KEY_META, input::KEYBOARD_ID);
	Engine::instance()->input()->addEventToAction("Right", input::KEY_DELETE, input::KEYBOARD_ID);
	Engine::instance()->input()->addEventToAction("write", input::KEY_Z, input::KEYBOARD_ID);
	// Lanza el bucle de juego
	while (!Engine::instance()->pollEvents() && !Engine::instance()->input()->isKeyPressed(input::KEY_A, input::KEYBOARD_ID))
	{
		// para que se vea mejor
		Debug::out("\n\n\n\n\n\n\n\n\n\n\n\n");
		
		// Pruebas de acciones
		//if (Engine::instance()->input()->isActionPressed("Jump")) Debug::out("Jump");
		//if (Engine::instance()->input()->isActionPressed("Left")) Debug::out("Left");
		//if (Engine::instance()->input()->isActionPressed("Right")) Debug::out("Right");
		// Pruebas de botones
		//if (Engine::instance()->input()->isKeyPressed(input::MOUSE_BUTTON_LEFT)) {
		//	Debug::out("StartWrite");
		//	Engine::instance()->input()->startTextInput();
		//}
		//if (Engine::instance()->input()->isKeyReleased(input::MOUSE_BUTTON_RIGHT))
		//{
		//	Engine::instance()->input()->stopTextInput();
		//	Debug::out("end write: ", Engine::instance()->input()->getTextInput());
		//}

		// Pruebas de axis de raton
		/*Debug::out("MOUSE_AXIS_X: ", Engine::instance()->input()->getAxis(input::MOUSE_AXIS_X));
		Debug::out("MOUSE_AXIS_Y: ", Engine::instance()->input()->getAxis(input::MOUSE_AXIS_Y));
		Debug::out("MOUSE_AXIS_REL_X: ", Engine::instance()->input()->getAxis(input::MOUSE_AXIS_REL_X));
		Debug::out("MOUSE_AXIS_REL_Y: ", Engine::instance()->input()->getAxis(input::MOUSE_AXIS_REL_Y));
		Debug::out("MOUSE_AXIS_SCROLL_X: ", Engine::instance()->input()->getAxis(input::MOUSE_AXIS_SCROLL_X));
		Debug::out("MOUSE_AXIS_SCROLL_Y: ", Engine::instance()->input()->getAxis(input::MOUSE_AXIS_SCROLL_Y));*/

		// Pruebas con gamepad

		Debug::out("MOUSE_AXIS_X: ", Engine::instance()->input()->getAxis(input::GAMEPAD_AXIS_LEFT_TRIGGER));
		Debug::out("MOUSE_AXIS_REL_X: ", Engine::instance()->input()->getAxis(input::GAMEPAD_AXIS_LEFT_X));
		Debug::out("MOUSE_AXIS_REL_Y: ", Engine::instance()->input()->getAxis(input::GAMEPAD_AXIS_LEFT_Y));
		Debug::out("MOUSE_AXIS_Y: ", Engine::instance()->input()->getAxis(input::GAMEPAD_AXIS_RIGHT_TRIGGER));
		Debug::out("MOUSE_AXIS_SCROLL_X: ", Engine::instance()->input()->getAxis(input::GAMEPAD_AXIS_RIGHT_X));
		Debug::out("MOUSE_AXIS_SCROLL_Y: ", Engine::instance()->input()->getAxis(input::GAMEPAD_AXIS_RIGHT_Y));

	}

	// Cierre limpio del engine
	Engine::release();
}
