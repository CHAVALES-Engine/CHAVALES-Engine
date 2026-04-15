#pragma once
#include "Component.h"

class Script
{
public:
    ~Script() = default;
};

class ScriptComponent :
    public  core::Component
{
public:
    ScriptComponent();
    ~ScriptComponent() = default;

    bool init(const Properties& p) override;
    void ready()                   override;
    void enable()                  override;
    void fixedUpdate()             override;
    void update(uint64_t deltaTime)override;
    void disable()                 override;
    void destroy()                 override;
private:
    std::unique_ptr<Script> _script;
};

