#pragma once
#include <string>

#include "Color.h"
#include "Debug.h"
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
		static inline core::Color _clearColor = CHAVAL_VACIO;
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
				{ "game", toml::table{{
					{ "first_scene", _firstScene }, 
					{ "game_dll", _gameDLL }
				}}},
				{ "window", toml::table{{
					{ "window_name", _windowName },
					{ "icon_root", _iconRoot },

					{ "clear_color_r", _clearColor.getRed() },
					{ "clear_color_g", _clearColor.getGreen() },
					{ "clear_color_b", _clearColor.getBlue() },

					{ "window_width", _windowWidth },
					{ "window_height",_windowHeight }
				}}}
			} };
		}
		/**
		 * @brief Lee un documento TOML y rellena los campos.
		 */
		static void Deserialize(const toml::table& data)
		{
			_firstScene = data["game"]["first_scene"].value_or("");
			_gameDLL = data["game"]["game_dll"].value_or("");

			_windowName = data["window"]["window_name"].value_or("");
			_iconRoot = data["window"]["icon_root"].value_or("");

			float r = data["window"]["clear_color_r"].as_floating_point()->get();
			float g = data["window"]["clear_color_r"].as_floating_point()->get();
			float b = data["window"]["clear_color_r"].as_floating_point()->get();

			_clearColor = { r,g,b,1.0f};

			_windowWidth = data["window"]["window_width"].as_integer()->get();
			_windowHeight = data["window"]["window_height"].as_integer()->get();
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