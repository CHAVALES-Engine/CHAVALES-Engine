#pragma once
/**
 * @file guid.h
 * @brief Estructura que representa la guid del motor,
 *      > utilizamos la GUID que genera windows pero la envolvemos en un struct propio para meter metodos propios
 */
#include <objbase.h>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

struct ChavalesGUID
{
	/**
	 * @brief Hay que dividir la UID en 2 porque no existe un uint128_t, y UID es un struct con 4 ints de 32
	 */
	uint64_t high;	// Indica variante + secuencia de reloj + MAC [Data4: 8 bytes]
	uint64_t low;	// indica timestamp [Data1: 4 bytes][Data2: 2 bytes][Data3: 2 bytes]

	/**
	 * @brief Sobrecarga del operador == para comparativas.
	 * @param other - guid con la que comparar.
	 * @return bool - Es igual?
	 */
	bool operator==(const ChavalesGUID& other) const {
		return high == other.high && low == other.low;
	}
	/**
	 * @brief Sobrecarga del operador != para comparativas.
	 * @param other - guid con la que comparar.
	 * @return bool - Es distinto?
	 */
	bool operator!=(const ChavalesGUID& other) const {
		return !(*this == other);
	}
	/**
	 * @brief Metodo que convierte el guid en string representado como numeros en hexadecimal.
	 * @return std::string - Numero hexadecimal a string.
	 */
	std::string toString() const
	{
		std::stringstream ss;
		// indica que se escriba una secuencia compuesta en hexadecimales.
		ss << std::hex  // indica que se escriba en hexadecimal.
			<< std::setw(16) << std::setfill('0') << high	// Se reservan 16 caracteres con el caracter 0, y se rellena con high.
			<< std::setw(16) << std::setfill('0') << low;	// Se reservan 16 caracteres con el caracter 0, y se rellena con low.
		return ss.str();
	}
	/**
	 * @brief Pasa un string a un guid propio.
	 * @param str - String a parsear.
	 * @return guid - guid construido.
	 */
	static ChavalesGUID fromString(const std::string& str)
	{
		ChavalesGUID id;
		// stoull : string to unsigned long long (uint64_t)
		id.high = std::stoull(str.substr(0, 16), nullptr, 16);
		id.low = std::stoull(str.substr(16, 16), nullptr, 16);
		return id;
	}
	/**
	 * @brief Genera una uid.
	 * @return guid - guid generada.
	 */
	static ChavalesGUID generate()
	{
		// Solo de windows.
		GUID _guid; // [Data1: 4 bytes][Data2: 2 bytes][Data3: 2 bytes][Data4: 8 bytes]
		CoCreateGuid(&_guid);

		ChavalesGUID result;
		// memcpy copia bytes de una direccion de memoria a otra, sin saber ni importarle que tipo son.
		memcpy(&result.low, &_guid.Data1, sizeof(uint64_t));
		memcpy(&result.high, &_guid.Data4, sizeof(uint64_t));
		return result;
	}
	/**
	 * @brief Devuelve una id invalido para comprobar errores e inicializar.
	 * @return guid - guid invalido { 0, 0}.
	 */
	static ChavalesGUID invalid() { return { 0, 0 }; }
	/**
	 * @brief Devuelve si un guid es valido.
	 * @return bool - Valido?
	 */
	bool isValid() const { return high != 0 || low != 0; }
};


namespace std
{
	template<> struct hash<ChavalesGUID>
	{
		// Sobrecargamos el operador() de los hashes para que se pueda hacer uno de nuestra guid
		// unordered_map hashea la key por debajo
		size_t operator()(const ChavalesGUID& id) const
		{
			return hash<uint64_t>()(id.high) ^ (hash<uint64_t>()(id.low) << 1);
		}
	};
}