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

	/*
	* @brief Inicializa la escena
	*/
	bool Init();
	/*
	* @brief Devuelve la posicion en la escena de physx del componente pasado
	* @param id del componente
	*/
	core::Vector3<> GetPhysicsPosition(ComponentID id);
	/*
	* @brief Devuelve la rotación en la escena de physx del componente pasado
	* @param id del componente
	*/
	core::Quaternion<> GetPhysicsRotation(ComponentID id);
	/*
	* @brief Update 
	* @param deltatime
	*/
	void Update(float dt);

	bool rayCast(const PxVec3& origin,const PxVec3& direction,float maxDistance);

	//COLLIDERS
	/*
	* @brief Crea un collider con forma de caja
	* @param tamano, posición local, posición global, rotacion global, rotacion local, bool isTrigger, bool isDynamic
	*/
	ComponentID  CreateBoxShape(core::Vector3<> dimension, const core::Vector3<>& center, core::Vector3<> pos, const core::Quaternion<> rot, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger);
	/*
	* @brief Crea un collider con forma de cápsula/esfera
	* @param radio, altura, posición local, posición global, rotacion global, rotacion local, bool isTrigger, bool isDynamic
	*/
	ComponentID CreateCapsuleShape(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, const core::Quaternion<> rot, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger);
	/*
	* @brief Devuelve los eventos que recibe un componente
	* @param id del componente
	*/
	std::vector<PhysicsEvent> getEventsFor(ComponentID id);
	/*
	* Limpia la lista de eventos registrados
	*/
	void clearEvents();
	/*
	* Setea la posición de un componente en physx
	* @param id del componente y la posición a poner
	*/
	void SetPhysicsPosition(ComponentID id, const core::Vector3<>& pos);
	/*
	* Setea la rotación de un componente en physx
	* @param id del componente y la rot a poner
	*/
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
	/*
	* Une una box shape a un actor que viene de un rigidbody
	* @param id del rigidbody al que se une, tam, pos local, rot local, bool isTrigger
	*/
	void AttachBoxShape(ComponentID bodyID, const core::Vector3<> size, const core::Vector3<>& center, const core::Quaternion<> rotationLoc, bool isTrigger);
	/*
	* Une una capsuele/sphere shape a un actor que viene de un rigidbody
	* @param id del rigidbody al que se une, radio, altura, pos local, rot local, bool isTrigger
	*/
	void AttachCapsuleShape(ComponentID bodyID, float radius, float height, const core::Vector3<>& center, const core::Quaternion<> rotationLoc, bool isTrigger);
	/*
	* Setea el transform de físicas
	* @param id del componente, posicion, rotacion
	*/
	void setPhysicsTransform(ComponentID id, const core::Vector3<>& pos, const core::Quaternion<>& rot);

	//CALLBACKS
	/*
	* Detecta cuando dos objetos trigger se tocan
	*/
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
	/*
	* Detecta cuando dos colliders colisionan
	*/
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
	/*
	* Detecta cuando dos colliders dejan de colisionar
	*/
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {}
	void onWake(physx::PxActor** actors, physx::PxU32 count) override {}
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override {}
	void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {}

	//MAYERIALES
	uint32_t CreateMaterial(float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);
	void UpdateMaterial(uint32_t id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);

	//LIMPIEZA
	/*
	* Destruye los actores
	*/
	void DestroyBody(ComponentID id);
	/*
	* Destruye los materiales
	*/
	void DestroyMaterial(uint32_t id);
	/*
	* Limpia la escena
	*/
	void ClearScene();

	//DEBBUG
	/*
	* Devuelve un struct con los datos de las shapes
	*/
	std::vector<ShapeRenderData> GetRenderData();

	//RELOAD
	/*
	* Borra y vuelve a crear la escena de physx limpia
	*/
	void ReloadPhysics();

private:
	Raycast raycast;
	ComponentID nextID = 1;
	ComponentID nextIDMaterial = 1;
	std::unordered_map<ComponentID, PhysXComponent> physicsMap;
	std::unordered_map<physx::PxRigidActor*, ComponentID> actorToID;
	std::vector<PhysicsEvent> eventQueue;
};
