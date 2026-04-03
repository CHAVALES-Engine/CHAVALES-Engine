#pragma once
#include <string>

#include "Color.h"
#include "Debug.h"
using namespace std;

#include "EngineAPI.h"


#define TOML_HEADER_ONLY 1
#define CONFIGURATOR_PATH "./configurator.toml"
#include "toml.hpp"

namespace core
{
	/**
	 * @brief Clase para gestionar la configuracion del juego.
	 *
	 *		Clase estatica que guarda la informacion de la configuracion del juego.
	 */
	class ENGINE_API GameConfigurator
	{
	public:
		// --- Carga de juegos
		/*
		* @brief
		*	Ruta donde buscar las escenas del juego
		*/
		static inline std::string _scenesRoot = "./game/scenes/";
		/*
		* @brief
		*	Nombre del .lua de la escena inicial
		*/
		static inline std::string _firstScene = "scene1";
		/*
		* @brief
		*	Nombre del .dll del juego
		*/
		static inline std::string _gameDLL = "";
		/*
		* @brief
		*	Si se usara o no el .toml para la carga
		*/
		static inline std::string _useTOML = "";

		// --- SDL
		/*
		* @brief
		*	Nombre de la ventana del juego
		*/
		static inline std::string _windowName = "";
		/*
		* @brief
		*	Ruta para la imagen del icono relativa a la carpeta de assets
		*/
		static inline std::string _iconRoot = "";
		/*
		* @brief
		*	Color del vacio
		*/
		static inline core::Color _clearColor =	CHAVAL_VACIO;
		/*
		* @brief
		*	Ancho de la ventana
		*/
		static inline int _windowWidth = 1920;
		/*
		* @brief
		*	Alto de la ventana
		*/
		static inline int _windowHeight = 1080;

		// --- Assets
		/*
		* @brief
		*	Ruta donde buscar los assets del juego
		*/
		static inline std::string _assetsRoot = "./game/assets/";
		/*
		* @brief
		*	Nombre del .lua de la lista de assets
		*/
		static inline std::string _assetsList = "assetList";

		// Serializacion
		/**
		 * @brief Construye el documento TOML desde los datos.
		 */
		static toml::table Serialize()
		{
			return toml::table{ {
				{ "scenes", toml::table{{
					{ "root",        _scenesRoot },
					{ "first_scene", _firstScene }
				}}},
				{ "assets", toml::table{{
					{ "root", _assetsRoot }
				}}}
			} };
		}
		/**
		 * @brief Lee un documento TOML y rellena los campos.
		 */
		static void Deserialize(const toml::table& data)
		{
			_scenesRoot = data["scenes"]["root"].value_or("");
			_firstScene = data["scenes"]["first_scene"].value_or("");
			_assetsRoot = data["assets"]["root"].value_or("");
		}
		/**
		 * @brief Guardar a disco.
		 */
		static void SaveToFile(const std::string& path)
		{
			std::ofstream file(path);
			file << Serialize();
		}
		/**
		 * @brief Cargar desde disco.
		 */
		static bool LoadFromFile(const std::string& path)
		{
			try
			{
				toml::table data = toml::parse_file(path);
				Deserialize(data);
				return true;
			}
			catch (const toml::parse_error& e)
			{
				Debug::error("GAME CONFIGURATOR: ", e.description());
				return false;
			}
		}
	};
}