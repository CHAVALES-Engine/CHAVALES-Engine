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
	core::Color
>;

using Properties = std::unordered_map<std::string, Property>;