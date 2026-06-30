#include "MessagesManager.h"
#include "checkMLNew.h"


core::MessagesManager& core::MessagesManager::instance()
{
	assert(_instance);
	return *_instance;
}

void core::MessagesManager::init()
{
	if (_instance == nullptr)
	{
		_instance = new MessagesManager();
	}
}

void core::MessagesManager::release()
{
	if (_instance != nullptr)
	{
		//_instance->_messages.clear();

		delete _instance;
		_instance = nullptr;
	}
}