#include "PhysicsMaterial.h"
#include "PluginSDK.h"

REGISTER_COMPONENT(PhysicsMaterial);

PhysicsMaterial::PhysicsMaterial() {
	staticFriction = 0.6f;
	dynamicFriction = 0.6f;
	restitution = 0.0f;

	frictionCombine = CombineMode::Av;
	bounceCombine = CombineMode::Av;
}

/**
* @brief Constructor para inicializar todos los valores segun el usuario.
*/
PhysicsMaterial::PhysicsMaterial(float staticF, float dynamicF, float rest, CombineMode frictionMode, CombineMode bounceMode) {
	staticFriction = staticF;
	dynamicFriction = dynamicF;
	restitution = rest;

	frictionCombine = frictionMode;
	bounceCombine = bounceMode;
}

bool PhysicsMaterial::init(const Properties& p)
{
	staticFriction = getProperty<float>(p, "staticFriction");
	dynamicFriction = getProperty<float>(p, "dynamicFriction");
	restitution = getProperty<float>(p, "restitution");

	frictionCombine = static_cast<CombineMode>(getProperty<int>(p, "frictionCombine"));
	bounceCombine = static_cast<CombineMode>(getProperty<int>(p, "bounceCombine"));

	return true;
}

void PhysicsMaterial::ready()
{
	if (staticFriction < 0.0f) staticFriction = 0.0f;
	if (dynamicFriction < 0.0f) dynamicFriction = 0.0f;
	if (restitution < 0.0f) restitution = 0.0f;
	if (restitution > 1.0f) restitution = 1.0f;
}

void PhysicsMaterial::update(uint64_t dt)
{
}

/**
* @brief Combina la cantidad de friccion y rebote segun el modo.
* @return La friccion combinada.
*/
float PhysicsMaterial::Combine(float a, float b, CombineMode mode) {
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
float PhysicsMaterial::ComputeDynamicFriction(const PhysicsMaterial& a, const PhysicsMaterial& b) {
	return Combine(a.dynamicFriction, b.dynamicFriction, a.frictionCombine);
}

/**
* @brief Computa la friccion estática.
* @return La combinacion estática de friccion.
*/
float PhysicsMaterial::ComputeStaticFriction(const PhysicsMaterial& a, const PhysicsMaterial& b) {
	return Combine(a.staticFriction, b.staticFriction, a.frictionCombine);
}

/**
* @brief Computa la restitución.
* @return La combinacion de restitución.
*/
float PhysicsMaterial::ComputeRestitution(const PhysicsMaterial& a, const PhysicsMaterial& b) {
	return Combine(a.restitution, b.restitution, a.bounceCombine);
}