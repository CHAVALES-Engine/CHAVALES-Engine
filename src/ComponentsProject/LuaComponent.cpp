#include "LuaComponent.h"

LuaComponent::~LuaComponent()
{
}

bool LuaComponent::init(const Properties& p)
{
	return Component::init(p);
}

void LuaComponent::ready()
{
	Component::ready();
}
