/*
 * @file
 * @brief
 */
#pragma once
#include <cstdint>
#include <chrono>

class Timing
{
public:
	Timing() = default;
	~Timing() = default;

	/*
	* @brief
	*	Tiempo desde la ultima actualizacion.
	*/
	static uint64_t deltaTime;

	/*
	* @brief
	*	Tasa de frames por ms.
	*/
	static const uint64_t FRAME_RATE = 1000 / 60; // 1000 ms / 60 frames

	static std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

	static inline uint64_t calculateDeltaTime()
	{
		return (std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::high_resolution_clock::now() - startTime)).count();
	}

	static inline std::chrono::time_point<std::chrono::high_resolution_clock> calculateNow()
	{
		return std::chrono::high_resolution_clock::now();
	}

	static inline void setDeltaTime(uint64_t dt)
	{
		deltaTime = dt;
	}
};