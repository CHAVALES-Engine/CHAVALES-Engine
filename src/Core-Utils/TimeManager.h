#pragma once


#include "EngineAPI.h"
#include "IndexPQ.h"
#include "functional"
#include "Timer.h"


namespace core
{
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
		* @brief Para el timer y lo
		*
		* @param timer - Timer a reanudar.
		*
		* @return bool - si se ha podido reanudar.
		*/
		bool stopTimer(Timer& timer);
	private:
		TimerManager();
		/**
		 * Cantidad maxima de timers a la vez.
		 */
		inline static int QUANTITY = 1000;
		/**
		 * @brief Vector de ids libres para usar con timers.
		 */
		std::vector<uint64_t> _freeIDs;
		/**
		* @brief Cola de timers.
		*/
		inline static IndexPQ<Timer> _timers{ 1000 };
	};
}