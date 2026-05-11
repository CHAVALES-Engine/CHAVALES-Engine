#pragma once
#include "EngineComponent.h"
#include <EngineAPI.h>


class Script;

class ENGINE_API ScriptComponent :
    public  EngineComponent
{
public:
    ScriptComponent();
	~ScriptComponent();

    bool init(const Properties& p) override;
    void ready() override;
    void enable() override;
    void fixedUpdate() override;
    void update(uint64_t deltaTime)override;
    void disable() override;
    void destroy() override;

private:
    std::unique_ptr<Script> _script;
};

