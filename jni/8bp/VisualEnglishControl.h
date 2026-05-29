#pragma once

#include "Types.h"
#include <Vector/Vectors.h>

struct VisualEnglishControl : Class {
    Field<0x3b0, Vec2d> mEnglish; // ShotSpin

    VisualEnglishControl(ptr instance = 0) : Class(instance), mEnglish(instance) {}
};
