#pragma once

#include "Types.h"
#include "Foundation.h"
#include "State.h"

struct StateManager : Class { // @"StateManager"
    Field<0x8, PNSArray<State>*> mStateStack;

    StateManager(ptr instance = 0) : Class(instance), mStateStack(instance) {}
    
    int32_t getCurrentStateId() {
        auto& stateStack = mStateStack();
        if (!stateStack) return -1;

        State lastState = stateStack[-1];
        if (!lastState) return -1;

        return lastState.mStateId();
    }

    operator bool() { return instance && isInstanceOf("StateManager"); }
};

struct MainStateManager : StateManager {
    MainStateManager(ptr instance = 0) : StateManager(instance) {}
    
    bool isInMenu() { return getCurrentStateId() == 3; }
    bool isInGame() { return getCurrentStateId() == 4; }
};

struct GameStateManager : StateManager {
    GameStateManager(ptr instance = 0) : StateManager(instance) {}
    
    // 1 in menu
    // 3 entering game
    // 4 my turn
    // 5 my turn but cant shoot
    // 6 ball rolling?
    // 7 opponent turn
    // 8 opponent turn but cant shoot
    // 10 game ended
    bool isPlayerTurn() { return getCurrentStateId() == 4; }
};