#pragma once
#include <iostream>

namespace core
{
	// @brief Clase recurso encargada de gestionar y cargar recursos en ram y vram para agilizar el uso de los mismos.
	class Resource
	{
	public:
		enum state { UNLOAD, LOADING, LOAD, ERROR };
		//enum Type { MESH, TEXTURE, FONT, SOUND, NONE };
		Resource(const std::string & id, const std::string & path, void* unloadfunc) : 
		_id(id), _path(path), _refCounter(0), _state(state::UNLOAD), _ptr(nullptr), _unloadFuncPtr() {}
		virtual ~Resource() = 0;
		// @brief carga el recurso  => a implementar por las clases especificas.
		// @return bool - Cargado correctamente.
		virtual bool load() = 0;
		// @brief descarga el recurso  => a implementar por las clases especificas.
		// @return bool - Descargado correctamente.
		virtual bool unLoad() {
			if (_unloadFuncPtr()) {
				_state = state::UNLOAD;
				_ptr = nullptr;
				return true;
			}
			_state = state::ERROR;
			return false;
		};
		// @brief anyade una referencia al contador
		void addReference() {
			if (_state == state::UNLOAD || _state == state::ERROR || !_ptr) return;
			_refCounter++;
		};
		void removeReference()
		{
			if (_state == state::UNLOAD || _state == state::ERROR || !_ptr) return;
			_refCounter--;
			if (_refCounter <= 0)
				unLoad();
		};
		int getCounter() const { return _refCounter; }
		state getState() const { return _state; }
		//Type getType() const { return _type; }
	protected:
		// @brief Settea el puntero al recurso.
		// @param ptr - Puntero al recurso.
		void _setPtr(void* ptr) { _ptr = ptr; }

		std::string _id;	  // ID / Nombre del recurso.
		std::string _path;	  // Path al reccurso.
		int _refCounter;	  // Contador de referencias a este recurso.
		state _state;		  // Estado del puntero.
		//Type _type = NONE;    // Tipo de recurso.
		void* _ptr;			  // Puntero al propio recurso.
		void* _unloadFuncPtr; // Puntero a la funcion de descarga del recurso.
	};
}
