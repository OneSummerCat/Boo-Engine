cmake_minimum_required(VERSION 3.12)  # 设置CMake最低版本要求
project(BooEngine)                          # 项目名称（对应TARGET）

set(CMAKE_CXX_STANDARD 20)            # -std=c++20
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
# ========== 创建源文件分组，保持目录结构 ==========
# 为每个源文件创建对应的VS过滤器（文件夹）
foreach(FILE ${ALL_SOURCES})
    # 获取文件的相对路径
    file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ${FILE})
    # 获取文件所在目录
    get_filename_component(FILE_DIR "${REL_PATH}" DIRECTORY)
    
    # 如果文件在根目录，放到"root"组
    if("${FILE_DIR}" STREQUAL "")
        set(FILE_DIR "root")
    endif()
    
    # 将文件添加到对应的源文件组
    source_group("${FILE_DIR}" FILES ${FILE})
endforeach()


add_executable(
    ${PROJECT_NAME} 
    ${ALL_SOURCES}
    app.cpp
)  


if(WIN32)
    message(STATUS "Configuring for Windows platform")
    set(VULKAN_ROOT "E:/software/vulkan")
    set(GLFW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw")
    set(FREE_TYPE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2")
    

    # 头文件包含（BEFORE 确保优先于系统路径，避免 Mono.framework 的 freetype 干扰）
    target_include_directories(${PROJECT_NAME} BEFORE PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/stb
        ${VULKAN_ROOT}/include
        ${GLFW_ROOT}/include
        ${FREE_TYPE_ROOT}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/fastgltf/include
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/simdjson/include
    )
    # 库链接
    target_link_libraries(${PROJECT_NAME} PRIVATE
        #glfw
        ${GLFW_ROOT}/lib-vc2022/glfw3.lib  #静态库
        #vulkan
        ${VULKAN_ROOT}/Lib/vulkan-1.lib  
        ${VULKAN_ROOT}/Lib/shaderc_combinedd.lib   # DEBUG 版本的 shaderc 库，用于调试和开发。

        #freetype
        ${FREE_TYPE_ROOT}/lib/freetype.lib
    )

     # ========== 资源复制配置（新增部分） ==========
    # 定义资源复制宏
    macro(copy_resources SOURCE_DIR DEST_DIR)
        if(EXISTS "${SOURCE_DIR}")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E rm -rf "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${DEST_DIR}"
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${DEST_DIR}"
                COMMENT "复制 ${SOURCE_DIR} 到 ${DEST_DIR}"
            )
        else()
            message(WARNING "资源目录不存在: ${SOURCE_DIR}")
        endif()
    endmacro()
    
    # 复制 assets 到 assets
    copy_resources("${CMAKE_CURRENT_SOURCE_DIR}/assets" "assets")
   
    
    # 设置VS调试工作目录为输出目录（确保相对路径资源查找正确）
    # 这行代码是设置 Visual Studio 调试器的工作目录。让我用简单的例子解释它的作用：设置后，你可以在 VS 中按 F5 调试程序时，工作目录默认是 .sln 文件所在的目录（通常是项目根目录）
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
    
    # 可选：显示资源复制完成信息
    message(STATUS "资源复制配置完成：")
    message(STATUS "  - assets -> assets")
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
# 内置资源
# file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/resources)
# file(COPY engine/resources/ DESTINATION resources)
# # 项目资源
# file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/assets)
# file(COPY assets/ DESTINATION assets)
# file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/assets)
# file(COPY assets/ DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/assets)
