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

#include "Maths.h"

namespace core
{
	/**
	* @brief Estructura Timer.
	*/
	class Timer
	{
	public:
		Timer() = default;
		Timer(uint64_t id, uint64_t initTime, uint64_t duration, std::function<void()> func) :
			_id(id), _initTime(initTime), _endTime(initTime + duration), _leftTime(duration), _func(func)
		{
		}
		const uint64_t id() const { return _id; }
		/**
		* @brief Devuelve el id del timer.
		*
		* @return uint64_t - Tiempo restante en milisegundos.
		*/
		const bool isPaused() const { return _paused; }
		/**
		* @brief Devuelve el tiempo restante.
		*
		* @return uint64_t - Tiempo restante en milisegundos.
		*/
		const uint64_t timeLeftMS()
		{
			uint64_t now = Clock::getRunningTime();
			if (!_paused)
				_leftTime = (now >= _endTime) ? 0 : _endTime - now; // evita underflow
			return Maths::Max<uint64_t>(0, (int64_t)_leftTime);
		}
		/**
		* @brief Devuelve el tiempo restante.
		*
		* @return double_t - Tiempo restante en segundos.
		*/
		const double timeLeftSec() const
		{
			uint64_t now = Clock::getRunningTime();
			return Maths::Max<double>(0, (_endTime - now) / 1000.0);
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
		/** @brief Solo para uso interno del manager al reasignar IDs. */
		void setID(uint64_t id)
		{
			_id = id;
		}
		/**
		 * @brief Pausa el timer guardando el tiempo restante.
		 *
		 * @return bool - false si ya estaba pausado.
		 */
		const bool pause()
		{
			if (_paused)
			{
				Debug::error("Timer ", _id, " ya está pausado.");
				return false;
			}
			_leftTime = timeLeftMS();
			_paused = true;
			return true;
		}
		/**
		* @brief Reanuda el timer recalculando _endTime a partir del tiempo restante.
		* @return bool - false si no estaba pausado.
		*/
		bool resume()
		{
			if (!_paused)
			{
				Debug::error("Timer ", _id, " no está pausado.");
				return false;
			}
			_endTime = Clock::getRunningTime() + _leftTime;
			_paused = false;
			return true;
		}
		void executeFunc() const
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
	private:
		/**
		 * @brief Id del timer.
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
		* @brief Tiempo restante.
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
	};


	/**
	* @brief Clase manejadora de Timers.
	*/
	class TimerManager
	{
	public:
		static TimerManager& instance()
		{
			static TimerManager instance; // Se crea la primera vez, destruye al cerrar
			return instance;
		}

		// Eliminar copia y movimiento
		TimerManager(const TimerManager&) = delete;
		TimerManager& operator=(const TimerManager&) = delete;
		TimerManager(TimerManager&&) = delete;
		TimerManager& operator=(TimerManager&&) = delete;


		/**
		* @brief Actualiza la cola de timers ejecutando los que hayan llegado al final.
		*/
		void update();
		/**
		* @brief Crea un timer nuveo y lo mete a la cola.
		*
		* @param duration - Duracion del timer en segundos.
		* @param funcptr - Puntero a funcion ejecutada al acabar el timer.
		*
		* @return Timer - Devuelve el timer creado. Vacio si no se ha creado bien.
		*/
		Timer createTimer(double_t duration, std::function<void()> func);
		/**
		* @brief Pausa un timer.
		*
		* @param timer - Timer a pausar.
		*
		* @return bool - si se ha podido pausar.
		*/
		bool pauseTimer(Timer& timer);
		/**
		* @brief Marca un timer para reanudarlo.
		*
		* @param timer - Timer a reanudar.
		*
		* @return bool - si se ha podido reanudar.
		*/
		bool resumeTimer(Timer& timer);
	private:
		TimerManager() = default;

		inline static int _curId = 0;
		/**
		* @brief Cola de timers.
		*/
		inline static IndexPQ<Timer> _timers{ 1000 };
	};
}
