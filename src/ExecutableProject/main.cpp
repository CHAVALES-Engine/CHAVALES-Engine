/*
 * @file main.cpp
 * @brief Main file for initialization.
 */
#include <iostream>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <stdio.h>
#include <stdlib.h>
#include "Engine.h"
#include <Debug.h>

using namespace core;

int main(int argc, char* argv[]) {

	Engine::init();

	// Ejemplos de debug:
	Debug::open();
	Debug::out(Debug::DebugMode::DEBUG_BOTH, "Hola soy un ejemplo de mensaje normal con un int: ", 23, "\nY una salto de linea.");
	Debug::out(Debug::DebugMode::DEBUG_BOTH, "Otro ejemplo");
	Debug::error(Debug::DebugMode::DEBUG_BOTH, "Hola soy un ejemplo de error.");
	Debug::warning(Debug::DebugMode::DEBUG_BOTH, "Hola soy un ejemplo de Warning");

	//sol::state lua;
	//lua.script_file("variables.lua");
	//// the type "sol::state" behaves 
	//// exactly like a table!
	//bool isfullscreen = lua["config"]["fullscreen"]; // can get nested variables
	//sol::table config = lua["config"];
	//assert(!isfullscreen)

	return 0;
}
