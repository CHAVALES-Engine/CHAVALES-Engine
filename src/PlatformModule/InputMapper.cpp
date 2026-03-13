#include "InputMapper.h"

#include <Debug.h>


input::InputMapper::InputMapper()
{

}

void input::InputMapper::addEvent(const std::string& actionName, InputEvent inputEvent, DeviceID id)
{
	if (hasAction(actionName))
	{
		//PAIGRO???
		Debug::warning("Action named \"", actionName, "\" does not exists so it is created.");
	}

	// Si el evento ya esta no metemos nada.
	if (_entryIsInMap(actionName, { id, inputEvent }))
	{
		Debug::error("Event named \"", actionName, "\" alredy exists so it is not added again.");
		return;
	}

	// Metemos el evento y si es necesario la accion tambien.
	_mapper[actionName].push_back({ id, inputEvent });
	Debug::out("Event added to action named \"", actionName, "\".");
}

void input::InputMapper::removeEvent(const std::string& actionName, InputEvent inputEvent, DeviceID id)
{
	// Si no existe la accion no se puede eliminar nada.
	if (!hasAction(actionName))
	{
		Debug::error("Action named \"", actionName, "\" does not exists so it can not removed event.");
		return;
	}

	// Buscamos el evento y su id asociada, si no esta no se puede eliminar nada.
	InputMappperEntry aux = { id, inputEvent };
	auto position = std::find(_mapper[actionName].begin(), _mapper[actionName].end(), aux);
	if (position == _mapper[actionName].end())
	{
		Debug::error("Action named \"", actionName, "\" does not contain event so it can not be removed.");
		return;
	}

	// Eliminamos el evento.
	_mapper[actionName].erase(position);
	Debug::out("Event removed from action called \"", actionName, "\".");

	// Si la accion se ha quedado vacia la eliminamos.
	if (_mapper[actionName].empty()) _removeAction(actionName);
}

void input::InputMapper::removeEvents(const std::string& actionName)
{
	// Si no existe la accion no se puede eliminar nada.
	if (!hasAction(actionName))
	{
		Debug::error("Action named \"", actionName, "\" does not exists so it can not removed its events.");
		return;
	}

	//PAIGRO todo

}

void input::InputMapper::removeEventsFromID(const std::string& actionName, DeviceID id)
{
	// Si no existe la accion no se puede eliminar nada.
	if (!hasAction(actionName))
	{
		Debug::error("Action named \"", actionName, "\" does not exists so it can not removed its events.");
		return;
	}

	// PAIGR todo
}

std::vector<input::InputEvent> input::InputMapper::getInputEvents(const std::string& actionName, DeviceID id)
{
	if (!hasAction(actionName))
	{
		Debug::error("Action named \"", actionName, "\" does not exists so its events can not be returned.");
		return {};
	}

	std::vector<InputMappperEntry> action = _mapper[actionName];
	std::vector<InputEvent> events;

	for (int i = 0; i < action.size(); i++)
	{
		if (action[i].first == id)
		{
			events.push_back(action[i].second);
		}
	}

	return events;
}

std::vector<std::string> input::InputMapper::getActions()
{
	//PAIGRO todo

	return std::vector<std::string>();
}

bool input::InputMapper::hasAction(const std::string& actionName) const
{
	return (_mapper.find(actionName) == _mapper.end());
}

float input::InputMapper::getActionAxis(const std::string& actionName, DeviceID id) const
{
	//que?

	return 0.0f;
}


void input::InputMapper::_removeAction(const std::string& actionName)
{
	if (_mapper.find(actionName) == _mapper.end())
	{
		Debug::error("Action named \"", actionName, "\" doesnt exists so it can not be removed.");
		return;
	}

	_mapper.erase(actionName);
	Debug::out("Action named \"", actionName, "\" removed.");
}

bool input::InputMapper::_entryIsInMap(const std::string& actionName, InputMappperEntry entry)
{
	if (_mapper.find(actionName) == _mapper.end())
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
