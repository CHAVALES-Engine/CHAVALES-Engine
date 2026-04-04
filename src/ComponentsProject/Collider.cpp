#include "Collider.h"
#include "PluginSDK.h"

REGISTER_COMPONENT(Collider);



bool Collider::init(const Properties& p)
{
	//cosas de lua
	return false;
}

void Collider::ready()
{

//en el ready miro si tiene un rigidbody real y si lo tiene le paso al metodo los datos del rigidbody
}

void Collider::update(uint64_t deltaTime)
{
}
