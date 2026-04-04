// PhysicModule.cpp : Defines the functions for the static library.
//
//#include "../../dependencies/PhysX/physx/include/PxPhysicsAPI.h"
#include "PhysicsModule.h"
#include <PxPhysicsAPI.h>


using namespace physx;
struct PhysXComponent
{
	PxRigidActor* actor = nullptr;
	PxShape* shape = nullptr;
};

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


ComponentID PhysicsModule::CreateBoxShape(core::Vector3<> size, core::Vector3<> position, bool isDynamic)
{//a lo mejor tengo que pasar si es rigidbody dinamico o estatico para ver cual creo de physx
	if (!gPhysics /*|| !scene*/)
		return 0;

	PxTransform transform(PxVec3(position.getX(), position.getY(), position.getZ()));

	PxRigidActor* actor = isDynamic ? static_cast<PxRigidActor*>(gPhysics->createRigidDynamic(transform)) :
		static_cast<PxRigidActor*>(gPhysics->createRigidStatic(transform));

	PxBoxGeometry geo(size.getX() * 0.5f, size.getY() * 0.5f, size.getZ() * 0.5f);

	PxMaterial* material = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxShape* shape = gPhysics->createShape(geo, *material);
	actor->attachShape(*shape);

	if (isDynamic)
		PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), 10.0f);

	/*scene->addActor(*actor);*/

	// Generar ID y guardar en mapa
	ComponentID id = nextID++;
	physicsMap[id] = { actor, shape };

	return id;

}


void PhysicsModule::SetPhysicsPosition(ComponentID id, const core::Vector3<>& pos)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;

	PxRigidActor* actor = it->second.actor;
	actor->setGlobalPose(PxTransform(PxVec3(pos.getX(), pos.getY(), pos.getZ())));
}