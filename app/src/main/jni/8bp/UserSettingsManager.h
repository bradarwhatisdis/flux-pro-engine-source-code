#pragma once

#include "Types.h"
#include "Foundation.h"

struct UserSettingsManager : Class {
    Field<0x1c, int> mPowerGaugeLocation; // 0: left, 1: right
    Field<0x20, int> mPowerGaugeOrientation; // 0: horizontal, 1: vertical
 
    UserSettingsManager(ptr instance = 0) : Class(instance), mPowerGaugeLocation(instance), mPowerGaugeOrientation(instance) {}

    operator bool() { return instance && isInstanceOf("UserSettingsManager"); }
};

static UserSettingsManager sharedUserSettingsManager;
