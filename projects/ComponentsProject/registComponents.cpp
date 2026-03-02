
#include <Component.h>
#include <ComponentRegister.h>
#include "Transform.h"

static bool registBaseComponent() {
	ComponentRegister::registComponent("Transform", []() {
		return std::make_unique<core::Transform>(); });
}
