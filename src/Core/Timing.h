/*
 * @file
 * @brief
 */
#pragma once
#include <cstdint>
#include <chrono>

namespace core
{
	class Timing
	{
	public:
		Timing() = default;
		~Timing() = default;

		/*
		* @brief
		*	Tiempo desde la ultima actualizacion.
		*/
		static inline uint64_t deltaTime = 0;

		/*
		* @brief
		*	Tasa de frames por ms.
		*/
		static const uint64_t FRAME_RATE = 1000 / 60; // 1000 ms / 60 frames

		static inline void setDeltaTime(uint64_t dt)
		{
			deltaTime = dt;
		}

		static inline uint64_t calculateDeltaTime(std::chrono::high_resolution_clock::time_point sT)
		{
			return (std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::high_resolution_clock::now().time_since_epoch() - sT.time_since_epoch())).count();
		} 

		static inline std::chrono::high_resolution_clock::time_point getNow()
		{
			return std::chrono::high_resolution_clock::now();
		}

		static inline uint64_t getRunningTime()
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch() - _startTime.time_since_epoch()).count();
		}

	private:
		static std::chrono::high_resolution_clock::time_point _startTime;
	};
}
