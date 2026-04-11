#pragma once
#include "Component.h"

class LuaComponent :
    public  core::Component
{
private:
    LuaComponent() = default;
    ~LuaComponent();

    bool init(const Properties& p) override;
    void ready() override;
};

