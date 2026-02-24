/*
 * @file
 * @brief
 */
#pragma once
#include <cstdint>

class Timing
{
public:
	Timing() = default;
	~Timing() = default;

	static uint32_t deltaTime()
	{
		// TODO: llama a getTicks64 de SDL 
		return 0;
	}
};