#pragma once
#include "Vector3.h"
#include "Maths.h"

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

		//identidad
		inline Quaternion identity() {
			return Quaternion();
		}

		//angulo en grados entre dos rotaciones
		inline float angle(Quaternion a, Quaternion b) {
			float d = dot(b);
			d = Clamp(d, -1.0f, 1.0f);
			float angleRad = 2.0f * acos(fabs(d));
			return angleRad * (180.0f / M_PI);
		}

		//crea quaternion a partir de angulo y eje
		inline Quaternion angleAxis(float angle, Vector3<> axis) {
			float angleRad = angle * M_PI / 180.0f;//grados a radianes
			Vector3<> n = axis.normalized();
			return Quaternion(n.getX() * Maths::Sin(angleRad / 2), n.getY() * Maths::Sin(angleRad / 2), n.getZ() * Maths::Sin(angleRad / 2), Maths::Cos(angleRad / 2));
		}

		//Longitud del quaternion
		inline float length() {
			return sqrt((pow(_w, 2) + pow(_x, 2) + pow(_y, 2) + pow(_z, 2)));
		}

		//Normalizacion de quaternion
		inline void normalize() {
			float len = length();

			_w = _w / len; _x = _x / len; _y = _y / len; _z = _z / len;
		}

		//Cambio de parte vectorial del quaternion (conjugate)
		inline void conjugate() {
			_x = -_x; _y = -_y; _z = -_z;
		}

		//Invierte el quaternion
		inline void inverse() {
			float len = length();

			if (len > 0.0f){
				conjugate();

				_w /= len;
				_x /= len;
				_y /= len;
				_z /= len;
			}
		}

		inline Quaternion Lerp(Quaternion a, Quaternion b, float t) {
			t = Maths::Clamp(t, 0.0f, 1.0f);//control de t
			return a + (b - a) * t;
		}

		//Metodo slerp (rota el quaternion desde la rotacion original hasta la nueva en el tiempo t)
		inline Quaternion slerp(const Quaternion& a, const Quaternion& b, float t) {
			float dot = a.dot(b);

			Quaternion b2 = b;
			if (dot < 0.0f) {
				dot = -dot;
				b2._w = -b2._w;
				b2._x = -b2._x;
				b2._y = -b2._y;
				b2._z = -b2._z;
			}

			float theta_0 = acos(dot);
			float theta = theta_0 * t;

			float sin_theta = Maths::Sin(theta);
			float sin_theta_0 = Maths::Sin(theta_0);

			float s0 = std::cos(theta) - dot * sin_theta / sin_theta_0;
			float s1 = sin_theta / sin_theta_0;

			return Quaternion((a._x * s0) + (b2._x * s1), (a._y * s0) + (b2._y * s1), (a._z * s0) + (b2._z * s1), (a._w * s0) + (b2._w * s1));
		}

		//Producto escalar
		inline float dot(const Quaternion& q) const {
			return _w * q._w + _x * q._x + _y * q._y + _z * q._z;
		}

		inline void rotate(Vector3<> v) {
			//cada comp a un quaternion
			Quaternion qx = Quaternion::AngleAxis(v.getX(), core::Vector3(1.0f, 0.0f, 0.0f));
			Quaternion qy = Quaternion::AngleAxis(v.getY(), core::Vector3(0.0f, 1.0f, 0.0f));
			Quaternion qz = Quaternion::AngleAxis(v.getZ(), core::Vector3(0.0f, 0.0f, 1.0f));

			*this = qz * qy * qx * (*this);
			*this = this->normalized();
		}

		//conversion de un quaternion
		inline Vector3<> Euler() {
			Vector3<> euler;
			//componentes
			float sinr_cosp = 2.0f * (_w * _x + _y * _z);
			float cosr_cosp = 1.0f - 2.0f * (_x * _x + _y * _y);
			euler.setX(std::atan2(sinr_cosp, cosr_cosp) * 180.0f / M_PI);

			float sinp = 2.0f * (_w * _y - _z * _x);
			if (std::fabs(sinp) >= 1.0f)
				euler.setY(std::copysign(90.0f, sinp));
			else
				euler.setY(std::asin(sinp) * 180.0f / M_PI);

			float siny_cosp = 2.0f * (_w * _z + _x * _y);
			float cosy_cosp = 1.0f - 2.0f * (_y * _y + _z * _z);
			euler.setZ(std::atan2(siny_cosp, cosy_cosp) * 180.0f / M_PI);

			return euler;
		}

		//operadores
		inline Quaternion& operator=(const Quaternion& q) { //igualdad
			_x = q._x;
			_y = q._y;
			_z = q._z;
			_w = q._w;
			return *this;
		}

		inline bool operator==(const Quaternion& q) { //igualdad

			return (_x == q.getX() && _y == q.getY() && _z == q.getZ() && _w == q.getW());
		}

		inline bool operator!=(const Quaternion& q) { //igualdad

			return (_x != q.getX() || _y != q.getY() || _z != q.getZ() || _w != q.getW());
		}


		inline Quaternion operator*(const Quaternion& q) const {//multiplicacion
			return Quaternion(
				_w * q._x + _x * q._w + _y * q._z - _z * q._y, // x
				_w * q._y - _x * q._z + _y * q._w + _z * q._x, // y
				_w * q._z + _x * q._y - _y * q._x + _z * q._w, // z
				_w * q._w - _x * q._x - _y * q._y - _z * q._z  // w
			);
		}

		inline Quaternion operator*(float a) const {//multiplicacion por numero
			return Quaternion(
				_x * a, _y * a, _z * a, _w * a);
		}

		inline Quaternion operator+(const Quaternion& q) const {//suma
			return Quaternion(_x + q._x, _y + q._y, _z + q._z, _w + q._w);
		}

		inline Quaternion operator-(const Quaternion& q) const {//resta
			return Quaternion(_x - q._x, _y - q._y, _z - q._z, _w - q._w);
		}
	};
}

