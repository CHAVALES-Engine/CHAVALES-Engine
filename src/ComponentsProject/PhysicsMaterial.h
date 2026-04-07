#pragma once
#include <algorithm>
#include "../../src/Core-Defs/Component.h"

class PhysicsMaterial : public core::Component
{
public:
	//VARIABLES
	/**
	* @brief Enumerador con los distintos tipos de intensidad respecto a la combinación.
	*/
	enum class CombineMode {
		Av,
		Min,
		Max,
		Mult
	};

	/**
	* @brief Variable de fricción estática.
	*/
	float staticFriction;

	/**
	* @brief Variable de fricción dinámica.
	*/
	float dynamicFriction;

	/**
	* @brief Variable de resitución.
	*/
	float restitution;

	/**
	* @brief Variable del tipo enumerador para la combinacion de fricción.
	*/
	CombineMode frictionCombine;

	/**
	* @brief Variable del tipo enumerador para la combinacion de rebote.
	*/
	CombineMode bounceCombine;

	/**
	* @brief Constructor predeterminado.
	*/

	//METODOS
	PhysicsMaterial();

	/**
	* @brief Constructor para inicializar todos los valores segun el usuario.
	*/
	PhysicsMaterial(float staticF, float dynamicF, float rest, CombineMode frictionMode = CombineMode::Av, CombineMode bounceMode = CombineMode::Av);

	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t dt) override;

	/**
	* @brief Combina la cantidad de friccion y rebote segun el modo.
	* @return La friccion combinada.
	*/
	static float Combine(float a, float b, CombineMode mode);

	/**
	* @brief Computa la friccion dinámica.
	* @return La combinacion dinámica de friccion.
	*/
	static float ComputeDynamicFriction(const PhysicsMaterial& a, const PhysicsMaterial& b);

	/**
	* @brief Computa la friccion estática.
	* @return La combinacion estática de friccion.
	*/
	static float ComputeStaticFriction(const PhysicsMaterial& a, const PhysicsMaterial& b);

	/**
	* @brief Computa la restitución.
	* @return La combinacion de restitución.
	*/
	static float ComputeRestitution(const PhysicsMaterial& a, const PhysicsMaterial& b);
};