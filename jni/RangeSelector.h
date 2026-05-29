#pragma once

#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "include/external/imgui/inc/persistence.h"
#include "include/includes.h"
#include "include/java.h"
#include "include/input.h"

struct RangeSelector {
    ImVec4 Range = {0, 0, 0, 0}; // x, y, w, h
    bool Visible = false;
    bool Selecting = false;
    bool Editing = false;

    std::vector<ImVec4> UndoStack;
    std::vector<ImVec4> RedoStack;

    void PushState() {
        UndoStack.push_back(Range);
        if (UndoStack.size() > 50) UndoStack.erase(UndoStack.begin());
        RedoStack.clear();
    }

    void Undo() {
        if (UndoStack.empty()) return;
        RedoStack.push_back(Range);
        Range = UndoStack.back();
        UndoStack.pop_back();
    }

    void Redo() {
        if (RedoStack.empty()) return;
        UndoStack.push_back(Range);
        Range = RedoStack.back();
        RedoStack.pop_back();
    }

    // Internal state
    // 0: None, 1: Move, 2: N, 3: E, 4: S, 5: W, 6: NE, 7: SE, 8: SW, 9: NW
    int ResizeMode = 0;
    ImVec2 DragStart;
    ImVec4 InitialRange;

    void DrawSelector() {
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* fg = ImGui::GetForegroundDrawList();

        // 1. Handle Selection (Creation) - Marquee selection
        if (Selecting) {
            // While selecting, we just update the width/height based on start pos
            float curr_w = io.MousePos.x - Range.x;
            float curr_h = io.MousePos.y - Range.y;

            // Visual feedback uses raw values, normalization happens on release
            ImVec2 p1(Range.x, Range.y);
            ImVec2 p2(io.MousePos);

            fg->AddRect(p1, p2, IM_COL32(255, 255, 0, 255), 0.0f, 0, 2.0f);
            fg->AddRectFilled(p1, p2, IM_COL32(255, 255, 0, 50));

            if (io.MouseReleased[0]) {
                Selecting = false;

                // Finalize rect
                Range.z = curr_w;
                Range.w = curr_h;

                // Normalize
                if (Range.z < 0) { Range.x += Range.z; Range.z *= -1; }
                if (Range.w < 0) { Range.y += Range.w; Range.w *= -1; }

                // Minimum size
                if (Range.z < 50) Range.z = 50;
                if (Range.w < 50) Range.w = 50;
            } return; // Skip other logic while selecting
        }

        ImVec2 p1(Range.x, Range.y);
        ImVec2 p2(Range.x + Range.z, Range.y + Range.w);

        // Draw Selection
        if (Visible || Editing) {
            fg->AddRectFilled(p1, p2, IM_COL32(0, 255, 0, 40));
            fg->AddRect(p1, p2, IM_COL32(0, 255, 0, 255), 0.0f, 0, 2.0f);
        }

        // 2. Handle Editing (Resize/Move) or start new selection
        if (Editing) {
            // Draw Handles
            float handle_size = ImGui::GetFrameHeight() * 0.15f;
            float half_handle = handle_size / 2.0f;
            ImU32 handle_col = IM_COL32(255, 255, 255, 255);

            // Positions
            ImVec2 corners[4] = { p1, {p2.x, p1.y}, p2, {p1.x, p2.y} };
            ImVec2 midpoints[4] = {
                {p1.x + Range.z/2, p1.y}, // Top
                {p2.x, p1.y + Range.w/2}, // Right
                {p1.x + Range.z/2, p2.y}, // Bottom
                {p1.x, p1.y + Range.w/2}  // Left
            };

            // Draw corner handles (L-shapes)
            float arm_len = ImGui::GetFrameHeight() * 0.3f;
            float thickness = 4.0f;

            // Top-Left
            fg->AddLine(corners[0], ImVec2(corners[0].x + arm_len, corners[0].y), handle_col, thickness);
            fg->AddLine(corners[0], ImVec2(corners[0].x, corners[0].y + arm_len), handle_col, thickness);
            // Top-Right
            fg->AddLine(corners[1], ImVec2(corners[1].x - arm_len, corners[1].y), handle_col, thickness);
            fg->AddLine(corners[1], ImVec2(corners[1].x, corners[1].y + arm_len), handle_col, thickness);
            // Bottom-Right
            fg->AddLine(corners[2], ImVec2(corners[2].x - arm_len, corners[2].y), handle_col, thickness);
            fg->AddLine(corners[2], ImVec2(corners[2].x, corners[2].y - arm_len), handle_col, thickness);
            // Bottom-Left
            fg->AddLine(corners[3], ImVec2(corners[3].x + arm_len, corners[3].y), handle_col, thickness);
            fg->AddLine(corners[3], ImVec2(corners[3].x, corners[3].y - arm_len), handle_col, thickness);

            // Draw edge handles (lines)
            float edge_len = ImGui::GetFrameHeight() * 0.2f;
            // Top
            fg->AddLine(ImVec2(midpoints[0].x - edge_len/2, midpoints[0].y), ImVec2(midpoints[0].x + edge_len/2, midpoints[0].y), handle_col, thickness);
            // Right
            fg->AddLine(ImVec2(midpoints[1].x, midpoints[1].y - edge_len/2), ImVec2(midpoints[1].x, midpoints[1].y + edge_len/2), handle_col, thickness);
            // Bottom
            fg->AddLine(ImVec2(midpoints[2].x - edge_len/2, midpoints[2].y), ImVec2(midpoints[2].x + edge_len/2, midpoints[2].y), handle_col, thickness);
            // Left
            fg->AddLine(ImVec2(midpoints[3].x, midpoints[3].y - edge_len/2), ImVec2(midpoints[3].x, midpoints[3].y + edge_len/2), handle_col, thickness);

            // Interaction Logic
            if (io.MouseDown[0]) {
                if (ResizeMode == 0) {
                    // Check if hovering toolbar (Undo, Redo, Close) FIRST
                    float button_size = ImGui::GetFrameHeight() * 2.f;
                    float spacing = ImGui::GetStyle().ItemSpacing.x;
                    float margin = 20.0f;
                    // 3 buttons: Undo, Redo, Close
                    float total_width = (button_size * 3) + (spacing * 2);
                    ImVec2 toolbar_end(io.DisplaySize.x - margin, margin + button_size);
                    ImVec2 toolbar_start(toolbar_end.x - total_width, margin);

                    bool hovering_toolbar = io.MousePos.x >= toolbar_start.x && io.MousePos.x <= toolbar_end.x && io.MousePos.y >= toolbar_start.y && io.MousePos.y <= toolbar_end.y;
                    if (hovering_toolbar) return; // Dont touch background, toolbar will get touch in DrawButton

                    // Check hits
                    ImVec2 m = io.MousePos;
                    float grip = 50.0f; // Larger grip area for easier interaction

                    // Helper for hit test
                    auto Hit = [&](ImVec2 p) { return (m.x >= p.x - grip && m.x <= p.x + grip && m.y >= p.y - grip && m.y <= p.y + grip); };

                    if (Hit(corners[0])) ResizeMode = 9; // NW
                    else if (Hit(corners[1])) ResizeMode = 6; // NE
                    else if (Hit(corners[2])) ResizeMode = 7; // SE
                    else if (Hit(corners[3])) ResizeMode = 8; // SW
                    else if (Hit(midpoints[0])) ResizeMode = 2; // N
                    else if (Hit(midpoints[1])) ResizeMode = 3; // E
                    else if (Hit(midpoints[2])) ResizeMode = 4; // S
                    else if (Hit(midpoints[3])) ResizeMode = 5; // W
                    else if (m.x >= p1.x && m.x <= p2.x && m.y >= p1.y && m.y <= p2.y) ResizeMode = 1; // Move
                    else {
                        // Clicked outside - start new marquee selection at mouse position
                        PushState();
                        Selecting = true;
                        Range.x = io.MousePos.x;
                        Range.y = io.MousePos.y;
                        Range.z = 0;
                        Range.w = 0;
                        return;
                    }

                    if (ResizeMode != 0) {
                        PushState();
                        DragStart = m;
                        InitialRange = Range;
                    }
                } else {
                    // Apply Drag
                    ImVec2 delta = { io.MousePos.x - DragStart.x, io.MousePos.y - DragStart.y };

                    if (ResizeMode == 1) { // Move
                        Range.x = InitialRange.x + delta.x;
                        Range.y = InitialRange.y + delta.y;
                    } else {
                        // Resize logic
                        // N (Top)
                        if (ResizeMode == 2 || ResizeMode == 6 || ResizeMode == 9) {
                            float new_h = InitialRange.w - delta.y;
                            if (new_h < 50.0f) new_h = 50.0f;
                            Range.w = new_h;
                            Range.y = (InitialRange.y + InitialRange.w) - new_h;
                        }
                        // S (Bottom)
                        if (ResizeMode == 4 || ResizeMode == 7 || ResizeMode == 8) {
                            float new_h = InitialRange.w + delta.y;
                            if (new_h < 50.0f) new_h = 50.0f;
                            Range.w = new_h;
                        }
                        // W (Left)
                        if (ResizeMode == 5 || ResizeMode == 8 || ResizeMode == 9) {
                            float new_w = InitialRange.z - delta.x;
                            if (new_w < 50.0f) new_w = 50.0f;
                            Range.z = new_w;
                            Range.x = (InitialRange.x + InitialRange.z) - new_w;
                        }
                        // E (Right)
                        if (ResizeMode == 3 || ResizeMode == 6 || ResizeMode == 7) {
                            float new_w = InitialRange.z + delta.x;
                            if (new_w < 50.0f) new_w = 50.0f;
                            Range.z = new_w;
                        }
                    }
                }
            } else {
                ResizeMode = 0;
            }
        }
    }

    void Update() {
        DrawSelector();
        if (Editing) DrawToggleButton();
    }

    bool IsActive() { return Editing; }

    void DrawToggleButton() {
        if (!Editing) return;

        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* fg = ImGui::GetForegroundDrawList();

        float button_size = ImGui::GetFrameHeight() * 2.f;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float margin = 20.0f;

        auto DrawButton = [&](ImVec2 pos, const char* label, bool& clicked) {
            ImVec2 end(pos.x + button_size, pos.y + button_size);
            bool is_hovering = io.MousePos.x >= pos.x && io.MousePos.x <= end.x && io.MousePos.y >= pos.y && io.MousePos.y <= end.y;

            ImU32 bg_color = is_hovering ? IM_COL32(80, 80, 80, 200) : IM_COL32(50, 50, 50, 180);
            fg->AddRectFilled(pos, end, bg_color, 5.0f);
            fg->AddRect(pos, end, IM_COL32(200, 200, 200, 255), 5.0f, 0, 2.0f);

            // Draw icon
            ImVec2 center(pos.x + button_size/2, pos.y + button_size/2);
            float size = button_size * 0.3f;

            if (strcmp(label, "X") == 0) {
                fg->AddLine({center.x - size, center.y - size}, {center.x + size, center.y + size}, IM_COL32(255, 255, 255, 255), 3.0f);
                fg->AddLine({center.x + size, center.y - size}, {center.x - size, center.y + size}, IM_COL32(255, 255, 255, 255), 3.0f);
            } else if (strcmp(label, "U") == 0) { // Undo
                float shift = size * 0.1f;
                fg->AddLine(ImVec2(center.x + size/2 - shift, center.y - size/2), ImVec2(center.x - size/2 - shift, center.y), IM_COL32(255, 255, 255, 255), 3.0f);
                fg->AddLine(ImVec2(center.x - size/2 - shift, center.y), ImVec2(center.x + size/2 - shift, center.y + size/2), IM_COL32(255, 255, 255, 255), 3.0f);
            } else if (strcmp(label, "R") == 0) { // Redo
                float shift = size * 0.1f;
                fg->AddLine(ImVec2(center.x - size/2 + shift, center.y - size/2), ImVec2(center.x + size/2 + shift, center.y), IM_COL32(255, 255, 255, 255), 3.0f);
                fg->AddLine(ImVec2(center.x + size/2 + shift, center.y), ImVec2(center.x - size/2 + shift, center.y + size/2), IM_COL32(255, 255, 255, 255), 3.0f);
            }

            if (is_hovering && io.MouseClicked[0]) clicked = true;
        };

        // Close Button (Rightmost)
        ImVec2 close_pos(io.DisplaySize.x - button_size - margin, margin);
        bool close_clicked = false;
        DrawButton(close_pos, "X", close_clicked);

        if (close_clicked) {
            // Save current rect to persistent storage
            persistent_float["fSelectedRangeX"] = Range.x;
            persistent_float["fSelectedRangeY"] = Range.y;
            persistent_float["fSelectedRangeW"] = Range.z;
            persistent_float["fSelectedRangeH"] = Range.w;
            save_persistence();
                
            Editing = false;
            Selecting = false;
            return;
        }

        // Redo Button
        ImVec2 redo_pos(close_pos.x - button_size - spacing, margin);
        bool redo_clicked = false;
        DrawButton(redo_pos, "R", redo_clicked);
        if (redo_clicked) Redo();

        // Undo Button
        ImVec2 undo_pos(redo_pos.x - button_size - spacing, margin);
        bool undo_clicked = false;
        DrawButton(undo_pos, "U", undo_clicked);
        if (undo_clicked) Undo();
    }

    void Start() {
        ImGuiIO& io = ImGui::GetIO();

        if (Range.z < 10.0f || Range.w < 10.0f) {
            float default_width = 300.0f;
            float default_height = 200.0f;

            Range.x = (io.DisplaySize.x - default_width) / 2.0f;
            Range.y = (io.DisplaySize.y - default_height) / 2.0f;
            Range.z = default_width;
            Range.w = default_height;
        }

        Selecting = false;
        Editing = true;
    }
};

RangeSelector selector;
