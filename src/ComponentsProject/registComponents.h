
#include <ComponentRegister.h>
#include "Transform.h"

static bool registBaseComponent() {
	ComponentRegister::instance().registComponent("Transform", []() {
		return std::make_unique<core::Transform>(); });
	return true;
}
