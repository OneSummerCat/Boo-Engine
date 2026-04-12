cmake_minimum_required(VERSION 3.15)
project(BooEnginePackage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 是否将三方库头文件一并打包到 dist/include/libs（默认开启）
option(BOO_INSTALL_THIRDPARTY_HEADERS "Install third-party headers into dist/include/libs" ON)

# 引入 engine 编译
add_subdirectory(engine)

# -----------------------------------------------
# 安装规则：cmake --install build --prefix ./dist
# -----------------------------------------------
set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/engine)

# 1. 静态库
install(TARGETS Engine
    ARCHIVE DESTINATION libs
)

# 2. 引擎自身头文件 -> include/engine/
install(FILES
    ${ENGINE_DIR}/boo.h
    ${ENGINE_DIR}/engine.h
    ${ENGINE_DIR}/log.h
    DESTINATION include/engine
)
install(DIRECTORY ${ENGINE_DIR}/core/
    DESTINATION include/engine/core
    FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY ${ENGINE_DIR}/platforms/
    DESTINATION include/engine/platforms
    FILES_MATCHING PATTERN "*.h"
)

# 3. 三方库头文件 -> include/libs/（开关控制）
if(BOO_INSTALL_THIRDPARTY_HEADERS)
    # fastgltf 头文件
    install(DIRECTORY ${ENGINE_DIR}/libs/fastgltf/include/
        DESTINATION include/libs/fastgltf
        # FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
    )
    # freetype2 头文件
    install(DIRECTORY ${ENGINE_DIR}/libs/freetype2/include/
        DESTINATION include/libs/freetype2
    )
    # glfw 头文件
    install(DIRECTORY ${ENGINE_DIR}/libs/glfw/include/
        DESTINATION include/libs/glfw
    )
    # glslang 头文件
    install(DIRECTORY ${ENGINE_DIR}/libs/glslang/include/
        DESTINATION include/libs/glslang
    )
    # nlohmann 头文件
    install(FILES
        ${ENGINE_DIR}/libs/nlohmann/json.hpp
        ${ENGINE_DIR}/libs/nlohmann/fifo_map.hpp
        DESTINATION include/libs/nlohmann
    )
    # simdjson 头文件
    install(DIRECTORY ${ENGINE_DIR}/libs/simdjson/include/
        DESTINATION include/libs/simdjson
        # FILES_MATCHING PATTERN "*.h"
    )
    # stb 头文件
    install(DIRECTORY ${ENGINE_DIR}/libs/stb/
        DESTINATION include/libs/stb
        # FILES_MATCHING PATTERN "*.h"
    )
    # volk 头文件
    install(FILES
        ${ENGINE_DIR}/libs/volk/volk.h
        DESTINATION include/libs/volk
    )
    # vulkan 头文件
    install(DIRECTORY ${ENGINE_DIR}/libs/vulkan/
        DESTINATION include/libs/vulkan
        # FILES_MATCHING PATTERN "*.h"
    )
endif()

# 4. 预编译二进制三方库 —— 按平台复制
if(APPLE)
    # glfw
    install(DIRECTORY ${ENGINE_DIR}/libs/glfw/lib/mac/
        DESTINATION libs/glfw
    )
    install(DIRECTORY ${ENGINE_DIR}/libs/freetype2/lib/mac/
        DESTINATION libs/freetype2
    )
    # glslang + SPIRV-Tools
    install(DIRECTORY ${ENGINE_DIR}/libs/glslang/lib/mac/
        DESTINATION libs/glslang
    )
elseif(WIN32)
    # glfw
    install(DIRECTORY ${ENGINE_DIR}/libs/glfw/lib/win/
        DESTINATION libs/glfw
    )
    install(DIRECTORY ${ENGINE_DIR}/libs/freetype2/lib/win/
        DESTINATION libs/freetype2
    )
    # glslang + SPIRV-Tools
    install(DIRECTORY ${ENGINE_DIR}/libs/glslang/lib/win/
        DESTINATION libs/glslang
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# 5. 安装示例 CMakeLists.txt 到 dist 根目录
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeLists.txt.in
    DESTINATION .
    RENAME CMakeLists.txt
)

