cmake_minimum_required(VERSION 3.12)  # 设置CMake最低版本要求
project(BooEngine)                          # 项目名称（对应TARGET）

set(CMAKE_CXX_STANDARD 20)            # -std=c++20
set(ENGINE_ROOT engine)

# ========== 自动收集源文件并保持目录结构 ==========
# 递归查找所有源文件
file(GLOB_RECURSE ENGINE_SOURCES 
    "engine/*.cpp"
    "engine/*.h"
    "engine/*.hpp"
)

file(GLOB_RECURSE PROJECT_SOURCES 
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
)
# 合并所有源文件
set(ALL_SOURCES ${ENGINE_SOURCES} ${PROJECT_SOURCES})
# 创建共享库
add_library(BooEngine SHARED
        ${ALL_SOURCES}
        boo-engine-jni-lib.cpp  # Android JNI 入口
)


# FreeType 预编译库路径
set(FREETYPE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/${ANDROID_ABI})
# 导入 FreeType 静态库
add_library(freetype STATIC IMPORTED)
set_target_properties(freetype PROPERTIES
    IMPORTED_LOCATION ${FREETYPE_ROOT}/lib/libfreetype.a
)

# shaderc 预编译库路径
set(SHADERC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/shaderc)
# 导入 shaderc 静态库
add_library(shaderc STATIC IMPORTED)
set_target_properties(shaderc PROPERTIES
    IMPORTED_LOCATION ${SHADERC_ROOT}/libs/c++_static/${ANDROID_ABI}/libshaderc.a
)

# 查找必要的 Android 库
find_library(log-lib log)
find_library(android-lib android)


  # 设置头文件包含路径
target_include_directories(BooEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} # 项目根目录,用于引擎头文件引入
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/stb
    ${FREETYPE_ROOT}/include/freetype2
    ${SHADERC_ROOT}/include
)

# 链接库
target_link_libraries(BooEngine
    ${log-lib}
    ${android-lib}
    freetype            # FreeType 库
    vulkan              # Android Vulkan 支持
    shaderc             # shaderc 库（运行时着色器编译）
)
