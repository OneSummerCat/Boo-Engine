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

set(ENGINE_SOURCES
    editor/assets/editor-assets-layout.cpp
    editor/assets/editor-assets.cpp
    editor/editor-config.cpp
    editor/editor-layout.cpp
    editor/editor.cpp
    editor/hierarchy/editor-hierarchy-layout.cpp
    editor/hierarchy/editor-hierarchy.cpp
    editor/ipc/editor-ipc.cpp
    editor/property/editor-property-layout.cpp
    editor/property/editor-property.cpp
    editor/property/out/editor-property-image-layout.cpp
    editor/property/out/editor-property-node-layout.cpp
    engine/boo.cpp
    engine/core/alpha/alpha.cpp
    engine/core/assets/asset-cache.cpp
    engine/core/assets/asset-load.cpp
    engine/core/assets/asset-task.cpp
    engine/core/assets/asset.cpp
    engine/core/assets/assets-manager.cpp
    engine/core/assets/material.cpp
    engine/core/assets/shader.cpp
    engine/core/assets/texture.cpp
    engine/core/component/component.cpp
    engine/core/component/ui/ui-layout-horizontal.cpp
    engine/core/component/ui/ui-layout-vertical.cpp
    engine/core/component/ui/ui-mask.cpp
    engine/core/component/ui/ui-tree/file-tree.cpp
    engine/core/component/ui/ui-tree/node-tree.cpp
    engine/core/component/ui/ui-tree/ui-tree.cpp
    engine/core/component/ui/ui-widget.cpp
    engine/core/event/event.cpp
    engine/core/font/freetype-mgr.cpp
    engine/core/game.cpp
    engine/core/gfx/gfx-context.cpp
    engine/core/gfx/gfx-mgr.cpp
    engine/core/gfx/gfx-object.cpp
    engine/core/gfx/gfx-pass.cpp
    engine/core/gfx/gfx-pipeline.cpp
    engine/core/gfx/gfx-queue.cpp
    engine/core/gfx/gfx-renderer.cpp
    engine/core/gfx/gfx-shader-compile.cpp
    engine/core/gfx/gfx-shader.cpp
    engine/core/gfx/gfx-texture.cpp
    engine/core/global/global.cpp
    engine/core/input/input.cpp
    engine/core/math/color.cpp
    engine/core/math/mat4.cpp
    engine/core/math/quat.cpp
    engine/core/math/size.cpp
    engine/core/math/vec2.cpp
    engine/core/math/vec3.cpp
    engine/core/renderer/ui/ui-renderer.cpp
    engine/core/renderer/ui/ui-sprite.cpp
    engine/core/renderer/ui/ui-text.cpp
    engine/core/scene/node-2d.cpp
    engine/core/scene/node-3d.cpp
    engine/core/scene/node.cpp
    engine/core/scene/scene.cpp
    engine/core/utils/file-util.cpp
    engine/core/utils/fmt-util.cpp
    engine/core/utils/sdf-util.cpp
    engine/core/utils/time-util.cpp
    engine/core/utils/uuid-util.cpp
    engine/engine.cpp
    engine/window/window.cpp
)

add_executable(
    ${PROJECT_NAME} 
    ${ENGINE_SOURCES}
    app.cpp
)  

if(APPLE)
    # 查找GLFW库
    find_package(glfw3 REQUIRED)
    # 查找Vulkan库
    find_package(Vulkan REQUIRED)
    # # 查找freetype库
    find_package(Freetype REQUIRED)

    # 头文件包含
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/stb
    )
    # 库链接
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        Vulkan::Vulkan
        shaderc_shared
        Freetype::Freetype
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
# CMAKE_CURRENT_BINARY_DIR：当前 CMakeLists.txt 对应的构建目录
# CMAKE_BINARY_DIR：整个项目的顶级构建目录
# CMAKE_CURRENT_SOURCE_DIR：当前 CMakeLists.txt 所在的源码目录
# CMAKE_SOURCE_DIR：整个项目的顶级源码目录
#编辑器
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/res)
file(COPY editor/res/ DESTINATION res)
#内置资源
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY engine/resources/ DESTINATION resources)