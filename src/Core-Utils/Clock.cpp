#include "Clock.h"

void core::Clock::setDeltaTime(uint64_t dt)
{
	deltaTime = dt;
}

uint64_t core::Clock::calculateDeltaTime(std::chrono::high_resolution_clock::time_point sT)
{
	return (std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now().time_since_epoch() - sT.time_since_epoch())).count();
}

std::chrono::high_resolution_clock::time_point core::Clock::getNow()
{
	return std::chrono::high_resolution_clock::now();
}

uint64_t core::Clock::getRunningTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch() - _startTime.time_since_epoch()).count();
}


std::chrono::high_resolution_clock::time_point core::Clock::_startTime = std::chrono::high_resolution_clock::now();
