#include "PhysicsMaterial.h"
#include "PluginSDK.h"
#include "Engine.h"
#include "Entity.h"
#include "Collider.h"
#include "PhysicsModule.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(PhysicsMaterial);

PhysicsMaterial::PhysicsMaterial()
{

}

bool PhysicsMaterial::init(const Properties& p)
{
	if (!Component::init(p)) return false;

	int friction, bounce;
	if (!setProperty(p, "frictionCombine", friction)) return false;
	if (!setProperty(p, "bounceCombine", bounce)) return false;
	if (friction < 0 || friction > 4 || bounce < 0 || bounce > 4) {
		Debug::error("[PHYSCSMATERIAL] CombineMode fuera de rango");
		return false;
	}
	frictionCombine = static_cast<CombineMode>(friction);
	bounceCombine = static_cast<CombineMode>(bounce);

	return true;
}

void PhysicsMaterial::ready()
{
	if (staticFriction < 0.0f) staticFriction = 0.0f;
	if (dynamicFriction < 0.0f) dynamicFriction = 0.0f;
	if (restitution < 0.0f) restitution = 0.0f;
	if (restitution > 1.0f) restitution = 1.0f;
	if (!entity) return;

	col = entity->hasComponent<Collider>();
	if (!col) {
		Debug::warning("[PHYSICSMATERIAL] Para que funcione el material la entidad necesita un collider");
		return;
	}
	collider = entity->getComponent<Collider>();


	if (col)
	{
		physicsShapeID = collider->getId();
		physicsMaterialID = physics()->CreateMaterial(
			physicsShapeID,
			staticFriction,
			dynamicFriction,
			restitution,
			static_cast<int>(frictionCombine),
			static_cast<int>(bounceCombine)
		);
	}
}

void PhysicsMaterial::update(uint64_t dt)
{
	if (physicsMaterialID == 0) return;

	physics()->UpdateMaterial(
		physicsMaterialID,
		staticFriction,
		dynamicFriction,
		restitution,
		static_cast<int>(frictionCombine),
		static_cast<int>(bounceCombine)
	);
}

void PhysicsMaterial::destroy()
{
	Component::destroy();
	physics()->DestroyMaterial(physicsMaterialID);
}

float PhysicsMaterial::Combine(float a, float b, CombineMode mode)
{
	switch (mode)
	{
	case CombineMode::Av:
		return (a + b) * 0.5f;

	case CombineMode::Min:
		return min(a, b);

	case CombineMode::Max:
		return max(a, b);

	case CombineMode::Mult:
		return a * b;
	}

	return (a + b) * 0.5f;
}

float PhysicsMaterial::ComputeDynamicFriction(const PhysicsMaterial& a, const PhysicsMaterial& b)
{
	return Combine(a.dynamicFriction, b.dynamicFriction, a.frictionCombine);
}

float PhysicsMaterial::ComputeStaticFriction(const PhysicsMaterial& a, const PhysicsMaterial& b)
{
	return Combine(a.staticFriction, b.staticFriction, a.frictionCombine);
}

float PhysicsMaterial::ComputeRestitution(const PhysicsMaterial& a, const PhysicsMaterial& b)
{
	return Combine(a.restitution, b.restitution, a.bounceCombine);
}