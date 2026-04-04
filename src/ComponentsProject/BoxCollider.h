#pragma once
#include "../../src/Core-Defs/Vector3.h"
#include "Collider.h"


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