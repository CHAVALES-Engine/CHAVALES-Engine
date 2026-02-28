/*
 * @file
 * @brief
 */
#pragma once

#include <cassert>
#include <stdlib.h>
#include <cmath>

namespace core
{
	// Macros
#define M_PI 3.141592653589793
#define M_INFINITY INFINITY;
#define M_RAD2DEG 360 / (M_PI * 2); // constante de conversion de radianes a grados [ deg = rad * rad2deg ]

	/**
	 * @brief Funciones matematicas.
	 *
	 *		Clase que implementa diferentes funciones y utilidades matematicas.
	 */
	class Maths
	{
	public:
		Maths() = default;
		~Maths() = default;

		// -- Operaciones
		/**
		 * @brief Devuelve el maximo entre a y b.
		 *
		 * @param a - numero a comparar.
		 * @param b - numero a comparar.
		 */
		template <class T>
		static T Max(T a, T b)
		{
			return a < b ? b : a;
		}

		/**
		 * @brief Devuelve el minimo entre a y b.
		 *
		 * @param a - numero a comparar.
		 * @param b - numero a comparar.
		 */
		template <class T>
		static T Min(T a, T b)
		{
			return a < b ? a : b;
		}

		/**
		 * @brief Devuelve el valor absoluto de a.
		 *
		 * @param a - numero con el que operar.
		 */
		template <class T>
		static T Abs(T a)
		{
			return std::abs(a);
		}

		/**
		 * @brief Devuelve el valor saturado entre minimo y maximo.
		 *
		 * @param a - numero con el que operar.
		 * @param min - cota minima.
		 * @param max - cota maxima.
		 */
		template <class T>
		static T Clamp(T a, T min, T max)
		{
			assert(min <= max);

			return Min(max, Max(min, a));
		}

		/**
		 * @brief Devuelve la raiz cuadrada de a.
		 *
		 * @param a - numero con el que operar.
		 */
		template <class T>
		static float Sqrt(T a)
		{
			return std::sqrtf((float)a);
		}

		/**
		 * @brief Devuelve a redondeado hacia abajo.
		 *
		 * @param a - numero con el que operar.
		 */
		static float Floor(float a)
		{
			return std::floorf(a);
		}

		/**
		 * @brief Devuelve a redondeado hacia arriba.
		 *
		 * @param a - numero con el que operar.
		 */
		static float Ceil(float a)
		{
			return std::ceilf(a);
		}

		/**
		 * @brief Devuelve a elevado a p.
		 *
		 * @param a - numero que elevar.
		 * @param p - potencia.
		 */
		static float Pow(float a, float p)
		{
			return std::powf(a, p);
		}

		/**
		 * @brief Devuelve e elevado a p.
		 *
		 * @param p - potencia.
		 */
		static float Exp(float p)
		{
			return std::expf(p);
		}

		/**
		 * @brief Devuelve el logaritmo de a en base b.
		 *
		 * @param a - numero con el que operar.
		 * @param b - base del logaritmo.
		 */
		static float Log(float a, float b)
		{
			return std::log(a) / std::log(b);
		}

		/**
		 * @brief Devuelve el logaritmo de a en base 10.
		 *
		 * @param a - numero con el que operar.
		 */
		static float Log10(float a)
		{
			return std::log(a);
		}

		// -- Trigonometria
		/**
		 * @brief Devuelve el coseno del angulo a en radianes.
		 *
		 * @param a - angulo en radianes.
		 */
		static float Cos(float a)
		{
			return cosf(a);
		}

		/**
		 * @brief Devuelve el seno del angulo a en radianes.
		 *
		 * @param a - angulo en radianes.
		 */
		static float Sin(float a)
		{
			return sinf(a);
		}

		/**
		 * @brief Devuelve la tangente del angulo a en radianes.
		 *
		 * @param a - angulo en radianes.
		 */
		static float Tan(float a)
		{
			return tanf(a);
		}

		/**
		 * @brief Interpola linealmente entre a y b en funcion de t.
		 *
		 *		El parametro t esta en el rango [0,1] 
		 *		Cuando t = 0 devuelve a.
		 *		Cuando t = 1 devuelve b.
		 *		Cuando t = 0.5 devuelve el valor intermedio entre a y b.
		 *	
		 * @param a - inicio de la interpolacion.
		 * @param b - final de la interpolacion.
		 * @param t - factor de interpolacion en el rango [0,1].
		 */
		static float Lerp(float a, float b, float t)
		{
			return a + t * (b - a);
		}
	};
}
