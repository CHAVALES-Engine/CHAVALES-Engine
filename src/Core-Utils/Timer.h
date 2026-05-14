/**
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
	class TimerManager;
	/**
	* @brief Estructura Timer.
	*/
	class Timer
	{
	public:
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
			return _id == -1 || (!_paused && _endTime < Clock::getRunningTime());
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
		Timer() = default;
	private:
		/**
		 * @brief Constructoras privadas para que el desarrollador no pueda instanciarlas.
		 */
		Timer(uint64_t id, uint64_t initTime, uint64_t duration, std::function<void()> func) :
			_id(id), _endTime(initTime + duration), _leftTime(duration), _func(func)
		{
		}
		/**
		 * @brief Pausa el timer guardando el tiempo restante.
		 *
		 * @return bool - false si ya estaba pausado.
		 */
		const bool pause()
		{
			if (_id == -1 || _paused)
			{
				Debug::error("Timer: ", _id, _id == -1 ? " is stopped." : " already paused.");
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
			if (_id == -1 || !_paused)
			{
				Debug::error("Timer: ", _id, _id == -1 ? " is stopped." : " is not paused.");
				return false;
			}
			_endTime = Clock::getRunningTime() + _leftTime;
			_paused = false;
			return true;
		}
		/**
		 * @brief Para un timer.
		 */
		void stop()
		{
			if (_id == -1)
			{
				Debug::error("Timer: already stopped.");
				return;
			}
			_id = -1;
			_func = nullptr;
			_paused = false;
			_endTime = -1;
			_leftTime = -1;
		}
		/**
		 * @brief ejecuta la funcion lambda almacenada
		 */
		void executeFunc() const
		{
			if (_func && !_paused)
				_func();
		}
		/** @brief Solo para uso interno del manager al reasignar IDs. */
		void setID(uint64_t id)
		{
			_id = id;
		}
		/**
		 * @brief Id del timer.
		 */
		uint64_t _id = 0;
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
		friend core::TimerManager;
	};
}
