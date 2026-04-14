#pragma once
#include <cstdint>
#include "CommonEnums.h"

using ComponentID = uint32_t;

enum class CollisionType {
    TriggerEnter,
    TriggerExit,
    CollisionEnter,
    CollisionExit
};

struct PhysicsEvent {
    ComponentID a;
    ComponentID b;
    CollisionType type;
};

struct ShapeRenderData
{
    ShapeType type;

    core::Vector3<> position;
    core::Quaternion<> rotation;

    // BOX
    core::Vector3<> size = { 0,0,0 };

    // CAPSULE / SPHERE
    float radius = 0.0f;
    float halfHeight = 0.0f; // 0 = sphere
};