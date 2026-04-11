#include "MessagesManager.h"
#include "checkMLNew.h"

template<typename... Args>
bool core::MessagesManager::createMessage(const std::string& name)
{
	if (_messages.find(name) != _messages.end())
	{
		Debug::warning("Mensaje con nombre: \"", name, "\" ya existe.");
		return false;
	}

	_messages.emplace(name, core::Message<Args...>{});
	Debug::out("Mensaje con nombre: \"", name, "\" creado.");
	return true;
}

template<typename... Args>
core::Message<Args...>* core::MessagesManager::getMessage(const std::string& name)
{
	auto it = _messages.find(name);
	if (it == _messages.end())
	{
		Debug::warning("Mensaje con nombre: \"", name, "\" no existe.");
		return nullptr;
	}

	return std::any_cast<core::Message<Args...>>(&it->second);
}

template<typename... Args>
bool core::MessagesManager::subscribeInMessage(const std::string& name, std::function<void(Args...)> func)
{
	auto it = _messages.find(name);
	if (it == _messages.end())
	{
		Debug::warning("Mensaje con nombre: \"", name, "\" no existe se creara uno nuevo.");
		createMessage(name);
		it = _messages.find(name);
	}

	auto* msg = std::any_cast<core::Message<Args...>>(&it->second);
	msg->subscribe(func);
	return true;
}

core::MessagesManager& core::MessagesManager::instance()
{
	static core::MessagesManager instance; // Se crea la primera vez, destruye al cerrar.
	return instance;
}
