#pragma once
#include <unordered_map>
#include <variant>
#include <string>

/*
#include "Color.h"
#include "Quaternion.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
*/

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)	// exportando desde engine
#else
#define ENGINE_API __declspec(dllimport)	// consumiendo desde fuera
#endif

using Property = std::variant<
	int,
	float,
	bool,
	std::string/*,
	core::Vector2<float>,
	core::Vector3<float>,
	core::Vector4<float>,
	core::Quaternion<float>,
	core::Color*/
	//...
>;
using Properties = std::unordered_map<std::string, Property>;