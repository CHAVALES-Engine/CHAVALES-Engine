/*
 * @file
 * @brief
 */

#pragma once

#include <cstdint>
#include <queue>
#include <chrono>

#include <Debug.h>

namespace core
{
	/**
	* @brief Estructura Timer.
	*/
	struct Timer
	{
		/**
		* @brief Tiempo en el que empezo el timer.
		*/
		uint64_t initTime = 0;
		/**
		* @brief Tiempo en el que acabara el timer.
		*/
		uint64_t endTime = 0;
		/**
		* @brief Puntero a funcion lladado al acabar el timer.
		*/
		void (*func)() = nullptr;

		/**
		* @brief Devuelve si ha acabado el Timer.
		* 
		* @return bool - Devuelve si ha acabado el Timer.
		*/
		const bool isEnded() const
		{
			// TODO: coger el timeStamp de Time
			return endTime < 0;
		}

		/**
		* @brief Devuelve el tiempo restante.
		*
		* @return uint64_t - Tiempo restante en milisegundos.
		*/
		const uint64_t timeLeftMS() const
		{
			// TODO: coger el timeStamp de Time
			uint64_t now = 0;
			return endTime - now;
		}

		/**
		* @brief Devuelve el tiempo restante.
		*
		* @return double_t - Tiempo restante en segundos.
		*/
		const double_t timeLeftSec() const
		{
			// TODO: coger el timeStamp de Time
			uint64_t now = 0;
			return (endTime - now) / 1000;
		}

		/**
		* @brief Sobrecarga del operador '<'.
		*
		* @return bool - True si this es menor que el other.
		*/
		bool operator<(const Timer other) const
		{
			return endTime < other.endTime;
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
			// TODO: coger el timeStamp de Time
			uint64_t now = 0;
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
		static Timer createTimer(double_t duration, void (*funcptr)())
		{
			// TODO: coger el timeStamp de Time
			uint64_t now = 0;
			uint64_t end = now + (duration / 1000);

			if (now < 0 || end < now || funcptr == nullptr)
			{
				Debug::error(Debug::DebugMode::DEBUG_BOTH, "Timer con duracion: ", duration, " no creado correactamente.");
				return;
			}

			Timer t = { now, end, funcptr };
			_timers.push(t);

			return t;
		}

	private:

		/**
		* @brief Actualiza la cola de timers ejecutando los que hayan llegado al final. Privado.
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
		* @brief Cola de timers.
		*/
		static std::priority_queue<Timer> _timers;
	};
}
