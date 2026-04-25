#include "MessagesManager.h"
#include "checkMLNew.h"


core::MessagesManager& core::MessagesManager::instance()
{
	static core::MessagesManager instance; // Se crea la primera vez, destruye al cerrar.
	return instance;
}
