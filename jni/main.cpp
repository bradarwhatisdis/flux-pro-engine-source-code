#include "include/includes.h"
#include "include/hook.h"

#include <android/log.h>
#include <stdint.h>
#include <jni.h>

#include "include/input.h"
#include "include/java.h"

#include "include/obfuscation.h"

#include "include/manual_dlsym.h"
#include "include/random_defs.h"

#include "draw.h"

DEFINES(int32_t, setActiveVisualCue, ptr arg1) {
    sharedGameManager = arg1;
    // LOGI("GameManager %p", arg1);
    return _setActiveVisualCue(arg1);
}

void __HOOKS__() {
    LOGI("__HOOKS__");

    HOOK(libmain + O(0x2d911e0), setActiveVisualCue); // to get sharedGameManager
    // HOOK(libmain + 0x368b390, STRUE); // isVIPFeatureActive
    // HOOK(libmain + 0x358fdc4, STRUE); // isPayingUser
    HOOK(libmain + O(0x3068c94), StartMatch);

    // HOOK(libmain + 0x390cd80, convertToGL);
    // HOOK(libmain + 0x390cfd4, convertToUI);
    // HOOK(libmain + 0x2b1bb3c, FUN_02b1bb3c);
    // HOOK(libmain + 0x2b1bfc0, _FUN_02b1bfc0);
    // HOOK(libmain + 0x392376c, convertTouchToNodeSpace);

    // HOOKS("libEGL.so", "eglSwapBuffers", Draw);
    xhook_clear();
    xhook_register(O(".*/com.miniclip.eightballpool/.*"), O("eglSwapBuffers"), (void*)Draw, (void**)&_Draw);
    if (xhook_refresh(0)) LOGI("xhook_refresh failed");
}

void __1__() {
    LOGI("LIB LOADED SUCCESSFULLY");

    sleep(2);

    PACKAGE_NAME = string(getcmdline());
    LOGI("cmdline: %s", PACKAGE_NAME.c_str());
    
    __IMGUI__();

    sleep(10);
    libmain = get8BPbase();
    LOGI("libmain: %p", libmain);

    __HOOKS__();
    __INPUT__();

    setup_global_segv_handler();
    SetupSignalTraceHandler();
    
    LOGI("RETURNING NOW MY GODDY GOD");
}


#include "kill.h"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad");
    VM = vm;

    CALL(0);
    
    pthread(__1__);

    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL Java_android_service_SurfaceView_onSendConfig(JNIEnv* env, jobject thiz, jstring key, jstring value) {
    const char* k = env->GetStringUTFChars(key, nullptr);
    const char* v = env->GetStringUTFChars(value, nullptr);
    
    bool isOn = (v[0] == '1');
    
    if (strcmp(k, "ESP::LINES") == 0) {
        persistent_bool[O("bESP_DrawPredictionLine")] = isOn;
    } else if (strcmp(k, "ESP::POCKETS") == 0) {
        persistent_bool[O("bESP_DrawPockets")] = isOn;
    } else if (strcmp(k, "ESP::STATES") == 0) {
        persistent_bool[O("bESP_DrawPocketsShotState")] = isOn;
    } else if (strcmp(k, "AUTO::PLAY") == 0) {
        persistent_bool[O("bAutoPlay")] = isOn;
    } else if (strcmp(k, "AUTO::QUEUE") == 0) {
        persistent_bool[O("bAutoQueue")] = isOn;
    }
    
    save_persistence();
    
    env->ReleaseStringUTFChars(key, k);
    env->ReleaseStringUTFChars(value, v);
}

extern "C" JNIEXPORT void JNICALL Java_android_service_SurfaceView_onCanvasDraw(JNIEnv* env, jobject thiz, jobject canvas, jint w, jint h, jfloat d) {
}

extern "C" JNIEXPORT jstring JNICALL Java_android_service_SurfaceView_getExpTime(JNIEnv* env, jobject thiz) {
    return env->NewStringUTF(g_ExpTime.empty() ? "N/A" : g_ExpTime.c_str());
}

extern "C" JNIEXPORT jboolean JNICALL Java_android_service_SurfaceView_MenuColor(JNIEnv* env, jobject thiz) {
    return (jboolean)(logged_in ? JNI_TRUE : JNI_FALSE);
}

