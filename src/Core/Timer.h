/*
 * @file
 * @brief
 */

#pragma once

#include <cstdint>

#include <IndexPQ.h>
#include <Clock.h>
#include <functional>
#include <Debug.h>

namespace core
{
	/**
	* @brief Estructura Timer.
	*/
	class Timer
	{
	private:
		/**
		 *
		 */
		uint64_t _id = 0;
		/**
		* @brief Tiempo en el que empezo el timer.
		*/
		uint64_t _initTime = 0;
		/**
		* @brief Tiempo en el que acabara el timer.
		*/
		uint64_t _endTime = 0;
		/**
		* @brief
		*/
		uint64_t _leftTime = 0;
		/*
		 * @brief Indica si esta pausado.
		 */
		bool _paused = false;
		/**
		* @brief Puntero a funcion lladado al acabar el timer.
		*/
		std::function<void()> _func = nullptr;
	public:
		Timer() = default;
		Timer(uint64_t id, uint64_t initTime, uint64_t duration, std::function<void()> func) :
			_id(id), _initTime(initTime), _endTime(initTime + duration), _leftTime(duration), _func(func)
		{
		}
		const bool getPaused(const bool paused)
		{
			_paused = paused;
			return true;
		}
		const uint64_t getID() const
		{
			return _id;
		}
		void setID(uint64_t id) 
		{
			_id = id;
		}
		const bool setPaused(const bool paused) 
		{
			// Si esta pausado y lo queremos pausar => Error
			if (_paused && paused) {
				Debug::error("Timer: ", _id, " is already paused.");
				return false;
			}
			if (paused)
			{
				uint64_t now = Clock::getRunningTime();
				_leftTime = _endTime - now;
			}

			_paused = paused;
			return true;
		}
		/**
		* @brief Devuelve si ha acabado el Timer.
		*
		* @return bool - Devuelve si ha acabado el Timer.
		*/
		const bool isEnded() const
		{
			return !_paused && _endTime < Clock::getRunningTime();
		}
		/**
		* @brief Devuelve el tiempo restante.
		*
		* @return uint64_t - Tiempo restante en milisegundos.
		*/
		const uint64_t timeLeftMS() 
		{
			uint64_t now = Clock::getRunningTime();
			if (!_paused)
				_leftTime = _endTime - now;
			return _leftTime;
		}
		const bool isPaused()
		{
			return _paused;
		}
		/**
		* @brief Devuelve el tiempo restante.
		*
		* @return double_t - Tiempo restante en segundos.
		*/
		const double_t timeLeftSec() const
		{
			uint64_t now = Clock::getRunningTime();
			return (_endTime - now) / 1000;
		}
		const void executeFunc() const
		{
			_func();
		}
		/**
		* @brief Sobrecarga del operador '<'.
		*
		* @return bool - True si this es menor que el other.
		*/
		bool operator<(const Timer other) const
		{
			return !_paused && _endTime < other._endTime;
		}
	};


	/**
	* @brief Clase manejadora de Timers.
	*/
	class TimerManager
	{
	public:
		/**
		* @brief Actualiza la cola de timers ejecutando los que hayan llegado al final.
		*/
		static void update()
		{
			uint64_t now = Clock::getRunningTime();
			_privUpdate(now);
		}

		/**
		* @brief Crea un timer nuveo y lo mete a la cola.
		*
		* @param duration - Duracion del timer en segundos.
		* @param funcptr - Puntero a funcion ejecutada al acabar el timer.
		*
		* @return Timer - Devuelve el timer creado. Vacio si no se ha creado bien.
		*/
		static Timer createTimer(double_t duration, std::function<void()> func)
		{
			uint64_t now = Clock::getRunningTime();
			uint64_t end = now + (duration / 1000);

			if (now < 0 || end < now || func == nullptr)
			{
				Debug::error(Debug::DebugMode::DEBUG_BOTH, "Timer con duracion: ", duration, " no creado correactamente.");
			}
			_curId++;
			Timer t(_curId, now, end, func);
			_timers.push(_curId, t);

			return t;
		}

		static bool pauseTimer(Timer& timer)
		{
			timer.setPaused(true);
			_timers.update(timer.getID(), timer);
		}
		static bool resumeTimer(Timer& timer)
		{

		}
	private:
		inline static int _curId= 0;
		/**
		* @brief Actualiza la cola de timers ejecutando los que hayan llegado al final. Privado.
		*/
		static void _privUpdate(uint64_t now)
		{
			Timer t = _timers.top().prioridad;
			if (t.timeLeftMS() <= 0)
			{
				t.executeFunc();
				_timers.pop();
				_privUpdate(now);
			}
		}

		/**
		* @brief Cola de timers.
		*/
		inline static IndexPQ<Timer> _timers{ 1000 };
	};
}
