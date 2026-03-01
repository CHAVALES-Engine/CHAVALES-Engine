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

