#include "PhysicsModule.h"
//#include <PxPhysicsAPI.h>

using namespace physx;

struct PhysXComponent
{
	PxRigidActor* actor = nullptr;
	PxShape* shape = nullptr;
};

void fnPhysicModule() {}

PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;

PxFoundation* gFoundation = nullptr;
PxPhysics* gPhysics = nullptr;
PxPvd* gPvd = nullptr;

PxScene* gScene = nullptr;
PxMaterial* defaultMaterial = nullptr;

std::unordered_map<ComponentID, PxMaterial*> materialMap;

static PxCombineMode::Enum ToPxCombine(int mode)
{
	switch (mode)
	{
	case 0: return PxCombineMode::eAVERAGE;
	case 1: return PxCombineMode::eMIN;
	case 2: return PxCombineMode::eMAX;
	case 3: return PxCombineMode::eMULTIPLY;
	}
	return PxCombineMode::eAVERAGE;
}

static PxFilterFlags CustomFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	//si alguno es trigger
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	//colision normal
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eDETECT_DISCRETE_CONTACT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
	
	return PxFilterFlag::eDEFAULT;
}

PhysicsModule::PhysicsModule() {}

bool PhysicsModule::Init()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation) return false;
	gPvd = PxCreatePvd(*gFoundation);
	if (gPvd)
	{
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		if (transport)
			gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	}
	PxTolerancesScale scale;
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, scale, true, gPvd);
	PxInitExtensions(*gPhysics, gPvd);
	defaultMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	PxDefaultCpuDispatcher* dispatcher = PxDefaultCpuDispatcherCreate(2);

	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
	sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
	//sceneDesc.flags |= PxSceneFlag::eREQUIRE_RW_LOCK;
	
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = CustomFilterShader;
	sceneDesc.simulationEventCallback = this;

	gScene = gPhysics->createScene(sceneDesc);

	if (!gScene)
		return false;

	return gPhysics != nullptr;
}

ComponentID PhysicsModule::CreateRigidBody(core::Vector3<> pos, float mass, bool useGravity)
{
	if (!gPhysics || !gScene) return 0;
	PxTransform transform(PxVec3(pos.getX(), pos.getY(), pos.getZ()));
	PxRigidDynamic* body = gPhysics->createRigidDynamic(transform);
	if (!body) return 0;
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *defaultMaterial);
	body->attachShape(*shape);
	PxRigidBodyExt::setMassAndUpdateInertia(*body, mass > 0.0f ? mass : 1.0f);
	body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !useGravity);
	gScene->addActor(*body);

	//id del actor del rigidbody
	ComponentID id = nextID++;
	physicsMap[id] = { body, nullptr };
	actorToID[body] = id;

	return id;
}

ComponentID PhysicsModule::CreateBoxShape(core::Vector3<> size, core::Vector3<> position, bool isDynamic, bool isKinematic, bool isTrigger)
{
	if (!gPhysics || !gScene) return 0;

	PxTransform transform(PxVec3(position.getX(), position.getY(), position.getZ()));
	PxRigidActor* actor = isDynamic ? static_cast<PxRigidActor*>(gPhysics->createRigidDynamic(transform)) : static_cast<PxRigidActor*>(gPhysics->createRigidStatic(transform));
	if (!actor) return 0;

	//kinematic
	if (isDynamic && isKinematic)
		static_cast<PxRigidDynamic*>(actor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	PxBoxGeometry geo(size.getX() * 0.5f, size.getY() * 0.5f, size.getZ() * 0.5f);

	PxShape* shape = gPhysics->createShape(geo, *defaultMaterial);
	if (!shape) return 0;

	PxFilterData filterData;
	filterData.word0 = 1;//layer
	filterData.word1 = 1;//layer

	shape->setSimulationFilterData(filterData);
	if (isTrigger)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	else
	{
		shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	}

	actor->attachShape(*shape);
	
	//masa si es dinamic
	if (isDynamic && !isKinematic) PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), 10.0f);

	gScene->addActor(*actor);
	ComponentID id = nextID++;
	physicsMap[id] = { actor, shape };
	actorToID[actor] = id;
	
	return id;
}

ComponentID PhysicsModule::CreateCapsuleShape(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, bool isDynamic, bool isKinematic, bool isTrigger)
{
	if (!gPhysics || !gScene) return 0;

	PxTransform transform(PxVec3(worldPos.getX(), worldPos.getY(), worldPos.getZ()));
	PxRigidActor* actor = isDynamic ? static_cast<PxRigidActor*>(gPhysics->createRigidDynamic(transform)) : static_cast<PxRigidActor*>(gPhysics->createRigidStatic(transform));
	if (!actor) return 0;
	if (isDynamic && isKinematic) static_cast<PxRigidDynamic*>(actor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	PxCapsuleGeometry geo(radius, height * 0.5f);
	PxShape* shape = gPhysics->createShape(geo, *defaultMaterial);
	if (!shape) return 0;
	PxFilterData filterData;
	filterData.word0 = 1;//layer
	filterData.word1 = 1;//layer

	shape->setSimulationFilterData(filterData);
	if (isTrigger)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	else
	{
		shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	}

	PxTransform localPose(PxVec3(center.getX(), center.getY(), center.getZ()));
	shape->setLocalPose(localPose);
	actor->attachShape(*shape);

	if (isDynamic && !isKinematic)
		PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), 10.f);
	gScene->addActor(*actor);
	ComponentID id = nextID++;
	physicsMap[id] = { actor, shape };
	actorToID[actor] = id;
	return id;
}

std::vector<PhysicsEvent> PhysicsModule::getEventsFor(ComponentID id)
{
	std::vector<PhysicsEvent> result;

	for (auto& e : eventQueue)
	{
		if (e.a == id || e.b == id)
			result.push_back(e);
	}

	return result;
}

void PhysicsModule::clearEvents()
{
	eventQueue.clear();
}

void PhysicsModule::SetPhysicsPosition(ComponentID id, const core::Vector3<>& pos)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	it->second.actor->setGlobalPose(PxTransform(PxVec3(pos.getX(), pos.getY(), pos.getZ())));
}

core::Vector3<> PhysicsModule::GetPhysicsPosition(uint32_t id)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return { 0,0,0 };
	PxTransform t = it->second.actor->getGlobalPose();
	return core::Vector3<>(t.p.x, t.p.y, t.p.z);
}

core::Vector3<> PhysicsModule::GetLinearVelocity(uint32_t id)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return { 0,0,0 };
	PxRigidDynamic* body = it->second.actor->is<PxRigidDynamic>();
	if (!body) return { 0,0,0 };
	PxVec3 vel = body->getLinearVelocity();
	return core::Vector3<>(vel.x, vel.y, vel.z);
}

void PhysicsModule::SetLinearVelocity(uint32_t id, core::Vector3<> vel)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidDynamic* body = it->second.actor->is<PxRigidDynamic>();
	if (!body) return;
	body->setLinearVelocity(PxVec3(vel.getX(), vel.getY(), vel.getZ()));
}

void PhysicsModule::AddForce(uint32_t id, core::Vector3<> force)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidDynamic* body = it->second.actor->is<PxRigidDynamic>();
	if (!body) return;
	body->addForce(PxVec3(force.getX(), force.getY(), force.getZ()), PxForceMode::eFORCE);
}

void PhysicsModule::AddImpulse(uint32_t id, core::Vector3<> impulse)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidDynamic* body = it->second.actor->is<PxRigidDynamic>();
	if (!body) return;
	body->addForce(PxVec3(impulse.getX(), impulse.getY(), impulse.getZ()), PxForceMode::eIMPULSE);
}

uint32_t PhysicsModule::CreateMaterial(float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine)
{
	if (!gPhysics) return 0;
	PxMaterial* mat = gPhysics->createMaterial(staticF, dynamicF, restitution);
	if (!mat) return 0;
	mat->setFrictionCombineMode(ToPxCombine(frictionCombine));
	mat->setRestitutionCombineMode(ToPxCombine(bounceCombine));
	//usa tus propios ids
	ComponentID id = nextIDMaterial++;
	materialMap[id] = mat;
	return id;
}

void PhysicsModule::UpdateMaterial(uint32_t id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine)
{
	auto it = materialMap.find(id);
	if (it == materialMap.end()) return;
	PxMaterial* mat = it->second;
	if (!mat) return;
	mat->setStaticFriction(staticF);
	mat->setDynamicFriction(dynamicF);
	mat->setRestitution(restitution);
	mat->setFrictionCombineMode(ToPxCombine(frictionCombine));
	mat->setRestitutionCombineMode(ToPxCombine(bounceCombine));
}

void PhysicsModule::Update(float dt)
{
	if (!gScene) return;

	//gScene->flushSimulation();
	gScene->simulate(dt);
	gScene->fetchResults(true);
	/*for (auto& [id, comp] : physicsMap)
	{
		PxTransform t = comp.actor->getGlobalPose();
	}*/
}

void PhysicsModule::onTrigger(PxTriggerPair* pairs, PxU32 count) {
	for (physx::PxU32 i = 0; i < count; i++)
	{
		auto* triggerActor = (physx::PxRigidActor*)pairs[i].triggerActor;
		auto* otherActor = (physx::PxRigidActor*)pairs[i].otherActor;
		if (!triggerActor || !otherActor) continue;//compruebo

		auto itA = actorToID.find(triggerActor);
		auto itB = actorToID.find(otherActor);
		if (itA == actorToID.end() || itB == actorToID.end())//comprubeo
			continue;

		ComponentID a = itA->second;
		ComponentID b = itB->second;

		if (pairs[i].status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			eventQueue.push_back({ a, b, CollisionType::TriggerEnter });
		if (pairs[i].status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			eventQueue.push_back({ a, b, CollisionType::TriggerExit });
	}
}

void PhysicsModule::AttachBoxShape(ComponentID bodyID, const core::Vector3<> size, const core::Vector3<>& center)
{
	auto it = physicsMap.find(bodyID);
	if (it == physicsMap.end()) return;

	PxRigidActor* actor = it->second.actor;
	if (!actor) return;

	PxShape* shape = gPhysics->createShape(PxBoxGeometry(size.getX() * 0.5f, size.getY() * 0.5f, size.getZ() * 0.5f),*defaultMaterial);
	if (!shape) return;
	PxTransform localPose(PxVec3(center.getX(), center.getY(), center.getZ()));
	shape->setLocalPose(localPose);

	actor->attachShape(*shape);
	it->second.shape = shape;
}

void PhysicsModule::AttachCapsuleShape(ComponentID bodyID, float radius, float height, const core::Vector3<>& center)
{
	auto it = physicsMap.find(bodyID);
	if (it == physicsMap.end()) return;

	PxRigidActor* actor = it->second.actor;
	if (!actor) return;

	PxCapsuleGeometry geo(radius, height * 0.5f);
	PxShape* shape = gPhysics->createShape(geo, *defaultMaterial);
	if (!shape) return;
	PxTransform localPose(PxVec3(center.getX(), center.getY(), center.getZ()));
	shape->setLocalPose(localPose);

	actor->attachShape(*shape);

	it->second.shape = shape;
}

void PhysicsModule::setPhysicsTransform(ComponentID id, const core::Vector3<>& pos, const core::Quaternion<>& rot)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidActor* actor = it->second.actor;
	if (!actor) return;
	PxTransform t(PxVec3(pos.getX(), pos.getY(), pos.getZ()),PxQuat(rot.getX(), rot.getY(), rot.getZ(), rot.getW()));
	actor->setGlobalPose(t);
}