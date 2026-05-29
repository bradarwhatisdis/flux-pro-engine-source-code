#pragma once

#include "Types.h"

struct State : Class {
    Field<0x18, int32_t> mStateId;

    State(ptr instance = 0) : Class(instance), mStateId(instance) {}

    operator bool() { return instance && isInstanceOf("State"); }
};