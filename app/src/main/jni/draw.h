#pragma once

#include <Vector/Vectors.h>
#include <imgui/imgui.h>

#include "resources.h"

using namespace ImGui;
using namespace std;

#include "include/includes.h"

#include "8bp.h"
#include "8bp/Ruleset.h"
#include "imgui/inc/8bp.h"
#include "keylogin.h"
#include "oxorany/oxorany.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

#define rw(pct) (ImGui::GetIO().DisplaySize.x * (pct))
#define rh(pct) (ImGui::GetIO().DisplaySize.y * (pct))
#define rclamp(v,lo,hi) ((v)<(lo)?(lo):((v)>(hi)?(hi):(v)))

struct MenuState {
    bool isOpen = false;
    int currentTab = 0;
    float sidebarW = 0.0f;
    float animProgress = 0.0f;
    float menuAlpha = 0.0f;
    float menuScale = 0.9f;
    float slideY = 30.0f;
    ImVec4 accentColor = ACCENT;
};
static MenuState g_menu;

static float EaseOutBack(float x) {
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    return 1.0f + c3 * powf(x - 1.0f, 3.0f) + c1 * powf(x - 1.0f, 2.0f);
}

static float EaseOutQuart(float x) {
    return 1.0f - powf(1.0f - x, 4.0f);
}

static void DrawGradientRect(ImDrawList* dl, ImVec2 p1, ImVec2 p2, ImU32 col1, ImU32 col2, bool horizontal = true) {
    if (horizontal) {
        dl->AddRectFilledMultiColor(p1, p2, col1, col2, col2, col1);
    } else {
        dl->AddRectFilledMultiColor(p1, p2, col1, col1, col2, col2);
    }
}

static bool SidebarButton(const char* label, const char* icon, bool selected, float width) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    float btnH = rclamp(rh(0.065f), 44.0f, 64.0f);
    ImVec2 size = ImVec2(width - 20.0f, btnH);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    static std::map<ImGuiID, float> hoverAnim;
    float& animT = hoverAnim[id];
    float targetT = (selected || hovered) ? 1.0f : 0.0f;
    animT += (targetT - animT) * g.IO.DeltaTime * 12.0f;

    ImDrawList* dl = window->DrawList;

    ImU32 colSel1 = IM_COL32((int)(ACCENT.x*255), (int)(ACCENT.y*255), (int)(ACCENT.z*255), 255);
    ImU32 colSel2 = IM_COL32((int)(ACCENT_ACTIVE.x*255), (int)(ACCENT_ACTIVE.y*255), (int)(ACCENT_ACTIVE.z*255), 255);

    if (selected) {
        dl->AddRectFilled(bb.Min, bb.Max, colSel1, 20.0f);
        dl->AddRectFilled(ImVec2(bb.Min.x, bb.Min.y + 2), ImVec2(bb.Min.x + 6, bb.Max.y - 2), IM_COL32(255, 255, 255, 180), 3.0f);
    } else if (animT > 0.01f) {
        ImU32 hoverCol = IM_COL32((int)(ACCENT.x*255*0.15f), (int)(ACCENT.y*255*0.15f), (int)(ACCENT.z*255*0.15f), (int)(180*animT));
        dl->AddRectFilled(bb.Min, bb.Max, hoverCol, 12.0f);
    }

    float iconOffset = 8.0f * animT;
    ImVec2 iconCenter = ImVec2(bb.Min.x + 28.0f + iconOffset, bb.Min.y + size.y * 0.5f);
    float iconR = size.y * 0.2f;

    if (selected) {
        dl->AddCircleFilled(iconCenter, iconR + 1, IM_COL32(255, 255, 255, 255), 16);
        dl->AddCircleFilled(iconCenter, iconR - 2, Col32(ACCENT), 16);
    } else {
        int alpha = (int)(120 + 80 * animT);
        dl->AddCircle(iconCenter, iconR, IM_COL32((int)(ACCENT.x*255), (int)(ACCENT.y*255), (int)(ACCENT.z*255), alpha), 16, 2.0f);
    }

    float textX = bb.Min.x + 52.0f + iconOffset;
    float textY = bb.Min.y + (size.y - g.FontSize) * 0.5f;
    dl->AddText(ImVec2(textX, textY), selected ? IM_COL32(255, 255, 255, 255) : IM_COL32((int)(TXT_MAIN.x*255), (int)(TXT_MAIN.y*255), (int)(TXT_MAIN.z*255), (int)(200+55*animT)), label);

    return pressed;
}

static bool ToggleSwitch(const char* label, bool* v) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    float height = rclamp(rh(0.045f), 30.0f, 44.0f);
    float width = height * 1.75f;
    float radius = height * 0.5f;

    ImVec2 textSize = CalcTextSize(label);
    ImVec2 pos = window->DC.CursorPos;
    float rowH = ImMax(height, textSize.y) + style.FramePadding.y * 2 + 8.0f;
    ImVec2 size = ImVec2(GetContentRegionAvail().x, rowH);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    if (pressed) *v = !*v;

    static std::map<ImGuiID, float> switchAnim;
    float& animT = switchAnim[id];
    float targetT = *v ? 1.0f : 0.0f;
    animT += (targetT - animT) * g.IO.DeltaTime * 14.0f;

    static std::map<ImGuiID, float> hoverBgAnim;
    float& hAnim = hoverBgAnim[id];
    float hTarget = hovered ? 1.0f : 0.0f;
    hAnim += (hTarget - hAnim) * g.IO.DeltaTime * 10.0f;

    ImDrawList* dl = window->DrawList;

    if (hAnim > 0.01f) {
        ImU32 hoverBg = IM_COL32((int)(ACCENT.x*255*0.10f), (int)(ACCENT.y*255*0.10f), (int)(ACCENT.z*255*0.10f), (int)(100*hAnim));
        dl->AddRectFilled(bb.Min, bb.Max, hoverBg, 10.0f);
    }

    float toggleX = bb.Max.x - width - 15.0f;
    float toggleY = bb.Min.y + (size.y - height) * 0.5f;
    ImVec2 togglePos = ImVec2(toggleX, toggleY);
    ImVec2 toggleEnd = ImVec2(toggleX + width, toggleY + height);

    ImVec4 offColor = FRAME_BG;
    ImVec4 onColor = ACCENT;
    ImVec4 bgColorV = ImLerp(offColor, onColor, animT);
    dl->AddRectFilled(togglePos, toggleEnd, ImColor(bgColorV), radius);

    float knobX = toggleX + radius + (width - height) * animT;
    float knobY = toggleY + radius;
    float knobR = radius - 4.0f;

    dl->AddCircleFilled(ImVec2(knobX, knobY), knobR + 2.0f, IM_COL32(0, 0, 0, 40));
    dl->AddCircleFilled(ImVec2(knobX, knobY), knobR, IM_COL32(255, 255, 255, 255));

    dl->AddText(ImVec2(bb.Min.x + 15.0f, bb.Min.y + (size.y - textSize.y) * 0.5f), Col32(TXT_MAIN), label);

    return pressed;
}

INLINE void DrawAutoQueue() {
    if (!g_Token.empty() && !g_Auth.empty() && g_Token == g_Auth) {
        static std::chrono::steady_clock::time_point last_call_time;
        static std::chrono::steady_clock::time_point countdown_start;
        static bool counting = false;

        auto now = std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_call_time).count() > 500) {
            counting = false;
        }
        last_call_time = now;

        if (!counting) {
            counting = true;
            countdown_start = now;
        }

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - countdown_start).count();
        int remaining_ms = 3000 - elapsed;

        if (remaining_ms <= 0) {
            if (sharedMenuManager.getMenuStateId() == 13) PopMenuState(13);
            StartLastMatch();
            counting = false;
            return;
        }

        float w = rclamp(rw(0.40f), 300.0f, 500.0f);
        float h = rclamp(rh(0.35f), 200.0f, 350.0f);

        SetNextWindowPos(ImVec2(rw(0.5f), rh(0.5f)), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
        PushStyleColor(ImGuiCol_WindowBg, ImVec4(BG_WINDOW.x, BG_WINDOW.y, BG_WINDOW.z, 0.95f));
        PushStyleVar(ImGuiStyleVar_WindowRounding, 20.0f);

        if (Begin(O("##AutoQueue"), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) {
            ImDrawList* dl = GetWindowDrawList();
            ImVec2 winPos = GetWindowPos();
            ImVec2 winSize = GetWindowSize();

            float headerH = rclamp(rh(0.09f), 50.0f, 90.0f);
            ImU32 h1 = IM_COL32((int)(ACCENT.x*255), (int)(ACCENT.y*255), (int)(ACCENT.z*255), 255);
            ImU32 h2 = IM_COL32((int)(ACCENT_ACTIVE.x*255), (int)(ACCENT_ACTIVE.y*255), (int)(ACCENT_ACTIVE.z*255), 255);
            DrawGradientRect(dl, winPos, ImVec2(winPos.x + winSize.x, winPos.y + headerH), h1, h2, true);
            dl->AddRectFilled(winPos, ImVec2(winPos.x + winSize.x, winPos.y + 20), h1, 20.0f, ImDrawFlags_RoundCornersTop);

            ImVec2 titleSize = CalcTextSize(O("Auto Queue"));
            dl->AddText(ImVec2(winPos.x + (winSize.x - titleSize.x) * 0.5f, winPos.y + headerH * 0.5f - titleSize.y * 0.5f), IM_COL32(255, 255, 255, 255), O("Auto Queue"));

            SetCursorPosY(headerH + 20.0f);
            float font_scale = rclamp(rh(0.045f) / GetFontSize(), 2.0f, 5.0f);
            SetWindowFontScale(font_scale);

            std::string count_str = std::to_string((remaining_ms / 1000) + 1);
            auto text_size = CalcTextSize(count_str.c_str());
            SetCursorPosX((winSize.x - text_size.x) * 0.5f);
            TextColored(ImVec4(ACCENT.x, ACCENT.y, ACCENT.z, 1.0f), "%s", count_str.c_str());

            SetWindowFontScale(1.0f);

            float cancelBtnH = rclamp(rh(0.07f), 40.0f, 60.0f);
            SetCursorPosY(winSize.y - cancelBtnH - 20.0f);
            SetCursorPosX(20.0f);
            PushStyleColor(ImGuiCol_Button, FRAME_BG);
            PushStyleColor(ImGuiCol_ButtonHovered, FRAME_BG_HOVER);
            PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);

            if (Button(O("Cancel"), ImVec2(winSize.x - 40.0f, cancelBtnH))) {
                persistent_bool[O("bAutoQueue")] = false;
                counting = false;
            }

            PopStyleVar();
            PopStyleColor(2);
            End();
        }
        PopStyleVar();
        PopStyleColor();
    }
}

INLINE void DrawESP(ImDrawList* draw) {
    if (!g_Token.empty() && !g_Auth.empty() && g_Token == g_Auth) {
        if (!sharedGameManager) return;

        UpdateScreenTable();

        sharedDirector = F(ptr, libmain + O(0x4f06288));
        if (!sharedDirector) return;

        sharedUserInfo = F(ptr, libmain + O(0x4e9feb8));
        if (!sharedUserInfo) return;

        F(bool, sharedUserInfo + 0x340) = true;

        sharedMainManager = F(ptr, libmain + O(0x4dde3e0));
        if (!sharedMainManager) return;

        sharedMenuManager = F(ptr, libmain + O(0x4dfe838));
        if (!sharedMenuManager) return;

        MainStateManager mainStateManager = sharedMainManager.mStateManager;
        if (!mainStateManager) return;
        if (!mainStateManager.isInGame()) {
        if (persistent_bool[O("bAutoQueue")]) {
            if (!sharedMenuManager.isInQueue()) DrawAutoQueue();
        } return;
        }

        auto visualCue = sharedGameManager.mVisualCue();

        Ball::Classification myclass = sharedGameManager.getPlayerClassification();

        Table table = sharedGameManager.mTable;
        if (!table) return;

        auto tableProperties = table.mTableProperties();
        if (!tableProperties) return;

        auto& pockets = tableProperties.mPockets();

        if (persistent_bool[O("bESP_DrawPockets")]) {
            for (int i = 0; i < 6; i++) {
                auto screenPos = WorldToScreen(pockets[i]);
                draw->AddCircle(ImVec2(screenPos.x, screenPos.y), 40, WHITE, 0, 3.f);
            }
        }

        GameStateManager gameStateManager = sharedGameManager.mStateManager;
        if (!gameStateManager) return;

        if (persistent_bool[O("bAutoPlay")]) AutoPlay::Update();

        auto stateId = gameStateManager.getCurrentStateId();
        if (stateId == 4) gPrediction->determineShotResult(false);
        if (stateId == 6 || stateId == 7 || stateId == 8) return;

        if (persistent_bool[O("bESP_DrawPocketsShotState")]) {
            for (int i = 0; i < 6; i++) {
                if (Prediction::pocketStatus[i]) {
                    auto screenPos = WorldToScreen(pockets[i]);
                    draw->AddCircle(ImVec2(screenPos.x, screenPos.y), 40, GREEN, 0, 5.f);
                }
            }
        }

        if (persistent_bool[O("bESP_DrawPredictionLine")]) {
            for (int i = 0; i < gPrediction->guiData.ballsCount; i++) {
                auto& ball = gPrediction->guiData.balls[i];

                if (ball.initialPosition != ball.predictedPosition) {
                    ImVec2 lastPos{};
                    for (int j = 1; j < ball.positions.size(); j++) {
                        auto point = WorldToScreen(ball.positions[j]);
                        if (lastPos.x || lastPos.y) draw->AddLine(lastPos, point, colors[i], 10.f);
                        lastPos = point;
                    }
                }
            }
        }

        if (persistent_bool[O("bESP_DrawPredictionLine")]) {
            for (int i = 0; i < gPrediction->guiData.ballsCount; i++) {
                auto& ball = gPrediction->guiData.balls[i];

                if (ball.initialPosition != ball.predictedPosition) {
                    draw->AddCircle(WorldToScreen(ball.initialPosition), 20, colors[i], 0, 6.f);
                    draw->AddCircleFilled(WorldToScreen(ball.predictedPosition), 20, colors[i]);
                }
            }
        }
    }
}

#include "ButtonClicker.h"

// ─── Splash Screen ─────────────────────────────────────────
#define SPLASH_DURATION 3.0f

struct SplashState {
    float startTime = 0.0f;
    bool done = false;
    int statusIdx = 0;
    float statusTimer = 0.0f;
    float progress = 0.0f;
};
static SplashState g_splash;

static void DrawSplashScreen(ImGuiIO& io) {
    float t = ImGui::GetTime();
    if (g_splash.startTime == 0.0f) {
        g_splash.startTime = t;
        g_splash.progress = 0.0f;
    }

    float elapsed = t - g_splash.startTime;
    if (elapsed >= SPLASH_DURATION) {
        g_splash.done = true;
        return;
    }

    g_splash.progress = ImMin(elapsed / SPLASH_DURATION, 1.0f);

    float easeProgress = g_splash.progress < 0.5f
        ? 2.0f * g_splash.progress * g_splash.progress
        : 1.0f - powf(-2.0f * g_splash.progress + 2.0f, 2.0f) * 0.5f;

    float dsW = io.DisplaySize.x;
    float dsH = io.DisplaySize.y;
    ImVec2 center(dsW * 0.5f, dsH * 0.35f);

    SetNextWindowPos(ImVec2(0, 0));
    SetNextWindowSize(io.DisplaySize);
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    Begin(O("##Splash"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBringToFrontOnFocus);
    PopStyleColor();

    ImDrawList* dl = GetWindowDrawList();

    // Background gradient (#1a0533 → #0A0C14 → #0D1A2E)
    ImU32 bg1 = IM_COL32(26, 5, 51, 255);
    ImU32 bg2 = IM_COL32(10, 12, 20, 255);
    ImU32 bg3 = IM_COL32(13, 26, 46, 255);
    dl->AddRectFilledMultiColor(ImVec2(0,0), ImVec2(dsW, dsH*0.5f), bg1, bg1, bg2, bg2);
    dl->AddRectFilledMultiColor(ImVec2(0, dsH*0.5f), ImVec2(dsW, dsH), bg2, bg2, bg3, bg3);

    // Radial glow behind logo
    float pulse = (sinf(t * 2.0f) + 1.0f) * 0.5f;
    float glowR = dsW * 0.25f;
    for (int i = 5; i >= 0; i--) {
        float r = glowR * (1.0f + i * 0.08f);
        int a = (int)(12.0f * pulse * (1.0f - i * 0.15f));
        dl->AddCircleFilled(center, r, IM_COL32(168, 85, 247, a), 64);
    }

    // Grid subtle
    ImU32 gridCol = IM_COL32(168, 85, 247, 6);
    for (int x = 0; x < dsW; x += 40) {
        dl->AddLine(ImVec2(x, 0), ImVec2(x + dsH*0.3f, dsH), gridCol, 0.5f);
    }

    // Logo
    float logoScale = 0.8f + 0.2f * ImMin(easeProgress * 2.0f, 1.0f);
    float logoSize = rclamp(rh(0.06f), 28.0f, 48.0f);
    SetWindowFontScale(logoSize / GetFontSize());

    const char* logoText = "FLUX ENGINE";
    ImVec2 logoSizeV = CalcTextSize(logoText);
    float logoX = center.x - logoSizeV.x * 0.5f;
    float logoY = center.y - logoSizeV.y * 0.5f;

    // Glow shadow layers for logo
    ImU32 glowCol = IM_COL32(168, 85, 247, (int)(80.0f * pulse));
    for (int i = 3; i >= 0; i--) {
        float offset = (i + 1) * 3.0f;
        dl->AddText(ImVec2(logoX + offset, logoY + offset), glowCol, logoText);
    }
    dl->AddText(ImVec2(logoX, logoY), IM_COL32(241, 245, 249, 255), logoText);

    SetWindowFontScale(1.0f);

    // Subtitle
    float subAlpha = ImMin((easeProgress - 0.15f) * 3.0f, 1.0f);
    if (subAlpha > 0.0f) {
        float subSlide = 10.0f * (1.0f - subAlpha);
        const char* subText = "Premium 8 Ball Pool Mod";
        ImVec2 subSize = CalcTextSize(subText);
        float subX = center.x - subSize.x * 0.5f;
        float subY = center.y + logoSizeV.y * 0.5f + rh(0.02f) + subSlide;
        dl->AddText(ImVec2(subX, subY), IM_COL32(148, 163, 184, (int)(255 * subAlpha)), subText);
    }

    // Spinner (12 dots with gradient trailing)
    float spinnerT = t * 5.0f;
    float spCX = center.x;
    float spCY = center.y + rh(0.08f);
    float spR = rclamp(rh(0.05f), 24.0f, 48.0f);

    for (int i = 0; i < 12; i++) {
        float angle = spinnerT + (i * PI * 2.0f / 12.0f);
        float a = (float)(12 - i) / 12.0f;
        float dotR2 = rclamp(rh(0.008f), 3.0f, 6.0f);

        float progress = (float)i / 11.0f;
        ImVec4 dotCol = LerpCol(
            ImVec4(0.231f, 0.510f, 0.965f, a),  // #3B82F6 blue
            ImVec4(0.659f, 0.333f, 0.969f, a),  // #A855F7 purple
            progress
        );

        ImVec2 dotPos = ImVec2(
            spCX + cosf(angle) * spR,
            spCY + sinf(angle) * spR
        );
        dl->AddCircleFilled(dotPos, dotR2, Col32(dotCol), 16);
    }

    // Progress bar
    float pbW = rclamp(rw(0.40f), 240.0f, 400.0f);
    float pbH = rh(0.004f);
    float pbX = center.x - pbW * 0.5f;
    float pbY = spCY + spR + rh(0.03f);

    // Track
    dl->AddRectFilled(ImVec2(pbX, pbY), ImVec2(pbX + pbW, pbY + pbH), IM_COL32(30, 41, 59, 255), pbH * 0.5f);

    // Fill
    float fillW = pbW * easeProgress;
    if (fillW > 2.0f) {
        ImU32 fill1 = IM_COL32(168, 85, 247, 255);  // #A855F7
        ImU32 fill2 = IM_COL32(59, 130, 246, 255);  // #3B82F6
        dl->AddRectFilledMultiColor(ImVec2(pbX, pbY), ImVec2(pbX + fillW, pbY + pbH), fill1, fill2, fill2, fill1);

        // Glow tip
        float tipGlow = rh(0.015f);
        ImU32 tipCol = IM_COL32(168, 85, 247, (int)(60.0f * pulse));
        dl->AddCircleFilled(ImVec2(pbX + fillW, pbY + pbH * 0.5f), tipGlow, tipCol, 16);
    }

    // Status text with animated ellipsis
    const char* statusTexts[] = {
        "Menginisialisasi", "Memuat sumber daya",
        "Mengautentikasi", "Menyambungkan", "Berhasil"
    };
    int totalStatus = sizeof(statusTexts) / sizeof(statusTexts[0]);
    int idx = ImMin((int)(easeProgress * totalStatus), totalStatus - 1);

    int dots = ((int)(t * 2.5f) % 4);
    char statusBuf[64];
    snprintf(statusBuf, sizeof(statusBuf), "%s%s", statusTexts[idx],
        dots == 0 ? "   " : (dots == 1 ? ".  " : (dots == 2 ? ".. " : "...")));

    float stScale = rclamp(rh(0.018f), 0.8f, 1.0f);
    SetWindowFontScale(stScale);
    ImVec2 stSize = CalcTextSize(statusBuf);
    SetCursorPosX(center.x - stSize.x * 0.5f);
    SetCursorPosY(pbY + pbH + rh(0.02f));
    TextColored(TXT_MUTED, "%s", statusBuf);
    SetWindowFontScale(1.0f);

    End();
}

static void DrawSidebar(float sidebarW, float winH, ImGuiIO& io) {
    ImDrawList* dl = GetWindowDrawList();
    ImVec2 winPos = GetWindowPos();

    ImU32 bg1 = IM_COL32((int)(BG_PANEL.x*255), (int)(BG_PANEL.y*255), (int)(BG_PANEL.z*255), (int)(BG_PANEL.w*255));
    ImU32 bg2 = IM_COL32((int)(BG_ELEVATED.x*255), (int)(BG_ELEVATED.y*255), (int)(BG_ELEVATED.z*255), (int)(BG_ELEVATED.w*255));
    DrawGradientRect(dl, winPos, ImVec2(winPos.x + sidebarW, winPos.y + winH), bg1, bg2, false);
    dl->AddLine(ImVec2(winPos.x + sidebarW, winPos.y), ImVec2(winPos.x + sidebarW, winPos.y + winH), Col32(BORDER_GLASS), 1.0f);

    SetCursorPos(ImVec2(0, rh(0.03f)));

    BeginGroup();

    Dummy(ImVec2(sidebarW, rh(0.01f)));
    SetCursorPosX(rw(0.03f));

    float titleScale = rclamp(rh(0.028f), 1.0f, 1.4f);
    PushStyleColor(ImGuiCol_Text, ACCENT);
    SetWindowFontScale(titleScale);
    Text(O("FLUX"));
    SetWindowFontScale(1.0f);
    PopStyleColor();

    SetCursorPosX(rw(0.03f));
    TextColored(TXT_MUTED, O("Engine v1.0"));

    Dummy(ImVec2(sidebarW, rh(0.04f)));

    SetCursorPosX(rw(0.02f));
    if (SidebarButton(O("ESP"), O(">"), g_menu.currentTab == 0, sidebarW)) g_menu.currentTab = 0;

    Dummy(ImVec2(0, rh(0.008f)));
    SetCursorPosX(rw(0.02f));
    if (SidebarButton(O("Auto Play"), O(">"), g_menu.currentTab == 1, sidebarW)) g_menu.currentTab = 1;

    Dummy(ImVec2(0, rh(0.008f)));
    SetCursorPosX(rw(0.02f));
    if (SidebarButton(O("Auto Queue"), O(">"), g_menu.currentTab == 2, sidebarW)) g_menu.currentTab = 2;

    EndGroup();
}

static void DrawContentArea(float sidebarW, float winW, float winH, ImGuiIO& io) {
    bool need_save = false;

    ImDrawList* dl = GetWindowDrawList();
    ImVec2 winPos = GetWindowPos();
    float headerH = rclamp(rh(0.08f), 45.0f, 75.0f);

    ImU32 ch1 = IM_COL32((int)(BG_ELEVATED.x*255), (int)(BG_ELEVATED.y*255), (int)(BG_ELEVATED.z*255), (int)(BG_ELEVATED.w*255));
    ImU32 ch2 = IM_COL32((int)(BG_PANEL.x*255), (int)(BG_PANEL.y*255), (int)(BG_PANEL.z*255), (int)(BG_PANEL.w*255));
    DrawGradientRect(dl, ImVec2(winPos.x + sidebarW, winPos.y), ImVec2(winPos.x + winW, winPos.y + headerH), ch1, ch2, false);
    dl->AddLine(ImVec2(winPos.x + sidebarW, winPos.y + headerH), ImVec2(winPos.x + winW, winPos.y + headerH), Col32(BORDER_GLASS), 1.0f);

    const char* tabTitles[] = { "ESP Settings", "Auto Play", "Auto Queue" };

    float titleScale = rclamp(rh(0.025f), 1.0f, 1.3f);
    SetCursorPos(ImVec2(sidebarW + rw(0.025f), headerH * 0.5f - GetFontSize() * titleScale * 0.5f));
    SetWindowFontScale(titleScale);
    TextColored(TXT_MAIN, "%s", tabTitles[g_menu.currentTab]);
    SetWindowFontScale(1.0f);

    float contentX = sidebarW + rw(0.015f);
    float contentY = headerH + rh(0.015f);
    float contentW = winW - sidebarW - rw(0.03f);
    float contentH = winH - headerH - rh(0.025f);

    SetCursorPos(ImVec2(contentX, contentY));

    PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
    PushStyleVar(ImGuiStyleVar_ScrollbarSize, 6.0f);
    BeginChild(O("##ContentArea"), ImVec2(contentW, contentH), false);

    switch (g_menu.currentTab) {
        case 0: {
            Dummy(ImVec2(0, rh(0.01f)));
            if (BeginTable(O("##ESPTbl"), 2, ImGuiTableFlags_SizingStretchSame)) {
                TableNextColumn(); need_save |= ToggleSwitch(O("Draw Prediction Lines"), &persistent_bool[O("bESP_DrawPredictionLine")]);
                TableNextColumn(); need_save |= ToggleSwitch(O("Draw Pockets"), &persistent_bool[O("bESP_DrawPockets")]);
                TableNextRow();
                TableNextColumn(); need_save |= ToggleSwitch(O("Draw Shot State"), &persistent_bool[O("bESP_DrawPocketsShotState")]);
                EndTable();
            }
            break;
        }

        case 1: {
            Dummy(ImVec2(0, rh(0.01f)));
            if (BeginTable(O("##AutoPlayTbl"), 2, ImGuiTableFlags_SizingStretchSame)) {
                TableNextColumn(); need_save |= ToggleSwitch(O("Enable Auto Play"), &persistent_bool[O("bAutoPlay")]);
                TableNextColumn();
                float textScale = rclamp(rh(0.020f), 0.85f, 1.1f);
                SetWindowFontScale(textScale);
                TextColored(TXT_MUTED, O("Auto play will automatically"));
                TextColored(TXT_MUTED, O("aim and shoot for you"));
                SetWindowFontScale(1.0f);
                EndTable();
            }
            break;
        }

        case 2: {
            Dummy(ImVec2(0, rh(0.01f)));
            if (BeginTable(O("##AutoQTbl"), 2, ImGuiTableFlags_SizingStretchSame)) {
                TableNextColumn(); need_save |= ToggleSwitch(O("Enable Auto Queue"), &persistent_bool[O("bAutoQueue")]);

                TableNextColumn();
                TextColored(TXT_MAIN, O("Mode"));
                Dummy(ImVec2(0, rh(0.01f)));
                PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
                PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(rw(0.02f), rh(0.015f)));
                PushStyleColor(ImGuiCol_FrameBg, FRAME_BG);
                PushStyleColor(ImGuiCol_FrameBgHovered, FRAME_BG_HOVER);
                SetNextItemWidth(GetContentRegionAvail().x);
                need_save |= Combo("##mode", &persistent_int["iAutoQueue_Mode"], "Last Selected\0Smart\0");
                PopStyleColor(2);
                PopStyleVar(2);

                if (persistent_int["iAutoQueue_Mode"] == 1) {
                    Dummy(ImVec2(0, rh(0.02f)));
                    TextColored(TXT_MAIN, O("Bet Percent"));
                    Dummy(ImVec2(0, rh(0.01f)));
                    PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
                    PushStyleVar(ImGuiStyleVar_GrabRounding, 10.0f);
                    PushStyleColor(ImGuiCol_FrameBg, FRAME_BG);
                    PushStyleColor(ImGuiCol_SliderGrab, ACCENT);
                    PushStyleColor(ImGuiCol_SliderGrabActive, ACCENT_ACTIVE);
                    SetNextItemWidth(GetContentRegionAvail().x);
                    need_save |= SliderInt("##betpercent", &persistent_int["iAutoQueue_BetPercent"], 1, 100, "%d%%");
                    PopStyleColor(3);
                    PopStyleVar(2);
                }

                TableNextRow();
                TableSetColumnIndex(0);
                float textScale = rclamp(rh(0.020f), 0.85f, 1.1f);
                SetWindowFontScale(textScale);
                TextColored(TXT_MUTED, O("You will be auto queued to"));
                TextColored(TXT_MUTED, O("the last game mode you played"));
                SetWindowFontScale(1.0f);
                EndTable();
            }
            break;
        }
    }

    if (need_save) save_persistence();

    EndChild();
    PopStyleVar();
    PopStyleColor();
}

INLINE void DrawMenu(ImGuiIO& io) {
    if (!g_Token.empty() && !g_Auth.empty() && g_Token == g_Auth) {
        if (is_segv_handler_active()) {
            jump_buffer_active = 1;
            if (!sigsetjmp(jump_buffer, 1)) DrawESP(GetBackgroundDrawList());
            jump_buffer_active = 0;
        }

        float dt = io.DeltaTime;

        float targetAlpha = g_menu.isOpen ? 1.0f : 0.0f;
        float targetSlide = g_menu.isOpen ? 0.0f : 30.0f;
        float targetScale = g_menu.isOpen ? 1.0f : 0.9f;

        g_menu.menuAlpha += (targetAlpha - g_menu.menuAlpha) * dt * 10.0f;
        g_menu.slideY += (targetSlide - g_menu.slideY) * dt * 8.0f;
        g_menu.menuScale += (targetScale - g_menu.menuScale) * dt * 10.0f;

        if (g_menu.menuAlpha > 0.01f) {
            float winW = rclamp(rw(0.92f), 600.0f, 1200.0f);
            float winH = rclamp(rh(0.88f), 360.0f, 900.0f);
            g_menu.sidebarW = rclamp(rw(0.22f), 150.0f, 320.0f);

            float easedAlpha = EaseOutQuart(g_menu.menuAlpha);

            SetNextWindowSize(ImVec2(winW, winH), ImGuiCond_Always);
            SetNextWindowPos(ImVec2(rw(0.5f), rh(0.5f) + g_menu.slideY), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

            PushStyleColor(ImGuiCol_WindowBg, ImVec4(BG_WINDOW.x, BG_WINDOW.y, BG_WINDOW.z, BG_WINDOW.w * easedAlpha));
            PushStyleVar(ImGuiStyleVar_WindowRounding, 16.0f);
            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
            PushStyleVar(ImGuiStyleVar_Alpha, easedAlpha);

            ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

            if (Begin(O("##MainMenu"), &g_menu.isOpen, winFlags)) {
                ImDrawList* dl = GetWindowDrawList();
                ImVec2 winPos = GetWindowPos();

                ImU32 borderCol = IM_COL32((int)(ACCENT.x*255), (int)(ACCENT.y*255), (int)(ACCENT.z*255), (int)(100*easedAlpha));
                dl->AddRect(winPos, ImVec2(winPos.x+winW, winPos.y+winH), borderCol, 16.0f, 0, 1.5f);

                DrawSidebar(g_menu.sidebarW, winH, io);
                DrawContentArea(g_menu.sidebarW, winW, winH, io);
            }
            End();

            PopStyleVar(4);
            PopStyleColor();
        }
    }
}

static void DrawFloatingButton(ImGuiIO& io) {
    static ImVec2 buttonPos = ImVec2(0, 0);
    static bool posInitialized = false;
    if (!posInitialized) {
        buttonPos = ImVec2(rw(0.05f), rh(0.07f));
        posInitialized = true;
    }
    static bool isDragging = false;
    static float hoverAnim = 0.0f;

    float buttonRadius = rclamp(ImMin(rw(0.025f), rh(0.045f)), 24.0f, 44.0f);
    float buttonSize = buttonRadius * 2.0f;
    float textW = rw(0.10f);
    float totalWidth = buttonSize + textW + rw(0.015f);
    float totalHeight = buttonSize + 4.0f;

    bool isHovered = false;

    SetNextWindowPos(buttonPos, ImGuiCond_Always);
    SetNextWindowSize(ImVec2(totalWidth, totalHeight), ImGuiCond_Always);
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    if (Begin(O("##FloatBtn"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) {
        ImDrawList* dl = GetWindowDrawList();

        ImVec2 center = ImVec2(buttonPos.x + buttonRadius + 2, buttonPos.y + buttonRadius + 2);

        SetCursorPos(ImVec2(0, 0));
        InvisibleButton(O("##FloatBtnHit"), ImVec2(totalWidth, totalHeight));
        isHovered = IsItemHovered();

        float targetHover = isHovered ? 1.0f : 0.0f;
        hoverAnim += (targetHover - hoverAnim) * io.DeltaTime * 10.0f;

        float currentRadius = buttonRadius + hoverAnim * 4.0f;

        dl->AddCircleFilled(ImVec2(center.x + 2, center.y + 3), currentRadius, IM_COL32(0, 0, 0, 60), 32);
        dl->AddCircleFilled(center, currentRadius, Col32(ACCENT), 32);
        dl->AddCircleFilled(center, currentRadius - 4, Col32(ACCENT_HOVER), 32);
        dl->AddCircle(center, currentRadius, Col32(ImVec4(ACCENT.x, ACCENT.y, ACCENT.z, 0.7f+0.2f*hoverAnim)), 32, 2.5f);

        float iconSize = rclamp(rh(0.015f), 10.0f, 16.0f);
        ImU32 iconColor = IM_COL32(255, 255, 255, 255);

        if (g_menu.isOpen) {
            dl->AddLine(ImVec2(center.x - iconSize, center.y - iconSize), ImVec2(center.x + iconSize, center.y + iconSize), iconColor, 3.5f);
            dl->AddLine(ImVec2(center.x + iconSize, center.y - iconSize), ImVec2(center.x - iconSize, center.y + iconSize), iconColor, 3.5f);
        } else {
            float barW = iconSize;
            float barH = rh(0.004f);
            float gap = rh(0.007f);
            float halfH = barH * 0.5f;
            dl->AddRectFilled(ImVec2(center.x - barW, center.y - gap - barH), ImVec2(center.x + barW, center.y - gap), iconColor, 2.0f);
            dl->AddRectFilled(ImVec2(center.x - barW, center.y - halfH), ImVec2(center.x + barW, center.y + halfH), iconColor, 2.0f);
            dl->AddRectFilled(ImVec2(center.x - barW, center.y + gap), ImVec2(center.x + barW, center.y + gap + barH), iconColor, 2.0f);
        }

        float textX = buttonPos.x + buttonSize + rw(0.015f);
        float textY = buttonPos.y + (totalHeight - GetFontSize() * 2.0f - rh(0.01f)) * 0.5f;
        float fontScale = rclamp(rh(0.018f), 0.8f, 1.2f);
        SetWindowFontScale(fontScale);
        dl->AddText(ImVec2(textX, textY), Col32(ACCENT), O("FLUX"));
        dl->AddText(ImVec2(textX, textY + GetFontSize() * fontScale + rh(0.005f)), Col32(TXT_MUTED), O("Engine"));
        SetWindowFontScale(1.0f);

        if (IsItemActive() && IsMouseDragging(0)) {
            isDragging = true;
            buttonPos.x += io.MouseDelta.x;
            buttonPos.y += io.MouseDelta.y;
            buttonPos.x = ImClamp(buttonPos.x, 0.0f, io.DisplaySize.x - totalWidth);
            buttonPos.y = ImClamp(buttonPos.y, 0.0f, io.DisplaySize.y - totalHeight);
        }

        if (IsItemHovered() && IsMouseReleased(0) && !isDragging) {
            g_menu.isOpen = !g_menu.isOpen;
        }

        if (!IsItemActive()) isDragging = false;
    }
    End();

    PopStyleVar(2);
    PopStyleColor();
}


static bool first_time = true;
INLINE void DrawLogin(ImGuiIO& io) {
    if (logged_in) return DrawMenu(io);

    SetNextWindowPos(ImVec2(0, 0));
    SetNextWindowSize(io.DisplaySize);
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.55f));
    Begin(O("##Overlay"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBringToFrontOnFocus);
    PopStyleColor();

    float cardW = rclamp(rw(0.70f), 360.0f, 640.0f);
    float cardH = rclamp(rh(0.58f), 320.0f, 520.0f);

    SetNextWindowSize(ImVec2(cardW, cardH), ImGuiCond_Always);
    SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    PushStyleColor(ImGuiCol_WindowBg, BG_WINDOW);
    PushStyleVar(ImGuiStyleVar_WindowRounding, 20.0f);
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

    Begin(O("##LoginCard"), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

    ImDrawList* dl = GetWindowDrawList();
    ImVec2 winPos = GetWindowPos();

    float headerH = rclamp(rh(0.14f), 80.0f, 130.0f);
    ImU32 gh1 = IM_COL32((int)(ACCENT.x*255), (int)(ACCENT.y*255), (int)(ACCENT.z*255), 255);
    ImU32 gh2 = IM_COL32((int)(ACCENT_ACTIVE.x*255), (int)(ACCENT_ACTIVE.y*255), (int)(ACCENT_ACTIVE.z*255), 255);
    DrawGradientRect(dl, winPos, ImVec2(winPos.x + cardW, winPos.y + headerH), gh1, gh2, true);
    dl->AddRectFilled(winPos, ImVec2(winPos.x + cardW, winPos.y + 20), gh1, 20.0f, ImDrawFlags_RoundCornersTop);

    float titleScale = rclamp(rh(0.032f), 1.2f, 1.8f);
    SetWindowFontScale(titleScale);
    ImVec2 titleSize = CalcTextSize("FLUX ENGINE");
    dl->AddText(ImVec2(winPos.x + (cardW - titleSize.x) * 0.5f, winPos.y + headerH * 0.3f), IM_COL32(255, 255, 255, 255), "FLUX ENGINE");
    SetWindowFontScale(1.0f);

    ImVec2 subSize = CalcTextSize("Premium 8 Ball Pool Mod");
    dl->AddText(ImVec2(winPos.x + (cardW - subSize.x) * 0.5f, winPos.y + headerH * 0.65f), Col32(ImVec4(1.0f, 1.0f, 1.0f, 0.75f)), "Premium 8 Ball Pool Mod");

    SetCursorPosY(headerH + rh(0.02f));

    if (!ERROR_MESSAGE.empty()) {
        SetCursorPosX(rw(0.04f));
        PushTextWrapPos(cardW - rw(0.04f));
        TextColored(ImVec4(DANGER.x, DANGER.y, DANGER.z, 1.0f), "%s", ERROR_MESSAGE.c_str());
        PopTextWrapPos();
        Dummy(ImVec2(0, rh(0.02f)));
    }

    if (is_logging_in) {
        SetCursorPosY(headerH + rh(0.06f));

        static float spinner_angle = 0.0f;
        spinner_angle += io.DeltaTime * 5.0f;

        float spinner_size = rclamp(rh(0.05f), 24.0f, 48.0f);
        ImVec2 spinnerCenter = ImVec2(winPos.x + cardW * 0.5f, winPos.y + cardH * 0.5f - rh(0.03f));

        for (int i = 0; i < 12; i++) {
            float angle = spinner_angle + (i * PI * 2.0f / 12.0f);
            float alpha = (float)(12 - i) / 12.0f;
            float dotR = rclamp(rh(0.008f), 4.0f, 8.0f);
            ImVec2 dotPos = ImVec2(
                spinnerCenter.x + cosf(angle) * spinner_size,
                spinnerCenter.y + sinf(angle) * spinner_size
            );
            dl->AddCircleFilled(dotPos, dotR, IM_COL32((int)(ACCENT.x*255), (int)(ACCENT.y*255), (int)(ACCENT.z*255), (int)(alpha*255)));
        }

        float loadingScale = rclamp(rh(0.022f), 0.9f, 1.2f);
        SetWindowFontScale(loadingScale);
        ImVec2 loadingSize = CalcTextSize("Authenticating...");
        SetCursorPosX((cardW - loadingSize.x) * 0.5f);
        SetCursorPosY(cardH * 0.7f);
        TextColored(TXT_MUTED, "Authenticating...");
        SetWindowFontScale(1.0f);
    } else {
        SetCursorPosY(headerH + rh(0.03f));

        float infoScale = rclamp(rh(0.020f), 0.85f, 1.1f);
        SetWindowFontScale(infoScale);
        ImVec2 infoSize = CalcTextSize("Copy your license key and tap login");
        SetCursorPosX((cardW - infoSize.x) * 0.5f);
        TextColored(TXT_MUTED, "Copy your license key and tap login");
        SetWindowFontScale(1.0f);

        Dummy(ImVec2(0, rh(0.06f)));

        bool AutoLogin = first_time && !persistent_string["key"].empty();

        float btnW = cardW - rw(0.10f);
        float btnH = rclamp(rh(0.08f), 44.0f, 72.0f);
        SetCursorPosX((cardW - btnW) * 0.5f);
        PushStyleColor(ImGuiCol_Button, FRAME_BG);
        PushStyleColor(ImGuiCol_ButtonHovered, FRAME_BG_HOVER);
        PushStyleColor(ImGuiCol_ButtonActive, ACCENT);
        PushStyleVar(ImGuiStyleVar_FrameRounding, 14.0f);

        if (AutoLogin || Button("LOGIN WITH CLIPBOARD", ImVec2(btnW, btnH))) {
            JNIEnv* env;
            jint getEnvResult = VM->GetEnv((void**)&env, JNI_VERSION_1_6);
            if (getEnvResult == JNI_EDETACHED) {
                if (VM->AttachCurrentThread(&env, nullptr) != 0) ERROR_MESSAGE = O("Failed to attach thread to JVM");
            } else if (getEnvResult != JNI_OK) {
                ERROR_MESSAGE = O("Failed to get JNIEnv");
            } else {
                std::thread([](std::string androidId, std::string key) {
                    Login(androidId, key);
                }, getAndroidID(env), AutoLogin ? persistent_string["key"] : getClipboard(env)).detach();
            }

            first_time = false;
        }

        PopStyleVar();
        PopStyleColor(3);

        Dummy(ImVec2(0, rh(0.04f)));

        float helpScale = rclamp(rh(0.018f), 0.8f, 1.0f);
        SetWindowFontScale(helpScale);
        ImVec2 helpSize = CalcTextSize("Your key will be read from clipboard");
        SetCursorPosX((cardW - helpSize.x) * 0.5f);
        TextColored(TXT_DIM, "Your key will be read from clipboard");
        SetWindowFontScale(1.0f);
    }

    End();
    PopStyleVar(3);
    PopStyleColor();

    End();
}


INLINE void SetupImgui() {
    PACKAGE_NAME = string(getcmdline());

    ImGui::CreateContext();

    auto& style = ImGui::GetStyle();
    auto& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;

    switch_theme(current_theme);

    load_persistence();
    load_imgui_style();

    static string INI_PATH = O("/data/user_de/0/") + PACKAGE_NAME + O("/no_backup/.ini");
    io.IniFilename = persistent_bool["bImguiAutoSave"] ? INI_PATH.c_str() : nullptr;
    io.ConfigWindowsMoveFromTitleBarOnly = persistent_bool["bMoveOnlyWithTitleBar"];

    float baseFontSize = rclamp(rh(0.028f), 14.0f, 22.0f);
    io.FontGlobalScale = 1.0f;
    ImFontConfig font_cfg;
    font_cfg.SizePixels = persistent_float["fFontScale"];
    if (font_cfg.SizePixels < 1.0f) font_cfg.SizePixels = baseFontSize;
    io.Fonts->AddFontDefault(&font_cfg);

    ImGui_ImplAndroid_Init();
    ImGui_ImplOpenGL3_Init(O("#version 300 es"));

    bImguiSetup = true;
}

DEFINES(EGLBoolean, Draw, EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &Width);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &Height);

    if (Width <= 0 || Height <= 0) return _Draw(dpy, surface);

    screenCenter = Vector2(Width / 2, Height / 2);

    if (!bImguiSetup) SetupImgui();

    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(Width, Height);
    ImGui::NewFrame();

    if (!is_segv_handler_active()) setup_global_segv_handler();

    if (g_splash.startTime == 0.0f && !g_splash.done) {
        g_splash.startTime = ImGui::GetTime();
    }

    if (!g_splash.done) {
        DrawSplashScreen(io);
    } else if (!g_Token.empty() && !g_Auth.empty() && g_Token == g_Auth) {
        DrawFloatingButton(io);
        DrawMenu(io);
    } else {
        DrawLogin(io);
    }
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui_ClearHoverEffect();

    return _Draw(dpy, surface);
}

void __IMGUI__() {
    create_directory_recursive(CONC(O("/data/user_de/0/"), PACKAGE_NAME.c_str(), O("/no_backup")));
}
