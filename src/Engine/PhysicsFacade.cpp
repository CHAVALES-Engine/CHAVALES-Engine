//#include "PhysicsFacade.h"
//#include "PhysicsModule.h"
//
//PhysicsFacade::PhysicsFacade(PhysicsModule* physics): _physics(physics)
//{}
//
//void PhysicsFacade::_registerActorEntity(ComponentID physicsID, core::Entity* entity)
//{
//	_physics->setActorEntity(physicsID, entity);
//}
//
//uint32_t PhysicsFacade::_createBoxCollider(const core::Vector3<>& size, const core::Vector3<>& center, const core::Vector3<>& pos, const core::Quaternion<> rotGlob, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger)
//{
//	return _physics->CreateBoxShape(size, center, pos, rotGlob, rotationLoc, isDynamic, isTrigger);
//}
//
//void PhysicsFacade::_setPhysicsPosition(uint32_t id, const core::Vector3<>& pos)
//{
//	_physics->SetPhysicsPosition(id, pos);
//}
//
//void PhysicsFacade::_setPhysicsRotation(uint32_t id, const core::Quaternion<>& rot)
//{
//	_physics->SetPhysicsRotation(id, rot);
//}
//
//core::Vector3<> PhysicsFacade::_getPhysicsPosition(uint32_t id)
//{
//	return _physics->GetPhysicsPosition(id);
//}
//
//core::Quaternion<> PhysicsFacade::_getPhysicsRotation(uint32_t id)
//{
//	return _physics->GetPhysicsRotation(id);
//}
//
//uint32_t PhysicsFacade::_createCapsuleCollider(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, const core::Quaternion<> rotGlob, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger)
//{
//	if (!_physics) return 0;
//	return _physics->CreateCapsuleShape(radius, height, center, worldPos, rotGlob, rotationLoc, isDynamic, isTrigger);
//}
//
//std::vector<PhysicsEvent> PhysicsFacade::_getPhysicsEvents(ComponentID id)
//{
//	return _physics->getEventsFor(id);
//}
//
//void PhysicsFacade::_clearPhysicsEvents()
//{
//	_physics->clearEvents();
//}
/////
//ComponentID PhysicsFacade::_attachBoxShapeToRigidBody(ComponentID bodyID, const core::Vector3<> size, const core::Vector3<>& center, const core::Quaternion<> rotation, bool isTrigger)
//{
//	if (!_physics) return 0;
//	_physics->AttachBoxShape(bodyID, size, center, rotation, isTrigger);
//	return bodyID; //devuelve el ID del RigidBody al que se unio
//}
//
//ComponentID PhysicsFacade::_attachCapsuleShapeToRigidBody(ComponentID bodyID, float radius, float height, const core::Vector3<>& center, const core::Quaternion<> rotation, bool isTrigger)
//{
//	if (!_physics) return 0;
//	_physics->AttachCapsuleShape(bodyID, radius, height, center, rotation, isTrigger);
//	return bodyID;
//}
//
//void PhysicsFacade::_setPhysicsTransform(ComponentID id, const core::Vector3<>& pos, const core::Quaternion<>& rot)
//{
//	if (!_physics) return;
//
//	_physics->setPhysicsTransform(id, pos, rot);
//}
//
//std::vector<PhysicsEvent> PhysicsFacade::_consumeEvents(ComponentID id)
//{
//	return _physics->consumeEventsFor(id);
//}
//
/////
//uint32_t PhysicsFacade::_createRigidBody(core::Vector3<> pos, float mass, bool useGravity, bool isKinematic)
//{
//	return _physics->CreateRigidBody(pos, mass, useGravity, isKinematic);
//}
//
//core::Vector3<> PhysicsFacade::_getLinearVelocity(uint32_t id)
//{
//	return _physics->GetLinearVelocity(id);
//}
//
//void PhysicsFacade::_setLinearVelocity(uint32_t id, core::Vector3<> vel)
//{
//	_physics->SetLinearVelocity(id, vel);
//}
//
//void PhysicsFacade::_setMass(uint32_t id, float mass)
//{
//	_physics->SetMass(id, mass);
//}
//
//float PhysicsFacade::_getMass(uint32_t id)
//{
//	return _physics->GetMass(id);
//}
//
//void PhysicsFacade::_setLinearDamping(uint32_t id, float damping)
//{
//	_physics->SetLinearDamping(id, damping);
//}
//
//float PhysicsFacade::_getLinearDamping(uint32_t id)
//{
//	return _physics->GetLinearDamping(id);
//}
//
//
//void PhysicsFacade::_addForce(uint32_t id, core::Vector3<> force, char mode)
//{
//	_physics->AddForce(id, force, mode);
//}
//
//void PhysicsFacade::_clearForce(uint32_t id, char mode)
//{
//	_physics->ClearForce(id, mode);
//}
//
//void PhysicsFacade::_blockAxes(uint32_t id, bool x, bool y, bool z)
//{
//	_physics->BlockAxes(id, x, y, z);
//}
//
//void PhysicsFacade::_blockAngles(uint32_t id, bool x, bool y, bool z)
//{
//	_physics->BlockAngles(id, x, y, z);
//}
//
//uint32_t PhysicsFacade::_createMaterial(ComponentID id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine)
//{
//	return _physics->CreateMaterial(id, staticF, dynamicF, restitution, frictionCombine, bounceCombine);
//}
//
//void PhysicsFacade::_updateMaterial(uint32_t id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine)
//{
//	_physics->UpdateMaterial(id, staticF, dynamicF, restitution, frictionCombine, bounceCombine);
//}
//
//void PhysicsFacade::_destroyMaterial(uint32_t id)
//{
//	if (_physics == nullptr) return;
//	_physics->DestroyMaterial(id);
//}
//
//bool PhysicsFacade::rayCast(const core::Vector3<>& origin,
//	const core::Vector3<>& direction,
//	float maxDistance,
//	RayInfo& rayInfo) const
//{
//	return _physics->rayCast({ origin.getX(), origin.getY(), origin.getZ() },
//		{ direction.getX(), direction.getY(), direction.getZ() },
//		maxDistance, rayInfo);
//}
//std::vector<ShapeRenderData> PhysicsFacade::GetPhysicsRenderData()
//{
//	if (!_physics) return {};
//	return _physics->GetRenderData();
//}
//
//void PhysicsFacade::SetGravity(const core::Vector3<>& gravity) const
//{
//	_physics->SetGravity(gravity);
//}
//
//void PhysicsFacade::_deletePhysicsComponent(ComponentID id)
//{
//	_physics->DestroyBody(id);
//}
//void PhysicsFacade::_deletePhysicsMaterial(ComponentID id)
//{
//	_physics->DestroyMaterial(id);
//}
//void PhysicsFacade::_setActorEnabled(ComponentID id, bool enabled, bool isTrigger)
//{
//	_physics->SetActorEnabled(id, enabled, isTrigger);
//}