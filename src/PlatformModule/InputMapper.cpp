#include "InputMapper.h"

#include <algorithm>
#include <vector>

#include <Debug.h>
#include "checkMLNew.h"

void input::InputMapper::addEventToAction(const std::string& actionName, InputEvent inputEvent, DeviceID id)
{
	if (!hasAction(actionName))
	{
		Debug::warning("[Input] Action named \"", actionName, "\" does not exists so it will be created.");
	}


	// Si el evento ya esta no metemos nada.
	if (_entryIsInMap(actionName, { id, inputEvent }))
	{
		Debug::error("[Input] Event named \"", actionName, "\" alredy exists so it is not added again.");
		return;
	}


	// Metemos el evento y si es necesario la accion tambien.
	_mapper[actionName].push_back({ id, inputEvent });
	Debug::out("[Input] Event added to action named \"", actionName, "\" in device ", id, ".");
}

void input::InputMapper::removeEvent(const std::string& actionName, InputEvent inputEvent, DeviceID id)
{
	// Si no existe la accion no se puede eliminar nada.
	if (!hasAction(actionName))
	{
		Debug::error("[Input] Action named \"", actionName, "\" does not exists so it can not removed event.");
		return;
	}


	// Si es id por defecto eliminamos todos los eventos de ese tipo de esa accion.
	if (id == ANY_DEVICE)
	{
		_removeEvents(actionName, inputEvent);
		return;
	}


	// Buscamos el evento y su id asociada, si no esta no se puede eliminar nada.
	InputMappperEntry aux = { id, inputEvent };
	auto position = std::find(_mapper[actionName].begin(), _mapper[actionName].end(), aux);
	if (position == _mapper[actionName].end())
	{
		Debug::error("[Input] Action named \"", actionName, "\" does not contain event so it can not be removed.");
		return;
	}

	// Eliminamos el evento.
	_mapper[actionName].erase(position);
	Debug::out("[Input] Event removed from action called \"", actionName, "\".");


	// Si la accion se ha quedado vacia la eliminamos.
	if (_mapper[actionName].empty()) _removeAction(actionName);
}

void input::InputMapper::removeEvents(const std::string& actionName)
{
	// Si no existe la accion no se puede eliminar nada.
	if (!hasAction(actionName))
	{
		Debug::error("[Input] Action named \"", actionName, "\" does not exists so it can not removed its events.");
		return;
	}

	_mapper.erase(actionName); // Eliminamos toda la accion => se elimina todo lo de dentro tambien.
	Debug::out("[Input] All events from \"", actionName, "\" deleted.");
}

void input::InputMapper::removeEventsFromID(const std::string& actionName, DeviceID id)
{
	// Si no existe la accion no se puede eliminar nada.
	if (!hasAction(actionName))
	{
		Debug::error("[Input] Action named \"", actionName, "\" does not exists so it can not removed its events.");
		return;
	}

	// Si es id por defecto se eliminan todos.
	if (id == ANY_DEVICE)
	{
		removeEvents(actionName);
		return;
	}


	std::vector<InputMappperEntry>& action = _mapper[actionName]; // Vector con id-IE de la accion.

	// erase-remove_if idiom. Elimina en un rango los elementos recolocados por remove_if (no elimina solo mueve al final y devuelve el it al inicio de esa 
	// recolocacion) los elementos que cumplan el predicado.
	action.erase( // Erase con rango.
		std::remove_if( // Recoloca dado un incio y final y el predicado.
			action.begin(),
			action.end(),
			[id](const InputMappperEntry& entry) { return entry.first == id; }),
		action.end());
	Debug::out("[Input] Events with id ", id, " removed from action named \"", actionName, "\".");

	// Si la accion se ha quedado vacia la eliminamos.
	if (_mapper[actionName].empty()) _removeAction(actionName);
}

std::vector<input::InputEvent> input::InputMapper::getInputEvents(const std::string& actionName, DeviceID id)
{
	// Si no existe la accion no se puede devolver nada.
	if (!hasAction(actionName))
	{
		Debug::error("[Input] Action named \"", actionName, "\" does not exists so its events can not be returned.");
		return {};
	}


	std::vector<InputMappperEntry>& action = _mapper[actionName]; // Vector con id-IE de la accion.
	std::vector<InputEvent> events; // Vector de eventos a devolver.
	events.reserve(_mapper[actionName].size());

	for (size_t i = 0; i < action.size(); i++)
	{
		if (id == ANY_DEVICE || action[i].first == id)
		{
			events.push_back(action[i].second);
		}
	}

	return events;
}

std::vector<std::string> input::InputMapper::getActions()
{
	std::vector<std::string> actions; // Vector de acciones a devolver.
	actions.reserve(_mapper.size());

	for (auto it : _mapper)
	{
		actions.push_back(it.first);
	}

	return actions;
}

bool input::InputMapper::hasAction(const std::string& actionName) const
{
	return (_mapper.find(actionName) != _mapper.end());
}


void input::InputMapper::_removeAction(const std::string& actionName)
{
	if (_mapper.find(actionName) == _mapper.end())
	{
		Debug::error("[Input] Action named \"", actionName, "\" does not exists so it can not be removed.");
		return;
	}

	_mapper.erase(actionName);
	Debug::out("[Input] Action named \"", actionName, "\" removed.");
}

bool input::InputMapper::_entryIsInMap(const std::string& actionName, InputMappperEntry entry)
{
	if (!hasAction(actionName))
	{
		return false;
	}
	auto position = std::find(_mapper[actionName].begin(), _mapper[actionName].end(), entry);
	if (position == _mapper[actionName].end())
	{
		return false;
	}

	return true;
}

void input::InputMapper::_removeEvents(const std::string& actionName, InputEvent inputEvent)
{
	std::vector<InputMappperEntry>& action = _mapper[actionName]; // Vector con id-IE de la accion.

	// erase-remove_if idiom. Elimina en un rango los elementos recolocados por remove_if (no elimina solo mueve al final y devuelve el it al inicio de esa 
	// recolocacion) los elementos que cumplan el predicado.
	action.erase( // Erase con rango.
		std::remove_if( // Recoloca dado un incio y final y el predicado.
			action.begin(),
			action.end(),
			[inputEvent](const InputMappperEntry& entry) { return entry.second == inputEvent; }),
		action.end());
}
