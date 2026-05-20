/**
 * @file
*/
#pragma once
#include <Component.h>
#include <vector>


// @brief MAcro de exportacion de componentes para poder usarlos directamente desde otras dlls
#ifdef PLUGIN_EXPORTS
#define PLUGIN_API __declspec(dllexport)	// exportando desde engine
#else
#define PLUGIN_API __declspec(dllimport)	// consumiendo desde fuera
#endif


namespace PluginSDK {

	/**
	* @brief
	*	Singleton PluginComponentRegistry, compartido en un plugin.
	*	Funciona como un registro local de componentes
	*/
	class PluginComponentRegistry {
	public:
		/**
		* @brief Instancia del PluginRegistry
		*/
		static PluginComponentRegistry& instance() {
			static PluginComponentRegistry instance;
			return instance;
		}
		/**
		* @brief Aniade un descriptor al registro local
		*
		* @param name - nombre del componente
		* @param ComponentConstructor - funcion creadora del componente
		*/
		void add(const char* name, core::ComponentConstructor ComponentConstructor) {
			_descriptors.push_back({ name, std::move(ComponentConstructor) });
		}
		/**
		* @brief Acceso a los descriptores contenidos
		*
		* @return std::vector<core::ComponentDescriptor>& - vector de descriptores de componentes
		*/
		const std::vector<core::ComponentDescriptor>& get() const {
			return _descriptors;
		}
	private:
		/**
		* @brief Lista de componentes registrados en la dll del plugin
		*/
		std::vector<core::ComponentDescriptor> _descriptors;
	};

	/**
	* @brief
	*	Struct que al crearse registra el componente de tipo T en el
	*	registro local del plugin
	*/
	template<typename T>
	struct AutoRegisterComponent {
		/**
		* @brief
		*	Constructor explicito que aniade una funcion constructora
		*	lambda del objeto T (En nuestro caso, clases hijas de component)
		*/
		explicit AutoRegisterComponent(const char* name) {
			PluginComponentRegistry::instance().add(name, []() {
				return std::make_unique<T>();
				});
		}
	};

	/**
	* @brief
	*	Singleton PluginResourcesRegistry, compartido en un plugin.
	*	Funciona como un registro local de resources a precargar
	*/
	class PluginResourcesRegistry {
	public:
		/**
		* @brief Instancia del PluginResourcesRegistry
		*/
		static PluginResourcesRegistry& instance() {
			static PluginResourcesRegistry instance;
			return instance;
		}

		/**
		* @brief Mete un resource al registro local.
		* @param path - Ruta del archivo (ej: "models/player/player.mesh").
		*/
		void add(const std::string& path) {
			_resources.push_back(path);
		}

		/**
		* @brief Acceso a los resources contenidos.
		* @return const std::vector<std::string>& - vector de paths de resources.
		*/
		const std::vector<std::string>& get() const {
			return _resources;
		}
	private:
		/**
		* @brief Lista de resources registrados en la dll del plugin
		*/
		std::vector<std::string> _resources;
	};

	/**
	* @brief
	*	Struct que al crearse registra un resource en el registro local del plugin
	*/
	struct AutoRegisterResource {
		/**
		* @brief Constructor que mete un resource al registro local.
		* @param path - ruta del archivo.
		*/
		explicit AutoRegisterResource(const std::string& path) {
			PluginResourcesRegistry::instance().add(path);
		}
	};
}


// ======= COMPONENTS =======
/**
* @brief
*	Macro para registrar un componente en el registro local
* @param Clase a registrar
	* @example
	*	REGISTER_COMPONENT("Transform")
	*	REGISTER_COMPONENT("Collider")
*/
#define REGISTER_COMPONENT(TYPE) \
    static PluginSDK::AutoRegisterComponent<TYPE> comp_##TYPE(#TYPE)
// # Convierte el nombre de una clase en texto
// la variable estatica se llama como la clase que le pases, 
// y crea una entrada en el registro local con su mismo nombre

// Indica al linker que getPluginComponent, aunque no esta siendo usado en nuestro proyecto, debe ser incluido en el .obj
#pragma comment(linker, "/INCLUDE:getPluginComponents")
	/**
	* @brief
	*	Funcion de C exportada para acceder a los componentes registrados por el singleton
	* @param & count - Se asigna el numero de componentes que hay registrados
	*/
extern "C" __declspec(dllexport) // Especificacion de exportacion que mantiene el nombre sin alterar por parametros en compilacion
// (__declspec(dllexport))solo para windows
const core::ComponentDescriptor* getPluginComponents(size_t& count);


// ======= RESOURCES =======
// Macro que concatena dos valores
#define _PRELOAD_CONCAT(a, b) a##b
// Macro que expande counter primero y concatena con un prefijo
#define _PRELOAD_NAME(prefix, counter) _PRELOAD_CONCAT(prefix, counter)
/**
* @brief Macro para registrar un resource en el registro local.
* @param PATH - ruta del archivo.
* @example
*	PRELOAD_RESOURCE("models/player.mesh")
*	PRELOAD_RESOURCE("textures/player_diffuse.png")
*/
#define PRELOAD_RESOURCE(PATH) \
	inline static PluginSDK::AutoRegisterResource _PRELOAD_NAME(resource_, __COUNTER__)(PATH);

// Indica al linker que getPluginPreloadAssets, aunque no esta siendo usado en nuestro proyecto, debe ser incluido en el .obj
#pragma comment(linker, "/INCLUDE:getPluginPreloadResources")
	/**
	* @brief
	*	Funcion de C exportada para acceder a los preloadAssets registrados por el singleton
	* @param & count - Se asigna el numero de preloadAssets que hay registrados
	*/
extern "C" __declspec(dllexport) // Especificacion de exportacion que mantiene el nombre sin alterar por parametros en compilacion
// (__declspec(dllexport))solo para windows
const std::string* getPluginPreloadResources(size_t& count);
