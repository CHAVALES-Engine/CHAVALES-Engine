/*
 * @file main.cpp
 * @brief Main file for initialization.
 */
#include <iostream>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

int main(int argc, char* argv[]) {
	sol::state lua;
	lua.script_file("variables.lua");
	// the type "sol::state" behaves 
	// exactly like a table!
	bool isfullscreen = lua["config"]["fullscreen"]; // can get nested variables
	sol::table config = lua["config"];
	assert(!isfullscreen);
	return 0;
}
