/**
 * @file
 * @brief
 */

#pragma once
#include "EngineComponent.h"
#include <EngineAPI.h>

class Engine;

/**
 * @class PhysicsMaterial
 * @brief Define las propiedades fisicas de un material en colisiones.
 *
 * Controla friccion (estatica y dinamica) y rebote (restitution),
 * asi como como se combinan estos valores entre dos materiales.
 */
class ENGINE_API PhysicsMaterial : public EngineComponent
{
/*
 * +-----------------+
 * | PHYSICSMATERIAL |
 * +-----------------+
 *
 * --- Ejemplo de uso en lua ---
 *		PhysicsMaterial = {
 *			staticFriction = 10.0,
 *			dynamicFriction = 10.0,
 *			restitution = 10.0,
 *			frictionCombine = 0/1/2/3,
 *			bounceCombine = 0/1/2/3
 *		}
*/
public:

	/**
	 * @brief Modos de combinacion entre dos materiales
	 */
	enum class CombineMode {
		Av,   // Promedio
		Min,  // Mínimo
		Max,  // Máximo
		Mult  // Multiplicación
	};

	/**
	 * @brief Friccion estatica (resistencia inicial al movimiento)
	 */
	float staticFriction = 0.6f;

	/**
	 * @brief Friccion dinamica (resistencia en movimiento)
	 */
	float dynamicFriction = 0.6f;

	/**
	 * @brief Restitucion (rebote, 0 = sin rebote, 1 = rebote perfecto)
	 */
	float restitution = 0.0f;

	/**
	 * @brief Modo de combinacion de friccion entre materiales
	 */
	CombineMode frictionCombine = CombineMode::Max;

	/**
	 * @brief Modo de combinacion del rebote
	 */
	CombineMode bounceCombine = CombineMode::Max;

	/**
	 * @brief ID del material en el sistema de fisicas
	 */
	uint32_t physicsMaterialID = 0;
	uint32_t physicsShapeID = 0;
	
	PhysicsMaterial();

	/**
	 * @brief Constructor con parametros fisicos
	 */
	PhysicsMaterial(float staticF, float dynamicF, float rest,
		CombineMode frictionMode = CombineMode::Max,
		CombineMode bounceMode = CombineMode::Max);

	bool init(const Properties& p) override;

	/**
	 * @brief Llamado cuando el material esta listo
	 */
	virtual void ready() override;

	/**
	 * @brief Actualizacion por frame (normalmente innecesaria en materiales)
	 */
	virtual void update(uint64_t dt) override;

	void destroy() override;

	/**
	 * @brief Combina dos valores segun el modo especificado
	 */
	static float Combine(float a, float b, CombineMode mode);

	/**
	 * @brief Calcula friccion dinamica combinada entre dos materiales
	 */
	static float ComputeDynamicFriction(const PhysicsMaterial& a, const PhysicsMaterial& b);

	/**
	 * @brief Calcula friccion estatica combinada entre dos materiales
	 */
	static float ComputeStaticFriction(const PhysicsMaterial& a, const PhysicsMaterial& b);

	/**
	 * @brief Calcula restitucion combinada entre dos materiales
	 */
	static float ComputeRestitution(const PhysicsMaterial& a, const PhysicsMaterial& b);
};