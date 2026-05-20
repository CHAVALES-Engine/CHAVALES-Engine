/**
 * @file
 * @brief
 */
#pragma once
#include <cstdint>
#include <chrono>
#include "EngineAPI.h"
namespace core
{
	 /**
	 * @brief Reloj.
	 *
	 *	Clase que gestiona el tiempo en la aplicacion.
	 */
	class ENGINE_API Clock
	{
	public:
		Clock() = default;
		~Clock() = default;

		/** @brief Tiempo desde la ultima actualizacion.
		*/
		static inline uint64_t deltaTime = 0;

		/**
		* @brief Tasa de frames por milisegundo. 60 FPS.
		*/
		static constexpr uint64_t FRAME_RATE = 1000 / 60; // 1000 ms / 60 frames

		/**
		 * brief Numero máximo de pasos fijos por frame
		 */
		static constexpr int MAX_FIXED_STEPS = 4;


		/** @brief Actualiza deltaTime.
		*	@param dt - deltaTime.
		*/
		static inline void setDeltaTime(uint64_t dt);

		/** @brief Calcula el valor de deltaTime desde un tiempo inicial dado.
		*	@param sT - Tiempo desde el que calcular el deltaTime.
		*/
		static inline uint64_t calculateDeltaTime(std::chrono::high_resolution_clock::time_point sT);

		/** @brief Calcula el valor de deltaTime desde un tiempo inicial dado.
		*	@param start - momento inicial en ms.
		*	@param end - momento final en ms.
		*/
		static inline uint64_t calculateDeltaTime(
			std::chrono::high_resolution_clock::time_point start,
			std::chrono::high_resolution_clock::time_point end);

		/** @brief Tiempo actual.
		*/
		static inline std::chrono::high_resolution_clock::time_point getNow();

		/** @brief Tiempo que lleva el programa en ejecucion.
		*/
		static inline uint64_t getRunningTime();

	private:
		/** @brief Tiempo en el que comienza el programa.
		*/
		static std::chrono::high_resolution_clock::time_point _startTime;
	};
}
