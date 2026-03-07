@echo off
chcp 65001 >nul
# Boo-Engine Windows Build Script
# Author: 杨宗宝
REM ==========================================
REM 创建build目录并运行CMake
REM ==========================================
echo "=========================================="
echo "创建build目录并运行CMake"
echo "=========================================="

REM 检查CMake是否可用
cmake --version >nul 2>&1
if errorlevel 1 (
    echo 错误: 未找到CMake，请安装CMake并添加到环境变量
    goto :error
)

REM 创建build目录（如果不存在）
if not exist "build" (
    echo 正在创建build目录...
    mkdir build
    if errorlevel 1 (
        echo 错误: 创建build目录失败
        goto :error
    ) else (
        echo build目录创建成功
    )
) else (
    echo build目录已存在
)

REM 进入build目录并运行CMake
cd build
echo 当前目录: %cd%

echo 正在运行CMake配置...
cmake .. -G "Visual Studio 17 2022" -A x64

if errorlevel 1 (
    echo 错误: CMake配置失败
    cd ..
    goto :error
) else (
    echo CMake配置成功
)

cd ..

echo ""
echo "✓ CMake配置成功"
echo ""
