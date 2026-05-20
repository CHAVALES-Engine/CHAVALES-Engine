/**
 * @file
 * @brief
 */

#pragma once
#include <string>
#include "ScriptsManager.h"

namespace core
{
	class Entity;
}

class ENGINE_API  Script {
public:
    Script();
    ~Script();

    bool loadScript(const std::string& path);
    bool executeFunction(const std::string& name) const;
    bool executeFunction(const std::string& name, std::initializer_list<Property> args);
    bool executeWithProps(const std::string& name, const Properties& props);
    void setEntity(core::Entity* entity)const;


private:
    ScriptHandle _handle;
};