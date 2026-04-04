#pragma once
#include  "../../src/Core-Defs/Component.h"
//#include "../../src/PhysicsModule/RigidBody.h"


class Engine;

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

	//Rigidbody* attachedRigidbody = nullptr;//si tiene rigidbody atacheado

	core::Vector3<> center = { 0,0,0 };//offset al rigidbody
	Engine* _eng;

public:
	Collider() {};
	~Collider() {};

	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t deltaTime) override;
};