#include "Timer.h"

void core::TimerManager::update()
{

	//Debug::out("TIMER UPDATE SIZE: ", _timers.size() );
	while (!_timers.empty())
	{
		// si el top no ha terminado salimos
		if (!_timers.top().prioridad.isEnded())
			break;

		const int   id = _timers.top().elem;
		const Timer t = _timers.top().prioridad;
		const bool  cancelled = _cancelled.count(id) > 0;

		_timers.pop();
		freeId(id);

		Debug::warning("Timer ", t.id(), " ended");
		// si ha sido cancelado eliminamos de la lista de cancelados
		if (cancelled)
			_cancelled.erase(id);
		else
			t.executeFunc();
	}

}

core::Timer core::TimerManager::createTimer(double_t duration, std::function<void()> func)
{
	Debug::out("timer..................");
	if (_freeIds.empty())
	{
		Debug::error("TimerManager: max capacity reached (", CAPACITY, ").");
		assert(false && "TimerManager: sin IDs libres.");
		return {};
	}
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
	const int      id = allocId();

	Timer t(id, now, durationMS, func);
	_timers.push(id, t);
	return t;
}

bool core::TimerManager::pauseTimer(Timer& timer)
{
	const uint64_t id = timer.id();
	if (!isAlive(id))
	{
		Debug::error("Timer ", id," is dead.");
		return false;
	}

	bool ok = timer.pause();
	if (ok) _timers.update(timer.id(), timer);
	return ok;
}

bool core::TimerManager::resumeTimer(Timer& timer)
{
	const uint64_t id = timer.id();
	if (!isAlive(id))
	{
		Debug::error("Timer ", id, " is dead.");
		return false;
	}

	bool ok = timer.resume();
	if (ok) _timers.update(id, timer);
	return ok;
}

bool core::TimerManager::cancelTimer(Timer& timer)
{
	const int id = timer.id();
	if (!isAlive(id))
	{
		Debug::error("Timer ", id, " is dead.");
		return false;
	}

	_cancelled.insert(id);
	timer = {}; // invalida la copia que viene, id -1.
	return true;
}

core::TimerManager::TimerManager()
{
	// Rellenar la lista libre con todos los IDs disponibles [0, CAPACITY)
	_freeIds.reserve(CAPACITY);
	for (int i = CAPACITY - 1; i >= 0; --i)
		_freeIds.push_back(i);
}

uint64_t core::TimerManager::allocId()
{
	const uint64_t id = _freeIds.back();
	// Quita la ultima libre.
	_freeIds.pop_back();
	return id;
}

void core::TimerManager::freeId(uint64_t id)
{
	// La anyade al final
	_freeIds.push_back(id);
}

bool core::TimerManager::isAlive(int id) const
{
	if (id < 0 || id >= CAPACITY) return false;
	try { _timers.priority(id); return true; }
	catch (...) { return false; }
}
