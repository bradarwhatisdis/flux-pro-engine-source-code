#pragma once

#include <iostream>
#include <map>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "external/json/json.hpp"
using json = nlohmann::json;

#include "includes.h"

static std::map<std::string, bool> features_bool = {
    {"bAIM", false},
    {"bAIM_IgnoreBots", false},
    {"bAIM_CheckVisibility", true},
    {"bAIM_IgnoreKnocked", true},
    {"bAIM_SnapLine", false},
    {"bAIM_DrawFov", true},
    {"bAIM_NoTargetHideFov", true},

    {"bESP", false},
    {"bESP_Line", false},
    {"bESP_LineBots", false},
    {"bESP_Skeleton", false},
    {"bESP_Box", false},
    {"bESP_Health", false},
    {"bESP_Name", false},
    {"bESP_Distance", false},

    {"bBulletTrack", false},
    {"bBulletTrack_IgnoreKnocked", true},
    {"bBulletTrack_IgnoreBots", false},

    {"bXray", false},
    {"bSkinHack", false},
    {"bSpeed", false},
    {"bNoRecoil", false},
    {"bNoSpread", false},

    {"bBetterTouch", false}
};

static std::map<std::string, float> features_float = {
    {"fAIM_Fov", 100.0f},
    {"fSpeed", 1.0f},
    {"fESP_LineThickness", 2.0f},
    {"fESP_SkeletonThickness", 2.0f},
    {"fWeapon_FireSpeed", 0.01f},
    {"fBulletTrack_Probability", 70.0f},
    {"fHeight", 0.0f},
    {"fWidth", 0.0f}
};

static std::map<std::string, int> features_int = {
    {"iAIM_TargetBone", 0},
    {"iAIM_Trigger", 0},
    {"iESP_Point", 0},
    {"iBulletTrack_TargetBone", 0},
};

static std::map<std::string, ImColor> features_color = {
    {"cXray", CRED},
    {"cXrayHidden", CWHITE},
};

static const char* targetBone[] = { "Head", "Chest" };
static const char* aimbotTrigger[] = { "Aim+Shoot", "Aim", "Shoot", "Always" };
static const char* espPoint[] = { "Bottom", "Center", "Top" };
