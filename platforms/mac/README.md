# Boo-Engine：基于Vulkan的现代游戏引擎-macOS适配

> 本文将详细介绍Boo-Engine引擎在macOS平台的适配方案，帮助开发者快速搭建开发环境并创建项目。

## 🎯 引擎简介

Boo-Engine是一款轻量级游戏引擎，专注于跨平台开发，支持Windows、macOS和Android等多个平台。引擎采用现代C++开发，提供了简洁易用的API接口，适合各类2D和3D游戏开发。

![](https://files.mdnice.com/user/3069/3f0d1749-839f-435d-9af2-f598313af5a7.png)


## 📋 环境要求

在开始之前，请确保你的macOS系统满足以下要求：

- **操作系统**：macOS
- **开发工具**：Trae IDE
- **构建工具**：CMake 3.20+
- **依赖库**：
  - GLFW3（窗口管理）
  - Vulkan SDK（图形渲染）
  - stb（图像加载，已包含在引擎中）

## 🚀 快速开始

### 1. 下载引擎

首先，从GitHub仓库下载Boo-Engine引擎源码：

```bash
git clone https://github.com/carlsoyzy/Boo-Engine.git
cd Boo-Engine
```

### 2. 环境搭建

#### 安装CMake

1. 访问[CMake官网](https://cmake.org/download/)
2. 下载并安装最新版本的CMake for macOS
3. 或者使用Homebrew安装：
   ```bash
   brew install cmake
   ```

#### 安装GLFW

使用Homebrew安装GLFW：

```bash
brew install glfw
```

#### 安装Vulkan SDK

1. 访问[Vulkan SDK官网](https://vulkan.lunarg.com/sdk/home)
2. 下载并安装最新版本的Vulkan SDK for macOS
3. 按照安装向导完成安装
4. 确保Vulkan SDK的环境变量已正确设置


### 3. 创建macOS项目

Boo-Engine提供了便捷的项目创建工具，让你可以快速搭建macOS平台的开发环境。

#### 步骤1：运行创建工具

在引擎根目录执行以下命令：

```bash
python boo-engine.py
```

#### 步骤2：选择平台

在菜单中选择「3. macOS」平台：

```
请选择目标平台：
  1. Android - 适用于Android移动应用开发
  2. Windows - 适用于Windows桌面应用开发
  3. macOS - 适用于苹果Mac电脑应用开发

请输入选项编号 (1-3) [推荐: 3]: 3
```

#### 步骤3：选择项目目录

输入你想要创建项目的绝对路径：

```
请输入项目创建目录（绝对路径）: /Users/username/Projects
绝对路径: /Users/username/Projects
✅ 目录验证通过: /Users/username/Projects
```

#### 步骤4：输入项目名称

输入你想要创建的项目名称：

```
请输入项目名称: MyGame
✅ 项目名称验证通过: MyGame
项目完整路径: /Users/username/Projects/MyGame
```

#### 步骤5：确认创建

确认创建项目：

```
确认创建项目？(y/n): y
```

#### 步骤6：等待创建完成

工具会自动完成以下操作：
- 创建项目目录
- 复制引擎核心文件
- 复制macOS平台相关文件

完成后会显示：

```
🎉 macOS 项目创建完成！

📂 项目名称: MyGame
📍 项目位置: /Users/username/Projects/MyGame

📁 项目结构:
  MyGame/
  ├── engine/          # 引擎源代码
  ├── ai/              # AI 模块
  └── [平台文件]        # macOS 平台相关文件
```

## 🔧 项目构建运行

### 方法1：使用构建脚本

项目创建完成后，可以使用提供的构建脚本来编译项目：

1. 进入项目目录：
   ```bash
   cd /Users/username/Projects/MyGame
   ```

2. 运行构建脚本：
   ```bash
   ./build.sh
   ```
   
![](https://files.mdnice.com/user/3069/0abce100-2e80-4312-a9e1-477fff35a27d.jpg)
   

## 📁 项目结构

创建的macOS项目具有以下结构：

```
MyGame/
├── engine/          # 引擎核心代码
│   ├── core/        # 核心功能模块
│   ├── render/      # 渲染模块
│   └── utils/       # 工具类
├── ai/              # AI模块（可选）
├── src/             # 项目源代码
│   └── app.cpp      # 应用入口
├── assets/          # 资源文件
├── libs/            # 第三方库
├── build/           # 构建目录
├── CMakeLists.txt   # CMake配置文件
├── build.sh         # 构建脚本
└── README.md        # 项目说明
```

## 📞 技术支持

如果遇到问题，可以通过以下方式获取帮助：

- **QQ群**：1091366480
- **微信**：Tidewalker_16



