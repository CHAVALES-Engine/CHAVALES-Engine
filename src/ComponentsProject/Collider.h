#pragma once
#include  "../../src/Core-Defs/Component.h"
//#include "../../src/PhysicsModule/RigidBody.h"
//#include "../../src/Core-Defs/Defs.h"
using ComponentID = unsigned int;
class Engine;
class Transform;
class RigidBody;
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
 *		box = Vector3.new(120, 120, 120), o capsule = { radius = 0.5, height = 2 }, --size // si pones la altura a 0 en capsule haces una esfera
 *		dynamic = true,
 *		trigger = false,
 *      center = Vector3.new(0,0,0)
 * }
 *
 * --- Ejemplo de inicializacion ---
 * En bool init(const Properties& p):
 *		# Ej1, asignacion:
 *			isDynamic = getProperty<bool>(p, "dynamic");
 *		# Ej2, setter:
 * ...
 *
*/
enum class ShapeType { Box, Capsule };

class Collider : public core::Component
{
protected:

	bool isTrigger = false;
	bool isDynamic = false;//o pared o con gravedad
	bool initialized = false;

	core::Vector3<> size = { 1,1,1 };
	core::Vector3<> center = { 0,0,0 };//offset respecto a la entidad, donde esta el collider
	int radius, height;

	Engine* _eng;
	ComponentID physicsID = 0;
	Transform* transform;//entidad .pos es la posicion de la entidad
	RigidBody* rigidBody = nullptr;
	ShapeType shapeType;//default


public:
	Collider() {};
	~Collider() {};

	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t deltaTime) override;

	void onTriggerEnter(ComponentID other);
	void onTriggerExit(ComponentID other);
	void onCollisionEnter(ComponentID other);
	void onCollisionExit(ComponentID other);


	//getter
	ComponentID getId() { return physicsID; };
	const core::Vector3<>& getCenter() const { return center; }
};

//unorder map que relacione id con entidad, la entidad va a compartir id con el sgruct q guarda su actor y forma. Luego a traves de engine usando physx
// le pasare el unordermap que necesite rollo un metoodo que me permita coger la info del actr y shape segun su id y 
//con esa info en collider update actualizare el movimiento del transfor... mas adelante la shape