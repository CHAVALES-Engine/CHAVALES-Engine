/*
 * @file
 * @brief
 */
#pragma once

#include <iostream>
#include <string>
#include <fstream>

namespace chavalescore
{

	/*//------ANSI codes:

	"\033[x;...;y;zm texto

	//----Texto: (z)
	//--Negro:			30
	//--Rojo:			31
	//--Verde:			32
	//--Amarillo:		33
	//--Azul			34
	//--Magenta:		35
	//--Cian:			36
	//--Blanco:			37

	//----Fondo: (z)
	//--Rojo:			41
	//--Verde:			42
	//--Amarillo:		43
	//--Azul:			44
	//--Magenta:		45
	//--Cian:			46
	//--Blanco:			47

	//----Formatos: (x...y)
	//--Claro:		     1
	//--Subrayado:		 4
	//--Invertir:		 7
	//--Oculto:			 8
	//--Tachado:		 9

	//----Reset:
	//--Reset:		     0
	*/


	class Debug
	{
	public:
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
			std::cout << message << std::endl;
#endif
		}
	};


#ifdef _DEBUG
#define _Out(...) \
	std::cout << "[M] " << __VA_ARGS__ << std::endl;\

#else
#define _Out(...) 
	std::ofstream file;
	//fil
#endif

#ifdef _DEBUG
#define _Error(...) \
std::cout << "\033[1;31m[E] " << __VA_ARGS__ << "\033[0m" << std::endl;\
//PAIGRO AQUI: mirar para que le puedas meter el numero de formato y decidir si clase como arriba o asi como esta.
#else
	
#define _Error(...) 
#endif

#ifdef _DEBUG
#define _Warnig(...) \
std::cout << "\033[1;33m[W] " << __VA_ARGS__ << "\033[0m" << std::endl;\

#else
#define _Warnig(...) 
#endif

}
