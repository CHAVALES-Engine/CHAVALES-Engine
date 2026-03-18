#pragma once
#include <algorithm>

class PhysicsMaterial
{
public:
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
	PhysicsMaterial() {
		staticFriction = 0.6f;
		dynamicFriction = 0.6f;
		restitution = 0.0f;

		frictionCombine = CombineMode::Av;
		bounceCombine = CombineMode::Av;
	}

	/**
	* @brief Constructor para inicializar todos los valores segun el usuario.
	*/
	PhysicsMaterial(float staticF, float dynamicF, float rest, CombineMode frictionMode = CombineMode::Av, CombineMode bounceMode = CombineMode::Av) {
		staticFriction = staticF;
		dynamicFriction = dynamicF;
		restitution = rest;

		frictionCombine = frictionMode;
		bounceCombine = bounceMode;
	}

	/**
	* @brief Combina la cantidad de friccion y rebote segun el modo.
	* @return La friccion combinada.
	*/
	static float Combine(float a, float b, CombineMode mode) {
		switch (mode) {
		case CombineMode::Av:
			return (a + b) * 0.5f;

		case CombineMode::Min:
			return std::min(a, b);

		case CombineMode::Max:
			return std::max(a, b);

		case CombineMode::Mult:
			return a * b;
		}

		return (a + b) * 0.5f;
	}

	/**
	* @brief Computa la friccion dinámica.
	* @return La combinacion dinámica de friccion.
	*/
	static float ComputeDynamicFriction(const PhysicsMaterial& a, const PhysicsMaterial& b) {
		return Combine(a.dynamicFriction, b.dynamicFriction, a.frictionCombine);
	}

	/**
	* @brief Computa la friccion estática.
	* @return La combinacion estática de friccion.
	*/
	static float ComputeStaticFriction(const PhysicsMaterial& a, const PhysicsMaterial& b) {
		return Combine(a.staticFriction, b.staticFriction, a.frictionCombine);
	}

	/**
	* @brief Computa la restitución.
	* @return La combinacion de restitución.
	*/
	static float ComputeRestitution(const PhysicsMaterial& a, const PhysicsMaterial& b) {
		return Combine(a.restitution, b.restitution, a.bounceCombine);
	}
};