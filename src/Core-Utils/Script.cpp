#include "Script.h"

Script::Script() : _handle(NULL_SCRIPT) {}

Script::~Script() {
	if (_handle != NULL_SCRIPT)
		ScriptsManager::instance().releaseScript(_handle);
}

bool Script::loadScript(const std::string& path) {
	_handle = ScriptsManager::instance().loadScript(path);
	return _handle != NULL_SCRIPT;
}

bool Script::executeFunction(const std::string& name) const {
	return _handle != NULL_SCRIPT &&
		ScriptsManager::instance().execute(_handle, name);
}