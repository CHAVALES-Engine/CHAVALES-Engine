#pragma once
#include <cstdint>

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