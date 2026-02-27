/*
 * @file
 * @brief
 */
#pragma once
#include "Vector4.h"

namespace core
{
	// -- Constantes
	static const Vector4 BLACK = Vector4(0.0, 0.0, 0.0, 1.0);
	static const Vector4 WHITE = Vector4(1.0, 1.0, 1.0, 1.0);
	static const Vector4 CLEAR = Vector4(0.0, 0.0, 0.0, 0.0);
	static const Vector4 RED = Vector4(1.0, 0.0, 0.0, 1.0);
	static const Vector4 GREEN = Vector4(0.0, 1.0, 0.0, 1.0);
	static const Vector4 BLUE = Vector4(0.0, 0.0, 1.0, 1.0);
	static const Vector4 CYAN = Vector4(0.0, 1.0, 1.0, 1.0);
	static const Vector4 MAGENTA = Vector4(1.0, 0.0, 1.0, 1.0);
	static const Vector4 YELLOW = Vector4(1.0, 0.92, 0.016, 1.0);

	/**
	 * @brief Color que representa un color en RGBA.
	 *
	 *		Clase para representar un color en formato RGBA
	 *		rojo (r), verde (g), azul (b), and alfa (a).
	 *		Cada atributo es un float entre 0.0 y 1.0.
	 */
	class Color : public Vector4<float>
	{
	public:
		Color(float r, float g, float b, float a) :
			Vector4(r, g, b, a)
		{
		}

		// -- Getters
		/**
		 * @brief Getter de R (red).
		 */
		inline float getRed() const
		{
			return getX();
		}

		/**
		 * @brief Getter de G (green).
		 */
		inline float getGreen() const
		{
			return getY();
		}

		/**
		 * @brief Getter de B (blue).
		 */
		inline float getBlue() const
		{
			return getZ();
		}

		/**
		 * @brief Getter de A (alpha).
		 */
		inline float getAlpha() const
		{
			return getW();
		}
	};
}
