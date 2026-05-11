//#pragma once
//#include <vector>
//
//#include "PhysicsTypes.h"
//#include "Quaternion.h"
//#include "Vector3.h"
//
//class Engine;
//class PhysicsModule;
//
///**
// * @brief API publica para fisica del motor.
// * Expone solo los metodos seguros para componentes.
// * Para metodos internos, ver PhysicsModule.
// */
//class PhysicsFacade
//{
//private:
//	/**
//	 * @brief Clases amigas.
//	 */
//	friend Engine;
//	friend core::EngineComponent;
//	/**
//	 * @brief Referencia al modulo que envuelve.
//	 */
//	PhysicsModule* _physics;
//	PhysicsFacade(PhysicsModule* physics);
//
//	/**
//	 * emparenta un actor de physx con una entidad
//	 * @param physicsID
//	 * @param entity
//	 */
//	void _registerActorEntity(ComponentID physicsID, core::Entity* entity);
//	/*
//	* @brief Devuelve el id de la entidad que tiene el boxcollider y lo crea
//	*/
//	uint32_t _createBoxCollider(const core::Vector3<>& size, const core::Vector3<>& center, const core::Vector3<>& pos, const core::Quaternion<> rotGlob, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger);
//	/*
//	* @brief Setea la posicion fisica de la entidad
//	*/
//	void _setPhysicsPosition(uint32_t id, const core::Vector3<>& pos);
//	/*
//	* @brief Setea la posicion fisica de la entidad
//	*/
//	void _setPhysicsRotation(uint32_t id, const core::Quaternion<>& rot);
//	/*
//	* @brief Coge la posicion del transform de la entidad en cuando a physx para poder mover el transform de la entidad y que se vea en nuestra escena
//	*/
//	core::Vector3<> _getPhysicsPosition(uint32_t id);
//	/*
//	* @brief Coge la rotacion del transform de la entidad en cuando a physx para poder rotar el transform de la entidad y que se vea en nuestra escena
//	*/
//	core::Quaternion<> _getPhysicsRotation(uint32_t id);
//	/*
//	* @brief Devuelve el id de la entidad que tiene el capsulecollider y lo crea
//	*/
//	ComponentID _createCapsuleCollider(float radius, float height, const core::Vector3<>& center, const core::Vector3<>& worldPos, const core::Quaternion<> rotGlob, const core::Quaternion<> rotationLoc, bool isDynamic, bool isTrigger);
//	/*
//	* @brief Coge todos los eventos de fisica para poder acceder a ellos y saber cuando ocurren las colisiones
//	*/
//	std::vector<PhysicsEvent> _getPhysicsEvents(ComponentID id);
//	std::vector<PhysicsEvent> _consumeEvents(ComponentID id);
//	/*
//	* @brief Limpia el vector de eventos para no leerlos mas de una vez
//	*/
//	void _clearPhysicsEvents();
//	/*
//	* @brief Une box shape al rigidbody creado desde lua
//	*/
//	ComponentID _attachBoxShapeToRigidBody(ComponentID bodyID, const core::Vector3<> size, const core::Vector3<>& center, const core::Quaternion<> rotation, bool isTrigger);
//	/*
//	* @brief  Une capsule shape al rigidbody creado desde lua
//	*/
//	ComponentID _attachCapsuleShapeToRigidBody(ComponentID bodyID, float radius, float height, const core::Vector3<>& center, const core::Quaternion<> rotation, bool isTrigger);
//	/*
//	* @brief Setea el transform de Physx para el movimiento kinematico
//	*/
//	void _setPhysicsTransform(ComponentID id, const core::Vector3<>& pos, const core::Quaternion<>& rot);
//
//
//	uint32_t _createRigidBody(core::Vector3<> pos, float mass, bool useGravity, bool isKinematic);
//
//	core::Vector3<> _getLinearVelocity(uint32_t id);
//
//	void _setLinearVelocity(uint32_t id, core::Vector3<> vel);
//
//	void _setMass(uint32_t id, float mass);
//	float _getMass(uint32_t id);
//
//	void _setLinearDamping(uint32_t id, float damping);
//	float _getLinearDamping(uint32_t id);
//
//	void _addForce(uint32_t id, core::Vector3<> force, char mode);
//	void _clearForce(uint32_t id, char mode);
//
//	void _blockAxes(uint32_t id, bool x, bool y, bool z);
//	void _blockAngles(uint32_t id, bool x, bool y, bool z);
//
//	uint32_t _createMaterial(ComponentID id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);
//
//	void _updateMaterial(uint32_t id, float staticF, float dynamicF, float restitution, int frictionCombine, int bounceCombine);
//
//	void _destroyMaterial(uint32_t id);
//
//	/*
//	* @brief Elimina componente de physx usando su id
//	* @param physx id
//	*/
//	void _deletePhysicsComponent(ComponentID id);
//	/*
//	* @brief Elimina materiales usando su id
//	* @param physx id
//	*/
//	void _deletePhysicsMaterial(ComponentID id);
//	/*
//	* @brief Activa/desactiva collider
//	* @param physx id
//	*/
//	void _setActorEnabled(ComponentID id, bool enabled, bool isTrigger);
//
//public:
//
//	bool rayCast(const core::Vector3<>& origin,
//		const core::Vector3<>& direction,
//		float maxDistance,
//		RayInfo& rayInfo) const;
//	std::vector<ShapeRenderData> GetPhysicsRenderData();
//
//	void SetGravity(const core::Vector3<>& gravity = { 0.0f, -9.8f,0.0f }) const;
//};
//
