#!/bin/bash
# Boo-Engine Android Build Script
# Author: 杨宗宝
echo "=========================================="
echo "更新CMakeLists.txt"
echo "=========================================="

# 检查Python3是否安装
if ! command -v python3 &> /dev/null; then
    echo "✗ 错误：未找到python3，请先安装Python 3"
    exit 1
fi

# 检查CMakeLists.py是否存在
if [ ! -f "CMakeLists.py" ]; then
    echo "✗ 错误：未找到CMakeLists.py脚本"
    exit 1
fi

# 执行Python脚本更新CMakeLists.txt
echo "正在扫描源文件并更新CMakeLists.txt..."
python3 CMakeLists.py

# 检查执行结果
if [ $? -eq 0 ]; then
    echo "✓ CMakeLists.txt 更新成功"
else
    echo "✗ CMakeLists.txt 更新失败"
    exit 1
fi

echo ""
echo "✓ CMakeLists.txt 更新成功"
echo ""


