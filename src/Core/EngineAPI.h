#pragma once
#include <unordered_map>
#include <variant>
#include <string>

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)	// exportando desde engine
#else
#define ENGINE_API __declspec(dllimport)	// consumiendo desde fuera
#endif

using Property = std::variant<
	int,
	float,
	bool,
	std::string
	//Vector3
	//Vector4
	//Quaternion
	//Color
	//...
>;
using Properties = std::unordered_map<std::string, Property>;