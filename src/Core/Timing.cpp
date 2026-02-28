#include "Timing.h"

std::chrono::high_resolution_clock::time_point core::Timing::_startTime = std::chrono::high_resolution_clock::now();