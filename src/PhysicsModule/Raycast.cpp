#include "Raycast.h"
#include "checkMLNew.h"

#include "PxPhysicsAPI.h"

Raycast::Raycast(physx::PxScene* sceneRef)
{
	scene = sceneRef;
}

bool Raycast::Cast(
	const physx::PxVec3& origin,
	const physx::PxVec3& direction,
	float maxDistance,
	physx::PxRaycastBuffer& hitInfo
)
{
	return scene->raycast(
		origin,
		direction.getNormalized(),
		maxDistance,
		hitInfo
	);
}