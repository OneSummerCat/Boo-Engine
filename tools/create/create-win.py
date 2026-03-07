#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# create-win.py - Windows 平台项目创建脚本

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
    """第一步：复制引擎 engine 目录和 ai 目录到项目根目录"""
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

def get_win_platform_dir(engine_root):
    """获取 Windows 平台目录（使用 platforms/win）"""
    # 使用 platforms/win
    win_dir = engine_root / "platforms" / "win"
    if win_dir.exists():
        print(f"✅ 使用 Windows 平台目录: {win_dir}")
        return win_dir

    return None

def copy_win_platform_files(win_platform_dir, project_path):
    """第二步：复制 platforms/win 下的所有文件和文件夹到项目目录"""
    print("\n" + "="*50)
    print("第二步：复制 Windows 平台文件")
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
        print(f"📦 源目录: {win_platform_dir}")
        print(f"📦 目标目录: {project_path}")
        print()

        # 遍历 win 平台目录下的所有文件和文件夹
        for item in win_platform_dir.iterdir():
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

        print(f"\n✅ Windows 平台文件复制完成")
        return True
    except Exception as e:
        print(f"❌ 复制平台文件失败: {e}")
        import traceback
        traceback.print_exc()
        return False

def show_next_steps(project_path, project_name):
    """显示后续步骤"""
    print("\n" + "="*50)
    print("🎉 Windows 项目创建完成！")
    print("="*50)

    print(f"\n📂 项目名称: {project_name}")
    print(f"📍 项目位置: {project_path}")

    print(f"\n📁 项目结构:")
    print(f"  {project_name}/")
    print(f"  ├── engine/          # 引擎源代码")
    print(f"  ├── ai/              # AI 模块")
    print(f"  └── [平台文件]        # Windows 平台相关文件")

    print(f"\n📖 更多信息:")
    print(f"  • 项目目录已准备就绪")
    print(f"  • 可以开始在该目录中进行开发")

def main():
    """主函数"""
    if len(sys.argv) != 3:
        print("❌ 用法错误")
        print("用法: create-win.py <项目路径> <项目名称>")
        return 1

    project_path = Path(sys.argv[1])
    project_name = sys.argv[2]

    print(f"\n" + "="*50)
    print(f"Boo-Engine Windows 项目创建工具")
    print(f"="*50)
    print(f"\n项目名称: {project_name}")
    print(f"项目路径: {project_path}")

    # 获取引擎根目录
    engine_root = get_engine_root()
    print(f"引擎根目录: {engine_root}")

    # 获取 Windows 平台目录
    win_platform_dir = get_win_platform_dir(engine_root)
    if not win_platform_dir:
        print(f"\n❌ 错误: 找不到 Windows 平台目录")
        print("提示: 应该存在 platforms/win/ 目录")
        return 1

    # 检查项目目录是否存在
    if not project_path.exists():
        print(f"\n❌ 错误: 项目目录不存在: {project_path}")
        print("提示: 项目目录应该由 create.py 创建")
        return 1

    # 执行创建步骤
    print(f"\n开始创建 Windows 项目...")

    # 核心步骤
    steps = [
        ("复制引擎 engine 目录和 ai 目录", lambda: copy_engine_directory(engine_root, project_path)),
        ("复制 Windows 平台文件", lambda: copy_win_platform_files(win_platform_dir, project_path)),
    ]

    # 执行核心步骤
    for step_name, step_func in steps:
        if not step_func():
            print(f"\n❌ 创建失败: {step_name}")
            return 1

    # 显示完成信息
    show_next_steps(project_path, project_name)

    return 0

if __name__ == "__main__":
    sys.exit(main())
