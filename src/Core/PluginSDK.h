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
		static PluginComponentRegistry* instance() {
			return _instance;
		}
		/*
		* @brief Anyade un descriptor al registro local
		*
		* @param name - nombre del componente
		* @param factory - funcion creadora del componente
		*/
		void add(const char* name, core::ComponentFactory factory) {
			_descriptors.push_back({ name, std::move(factory) });
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
		static PluginComponentRegistry* _instance;
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
		*	Constructor explicito que anyade una funcion constructora
		*	lambda del objeto T (En nuestro caso, clases hijas de component)
		*/
		explicit AutoRegisterComponent(const char* name) {
			PluginComponentRegistry::instance().add(name, []() {
				return std::make_unique<T>();
				});
		}
	};

/*
* @brief 
*	Macro para registrar un componente en el registro local
* @param Clase a registrar
*/
#define REGISTER_COMPONENT(TYPE) \
    static AutoRegisterComponent<TYPE> #TYPE(#TYPE)
			// # Convierte el nombre de una clase en texto
			// la variable estatica se llama como la clase que le pases, 
			// y crea una entrada en el registro local con su mismo nombre
}