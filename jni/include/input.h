#pragma once

bool bTouchFromOutsideImGui = false;

#include "java.h"

DEFINES(void, nativeTouchesBegin, JNIEnv* env, jobject obj, jint i, float x, float y, jboolean isObscured, jboolean isPartiallyObscured) {
    // LOGI("nativeTouchesBegin %d %f %f %d %d", i, x, y, isObscured, isPartiallyObscured);
    if (!bImguiSetup) return _nativeTouchesBegin(env, obj, i, x, y, isObscured, isPartiallyObscured);

    if (i == 0) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[i] = true;
        io.MousePos = ImVec2(x, y);

        bTouchFromOutsideImGui = !IsPointOverAnyWindow({x, y});
        // LOGI("bTouchFromOutsideImGui %d", bTouchFromOutsideImGui);
    }

    // if (selector.IsActive()) return;
    if (bTouchFromOutsideImGui) return _nativeTouchesBegin(env, obj, i, x, y, isObscured, isPartiallyObscured);
}

DEFINES(void, nativeTouchesEnd, JNIEnv* env, jobject obj, jint i, float x, float y, jboolean isObscured, jboolean isPartiallyObscured) {
    // LOGI("nativeTouchesEnd %d %f %f %d %d", i, x, y, isObscured, isPartiallyObscured);
    if (!bImguiSetup) return _nativeTouchesEnd(env, obj, i, x, y, isObscured, isPartiallyObscured);
    
    if (i == 0) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[i] = false;
        
        if (bTouchFromOutsideImGui) bTouchFromOutsideImGui = false;
        else return;
    }
    
    // if (selector.IsActive()) return;
    return _nativeTouchesEnd(env, obj, i, x, y, isObscured, isPartiallyObscured);
}

DEFINES(void, nativeTouchesMove, JNIEnv* env, jobject obj, jintArray ids, jfloatArray xs, jfloatArray ys, jboolean isObscured, jboolean isPartiallyObscured) {
    jsize length = env->GetArrayLength(ids);
    if (!bImguiSetup || length <= 0) return _nativeTouchesMove(env, obj, ids, xs, ys, isObscured, isPartiallyObscured);

    jint* id_elements = env->GetIntArrayElements(ids, NULL);
    jfloat* x_elements = env->GetFloatArrayElements(xs, NULL);
    jfloat* y_elements = env->GetFloatArrayElements(ys, NULL);
    
    int i = id_elements[0];
    float x = x_elements[0], y = y_elements[0];

    // LOGI("nativeTouchesMove %d %f %f %d %d", i, x, y, isObscured, isPartiallyObscured);

    env->ReleaseIntArrayElements(ids, id_elements, JNI_ABORT);
    env->ReleaseFloatArrayElements(xs, x_elements, JNI_ABORT);
    env->ReleaseFloatArrayElements(ys, y_elements, JNI_ABORT);

    if (i == 0) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(x, y);
    }
    
    // if (selector.IsActive()) return;
    if (bTouchFromOutsideImGui) return _nativeTouchesMove(env, obj, ids, xs, ys, isObscured, isPartiallyObscured);
}

void NativeTouchesBegin(int idx, float x, float y) {
    // LOGI("NativeTouchesBegin %d %f %f", idx, x, y);
    _nativeTouchesBegin(GetJNIEnv(), nullptr, idx, x, y, false, false);
}

void NativeTouchesEnd(int idx, float x, float y) {
    // LOGI("NativeTouchesEnd %d %f %f", idx, x, y);
    _nativeTouchesEnd(GetJNIEnv(), nullptr, idx, x, y, false, false);
}

void NativeTouchesMove(int idx, float x, float y) {
    // LOGI("NativeTouchesMove %d %f %f", idx, x, y);
    JNIEnv* env = GetJNIEnv();

    jintArray ids = env->NewIntArray(1);
    env->SetIntArrayRegion(ids, 0, 1, &idx);
    jfloatArray xs = env->NewFloatArray(1);
    env->SetFloatArrayRegion(xs, 0, 1, &x);
    jfloatArray ys = env->NewFloatArray(1);
    env->SetFloatArrayRegion(ys, 0, 1, &y);
    
    _nativeTouchesMove(env, nullptr, ids, xs, ys, false, false);
    
    env->DeleteLocalRef(ids);
    env->DeleteLocalRef(xs);
    env->DeleteLocalRef(ys);
}

#include "xhook/xhook.h"

void __INPUT__() {
    LOGI("__INPUT__");
    
    xhook_clear();
    xhook_register(".*/com.miniclip.eightballpool/.*", "Java_com_miniclip_input_MCInput_nativeTouchesBegin", (void*)nativeTouchesBegin, (void**)&_nativeTouchesBegin);
    xhook_register(".*/com.miniclip.eightballpool/.*", "Java_com_miniclip_input_MCInput_nativeTouchesEnd", (void*)nativeTouchesEnd, (void**)&_nativeTouchesEnd);
    xhook_register(".*/com.miniclip.eightballpool/.*", "Java_com_miniclip_input_MCInput_nativeTouchesMove", (void*)nativeTouchesMove, (void**)&_nativeTouchesMove);
    if (xhook_refresh(0)) LOGI("xhook_refresh failed");
}
