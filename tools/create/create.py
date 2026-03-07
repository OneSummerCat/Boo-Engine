#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# create_project.py - 创建项目的具体逻辑

import os
import sys
import platform
import subprocess
from pathlib import Path

def get_creation_directory():
    """获取并检测创建目录"""
    print("\n" + "-" * 50)
    print("第一步：选择目录")
    print("-" * 50)

    while True:
        # 提示用户输入
        dir_input = input("\n请输入项目创建目录（绝对路径）: ").strip()

        # 检查是否为空
        if not dir_input:
            print("❌ 错误: 路径不能为空，请输入绝对路径")
            print("示例: /Users/yangzongyuan/personal/project")
            continue

        # 去除首尾的引号（支持单引号和双引号）
        if (dir_input.startswith('"') and dir_input.endswith('"')) or \
           (dir_input.startswith("'") and dir_input.endswith("'")):
            dir_input = dir_input[1:-1]

        # 将输入的字符串转换为Path对象
        input_path = Path(dir_input).expanduser()  # 支持 ~ 符号

        # 检查是否是绝对路径
        if not input_path.is_absolute():
            print(f"❌ 错误: 必须输入绝对路径，不支持相对路径")
            print(f"您输入的路径: {dir_input}")
            print("示例: /Users/yangzongyuan/personal/project")
            continue

        target_path = input_path
        print(f"绝对路径: {target_path}")

        # 检查路径是否存在
        if not target_path.exists():
            print(f"❌ 错误: 路径不存在 - {target_path}")
            create = input("是否创建此目录？(y/n): ").strip().lower()
            if create == 'y':
                try:
                    target_path.mkdir(parents=True, exist_ok=True)
                    print(f"✅ 目录创建成功: {target_path}")
                    return target_path
                except Exception as e:
                    print(f"❌ 创建目录失败: {e}")
                    continue
            else:
                print("请重新输入一个存在的路径")
                continue

        # 检查是否是目录
        if not target_path.is_dir():
            print(f"❌ 错误: {target_path} 是一个文件，不是目录")
            print("请输入目录路径")
            continue

        # 检查写入权限
        if not os.access(target_path, os.W_OK):
            print(f"⚠️  警告: {target_path} 没有写入权限")
            retry = input("是否继续使用此目录？(y/n): ").strip().lower()
            if retry != 'y':
                continue

        # 所有检查通过
        print(f"✅ 目录验证通过: {target_path}")
        return target_path

def get_project_name(base_dir):
    """获取项目名称并检查目录是否存在"""
    print("\n" + "-" * 50)
    print("第二步：输入项目名称")
    print("-" * 50)

    while True:
        project_name = input("\n请输入项目名称: ").strip()

        # 检查是否为空
        if not project_name:
            print("❌ 错误: 项目名称不能为空")
            continue

        # 检查是否包含非法字符
        invalid_chars = '<>:"/\\|?*'
        if any(char in project_name for char in invalid_chars):
            print(f"❌ 错误: 项目名称不能包含以下字符: {invalid_chars}")
            continue

        # 检查项目目录是否已存在
        project_path = base_dir / project_name
        if project_path.exists():
            print(f"⚠️  警告: 目录 {project_path} 已存在")
            print("请重新输入一个不同的项目名称")
            continue

        print(f"✅ 项目名称验证通过: {project_name}")
        print(f"项目完整路径: {project_path}")
        return project_name, project_path

def select_platform():
    """选择目标平台"""
    print("\n" + "-" * 50)
    print("第一步：选择目标平台")
    print("-" * 50)

    # 平台选项
    platforms = {
        '1': {
            'name': 'Android',
            'key': 'android',
            'description': '适用于Android移动应用开发',
            'script': 'create-android.py'
        },
        '2': {
            'name': 'Windows',
            'key': 'win',
            'description': '适用于Windows桌面应用开发',
            'script': 'create-win.py'
        },
        '3': {
            'name': 'macOS',
            'key': 'mac',
            'description': '适用于苹果Mac电脑应用开发',
            'script': 'create-mac.py'
        }
    }

    # 显示平台选项
    print("\n请选择目标平台：")
    for key, platform_info in platforms.items():
        print(f"  {key}. {platform_info['name']} - {platform_info['description']}")

    # 获取当前系统作为默认建议
    current_system = platform.system()
    default_choice = '1'  # 默认Android

    if current_system == 'Darwin':  # macOS
        default_choice = '3'
    elif current_system == 'Windows':
        default_choice = '2'
    elif current_system == 'Linux':
        default_choice = '1'  # Linux推荐Android

    while True:
        choice = input(f"\n请输入选项编号 (1-3) [推荐: {default_choice}]: ").strip()

        # 如果直接回车，使用推荐选项
        if not choice:
            choice = default_choice

        if choice in platforms:
            selected = platforms[choice]
            print(f"\n✅ 已选择平台: {selected['name']}")
            print(f"   平台标识: {selected['key']}")
            return selected['key'], selected['name'], selected['script']
        else:
            print("❌ 无效选项，请输入 1、2 或 3")

def create_project_directory(project_path):
    """创建项目目录"""
    try:
        project_path.mkdir(parents=True, exist_ok=False)
        print(f"✅ 项目目录创建成功: {project_path}")
        return True
    except FileExistsError:
        print(f"❌ 错误: 项目目录已存在: {project_path}")
        return False
    except Exception as e:
        print(f"❌ 创建项目目录失败: {e}")
        return False

def call_platform_script(script_name, project_path, project_name):
    """调用平台特定的创建脚本"""
    # 获取脚本所在目录（tools/create）
    script_dir = Path(__file__).parent
    script_path = script_dir / script_name

    # 检查脚本是否存在
    if not script_path.exists():
        print(f"❌ 错误: 找不到平台创建脚本: {script_path}")
        return False

    print(f"\n" + "=" * 50)
    print(f"开始执行平台创建脚本: {script_name}")
    print("=" * 50)

    try:
        # 调用平台脚本，传递项目路径和项目名称
        result = subprocess.run(
            [sys.executable, str(script_path), str(project_path), project_name],
            check=True,
            capture_output=False
        )

        if result.returncode == 0:
            print(f"\n✅ 平台项目创建成功！")
            return True
        else:
            print(f"\n❌ 平台项目创建失败，返回代码: {result.returncode}")
            return False

    except subprocess.CalledProcessError as e:
        print(f"❌ 执行平台脚本失败: {e}")
        return False
    except Exception as e:
        print(f"❌ 发生错误: {e}")
        return False

def get_android_project_directory():
    """获取已存在的 Android 工程目录"""
    print("\n" + "-" * 50)
    print("选择已有的 Android 工程目录")
    print("-" * 50)

    while True:
        # 提示用户输入
        dir_input = input("\n请输入 Android 工程根目录（绝对路径）: ").strip()

        # 检查是否为空
        if not dir_input:
            print("❌ 错误: 路径不能为空，请输入绝对路径")
            print("示例: /Users/yangzongyuan/AndroidStudioProjects/MyApp")
            continue

        # 去除首尾的引号（支持单引号和双引号）
        if (dir_input.startswith('"') and dir_input.endswith('"')) or \
           (dir_input.startswith("'") and dir_input.endswith("'")):
            dir_input = dir_input[1:-1]

        # 将输入的字符串转换为Path对象
        input_path = Path(dir_input).expanduser()  # 支持 ~ 符号

        # 检查是否是绝对路径
        if not input_path.is_absolute():
            print(f"❌ 错误: 必须输入绝对路径，不支持相对路径")
            print(f"您输入的路径: {dir_input}")
            print("示例: /Users/yangzongyuan/AndroidStudioProjects/MyApp")
            continue

        target_path = input_path
        print(f"绝对路径: {target_path}")

        # 检查路径是否存在
        if not target_path.exists():
            print(f"❌ 错误: 路径不存在 - {target_path}")
            print("请确保输入的是已创建好的 Android 工程目录")
            continue

        # 检查是否是目录
        if not target_path.is_dir():
            print(f"❌ 错误: {target_path} 是一个文件，不是目录")
            print("请输入目录路径")
            continue

        # 检查是否是有效的 Android 工程目录
        # 简单检查：看是否有 AndroidManifest.xml 或 build.gradle
        has_manifest = (target_path / "app" / "src" / "main" / "AndroidManifest.xml").exists() or \
                       (target_path / "src" / "main" / "AndroidManifest.xml").exists()
        has_gradle = (target_path / "build.gradle").exists() or \
                     (target_path / "build.gradle.kts").exists()

        if not (has_manifest or has_gradle):
            print(f"⚠️  警告: {target_path} 可能不是一个 Android 工程目录")
            print("提示: Android 工程目录应该包含 build.gradle 或 AndroidManifest.xml 文件")
            retry = input("是否继续使用此目录？(y/n): ").strip().lower()
            if retry != 'y':
                continue

        # 检查写入权限
        if not os.access(target_path, os.W_OK):
            print(f"⚠️  警告: {target_path} 没有写入权限")
            retry = input("是否继续使用此目录？(y/n): ").strip().lower()
            if retry != 'y':
                continue

        # 所有检查通过
        print(f"✅ Android 工程目录验证通过: {target_path}")
        return target_path

def create_project():
    """创建项目的主函数 - 供外部调用"""
    try:
        print("\n欢迎使用项目创建向导\n")

        # 第一步：选择平台
        platform_key, platform_name, platform_script = select_platform()

        # 根据平台类型走不同的流程
        if platform_key == 'android':
            # Android 平台：选择已有的 Android 工程目录
            print("\n" + "=" * 50)
            print("Android 平台创建流程")
            print("=" * 50)
            print("提示: Android 平台需要在已有的 Android 工程上添加引擎文件")

            project_path = get_android_project_directory()
            project_name = project_path.name  # 使用目录名作为项目名

            print("\n" + "=" * 50)
            print("✅ 项目信息收集完成！")
            print("=" * 50)
            print(f"📍 Android 工程路径: {project_path}")
            print(f"📂 项目名称: {project_name}")
            print(f"🎯 目标平台: {platform_name} ({platform_key})")

            # 确认操作
            print("\n" + "-" * 50)
            print("⚠️  警告: 此操作将向 Android 工程添加引擎文件")
            confirm = input("确认继续？(y/n): ").strip().lower()
            if confirm != 'y':
                print("⚠️  用户取消操作")
                return 1

            # 调用平台特定的创建脚本
            print("\n" + "=" * 50)
            print("开始添加引擎文件到 Android 工程...")
            print("=" * 50)

            if not call_platform_script(platform_script, project_path, project_name):
                print("\n⚠️  警告: 平台文件添加未完全成功")
                return 1

            # 完成
            print("\n" + "=" * 50)
            print("🎉 引擎文件添加完成！")
            print("=" * 50)
            print(f"\n项目位置: {project_path}")
            print(f"平台类型: {platform_name}")

        else:
            # Mac/Windows 平台：创建新项目
            print("\n" + "=" * 50)
            print(f"{platform_name} 平台创建流程")
            print("=" * 50)

            # 第二步：选择创建目录
            selected_dir = get_creation_directory()

            # 第三步：输入项目名称（内部会检查目录是否存在）
            project_name, project_path = get_project_name(selected_dir)

            print("\n" + "=" * 50)
            print("✅ 项目信息收集完成！")
            print("=" * 50)
            print(f"📁 基础目录: {selected_dir}")
            print(f"📂 项目名称: {project_name}")
            print(f"📍 完整路径: {project_path}")
            print(f"🎯 目标平台: {platform_name} ({platform_key})")

            # 确认创建
            print("\n" + "-" * 50)
            confirm = input("确认创建项目？(y/n): ").strip().lower()
            if confirm != 'y':
                print("⚠️  用户取消操作")
                return 1

            # 第四步：创建项目目录
            print("\n" + "=" * 50)
            print("开始创建项目...")
            print("=" * 50)

            if not create_project_directory(project_path):
                return 1

            # 第五步：调用平台特定的创建脚本
            if not call_platform_script(platform_script, project_path, project_name):
                print("\n⚠️  警告: 平台项目创建未完全成功")
                print(f"项目目录已创建: {project_path}")
                return 1

            # 完成
            print("\n" + "=" * 50)
            print("🎉 项目创建完成！")
            print("=" * 50)
            print(f"\n项目位置: {project_path}")
            print(f"平台类型: {platform_name}")

        print("\n接下来的步骤：")
        print(f"  1. cd {project_path}")
        print(f"  2. 查看 README.md 了解构建说明")

        return 0  # 成功返回0

    except KeyboardInterrupt:
        print("\n\n⚠️  用户中断操作")
        return 1
    except Exception as e:
        print(f"\n❌ 发生错误: {e}")
        import traceback
        traceback.print_exc()
        return 1

# 保持直接运行的能力
if __name__ == "__main__":
    sys.exit(create_project())
