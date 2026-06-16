/**
 * @file
 * @brief
 */

#pragma once
#include <string>

#include "Color.h"
#include "Debug.h"
#include "EngineAPI.h"

#define TOML_HEADER_ONLY 1
#define DLLs_PATH "./game/"
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
		static GameConfigurator& instance();

		// --- Carpeta root de contenido del juego
		/**
		* @brief
		*	Ruta donde buscar las escenas del juego
		*/
		std::string _root = "./game/";

		// --- Carga de juegos
		/**
		* @brief
		*	Ruta donde buscar las escenas del juego
		*/
		std::string _scenesRoot = "./game/scenes/";
		/**
		* @brief
		*	Nombre del .lua de la escena inicial
		*/
		std::string _firstScene = "scene_base";
		/**
		* @brief
		*	Nombre del .dll del juego
		*/
		std::string _gameDLL = "";
		/**
		* @brief
		*	Si se usara o no el .toml para la carga
		*/
		std::string _configType = "";

		// --- SDL
		/**
		* @brief
		*	Nombre de la ventana del juego
		*/
		std::string _windowName = "";
		/**
		* @brief
		*	Ruta para la imagen del icono relativa a la carpeta de assets
		*/
		std::string _iconRoot = "";
		/**
		* @brief
		*	Color del vacio
		*/
		core::Color _clearColor = CHAVAL_VACIO;
		/**
		* @brief
		*	Ancho de la ventana
		*/
		int _windowWidth = 1920;
		/**
		* @brief
		*	Alto de la ventana
		*/
		int _windowHeight = 1080;

		// --- Assets
		/**
		* @brief
		*	Ruta donde buscar los assets del juego
		*/
		std::string _assetsRoot = "./game/assets/";

		// Serializacion
		/**
		 * @brief Construye el documento TOML desde los datos.
		 */
		toml::table Serialize();
		/**
		 * @brief Lee un documento TOML y rellena los campos.
		 */
		void Deserialize(const toml::table& data);
		/**
		 * @brief Guardar a disco.
		 */
		void SaveToFile(const std::string& path);
		/**
		 * @brief Cargar desde disco.
		 */
		bool LoadFromFile(const std::string& path);

		std::string getWindowName(); 
		void setWindowName(std::string s); 
	private:
		template<typename T>
		static T GetValue(const toml::table& table, const std::string& key, const T& defaultValue)
		{
			try
			{
				auto node = table[key];
				if (!node) return defaultValue; //clave no existe

				if constexpr (std::is_same_v<T, std::string>)
					return node.value_or(defaultValue);
				else if constexpr (std::is_same_v<T, int>)
				{
					auto val = node.as_integer();
					return val ? val->get() : defaultValue;
				}
				else if constexpr (std::is_same_v<T, float>)
				{
					auto val = node.as_floating_point();
					return val ? static_cast<float>(val->get()) : defaultValue;
				}
				else if constexpr (std::is_same_v<T, bool>)
				{
					auto val = node.as_boolean();
					return val ? val->get() : defaultValue;
				}
			}
			catch (const std::exception& e)
			{
				Debug::warning("GAME CONFIGURATOR: Error leyendo ", key, ": ", e.what());
			}
			catch (...)
			{
				Debug::warning("GAME CONFIGURATOR: Error desconocido leyendo ", key, ".");
			}
			return defaultValue;
		}
	};
}