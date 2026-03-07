#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# create-mac.py - macOS 平台项目创建脚本

import os
import sys
import shutil
from pathlib import Path

def get_engine_root():
    """获取引擎根目录（Boo-Engine）"""
    script_path = Path(__file__).resolve()
    engine_root = script_path.parent.parent.parent
    return engine_root

def copy_engine_directory(engine_root, project_path):
    """第一步：复制引擎 engine 目录和 ai 目录到项目中"""
    print("\n" + "="*50)
    print("第一步：复制引擎 engine 目录和 ai 目录")
    print("="*50)

    # 复制 engine 目录
    engine_src = engine_root / "engine"
    engine_dst = project_path / "engine"

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
        return False

    # 复制 ai 目录
    ai_src = engine_root / "ai"
    ai_dst = project_path / "ai"

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

def get_mac_platform_dir(engine_root):
    """获取 Mac 平台目录（优先使用根目录的 mac，否则使用 platforms/mac）"""
    # 优先使用根目录的 mac 目录
    mac_dir = engine_root / "mac"
    if mac_dir.exists():
        print(f"✅ 使用 Mac 平台目录: {mac_dir}")
        return mac_dir

    # 回退到 platforms/mac
    mac_dir = engine_root / "platforms" / "mac"
    if mac_dir.exists():
        print(f"✅ 使用 Mac 平台目录: {mac_dir}")
        return mac_dir

    return None

def copy_mac_platform_files(mac_platform_dir, project_path):
    """第二步：复制 platforms/mac 下的所有文件和文件夹到项目目录"""
    print("\n" + "="*50)
    print("第二步：复制 Mac 平台文件")
    print("="*50)

    # 需要排除的文件和目录
    exclude_items = {
        '.DS_Store',
        '.git',
        '.gitignore',
        '__pycache__',
        '*.pyc',
        '.idea',
        '.vscode',
        'build'  # 排除构建目录
    }

    try:
        print(f"📦 源目录: {mac_platform_dir}")
        print(f"📦 目标目录: {project_path}")
        print()

        # 遍历 mac 平台目录下的所有文件和文件夹
        for item in mac_platform_dir.iterdir():
            # 跳过排除项
            if item.name in exclude_items or item.name.startswith('.'):
                print(f"  ⊗ 跳过: {item.name}")
                continue

            src_path = item
            dst_path = project_path / item.name

            if item.is_dir():
                print(f"  📁 复制目录: {item.name}")
                shutil.copytree(src_path, dst_path, dirs_exist_ok=True)
            else:
                print(f"  📄 复制文件: {item.name}")
                shutil.copy2(src_path, dst_path)

                # 如果是 .sh 脚本，设置可执行权限
                if item.suffix == '.sh':
                    os.chmod(dst_path, 0o755)
                    print(f"    → 已设置可执行权限")

        print(f"\n✅ Mac 平台文件复制完成")
        return True
    except Exception as e:
        print(f"❌ 复制平台文件失败: {e}")
        import traceback
        traceback.print_exc()
        return False

def create_gitignore(project_path):
    """创建 .gitignore 文件"""
    print("\n" + "="*50)
    print("附加步骤：创建 .gitignore")
    print("="*50)

    gitignore_content = """# Build directories
build/
cmake-build-*/

# IDE
.vscode/
.idea/
*.swp
*.swo
*~

# macOS
.DS_Store
.AppleDouble
.LSOverride

# Compiled files
*.o
*.a
*.so
*.dylib

# Executables
BooEngine
*.app

# CMake
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
Makefile

# Logs
*.log
"""

    gitignore_file = project_path / ".gitignore"

    try:
        gitignore_file.write_text(gitignore_content, encoding='utf-8')
        print(f"✅ .gitignore 创建完成")
        return True
    except Exception as e:
        print(f"⚠️  警告: 创建 .gitignore 失败: {e}")
        return True  # 不是致命错误

def create_build_directory(project_path):
    """创建 build 目录"""
    print("\n" + "="*50)
    print("附加步骤：创建 build 目录")
    print("="*50)

    build_dir = project_path / "build"

    try:
        build_dir.mkdir(exist_ok=True)
        print(f"✅ build 目录创建完成: {build_dir}")
        return True
    except Exception as e:
        print(f"⚠️  警告: 创建 build 目录失败: {e}")
        return True  # 不是致命错误

def show_next_steps(project_path, project_name):
    """显示后续步骤"""
    print("\n" + "="*50)
    print("🎉 macOS 项目创建完成！")
    print("="*50)

    print(f"\n📂 项目名称: {project_name}")
    print(f"📍 项目位置: {project_path}")

    print(f"\n📁 项目结构:")
    print(f"  {project_name}/")
    print(f"  ├── engine/          # 引擎源代码")
    print(f"  ├── ai/              # AI 模块")
    print(f"  ├── libs/            # 第三方库")
    print(f"  ├── src/             # 项目源代码")
    print(f"  ├── assets/          # 资源文件")
    print(f"  ├── build/           # 构建目录")
    print(f"  ├── app.cpp          # 应用程序入口")
    print(f"  ├── CMakeLists.txt   # CMake 配置")
    print(f"  ├── build.sh         # 构建脚本")
    print(f"  └── README.md        # 项目说明")

    print(f"\n🚀 接下来的步骤:")
    print(f"  1. cd {project_path}")
    print(f"  2. ./build.sh              # 编译项目")
    print(f"  3. ./build/BooEngine       # 运行程序")

    print(f"\n📖 更多信息:")
    print(f"  • 查看 README.md 了解详细说明")
    print(f"  • 编辑 app.cpp 开始你的开发")
    print(f"  • 修改 CMakeLists.txt 添加新源文件")

def main():
    """主函数"""
    if len(sys.argv) != 3:
        print("❌ 用法错误")
        print("用法: create-mac.py <项目路径> <项目名称>")
        return 1

    project_path = Path(sys.argv[1])
    project_name = sys.argv[2]

    print(f"\n" + "="*50)
    print(f"Boo-Engine macOS 项目创建工具")
    print(f"="*50)
    print(f"\n项目名称: {project_name}")
    print(f"项目路径: {project_path}")

    # 获取引擎根目录
    engine_root = get_engine_root()
    print(f"引擎根目录: {engine_root}")

    # 获取 Mac 平台目录
    mac_platform_dir = get_mac_platform_dir(engine_root)
    if not mac_platform_dir:
        print(f"\n❌ 错误: 找不到 Mac 平台目录")
        print("提示: 应该存在 mac/ 或 platforms/mac/ 目录")
        return 1

    # 检查项目目录是否存在
    if not project_path.exists():
        print(f"\n❌ 错误: 项目目录不存在: {project_path}")
        print("提示: 项目目录应该由 create.py 创建")
        return 1

    # 执行创建步骤
    print(f"\n开始创建 macOS 项目...")

    # 核心步骤
    steps = [
        ("复制引擎 engine 目录和 ai 目录", lambda: copy_engine_directory(engine_root, project_path)),
        ("复制 Mac 平台文件", lambda: copy_mac_platform_files(mac_platform_dir, project_path)),
    ]

    # 执行核心步骤
    for step_name, step_func in steps:
        if not step_func():
            print(f"\n❌ 创建失败: {step_name}")
            return 1

    # 附加步骤（失败不影响整体）
    create_build_directory(project_path)
    create_gitignore(project_path)

    # 显示完成信息
    show_next_steps(project_path, project_name)

    return 0

if __name__ == "__main__":
    sys.exit(main())
