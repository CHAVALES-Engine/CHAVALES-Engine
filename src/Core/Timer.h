/*
 * @file
 * @brief
 */

#pragma once

#include <cstdint>
#include <queue>
#include <chrono>

/**
*
* @brief TODO.
*
*/
struct Timer
{
	uint64_t initTime = 0;
	uint64_t endTime = 0;
	// callback;
	void (*func)() = nullptr;

	const bool isStopped() const 
	{
		// TODO: coger el timeStamp de Time
		return endTime < 0;
	}

	const uint64_t timeLeftMS() const
	{
		// TODO: coger el timeStamp de Time
		uint64_t now = 0;
		return endTime - now;
	}

	const double timeLeftSec() const
	{
		// TODO: coger el timeStamp de Time
		uint64_t now = 0;
		return (endTime - now) / 1000;
	}

	bool operator<(const Timer other) const 
	{ 
		return endTime < other.endTime; 
	}
};

/**
*
* @brief TODO.
*
*/
class TimerManager
{
public:
	/**
	* @brief TODO.
	*
	*/
	static void update()
	{
		// TODO: coger el timeStamp de Time
		uint64_t now = 0;
		_privUpdate(now);
	}

	/**
	* @brief TODO.
	*
	*/
	static Timer createTimer(double_t duration, void (*funcptr)())
	{
		// TODO: coger el timeStamp de Time
		uint64_t now = 0;
		uint64_t end = now + (duration / 1000);
		
		if (now < 0 || end < now || funcptr == nullptr)
			return ;
		Timer t = { now, end, funcptr };
		_timers.push(t);

		return t;
	}

private:

	/**
	* @brief TODO.
	*
	*/
	static void _privUpdate(uint64_t now)
	{
		Timer t = _timers.top();
		if (t.endTime <= now)
		{
			t.func();
			_timers.pop();
			_privUpdate(now);
		}
		else
			return;
	}

	/**
	* @brief TODO.
	*
	*/
	static std::priority_queue<Timer> _timers;
};
