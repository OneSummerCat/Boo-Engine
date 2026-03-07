#pragma once
#include <iostream>
#include <cstdio>
#include <ctime>
// Windows 平台
#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__) || defined(__WIN32__)
    #define BOO_PLATFORM_WINDOWS 1
    // 区分 32 位和 64 位
    #if defined(_WIN64)
        #define BOO_PLATFORM_WINDOWS_64 1  //暂时不区分
    #else
        #define BOO_PLATFORM_WINDOWS_32 1  //暂时不区分
    #endif

    // UWP (Universal Windows Platform)
    #if defined(WINAPI_FAMILY)
        #include <winapifamily.h>
        #if WINAPI_FAMILY == WINAPI_FAMILY_APP
            #define BOO_PLATFORM_UWP 1  //暂时不区分
        #endif
    #endif

// 苹果平台系列
#elif defined(__APPLE__) && defined(__MACH__)
    #define BOO_PLATFORM_APPLE 1
    #include <TargetConditionals.h>
    // macOS
    #if TARGET_OS_MAC && !TARGET_OS_IPHONE
        #define BOO_PLATFORM_MACOS 1 // macOS
    // iOS (包括真机和模拟器)
    #elif TARGET_OS_IPHONE
        #define BOO_PLATFORM_IOS 1
        // iOS 模拟器
        #if TARGET_IPHONE_SIMULATOR || TARGET_OS_SIMULATOR
            #define BOO_PLATFORM_IOS_SIMULATOR 1 // iOS 模拟器
        // iOS 真机
        #else
            #define BOO_PLATFORM_IOS_DEVICE 1 // iOS 真机
        #endif
    // tvOS
    #elif TARGET_OS_TV
        #define BOO_PLATFORM_TVOS 1 // 
        #if TARGET_OS_SIMULATOR
            #define BOO_PLATFORM_TVOS_SIMULATOR 1
        #else
            #define BOO_PLATFORM_TVOS_DEVICE 1
        #endif

    // watchOS
    #elif TARGET_OS_WATCH
        #define BOO_PLATFORM_WATCHOS 1
        #if TARGET_OS_SIMULATOR
            #define BOO_PLATFORM_WATCHOS_SIMULATOR 1
        #else
            #define BOO_PLATFORM_WATCHOS_DEVICE 1
        #endif
    #endif

// Android 平台
#elif defined(__ANDROID__)
    #define BOO_PLATFORM_ANDROID 1
    // Android API Level
    #include <android/api-level.h>
    #define BOO_ANDROID_API_LEVEL __ANDROID_API__
// 鸿蒙平台 (HarmonyOS)
#elif defined(__OHOS__) || defined(__HARMONY__) || defined(__HARMONYOS__)
    #define BOO_PLATFORM_HARMONYOS 1
    // 鸿蒙也基于 Linux 内核，需要在 Linux 之前检测
    // 具体宏定义可能因 SDK 版本而异
// Linux 平台
#elif defined(__linux__) || defined(__unix__) || defined(__unix)
    #define BOO_PLATFORM_LINUX 1
    // 区分不同的 Linux 发行版或嵌入式系统
    #if defined(__gnu_linux__)
        #define BOO_PLATFORM_GNU_LINUX 1
    #endif
// FreeBSD
#elif defined(__FreeBSD__)
    #define BOO_PLATFORM_FREEBSD 1
// 其他 Unix-like 系统
#elif defined(__unix__) || defined(__unix)
    #define BOO_PLATFORM_UNIX 1

#else
    #define BOO_PLATFORM_UNKNOWN 1
    #warning "Unknown platform"
#endif