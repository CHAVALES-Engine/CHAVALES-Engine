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
#include "EngineAPI.h"

#include "Maths.h"
#include <unordered_set>

namespace core
{
	/**
	* @brief Estructura Timer.
	*/
	class Timer
	{
	public:
		/**
		* @brief Devuelve el id del timer.
		*
		* @return uint64_t - Tiempo restante en milisegundos.
		*/
		const uint64_t id() const { return _id; }
		/**
		* @brief Devuelve si el timer esta pausado.
		*
		* @return bopl - Timer pausado.
		*/
		const bool isPaused() const { return _paused; }
		/**
		* @brief Devuelve el tiempo restante.
		*
		* @return uint64_t - Tiempo restante en milisegundos.
		*/
		const uint64_t timeLeftMS()
		{
			if (!_paused)
			{
				uint64_t now = Clock::getRunningTime();
				_leftTime = (now < _endTime) ? (_endTime - now) : 0;
			}
			return _leftTime;
		}
		/**
		* @brief Devuelve el tiempo restante.
		*
		* @return double_t - Tiempo restante en segundos.
		*/
		const double timeLeftSec()
		{
			if (!_paused)
			{
				uint64_t now = Clock::getRunningTime();
				_leftTime = (now < _endTime) ? (_endTime - now) : 0;
			}
			return static_cast<double>(timeLeftMS()) / 1000.0;
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
		 * @brief Si esta activo el timer
		 * 
		 * @return bool - True si el timer corre y aun no ha terminado.
		 */
		bool isActive() const
		{
			return !_paused && !isEnded();
		}
		//
		/** @brief Solo para uso interno del manager al reasignar IDs. */
		void setID(uint64_t id)
		{
			_id = id;
		}
		/**
		* @brief Ejecuta la funcion 
		*/
		void executeFunc() const
		{
			if (!isEnded() && _func)
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
		friend class TimerManager;
		// Constructor privado para que el usuario no pueda instanciar timers.
		Timer() = default;
		Timer(uint64_t id, uint64_t initTime, uint64_t duration, std::function<void()> func) :
			_id(id),
			_initTime(initTime),
			_endTime(initTime + duration),
			_leftTime(duration),
			_func(func)
		{
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
				Debug::error("Timer ", _id, " is already paused.");
				return false;
			}
			uint64_t now = Clock::getRunningTime();
			_leftTime = (now < _endTime) ? (_endTime - now) : 0;
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
				Debug::error("Timer ", _id, " isn't paused.");
				return false;
			}
			_endTime = Clock::getRunningTime() + _leftTime;
			_paused = false;
			return true;
		}
		/**
		* @brief Devuelve el momento real en el que va a terminar el timer.
		*
		* @return uint_t - UINT64_MAX si pausada, si no _endTime;
		*/
		uint64_t effectiveEndTime() const
		{
			return _paused ? UINT64_MAX : _endTime;
		}
		/**
		 * @brief Id del timer.
		 */
		uint64_t _id = -1;
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
	class ENGINE_API TimerManager
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
		*``` 
		*	Timer t = TimerManager::instance().createTimer(5.0, []{ doSomething(); });
		*```
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
		/**
		 * @brief Cancela y elimina un timer antes de que venza.
		 *
		 * @param timer  Timer a cancelar.
		 * @return false si no se encontró.
		 */
		bool cancelTimer(Timer& timer);
	private:

		TimerManager();
		/**
		 * @brief busca una id libre.
		 *
		 * @return int - id libre.
		 */
		uint64_t allocId();
		/**
		 * @brief Anyade una id a la lista de ids liberadas listas para usar.
		 */
		void freeId(uint64_t id);
		/**
		 * @brief Comprueba si el ID tiene un timer activo en el heap.
		 * priority() lanza std::domain_error si el elemento no esta insertado.
		 */
		bool isAlive(int id) const;

		static constexpr int CAPACITY = 1000;

		// Min-heap indexado: clave = int ID, prioridad = Timer (por _endTime)
		IndexPQ<Timer>          _timers{ CAPACITY };

		// IDs disponibles para reutilizar al crear nuevos timers
		std::vector<uint64_t>        _freeIds;

		// IDs de timers cancelados pendientes de descarte en update()
		std::unordered_set<uint64_t> _cancelled;
	};
}
