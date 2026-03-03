#include "Clock.h"

std::chrono::high_resolution_clock::time_point core::Clock::_startTime = std::chrono::high_resolution_clock::now();