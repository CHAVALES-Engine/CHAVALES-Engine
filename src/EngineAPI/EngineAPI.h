#pragma once
/*
 * @file EngineAPI.cpp
 * @brief Defines the functions for the EngineAPI static library.
 */
class PlatformModule;

class EngineAPI
{
public:

	static bool init();

	static EngineAPI* instance();
	
	static void release();

private:

	EngineAPI();
	~EngineAPI();

	bool _initPriv();

	static EngineAPI* _instance;

	PlatformModule* _platformModule = nullptr;

};
