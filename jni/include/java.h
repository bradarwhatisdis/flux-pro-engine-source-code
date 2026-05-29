#pragma once

#include <jni.h>
#include <string>
#include <cstddef>

#include "includes.h"

#define EXPORT extern "C" __attribute__((visibility("default")))

static JavaVM* VM;

string GetClassName(JNIEnv* env, jclass clazz) {
    jclass classClass = env->FindClass("java/lang/Class");
    if (!classClass) {
        LOGE("Failed to find Class class");
        return "nullptr";
    }
    
    jmethodID getNameMethod = env->GetMethodID(classClass, "getName", "()Ljava/lang/String;");
    if (!getNameMethod) {
        LOGE("Failed to get getName method");
        return "nullptr";
    }
    
    jstring classNameStr = (jstring)env->CallObjectMethod(clazz, getNameMethod);
    if (!classNameStr) {
        LOGE("Failed to get class name");
        return "nullptr";
    }
    
    const char* classNameChars = env->GetStringUTFChars(classNameStr, nullptr);
    string className = classNameChars;
    env->ReleaseStringUTFChars(classNameStr, classNameChars);
    return className;
}

string GetClassName(JNIEnv* env, jobject object) {
    jclass objectClass = env->GetObjectClass(object);
    return GetClassName(env, objectClass);
}

string GetMethodName(JNIEnv* env, jobject method) {
    jclass methodClass = env->FindClass("java/lang/reflect/Method");
    if (!methodClass) {
        LOGE("Failed to find Method class");
        return "nullptr";
    }
    
    jmethodID getNameMethod = env->GetMethodID(methodClass, "getName", "()Ljava/lang/String;");
    if (!getNameMethod) {
        LOGE("Failed to get getName method");
        return "nullptr";
    }
    
    jstring methodNameStr = (jstring)env->CallObjectMethod(method, getNameMethod);
    if (!methodNameStr) {
        LOGE("Failed to get method name");
        return "nullptr";
    }
    
    const char* methodNameChars = env->GetStringUTFChars(methodNameStr, nullptr);
    string methodName = methodNameChars;
    env->ReleaseStringUTFChars(methodNameStr, methodNameChars);
    return methodName;
}

jclass FindClass(JNIEnv* env, const char* name) {
    jclass activityThreadClass = env->FindClass("android/app/ActivityThread");
    if (!activityThreadClass) {
        LOGE("Failed to find ActivityThread class");
        return nullptr;
    }
    
    jmethodID currentActivityThreadMethod = env->GetStaticMethodID(activityThreadClass, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject activityThread = env->CallStaticObjectMethod(activityThreadClass, currentActivityThreadMethod);
    
    while (!activityThread) {
        sleepm(100);
        activityThread = env->CallStaticObjectMethod(activityThreadClass, currentActivityThreadMethod);
    }
    
    jmethodID getApplicationMethod = env->GetMethodID(activityThreadClass, "getApplication", "()Landroid/app/Application;");
    jobject application = env->CallObjectMethod(activityThread, getApplicationMethod);
    
    if (!application) {
        LOGE("Failed to get application context");
        return nullptr;
    }
    
    jmethodID getClassLoaderMethod = env->GetMethodID(env->FindClass("android/content/Context"), "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject classLoader = env->CallObjectMethod(application, getClassLoaderMethod);
    
    jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
    jmethodID loadClassMethod = env->GetMethodID(classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    
    jstring className = env->NewStringUTF(name);
    jclass clazz = (jclass)env->CallObjectMethod(classLoader, loadClassMethod, className);
    env->DeleteLocalRef(className);
    
    if (!clazz) {
        LOGE("Failed to find class!");
        return nullptr;
    }

    return clazz;
}

void __ORIENTATION__() {
    LOGI("Orientation monitor thread started");
    
    if (!VM) {
        LOGE("VM is null");
        return;
    }

    JNIEnv* env;
    jint getEnvResult = VM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (getEnvResult == JNI_EDETACHED) {
        if (VM->AttachCurrentThread(&env, nullptr) != 0) {
            LOGE("Failed to attach thread to JVM");
            return;
        }
    } else if (getEnvResult != JNI_OK) {
        LOGE("Failed to get JNIEnv");
        return;
    }

    auto activityThreadClass = env->FindClass("android/app/ActivityThread");
    auto sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, "sCurrentActivityThread", "Landroid/app/ActivityThread;");
    auto sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
    auto mInitialApplicationField = env->GetFieldID(activityThreadClass, "mInitialApplication", "Landroid/app/Application;");
    auto mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);

    auto contextClass = env->FindClass("android/content/Context");
    auto getSystemServiceMethod = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    auto windowServiceStr = env->NewStringUTF("window");
    auto windowManager = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, windowServiceStr);

    auto windowManagerClass = env->FindClass("android/view/WindowManager");
    auto getDefaultDisplayMethod = env->GetMethodID(windowManagerClass, "getDefaultDisplay", "()Landroid/view/Display;");
    auto display = env->CallObjectMethod(windowManager, getDefaultDisplayMethod);

    auto displayClass = env->FindClass("android/view/Display");
    auto getRotationMethod = env->GetMethodID(displayClass, "getRotation", "()I");

    LOGI("Orientation monitor thread working");

    while (true) {
        Orientation = env->CallIntMethod(display, getRotationMethod);
        sleep(1);
    }
}


std::string getClipboard(JNIEnv *env = nullptr) {
    if (!VM) return "";

    if (!env) {
        JNIEnv* env;
        jint getEnvResult = VM->GetEnv((void**)&env, JNI_VERSION_1_6);
        if (getEnvResult == JNI_EDETACHED) {
            if (VM->AttachCurrentThread(&env, nullptr) != 0) {
                LOGE("Failed to attach thread to JVM");
                return "";
            }
        } else if (getEnvResult != JNI_OK) {
            LOGE("Failed to get JNIEnv");
            return "";
        }
    }

    auto activityThreadClass = env->FindClass("android/app/ActivityThread");
    auto sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, "sCurrentActivityThread", "Landroid/app/ActivityThread;");
    auto sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);

    auto mInitialApplicationField = env->GetFieldID(activityThreadClass, "mInitialApplication", "Landroid/app/Application;");
    auto mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);

    auto contextClass = env->FindClass("android/content/Context");
    auto getSystemServiceMethod = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

    auto str = env->NewStringUTF("clipboard");
    auto clipboardManager = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, str);
    env->DeleteLocalRef(str);

    auto ClipboardManagerClass = env->FindClass("android/content/ClipboardManager");
    auto getText = env->GetMethodID(ClipboardManagerClass, "getText", "()Ljava/lang/CharSequence;");

    auto CharSequenceClass = env->FindClass("java/lang/CharSequence");
    auto toStringMethod = env->GetMethodID(CharSequenceClass, "toString", "()Ljava/lang/String;");

    auto text = env->CallObjectMethod(clipboardManager, getText);
    std::string result;
    if (text) {
        str = (jstring) env->CallObjectMethod(text, toStringMethod);
        result = env->GetStringUTFChars(str, 0);
        env->DeleteLocalRef(str);
        env->DeleteLocalRef(text);
    }

    env->DeleteLocalRef(CharSequenceClass);
    env->DeleteLocalRef(ClipboardManagerClass);
    env->DeleteLocalRef(clipboardManager);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
    
    LOGI("GetClipboard: %s", result.c_str());
    return result;
}

std::string getAndroidID(JNIEnv *env = nullptr) {
    if (!VM) return "";

    if (!env) {
        JNIEnv* env;
        jint getEnvResult = VM->GetEnv((void**)&env, JNI_VERSION_1_6);
        if (getEnvResult == JNI_EDETACHED) {
            if (VM->AttachCurrentThread(&env, nullptr) != 0) {
                LOGE("Failed to attach thread to JVM");
                return "";
            }
        } else if (getEnvResult != JNI_OK) {
            LOGE("Failed to get JNIEnv");
            return "";
        }
    }

    auto activityThreadClass = env->FindClass("android/app/ActivityThread");
    auto sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, "sCurrentActivityThread", "Landroid/app/ActivityThread;");
    auto sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
    auto mInitialApplicationField = env->GetFieldID(activityThreadClass, "mInitialApplication", "Landroid/app/Application;");
    auto mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);

    // Get ContentResolver
    auto contextClass = env->FindClass("android/content/Context");
    auto getContentResolverMethod = env->GetMethodID(contextClass, "getContentResolver", "()Landroid/content/ContentResolver;");
    auto contentResolver = env->CallObjectMethod(mInitialApplication, getContentResolverMethod);

    // Settings.Secure.ANDROID_ID
    auto settingsSecureClass = env->FindClass("android/provider/Settings$Secure");
    auto androidIdField = env->GetStaticFieldID(settingsSecureClass, "ANDROID_ID", "Ljava/lang/String;");
    auto androidIdStr = (jstring)env->GetStaticObjectField(settingsSecureClass, androidIdField);

    // Settings.Secure.getString(ContentResolver, ANDROID_ID)
    auto getStringMethod = env->GetStaticMethodID(settingsSecureClass, "getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
    auto idJStr = (jstring)env->CallStaticObjectMethod(settingsSecureClass, getStringMethod, contentResolver, androidIdStr);

    std::string result;

    if (idJStr) {
        const char* idChars = env->GetStringUTFChars(idJStr, 0);
        result = idChars;
        env->ReleaseStringUTFChars(idJStr, idChars);
        env->DeleteLocalRef(idJStr);
    }

    if (settingsSecureClass) env->DeleteLocalRef(settingsSecureClass);
    if (androidIdStr) env->DeleteLocalRef(androidIdStr);
    if (contentResolver) env->DeleteLocalRef(contentResolver);
    if (contextClass) env->DeleteLocalRef(contextClass);
    if (mInitialApplication) env->DeleteLocalRef(mInitialApplication);
    if (activityThreadClass) env->DeleteLocalRef(activityThreadClass);

    LOGI("GetAndroidID: %s", result.c_str());
    return result;
}


JNIEnv* GetJNIEnv() {
    JNIEnv* env;
    jint getEnvResult = VM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (getEnvResult == JNI_EDETACHED) {
        if (VM->AttachCurrentThread(&env, nullptr) != 0) {
            LOGE("Failed to attach thread to JVM");
            return nullptr;
        }
    } else if (getEnvResult != JNI_OK) {
        LOGE("Failed to get JNIEnv");
        return nullptr;
    } return env;
}