#pragma once

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
