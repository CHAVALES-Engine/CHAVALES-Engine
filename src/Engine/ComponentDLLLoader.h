#pragma once
#include <string>
#include <vector>
#include <functional>
#if _WIN64
#include <Windows.h>
#endif // _WIN64
#include <EngineAPI.h>

// fordward declarations.
class Engine;

/**
* @brief La clase ComponentDLLLoader es la encargada de cargar dlls de componentes y buscar la funcion exportadora
*		del registro local de componentes de cada dll, para pasarselos al registro global del motor.
*		
*		> Actualmente este dllloader es solo usable en windows
*/
class ENGINE_API ComponentDLLLoader
{
private:
	ComponentDLLLoader() = default;
	/**
	* @brief Destructor.
	*
	*		Descarga todas las librerias.
	*/
	~ComponentDLLLoader();
	// @brief Callback que se llama cuando una DLL se recarga
	using ReloadCallback = std::function<void(const std::string& path)>;
public:
	/**
	* @brief Metodo para cargar librerias.
	*
	*		Carga la libreria del path.
	* @param path - Path a la libreria.
	* @return bool - Si ha cargado de forma exitosa.
	*/
	static bool load(const std::string& path);
	/**
	* @brief Metodo para descarga la libreria del path.
	* @param path - Path a la libreria.
	* @return bool - Si se ha descargado correctamente.
	*/
	static bool unload(const std::string& path);	
	/**
	* @brief Descarga todas las lubrerias dinamicas.
	*/
	void unLoadAll();
	/**
	 * @brief Comprueba si es necesario recargar alguna de las dll.
	 */
	static bool checkReload();
	/**
	 * @brief Settea un callback de reload, llamado cuando se tenga que recargar una dll.
	 * @param cb - std::function<void(const std::string& path)> funcion a llamar.
	 */
	void setReloadCallback(ReloadCallback const & cb);
private:
	/**
	 * @brief Struct con informacion asociada a una entrada a la libreria, necesaria para recarga.
	 */
	struct LoadedLibrary {
		#if _WIN64
		HMODULE		handle;
		#endif // _WIN64
		std::string path;
		std::string tempPath;
		FILETIME	lastWriteTime;
		int			idOnVector;
	};

	/**
	 * @brief Descarga una libreria a partir de su entrada en el vector.
	 * @param entry 
	 * @return 
	 */
	static void _unload(LoadedLibrary& entry);
	/**
	 * @brief Recarga una libreria.
	 * @param entry 
	 */
	static void _reload(LoadedLibrary& entry);

	static std::string _makeTempPath(const std::string& originalPath);

	static FILETIME _getFileWriteTime(const std::string& path);
	/**
	* @brief Vector contenedor de informacion de dlls.
	*/
	static std::vector<LoadedLibrary> _libraries;
	/**
	 * @brief Funcion lambda a llamar cuando se hace un reload de una libreria
	 */
	ReloadCallback _reloadCallback;
	/**
	 *@brief Clase Engine amiga.
	 */
	friend Engine;
};

