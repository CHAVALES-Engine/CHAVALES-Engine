#pragma once
/**
* @file DLLLoader
* @brief DLL loader, este dllloader es solo usable en windows
*/

#include <string>
#include <vector>
#if _WIN64
#include <Windows.h>
#endif // _WIN64

#include <EngineAPI.h>



/**
* @brief La clase ComponentDLLLoader es la encargada de cargar dlls de componentes y buscar la funcion exportadora
*		del registro local de componentes de cada dll, para pasarselos al registro global del motor.
*/
class ENGINE_API ComponentDLLLoader
{
public:
	ComponentDLLLoader();
	/**
	* @brief Destructor.
	*
	*		Descarga todas las librerias.
	*/
	~ComponentDLLLoader();
	/**
	* @brief Metodo para cargar librerias.
	*
	*		Carga la libreria del path.
	* @param path - Path a la libreria.
	* @return bool - Si ha cargado de forma exitosa.
	*/
	bool load(const std::string& path);
	/**
	* @brief  Descarga todas las librerias.
	*
	*			Descarga todas las lubrerias dinamicas.
	*/
	void unLoadAll();
	/**
	* @brief Metodo para descargar libraria.
	*		
	*		TODO: descarga la libreria del path.
	* @param path - Path a la libreria.
	* @return bool - Si se ha descargado correctamente.
	*/
	bool unload(const std::string& path);

private:
	struct LoadedLibrary {
		#if _WIN64
		HMODULE handle;
		#endif // _WIN64
		std::string path;
		std::string tempPath;
		FILETIME lastWriteTime;
	};
	/**
	* @brief
	*		Vector contenedor de informacion de dlls.
	*/
	std::vector<LoadedLibrary> _libraries;

};

