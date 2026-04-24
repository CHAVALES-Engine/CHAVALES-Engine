#pragma once
#include "../../src/Core-Defs/Vector3.h"
#include "../../src/Core-Defs/Component.h"

class Engine;

/**
 * @class RigidBody
 * @brief Componente físico que representa un cuerpo rígido dentro del motor.
 *
 * Encapsula propiedades físicas como masa, velocidad, gravedad y damping.
 * Sirve como puente entre el motor y el sistema de físicas.
 */
class RigidBody : public core::Component {
/*
 * +----------+
 * | RIGIDBODY |
 * +----------+
 *
 * --- Ejemplo de uso en lua ---
 *		RigidBody = {
 *			useGravity = true, 
 *			isKinematic = false,
 *			mass = 10.0,
 *			position = Vector3.new(0, 0, -10),     
 *			velocity = Vector3.new(0, 200, 0),   
 *			damping = 0.0
 *			blockAxes = { false, false, true },
 *			blockAngles = { false, false, true }
 *		}
*/
public:
	/**
	 * @brief Referencia al engine principal
	 */
	Engine* _eng = nullptr;

	/**
	 * @brief Valor de gravedad aplicado manualmente (si procede)
	 */
	float gravVal = 0;

	/**
	 * @brief ID interno del objeto en el sistema de físicas
	 */
	unsigned int physicsID;

	/**
	 * @brief Indica si el objeto usa gravedad
	 */
	bool useGravity = true;

	/**
	 * @brief Indica si el cuerpo es cinemático (no afectado por fuerzas)
	 */
	bool isKinematic;

	RigidBody();

	/**
	 * @brief Inicialización del componente con propiedades
	 */
	bool init(const Properties& p) override;

	/**
	 * @brief Llamado cuando el objeto está listo
	 */
	virtual void ready() override;

	/**
	 * @brief Actualización por frame
	 * @param deltaTime Tiempo entre frames (normalmente en ms)
	 */
	virtual void update(uint64_t deltaTime) override;

	/**
	 * @brief Obtiene la velocidad actual del rigidbody
	 */
	core::Vector3<> getVelocity();

	/**
	 * @brief Obtiene la posición actual del rigidbody
	 */
	core::Vector3<> getPosition();

	/**
	 * @brief Obtiene la rotación actual del rigidbody
	 */
	core::Quaternion<> getRotation();

	/**
	 * @brief Obtiene la masa del objeto
	 */
	float getMass();

	/**
	 * @brief Establece la velocidad del rigidbody
	 */
	void setVelocity(core::Vector3<> vel);

	/**
	 * @brief Establece la posición del rigidbody
	 */
	void setPosition(core::Vector3<> pos);

	/**
	 * @brief Establece la rotación del rigidbody
	 */
	void setRotation(core::Quaternion<> rot);

	/**
	 * @brief Establece la masa del objeto
	 */
	void setMass(float mass);

	/**
	 * @brief Establece el damping lineal (resistencia al movimiento)
	 */
	void setLinearDamping(float damping);

	/**
	 * @brief Obtiene el damping lineal
	 */
	float getLinearDamping();

	/**
	 * @brief Aplica una fuerza al rigidbody
	 * @param force Vector de fuerza
	 * @param mode Tipo de fuerza (impulso, fuerza continua, etc.)
	 */
	void AddForce(core::Vector3<> force, char mode);

	/**
	 * @brief Limpia fuerzas aplicadas
	 * @param mode Tipo de fuerza a limpiar
	 */
	void ClearForce(char mode);

	/**
	 * @brief Bloquea ejes
	 * @param x bloquea x
	 * @param y bloquea y
	 * @param z bloquea z
	 */
	void blockAxes(bool x, bool y, bool z);

	/**
	 * @brief Bloquea rotacion
	 * @param x bloquea x
	 * @param y bloquea y
	 * @param z bloquea z
	 */
	void blockAngles(bool x, bool y, bool z);

	/**
	 * @brief Obtiene el ID físico del objeto
	 */
	uint32_t getId() { return physicsID; };

	/**
	 * @brief Indica si es cinemático
	 */
	bool getIsKinematic() { return isKinematic; };

};