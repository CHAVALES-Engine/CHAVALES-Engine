/*
 * @file
 * @brief
 */
#pragma once

#include <iostream>
#include <fstream>
#include <utility>
#include <cassert>

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

	/**
	*
	* @brief Para hacer mensajes de Debug.
	*
	* TODO: Timestamp.
	PAIGRO AQUI.
	*
	*/
	class Debug
	{
	public:

		/**
		*
		* @brief Enumerado con las distintas formas de hacer Debug.
		*
		*/
		enum class DebugMode
		{
			/** Solo en consola. */
			DEBUG_CONS,
			/** Solo en fichero. */
			DEBUG_FILE,
			/** Consola y fichero. */
			DEBUG_BOTH
		};

		/**
		* @brief Abre el fichero de Log.
		* Si ya esta abierto el fichero no lo vuelve a abrir.
		*
		* @param file - Nombre del archivo ("log.log" por defecto).
		*/
		static void open(const std::string& file = "log.log")
		{
			if (!_file.is_open()) _file.open(file);
			if (!_file.is_open()) error(DebugMode::DEBUG_CONS, "[DEBUG] Fichero log no se ha podido abrir.");
		}

		/**
		* @brief Cierra el archivo de Log.
		* No hace nada si no estaba abierto.
		*/
		static void close()
		{
			if (_file.is_open()) _file.close();
		}

		/**
		* @brief Mensaje informativo.
		* Escribe un mensaje informativo ([M]) y en color blanco en consola.
		*
		* @param mode - Modo de Debug (DEBUG_CONS (solo consola), DEBUG_FILE (solo fichero) o DEBUG_BOTH (ambos)).
		* @param ...args - Mensaje a escribir.
		*/
		template<typename... Args>
		static void out(DebugMode mode, Args&&... args)
		{
			_write(mode, "", "[M] ", "\n", std::forward<Args>(args)...);
		}

		/**
		* @brief Mensaje de aviso.
		* Escribe un mensaje de aviso ([W]) y en color amarillo en consola.
		*
		* @param mode - Modo de Debug (DEBUG_CONS (solo consola), DEBUG_FILE (solo fichero) o DEBUG_BOTH (ambos)).
		* @param ...args - Mensaje a escribir.
		*/
		template<typename... Args>
		static void warning(DebugMode mode, Args&&... args)
		{
			_write(mode, "\033[1;33m", "[W] ", "\n", std::forward<Args>(args)...);
		}

		/**
		* @brief Mensaje de error.
		* Escribe un mensaje de error ([E]) y en color rojo en consola.
		*
		* @param mode - Modo de Debug (DEBUG_CONS (solo consola), DEBUG_FILE (solo fichero) o DEBUG_BOTH (ambos)).
		* @param ...args - Mensaje a escribir.
		*/
		template<typename... Args>
		static void error(DebugMode mode, Args&&... args)
		{
			_write(mode, "\033[1;31m", "[E] ", "\n", std::forward<Args>(args)...);
		}

	private:

		/**
		* @brief Esscribe en el sitio que debe el mensaje de Debug.
		* 
		* @param mode - Modo de Debug (DEBUG_CONS (solo consola), DEBUG_FILE (solo fichero) o DEBUG_BOTH (ambos)).
		* @param begin - Para los mensajes con color, el color.
		* @param type - Tipo de mensaje ([M], [W] o [E])..
		* @param end - Final de mensaje.
		* @param ...args - Mensaje a escribir.
		*/
		template<typename... Args>
		static void _write(DebugMode mode, const char* color, const char* type, const char* end, Args&&...args)
		{
			if (mode == DebugMode::DEBUG_CONS || mode == DebugMode::DEBUG_BOTH)
			{
				std::cout << color << type;
				(std::cout << ... << args);
				std::cout << "\033[0m" << end;
			}

			if (_file.is_open() && (mode == DebugMode::DEBUG_FILE || mode == DebugMode::DEBUG_BOTH))
			{
				_file << type;
				(_file << ... << args);
				_file << '\n';
			}
		}
			/**
			* @brief
			* Archivo de salida del Debug.
			*/
		static inline std::ofstream _file;
	};
}
