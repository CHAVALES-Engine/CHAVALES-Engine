/*
 * @file
 * @brief
 */
#pragma once

#include <cmath>

namespace core
{
	/**
	 * @brief Vector bidimensional.
	 *
	 *		Clase que implementa un vector bidimensional.
	 */
	template <typename T = float>
	class Vector2
	{
	public:
		// -- Constructores
		/**
		 * @brief Constructor vacio.
		 *
		 *		Constructor vacio, se inicializan x e y a 0.
		 */
		Vector2() :
			_x(0), _y(0)
		{
		}

		/**
		 * @brief Constructor de un solo parametro.
		 *
		 *		Constructor de un solo parametro, se inicializan todas las componentes con el.
		 * @param a - Escalar con el que inicializar a todas las componentes.
		 */
		Vector2(T a) :
			_x(a), _y(a)
		{
		}

		/**
		 * @brief Constructor de dos parametros.
		 *
		 * @param x - Escalar con el que inicializar la componente X.
		 * @param y - Escalar con el que inicializar la componente Y.
		 */
		Vector2(T x, T y) :
			_x(x), _y(y)
		{
		}

		/**
		 * @brief Constructor por copia.
		 *
		 * @param v - Vector a copiar.
		 */
		Vector2(const Vector2& v) :
			_x(v.getX()), _y(v.getY())
		{
		}

		/**
		 * @brief Constructor de movimiento.
		 *
		 * @param v - Vector del que transferir los datos.
		 */
		Vector2(Vector2&& v) noexcept :
			_x(v.getX()), _y(v.getY())
		{
		}

		~Vector2()
		{
		}

		// -- Getters
		/**
		 * @brief Getter de X.
		 */
		inline T getX() const
		{
			return _x;
		}

		// -- Getters
		/**
		 * @brief Getter de Y.
		 */
		inline T getY() const
		{
			return _y;
		}

		// -- Setters
		/**
		 * @brief Setter de X.
		 * 
		 * @param x - Valor a settear en X.
		 */
		inline void setX(T x)
		{
			_x = x;
		}

		/**
		 * @brief Setter de Y.
		 *
		 * @param y - Valor a settear en Y.
		 */
		inline void setY(T y)
		{
			_y = y;
		}

		/**
		 * @brief Setter de X e Y de un vector ya existente.
		 *
		 * @param x - Valor a settear en X.
		 * @param y - Valor a settear en Y.
		 */
		inline void set(T x, T y)
		{
			_x = x;
			_y = y;
		}

		/**
		 * @brief Setter de X e Y de un vector ya existente.
		 *
		 * @param v - vector del que sacar X e Y para settear.
		 */
		inline void set(const Vector2& v)
		{
			_x = v.getX();
			_y = v.getY();
		}

		// -- Operadores
		/**
		 * @brief Suma dos vectores.
		 *
		 * @param v - vector a sumar.
		 */
		inline Vector2 operator+(const Vector2& v)
		{
			return Vector2(_x + v._x, _y + v._y);
		}

		inline Vector2& operator+=(const Vector2& v)
		{
			return *this = *this + v;
		}

		/**
		 * @brief Resta dos vectores.
		 *
		 * @param v - vector a restar.
		 */
		inline Vector2 operator-(const Vector2& v)
		{
			return Vector2(_x - v._x, _y - v._y);
		}

		inline Vector2& operator-=(const Vector2& v)
		{
			return *this = *this - v;
		}

		/**
		 * @brief Producto escalar de dos vectores.
		 *
		 * @param v - vector a multiplicar.
		 */
		inline T operator*(const Vector2& v)
		{
			return _x * v.getX() + _y * v.getY();
		}

		inline Vector2& operator*=(const T& a)
		{
			return *this = *this * a;
		}

		/**
		 * @brief Multiplica un vector por un escalar.
		 *
		 * @param a - escalar a multiplicar.
		 */
		inline Vector2 operator*(T a)
		{
			return Vector2(_x * a, _y * a);
		}

		/**
		 * @brief Divide un vector por un escalar.
		 *
		 * @param a - escalar por el que dividir.
		 */
		inline Vector2 operator/(T a)
		{
			return Vector2(_x / a, _y / a);
		}

		inline Vector2& operator/=(const T& a)
		{
			return *this = *this / a;
		}

		/**
		 * @brief Devuelve true si los vectores son iguales.
		 *
		 * @param v - vector a comparar.
		 */
		inline bool operator==(const Vector2& v)
		{
			return (_x == v._x) && (_y == v._y);
		}

		/**
		 * @brief Devuelve true si los vectores son diferentes.
		 *
		 * @param v - vector a comparar.
		 */
		inline bool operator!=(const Vector2& v)
		{
			return !operator==(v);
		}

		/**
		 * @brief Asignacion.
		 *
		 * @param v - vector a asignar.
		 */
		inline Vector2& operator=(const Vector2& v)
		{
			_x = v.getX();
			_y = v.getY();

			return *this;
		}

		/**
		 * @brief Devuelve el modulo del vector.
		 */
		inline T magnitude()
		{
			return sqrt(pow(_x, 2) + pow(_y, 2));
		}

		/**
		 * @brief Devuelve el vector normalizado.
		 */
		inline Vector2 normalized()
		{
			return *this / magnitude();
		}

	private:
		/**
		 * @brief Componente X del vector.
		 */
		T _x;

		/**
		 * @brief Componente Y del vector.
		 */
		T _y;
	};
}
