#include "ComponentsRegister.h"

ComponentsRegister::ComponentsRegister()
{
}

ComponentsRegister::~ComponentsRegister()
{
}

bool ComponentsRegister::regist(const std::string& name, const ComponentFactory& factory)
{
	
	//components[name] = factory.create;
	return true;
}

bool ComponentsRegister::create(const std::string& name)
{
	//return components[name]()
}
