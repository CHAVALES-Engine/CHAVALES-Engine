#pragma once
#include <Component.h>
#include <vector>

namespace PluginSDK {

	/*
	* @brief
	*	Singleton PluginComponentRegistry, compartido en un plugin.
	*	Funciona como un registro local de componentes
	*/
	class PluginComponentRegistry {
	public:
		/*
		* @brief Instancia del PluginRegistry
		*/
		static PluginComponentRegistry& instance() {
			static PluginComponentRegistry instance;
			return instance;
		}
		/*
		* @brief Aniade un descriptor al registro local
		*
		* @param name - nombre del componente
		* @param ComponentConstructor - funcion creadora del componente
		*/
		void add(const char* name, core::ComponentConstructor ComponentConstructor) {
			_descriptors.push_back({ name, std::move(ComponentConstructor) });
		}
		/*
		* @brief Acceso a los descriptores contenidos
		*
		* @return std::vector<core::ComponentDescriptor>& - vector de descriptores de componentes
		*/
		const std::vector<core::ComponentDescriptor>& get() const {
			return _descriptors;
		}
	private:
		/*
		* @brief Lista de componentes registrados en la dll del plugin
		*/
		std::vector<core::ComponentDescriptor> _descriptors;
	};

	/*
	* @brief
	*	Struct que al crearse registra el componente de tipo T en el
	*	registro local del plugin
	*/
	template<typename T>
	struct AutoRegisterComponent {
		/*
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
}
/*
* @brief
*	Macro para registrar un componente en el registro local
* @param Clase a registrar
*/
#define REGISTER_COMPONENT(TYPE) \
    static PluginSDK::AutoRegisterComponent<TYPE> reg_##TYPE(#TYPE)
// # Convierte el nombre de una clase en texto
// la variable estatica se llama como la clase que le pases, 
// y crea una entrada en el registro local con su mismo nombre

/*
* @brief
*	Funcion de C exportada para acceder a los componentes registrados por el singleton
* @param & count - Se asigna el numero de componentes que hay registrados
*/
extern "C" __declspec(dllexport) // Especificacion de exportacion que mantiene el nombre sin alterar por parametros en compilacion
// (__declspec(dllexport))solo para windows
const core::ComponentDescriptor* getPluginComponents(size_t& count);

