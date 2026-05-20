#pragma once
/**
 * @file PropertyConverters.h
 * @brief Conversores entre sol::object (Lua) y Property (C++)
 *
 * Permite pasar tipos complejos (Vector3, Color, ...) entre Lua y C++
 */
#pragma once

#include "ScriptsManager.h"
#include "Defs.h"
#include <sol/sol.hpp>

namespace ScriptManager
{
	// ======= sol::object -> Property =======
	/**
	 * @brief Convierte un sol::object a Property.
	 */
	static Property solObjectToProperty(const sol::object& obj)
	{
		if (!obj.valid()) {
			return Property(0);
		}

		// Tipos simples
		if (obj.is<int>()) {
			return Property(std::in_place_type<int>, obj.as<int>());
		}
		if (obj.is<float>()) {
			return Property(obj.as<float>());
		}
		if (obj.is<double>()) {
			return Property(obj.as<float>());  // Convertir double -> float
		}
		if (obj.is<bool>()) {
			return Property(std::in_place_type<bool>, obj.as<bool>());
		}
		if (obj.is<std::string>()) {
			return Property(obj.as<std::string>());
		}
		if (obj.is<const char*>()) {
			return Property(std::string(obj.as<const char*>()));
		}

		// Tipos complejos del motor
		if (obj.is<core::Vector2<>>()) {
			return Property(obj.as<core::Vector2<>>());
		}
		if (obj.is<core::Vector3<>>()) {
			return Property(obj.as<core::Vector3<>>());
		}
		if (obj.is<core::Vector3<bool>>()) {
			return Property(obj.as<core::Vector3<bool>>());
		}
		if (obj.is<core::Vector4<>>()) {
			return Property(obj.as<core::Vector4<>>());
		}
		if (obj.is<core::Quaternion<>>()) {
			return Property(obj.as<core::Quaternion<>>());
		}
		if (obj.is<core::Color>()) {
			return Property(obj.as<core::Color>());
		}

		// Vectores
		if (obj.is<std::vector<int>>()) {
			return Property(obj.as<std::vector<int>>());
		}
		if (obj.is<std::vector<float>>()) {
			return Property(obj.as<std::vector<float>>());
		}
		if (obj.is<std::vector<bool>>()) {
			return Property(obj.as<std::vector<bool>>());
		}
		if (obj.is<std::vector<std::string>>()) {
			return Property(obj.as<std::vector<std::string>>());
		}
		if (obj.is<std::vector<core::Vector2<>>>()) {
			return Property(obj.as<std::vector<core::Vector2<>>>());
		}
		if (obj.is<std::vector<core::Vector3<>>>()) {
			return Property(obj.as<std::vector<core::Vector3<>>>());
		}
		if (obj.is<std::vector<core::Vector4<>>>()) {
			return Property(obj.as<std::vector<core::Vector4<>>>());
		}
		if (obj.is<std::vector<core::Quaternion<>>>()) {
			return Property(obj.as<std::vector<core::Quaternion<>>>());
		}
		if (obj.is<std::vector<core::Color>>()) {
			return Property(obj.as<std::vector<core::Color>>());
		}

		// Tipo desconocido
		Debug::warning("[ScriptsManager] Tipo Lua no soportado en Property");
		return Property(0);
	}

	// ======= Property -> sol::object =======
	/**
	 * @brief Convierte un Property a sol::object para retornar a Lua.
	 * @param prop - Property a convertir
	 * @param state - sol::state para crear el objeto
	 */
	static sol::object propertyToSolObject(const Property& prop, sol::this_state state)
	{
		try {
			// int
			if (auto* v = std::get_if<int>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			// float
			if (auto* v = std::get_if<float>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			// bool
			if (auto* v = std::get_if<bool>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			// std::string
			if (auto* v = std::get_if<std::string>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			// const char*
			if (auto* v = std::get_if<const char*>(&prop)) {
				return sol::object(state, sol::in_place, std::string(*v));
			}
			// Vector2
			if (auto* v = std::get_if<core::Vector2<>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			// Vector3
			if (auto* v = std::get_if<core::Vector3<>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			// Vector3<bool>
			if (auto* v = std::get_if<core::Vector3<bool>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			// Vector4
			if (auto* v = std::get_if<core::Vector4<>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			// Quaternion
			if (auto* v = std::get_if<core::Quaternion<>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			// Color
			if (auto* v = std::get_if<core::Color>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			// Vectores
			if (auto* v = std::get_if<std::vector<int>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			if (auto* v = std::get_if<std::vector<float>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			if (auto* v = std::get_if<std::vector<bool>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			if (auto* v = std::get_if<std::vector<std::string>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			if (auto* v = std::get_if<std::vector<core::Vector2<>>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			if (auto* v = std::get_if<std::vector<core::Vector3<>>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			if (auto* v = std::get_if<std::vector<core::Vector4<>>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			if (auto* v = std::get_if<std::vector<core::Quaternion<>>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			if (auto* v = std::get_if<std::vector<core::Color>>(&prop)) {
				return sol::object(state, sol::in_place, *v);
			}
			if (auto* v = std::get_if<core::Entity*>(&prop)) {
				return sol::object(state, sol::in_place, *v);    // Usertype Entity
			}
			if (auto* v = std::get_if<std::shared_ptr<core::Scene>>(&prop)) {
				return sol::object(state, sol::in_place, *v);    // Usertype Scene
			}
			if (auto* v = std::get_if<RayInfo>(&prop)) {
				return sol::object(state, sol::in_place, *v);    // Usertype RayInfo
			}
		}
		catch (const std::exception& e) {
			Debug::error("[ScriptsManager] Error convirtiendo Property a sol::object: ", e.what());
		}

		return sol::nil;
	}

	// ======= sol::object -> std::any =======
	/**
	 * @brief Convierte un sol::object a std::any.
	 * Usado para pasar argumentos a Component::call que recibe vector<std::any>.
	 */
	static std::any solObjectToAny(const sol::object& obj)
	{
		if (!obj.valid()) return std::any();

		// Tipos simples
		if (obj.is<bool>())   return std::any(obj.as<bool>());
		if (obj.is<int>())    return std::any(obj.as<int>());
		if (obj.is<float>())  return std::any(obj.as<float>());
		if (obj.is<double>()) return std::any(static_cast<float>(obj.as<double>()));
		if (obj.is<std::string>()) return std::any(obj.as<std::string>());

		// Tipos complejos
		if (obj.is<core::Vector2<>>())    return std::any(obj.as<core::Vector2<>>());
		if (obj.is<core::Vector3<>>())    return std::any(obj.as<core::Vector3<>>());
		if (obj.is<core::Vector4<>>())    return std::any(obj.as<core::Vector4<>>());
		if (obj.is<core::Quaternion<>>()) return std::any(obj.as<core::Quaternion<>>());
		if (obj.is<core::Color>())        return std::any(obj.as<core::Color>());

		// Strings literales
		if (obj.is<const char*>()) return std::any(std::string(obj.as<const char*>()));

		Debug::warning("[ScriptsManager] Tipo Lua no convertible a std::any");
		return std::any();
	}
	/**
	 * @brief Convierte argumentos Lua a vector<std::any> para Component::call.
	 */
	static std::vector<std::any> solArgsToAnyVector(const sol::variadic_args& args)
	{
		std::vector<std::any> result;
		result.reserve(args.size());
		for (const auto& arg : args)
			result.push_back(solObjectToAny(arg));
		return result;
	}

	// ======= std::any -> Property =======
	/**
	 * @brief Convierte std::any a Property intentando castear a tipos conocidos.
	 * Para Component::call que retorna std::any.
	 */
	static Property anyToProperty(const std::any& any_value)
	{
		if (any_value.has_value()) {
			// Tipos simples
			if (auto* b = std::any_cast<bool>(&any_value)) {
				return Property(std::in_place_type<bool>, *b);
			}
			if (auto* i = std::any_cast<int>(&any_value)) {
				return Property(std::in_place_type<int>, *i);
			}
			if (auto* f = std::any_cast<float>(&any_value)) {
				return Property(*f);
			}
			if (auto* d = std::any_cast<double>(&any_value)) {
				return Property(static_cast<float>(*d));
			}
			if (auto* s = std::any_cast<std::string>(&any_value)) {
				return Property(*s);
			}
			if (auto* c = std::any_cast<const char*>(&any_value)) {
				return Property(std::string(*c));
			}

			// Tipos complejos
			if (auto* v3 = std::any_cast<core::Vector3<>>(&any_value)) {
				return Property(*v3);
			}
			if (auto* v2 = std::any_cast<core::Vector2<>>(&any_value)) {
				return Property(*v2);
			}
			if (auto* v4 = std::any_cast<core::Vector4<>>(&any_value)) {
				return Property(*v4);
			}
			if (auto* q = std::any_cast<core::Quaternion<>>(&any_value)) {
				return Property(*q);
			}
			if (auto* col = std::any_cast<core::Color>(&any_value)) {
				return Property(*col);
			}
			if (auto* vb3 = std::any_cast<core::Vector3<bool>>(&any_value)) {
				return Property(*vb3);
			}

			// Vectores
			if (auto* vi = std::any_cast<std::vector<int>>(&any_value)) {
				return Property(*vi);
			}
			if (auto* vf = std::any_cast<std::vector<float>>(&any_value)) {
				return Property(*vf);
			}
			if (auto* vb = std::any_cast<std::vector<bool>>(&any_value)) {
				return Property(*vb);
			}
			if (auto* vs = std::any_cast<std::vector<std::string>>(&any_value)) {
				return Property(*vs);
			}
			if (auto* vv2 = std::any_cast<std::vector<core::Vector2<>>>(&any_value)) {
				return Property(*vv2);
			}
			if (auto* vv3 = std::any_cast<std::vector<core::Vector3<>>>(&any_value)) {
				return Property(*vv3);
			}
			if (auto* vv4 = std::any_cast<std::vector<core::Vector4<>>>(&any_value)) {
				return Property(*vv4);
			}
			if (auto* vq = std::any_cast<std::vector<core::Quaternion<>>>(&any_value)) {
				return Property(*vq);
			}
			if (auto* vcol = std::any_cast<std::vector<core::Color>>(&any_value)) {
				return Property(*vcol);
			}
		}

		Debug::warning("[ScriptsManager] std::any contiene tipo no soportado o esta vacio");
		return Property(0);
	}
}