#pragma once
#include "Collider.h"

/*
 * +------------------+
 * | CAPSULE COLLIDER |
 * +------------------+
 *
 * --- Ejemplo de uso en lua ---
 * CapsuleCollider = {
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
class CapsuleCollider : public Collider//Sirve de esfera si pones radio y altura iguales
{
private:
	float radio = 0.5f;
	float altura = 1.0f;

public:

	CapsuleCollider() = default;

	//setter
	void SetRadius(float r) { radio = r; }
	void SetHeight(float h) { altura = h; }

	//getter
	float GetRadius() const { return radio; }
	float GetHeight() const { return altura; }
};