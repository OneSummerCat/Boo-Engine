cmake_minimum_required(VERSION 3.12)  # 设置CMake最低版本要求
project(BooEngine)                          # 项目名称（对应TARGET）

set(CMAKE_CXX_STANDARD 17)            # -std=c++17
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 生成compile_commands.json
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC或Clang编译器
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif(MSVC)
    # MSVC编译器
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
endif()
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


add_executable(
    ${PROJECT_NAME} 
    ${ALL_SOURCES}
    app.cpp
)  

if(APPLE)
    # 查找GLFW库
    find_package(glfw3 REQUIRED)
    # 查找Vulkan库
    find_package(Vulkan REQUIRED)

    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(FOUNDATION_LIBRARY Foundation REQUIRED)
    find_library(APPKIT_LIBRARY AppKit REQUIRED)

    # freetype 依赖的系统库
    find_package(ZLIB REQUIRED)
    find_package(BZip2 REQUIRED)
    find_package(PNG REQUIRED)


    # 头文件包含（BEFORE 确保优先于系统路径，避免 Mono.framework 的 freetype 干扰）
    target_include_directories(${PROJECT_NAME} BEFORE PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/include
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/stb
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/nativefiledialog/include
    )
    # 库链接
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        Vulkan::Vulkan
        shaderc_shared

        # freetype 及其依赖库（freetype 必须在依赖库之前）
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/lib/libfreetype.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/nativefiledialog/lib/libnfd.a
        
        ZLIB::ZLIB
        BZip2::BZip2
        PNG::PNG

        ${COCOA_LIBRARY}
        ${FOUNDATION_LIBRARY}
        ${APPKIT_LIBRARY}
       
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
# CMAKE_CURRENT_BINARY_DIR：当前 CMakeLists.txt 对应的构建目录
# CMAKE_BINARY_DIR：整个项目的顶级构建目录
# CMAKE_CURRENT_SOURCE_DIR：当前 CMakeLists.txt 所在的源码目录
# CMAKE_SOURCE_DIR：整个项目的顶级源码目录
# #编辑器
# file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/res)
# file(COPY editor/res/ DESTINATION res)
#内置资源
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY engine/resources/ DESTINATION resources)