#pragma once
// TODO: This is an example of a library function
#include "../../dependencies/PhysX/physx/include/PxPhysicsAPI.h"
#include "../../src/Core-Defs/Vector3.h"
#include "../../src/Core-Defs/PhysicsTypes.h"
#include <unordered_map>
#include "../../dependencies/PhysX/physx/include/PxSimulationEventCallback.h"

struct PhysXComponent;


class PhysicsModule : public physx::PxSimulationEventCallback
{
public:

	PhysicsModule();
	virtual ~PhysicsModule() = default;

	bool Init();
	core::Vector3<> GetPhysicsPosition(ComponentID id);
	void Update(float dt) ;

	//colliders
	ComponentID  CreateBoxShape(core::Vector3<> dimension, core::Vector3<> pos, bool isDynamic, bool isKinematic);
	ComponentID CreateCapsuleShape(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, bool isDynamic, bool isKinematic);
	std::vector<PhysicsEvent> getEventsFor(ComponentID id);
	void clearEvents();
	void SetPhysicsPosition(ComponentID id, const core::Vector3<>& pos);

	//rigidbody
	uint32_t CreateRigidBody(core::Vector3<> pos, float mass, bool useGravity);
	core::Vector3<> GetLinearVelocity(uint32_t id);
	void SetLinearVelocity(uint32_t id, core::Vector3<> vel);
	void AddForce(uint32_t id, core::Vector3<> force);
	void AddImpulse(uint32_t id, core::Vector3<> impulse);


	//callbacks
	void onContact(const physx::PxContactPairHeader& pairHeader,
		const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {
	};

	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;

	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {}
	void onWake(physx::PxActor** actors, physx::PxU32 count) override {}
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override {}
	void onAdvance(const physx::PxRigidBody* const* bodyBuffer,const physx::PxTransform* poseBuffer,const physx::PxU32 count) override {}


	uint32_t CreateMaterial(float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);

	void UpdateMaterial(uint32_t id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);
private:
	ComponentID nextID = 1;
	std::unordered_map<ComponentID, PhysXComponent> physicsMap;
	std::unordered_map<physx::PxRigidActor*, ComponentID> actorToID;
	std::vector<PhysicsEvent> eventQueue;
};
