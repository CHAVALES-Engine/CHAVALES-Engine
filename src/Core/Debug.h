#pragma once

#include <string>

class Debug
{
	/**
	 * @brief Escribe en consola un mensaje.
	 *
	 *		Escribe en consola un mensaje.
	 *
	 * @param message - Mensaje a escribir.
	 */
	inline void Out(std::string message) 
	{
		#if _DEBUG
		//std::cout << message << std::endl;
		#endif
	}

#ifdef _DEBUG
#define _Out(...) \
std::cout << __VA_ARGS__ << std::endl;\

#else
#define _Out(...) 
#endif
};

