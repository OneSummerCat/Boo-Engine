#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# create.py - 主菜单脚本

import sys
from pathlib import Path

# 导入创建项目的模块
try:
    from tools.create.create import create_project
except ImportError:
    print("错误: 找不到 create.py 文件")
    print("请确保 create.py 在同一目录下")
    sys.exit(1)

def show_menu():
    """显示主菜单"""
    print("=" * 50)
    print("项目创建工具")
    print("=" * 50)
    
    # 操作菜单
    operations = {
        '1': {
            'name': '创建项目',
            'description': '创建一个新的项目',
            'function': create_project  # 直接存储函数引用
        }
    }
    
    print("\n请选择操作：")
    for key, op in operations.items():
        print(f"  {key}. {op['name']} - {op['description']}")
    
    print("  q. 退出")
    
    while True:
        choice = input("\n请输入选项: ").strip().lower()
        
        if choice == 'q':
            print("退出程序")
            return None
        
        if choice in operations:
            print(f"\n✅ 已选择: {operations[choice]['name']}")
            return operations[choice]['function']
        else:
            print("❌ 无效选项，请重新输入")

def main():
    """主函数"""
    try:
        while True:
            # 显示菜单并获取选择的函数
            selected_function = show_menu()
            
            if selected_function is None:
                break
            
            print("\n" + "=" * 50)
            print(f"启动创建向导...")
            print("=" * 50)
            
            # 直接调用函数
            result = selected_function()
            
            if result != 0:
                print(f"\n⚠️  操作返回错误代码: {result}")
            
            print("\n" + "=" * 50)
            input("按回车键返回主菜单...")
            
    except KeyboardInterrupt:
        print("\n\n⚠️  用户中断操作")
        return 1
    except Exception as e:
        print(f"\n❌ 发生错误: {e}")
        return 1
    
    return 0

if __name__ == "__main__":
    sys.exit(main())