/**
 * @file
 * @brief
 */
#pragma once

#include <cassert>
#include <stdlib.h>
#include <cmath>
#include <random>

namespace core
{
	// Macros
	/** @brief
	*	Constante que representa PI.
	*/
#define M_PI 3.141592653589793
	/** @brief
	*	Constante que representa el infinito.
	*/
#define M_INFINITY INFINITY;
	/** @brief
	*	Constante de conversion de radianes a grados. Uso: [ deg = rad * rad2deg ].
	*/
#define M_RAD2DEG 360 / (M_PI * 2);
	/** @brief
	*	Constante de conversion de grados a radianes. Uso: [ rad = deg * deg2rad ].
	*/
#define M_DEG2RAD (M_PI * 2) / 360;

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

		/*
		* @brief Devuelve el Mersenne Twister encargado de la gestion de número nuevos
		*/
		static std::mt19937& getRandomGenerator() {
			static std::mt19937 randomGen(std::random_device{}());
			return randomGen;
		}

		/*
		* @brief Asigna la semilla al sistema de generación.
		* 
		* @param seed - Nueva semilla a asigar.
		*/
		static void setRandomSeed(unsigned int seed) {
			getRandomGenerator().seed(seed);
		}

		/*
		* @brief Devuelve un entero aleatorio entre min y max, ambos incluidos. [min,max]
		*
		* @param min - límite inferior del Random.
		* @param max - límite superior del Random.
		*/
		static int Random(int min, int max) {
			std::uniform_int_distribution<int> dist(min, max);
			return dist(getRandomGenerator());
		}

		/*
		* @brief Devuelve un float aleatorio entre min y max,exclusivos. [min,max)
		*
		* @param min - límite inferior del Random.
		* @param max - límite superior del Random.
		*/
		static float Random(float min, float max) {
			std::uniform_real_distribution<float> dist(min, max);
			return dist(getRandomGenerator());
		}

		/*
		* @brief Devuelve un double aleatorio entre min y max,  exclusivos. [min,max)
		*
		* @param min - límite inferior del Random.
		* @param max - límite superior del Random.
		*/
		static double Random(double min, double max) {
			std::uniform_real_distribution<double> dist(min, max);
			return dist(getRandomGenerator());
		}
		/*
		* @brief Devuelve un double aleatorio entre 0.0 y 1.0,  exclusivos. [0.0,1.0)
		*/
		static double Random() {
			std::uniform_real_distribution<double> dist;
			return dist(getRandomGenerator());
		}
	};
}
