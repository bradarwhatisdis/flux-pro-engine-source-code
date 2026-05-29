
#pragma once

#include "Types.h"
#include "StateManager.h"

struct MainManager : Class {
    Field<0x3b0, MainStateManager> mStateManager;
    
    MainManager(ptr instance = 0) : Class(instance), mStateManager(instance) {}

    operator bool() { return instance && this->isInstanceOf("MainManager"); }
};

static MainManager sharedMainManager;
