#include "TimeManager.h"
#include "checkMLNew.h"

void core::TimerManager::update()
{

	//Debug::out("TIMER UPDATE SIZE: ", _timers.size() );
	while (!_timers.empty())
	{
		Timer t = _timers.top().prioridad;
		//Debug::warning("Update timer: ", t.id(), " - ", t.timeLeftMS());

		if (t.timeLeftMS() > 0) break;

		Debug::warning("Timer: ", t.id(), " ended");
		t.executeFunc();
		_timers.pop();
	}

}

core::Timer core::TimerManager::createTimer(double_t duration, std::function<void()> func)
{
	if (duration <= 0.0)
	{
		Debug::error("TimerManager::createTimer — invalid duration: ", duration);
		return {};
	}
	if (func == nullptr)
	{
		Debug::error("TimerManager::createTimer — func is nullptr");
		return {};
	}
	const uint64_t now = Clock::getRunningTime();
	const uint64_t durationMS = (duration * 1000);
	const uint64_t id = _freeIDs.back();
	_freeIDs.pop_back();

	Timer t(id, now, durationMS, func);
	_timers.push(id, t);

	Debug::warning("Timer: ", id, " created, duration: ", duration, "s");

	return t;
	//return std::shared_ptr<Timer>(std::move(t));
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
	// reserva una cantidad maxima de timers
	_freeIDs.reserve(QUANTITY);
	// rellena los freeids con la cantidad maxima
	for (int i = 0; i < QUANTITY; ++i)
		_freeIDs.push_back(QUANTITY - i);
}
