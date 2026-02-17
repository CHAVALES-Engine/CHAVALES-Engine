/*
 * @file EngineAPI.cpp
 * @brief Defines the functions for the EngineAPI static library.
 */
#include "pch.h"
#include "framework.h"
#include "EngineAPI.h"

#include "PlatformModule.h"


// TODO: This is an example of a library function
void fnEngineAPI()
{
}

bool EngineAPI::init()
{
	_instance = new EngineAPI();

	if (_instance == nullptr) return false;

	return _instance->_initPriv();
}

EngineAPI* EngineAPI::instance()
{
	return _instance;
}

void EngineAPI::release()
{
	delete _instance->_platformModule;

	delete _instance;
}

EngineAPI::EngineAPI()
{

}

EngineAPI::~EngineAPI()
{

}

bool EngineAPI::_initPriv()
{

	_platformModule = new PlatformModule();

	if (!_platformModule->Init())
	{
		delete _platformModule;
		return false;
	}
	return true;
}
