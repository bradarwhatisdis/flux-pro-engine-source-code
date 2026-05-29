#pragma once

#include "../src/imgui.h"
#include "modern_style.h"

void StyleColorsCustom(ImGuiStyle* _style = nullptr);
void SetupGlassmorphismStyle();

static int current_theme = 0;

static const char* themes[] = { "Glassmorphism", "Custom", "Dark", "Light", "Classic" };

static void switch_theme(int current_theme) {
    switch (current_theme) {
        case 0: SetupGlassmorphismStyle(); return;
        case 1: StyleColorsCustom(); break;
        case 2: ImGui::StyleColorsDark(); break;
        case 3: ImGui::StyleColorsLight(); break;
        case 4: ImGui::StyleColorsClassic(); break;
    }
    
    ImGuiStyle& style = ImGui::GetStyle();
    style.TouchExtraPadding = ImVec2(10.0f, 10.0f);
    style.ItemSpacing = ImVec2(14, 12);
    style.ItemInnerSpacing = ImVec2(12, 10);
    style.FramePadding = ImVec2(16, 12);
    style.WindowPadding = ImVec2(18, 18);
    style.ScrollbarSize = 10.0f;
    style.GrabMinSize = 14.0f;
}
