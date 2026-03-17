#pragma once
#include <Vector3.h>
#include "Collider.h"


class BoxCollider : public Collider
{
private:
	core::Vector3<> size = { 1,1,1 };//inicial
public:
	BoxCollider() = default;//para objs sin rigidbody
	BoxCollider(Rigidbody* rb) : Collider(rb) {}//con rigidbody
	BoxCollider(Rigidbody* rb, bool trigger) : Collider(rb, trigger) {}//con trigger

	//setter
	void SetSize(const core::Vector3<>& newSize) { size = newSize; }
	//getter
	core::Vector3<> GetSize() const { return size; }
};