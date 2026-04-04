#pragma once
#include "../../src/Core-Defs/Vector3.h"
#include "Collider.h"

/*
 * +--------------+
 * | BOX COLLIDER |
 * +--------------+
 *
 * --- Ejemplo de uso en lua ---
 * BoxCollider = {
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
class BoxCollider : public Collider
{
private:
	core::Vector3<> size = { 1,1,1 };//inicial
public:
	BoxCollider() = default;//para objs sin rigidbody
	

	//setter
	void SetSize(const core::Vector3<>& newSize) { size = newSize; }
	//getter
	core::Vector3<> GetSize() const { return size; }
};