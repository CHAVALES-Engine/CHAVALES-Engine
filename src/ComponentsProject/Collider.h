#pragma once
#include  "../../src/Core-Defs/Component.h"
//#include "../../src/PhysicsModule/RigidBody.h"
//#include "../../src/Core-Defs/Defs.h"
using ComponentID = unsigned int;
class Engine;
namespace core {
	class Entity;
}
/*
 * +----------+
 * | COLLIDER |
 * +----------+
 *
 * --- Ejemplo de uso en lua ---
 * Collider = {
 *		...
 * }
 *
 * --- Ejemplo de inicializacion ---
 * En bool init(const Properties& p):
 *		# Ej1, asignacion:
 * ...
 *		# Ej2, setter:
 * ...
 *
*/
class Collider : public core::Component
{
protected:

	bool enabled = true;
	bool isTrigger = false;

	core::Vector3<> size = { 1,1,1 };
	bool isDynamic = false;
	//Rigidbody* attachedRigidbody = nullptr;//si tiene rigidbody atacheado

	core::Vector3<> center = { 0,0,0 };//offset al rigidbody
	Engine* _eng;
	ComponentID physicsID = 0;

public:
	Collider() {};
	~Collider() {};

	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t deltaTime) override;
};