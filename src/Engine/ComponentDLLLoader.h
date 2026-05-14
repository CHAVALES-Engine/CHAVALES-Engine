#pragma once
#include <string>
#include <vector>
#include <functional>
#if _WIN64
#include <Windows.h>
#endif // _WIN64
#include <EngineAPI.h>

class Engine;

/**
* @file
* @brief La clase ComponentDLLLoader es la encargada de cargar dlls de componentes y buscar la funcion exportadora
*		del registro local de componentes de cada dll, para pasarselos al registro global del motor.
*		Es una clase singleton para ser facilmente accesible por aquello que la necesite.
*		
*		(!) Actualmente este dllloader es solo usable en Windows.
*/
class ENGINE_API ComponentDLLLoader
{
private:
	ComponentDLLLoader() = default;
	/**
	* @brief Destructor.
	*	Descarga todas las librerias.
	*/
	~ComponentDLLLoader();
	// @brief Callback que se llama cuando una DLL se recarga
	using ReloadCallback = std::function<void(const std::string& path)>;
public:

	// Eliminar copia y movimiento
	ComponentDLLLoader(const ComponentDLLLoader&) = delete;
	ComponentDLLLoader& operator=(const ComponentDLLLoader&) = delete;
	ComponentDLLLoader(ComponentDLLLoader&&) = delete;
	ComponentDLLLoader& operator=(ComponentDLLLoader&&) = delete;

	static ComponentDLLLoader& instance();
	/**
	* @brief Metodo para carga de libreria desde un path.
	* @param path - Path a la libreria.
	* @return bool - Si ha cargado de forma exitosa.
	*/
	bool load(const std::string& path);
	/**
	 * @brief Carga todas las dll de una carpeta.
	 * @param path - Ruta al directorio de dlls.
	 * @return bool - Si se ha cargado de forma exitosa.
	 */
	bool loadAll(const std::string& path);
	/**
	* @brief Metodo para descarga de libreria desde un path.
	* @param path - Path a la libreria.
	* @return bool - Si se ha descargado correctamente.
	*/
	bool unload(const std::string& path);	
	/**
	* @brief Descarga todas las lubrerias dinamicas.
	*/
	void unLoadAll();
	/**
	 * @brief Comprueba si es necesario recargar alguna de las dll.
	 */
	bool checkReload();
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
	 * @brief Descarga una libreria y elimina la entrada del vector.
	 * @param library  - Libreria a borrar.
	 */
	bool _unload(LoadedLibrary& library);
	/**
	 * @brief Recarga una libreria.
	 * @param library - Libreria a recargar.
	 */
	void _reload(LoadedLibrary& library);
	/**
	 * @brief Construye un "path_hot" temporal para leerlo y poder modificar mientras el original.
	 * @param originalPath - Path a hacer un "path_hot".
	 * @return std::string - Path temporal.
	 */
	std::string _makeTempPath(const std::string& originalPath);
	/**
	 * @brief Coge el momento en el que ha sido escrito un fichero.
	 * @param path - Ruta al fichero a comprobar.
	 * @return FILETIME - Struct de windows que representa el instante de tiempo.
	 */
	FILETIME _getFileWriteTime(const std::string& path);
	/**
	 * @brief Comprueba si un fichero esta siendo modificado.
	 * @param path - Ruta al fichero a comprobar.
	 * @return bool - Si el fichero esta libre.
	 */
	bool _isFileFree(const std::string& path);
	/**
	* @brief Vector contenedor de informacion de dlls.
	*/
	std::vector<LoadedLibrary> _libraries;
	/**
	 * @brief Funcion lambda a llamar cuando se hace un reload de una libreria
	 */
	ReloadCallback _reloadCallback;
	/**
	 *@brief Clase Engine amiga.
	 */
	friend Engine;
};