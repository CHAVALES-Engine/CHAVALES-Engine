#pragma once
// TODO: This is an example of a library function
#include "../../dependencies/PhysX/physx/include/PxPhysicsAPI.h"
#include "../../src/Core-Defs/Vector3.h"
#include "../../src/Core-Defs/Quaternion.h"
#include "../../src/Core-Defs/PhysicsTypes.h"
#include <unordered_map>
#include "../../dependencies/PhysX/physx/include/PxSimulationEventCallback.h"
#include "Raycast.h"

struct PhysXComponent;


class PhysicsModule : public physx::PxSimulationEventCallback
{
public:

	PhysicsModule();
	virtual ~PhysicsModule();

	bool Init();
	core::Vector3<> GetPhysicsPosition(ComponentID id);
	core::Quaternion<> GetPhysicsRotation(ComponentID id);
	void Update(float dt);

	bool rayCast(const PxVec3& origin,
		const PxVec3& direction,
		float maxDistance);

	//colliders
	ComponentID  CreateBoxShape(core::Vector3<> dimension, const core::Vector3<>& center, core::Vector3<> pos, const core::Quaternion<> rot, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger);
	ComponentID CreateCapsuleShape(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, const core::Quaternion<> rot, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger);
	std::vector<PhysicsEvent> getEventsFor(ComponentID id);
	void clearEvents();
	void SetPhysicsPosition(ComponentID id, const core::Vector3<>& pos);
	void SetPhysicsRotation(ComponentID id, const core::Quaternion<>& pos);

	//rigidbody
	uint32_t CreateRigidBody(core::Vector3<> pos, float mass, bool useGravity, bool isKinematic);
	core::Vector3<> GetLinearVelocity(uint32_t id);
	void SetLinearVelocity(uint32_t id, core::Vector3<> vel);
	void AddForce(uint32_t id, core::Vector3<> force, char mode);
	void ClearForce(uint32_t id, char mode);

	float GetMass(uint32_t id);
	void SetMass(uint32_t id, float mass);

	float GetLinearDamping(uint32_t id);
	void SetLinearDamping(uint32_t id, float damping);

	//collider + rigidbody
	void AttachBoxShape(ComponentID bodyID, const core::Vector3<> size, const core::Vector3<>& center, const core::Quaternion<> rotationLoc, bool isTrigger);
	void AttachCapsuleShape(ComponentID bodyID, float radius, float height, const core::Vector3<>& center, const core::Quaternion<> rotationLoc, bool isTrigger);
	void setPhysicsTransform(ComponentID id, const core::Vector3<>& pos, const core::Quaternion<>& rot);

	//callbacks
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {}
	void onWake(physx::PxActor** actors, physx::PxU32 count) override {}
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override {}
	void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {}

	//materiales
	uint32_t CreateMaterial(float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);
	void UpdateMaterial(uint32_t id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);

	//limpieza
	void DestroyBody(ComponentID id);
	void DestroyMaterial(uint32_t id);
	void ClearScene();

	//debbug
	std::vector<ShapeRenderData> GetRenderData();
private:
	Raycast raycast;
	ComponentID nextID = 1;
	ComponentID nextIDMaterial = 1;
	std::unordered_map<ComponentID, PhysXComponent> physicsMap;
	std::unordered_map<physx::PxRigidActor*, ComponentID> actorToID;
	std::vector<PhysicsEvent> eventQueue;
};
