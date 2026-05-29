#pragma once

#include "../src/imgui.h"
#include "include/includes.h"

#include "persistence.h"

#include <android/input.h>

bool g_DraggingFromOutsideImGui = false;
int g_ActivePointerIndex = -1;

bool ShouldConsumeTouch(AInputEvent *inputEvent) {
    if (!inputEvent) return false;
    if (AInputEvent_getType(inputEvent) != AINPUT_EVENT_TYPE_MOTION) return false;

    int32_t action = AMotionEvent_getAction(inputEvent);
    int32_t pointer_index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
    int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;

    int32_t pointer_count = AMotionEvent_getPointerCount(inputEvent);
    if (pointer_index < 0 || pointer_index >= pointer_count) return false;
    if (pointer_index != 0) return false;
    
    float x = AMotionEvent_getRawX(inputEvent, pointer_index);
    float y = AMotionEvent_getRawY(inputEvent, pointer_index);

    // bool xyz = IsPointOverAnyWindow({x, y});
    // LOGI("ShouldConsumeTouch: %f %f: %i", x, y, xyz);
    
    if (actionMasked == AMOTION_EVENT_ACTION_DOWN) {
        if (!IsPointOverAnyWindow({x, y})) {
            g_DraggingFromOutsideImGui = true;
            g_ActivePointerIndex = pointer_index;
        } else {
            g_DraggingFromOutsideImGui = false;
            g_ActivePointerIndex = -1;
        }
    } else if (actionMasked == AMOTION_EVENT_ACTION_UP || actionMasked == AMOTION_EVENT_ACTION_CANCEL) {
        if (pointer_index == g_ActivePointerIndex) {
            g_DraggingFromOutsideImGui = false;
            g_ActivePointerIndex = -1;
            return false;
        }
    }

    if (g_DraggingFromOutsideImGui && pointer_index == g_ActivePointerIndex) return false;

    return IsPointOverAnyWindow({x, y});
}
