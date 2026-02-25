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

	static uint64_t DELTA_TIME;
	static const uint64_t FRAME_RATE = 1000 / 60; // 1000 ms / 60 frames
};