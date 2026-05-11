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
	staticFriction = 0.6f;
	dynamicFriction = 0.6f;
	restitution = 0.0f;

	frictionCombine = CombineMode::Av;
	bounceCombine = CombineMode::Av;
}

PhysicsMaterial::PhysicsMaterial(float staticF, float dynamicF, float rest,
	CombineMode frictionMode, CombineMode bounceMode)
{
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
	//_eng = Engine::instance();

	if (staticFriction < 0.0f) staticFriction = 0.0f;
	if (dynamicFriction < 0.0f) dynamicFriction = 0.0f;
	if (restitution < 0.0f) restitution = 0.0f;
	if (restitution > 1.0f) restitution = 1.0f;
	if (!entity) return;

	auto collider = entity->getComponent<Collider>();
	if (!collider) return;
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
	//if (_eng != nullptr)
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
