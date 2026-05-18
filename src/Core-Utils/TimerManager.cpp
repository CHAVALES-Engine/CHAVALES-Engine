#include "TimerManager.h"
#include "checkMLNew.h"

void core::TimerManager::update()
{
	while (!_timers.empty())
	{
		Timer t = _timers.top().prioridad;

		if (t.timeLeftMS() > 0) break;

		Debug::warning("[TimerManager] Timer: ", t.id(), " ended");
		t.executeFunc();
		_timers.pop();
	}
}

core::Timer core::TimerManager::createTimer(double_t duration, std::function<void()> func)
{
	if (duration <= 0.0)
	{
		Debug::error("[TimerManager] Invalid duration: ", duration);
		return {};
	}
	if (func == nullptr)
	{
		Debug::error("[TimerManager] Func is nullptr");
		return {};
	}
	const uint64_t now = Clock::getRunningTime();
	const uint64_t durationMS = (duration * 1000);
	const uint64_t id = _freeIDs.back();
	_freeIDs.pop_back();

	Timer t(id, now, durationMS, func);
	_timers.push(id, t);

	Debug::warning("[TimerManager] Timer: ", id, " created, duration: ", duration, "s");

	return t;
}

bool core::TimerManager::pauseTimer(Timer& timer)
{
	bool ok = timer.pause();
	if (ok) _timers.update(timer.id(), timer);
	return ok;
}

bool core::TimerManager::resumeTimer(Timer& timer)
{
	bool ok = timer.resume();
	if (ok) _timers.update(timer.id(), timer);
	return ok;
}

bool core::TimerManager::stopTimer(Timer& timer)
{
	_freeIDs.push_back(timer.id());
	timer.stop();
	return true;
}

core::TimerManager::TimerManager()
{
	// Reserva una cantidad maxima de timers.
	_freeIDs.reserve(QUANTITY);
	// Rellena los freeids con la cantidad maxima.
	for (int i = 0; i < QUANTITY; ++i)
		_freeIDs.push_back(QUANTITY - i);
}