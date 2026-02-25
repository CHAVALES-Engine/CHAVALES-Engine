/*
 * @file 
 * @brief 
 */
#pragma once

#include <cmath>

/**
 * @brief Vector cuatridimensional.
 *
 *		Clase que implementa un vector cuatridimensional.
 */
template <typename T = float>
class Vector4
{
public:
	// -- Constructores
	/**
	 * @brief Constructor vacio.
	 *
	 *		Constructor vacio, se inicializan x e y a 0.
	 */
	Vector4() :
		_x(0), _y(0), _z(0), _w(0) {
	}

	/**
	 * @brief Constructor de un solo parametro.
	 *
	 *		Constructor de un solo parametro, se inicializan todas las componentes con el.
	 * @param a - Escalar con el que inicializar a todas las componentes.
	 */
	Vector4(T a) :
		_x(a), _y(a), _z(a), _w(a) {
	}

	/**
	 * @brief Constructor de dos parametros.
	 *
	 * @param x - Escalar con el que inicializar la componente X.
	 * @param y - Escalar con el que inicializar la componente Y.
	 * @param z - Escalar con el que inicializar la componente Z.
	 * @param w - Escalar con el que inicializar la componente W.
	 */
	Vector4(T x, T y, T z, T w) :
		_x(x), _y(y), _z(z), _w(w) {
	}

	/**
	 * @brief Constructor por copia.
	 *
	 * @param v - Vector a copiar.
	 */
	Vector4(const Vector4& v) :
		_x(v.getX()), _y(v.getY()), _z(v.getZ()), _w(v.getW()) {
	}

	/**
	 * @brief Constructor de movimiento.
	 *
	 * @param v - Vector del que transferir los datos.
	 */
	Vector4(Vector4&& v) noexcept :
		_x(v.getX()), _y(v.getY()), _z(v.getZ()), _w(v.getW())
	{
	}

	~Vector4()
	{
	}

	// -- Getters
	/**
	 * @brief Getter de X.
	 */
	inline T getX() const {
		return _x;
	}

	/**
	 * @brief Getter de Y.
	 */
	inline T getY() const {
		return _y;
	}

	/**
	 * @brief Getter de Z.
	 */
	inline T getZ() const {
		return _z;
	}

	/**
	 * @brief Getter de W.
	 */
	inline T getW() const {
		return _w;
	}

	// -- Setters
	/**
	 * @brief Setter de X.
	 *
	 * @param x - Valor a settear en X.
	 */
	inline void setX(T x) {
		_x = x;
	}

	/**
	 * @brief Setter de Y.
	 *
	 * @param y - Valor a settear en Y.
	 */
	inline void setY(T y) {
		_y = y;
	}

	/**
	 * @brief Setter de Z.
	 *
	 * @param z - Valor a settear en Z.
	 */
	inline void setZ(T z) {
		_z = z;
	}

	/**
	 * @brief Setter de W.
	 *
	 * @param w - Valor a settear en W.
	 */
	inline void setW(T w) {
		_w = w;
	}

	/**
	 * @brief Setter de X Y Z W de un vector ya existente.
	 *
	 * @param x - Valor a settear en X.
	 * @param y - Valor a settear en Y.
	 * @param z - Valor a settear en Z.
	 * @param w - Valor a settear en W.
	 */
	inline void set(T x, T y, T z) {
		_x = x;
		_y = y;
		_z = z;
	}

	/**
	 * @brief Setter de X Y Z W de un vector ya existente.
	 *
	 * @param v - vector del que sacar X Y Z W para settear.
	 */
	inline void set(const Vector4& v) {
		_x = v.getX();
		_y = v.getY();
		_z = v.getZ();
		_w = v.getW();
	}

	// -- Operadores
	/**
	 * @brief Suma dos vectores.
	 *
	 * @param v - vector a sumar.
	 */
	inline Vector4& operator+(const Vector4& v)
	{
		return Vector4(_x + v._x, _y + v._y, _z + v._z, _w + v._w);
	}

	/**
	 * @brief Resta dos vectores.
	 *
	 * @param v - vector a restar.
	 */
	inline Vector4& operator-(const Vector4& v)
	{
		return Vector4(_x - v._x, _y - v._y, _z - v._z, _w - v._w);
	}

	/**
	 * @brief Producto escalar de dos vectores.
	 *
	 * @param v - vector a multiplicar.
	 */
	inline T operator*(const Vector4& v)
	{
		return _x * v.getX() + _y * v.getY() + _z * v.getZ() + _w * v.getW();
	}

	/**
	 * @brief Multiplica un vector por un escalar.
	 *
	 * @param a - escalar a multiplicar.
	 */
	inline Vector4& operator*(T a)
	{
		return Vector4(_x * a, _y * a, _z * a, _w * a);
	}

	/**
	 * @brief Divide un vector por un escalar.
	 *
	 * @param a - escalar por el que dividir.
	 */
	inline Vector4& operator/(T a)
	{
		return Vector4(_x / a, _y / a, _z / a, _w / a);
	}

	/**
	 * @brief Devuelve true si los vectores son iguales.
	 *
	 * @param v - vector a comparar.
	 */
	inline bool operator==(const Vector4& v)
	{
		return (_x == v._x) && (_y == v._y) && (_z == v._z) && (_w == v._w);
	}

	/**
	 * @brief Devuelve true si los vectores son diferentes.
	 *
	 * @param v - vector a comparar.
	 */
	inline bool operator!=(const Vector4& v)
	{
		return !operator==(v);
	}

	/**
	 * @brief Asignacion.
	 *
	 * @param v - vector a asignar.
	 */
	inline Vector4& operator=(const Vector4& v)
	{
		_x = v.getX();
		_y = v.getY();
		_z = v.getZ();
		_w = v.getW();

		return *this;
	}

	/**
	 * @brief Devuelve el modulo del vector.
	 */
	inline T magnitude()
	{
		return sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2) + pow(_w, 2));
	}

	/**
	 * @brief Devuelve el vector normalizado.
	 */
	inline Vector4 normalized()
	{
		return *this / magnitude();
	}

private:
	/**
	 * @brief Componente X del vector.
	 */
	T  _x;

	/**
	 * @brief Componente Y del vector.
	 */
	T  _y;

	/**
	 * @brief Componente Z del vector.
	 */
	T  _z;

	/**
	 * @brief Componente W del vector.
	 */
	T  _w;
};