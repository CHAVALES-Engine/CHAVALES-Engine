#pragma once
#include <algorithm>

class PhysicsMaterial
{
public:
    enum class CombineMode
    {
        Av,
        Min,
        Max,
        Mult
    };

    float staticFriction;

    float dynamicFriction;

    float restitution;

    CombineMode frictionCombine;

    CombineMode bounceCombine;

    PhysicsMaterial()
    {
        staticFriction = 0.6f;
        dynamicFriction = 0.6f;
        restitution = 0.0f;

        frictionCombine = CombineMode::Av;
        bounceCombine = CombineMode::Av;
    }

    PhysicsMaterial(
        float staticF,
        float dynamicF,
        float rest,
        CombineMode frictionMode = CombineMode::Av,
        CombineMode bounceMode = CombineMode::Av
    )
    {
        staticFriction = staticF;
        dynamicFriction = dynamicF;
        restitution = rest;

        frictionCombine = frictionMode;
        bounceCombine = bounceMode;
    }

    static float Combine(float a, float b, CombineMode mode)
    {
        switch (mode)
        {
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

    static float ComputeDynamicFriction(
        const PhysicsMaterial& a,
        const PhysicsMaterial& b
    )
    {
        return Combine(
            a.dynamicFriction,
            b.dynamicFriction,
            a.frictionCombine
        );
    }

    static float ComputeStaticFriction(
        const PhysicsMaterial& a,
        const PhysicsMaterial& b
    )
    {
        return Combine(
            a.staticFriction,
            b.staticFriction,
            a.frictionCombine
        );
    }

    static float ComputeRestitution(
        const PhysicsMaterial& a,
        const PhysicsMaterial& b
    )
    {
        return Combine(
            a.restitution,
            b.restitution,
            a.bounceCombine
        );
    }
};