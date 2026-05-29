#pragma once

#include "../src/imgui.h"
#include "custom_theme.h"
#include "helpers.h"
#include "dynamic.h"

#include "json/json.hpp"
using json = nlohmann::json;

#include <fstream>

#include "include/includes.h"

static float Rounding = 6.0f;

static const char* targetBone[] = { "Head", "Chest" };
static const char* aimbotTrigger[] = { "Aim+Shoot", "Aim", "Shoot", "Always" };
static const char* espPoint[] = { "Bottom", "Center", "Top" };
static const char* espLocation[] = { "0", "1", "2" };

#define WHITE              ImColor(255, 255, 255)
#define RED                ImColor(255, 0, 0)
#define GREEN              ImColor(0, 255, 0)
#define LIME               ImColor(0, 255, 0)
#define BLUE               ImColor(0, 0, 255)
#define BLACK              ImColor(0, 0, 0)
#define PURPLE             ImColor(128, 0, 128)
#define GREY               ImColor(128, 128, 128)
#define YELLOW             ImColor(255, 255, 0)
#define ORANGE             ImColor(255, 165, 0)
#define DARKGREEN          ImColor(0, 100, 0)
#define PINK               ImColor(255, 192, 203)
#define BROWN              ImColor(165, 42, 42)
#define CYAN               ImColor(0, 255, 255)

static std::map<std::string, ImColor> persistent_colors = {
    {"cESP_Line", CRED},
    {"cESP_LineHidden", CGREEN},
    {"cESP_LineBots", ORANGE},
    {"cESP_LineBotsHidden", CWHITE},
    {"cESP_LineBoss", PINK},
    {"cESP_LineBossHidden", YELLOW},
    {"cAIM_SnapLine", CYAN},
    {"cAIM_SnapLineBot", YELLOW},
    {"cESP_Skeleton", CRED},
    {"cESP_SkeletonHidden", CGREEN},
    {"cESP_Box", CRED},
    {"cESP_BoxHidden", CGREEN},
    {"cESP_Health", CWHITE},
    {"cESP_HealthHidden", CBLACK},
    {"cESP_Name", CRED},
    {"cESP_NameHidden", CGREEN},
    {"cESP_Distance", CRED},
    {"cESP_UID", CRED},
    {"cESP_DistanceHidden", CGREEN},

    {"cESP_BorderAlert", CRED},
    {"cESP_BorderAlertHidden", CGREEN},

    {"cXray", CRED},
    {"cXrayHidden", CWHITE},
};

static std::map<std::string, bool> persistent_bool = {
    {"bESP", true},
    {"bESP_DrawPredictionLine", true},
    {"bESP_DrawPredictionPos", true},
    {"bESP_DrawInitialPos", true},
    {"bESP_DrawPockets", true},
    {"bESP_DrawPocketsShotState", true},

    {"bAIM", true},
    {"bAutoPlay", false},

    {"bImguiAutoSave", true},
    {"bAlwaysAutoResize", true},
    {"bMoveOnlyWithTitleBar", false},
    {"bStackTraceSEGV", true},
    {"bStackTraceMemcpy", false},
    {"bEnableDebug", true},
};

static std::map<std::string, float> persistent_float = {
    {"fAIM_AngleStep", 0.0001f},
    {"fAIM_Fov", 80.0f},
    {"fAIM_NoRecoilMultiplier", 0.01f},
    
    {"fSpeed", 1.0f},
    {"fESP_LineThickness", 2.0f},
    {"fESP_SkeletonThickness", 2.0f},
    {"fAIM_SnapLineThickness", 2.0f},
    {"fESP_HiddenAlpha", 0.5f},
    {"fWeapon_FireSpeed", 0.01f},
    {"fBulletTrack_Probability", 70.0f},
    {"fHeight", 0.0f},
    {"fWidth", 0.0f},
    {"fANYTHING", 1.0f},
    {"fANYTHING2", 1.0f},
    {"fAIM_NoRecoilMultiplier", 1.5f},

    {"fFontScale", 40.0f},
    {"fESP_UpdateFps", 60.0f},
    {"fAIM_SnapStrength", 0.04f},
    {"fAIM_MaxSnapPerFrame", 1.f},

    {"fTableLeft", 0.185},
    {"fTableTop", 0.237},
    {"fTableRight", 0.815},
    {"fTableBottom", 0.912},
    
    {"fSelectedRangeX", 0.0f},
    {"fSelectedRangeY", 0.0f},
    {"fSelectedRangeW", 80.0f},
    {"fSelectedRangeH", 580.0f},
};

static std::map<std::string, int> persistent_int = {
    {"iAIM_WindowX", 120},
    {"iAIM_WindowY", 30},
    
    {"iESP_Point", 2},
    {"iBulletTrack_TargetBone", 0},

    {"iFPS", 30},
    {"iXray", 0},

    {"iNotchPadding", 90},
};

static std::map<std::string, std::string> persistent_string = {
    {"key", ""},
};


static void save_persistence() {
    // LOGI("save_persistence");

    json j;

    for (const auto& pair : persistent_bool) j[pair.first] = pair.second;
    for (const auto& pair : persistent_float) j[pair.first] = pair.second;
    for (const auto& pair : persistent_int) j[pair.first] = pair.second;
    for (const auto& pair : persistent_string) j[pair.first] = pair.second;
    for (const auto& pair : persistent_colors) j[pair.first] = {pair.second.Value.x, pair.second.Value.y, pair.second.Value.z, pair.second.Value.w};

    std::ofstream out("/data/user_de/0/" + PACKAGE_NAME + "/no_backup/persistence.json");
    out << j.dump(2);
    out.close();
}

static void load_persistence() {
    json j = json::object();

    std::ifstream in("/data/user_de/0/" + PACKAGE_NAME + "/no_backup/persistence.json");
    if (in.is_open()) {
        try { in >> j; }
        catch(...) {}
        in.close();
    }

    for (auto& pair : persistent_bool) if (j.contains(pair.first)) pair.second = j[pair.first];
    for (auto& pair : persistent_float) if (j.contains(pair.first)) pair.second = j[pair.first];
    for (auto& pair : persistent_int) if (j.contains(pair.first)) pair.second = j[pair.first];
    for (auto& pair : persistent_string) if (j.contains(pair.first)) pair.second = j[pair.first];
    for (auto& pair : persistent_colors) {
        if (j.contains(pair.first) && j[pair.first].is_array() && j[pair.first].size() == 4) {
            pair.second.Value.x = j[pair.first][0];
            pair.second.Value.y = j[pair.first][1];
            pair.second.Value.z = j[pair.first][2];
            pair.second.Value.w = j[pair.first][3];
        }
    }
}

static void save_imgui_style(ImGuiStyle* _style = nullptr) {
    json j;

    ImGuiStyle& style = _style ? *_style : ImGui::GetStyle();
    j["BorderSize"] = style.FrameBorderSize;
    j["ScrollbarSize"] = style.ScrollbarSize;
    j["Rounding"] = Rounding;
    j["Alpha"] = style.Alpha;
    j["ThemeIndex"] = current_theme;

    std::ofstream out("/data/user_de/0/" + PACKAGE_NAME + "/no_backup/style.json");
    out << j.dump(2);
    out.close();
}

static void load_imgui_style(ImGuiStyle* _style = nullptr) {
    json j = json::object();

    std::ifstream in("/data/user_de/0/" + PACKAGE_NAME + "/no_backup/style.json");
    if (in.is_open()) {
        try { in >> j; }
        catch(...) {}
        in.close();
    }

    current_theme = j.value("ThemeIndex", current_theme);

    switch_theme(current_theme);

    ImGuiStyle& style = _style ? *_style : ImGui::GetStyle();
    style.FrameBorderSize = j.value("BorderSize", 0.1f);
    style.ScrollbarSize = j.value("ScrollbarSize", 32.0f);
    style.Alpha = j.value("Alpha", 0.94f);
    
    float rounding = j.value("Rounding", Rounding);
    
    style.FrameRounding = rounding;
    style.WindowRounding = rounding;
    style.ChildRounding = rounding;
    style.PopupRounding = rounding;
    style.ScrollbarRounding = rounding;
    style.GrabRounding = rounding;
    style.LogSliderDeadzone = rounding;
    style.TabRounding = rounding;

    style.WindowPadding = ImVec2(1, 0);
    // style.ItemInnerSpacing.x = 8.0f;
}

