#pragma once

#include "../src/imgui.h"
#include <map>

struct AnimState {
    float hoverT = 0.0f;
    float activeT = 0.0f;
    float heldTime = 0.0f;
};

static std::map<ImGuiID, AnimState> g_animStates;

static AnimState& GetAnim(ImGuiID id) {
    return g_animStates[id];
}

static void TickAnim(AnimState& s, bool hovered, bool held, float dt) {
    float targetHover = hovered ? 1.0f : 0.0f;
    float targetActive = held ? 1.0f : 0.0f;
    s.hoverT += (targetHover - s.hoverT) * dt * 10.0f;
    s.activeT += (targetActive - s.activeT) * dt * 14.0f;
    if (held) s.heldTime += dt;
    else s.heldTime = 0.0f;
}

static ImVec4 LerpCol(const ImVec4& a, const ImVec4& b, float t) {
    return ImVec4(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t,
        a.w + (b.w - a.w) * t
    );
}

static ImU32 Col32(const ImVec4& c) {
    return IM_COL32(
        (int)(c.x * 255.0f),
        (int)(c.y * 255.0f),
        (int)(c.z * 255.0f),
        (int)(c.w * 255.0f)
    );
}

// ─── ORANGE & BLACK COLOR PALETTE ─────────────────────────────
// Accent: Vibrant Orange (#FF6B00)
#define ACCENT       ImVec4(1.000f, 0.420f, 0.000f, 1.00f) // #FF6B00
#define ACCENT_HOVER ImVec4(1.000f, 0.522f, 0.200f, 1.00f) // #FF8533
#define ACCENT_ACTIVE ImVec4(0.878f, 0.376f, 0.000f, 1.00f) // #E06000

#define TXT_MAIN     ImVec4(0.950f, 0.950f, 0.960f, 1.00f) // #F2F2F5
#define TXT_MUTED    ImVec4(0.620f, 0.620f, 0.640f, 1.00f) // #9E9EA3
#define TXT_DIM      ImVec4(0.420f, 0.420f, 0.440f, 1.00f) // #6B6B70

#define BG_WINDOW    ImVec4(0.000f, 0.000f, 0.000f, 0.85f) // black x 0.85
#define BG_PANEL     ImVec4(0.050f, 0.050f, 0.050f, 0.92f) // #0D0D0D x 0.92
#define BG_ELEVATED  ImVec4(0.090f, 0.090f, 0.090f, 0.95f) // #171717 x 0.95

#define BORDER_GLASS ImVec4(1.000f, 1.000f, 1.000f, 0.08f) // white x 0.08
#define BORDER_SOLID ImVec4(0.150f, 0.150f, 0.150f, 1.00f) // #262626

#define FRAME_BG         ImVec4(0.000f, 0.000f, 0.000f, 0.40f) // black x 0.40
#define FRAME_BG_HOVER   ImVec4(0.120f, 0.120f, 0.120f, 0.50f) // #1F1F1F x 0.50
#define FRAME_BG_ACTIVE  ImVec4(0.180f, 0.180f, 0.180f, 0.60f) // #2E2E2E x 0.60

#define HEADER           ImVec4(0.050f, 0.050f, 0.050f, 1.00f)
#define HEADER_HOVER     ImVec4(0.100f, 0.100f, 0.100f, 1.00f)
#define HEADER_ACTIVE    ImVec4(0.150f, 0.150f, 0.150f, 1.00f)

#define SCROLL_BG     ImVec4(0.050f, 0.050f, 0.050f, 0.80f)
#define SCROLL_GRAB   ImVec4(0.300f, 0.300f, 0.300f, 1.00f)
#define SCROLL_HOVER  ACCENT
#define SCROLL_ACTIVE ACCENT_HOVER

#define SUCCESS ImVec4(0.400f, 0.800f, 0.600f, 1.00f)
#define DANGER  ImVec4(0.957f, 0.282f, 0.282f, 1.00f)

static void SetupGlassmorphismStyle() {
    auto& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    // ── Colors ──────────────────────────────────────────────
    colors[ImGuiCol_Text]                 = TXT_MAIN;
    colors[ImGuiCol_TextDisabled]         = TXT_DIM;
    colors[ImGuiCol_WindowBg]             = BG_WINDOW;
    colors[ImGuiCol_ChildBg]              = BG_PANEL;
    colors[ImGuiCol_PopupBg]              = ImVec4(BG_ELEVATED.x, BG_ELEVATED.y, BG_ELEVATED.z, 0.95f);
    colors[ImGuiCol_Border]               = BORDER_GLASS;
    colors[ImGuiCol_BorderShadow]         = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_FrameBg]              = FRAME_BG;
    colors[ImGuiCol_FrameBgHovered]       = FRAME_BG_HOVER;
    colors[ImGuiCol_FrameBgActive]        = FRAME_BG_ACTIVE;
    colors[ImGuiCol_TitleBg]              = BG_PANEL;
    colors[ImGuiCol_TitleBgActive]        = BG_ELEVATED;
    colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(BG_PANEL.x, BG_PANEL.y, BG_PANEL.z, 0.75f);
    colors[ImGuiCol_MenuBarBg]            = BG_PANEL;
    colors[ImGuiCol_ScrollbarBg]          = SCROLL_BG;
    colors[ImGuiCol_ScrollbarGrab]        = SCROLL_GRAB;
    colors[ImGuiCol_ScrollbarGrabHovered] = SCROLL_HOVER;
    colors[ImGuiCol_ScrollbarGrabActive]  = SCROLL_ACTIVE;
    colors[ImGuiCol_CheckMark]            = ACCENT;
    colors[ImGuiCol_SliderGrab]           = ACCENT;
    colors[ImGuiCol_SliderGrabActive]     = ACCENT_ACTIVE;
    colors[ImGuiCol_Button]               = FRAME_BG;
    colors[ImGuiCol_ButtonHovered]        = FRAME_BG_HOVER;
    colors[ImGuiCol_ButtonActive]         = ACCENT;
    colors[ImGuiCol_Header]               = HEADER;
    colors[ImGuiCol_HeaderHovered]        = HEADER_HOVER;
    colors[ImGuiCol_HeaderActive]         = ACCENT;
    colors[ImGuiCol_Separator]            = BORDER_GLASS;
    colors[ImGuiCol_SeparatorHovered]     = ACCENT;
    colors[ImGuiCol_SeparatorActive]      = ACCENT;
    colors[ImGuiCol_ResizeGrip]           = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_ResizeGripActive]     = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_Tab]                  = FRAME_BG;
    colors[ImGuiCol_TabHovered]           = FRAME_BG_HOVER;
    colors[ImGuiCol_TabActive]            = ACCENT;
    colors[ImGuiCol_TabUnfocused]         = FRAME_BG;
    colors[ImGuiCol_TabUnfocusedActive]   = HEADER;
    colors[ImGuiCol_PlotLines]            = ACCENT;
    colors[ImGuiCol_PlotLinesHovered]     = ACCENT_HOVER;
    colors[ImGuiCol_PlotHistogram]        = ACCENT;
    colors[ImGuiCol_PlotHistogramHovered] = ACCENT_HOVER;
    colors[ImGuiCol_TableHeaderBg]        = HEADER;
    colors[ImGuiCol_TableBorderStrong]    = ImVec4(BORDER_SOLID.x, BORDER_SOLID.y, BORDER_SOLID.z, 0.50f);
    colors[ImGuiCol_TableBorderLight]     = BORDER_GLASS;
    colors[ImGuiCol_TableRowBg]           = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_TableRowBgAlt]        = ImVec4(1, 1, 1, 0.03f);
    colors[ImGuiCol_TextSelectedBg]       = ImVec4(ACCENT.x, ACCENT.y, ACCENT.z, 0.30f);
    colors[ImGuiCol_DragDropTarget]       = ACCENT;
    colors[ImGuiCol_NavHighlight]         = ACCENT;
    colors[ImGuiCol_NavWindowingHighlight]= ImVec4(1, 1, 1, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]    = ImVec4(0, 0, 0, 0.40f);
    colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0, 0, 0, 0.55f);

    // ── Geometry ────────────────────────────────────────────
    style.WindowRounding     = 16.0f;
    style.ChildRounding      = 12.0f;
    style.PopupRounding      = 14.0f;
    style.FrameRounding      = 10.0f;
    style.GrabRounding       = 8.0f;
    style.ScrollbarRounding  = 10.0f;
    style.TabRounding        = 10.0f;

    style.WindowBorderSize   = 1.0f;
    style.ChildBorderSize    = 0.0f;
    style.PopupBorderSize    = 0.0f;
    style.FrameBorderSize    = 0.0f;
    style.TabBorderSize      = 0.0f;

    // ── Spacing (touch-friendly) ────────────────────────────
    style.WindowPadding      = ImVec2(18.0f, 18.0f);
    style.FramePadding       = ImVec2(18.0f, 14.0f);
    style.CellPadding        = ImVec2(12.0f, 10.0f);
    style.ItemSpacing        = ImVec2(14.0f, 14.0f);
    style.ItemInnerSpacing   = ImVec2(12.0f, 10.0f);
    style.IndentSpacing      = 28.0f;
    style.ScrollbarSize      = 8.0f;
    style.GrabMinSize        = 14.0f;
    style.TouchExtraPadding  = ImVec2(8.0f, 8.0f);

    // ── Misc ────────────────────────────────────────────────
    style.Alpha              = 1.0f;
    style.DisabledAlpha      = 0.45f;
    style.WindowTitleAlign   = ImVec2(0.5f, 0.5f);
    style.ButtonTextAlign    = ImVec2(0.5f, 0.5f);
    style.AntiAliasedLines   = true;
    style.AntiAliasedLinesUseTex = true;
    style.AntiAliasedFill    = true;

    g_animStates.clear();
}

// ── Modern Orange Button ──────────────────────────────────────
static bool ModernButton(const char* label, const ImVec2& size = ImVec2(0, 0))
{
    ImGui::PushID(label);
    ImVec2 labelSize = ImGui::CalcTextSize(label);
    ImVec2 sz = size;
    if (sz.x == 0.0f) sz.x = labelSize.x + ImGui::GetStyle().FramePadding.x * 2.0f;
    if (sz.y == 0.0f) sz.y = labelSize.y + ImGui::GetStyle().FramePadding.y * 2.0f;

    static const char* BTN_ID = "##btn";
    ImGui::InvisibleButton(BTN_ID, sz);
    ImGuiID id = ImGui::GetID(BTN_ID);
    bool hovered = ImGui::IsItemHovered();
    bool held = ImGui::IsItemActive();
    bool pressed = ImGui::IsItemClicked(0);

    float dt = ImGui::GetIO().DeltaTime;
    AnimState& anim = GetAnim(id);
    TickAnim(anim, hovered, held, dt);

    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();

    ImDrawList* dl = ImGui::GetWindowDrawList();
    float hoverT = anim.hoverT;
    float activeT = anim.activeT;
    float pressScale = 1.0f - activeT * 0.04f;

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4 colNorm = style.Colors[ImGuiCol_Button];
    ImVec4 colHov  = style.Colors[ImGuiCol_ButtonHovered];
    ImVec4 colAct  = style.Colors[ImGuiCol_ButtonActive];
    ImU32 bgCol = Col32(LerpCol(LerpCol(colNorm, colHov, hoverT), colAct, activeT));

    ImVec2 center((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f);
    float hw = (max.x - min.x) * 0.5f * pressScale;
    float hh = (max.y - min.y) * 0.5f * pressScale;

    dl->AddRectFilled(ImVec2(center.x - hw, center.y - hh),
                      ImVec2(center.x + hw, center.y + hh),
                      bgCol, style.FrameRounding * pressScale);

    if (hoverT > 0.01f) {
        ImU32 glow = IM_COL32(
            (int)(ACCENT.x * 255.0f * hoverT * 0.12f),
            (int)(ACCENT.y * 255.0f * hoverT * 0.12f),
            (int)(ACCENT.z * 255.0f * hoverT * 0.12f),
            (int)(hoverT * 35)
        );
        dl->AddRectFilled(ImVec2(center.x - hw, center.y - hh),
                          ImVec2(center.x + hw, center.y + hh),
                          glow, style.FrameRounding * pressScale);
    }

    ImU32 textCol = Col32(style.Colors[ImGuiCol_Text]);
    dl->AddText(ImVec2(center.x - labelSize.x * 0.5f, center.y - labelSize.y * 0.5f),
                textCol, label);

    ImGui::PopID();
    return pressed;
}

// ── Modern Orange Checkbox ────────────────────────────────────
static bool ModernCheckbox(const char* label, bool* v)
{
    ImGui::PushID(label);
    ImVec2 labelSize = ImGui::CalcTextSize(label);
    float boxSize = 24.0f;
    float fs = ImGui::GetFontSize();
    float totalH = (boxSize > fs ? boxSize : fs) + ImGui::GetStyle().FramePadding.y * 2.0f;
    float fullW = ImGui::GetContentRegionAvail().x;

    static const char* CB_ID = "##cb";
    ImGui::InvisibleButton(CB_ID, ImVec2(fullW, totalH));
    ImGuiID id = ImGui::GetID(CB_ID);
    bool hovered = ImGui::IsItemHovered();
    bool held = ImGui::IsItemActive();
    bool pressed = ImGui::IsItemClicked(0);
    if (pressed) *v = !*v;

    float dt = ImGui::GetIO().DeltaTime;
    AnimState& anim = GetAnim(id);
    TickAnim(anim, hovered, held, dt);

    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImGuiStyle& style = ImGui::GetStyle();

    float boxX = min.x + 14.0f;
    float boxY = min.y + (totalH - boxSize) * 0.5f;

    ImVec4 frCol = LerpCol(style.Colors[ImGuiCol_FrameBg],
                            style.Colors[ImGuiCol_FrameBgHovered], anim.hoverT);

    dl->AddRectFilled(ImVec2(boxX, boxY), ImVec2(boxX + boxSize, boxY + boxSize),
                      Col32(frCol), 6.0f);
    dl->AddRect(ImVec2(boxX, boxY), ImVec2(boxX + boxSize, boxY + boxSize),
                Col32(BORDER_GLASS), 6.0f);

    if (*v) {
        ImVec4 fillCol = LerpCol(ACCENT, ACCENT_HOVER, anim.hoverT);
        dl->AddRectFilled(ImVec2(boxX + 3.0f, boxY + 3.0f),
                          ImVec2(boxX + boxSize - 3.0f, boxY + boxSize - 3.0f),
                          Col32(fillCol), 4.0f);
        dl->AddLine(ImVec2(boxX + 6.0f, boxY + 12.0f),
                    ImVec2(boxX + 11.0f, boxY + 17.0f),
                    IM_COL32(255, 255, 255, 255), 2.5f);
        dl->AddLine(ImVec2(boxX + 11.0f, boxY + 17.0f),
                    ImVec2(boxX + 18.0f, boxY + 7.0f),
                    IM_COL32(255, 255, 255, 255), 2.5f);
    }

    dl->AddText(ImVec2(boxX + boxSize + 14.0f,
                       min.y + (totalH - ImGui::GetFontSize()) * 0.5f),
                Col32(style.Colors[ImGuiCol_Text]), label);

    ImGui::PopID();
    return pressed;
}
