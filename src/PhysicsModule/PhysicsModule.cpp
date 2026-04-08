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

PxScene* gScene = NULL;

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

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	PxDefaultCpuDispatcher* dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher;

	//colisiones
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	//crear escena
	gScene = gPhysics->createScene(sceneDesc);

	if (!gScene)
		return false;

	return gPhysics != nullptr;
}


ComponentID PhysicsModule::CreateBoxShape(core::Vector3<> size, core::Vector3<> position, bool isDynamic, bool isKinematic)
{//a lo mejor tengo que pasar si es rigidbody dinamico o estatico para ver cual creo de physx
	if (!gPhysics || !gScene) return 0;

	PxTransform transform(PxVec3(position.getX(), position.getY(), position.getZ()));

	PxRigidActor* actor = isDynamic ? static_cast<PxRigidActor*>(gPhysics->createRigidDynamic(transform)) :
		static_cast<PxRigidActor*>(gPhysics->createRigidStatic(transform));

	if (isDynamic && isKinematic)//lo hago kinematico
		static_cast<PxRigidDynamic*>(actor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	PxBoxGeometry geo(size.getX() * 0.5f, size.getY() * 0.5f, size.getZ() * 0.5f);

	PxShape* shape = gPhysics->createShape(geo, *defaultMaterial);
	actor->attachShape(*shape);

	if (isDynamic)
		PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), 10.0f);

	gScene->addActor(*actor);

	//id y guarda en mapa
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


core::Vector3<> PhysicsModule::GetPhysicsPosition(ComponentID id)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return { 0,0,0 };

	PxTransform t = it->second.actor->getGlobalPose();

	return core::Vector3<>(t.p.x,t.p.y,t.p.z);
}


ComponentID PhysicsModule::CreateCapsuleShape(float radius,float height, const core::Vector3<>& center, const core::Vector3<>& worldPos,bool isDynamic, bool isKinematic){
	if (!gPhysics || !gScene)
		return 0;

	//transform ini
	PxTransform transform(PxVec3(worldPos.getX(), worldPos.getY(), worldPos.getZ()));

	//estatico o dinamico
	PxRigidActor* actor = isDynamic ? static_cast<PxRigidActor*>(gPhysics->createRigidDynamic(transform)) :
		static_cast<PxRigidActor*>(gPhysics->createRigidStatic(transform));

	if (isDynamic && isKinematic)
		static_cast<PxRigidDynamic*>(actor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	//capsula si tienen mismo radio y altura es esfera
	PxCapsuleGeometry geo(radius, height * 0.5f);
	PxShape* shape = gPhysics->createShape(geo, *defaultMaterial);

	actor->attachShape(*shape);

	//masa si es dinamico
	if (isDynamic && !isKinematic) PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), 10.f);

	//anadido a escena
	gScene->addActor(*actor);

	//id
	ComponentID id = nextID++;
	physicsMap[id] = { actor, shape };

	return id;
}
void PhysicsModule::Update(float dt)
{
	if (!gScene) return;

	gScene->simulate(dt);
	gScene->fetchResults(true);

	// sincronizar con Ogre
	for (auto& [id, comp] : physicsMap)
	{
		PxTransform t = comp.actor->getGlobalPose();

		//deberia actualizar y luego llamara a esto con engine paa mover los transforms o cambiarlos fuera con ogre
	}
}