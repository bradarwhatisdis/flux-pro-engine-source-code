#pragma once

#include "Types.h"

struct CCDirector : Class {

    CCDirector(ptr instance = 0) : Class(instance) {}

    operator bool() { return instance && this->isInstanceOf("CCDirector"); }
};

static CCDirector sharedDirector;
