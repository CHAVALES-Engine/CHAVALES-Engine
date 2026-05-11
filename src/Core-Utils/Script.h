#pragma once
#include <string>
#include "ScriptsManager.h"

class ENGINE_API  Script {
public:
    Script();
    ~Script();

    bool loadScript(const std::string& path);
    bool executeFunction(const std::string& name) const;
    bool executeFunction(const std::string& name, ExecuteArgs args) const {
        return _handle != NULL_SCRIPT &&
            ScriptsManager::instance().execute(_handle, name, args);
    }

private:
    ScriptHandle _handle;
};