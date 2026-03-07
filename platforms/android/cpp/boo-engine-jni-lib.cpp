#include <jni.h>
#include <string>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_android.h>

#include "engine/boo.h"
#include "engine/engine.h"
#include "engine/log.h"
#include "engine/platforms/android/android.h"
#include "src/launch.h"

static ANativeWindow* g_nativeWindow = nullptr;
/**
 *  引擎实例
 */
static Engine *g_engine = nullptr;
static Android *g_android = nullptr;
static Launch *g_launch = nullptr;
static JavaVM *g_jvm = nullptr;

// ============================================
// JNI_OnLoad - 库加载时调用
// ============================================
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGI("JNI:library loaded");
    g_jvm = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        LOGE("JNI:Failed to get JNI environment");
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

// ============================================
// JNI_OnUnload - 库卸载时调用
// ============================================
JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved)
{
    LOGI("JNI:library unloading...");

    // 清理全局资源
    if (g_engine != nullptr)
    {
        delete g_engine;
        g_engine = nullptr;
    }
    //     if (g_nativeWindow != nullptr) {
    //         ANativeWindow_release(g_nativeWindow);
    //         g_nativeWindow = nullptr;
    //     }
    g_jvm = nullptr;
}
// 引擎接口=========================================
/**
 * 初始化
 */
/**
 * 对应 Java 中的：public static native long nativeInit(String config);
 * 包名：com.example.BooEngineSdk
 *
 * 函数名格式：Java_包名_类名_方法名
 * 注意：包名中的点(.)要替换成下划线(_)
 */
extern "C" JNIEXPORT jboolean JNICALL
Java_com_booengine_sdk_BooEngineSdk_EngineInit(
    JNIEnv *env,
    jclass clazz,
    jobject surface, jint width, jint height
    )
{
    try
    {
        g_nativeWindow = ANativeWindow_fromSurface(env, surface);
        // 创建引擎实例
        g_engine = new Engine();
        // 创建android 实例
        g_android = new Android(g_nativeWindow,width,height);
        g_engine->init(g_android);
        g_launch=new Launch();
        g_launch->init();
        LOGI("JNI:initialized successfully");
        return JNI_TRUE;
    }
    catch (const std::exception &e)
    {
        LOGE("JNI:initialized Exception %s", e.what());
        return JNI_FALSE;
    }
}
extern "C" JNIEXPORT void JNICALL
Java_com_booengine_sdk_BooEngineSdk_EngineResize(
     JNIEnv* env,
     jclass clazz,
     jint width,
     jint height) {
     LOGI("JNI:resize %d x %d", width, height);
     if (g_android != nullptr) {
         // TODO: 通知引擎窗口大小改变
         g_android->resize(width,height);
     }
}
extern "C" JNIEXPORT void JNICALL
Java_com_booengine_sdk_BooEngineSdk_EngineTick(
        JNIEnv* env,
        jclass clazz) {
    if (g_android != nullptr) {
        // TODO: 通知引擎窗口大小改变
        g_android->tick();
    }
    if(g_engine!= nullptr){
        g_engine->tick();
    }
}
extern "C" JNIEXPORT void JNICALL
Java_com_booengine_sdk_BooEngineSdk_EngineDestory(
        JNIEnv* env,
        jclass clazz) {
    LOGI("JNI:destory");
    delete g_android;
    g_android= nullptr;
    delete g_engine;
    g_engine= nullptr;
}



// ============================================
// 初始化引擎
// ============================================
// extern "C" JNIEXPORT jboolean JNICALL
// Java_com_booengine_sdk_BooEngineSdk_EngineIni(
//     JNIEnv* env,
//     jobject thiz,
//     jobject surface) {
//
//     LOGI("nativeInit called");
//
//     try {
////         // 如果已经初始化，先清理
////         if (g_engine != nullptr) {
////             LOGW("Engine already initialized, cleaning up...");
////             delete g_engine;
////             g_engine = nullptr;
////         }
////
////         // 获取 ANativeWindow
////         if (surface != nullptr) {
////             if (g_nativeWindow != nullptr) {
////                 ANativeWindow_release(g_nativeWindow);
////             }
////             g_nativeWindow = ANativeWindow_fromSurface(env, surface);
////
////             if (g_nativeWindow == nullptr) {
////                 LOGE("Failed to get ANativeWindow from Surface");
////                 return JNI_FALSE;
////             }
////
////             LOGI("ANativeWindow obtained: %dx%d",
////                  ANativeWindow_getWidth(g_nativeWindow),
////                  ANativeWindow_getHeight(g_nativeWindow));
////         }
//
//         // 创建引擎实例
//         g_engine = new Engine();
//
//         // TODO: 初始化引擎（需要根据您的引擎接口调整）
//         // g_engine->init(g_nativeWindow);
//
//         LOGI("BooEngine initialized successfully");
//         return JNI_TRUE;
//
//     } catch (const std::exception& e) {
//         LOGE("Exception during initialization: %s", e.what());
//         return JNI_FALSE;
//     }
//}

// // ============================================
// // 测试方法 - 返回字符串
// // ============================================
// extern "C" JNIEXPORT jstring JNICALL
// Java_com_booengine_sdk_MainActivity_stringFromJNI(
//     JNIEnv* env,
//     jobject /* this */) {

//     std::string hello = "Hello from BooEngine C++ " + std::string(__DATE__) + " " + std::string(__TIME__);
//     LOGI("stringFromJNI called: %s", hello.c_str());
//     return env->NewStringUTF(hello.c_str());
// }

// // ============================================
// // 窗口尺寸改变
// // ============================================
// extern "C" JNIEXPORT void JNICALL
// Java_com_booengine_sdk_MainActivity_nativeResize(
//     JNIEnv* env,
//     jobject thiz,
//     jint width,
//     jint height) {

//     LOGI("nativeResize: %d x %d", width, height);

//     if (g_engine != nullptr) {
//         // TODO: 通知引擎窗口大小改变
//         // g_engine->resize(width, height);
//     }
// }

// // ============================================
// // 渲染一帧
// // ============================================
// extern "C" JNIEXPORT void JNICALL
// Java_com_booengine_sdk_MainActivity_nativeDrawFrame(
//     JNIEnv* env,
//     jobject thiz) {

//     if (g_engine != nullptr) {
//         // TODO: 调用引擎的更新和渲染
//         // g_engine->update();
//         // g_engine->render();
//     }
// }

// // ============================================
// // 暂停引擎
// // ============================================
// extern "C" JNIEXPORT void JNICALL
// Java_com_booengine_sdk_MainActivity_nativePause(
//     JNIEnv* env,
//     jobject thiz) {

//     LOGI("nativePause called");

//     if (g_engine != nullptr) {
//         // TODO: 暂停引擎
//         // g_engine->pause();
//     }
// }

// // ============================================
// // 恢复引擎
// // ============================================
// extern "C" JNIEXPORT void JNICALL
// Java_com_booengine_sdk_MainActivity_nativeResume(
//     JNIEnv* env,
//     jobject thiz) {

//     LOGI("nativeResume called");

//     if (g_engine != nullptr) {
//         // TODO: 恢复引擎
//         // g_engine->resume();
//     }
// }

// // ============================================
// // 处理触摸事件
// // ============================================
// extern "C" JNIEXPORT void JNICALL
// Java_com_booengine_sdk_MainActivity_nativeTouchEvent(
//     JNIEnv* env,
//     jobject thiz,
//     jint action,
//     jfloat x,
//     jfloat y) {

//     if (g_engine != nullptr) {
//         // TODO: 处理触摸事件
//         // g_engine->handleTouch(action, x, y);

//         // 简单日志（可以注释掉以减少日志量）
//         // LOGD("Touch: action=%d, pos=(%.2f, %.2f)", action, x, y);
//     }
// }

// // ============================================
// // 清理引擎
// // ============================================
// extern "C" JNIEXPORT void JNICALL
// Java_com_booengine_sdk_MainActivity_nativeDestroy(
//     JNIEnv* env,
//     jobject thiz) {

//     LOGI("nativeDestroy called");

//     if (g_engine != nullptr) {
//         // TODO: 清理引擎资源
//         // g_engine->cleanup();
//         delete g_engine;
//         g_engine = nullptr;
//     }

//     if (g_nativeWindow != nullptr) {
//         ANativeWindow_release(g_nativeWindow);
//         g_nativeWindow = nullptr;
//     }

//     LOGI("BooEngine destroyed");
// }

// // ============================================
// // 加载资源（示例）
// // ============================================
// extern "C" JNIEXPORT jboolean JNICALL
// Java_com_booengine_sdk_MainActivity_nativeLoadAsset(
//     JNIEnv* env,
//     jobject thiz,
//     jstring assetPath) {

//     const char* path = env->GetStringUTFChars(assetPath, nullptr);
//     LOGI("Loading asset: %s", path);

//     jboolean result = JNI_FALSE;

//     if (g_engine != nullptr) {
//         // TODO: 加载资源
//         // result = g_engine->loadAsset(path) ? JNI_TRUE : JNI_FALSE;
//     }

//     env->ReleaseStringUTFChars(assetPath, path);
//     return result;
// }

// // ============================================
// // 获取引擎状态（示例）
// // ============================================
// extern "C" JNIEXPORT jstring JNICALL
// Java_com_booengine_sdk_MainActivity_nativeGetStatus(
//     JNIEnv* env,
//     jobject thiz) {

//     if (g_engine == nullptr) {
//         return env->NewStringUTF("Engine not initialized");
//     }

//     // TODO: 返回引擎状态信息
//     std::string status = "Engine running";
//     // status += "\nFPS: " + std::to_string(g_engine->getFPS());
//     // status += "\nObjects: " + std::to_string(g_engine->getObjectCount());

//     return env->NewStringUTF(status.c_str());
// }
