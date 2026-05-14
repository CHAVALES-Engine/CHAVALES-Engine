#include "PhysicsModule.h"
#include "PxPhysicsAPI.h"
#include "PxSimulationEventCallback.h"
#include "checkMLNew.h"
#include "Debug.h"
using namespace physx;

struct PhysXComponent
{
	PxRigidActor* actor = nullptr;
	std::vector<PxShape*> shapes;
	bool lockX = false, lockY = false, lockZ = false;
	bool lockAngX = false, lockAngY = false, lockAngZ = false;
};

PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;

PxFoundation* gFoundation = nullptr;
PxPhysics* gPhysics = nullptr;
PxPvd* gPvd = nullptr;

PxScene* gScene = nullptr;
PxMaterial* defaultMaterial = nullptr;
PxDefaultCpuDispatcher* dispatcher = nullptr;
PxPvdTransport* pvdTransport = nullptr;


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
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
		pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
		return PxFilterFlag::eDEFAULT;
	}

	//colision normal
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eDETECT_DISCRETE_CONTACT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eSOLVE_CONTACT;;

	return PxFilterFlag::eDEFAULT;
}

PhysicsModule::PhysicsModule() {}

PhysicsModule::~PhysicsModule()
{

	ClearScene();
	if (gScene) { gScene->release(); gScene = nullptr; }
	if (dispatcher) { dispatcher->release(); dispatcher = nullptr; }
	if (gPhysics) { gPhysics->release(); gPhysics = nullptr; }
	if (pvdTransport) { pvdTransport->release(); pvdTransport = nullptr; }
	if (gPvd) { gPvd->release(); gPvd = nullptr; }
	PxCloseExtensions();
	if (gFoundation) { gFoundation->release(); gFoundation = nullptr; }

	materialMap.clear();
}

bool PhysicsModule::Init()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation) return false;
	gPvd = PxCreatePvd(*gFoundation);
	if (gPvd)
	{
		pvdTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		if (pvdTransport)
			gPvd->connect(*pvdTransport, PxPvdInstrumentationFlag::eALL);
	}
	PxTolerancesScale scale;
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, scale, true, gPvd);
	PxInitExtensions(*gPhysics, gPvd);

	defaultMaterial = gPhysics->createMaterial(0.0f, 0.0f, 0.0f);
	if (!defaultMaterial) return false;
	ComponentID defaultMatID = nextIDMaterial++;
	materialMap[defaultMatID] = defaultMaterial;

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher = PxDefaultCpuDispatcherCreate(2);

	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
	sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;

	sceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;

	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = CustomFilterShader;
	sceneDesc.simulationEventCallback = this;
	Debug::out("[PhysicsModule] Callback asignado");
	gScene = gPhysics->createScene(sceneDesc);

	if (!gScene)
		return false;

	gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	gScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.0f);
	gScene->setGravity(gScene->getGravity() * 100.0f);
	raycast = Raycast(gScene);
	return gPhysics != nullptr;
}

bool PhysicsModule::rayCast(const PxVec3& origin,
	const PxVec3& direction,
	float maxDistance,
	RayInfo& rayInfo)
{
	PxRaycastBuffer hitInfo;
	raycast.Cast(origin, direction, maxDistance, hitInfo);
	if (!hitInfo.hasBlock) return false;
	rayInfo.hitPos = { hitInfo.block.position.x, hitInfo.block.position.y, hitInfo.block.position.z };
	auto it = actorToEntity.find(hitInfo.block.actor);
	rayInfo.otherEntity = it != actorToEntity.end() ? it->second : nullptr;
	return true;
}

ComponentID PhysicsModule::CreateRigidBody(core::Vector3<> pos, float mass, bool useGravity, bool isKinematic)
{
	if (!gPhysics || !gScene) return 0;
	PxTransform transform(PxVec3(pos.getX(), pos.getY(), pos.getZ()));
	PxRigidDynamic* body = gPhysics->createRigidDynamic(transform);
	if (!body) return 0;

	if (isKinematic)body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	else PxRigidBodyExt::setMassAndUpdateInertia(*body, mass > 0.0f ? mass : 1.0f);

	body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !useGravity);

	gScene->addActor(*body);

	//id del actor del rigidbody
	ComponentID id = nextID++;
	physicsMap[id] = new PhysXComponent({ body, {} });
	actorToID[body] = id;

	return id;
}

ComponentID PhysicsModule::CreateBoxShape(core::Vector3<> size, const core::Vector3<>& center, core::Vector3<> position, const core::Quaternion<> rot, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger)
{
	if (!gPhysics || !gScene) return 0;

	PxTransform transform(PxVec3(position.getX(), position.getY(), position.getZ()), PxQuat(rot.getX(), rot.getY(), rot.getZ(), rot.getW()));//posicion y rotacion del trasnform
	PxRigidActor* actor = isDynamic ? static_cast<PxRigidActor*>(gPhysics->createRigidDynamic(transform)) : static_cast<PxRigidActor*>(gPhysics->createRigidStatic(transform));
	if (!actor) return 0;

	PxBoxGeometry geo(size.getX() * 0.5f, size.getY() * 0.5f, size.getZ() * 0.5f);

	PxShape* shape = gPhysics->createShape(geo, *defaultMaterial, true);
	if (!shape) return 0;
	PxQuat qLoc(rotationLoc.getX(), rotationLoc.getY(), rotationLoc.getZ(), rotationLoc.getW());
	PxTransform localPose(PxVec3(center.getX(), center.getY(), center.getZ()), qLoc);//pos y rot locales
	shape->setLocalPose(localPose);

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

	gScene->addActor(*actor);
	ComponentID id = nextID++;
	physicsMap[id] = new PhysXComponent{ actor, {shape} };
	actorToID[actor] = id;
	return id;
}

ComponentID PhysicsModule::CreateCapsuleShape(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, const core::Quaternion<> rot, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger)
{
	if (!gPhysics || !gScene) return 0;

	PxTransform transform(PxVec3(worldPos.getX(), worldPos.getY(), worldPos.getZ()), PxQuat(rot.getX(), rot.getY(), rot.getZ(), rot.getW()));//pos y rot del trasnform de entity
	PxRigidActor* actor = isDynamic ? static_cast<PxRigidActor*>(gPhysics->createRigidDynamic(transform)) : static_cast<PxRigidActor*>(gPhysics->createRigidStatic(transform));
	if (!actor) return 0;

	PxShape* shape = nullptr;

	if (height <= 0.0f)//Esfera
	{
		PxSphereGeometry geo(radius);
		shape = gPhysics->createShape(geo, *defaultMaterial, true);
	}
	else//capsula
	{
		float halfHeight = (height * 0.5f) - radius;
		halfHeight = std::max(0.0f, halfHeight);
		PxCapsuleGeometry geo(radius, halfHeight);
		shape = gPhysics->createShape(geo, *defaultMaterial, true);
	}
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

	//rotadas en el eje Y para que se vean verticales
	PxQuat baseRot(PxHalfPi, PxVec3(0, 0, 1));
	PxQuat qLoc(rotationLoc.getX(), rotationLoc.getY(), rotationLoc.getZ(), rotationLoc.getW());//local
	PxQuat finalRot = baseRot * qLoc;//alineada eje Y + rot local collider

	PxTransform localPose(PxVec3(center.getX(), center.getY(), center.getZ()), finalRot);
	shape->setLocalPose(localPose);
	actor->attachShape(*shape);

	gScene->addActor(*actor);
	ComponentID id = nextID++;
	physicsMap[id] = new PhysXComponent{ actor, {shape} };
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

void PhysicsModule::setActorEntity(ComponentID physicsID, core::Entity* entity)
{
	auto it = physicsMap.find(physicsID);
	if (it != physicsMap.end())
		actorToEntity[it->second->actor] = entity;
}

void PhysicsModule::SetPhysicsPosition(ComponentID id, const core::Vector3<>& pos)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	auto body = it->second->actor->is<PxRigidDynamic>();
	if (body->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC)
		body->setKinematicTarget(PxTransform(pos.getX(), pos.getY(), pos.getZ(), it->second->actor->is<PxRigidDynamic>()->getGlobalPose().q));
	else
		it->second->actor->setGlobalPose(PxTransform(PxVec3(pos.getX(), pos.getY(), pos.getZ())));
}

void PhysicsModule::SetPhysicsRotation(ComponentID id, const core::Quaternion<>& rot)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxTransform rotation = it->second->actor->getGlobalPose();
	rotation.q = PxQuat(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
	it->second->actor->setGlobalPose(rotation);
}

core::Vector3<> PhysicsModule::GetPhysicsPosition(uint32_t id)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return { 0,0,0 };
	PxTransform t = it->second->actor->getGlobalPose();
	return core::Vector3<>(t.p.x, t.p.y, t.p.z);
}

core::Quaternion<> PhysicsModule::GetPhysicsRotation(uint32_t id)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return { 0,0,0,0 };
	PxTransform t = it->second->actor->getGlobalPose();
	return core::Quaternion<>(t.q.x, t.q.y, t.q.z, t.q.w);
}

core::Vector3<> PhysicsModule::GetLinearVelocity(uint32_t id)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return { 0,0,0 };
	PxRigidDynamic* body = it->second->actor->is<PxRigidDynamic>();
	if (!body) return { 0,0,0 };
	PxVec3 vel = body->getLinearVelocity();
	return core::Vector3<>(vel.x, vel.y, vel.z);
}

void PhysicsModule::SetLinearVelocity(uint32_t id, core::Vector3<> vel)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidDynamic* body = it->second->actor->is<PxRigidDynamic>();
	if (!body) return;
	body->setLinearVelocity(PxVec3(vel.getX(), vel.getY(), vel.getZ()));
}

void PhysicsModule::AddForce(uint32_t id, core::Vector3<> force, char mode)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidDynamic* body = it->second->actor->is<PxRigidDynamic>();
	if (!body) return;
	switch (mode) {
	case 'F':
		body->addForce(PxVec3(force.getX(), force.getY(), force.getZ()), PxForceMode::eFORCE);
		break;
	case 'I':
		body->addForce(PxVec3(force.getX(), force.getY(), force.getZ()), PxForceMode::eIMPULSE);
		break;
	case 'A':
		body->addForce(PxVec3(force.getX(), force.getY(), force.getZ()), PxForceMode::eACCELERATION);
		break;
	case 'V':
		body->addForce(PxVec3(force.getX(), force.getY(), force.getZ()), PxForceMode::eVELOCITY_CHANGE);
		break;
	}
}

void PhysicsModule::ClearForce(uint32_t id, char mode)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidDynamic* body = it->second->actor->is<PxRigidDynamic>();
	if (!body) return;
	switch (mode) {
	case 'F':
		body->clearForce(PxForceMode::eFORCE);
		break;
	case 'I':
		body->clearForce(PxForceMode::eIMPULSE);
		break;
	case 'A':
		body->clearForce(PxForceMode::eACCELERATION);
		break;
	case 'V':
		body->clearForce(PxForceMode::eVELOCITY_CHANGE);
		break;
	}
}

void PhysicsModule::BlockAxes(uint32_t id, bool x, bool y, bool z)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidDynamic* body = it->second->actor->is<PxRigidDynamic>();
	if (!body) return;
	it->second->lockX = x;
	it->second->lockY = y;
	it->second->lockZ = z;
	body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, x);
	body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, y);
	body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, z);
}

void PhysicsModule::BlockAngles(uint32_t id, bool x, bool y, bool z)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidDynamic* body = it->second->actor->is<PxRigidDynamic>();
	if (!body) return;
	it->second->lockAngX = x;
	it->second->lockAngY = y;
	it->second->lockAngZ = z;
	body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, x);
	body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, y);
	body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, z);
}

float PhysicsModule::GetMass(uint32_t id)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return 0.0f;
	PxRigidDynamic* body = it->second->actor->is<PxRigidDynamic>();
	if (!body) return 0.0f;
	return body->getMass();
}

void PhysicsModule::SetMass(uint32_t id, float mass)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidDynamic* body = it->second->actor->is<PxRigidDynamic>();
	if (!body) return;
	body->setMass(PxReal(mass));
}

float PhysicsModule::GetLinearDamping(uint32_t id)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return 0.0f;
	PxRigidDynamic* body = it->second->actor->is<PxRigidDynamic>();
	if (!body) return 0.0f;
	return body->getLinearDamping();
}

void PhysicsModule::SetLinearDamping(uint32_t id, float damping)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidDynamic* body = it->second->actor->is<PxRigidDynamic>();
	if (!body) return;
	body->setLinearDamping(PxReal(damping));
}

void PhysicsModule::SetGravity(core::Vector3<> gravity)
{
	if (!gScene) return;
	gScene->setGravity(PxVec3(gravity.getX(), gravity.getY(), gravity.getZ()));
}

uint32_t PhysicsModule::CreateMaterial(ComponentID id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine)
{
	if (!gPhysics) return 0;
	PxMaterial* mat = gPhysics->createMaterial(staticF, dynamicF, restitution);
	if (!mat) return 0;
	mat->setFrictionCombineMode(ToPxCombine(frictionCombine));
	mat->setRestitutionCombineMode(ToPxCombine(bounceCombine));
	//usa tus propios ids
	ComponentID cid = nextIDMaterial++;
	materialMap[cid] = mat;
	// Aplicar directamente al actor si existe
	auto actorIt = physicsMap.find(id);
	if (actorIt != physicsMap.end())
		for (PxShape* shape : actorIt->second->shapes)
			if (shape) shape->setMaterials(&mat, 1);
	return cid;
}

void PhysicsModule::UpdateMaterial(uint32_t id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine)
{
	auto it = materialMap.find(id);
	if (it == materialMap.end()) return;
	auto it1 = physicsMap.find(id);
	if (it1 == physicsMap.end()) return;
	PxMaterial* mat = it->second;
	PxShape* shape = it1->second->actor->is<PxShape>();
	if (!mat) return;
	mat->setStaticFriction(staticF);
	mat->setDynamicFriction(dynamicF);
	mat->setRestitution(restitution);
	mat->setFrictionCombineMode(ToPxCombine(frictionCombine));
	mat->setRestitutionCombineMode(ToPxCombine(bounceCombine));
	if (!shape) return;
	shape->setMaterials(&mat, 1);
}

void PhysicsModule::fixedUpdate(float dt)
{
	if (!gScene) return;

	gScene->simulate(dt / 1000.0f);
	gScene->fetchResults(true);
	// Reaplica los lock flags despues de la simulacion
	// Esto previene que PhysX ignore los locks durante calculos de colisiones
	for (auto& [id, component] : physicsMap) {
		PxRigidDynamic* body = component->actor->is<PxRigidDynamic>();
		if (!body) continue;
		// Reaplica locks lineales
		if (component->lockX || component->lockY || component->lockZ)
		{
			body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, component->lockX);
			body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, component->lockY);
			body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, component->lockZ);
		}
		// Reaplica locks angulares
		if (component->lockAngX || component->lockAngY || component->lockAngZ)
		{
			body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, component->lockAngX);
			body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, component->lockAngY);
			body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, component->lockAngZ);
		}
	}
}

void PhysicsModule::onTrigger(PxTriggerPair* pairs, PxU32 count) {
	for (physx::PxU32 i = 0; i < count; i++)
	{
		auto* triggerActor = (physx::PxRigidActor*)pairs[i].triggerActor;
		auto* otherActor = (physx::PxRigidActor*)pairs[i].otherActor;

		if (!triggerActor || !otherActor) continue;

		auto itA = actorToID.find(triggerActor);
		auto itB = actorToID.find(otherActor);

		auto itEntityA = actorToEntity.find(triggerActor);
		auto itEntityB = actorToEntity.find(otherActor);

		if (itA == actorToID.end() || itB == actorToID.end()) continue;
		if (itEntityA == actorToEntity.end() || itEntityB == actorToEntity.end()) continue;

		ComponentID a = itA->second;
		ComponentID b = itB->second;

		core::Entity* entityA = itEntityA->second;
		core::Entity* entityB = itEntityB->second;

		//ENTER
		if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			eventQueue.push_back({ a, b, CollisionType::TriggerEnter, entityB });
			eventQueue.push_back({ b, a, CollisionType::TriggerEnter, entityA });
		}

		//EXIT
		if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			eventQueue.push_back({ a, b, CollisionType::TriggerExit, entityB });
			eventQueue.push_back({ b, a, CollisionType::TriggerExit, entityA });
		}
	}
}

void PhysicsModule::AttachBoxShape(ComponentID bodyID, const core::Vector3<> size, const core::Vector3<>& center, const core::Quaternion<> rotationLoc, bool isTrigger)
{
	auto it = physicsMap.find(bodyID);
	if (it == physicsMap.end()) return;

	PxRigidActor* actor = it->second->actor;
	if (!actor) return;

	PxShape* shape = gPhysics->createShape(PxBoxGeometry(size.getX() * 0.5f, size.getY() * 0.5f, size.getZ() * 0.5f), *defaultMaterial, true);
	if (!shape) return;
	PxQuat qLoc(rotationLoc.getX(), rotationLoc.getY(), rotationLoc.getZ(), rotationLoc.getW());//rot local
	PxTransform localPose(PxVec3(center.getX(), center.getY(), center.getZ()), qLoc);//pos local
	shape->setLocalPose(localPose);

	PxFilterData filterData;
	filterData.word0 = 1;
	filterData.word1 = 1;
	shape->setSimulationFilterData(filterData);
	if (isTrigger)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	else
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	}

	actor->attachShape(*shape);
	PxRigidDynamic* dyn = actor->is<PxRigidDynamic>();
	if (dyn)
	{
		PxRigidBodyExt::setMassAndUpdateInertia(*dyn, dyn->getMass());
	}
	it->second->shapes.push_back(shape);
}

void PhysicsModule::AttachCapsuleShape(ComponentID bodyID, float radius, float height, const core::Vector3<>& center, const core::Quaternion<> rotationLoc, bool isTrigger)
{
	auto it = physicsMap.find(bodyID);
	if (it == physicsMap.end()) return;

	PxRigidActor* actor = it->second->actor;
	if (!actor) return;
	PxShape* shape;
	if (height <= 0.0f)//Esfera
	{
		shape = gPhysics->createShape(PxSphereGeometry(radius), *defaultMaterial, true);
	}
	else//capsula
	{
		float halfHeight = (height * 0.5f) - radius;
		halfHeight = std::max(0.0f, halfHeight);
		shape = gPhysics->createShape(PxCapsuleGeometry(radius, halfHeight), *defaultMaterial, true);
	}
	if (shape == NULL) return;

	//rotadas en el eje Y para que se vean verticales
	PxQuat baseRot(PxHalfPi, PxVec3(0, 0, 1));
	PxQuat qLoc(rotationLoc.getX(), rotationLoc.getY(), rotationLoc.getZ(), rotationLoc.getW());
	PxQuat finalRot = baseRot * qLoc;//correcion eje + rot local collider
	PxTransform localPose(PxVec3(center.getX(), center.getY(), center.getZ()), finalRot);
	shape->setLocalPose(localPose);

	PxFilterData filterData;
	filterData.word0 = 1;
	filterData.word1 = 1;
	shape->setSimulationFilterData(filterData);

	if (isTrigger)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	else
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	}

	actor->attachShape(*shape);
	PxRigidDynamic* dyn = actor->is<PxRigidDynamic>();
	if (dyn)
	{
		PxRigidBodyExt::setMassAndUpdateInertia(*dyn, dyn->getMass());
	}
	it->second->shapes.push_back(shape);
}

void PhysicsModule::setPhysicsTransform(ComponentID id, const core::Vector3<>& pos, const core::Quaternion<>& rot)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;
	PxRigidActor* actor = it->second->actor;
	if (!actor) return;
	PxTransform t(PxVec3(pos.getX(), pos.getY(), pos.getZ()), PxQuat(rot.getX(), rot.getY(), rot.getZ(), rot.getW()));
	actor->setGlobalPose(t);
}

void PhysicsModule::onContact(const PxContactPairHeader& pairHeader,
	const PxContactPair* pairs,
	PxU32 nbPairs)
{
	PxRigidActor* actor0 = pairHeader.actors[0]->is<PxRigidActor>();
	PxRigidActor* actor1 = pairHeader.actors[1]->is<PxRigidActor>();

	if (!actor0 || !actor1) return;

	auto itA = actorToID.find(actor0);
	auto itB = actorToID.find(actor1);
	auto entA = actorToEntity.find(actor0);
	auto entB = actorToEntity.find(actor1);

	if (entA == actorToEntity.end() || entB == actorToEntity.end() ||
		itA == actorToID.end() || itB == actorToID.end())
		return;

	ComponentID a = itA->second;
	ComponentID b = itB->second;
	core::Entity* entityA = entA->second;
	core::Entity* entityB = entB->second;

	for (PxU32 i = 0; i < nbPairs; i++)
	{
		if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			eventQueue.push_back({ a, b, CollisionType::CollisionEnter, entityB });
			eventQueue.push_back({ b, a, CollisionType::CollisionEnter, entityA });
		}

		if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			eventQueue.push_back({ a, b, CollisionType::CollisionExit, entityB });
			eventQueue.push_back({ b, a, CollisionType::CollisionExit, entityA });
		}
	}
}

void PhysicsModule::DestroyBody(ComponentID id)
{

	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;

	PhysXComponent* comp = it->second;
	PxRigidActor* actor = comp->actor;
	if (!gScene || !actor) return;

	//lo quito de la scene
	if (gScene)
		gScene->removeActor(*actor, true);

	//libero shapes del actor
	for (PxShape* shape : comp->shapes)
	{
		if (shape)
		{
			actor->detachShape(*shape);
			shape->release();
		}
	}
	comp->shapes.clear();

	actorToEntity.erase(actor);
	actorToID.erase(actor);
	actor->release();//libero al actor

	if (actorToID.find(actor) == actorToID.end())
		return;
	//borro mapas
	physicsMap.erase(it);
}

void PhysicsModule::DestroyMaterial(uint32_t id)
{
	auto it = materialMap.find(id);
	if (it == materialMap.end()) return;

	if (it->second)
		it->second->release();

	materialMap.erase(it);
}

void PhysicsModule::ClearScene()
{

	if (!gScene) return;

	std::vector<ComponentID> ids;
	for (auto& [id, _] : physicsMap)
		ids.push_back(id);

	for (auto id : ids)
		DestroyBody(id);

	physicsMap.clear();
	actorToEntity.clear();
	actorToID.clear();
	eventQueue.clear();
}

std::vector<ShapeRenderData> PhysicsModule::GetRenderData()
{
	std::vector<ShapeRenderData> result;

	for (auto& [id, comp] : physicsMap)
	{
		PxRigidActor* actor = comp->actor;
		if (!actor) continue;

		for (PxShape* shape : comp->shapes)
		{
			if (!shape) continue;

			PxShapeFlags flags = shape->getFlags();
			//si ambos estan descativados es que el collider esta disabled
			if (!(flags & PxShapeFlag::eSIMULATION_SHAPE) && !(flags & PxShapeFlag::eTRIGGER_SHAPE))
			{
				continue;//no renderiza
			}

			ShapeRenderData data;
			PxTransform pose = PxShapeExt::getGlobalPose(*shape, *actor);
			data.position = { pose.p.x, pose.p.y, pose.p.z };
			data.rotation = { pose.q.x, pose.q.y, pose.q.z, pose.q.w };


			const PxGeometry& geom = shape->getGeometry();
			switch (geom.getType())
			{
			case PxGeometryType::eBOX:
			{
				const PxBoxGeometry& box = static_cast<const PxBoxGeometry&>(geom);
				data.type = ShapeType::BOX;
				data.size = core::Vector3<>(box.halfExtents.x * 2.0f, box.halfExtents.y * 2.0f, box.halfExtents.z * 2.0f);
				break;
			}

			case PxGeometryType::eSPHERE:
			{
				const PxSphereGeometry& sphere = static_cast<const PxSphereGeometry&>(geom);

				data.type = ShapeType::CAPSULE;
				data.radius = sphere.radius;
				data.halfHeight = 0.0f;
				break;
			}

			case PxGeometryType::eCAPSULE:
			{
				const PxCapsuleGeometry& capsule = static_cast<const PxCapsuleGeometry&>(geom);

				data.type = ShapeType::CAPSULE;
				data.radius = capsule.radius;
				data.halfHeight = capsule.halfHeight;
				break;
			}

			default:
				continue;
			}

			result.push_back(data);
		}
	}

	return result;
}

void PhysicsModule::ReloadPhysics()
{
	physicsMap.clear();
	actorToID.clear();
	actorToEntity.clear();
	eventQueue.clear();

	//libero escena act
	if (gScene)
	{
		gScene->release();
		gScene = nullptr;
	}

	//nueva igual
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	if (!dispatcher)
		dispatcher = PxDefaultCpuDispatcherCreate(2);

	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = CustomFilterShader;
	sceneDesc.simulationEventCallback = this;

	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
	sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;

	sceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;

	gScene = gPhysics->createScene(sceneDesc);

	if (!gScene)
	{
		Debug::error("[PhysicsModule] Error recreando escena");
		return;
	}

	gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	gScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.0f);

	//reasigno
	raycast = Raycast(gScene);

}


void PhysicsModule::SetActorEnabled(ComponentID id, bool enabled, bool isTrigger)
{
	auto it = physicsMap.find(id);
	if (it == physicsMap.end()) return;

	PhysXComponent* comp = it->second;

	for (PxShape* shape : comp->shapes) {
		if (!shape) continue;

		if (enabled) {
			if (isTrigger) {
				shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
				shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
			}
			else {
				shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
				shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
			}
			shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		}
		else {
			//desactivo todo
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
			shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
		}
	}
}


std::vector<PhysicsEvent> PhysicsModule::consumeEventsFor(ComponentID id)
{
	std::vector<PhysicsEvent> result;
	std::vector<PhysicsEvent> remaining;

	for (auto& e : eventQueue)
	{
		if (e.a == id)// || e.b == id)
			result.push_back(e);
		else
			remaining.push_back(e);
	}

	eventQueue = std::move(remaining);
	return result;
}