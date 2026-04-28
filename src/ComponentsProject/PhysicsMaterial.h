#pragma once
#include <algorithm>
#include "Component.h"

class Engine;

/**
 * @class PhysicsMaterial
 * @brief Define las propiedades físicas de un material en colisiones.
 *
 * Controla fricción (estática y dinámica) y rebote (restitution),
 * así como cómo se combinan estos valores entre dos materiales.
 */
class PhysicsMaterial : public core::Component
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
	 * @brief Modos de combinación entre dos materiales
	 */
	enum class CombineMode {
		Av,   // Promedio
		Min,  // Mínimo
		Max,  // Máximo
		Mult  // Multiplicación
	};

	/**
	 * @brief Fricción estática (resistencia inicial al movimiento)
	 */
	float staticFriction = 0.6f;

	/**
	 * @brief Fricción dinámica (resistencia en movimiento)
	 */
	float dynamicFriction = 0.6f;

	/**
	 * @brief Restitución (rebote, 0 = sin rebote, 1 = rebote perfecto)
	 */
	float restitution = 0.0f;

	/**
	 * @brief Modo de combinación de fricción entre materiales
	 */
	CombineMode frictionCombine = CombineMode::Av;

	/**
	 * @brief Modo de combinación del rebote
	 */
	CombineMode bounceCombine = CombineMode::Av;

	/**
	 * @brief Referencia al engine
	 */
	Engine* _eng = nullptr;

	/**
	 * @brief ID del material en el sistema de físicas
	 */
	uint32_t physicsMaterialID = 0;

	PhysicsMaterial();

	/**
	 * @brief Constructor con parámetros físicos
	 */
	PhysicsMaterial(float staticF, float dynamicF, float rest,
		CombineMode frictionMode = CombineMode::Av,
		CombineMode bounceMode = CombineMode::Av);

	bool init(const Properties& p) override;

	/**
	 * @brief Llamado cuando el material está listo
	 */
	virtual void ready() override;

	/**
	 * @brief Actualización por frame (normalmente innecesaria en materiales)
	 */
	virtual void update(uint64_t dt) override;

	void destroy() override;

	/**
	 * @brief Combina dos valores según el modo especificado
	 */
	static float Combine(float a, float b, CombineMode mode);

	/**
	 * @brief Calcula fricción dinámica combinada entre dos materiales
	 */
	static float ComputeDynamicFriction(const PhysicsMaterial& a, const PhysicsMaterial& b);

	/**
	 * @brief Calcula fricción estática combinada entre dos materiales
	 */
	static float ComputeStaticFriction(const PhysicsMaterial& a, const PhysicsMaterial& b);

	/**
	 * @brief Calcula restitución combinada entre dos materiales
	 */
	static float ComputeRestitution(const PhysicsMaterial& a, const PhysicsMaterial& b);
};