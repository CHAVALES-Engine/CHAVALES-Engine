#include "Raycast.h"
#include "checkMLNew.h"

Raycast::Raycast(PxScene* sceneRef)
{
    scene = sceneRef;
}

bool Raycast::Cast(
    const PxVec3& origin,
    const PxVec3& direction,
    float maxDistance,
    PxRaycastBuffer& hitInfo
)
{
    return scene->raycast(
        origin,
        direction.getNormalized(),
        maxDistance,
        hitInfo
    );
}
