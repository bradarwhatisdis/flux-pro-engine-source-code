#pragma once

#include <android/native_activity.h>
#include <native_app_glue/android_native_app_glue.h>

#include <imgui/imgui.h>
#include <dlfcn/dlfcn.hpp>

#include "includes.h"
#include "hook.h"
// #include "obfuscate.h"

DEFINES(float, _AMotionEvent_getX, AInputEvent* motion_event, size_t pointer_index) {
    return ShouldConsumeTouch(motion_event) ? -99999.0f : __AMotionEvent_getX(motion_event, pointer_index);
}

DEFINES(float, _AMotionEvent_getY, AInputEvent* motion_event, size_t pointer_index) {
    return ShouldConsumeTouch(motion_event) ? -99999.0f : __AMotionEvent_getY(motion_event, pointer_index);
}

extern bool bImguiSetup;

// Custom input handler that adjusts coordinates for notch cutout
int32_t ImGui_ImplAndroid_HandleInputEvent_Adjusted(AInputEvent *input_event) {
    if (!input_event) return 0;
    
    ImGuiIO &io = ImGui::GetIO();
    int32_t event_type = AInputEvent_getType(input_event);
    
    if (event_type == AINPUT_EVENT_TYPE_MOTION) {
        int32_t event_action = AMotionEvent_getAction(input_event);
        int32_t event_pointer_index = (event_action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        event_action &= AMOTION_EVENT_ACTION_MASK;
        
        // auto screen_height = F(float, libmain + 0x4e294c8);
        auto screen_width = F(float, libmain + 0x4e294cc);
        
        float rawX = AMotionEvent_getRawX(input_event, event_pointer_index);
        float rawY = AMotionEvent_getRawY(input_event, event_pointer_index);
        
        float notchOffset = Orientation == 1 ? screen_width - (float)Width : 0.f;
        
        float adjustedX = rawX - notchOffset;
        float adjustedY = rawY;
        
        // LOGI("RAW: %.0f,%.0f | ADJ: %.0f,%.0f | Offset: %.0f | Screen: %.0fx%.0f | Window: %dx%d", \
             rawX, rawY, adjustedX, adjustedY, notchOffset, screen_width, screen_height, Width, Height);
        
        io.MousePos = ImVec2(adjustedX, adjustedY);
        
        if ((AMotionEvent_getToolType(input_event, event_pointer_index) == AMOTION_EVENT_TOOL_TYPE_FINGER) ||
            (AMotionEvent_getToolType(input_event, event_pointer_index) == AMOTION_EVENT_TOOL_TYPE_UNKNOWN)) {
            if (event_action == AMOTION_EVENT_ACTION_DOWN) {
                io.MouseDown[0] = true;
            } else if (event_action == AMOTION_EVENT_ACTION_UP) {
                io.MouseDown[0] = false;
            }
        }
        
        return 1;
    }
    
    return 0;
}

DEFINES(void, InitializeMotionEvent, AInputEvent* event, const void* msg) {
    _InitializeMotionEvent(event, msg);
    if (bImguiSetup && event && libmain) {
        auto height = F(float, libmain + 0x4e294c8);
        auto width = F(float, libmain + 0x4e294cc);

        if (width && height && Width > 0 && Height > 0) {
            ImGui_ImplAndroid_HandleInputEvent_Adjusted(event);
        }
    }
}

void __INPUT__() {
    LOGI("ENTERING INPUT");
    
    // HOOKS2("libandroid.so", "AMotionEvent_getX", _AMotionEvent_getX, "AMotionEvent_getY", _AMotionEvent_getY);
    HOOKS("libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE", InitializeMotionEvent);

    LOGI("RETURNING FROM INPUT");
}
