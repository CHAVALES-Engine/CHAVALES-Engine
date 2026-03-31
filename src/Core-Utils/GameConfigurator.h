#pragma once
#include <string>
using namespace std;

#include "EngineAPI.h"
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
		static inline std::string _scenesRoot = "";
		/*
		* @brief
		*	Nombre del .lua de la escena inicial
		*/
		static inline std::string _firstScene = "";

		// --- Assets
		static inline std::string _assetsRoot = "";
	};
}