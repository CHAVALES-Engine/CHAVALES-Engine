#pragma once
#include <iostream>
#include "Debug.h"

namespace core
{
	// @brief Clase recurso encargada de gestionar y cargar recursos en ram y vram para agilizar el uso de los mismos.
	class Resource
	{
	public:
		enum state { UNLOADED, LOADING, LOADED, ERROR };
		enum Type { MESH, TEXTURE, FONT, SOUND, NONE };
		Resource(const std::string& id, const std::string& path, Type type) :
			_id(id), _path(path), _refCounter(0), _state(UNLOADED), _type(type) {
		}
		virtual ~Resource();
		// @brief carga el recurso  => a implementar por las clases especificas.
		// @return bool - Cargado correctamente.
		virtual bool load();
		// @brief descarga el recurso  => a implementar por las clases especificas.
		// @return bool - Descargado correctamente.
		virtual bool unLoad()
		{
			if (!isValid()) return false;
			if (_refCounter > 0) {
				Debug::warning("No se puede descargar ", _id, ": ", _refCounter, " entidades lo usan");
				return false;
			}
			return true;
		};
		// @brief anyade una referencia al contador
		void addReference() {
			if (!isValid()) return;
			_refCounter++;
		};
		void removeReference()
		{
			if (!isValid()) return;
			_refCounter--;
			if (_refCounter <= 0)
				unLoad();
		};
		int getCounter() const { return _refCounter; }
		state getState() const { return _state; }
		Type getType() const { return _type; }
		bool isValid() const {
			return (_state == LOADED || _state == LOADING) && _type != NONE;
		}
		static Resource getInvalid()
		{
			return { "", "", NONE };
		}
	protected:
		// @brief Settea el puntero al recurso.
		// @param ptr - Puntero al recurso.
		//void _setPtr(void* ptr) { _ptr = ptr; }

		std::string _id;	  // ID / Nombre del recurso.
		std::string _path;	  // Path al reccurso.
		int _refCounter;	  // Contador de referencias a este recurso.
		state _state;		  // Estado del puntero.
		Type _type;    // Tipo de recurso.
		//void* _ptr;			  // Puntero al propio recurso.
	};
}
