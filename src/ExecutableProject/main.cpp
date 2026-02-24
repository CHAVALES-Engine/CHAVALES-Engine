/*
 * @file main.cpp
 * @brief Main file for initialization.
 */
#include <iostream>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <stdio.h>
#include <stdlib.h>
#include "EngineAPI.h"
#include <Debug.h>

using namespace chavalescore;

int main(int argc, char* argv[]) {

	EngineAPI::init();

	// Ejemplos de debug:
	_Out("Hola soy un ejemplo de mensaje normal con un int: " << 23 << "\nY una salto de linea.");
	_Error("Hola soy un ejemplo de error.");
	_Warnig("Hola soy un ejemplo de Warning");

	//sol::state lua;
	//lua.script_file("variables.lua");
	//// the type "sol::state" behaves 
	//// exactly like a table!
	//bool isfullscreen = lua["config"]["fullscreen"]; // can get nested variables
	//sol::table config = lua["config"];
	//assert(!isfullscreen)

	return 0;
}
