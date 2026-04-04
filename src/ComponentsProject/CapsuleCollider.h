#pragma once
#include "Collider.h"

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