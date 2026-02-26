/*
 * @file
 * @brief
 */
#pragma once
#include <cstdint>
#include <EngineAPI.h>
#include 

class Timing
{
public:
	Timing() = default;
	~Timing() = default;
	/*
	*
	*/
	static uint32_t deltaTime()
	{
		EngineAPI::instance()->getPlatform();
			return 0;
	}
};