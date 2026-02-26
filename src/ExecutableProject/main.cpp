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

	//EngineAPI::init();

	// Ejemplos de debug:
	Debug::open();
	Debug::out(Debug::DebugMode::DEBUG_BOTH, "Hola soy un ejemplo de mensaje normal con un int: ", 23, "\nY un salto de linea.");
	Debug::out(Debug::DebugMode::DEBUG_BOTH, "Otro ejemplo");
	Debug::error(Debug::DebugMode::DEBUG_BOTH, "Hola soy un ejemplo de error.");
	Debug::warning(Debug::DebugMode::DEBUG_CONS, "Hola soy un ejemplo de Warning");
	Debug::warning(Debug::DebugMode::DEBUG_FILE, "Hola solo voy en archivo");
	Debug::close();

	//sol::state lua;
	//lua.script_file("variables.lua");
	//// the type "sol::state" behaves 
	//// exactly like a table!
	//bool isfullscreen = lua["config"]["fullscreen"]; // can get nested variables
	//sol::table config = lua["config"];
	//assert(!isfullscreen)

	return 0;
}
