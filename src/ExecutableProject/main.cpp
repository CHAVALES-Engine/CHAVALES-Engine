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

int main(int argc, char* argv[]) {
	
	EngineAPI::init();

	
	return 0;
}