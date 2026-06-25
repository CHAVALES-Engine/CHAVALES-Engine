/**
 * @file
*/

#pragma once

#include <iostream>
#include <fstream>
#include <utility>
#include <cassert>
#include <locale.h>
#include <filesystem>
#include <cstdlib>

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
* @brief Para hacer mensajes de Debug.
*/
class Debug
{
public:
	/**
	* @brief Enumerado con las distintas formas de hacer Debug.
	*/
	enum class Mode
	{
		/** Solo en consola. */
		CONS,
		/** Solo en fichero. */
		FILE,
		/** Consola y fichero. */
		BOTH
	};

	/**
	* @brief Guarda el nombre del juego para poder guardar archivos pertenecientes a este en una carpeta con su nombre
	*/
	static void setLogPath(const std::string& path)
	{
		_logPath = path;
	}

	/**
	* @brief Abre el fichero de Log.
	* Si ya esta abierto el fichero no lo vuelve a abrir.
	*
	* @param file - Nombre del archivo ("log.log" por defecto).
	*/
	static void open(const std::string& file = "log.log")
	{
		if (_file.is_open()) return;

		char* buffer = nullptr;
		size_t len = 0;

		std::filesystem::path basePath;

		if (_dupenv_s(&buffer, &len, "LOCALAPPDATA") == 0 && buffer)
		{
			basePath = std::filesystem::path(buffer) / "ChavalesEngine";
			free(buffer);
		}
		else
		{
			basePath = std::filesystem::path("Logs");
		}

		if (!_logPath.empty())
		{
			basePath /= _logPath;
		}

		std::filesystem::create_directories(basePath);

		std::filesystem::path finalPath = basePath / file;

		_file.open(finalPath);

		if (!_file.is_open())
		{
			error(Mode::CONS, "[DEBUG] Fichero log no se ha podido abrir.");
		}

		setlocale(LC_ALL, "es_ES.utf8");
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
	* @param ...args - Mensaje a escribir.
	*/
	template <typename... Args>
	static void out(Args&&... args)
	{
		_write(Mode::BOTH, "", "[M] ", "\n", std::forward<Args>(args)...);
	}

	/**
	* @brief Sobrecarga de mensaje informativo.
	* Escribe un mensaje informativo ([M]) y en color blanco en consola.
	*
	* @param mode - Modo de Debug (DEBUG_CONS (solo consola), DEBUG_FILE (solo fichero) o DEBUG_BOTH (ambos)).
	* @param ...args - Mensaje a escribir.
	*/
	template <typename... Args>
	static void out(Mode mode, Args&&... args)
	{
		_write(mode, "", "[M] ", "\n", std::forward<Args>(args)...);
	}

	/**
	* @brief Mensaje de aviso.
	* Escribe un mensaje de aviso ([W]) y en color amarillo en consola.
	*
	* @param ...args - Mensaje a escribir.
	*/
	template <typename... Args>
	static void warning(Args&&... args)
	{
		_write(Mode::BOTH, "\033[1;33m", "[W] ", "\n", std::forward<Args>(args)...);
	}

	/**
	* @brief Sobrecarga de mensaje de aviso.
	* Escribe un mensaje de aviso ([W]) y en color amarillo en consola.
	*
	* @param mode - Modo de Debug (DEBUG_CONS (solo consola), DEBUG_FILE (solo fichero) o DEBUG_BOTH (ambos)).
	* @param ...args - Mensaje a escribir.
	*/
	template <typename... Args>
	static void warning(Mode mode, Args&&... args)
	{
		_write(mode, "\033[1;33m", "[W] ", "\n", std::forward<Args>(args)...);
	}

	/**
	* @brief Mensaje de error.
	* Escribe un mensaje de error ([E]) y en color rojo en consola.
	*
	* @param ...args - Mensaje a escribir.
	*/
	template <typename... Args>
	static void error(Args&&... args)
	{
		_write(Mode::BOTH, "\033[1;31m", "[E] ", "\n", std::forward<Args>(args)...);
	}

	/**
	* @brief Sobrecarga de mensaje de error.
	* Escribe un mensaje de error ([E]) y en color rojo en consola.
	*
	* @param mode - Modo de Debug (DEBUG_CONS (solo consola), DEBUG_FILE (solo fichero) o DEBUG_BOTH (ambos)).
	* @param ...args - Mensaje a escribir.
	*/
	template <typename... Args>
	static void error(Mode mode, Args&&... args)
	{
		_write(mode, "\033[1;31m", "[E] ", "\n", std::forward<Args>(args)...);
	}

private:
	/**
	* @brief Escribe en el sitio que debe el mensaje de Debug.
	*
	* @param mode - Modo de Debug (DEBUG_CONS (solo consola), DEBUG_FILE (solo fichero) o DEBUG_BOTH (ambos)).
	* @param begin - Para los mensajes con color, el color.
	* @param type - Tipo de mensaje ([M], [W] o [E])..
	* @param end - Final de mensaje.
	* @param ...args - Mensaje a escribir.
	*/
	template <typename... Args>
	static void _write(Mode mode, const char* color, const char* type, const char* end, Args&&... args)
	{
#ifdef _DEBUG
		if (mode == Mode::CONS || mode == Mode::BOTH)
		{
			std::cout << color << type;
			(std::cout << ... << args);
			std::cout << "\033[0m" << end;
			std::cout.flush();
		}
#endif

		if (_file.is_open() && (mode == Mode::FILE || mode == Mode::BOTH))
		{
			_file << type;
			(_file << ... << args);
			_file << '\n';
			_file.flush();
		}
	}

	/**
	* @brief
	* Archivo de salida del Debug.
	*/
	static inline std::ofstream _file;
	static inline std::string _logPath = "";
};
