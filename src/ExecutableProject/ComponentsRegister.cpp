#include "ComponentsRegister.h"
#include <Debug.h>
#include <Component.h>


bool ComponentsRegister::regist(const std::string& name, ec::ComponentFactory factory)
{	
	_components[name] = factory;
	return true;
}


ec::Component* ComponentsRegister::create(const std::string& name)
{
	return _components[name]();
}
