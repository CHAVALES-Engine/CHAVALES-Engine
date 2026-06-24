/**
 * @file
 * @brief
 */

#pragma once
#include <memory>
#include <variant>
#include <string>
#include <vector>
#include <unordered_map>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Color.h"
#include "InputDefs.h"
#include "PhysicsTypes.h" 
 // Forward declarations
namespace core {
	class Entity;
	class Scene;
}

using Property = std::variant<
	int,
	float,
	bool,
	std::string,
	core::Vector2<>,
	core::Vector3<>,
	core::Vector3<bool>,
	core::Vector4<>,
	core::Quaternion<>,
	core::Color,
	const char*,
	std::vector<int>,
	std::vector<float>,
	std::vector<bool>,
	std::vector<std::string>,
	std::vector<core::Vector2<>>,
	std::vector<core::Vector3<>>,
	std::vector<core::Vector4<>>,
	std::vector<core::Quaternion<>>,
	std::vector<core::Color>,
	// INPUT
	input::Key,
	input::MouseButton,
	input::MouseAxis,
	input::GamepadButton,
	input::GamepadAxis,
	std::pair<bool,input::DeviceID>,
	std::pair<float,input::DeviceID>,
	// ENGINE
	core::Entity*,
	std::shared_ptr<core::Scene>,
	RayInfo
>;

using Properties = std::unordered_map<std::string, Property>;