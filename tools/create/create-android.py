#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# create-android.py - Android 平台项目创建脚本

import os
import sys
import shutil
from pathlib import Path

def get_engine_root():
    """获取引擎根目录（Boo-Engine）"""
    script_path = Path(__file__).resolve()
    engine_root = script_path.parent.parent.parent
    return engine_root

def find_android_manifest_dir(project_path):
    """查找 AndroidManifest.xml 所在的目录"""
    # 常见的 AndroidManifest.xml 位置
    possible_paths = [
        project_path / "app" / "src" / "main",
        project_path / "src" / "main",
    ]

    for path in possible_paths:
        if (path / "AndroidManifest.xml").exists():
            return path

    return None

def copy_platform_directories(engine_root, project_path):
    """第一步：将 platforms/android 下的 assets, cpp, java 三个目录拷贝到 Android 工程对应目录"""
    print("\n" + "="*50)
    print("第一步：复制引擎目录（assets, cpp, java）")
    print("="*50)

    android_platform_dir = engine_root / "platforms" / "android"

    if not android_platform_dir.exists():
        print(f"❌ 错误: 找不到 Android 平台目录: {android_platform_dir}")
        return False

    # 查找 AndroidManifest.xml 所在目录
    manifest_dir = find_android_manifest_dir(project_path)
    if not manifest_dir:
        print(f"❌ 错误: 找不到 AndroidManifest.xml 文件")
        print("提示: 请确保这是一个有效的 Android 工程")
        return False

    print(f"✅ 找到 AndroidManifest.xml 所在目录: {manifest_dir}")

    # 要复制的目录列表
    directories_to_copy = ['assets', 'cpp', 'java']

    try:
        for dir_name in directories_to_copy:
            src_dir = android_platform_dir / dir_name
            dst_dir = manifest_dir / dir_name

            if not src_dir.exists():
                print(f"⚠️  警告: 找不到源目录 {dir_name}，跳过")
                continue

            print(f"\n📁 复制目录: {dir_name}")
            print(f"   源: {src_dir}")
            print(f"   目标: {dst_dir}")

            # 复制目录（如果目标已存在，会合并）
            shutil.copytree(src_dir, dst_dir, dirs_exist_ok=True)
            print(f"   ✓ {dir_name} 复制完成")

        print(f"\n✅ 引擎目录复制完成")
        return True
    except Exception as e:
        print(f"❌ 复制目录失败: {e}")
        import traceback
        traceback.print_exc()
        return False

def copy_engine_and_ai_directory(engine_root, manifest_dir):
    """第二步：将引擎和 ai 目录复制到 cpp 目录下"""
    print("\n" + "="*50)
    print("第二步：复制引擎 engine 目录和 ai 目录")
    print("="*50)

    cpp_dir = manifest_dir / "cpp"

    # 复制 engine 目录
    engine_src = engine_root / "engine"
    engine_dst = cpp_dir / "engine"

    if not engine_src.exists():
        print(f"❌ 错误: 找不到引擎目录: {engine_src}")
        return False

    try:
        print(f"📦 复制 engine:")
        print(f"   源目录: {engine_src}")
        print(f"   目标目录: {engine_dst}")
        shutil.copytree(engine_src, engine_dst, dirs_exist_ok=True)
        print(f"✅ 引擎目录复制完成")
    except Exception as e:
        print(f"❌ 复制引擎目录失败: {e}")
        import traceback
        traceback.print_exc()
        return False

    # 复制 ai 目录
    ai_src = engine_root / "ai"
    ai_dst = cpp_dir / "ai"

    if ai_src.exists():
        try:
            print(f"\n📦 复制 ai:")
            print(f"   源目录: {ai_src}")
            print(f"   目标目录: {ai_dst}")
            shutil.copytree(ai_src, ai_dst, dirs_exist_ok=True)
            print(f"✅ ai 目录复制完成")
        except Exception as e:
            print(f"⚠️  警告: 复制 ai 目录失败: {e}")
    else:
        print(f"⚠️  警告: ai 目录不存在，跳过")

    return True

def find_block_end(content, start_pos):
    """查找代码块的结束位置（匹配大括号）"""
    depth = 0
    i = start_pos
    while i < len(content):
        if content[i] == '{':
            depth += 1
        elif content[i] == '}':
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1

def find_cmake_file(project_path):
    """查找 CMakeLists.txt 文件的位置"""
    # 可能的 CMakeLists.txt 位置
    possible_paths = [
        project_path / "app" / "src" / "main" / "cpp" / "CMakeLists.txt",
        project_path / "src" / "main" / "cpp" / "CMakeLists.txt",
    ]

    for path in possible_paths:
        if path.exists():
            return path

    # 如果没找到，尝试递归搜索
    for cmake_file in project_path.rglob("**/cpp/CMakeLists.txt"):
        return cmake_file

    return None

def get_relative_path(from_file, to_file):
    """计算从一个文件到另一个文件的相对路径"""
    # 获取 from_file 的目录
    from_dir = from_file.parent
    # 计算相对路径
    try:
        rel_path = to_file.relative_to(from_dir)
        # 转换为 Unix 风格路径（Gradle 使用）
        return str(rel_path).replace('\\', '/')
    except ValueError:
        # 如果不在同一个路径下，返回绝对路径
        return str(to_file).replace('\\', '/')

def merge_gradle_config(engine_root, project_path):
    """第三步：自动合并 build.gradle 配置"""
    print("\n" + "="*50)
    print("第三步：合并 build.gradle 配置")
    print("="*50)

    # 引擎的 build.gradle 配置文件
    engine_gradle_file = engine_root / "platforms" / "android" / "build.gradle"

    # 用户的 build.gradle 文件（可能在 app/build.gradle 或其他位置）
    possible_gradle_paths = [
        project_path / "app" / "build.gradle",
        project_path / "build.gradle",
    ]

    user_gradle_file = None
    for path in possible_gradle_paths:
        if path.exists():
            user_gradle_file = path
            break

    if not user_gradle_file:
        print(f"⚠️  警告: 找不到用户的 build.gradle 文件")
        print(f"提示: 请手动添加引擎配置到你的 build.gradle 文件")
        return True  # 不是致命错误

    if not engine_gradle_file.exists():
        print(f"⚠️  警告: 找不到引擎的 build.gradle 配置文件")
        return True

    # 查找 CMakeLists.txt 文件
    cmake_file = find_cmake_file(project_path)
    if not cmake_file:
        print(f"⚠️  警告: 找不到 CMakeLists.txt 文件")
        print(f"提示: 请确保已正确复制 cpp 目录")
        return True

    # 计算相对路径
    cmake_relative_path = get_relative_path(user_gradle_file, cmake_file)
    print(f"✅ 找到 CMakeLists.txt: {cmake_file}")
    print(f"   相对路径: {cmake_relative_path}")

    try:
        print(f"📦 用户 build.gradle: {user_gradle_file}")
        print(f"📦 引擎 build.gradle: {engine_gradle_file}")

        # 读取文件内容
        user_content = user_gradle_file.read_text(encoding='utf-8')
        engine_content = engine_gradle_file.read_text(encoding='utf-8')

        # 检查是否已经包含引擎配置
        if 'externalNativeBuild' in user_content and 'cmake' in user_content:
            print(f"✅ build.gradle 中已包含 CMake 配置，跳过合并")
            return True

        # 查找 android { } 块的位置
        android_block_start = user_content.find('android {')

        if android_block_start == -1:
            print(f"❌ 错误: 找不到 android {{ }} 配置块")
            print(f"提示: 请确保这是一个有效的 Android 模块的 build.gradle 文件")
            return False

        # 创建备份
        backup_file = user_gradle_file.with_suffix('.gradle.backup')
        backup_file.write_text(user_content, encoding='utf-8')
        print(f"✅ 已创建备份: {backup_file}")

        # 查找 android 块的开始和结束位置
        android_start = user_content.find('{', android_block_start)
        android_end = find_block_end(user_content, android_start)

        if android_end == -1:
            print(f"❌ 错误: 无法找到 android {{ }} 块的结束位置")
            return False

        android_block_content = user_content[android_start:android_end+1]

        # 准备要插入的配置
        ndk_version_config = """    ndkVersion "23.2.8568313"  // 使用已安装的 NDK 版本
"""

        ndk_config = """        // NDK 配置
        ndk {
            abiFilters 'arm64-v8a', 'armeabi-v7a'  // 支持的 CPU 架构
        }"""

        cmake_config_in_defaultConfig = """        // CMake 配置
        externalNativeBuild {
            cmake {
                cppFlags "-std=c++20", "-frtti", "-fexceptions"
                arguments "-DANDROID_STL=c++_shared",
                          "-DANDROID_PLATFORM=android-28"
            }
        }"""

        # 使用动态计算的相对路径
        external_native_build = f"""    // CMake 外部构建配置
    externalNativeBuild {{
        cmake {{
            path file('{cmake_relative_path}')
            version '3.22.1'
        }}
    }}"""

        # 首先在 android 块开始处添加 ndkVersion（如果不存在）
        new_content = user_content

        if 'ndkVersion' not in android_block_content:
            # 在 android { 之后插入 ndkVersion
            insertion_point = android_start + 1
            new_content = (
                new_content[:insertion_point] +
                '\n' + ndk_version_config +
                new_content[insertion_point:]
            )
            print(f"  ✓ 已添加 ndkVersion 配置")
        else:
            print(f"  ✓ ndkVersion 已存在，跳过")

        # 重新查找 android 块（因为内容已改变）
        android_block_start = new_content.find('android {')
        android_start = new_content.find('{', android_block_start)
        android_end = find_block_end(new_content, android_start)
        android_block_content = new_content[android_start:android_end+1]

        # 查找 defaultConfig 块
        defaultConfig_pos = android_block_content.find('defaultConfig {')

        if defaultConfig_pos != -1:
            # 找到 defaultConfig，在其中插入配置
            print(f"  ✓ 找到 defaultConfig 块")

            # 找到 defaultConfig 块的开始位置（相对于整个文件）
            defaultConfig_start_in_file = android_start + defaultConfig_pos
            defaultConfig_brace = user_content.find('{', defaultConfig_start_in_file)

            # 在 defaultConfig { 后插入
            insertion_point = defaultConfig_brace + 1
            new_content = (
                new_content[:insertion_point] +
                '\n' + ndk_config + '\n' + cmake_config_in_defaultConfig + '\n' +
                new_content[insertion_point:]
            )

            print(f"  ✓ 已在 defaultConfig 中添加 NDK 和 CMake 配置")
        else:
            # 没有找到 defaultConfig，创建一个
            print(f"  ⚠️  未找到 defaultConfig 块，将创建新的")

            insertion_point = android_start + 1
            defaultConfig_block = f"""
    defaultConfig {{
{ndk_config}
{cmake_config_in_defaultConfig}
    }}
"""
            new_content = (
                new_content[:insertion_point] +
                defaultConfig_block +
                new_content[insertion_point:]
            )

            print(f"  ✓ 已创建 defaultConfig 并添加配置")

        # 在 android 块末尾插入 externalNativeBuild
        # 重新查找 android 块的结束位置（因为内容已经改变）
        android_block_start = new_content.find('android {')
        android_start = new_content.find('{', android_block_start)
        android_end = find_block_end(new_content, android_start)

        # 在 android 块的结束 } 之前插入
        new_content = (
            new_content[:android_end] +
            '\n' + external_native_build + '\n' +
            new_content[android_end:]
        )

        print(f"  ✓ 已在 android 块中添加 externalNativeBuild 配置")

        # 写入新内容
        user_gradle_file.write_text(new_content, encoding='utf-8')

        print(f"\n✅ build.gradle 配置合并完成")
        print(f"\n📝 已添加的配置:")
        print(f"  • ndkVersion: 23.2.8568313")
        print(f"  • NDK 配置 (abiFilters)")
        print(f"  • CMake 配置 (cppFlags, arguments)")
        print(f"  • externalNativeBuild 配置")
        print(f"  • CMakeLists.txt 路径: {cmake_relative_path}")
        print(f"\n💾 原文件已备份到: {backup_file.name}")
        print(f"\n⚠️  提示: 如果构建出现问题，可以从备份文件恢复")

        return True

    except Exception as e:
        print(f"❌ 合并 build.gradle 失败: {e}")
        import traceback
        traceback.print_exc()

        # 显示手动合并说明
        print(f"\n⚠️  自动合并失败，请手动合并配置")
        print(f"\n请将以下内容添加到 {user_gradle_file} 的 android {{ }} 块中:")
        print("\n" + "-" * 50)
        print(engine_content)
        print("-" * 50)

        return True  # 不作为致命错误

def show_next_steps(project_path, project_name):
    """显示后续步骤"""
    print("\n" + "="*50)
    print("🎉 引擎文件添加完成！")
    print("="*50)

    print(f"\n📂 项目名称: {project_name}")
    print(f"📍 项目位置: {project_path}")

    print(f"\n📁 已添加的内容:")
    print(f"  • assets/          - 资源文件目录")
    print(f"  • cpp/             - C++ 源代码目录")
    print(f"  │   ├── engine/    - 引擎源代码")
    print(f"  │   └── ai/        - AI 模块")
    print(f"  • java/            - Java 源代码目录")
    print(f"  • build.gradle     - 已自动合并引擎配置")

    print(f"\n🚀 接下来的步骤:")
    print(f"  1. 使用 Android Studio 打开项目")
    print(f"  2. Sync Gradle 并构建项目")
    print(f"  3. 开始开发你的应用")

    print(f"\n⚠️  重要提示:")
    print(f"  • build.gradle 配置已自动合并，如有问题可从备份恢复")
    print(f"  • 确保已安装 Android SDK 和 NDK")
    print(f"  • 如果构建失败，请检查 NDK 版本和 CMake 版本")
    print(f"  • NDK 推荐版本: 25.x 或更高")
    print(f"  • CMake 推荐版本: 3.22.1 或更高")

def main():
    """主函数"""
    if len(sys.argv) != 3:
        print("❌ 用法错误")
        print("用法: create-android.py <Android工程路径> <项目名称>")
        return 1

    project_path = Path(sys.argv[1])
    project_name = sys.argv[2]

    print(f"\n" + "="*50)
    print(f"Boo-Engine Android 引擎文件添加工具")
    print(f"="*50)
    print(f"\n项目名称: {project_name}")
    print(f"Android 工程路径: {project_path}")

    # 获取引擎根目录
    engine_root = get_engine_root()
    print(f"引擎根目录: {engine_root}")

    # 检查项目目录是否存在
    if not project_path.exists():
        print(f"\n❌ 错误: Android 工程目录不存在: {project_path}")
        return 1

    # 查找 AndroidManifest.xml 所在目录
    manifest_dir = find_android_manifest_dir(project_path)
    if not manifest_dir:
        print(f"\n❌ 错误: 这不是一个有效的 Android 工程")
        return 1

    # 执行添加步骤
    print(f"\n开始添加引擎文件到 Android 工程...")

    # 核心步骤
    steps = [
        ("复制引擎目录（assets, cpp, java）", lambda: copy_platform_directories(engine_root, project_path)),
        ("复制引擎和 ai 目录", lambda: copy_engine_and_ai_directory(engine_root, manifest_dir)),
        ("合并 build.gradle 配置", lambda: merge_gradle_config(engine_root, project_path)),
    ]

    # 执行核心步骤
    for step_name, step_func in steps:
        if not step_func():
            print(f"\n❌ 失败: {step_name}")
            return 1

    # 显示完成信息
    show_next_steps(project_path, project_name)

    return 0

if __name__ == "__main__":
    sys.exit(main())
