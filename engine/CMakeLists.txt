# -----------------------------------------------
# 收集源文件
# -----------------------------------------------
file(GLOB_RECURSE ENGINE_CORE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp"
)
file(GLOB_RECURSE ENGINE_PLATFORM_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/platforms/*.cpp"
)
file(GLOB_RECURSE ENGINE_LIB_FASTGLTF_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/fastgltf/*.cpp"
)
file(GLOB_RECURSE ENGINE_LIB_NLOHMANN_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/nlohmann/*.cpp"
)
file(GLOB_RECURSE ENGINE_LIB_SIMDJSON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/simdjson/*.cpp"
)
file(GLOB_RECURSE ENGINE_LIB_STB_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/stb/*.cpp"
)



# -----------------------------------------------
# 创建静态库
# -----------------------------------------------
add_library(Engine STATIC
    ${ENGINE_CORE_SOURCES}
    ${ENGINE_PLATFORM_SOURCES}
    ${ENGINE_LIB_SIMDJSON_SOURCES}
    ${ENGINE_LIB_FASTGLTF_SOURCES}
    ${ENGINE_LIB_NLOHMANN_SOURCES}
    ${ENGINE_LIB_STB_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/boo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp"
)

# 自定义输出文件名：libBooEngine.a / BooEngine.lib
set_target_properties(Engine PROPERTIES OUTPUT_NAME "BooEngine")
# Debug 版本添加 d 后缀
set_target_properties(Engine PROPERTIES DEBUG_POSTFIX "d")

# -----------------------------------------------
# 引擎自身头文件路径
# -----------------------------------------------
target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms
)

if(WIN32)
    # 收集当前目录的 .cpp 文件（不递归）
    file(GLOB ENGINE_SOURCES_ROOT
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    )
    file(GLOB_RECURSE ENGINE_SOURCES_SUB 
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    )

    set(ENGINE_ALL_SOURCES ${ENGINE_SOURCES_ROOT} ${ENGINE_SOURCES_SUB})
    # 【新增】将所有头文件添加到 target 用于显示
    target_sources(Engine PRIVATE ${ENGINE_ALL_SOURCES})
    # ========== 创建源文件分组，保持目录结构 ==========
    # 为每个源文件创建对应的VS过滤器（文件夹）
    foreach(FILE ${ENGINE_ALL_SOURCES})
        file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${FILE}")
        get_filename_component(FILE_DIR "${REL_PATH}" DIRECTORY)
        
        if(FILE_DIR)
            string(REPLACE "\\" "/" GROUP_NAME "${FILE_DIR}")
            source_group("${GROUP_NAME}" FILES "${FILE}")
        else()
            # 关键：使用空字符串，文件就会出现在项目根目录
            source_group("" FILES "${FILE}")
        endif()
    endforeach()
endif()

# -----------------------------------------------
# Volk 配置
# -----------------------------------------------
if(WIN32)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
    target_compile_definitions(Engine PRIVATE VK_USE_PLATFORM_WIN32_KHR)
elseif(UNIX AND NOT APPLE)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_XLIB_KHR)
elseif(APPLE)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_MACOS_MVK)
endif()
add_subdirectory(libs/volk)
# -----------------------------------------------
# 三方库头文件路径（跨平台统一 include/）
# -----------------------------------------------
target_include_directories(Engine BEFORE PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/fastgltf/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/nlohmann/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/simdjson/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/volk
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/vulkan/include
)

# -----------------------------------------------
# 链接 volk
# -----------------------------------------------
target_link_libraries(Engine PRIVATE
    volk
)

# -----------------------------------------------
# 平台相关链接库
# -----------------------------------------------
if(APPLE)
    target_link_libraries(Engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib/mac/libglfw3.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/lib/mac/libfreetype.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/mac/libglslang.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/mac/libglslang-default-resource-limits.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/mac/libSPIRV.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/mac/libSPIRV-Tools.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/mac/libSPIRV-Tools-opt.a
    )
elseif(WIN32)
    target_link_libraries(Engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib/win/lib-vc2022/glfw3.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/lib/win/freetype.lib
        # glslang
        debug ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/glslangd.lib
        optimized ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/glslang.lib
        debug ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/glslang-default-resource-limitsd.lib
        optimized ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/glslang-default-resource-limits.lib
        debug ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/SPIRVd.lib
        optimized ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/SPIRV.lib
        debug ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/SPIRV-Toolsd.lib
        optimized ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/SPIRV-Tools.lib
        debug ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/SPIRV-Tools-optd.lib
        optimized ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/SPIRV-Tools-opt.lib
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

message(STATUS "Engine sources collected: ${ENGINE_CORE_SOURCES}")
