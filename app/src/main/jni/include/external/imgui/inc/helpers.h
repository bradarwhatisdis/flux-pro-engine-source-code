#pragma once

#include "../src/imgui.h"
#include <string>
#include <vector>

static ImVec2 Centre(ImVec2 pos, std::string str, float size) {
    ImVec2 tsize = ImGui::CalcTextSizex(str.c_str(), size);
    return {pos.x - tsize.x / 2, pos.y - tsize.y / 2};
}

#define drawText(str, size, pos, col) AddText(0, size, Centre(pos, str, size), col, str);
#define DrawText(str, size, pos, col) AddText(0, size, pos, col, str);

// static ImVec2 ToImVec2(const std::vector<float>& arr) { return ImVec2(arr[0], arr[1]); }

#define CRED ImColor(1.0f, 0.0f, 0.0f, 1.0f)
#define CGREEN ImColor(0.0f, 1.0f, 0.0f, 1.0f)
#define CBLUE ImColor(0.0f, 0.0f, 1.0f, 1.0f)
#define CWHITE ImColor(1.0f, 1.0f, 1.0f, 1.0f)
#define CBLACK ImColor(0.0f, 0.0f, 0.0f, 1.0f)
