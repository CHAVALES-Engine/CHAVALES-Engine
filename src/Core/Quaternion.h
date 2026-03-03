/**
 * @class Quaternion
 * @brief Representa una rotación en 3D mediante quaternions.
 *
 * Contiene operaciones básicas como normalización, inversa, producto,
 * conversión a Euler, interpolaciones (Lerp, Slerp) y creación a partir de
 * ángulo y eje.
 */
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

		/**
		* @brief Quaternion identidad
		* @return Quaternion identidad
		*/
		inline Quaternion identity() {
			return Quaternion();
		}

		/**
		* @brief Angulo en grados entre dos rotaciones
		* @return angulo entre dos quaternion
		*/
		inline float angle(Quaternion a, Quaternion b) {
			float d = dot(b);
			d = Clamp(d, -1.0f, 1.0f);
			float angleRad = 2.0f * acos(fabs(d));
			return angleRad * (180.0f / M_PI);
		}

		/**
		* @brief Crea un quaternion a partir de un ángulo y un eje.
		* @param angle Ángulo en grados
		* @param axis Eje de rotación (Vector3) que será normalizado
		* @return Quaternion que representa la rotación
		*/
		inline Quaternion angleAxis(float angle, Vector3<> axis) {
			float angleRad = angle * M_PI / 180.0f;//grados a radianes
			Vector3<> n = axis.normalized();
			return Quaternion(n.getX() * Maths::Sin(angleRad / 2), n.getY() * Maths::Sin(angleRad / 2), n.getZ() * Maths::Sin(angleRad / 2), Maths::Cos(angleRad / 2));
		}

		/**
		* @brief Longitud del quaternion
		*/
		inline float length() {
			return sqrt((pow(_w, 2) + pow(_x, 2) + pow(_y, 2) + pow(_z, 2)));
		}

		/**
		* @brief Normalizacion del quaterion
		*/
		inline void normalize() {
			float len = length();

			_w = _w / len; _x = _x / len; _y = _y / len; _z = _z / len;
		}

		/**
		* @brief Cambio de parte vectorial del quaternion (conjugate)
		*/
		inline void conjugate() {
			_x = -_x; _y = -_y; _z = -_z;
		}

		/**
		* @brief Invierte el quaternion
		* @return Quaternion invertido
		*/
		inline void inverse() {
			float len = length();

			if (len > 0.0f) {
				conjugate();

				_w /= len;
				_x /= len;
				_y /= len;
				_z /= len;
			}
		}

		/**
		* @brief Metodo lerp (busca el punto intermedio interpolando entre a y b en tiempo t)
		* @param a Quaternion rotacion inicial, b quaternion rotacion final, t tiempo de rotacion
		* @return Quaternion final
		*/
		inline Quaternion Lerp(Quaternion a, Quaternion b, float t) {
			t = Maths::Clamp(t, 0.0f, 1.0f);//control de t
			return a + (b - a) * t;
		}

		/**
		* @brief Metodo slerp (rota el quaternion desde la rotacion original hasta la nueva en el tiempo t)
		* @param a Quaternion rotacion inicial, b quaternion rotacion final, t tiempo de rotacion
		* @return Quaternion final
		*/
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

		/**
		* @brief Multiplicacion escalar de dos quaternions
		* @param q Quaternion a multiplicar
		* @return Producto escalar
		*/
		inline float dot(const Quaternion& q) const {
			return _w * q._w + _x * q._x + _y * q._y + _z * q._z;
		}

		/**
		* @brief Aplica una rotación definida por ángulos Euler al quaternion actual.
		* @param v Vector3 con los ángulos de rotación en grados (x=roll, y=pitch, z=yaw)
		*/
		inline void rotate(Vector3<> v) {
			//cada comp a un quaternion
			Quaternion qx = Quaternion::AngleAxis(v.getX(), core::Vector3(1.0f, 0.0f, 0.0f));
			Quaternion qy = Quaternion::AngleAxis(v.getY(), core::Vector3(0.0f, 1.0f, 0.0f));
			Quaternion qz = Quaternion::AngleAxis(v.getZ(), core::Vector3(0.0f, 0.0f, 1.0f));

			*this = qz * qy * qx * (*this);
			*this = this->normalized();
		}

		/**
		* @brief Convierte el quaternion a ángulos Euler en grados.
		* @return Vector3 con los ángulos Euler (x=roll, y=pitch, z=yaw)
		*/
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
		/**
		* @brief Iguala dos quaternions
		* @param q Quaternion a igualar
		* @return Quaternion final
		*/
		inline Quaternion& operator=(const Quaternion& q) { //igualdad
			_x = q._x;
			_y = q._y;
			_z = q._z;
			_w = q._w;
			return *this;
		}

		/**
		* @brief Compara dos quaternions
		* @param q Quaternion a comparar
		* @return Si son iguales o no
		*/
		inline bool operator==(const Quaternion& q) { //igualdad

			return (_x == q.getX() && _y == q.getY() && _z == q.getZ() && _w == q.getW());
		}

		/**
		* @brief Compara dos quaternions
		* @param q Quaternion a comparar
		* @return Si son distintos o no
		*/
		inline bool operator!=(const Quaternion& q) { //igualdad

			return (_x != q.getX() || _y != q.getY() || _z != q.getZ() || _w != q.getW());
		}

		/**
		* @brief Multiplica dos quaternions combinando sus rotaciones.
		* @param q Quaternion a multiplicar
		* @return Quaternion resultante de la multiplicación
		*/
		inline Quaternion operator*(const Quaternion& q) const {//multiplicacion
			return Quaternion(
				_w * q._x + _x * q._w + _y * q._z - _z * q._y, // x
				_w * q._y - _x * q._z + _y * q._w + _z * q._x, // y
				_w * q._z + _x * q._y - _y * q._x + _z * q._w, // z
				_w * q._w - _x * q._x - _y * q._y - _z * q._z  // w
			);
		}

		/**
		* @brief Multiplica quaternion por numero.
		* @param a float a multiplicar
		* @return Quaternion resultante de la multiplicación
		*/
		inline Quaternion operator*(float a) const {//multiplicacion por numero
			return Quaternion(
				_x * a, _y * a, _z * a, _w * a);
		}

		/**
		* @brief Suma dos quaternions combinando sus rotaciones.
		* @param q Quaternion a sumar
		* @return Quaternion resultante de la suma
		*/
		inline Quaternion operator+(const Quaternion& q) const {//suma
			return Quaternion(_x + q._x, _y + q._y, _z + q._z, _w + q._w);
		}

		/**
		* @brief resta dos quaternions combinando sus rotaciones.
		* @param q Quaternion a restar
		* @return Quaternion resultante de la resta
		*/
		inline Quaternion operator-(const Quaternion& q) const {//resta
			return Quaternion(_x - q._x, _y - q._y, _z - q._z, _w - q._w);
		}
	};
}

