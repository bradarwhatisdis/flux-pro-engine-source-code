#pragma once

#include "Types.h"
#include "Foundation.h"
#include "CCNode.h"
#include "Ball.h"
#include <cmath>
#include "inc/NumberUtils.h"

struct VisualGuide : Instance { // objCType
    Field<0x28, double> mAimAngle; // VisualCue::aimAngle()
    Field<0xa0, ptr> mClassification; // isVisualGuidePointingToWrongBallClassification

    VisualGuide(ptr instance = 0) : Instance(instance), mAimAngle(instance), mClassification(instance) {}
};

struct VisualCue : CCNode {
    Field<0x3a8, VisualGuide> mVisualGuide;
    Field<0x3b0, double> mPower;

    VisualCue(ptr instance = 0) : CCNode(instance), mVisualGuide(instance), mPower(instance) {}

    double getShotAngle() {
        auto angle = mVisualGuide().mAimAngle();
        // angle = round(angle * 10000.0) / 10000.0;
        return NumberUtils::normalizeDoublePrecision(angle);
    }
    
    double getShotPower(bool strict = false) {
        auto power = mPower();
        if (strict && power <= 0.0) return 0.0;

        if (power <= 0.0 || power > 1.0) power = 1.f;
        else power = NumberUtils::normalizeDoublePrecision(power);

        auto maxPower = CUE_PROPERTIES_MAX_POWER;
        return (1.0 - sqrt(1.0 - power * maxPower / maxPower)) * maxPower;
    }
    
    operator bool() { return instance && this->isInstanceOf("VisualCue"); }
};
