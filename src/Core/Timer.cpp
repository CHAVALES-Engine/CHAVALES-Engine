#include "Timer.h"

void core::TimerManager::update()
{

	//Debug::out("TIMER UPDATE SIZE: ", _timers.size() );
	while (!_timers.empty())
	{
		Timer t = _timers.top().prioridad;
		Debug::warning("Update timer: ", t.id(), " - ", t.timeLeftMS());

		if (t.timeLeftMS() > 0) break;

		Debug::warning("Timer ", t.id(), " ended");
		t.executeFunc();
		_timers.pop();
	}

}

core::Timer core::TimerManager::createTimer(double_t duration, std::function<void()> func)
{

	Debug::out("timer..................");

	uint64_t now = Clock::getRunningTime();
	uint64_t durationMS = (duration * 1000);
	if (now < 0 || durationMS || func == nullptr)
	{
		Debug::error("Timer con duracion: ", duration, " no creado correactamente.");
	}
	_curId++;
	Timer t(_curId, now, durationMS, func);
	_timers.push(_curId, t);
	Debug::warning("Timer creado: ", durationMS, "s");

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