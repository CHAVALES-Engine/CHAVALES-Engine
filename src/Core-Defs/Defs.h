#pragma once

#include <variant>
#include <unordered_map>

#include "Color.h"
#include "Quaternion.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

using Property = std::variant<
	int,
	float,
	bool,
	std::string,
	core::Vector2<>,
	core::Vector3<>,
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
	std::vector<core::Color>
>;

using Properties = std::unordered_map<std::string, Property>;
