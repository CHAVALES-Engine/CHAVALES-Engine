#pragma once

#include <PxPhysicsAPI.h>

using namespace physx;

class Raycast
{
private:
    PxScene* scene;

public:
    Raycast(PxScene* sceneRef);

    bool Cast(
        const PxVec3& origin,
        const PxVec3& direction,
        float maxDistance,
        PxRaycastBuffer& hitInfo
    );
};

