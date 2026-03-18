// PhysicModule.cpp : Defines the functions for the static library.
//
//#include "../../dependencies/PhysX/physx/include/PxPhysicsAPI.h"
#include "PhysicsModule.h"
#include <PxPhysicsAPI.h>

#include "../../src/ComponentsProject/BoxCollider.h"
#include "../../src/ComponentsProject/CapsuleCollider.h"

using namespace physx;


void fnPhysicModule()
{
}

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxPvd* gPvd = NULL;

PxMaterial* defaultMaterial = nullptr;

PhysicsModule::PhysicsModule()
{
}

bool PhysicsModule::Init()
{
	gFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION,
		gAllocator,
		gErrorCallback
	);

	if (!gFoundation)
		return false;

	gPvd = PxCreatePvd(*gFoundation);

	if (gPvd)
	{
		PxPvdTransport* transport =
			PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

		if (transport)
			gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	}

	PxTolerancesScale scale;

	gPhysics = PxCreatePhysics(
		PX_PHYSICS_VERSION,
		*gFoundation,
		scale,
		true,
		gPvd
	);

	PxInitExtensions(*gPhysics, gPvd);
	defaultMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	return gPhysics != nullptr;
}

void PhysicsModule::AddCollider(Collider* col)
{
	if (!col) return;
	colliders.push_back(col);
	CreatePhysXShape(col);
}

void PhysicsModule::CreatePhysXShape(Collider* col)
{
	if (!col || !gPhysics) return;

	PxRigidActor* actor = nullptr;

	if (col->GetRigidbody())
	{
		actor = gPhysics->createRigidDynamic(PxTransform(
			PxVec3(col->GetRigidbody()->getPosition().getX(),
				col->GetRigidbody()->getPosition().getY(),
				col->GetRigidbody()->getPosition().getZ())));
	}
	else
	{
		actor = gPhysics->createRigidStatic(PxTransform(
			PxVec3(col->GetWorldPosition().getX(),
				col->GetWorldPosition().getY(),
				col->GetWorldPosition().getZ())));
	}

	PxShape* shape = nullptr;

	//tipo collider
	if (auto box = dynamic_cast<BoxCollider*>(col)) {
		PxBoxGeometry geo(box->GetSize().getX() * 0.5f,
			box->GetSize().getY() * 0.5f,
			box->GetSize().getZ() * 0.5f);
		shape = gPhysics->createShape(geo, *defaultMaterial);
	}
	else if (auto capsule = dynamic_cast<CapsuleCollider*>(col))
	{
		PxCapsuleGeometry geo(capsule->GetRadius(), capsule->GetHeight() * 0.5f);
		shape = gPhysics->createShape(geo, *defaultMaterial);
	}

	if (shape){
		actor->attachShape(*shape);
		col->SetShape(shape);
		col->SetActor(actor);

		// activar flags de trigger/enable
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, col->IsEnabled() && !col->IsTrigger());
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, col->IsEnabled() && col->IsTrigger());
	}
}
