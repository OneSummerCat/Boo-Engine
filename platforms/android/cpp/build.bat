@echo off
chcp 65001 >nul
echo 正在创建符号链接...

REM 创建editor目录链接
if exist "editor" (
    echo 已存在editor目录
) else (
    mklink /J "editor" "../../editor" >nul 2>&1
    if errorlevel 1 (
        echo 创建editor链接失败
    ) else (
        echo 创建editor链接成功
    )
)

REM 创建engine目录链接
if exist "engine" (
    echo 已存在engine目录
) else (
    mklink /J "engine" "../../engine" >nul 2>&1
    if errorlevel 1 (
        echo 创建engine链接失败
    ) else (
        echo 创建engine链接成功
    )
)

echo.
echo 符号链接创建完成
pause