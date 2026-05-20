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

bool Script::executeFunction(const std::string& name, std::initializer_list<Property> args) {
	return _handle != NULL_SCRIPT &&
		ScriptsManager::instance().execute(_handle, name, args);
}

bool Script::executeWithProps(const std::string& name, const Properties& props) {
	return _handle != NULL_SCRIPT &&
		ScriptsManager::instance().executeWithProps(_handle, name, props);
}

void Script::setEntity(core::Entity* entity) const
{
	ScriptsManager::instance().setScriptEntity(_handle, entity);
}