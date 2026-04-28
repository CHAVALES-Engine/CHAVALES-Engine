#pragma once
#include  "Component.h"
#include "CommonEnums.h"
#include "Message.h"

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
 *		type = BOX / CAPSULE
 *		shape = Vector3.new(400, 100, 400), / Vector3.new(50, 0, 0), --  Z no se usa, para esfera Y = 0   
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
 * 
 * --- Importante a la hora de crearlos ---
 *	El capsule colider va orientado en el eje Y siempre cuando se crea, 
 *  tambien le anade la rotacion que lleve el trasnform de la entidad a la que va unido
 *  La el mesh mide 1,2,1 por lo que si quieres poner todo acorde el collider debe medir x,x*4,0
 * La mesh del sphere es 2,2,2 asi que recuerda que es enorme y debe ser x,0,0
*/
class Collider : public core::Component
{
protected:
	/**
	 * @brief Si es un trigger o no
	 */
	bool isTrigger = false;
	/**
	 * @brief Si es dinámico o estático
	 */
	bool isDynamic = false;

	/**
	 * @brief Tamaño del collider si es box
	 */
	core::Vector3<> size = { 1,1,1 };
	/**
	 * @brief Tamaño del collider si es capsule
	 */
	int radius, height;
	/**
	 * @brief Offset respecto a la entidad, donde esta el collider
	 */
	core::Vector3<> center = { 0,0,0 };
	/**
	 * @brief Rotacion local del collider respecto al transform
	 */
	core::Quaternion<> rotation;

	/**
	 * @brief Referencia a engine
	 */
	Engine* _eng;
	/**
	 * @brief Id del collider o en caso de estar unido a rigidbody, id de ambos
	 */
	ComponentID physicsID = 0;
	/**
	 * @brief Transform de la entidad
	 */
	Transform* transform;
	/**
	 * @brief RigidBody al que está unido en caso de no ser null
	 */
	RigidBody* rigidBody = nullptr;
	/**
	 * @brief Shape del collider
	 */
	ShapeType shapeType;

public:
	// Callbacks
	core::Message<core::Entity*> _onTriggerEnter;
	core::Message<core::Entity*> _onTriggerExit;
	core::Message<core::Entity*> _onCollisionEnter;
	core::Message<core::Entity*> _onCollisionExit;
	/**
	 * @brief Constructora vacía
	 */
	Collider();
	/**
	 * @brief Destructora
	 */
	~Collider() = default;

	/**
	 * @brief Inicialización del componente con propiedades
	 * @param Properties
	 */
	bool init(const Properties& p) override;
	/**
	 * @brief Llamado cuando el objeto está listo
	 */
	void ready() override;
	/**
	 * @brief Actualización por frame
	 * @param deltaTime Tiempo entre frames (normalmente en ms)
	 */
	void update(uint64_t deltaTime) override;

	/**
	 * @brief Detección de solapamiento entre colliders donde al menos uno es trigger
	 * @param core::Entity other, otro collider contra el que choca
	 */
	void onTriggerEnter(core::Entity* other);
	/**
	 * @brief Detección de salida de solapamiento entre colliders donde al menos uno es trigger
	 * @param core::Entity other, otro collider contra el que había chocado
	 */
	void onTriggerExit(core::Entity* other);
	/**
	 * @brief Detección de choque entre colliders donde ninguno es trigger
	 * @param core::Entity other, otro collider contra el que choca
	 */
	void onCollisionEnter(core::Entity* other);
	/**
	 * @brief Detección de salida de choque entre colliders donde ninguno es trigger
	 * @param ComponentId other, otro collider contra el que había chocado
	 */
	void onCollisionExit(core::Entity* other);

	/**
	 * @brief Getter de la posición del collider respecto al transform de la entidad (posicion local)
	 */
	const core::Vector3<>& getCenter() const { return center; }

	/**
	 * @brief Obtiene el ID físico del objeto
	 */
	uint32_t getId() { return physicsID; };

	bool hasCollided = false;
	bool hasTriggered = false;

};
