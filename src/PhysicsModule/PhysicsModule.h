/**
* @file
*/

#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "PhysicsTypes.h"
#include <unordered_map>
#include "Raycast.h"
#include "EngineAPI.h"

struct PhysXComponent;

class ENGINE_API PhysicsModule : public physx::PxSimulationEventCallback
{
public:
	/*
	* @brief Constructor del módulo de físicas
	*/
	PhysicsModule();

	/*
	* @brief Destructor del módulo de físicas
	*/
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
	void fixedUpdate(float dt);

	/*
	* @brief Lanza un raycast en la escena
	* @param origen, direccion y distancia máxima
	*/
	bool rayCast(const PxVec3& origin, const PxVec3& direction, float maxDistance, RayInfo& rayInfo);

	//COLLIDERS
	/*
	* @brief Crea un collider con forma de caja
	* @param tamano, posición local, posición global, rotacion global, rotacion local, bool isTrigger, bool isDynamic
	*/
	ComponentID  CreateBoxShape(const core::Vector3<>& dimension, const core::Vector3<>& center, const core::Vector3<>& pos, const core::Quaternion<>& rot, const core::Quaternion<>& rotationLoc, bool isDynamic, bool isTrigger);
	/*
	* @brief Crea un collider con forma de cápsula/esfera
	* @param radio, altura, posición local, posición global, rotacion global, rotacion local, bool isTrigger, bool isDynamic
	*/
	ComponentID CreateCapsuleShape(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, const core::Quaternion<>& rot, const core::Quaternion<>& rotationLoc, bool isDynamic, bool isTrigger);
	/*
	* @brief Devuelve los eventos que recibe un componente
	* @param id del componente
	*/
	std::vector<PhysicsEvent> getEventsFor(ComponentID id);
	/*
	* Limpia la lista de eventos registrados
	*/
	void clearEvents();
	/**
	 * Asigna una entidad a una PhiscX actor.
	 * @param physicsID 
	 * @param entity 
	 */
	void setActorEntity(ComponentID physicsID, core::Entity* entity);
	/*
	* Setea la posicion de un componente en physx
	* @param id del componente y la posición a poner
	*/
	void SetPhysicsPosition(ComponentID id, const core::Vector3<>& pos);
	/*
	* Setea la rotacion de un componente en physx
	* @param id del componente y la rot a poner
	*/
	void SetPhysicsRotation(ComponentID id, const core::Quaternion<>& pos);

	//rigidbody
	/*
	* @brief Crea un rigidbody en la escena
	* @param posicion inicial, masa, uso de gravedad, si es cinematico
	*/
	uint32_t CreateRigidBody(const core::Vector3<>& pos, float mass, bool useGravity, bool isKinematic);

	/*
	* @brief Obtiene la velocidad lineal de un rigidbody
	* @param id del rigidbody
	*/
	core::Vector3<> GetLinearVelocity(uint32_t id);

	/*
	* @brief Setea la velocidad lineal de un rigidbody
	* @param id del rigidbody y nueva velocidad
	*/
	void SetLinearVelocity(uint32_t id, const core::Vector3<>& vel);

	/*
	* @brief Aplica una fuerza a un rigidbody
	* @param id del rigidbody, fuerza y modo
	*/
	void AddForce(uint32_t id, const core::Vector3<>& force, char mode);

	/*
	* @brief Limpia fuerzas aplicadas a un rigidbody
	* @param id del rigidbody y modo
	*/
	void ClearForce(uint32_t id, char mode);

	/**
	 * @brief Bloquea ejes
	 * @param x bloquea x
	 * @param y bloquea y
	 * @param z bloquea z
	 */
	void BlockAxes(uint32_t id, bool x, bool y, bool z);

	/**
	 * @brief Bloquea rotacion
	 * @param x bloquea x
	 * @param y bloquea y
	 * @param z bloquea z
	 */
	void BlockAngles(uint32_t id, bool x, bool y, bool z);

	/*
	* @brief Obtiene la masa de un rigidbody
	* @param id del rigidbody
	*/
	float GetMass(uint32_t id);

	/*
	* @brief Setea la masa de un rigidbody
	* @param id del rigidbody y nueva masa
	*/
	void SetMass(uint32_t id, float mass);

	/*
	* @brief Obtiene el damping lineal de un rigidbody
	* @param id del rigidbody
	*/
	float GetLinearDamping(uint32_t id);

	/*
	* @brief Setea el damping lineal de un rigidbody
	* @param id del rigidbody y nuevo damping
	*/
	void SetLinearDamping(uint32_t id, float damping);

	//collider + rigidbody
	/*
	* Une una box shape a un actor que viene de un rigidbody
	* @param id del rigidbody al que se une, tam, pos local, rot local, bool isTrigger
	*/
	void AttachBoxShape(ComponentID bodyID, const core::Vector3<>& size, const core::Vector3<>& center, const core::Quaternion<>& rotationLoc, bool isTrigger);
	/*
	* Une una capsuele/sphere shape a un actor que viene de un rigidbody
	* @param id del rigidbody al que se une, radio, altura, pos local, rot local, bool isTrigger
	*/
	void AttachCapsuleShape(ComponentID bodyID, float radius, float height, const core::Vector3<>& center, const core::Quaternion<>& rotationLoc, bool isTrigger);
	/*
	* Setea el transform de fisicas
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
	/*
	* @brief Setea la gravedad de la escena
	* @param gravedad en Vector3
	*/
	void SetGravity(const core::Vector3<>& gravity);
	//MATERIALES
	/*
	* @brief Crea un material fisico
	* @param friccion estatica, friccion dinamica, restitucion y modos de combinacion
	*/
	uint32_t CreateMaterial(ComponentID id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);

	/*
	* @brief Actualiza un material fisico existente
	* @param id del material y nuevos parametros
	*/
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

	//DEBUG
	/*
	* Devuelve un struct con los datos de las shapes
	*/
	std::vector<ShapeRenderData> GetRenderData();

	//RELOAD
	/*
	* Borra y vuelve a crear la escena de physx limpia
	*/
	void ReloadPhysics();

	void SetActorEnabled(ComponentID id, bool enabled, bool isTrigger);

	std::vector<PhysicsEvent> consumeEventsFor(ComponentID id);

	float getGravity();

private:
	Raycast _raycast;
	ComponentID _nextID = 1;
	ComponentID _nextIDMaterial = 1;
	std::unordered_map<ComponentID, PhysXComponent*> _physicsMap;
	std::unordered_map<physx::PxRigidActor*, ComponentID> _actorToID;
	std::unordered_map<physx::PxRigidActor*, core::Entity*> _actorToEntity;
	std::vector<PhysicsEvent> _eventQueue;

	const PxVec3 _defaultGravity = { 0.0f, -981.0f, 0.0f };
};