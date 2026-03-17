#pragma once
#include "../../src/Core/Vector3.h"
class Rigidbody {
public:
	/**
	* @brief Variable de masa.
	*/
	float mass = 1.0f;

	/**
	* @brief Vector de posición.
	*/
	core::Vector3<> position = { 0,0,0 };
	/**
	* @brief Vector de velocidad.
	*/
	core::Vector3<> velocity = { 0,0,0 };

	/**
	* @brief Vector de fuerza acumulada.
	*/
	core::Vector3<> accumulatedForce = { 0,0,0 };

	/**
	* @brief Booleano de gravedad (indica si hay gravedad aplicada o no).
	*/
	bool useGravity = true;

	/**
	* @brief Vector de gravedad.
	*/
	core::Vector3<> gravity = { 0,-9.80665f,0 };

	/**
	* @brief Devuelve la velocidad.
	* @return La velocidad
	*/
	core::Vector3<> getVelocity() { return velocity; };
	/**
	* @brief Devuelve la posición.
	* @return La posición
	*/
	core::Vector3<> getPosition() { return position; };

	/**
	* @brief Establece el valor vel a la velocidad.
	*/
	void setVelocity(core::Vector3<> vel) { velocity = vel; };
	/**
	* @brief Establece el valor pos a la posición.
	*/
	void setPosition(core::Vector3<> pos) { position = pos; };

	/**
	* @brief Añade la fuerza force a la fuerza acumulada.
	*/
	void AddForce(core::Vector3<> force) {
		accumulatedForce += force;
	}

	/**
	* @brief Añade impulso a la velocidad.
	*/
	void AddImpulse(core::Vector3<> impulse) {
		velocity += impulse * (1.0f / mass);
	}

	//HAY QUE LLAMARLO EN UPDATE DE PHYSICS MODULE CUANDO LO HAYA
	/**
	* @brief Actualiza en cada frame la posicion segun la velocidad, la fuerza y la gravedad.
	*/
	void Update(float dt) {

		if (useGravity)
			accumulatedForce += gravity * mass;

		core::Vector3<> acceleration = accumulatedForce * (1.0f / mass);

		velocity += acceleration * dt;
		position += velocity * dt;

		accumulatedForce = { 0,0,0 };
	}
};