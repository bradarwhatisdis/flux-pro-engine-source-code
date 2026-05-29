#pragma once

#include <cmath>

#include <Vector/Vectors.h>

#include "GameConstants.h"

#define NAN std::isnan

static constexpr double DAT_04c8b998 = 0.0; // F(double, libmain + 0x4c8b998) // v56.13.0
static constexpr double DAT_04c8bc98 = 4.71238898038; // F(double, libmain + 0x4c8bc98) // v56.13.0
static constexpr double DAT_04c8ba00 = 1.57079632679; // F(double, libmain + 0x4c8ba00) // v56.13.0
static constexpr double DAT_04c8b9f8 = 3.14159265359; // F(double, libmain + 0x4c8b9f8) // v56.13.0

void FUN_02b1bfc0(double *param_1,const Vector2D *param_2) {
    double *pdVar1;
    double dVar2;
    
    dVar2 = param_2->y;
    if (param_2->x == DAT_04c8b998) {
        pdVar1 = (double *)&DAT_04c8bc98;
        if (dVar2 < DAT_04c8b998 == (NAN(dVar2) || NAN(DAT_04c8b998))) {
            pdVar1 = (double *)&DAT_04c8ba00;
        }
        dVar2 = *pdVar1;
    }
    else {
        dVar2 = atan(dVar2 / param_2->x);
        dVar2 = round(dVar2 * 10000.0);
        dVar2 = dVar2 / 10000.0;
        if (param_2->x < DAT_04c8b998) {
            dVar2 = dVar2 + DAT_04c8b9f8;
        }
    }
    *param_1 = dVar2;
    return;
}


namespace NumberUtils {
    // sub_1C2B0D8 5.8.0 angle, power, spin, ball positions??? should be truncated
    inline double normalizeDoublePrecision(double value, double negativeThreshold = 0.0, double negativeExtraLen = 0.0, size_t maxLen = 7) {
        // return round(value * 10000.0) / 10000.0;
        // return value;
        if (std::abs(value) >= 10000.0) return std::floor(value);

        char buffer[256];
        std::snprintf(buffer, sizeof(buffer), "%lf", value);
        size_t strLen = std::strlen(buffer);

        size_t allowedLen = maxLen;
        if (value < negativeThreshold) allowedLen = maxLen + negativeExtraLen;
        if (strLen > allowedLen) buffer[allowedLen] = '\0';

        double result = 0.0;
        std::sscanf(buffer, "%lf", &result);
        return result;
    } // return (int)(value * 10000.0) / 10000.0; }

    double calcAngle(const Vec2d& delta) {
        double angle;
        // static auto FUN_02b1bfc0 = M(void, libmain + 0x2b1bfc0, const Vec2d*, double* outAngle);
        FUN_02b1bfc0(&angle, &delta);
        return angle;


        /* double angle;
        
        if (delta.x == 0.0) {
            angle = PI_1_5;
            if (delta.y >= 0.0) angle = PI_0_5;
        } else {
            angle = atan(delta.y / delta.x);
            angle = round(angle * 10000.0) / 10000.0;
            if (delta.x < 0.0) angle += PI;
        }

        return angle; */
    }

    inline double calcAngle(Vec2d source, Vec2d Destination) { return calcAngle(source - Destination); }
}

double ShotPowerToPower(double shotPower) {
    auto maxPower = CUE_PROPERTIES_MAX_POWER;
    double ratio = 1.0 - (shotPower / maxPower);
    double power = 1.0 - ratio * ratio;
    return NumberUtils::normalizeDoublePrecision(power);
}