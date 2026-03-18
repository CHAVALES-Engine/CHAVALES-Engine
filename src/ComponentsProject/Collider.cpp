#include "Collider.h"
#include "PluginSDK.h"

REGISTER_COMPONENT(Collider);

Collider::Collider(Rigidbody* rb)
{
	attachedRigidbody = rb;
}

Collider::Collider(Rigidbody* rb, bool trigger) : attachedRigidbody(rb), isTrigger(trigger) {}

////setters
//void Collider::SetEnabled(bool value)
//{
//	enabled = value;
//	//enable y !trigger = colision fisica
//	//enable y trigger no es colision fisica solo deteccion
//	//!enable entonces desactivado y nada
//	if (shape) {
//		//shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, value && !isTrigger);
//		//shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, value && isTrigger);//activaria modo trigger
//	}
//}
//
//void Collider::SetTrigger(bool value)
//{
//	isTrigger = value;
//	/*if (shape) {
//		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !value);
//		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, value);
//	}*/
//}
//
//void Collider::SetRigidbody(Rigidbody* rb)
//{
//	attachedRigidbody = rb;
//}
//
//
////gettters
//bool Collider::IsEnabled() const
//{
//	return enabled;
//}
//bool Collider::IsTrigger() const
//{
//	return isTrigger;
//}
//
//Rigidbody* Collider::GetRigidbody() const
//{
//	return attachedRigidbody;
//}
//
//core::Vector3<> Collider::GetWorldPosition()
//{
//	if (attachedRigidbody)
//		return attachedRigidbody->getPosition() + center;
//	return center;
//}

bool Collider::init(const Properties& p)
{
	enabled = getProperty<bool>(p, "colliderEnabled");
	isTrigger = getProperty<bool>(p, "isTrigger");
	/*shape = getProperty<PxShape>(p, "shape");
	actor = getProperty<PxRigidActor>(p, "actor");*/
	//center = getProperty<Vector3<>>(p, "isTrigger");

	return true;
}

//physx::PxShape* Collider::GetShape() const { return shape; }
//physx::PxRigidActor* Collider::GetActor() const { return actor; }
//void Collider::SetActor(physx::PxRigidActor* a) { actor = a; }
//void Collider::SetShape(physx::PxShape* s) { shape = s; }
