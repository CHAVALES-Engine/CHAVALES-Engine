#pragma once
#include <algorithm>
#include "../../src/Core-Defs/Component.h"

class Engine;

class PhysicsMaterial : public core::Component
{
public:

	enum class CombineMode {
		Av,
		Min,
		Max,
		Mult
	};

	float staticFriction = 0.6f;
	float dynamicFriction = 0.6f;
	float restitution = 0.0f;

	CombineMode frictionCombine = CombineMode::Av;
	CombineMode bounceCombine = CombineMode::Av;

	// PhysX
	Engine* _eng = nullptr;
	uint32_t physicsMaterialID = 0;

	PhysicsMaterial();
	PhysicsMaterial(float staticF, float dynamicF, float rest,
		CombineMode frictionMode = CombineMode::Av,
		CombineMode bounceMode = CombineMode::Av);

	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t dt) override;

	static float Combine(float a, float b, CombineMode mode);

	static float ComputeDynamicFriction(const PhysicsMaterial& a, const PhysicsMaterial& b);
	static float ComputeStaticFriction(const PhysicsMaterial& a, const PhysicsMaterial& b);
	static float ComputeRestitution(const PhysicsMaterial& a, const PhysicsMaterial& b);
};