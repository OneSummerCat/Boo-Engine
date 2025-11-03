cmake_minimum_required(VERSION 3.12)  # 设置CMake最低版本要求
project(app)                          # 项目名称（对应TARGET）

set(CMAKE_CXX_STANDARD 17)            # -std=c++17
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 生成compile_commands.json
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC或Clang编译器
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif(MSVC)
    # MSVC编译器
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
endif()
# if(WIN32)
#     # MinGW 编译器设置
#     set(CMAKE_C_COMPILER "gcc")
#     set(CMAKE_CXX_COMPILER "g++")
# endif()

set(ENGINE_SOURCES
    editor/editor-config.cpp
    editor/editor-layout.cpp
    editor/editor.cpp
    editor/ipc/editor-ipc.cpp
    engine/core/component/component-factory.cpp
    engine/core/component/component.cpp
    engine/core/game.cpp
    engine/core/gfx/gfx-context.cpp
    engine/core/gfx/gfx-mgr.cpp
    engine/core/gfx/gfx-object.cpp
    engine/core/gfx/gfx-pass.cpp
    engine/core/gfx/gfx-pipeline.cpp
    engine/core/gfx/gfx-queue.cpp
    engine/core/gfx/gfx-renderer.cpp
    engine/core/gfx/gfx-shader.cpp
    engine/core/gfx/gfx-texture.cpp
    engine/core/global/event.cpp
    engine/core/global/global.cpp
    engine/core/math/color.cpp
    engine/core/math/mat4.cpp
    engine/core/math/quat.cpp
    engine/core/math/size.cpp
    engine/core/math/vec2.cpp
    engine/core/math/vec3.cpp
    engine/core/renderer/ui/ui-renderer.cpp
    engine/core/renderer/ui/ui-sprite.cpp
    engine/core/scene/node-2d.cpp
    engine/core/scene/node-3d.cpp
    engine/core/scene/node.cpp
    engine/core/scene/scene.cpp
    engine/core/utils/time-util.cpp
    engine/core/utils/uuid-util.cpp
    engine/engine.cpp
    engine/window/window-mgr.cpp
)

add_executable(
    ${PROJECT_NAME} 
    ${ENGINE_SOURCES}
    app.cpp
)  

if(WIN32)
    message(STATUS "Configuring for Windows platform")
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")

    set(GLFW_ROOT "E:/software/glfw")
    set(MINGW64_ROOT "E:/software/msys64/mingw64")
    set(VULKAN_ROOT "E:/software/vulkan")
    # 头文件包含
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/stb
        ${GLFW_ROOT}/include
        ${MINGW64_ROOT}/include/freetype2
        ${VULKAN_ROOT}/Include
    )
    # 库链接
    target_link_libraries(${PROJECT_NAME} PRIVATE
        #glfw
        ${GLFW_ROOT}/lib-mingw-w64/libglfw3.a  #静态库
        #freetype
        ${MINGW64_ROOT}/lib/libfreetype.a
        ${MINGW64_ROOT}/lib/libpng.a        # freetype依赖
        ${MINGW64_ROOT}/lib/libz.a          # freetype依赖 zlib 压缩库
        ${MINGW64_ROOT}/lib/libharfbuzz.a   # freetype依赖 文字排版引擎
        ${MINGW64_ROOT}/lib/libgraphite2.a  # freetype依赖 Graphite 字体支持
        ${MINGW64_ROOT}/lib/libbrotlidec.a  # freetype依赖 Brotli 压缩库
        ${MINGW64_ROOT}/lib/libbrotlicommon.a  # freetype依赖 Brotli 压缩库
        ${MINGW64_ROOT}/lib/libbz2.a        # freetype依赖 BZip2 压缩库
        ${VULKAN_ROOT}/Lib/vulkan-1.lib     #
        # Windows 系统库 - 新增的部分
        rpcrt4.lib          # 用于 UuidCreate
        dwrite.lib          # 用于 DirectWrite (DWriteCreateFactory)
    )
elseif(APPLE)
    # 查找GLFW库
    find_package(glfw3 REQUIRED)
    # 查找Vulkan库
    find_package(Vulkan REQUIRED)
    # 查找freetype库
    find_package(Freetype REQUIRED)

    # 头文件包含
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/stb
    )
    # 库链接
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        Vulkan::Vulkan
        Freetype::Freetype
    )

else()
    message(FATAL_ERROR "Unsupported platform")
endif()

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/resources)
# 文件拷贝
file(COPY engine/resources/ DESTINATION resources)