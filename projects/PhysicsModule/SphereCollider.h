#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
private:
	float radio = 0.5f;

public:
	SphereCollider() = default;
	SphereCollider(Rigidbody* rb) : Collider(rb) {}
	SphereCollider(Rigidbody* rb, bool trigger) : Collider(rb, trigger) {}

	//setter
	void SetRadius(float r) { radio = r; }
	//getter
	float GetRadius() const { return radio; }
};