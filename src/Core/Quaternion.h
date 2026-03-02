#pragma once
#include <cmath>
#include "Vector3.h"

namespace core
{
	template <typename T = float>
	class Quaternion
	{
	private:
		T _x; T _y; T _z;//eje  * seno del angulo/2
		T _w;//cos del angulo/2
	public:
		Quaternion() : _x(0), _y(0), _z(0), _w(1) {}//identidad

		Quaternion(T x, T y, T z, T w) : _x(x), _y(y), _z(z), _w(w) {}

		Quaternion(const Quaternion& q) : _x(q._x), _y(q._y), _z(q._z), _w(q._w) {}

		//getters
		inline T getX() const { return _x; }
		inline T getY() const { return _y; }
		inline T getZ() const { return _z; }
		inline T getW() const { return _w; }

		//Longitud del quaternion
		float length() {
			return sqrt((pow(_w, 2) + pow(_x, 2) + pow(_y, 2) + pow(_z, 2)));
		}

		//Normalizacion de quaternion
		void normalize() {
			float len = length();

			_w = _w / len; _x = _x / len; _y = _y / len; _z = _z / len;
		}

		//Cambio de parte vectorial del quaternion (conjugate)
		void conjugate() {
			_x = -_x; _y = -_y; _z = -_z;
		}

		//Invierte el quaternion
		void inverse() {
			float len = length();

			if (len > 0.0f)
			{
				conjugate();

				_w /= len;
				_x /= len;
				_y /= len;
				_z /= len;
			}
		}

		//Metodo slerp (rota el quaternion desde la rotacion original hasta la nueva en el tiempo t)
		Quaternion slerp(const Quaternion& a, const Quaternion& b, float t) {
			float dot = a.dot(b);

			Quaternion b2 = b;
			if (dot < 0.0f)
			{
				dot = -dot;
				b2._w = -b2._w;
				b2._x = -b2._x;
				b2._y = -b2._y;
				b2._z = -b2._z;
			}

			float theta_0 = std::acos(dot);
			float theta = theta_0 * t;

			float sin_theta = std::sin(theta);
			float sin_theta_0 = std::sin(theta_0);

			float s0 = std::cos(theta) - dot * sin_theta / sin_theta_0;
			float s1 = sin_theta / sin_theta_0;

			return Quaternion(
				(a._x * s0) + (b2._x * s1),
				(a._y * s0) + (b2._y * s1),
				(a._z * s0) + (b2._z * s1),
				(a._w * s0) + (b2._w * s1)
			);
		}

		//Producto escalar
		inline float dot(const Quaternion& q) const
		{
			return _w * q._w + _x * q._x + _y * q._y + _z * q._z;
		}

		//rotate(Vector3) -> Añade el vector a la rotacion (rota el quaternion v grados).
		//fromAxisAngle -> Crea un quaternion a partir de un eje de rotacion y un angulo.
		//fromEuler -> Crea un quaternion a partir de angulos Euler.

	    //setters
		inline void set(T x, T y, T z, T w) { _x = x; _y = y; _z = z; _w = w; }

		//operadores
		Quaternion& operator=(const Quaternion& q) { //igualdad
			_x = q._x;
			_y = q._y;
			_z = q._z;
			_w = q._w;
			return *this;
		}

		inline Quaternion operator*(const Quaternion& q) const {//multiplicacion
			return Quaternion(
				_w * q._x + _x * q._w + _y * q._z - _z * q._y,
				_w * q._y - _x * q._z + _y * q._w + _z * q._x,
				_w * q._z + _x * q._y - _y * q._x + _z * q._w,
				_w * q._w - _x * q._x - _y * q._y - _z * q._z
			);
		}
	};
}

