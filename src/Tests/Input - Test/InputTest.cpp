// InputTest.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>

#include <Engine.h>
#include <InputFacade.h>

int main()
{
	// Inicializa el Engine
	Engine::init();
	// Lanza el bucle de juego
	while (!Engine::instance()->input()->isKeyPressed(input::KEY_A, input::KEYBOARD_ID))
	{
		Engine::instance()->syncronize();
			//std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;



	}












	// Cierre limpio del engine
	Engine::release();
}
