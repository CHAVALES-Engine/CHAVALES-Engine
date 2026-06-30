/**
 * @file
 * @brief
 */

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

		/**
		 * @brief Obtiene la instancia unica del TimerManager.
		 * @return TimerManager& Referencia a la instancia global.
		 */
		static TimerManager& instance();

		/**
		 * @brief Inicializa el singleton del TimerManager.
		 */
		static void init();

		/**
		 * @brief Libera la instancia global del TimerManager.
		 */
		static void release();

		// Eliminar copia y movimiento
		TimerManager(const TimerManager&) = delete;
		TimerManager& operator=(const TimerManager&) = delete;
		TimerManager(TimerManager&&) = delete;
		TimerManager& operator=(TimerManager&&) = delete;

		/**
		 * @brief Actualiza la cola ejecutando timers expirados
		 */
		void update();

		/**
		 * @brief Crea un nuevo timer.
		 *
		 * @param duration -> duracion del timer en segundos
		 * @param func -> funcion ejecutada al acabar.
		 *
		 * @return Timer Timer creado.
		 */
		Timer createTimer(double_t duration, std::function<void()> func);

		/**
		 * @brief Pausa un timer.
		 * @param timer -> timer a pausar.
		 * @return bool -> true si se pudo pausar.
		 */
		bool pauseTimer(Timer& timer);

		/**
		 * @brief Reanuda un timer.
		 *
		 * @param timer ->timer a reanudar.
		 *
		 * @return bool -> true si se pudo reanudar.
		 */
		bool resumeTimer(Timer& timer);

		/**
		 * @brief Detiene un timer.
		 *
		 * @param timer -> timer a detener.
		 *
		 * @return bool -> true si se detuvo correctamente.
		 */
		bool stopTimer(Timer& timer);

	private:

		TimerManager();

		~TimerManager() = default;

		/**
		 * @brief Cantidad max de timers simultaneos.
		 */
		inline static constexpr int QUANTITY = 1000;

		/**
		 * @brief Vector de IDs libres reutilizables.
		 */
		std::vector<uint64_t> _freeIDs;

		/**
		 * @brief Cola de prioridad de timers activos.
		 */
		inline static IndexPQ<Timer> _timers{ QUANTITY };

		/**
		 * @brief Instancia unica del singleton
		 */
		inline static TimerManager* _instance = nullptr;
	};
}