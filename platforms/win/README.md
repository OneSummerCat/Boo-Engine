# Boo-Engine：基于Vulkan的现代游戏引擎-Windows适配

> 本文将详细介绍Boo-Engine引擎在Windows平台的适配方案，帮助开发者快速搭建开发环境并创建项目。

## 🎯 引擎简介

Boo-Engine是一款轻量级游戏引擎，专注于跨平台开发，支持Windows、macOS和Android等多个平台。引擎采用现代C++开发，提供了简洁易用的API接口，适合各类2D和3D游戏开发。

![](https://files.mdnice.com/user/3069/3f0d1749-839f-435d-9af2-f598313af5a7.png)


## 📋 环境要求

在开始之前，请确保你的Windows系统满足以下要求：

- **操作系统**：Windows 10/11 64位
- **开发工具**：Visual Studio 2022（推荐）
- **构建工具**：CMake 3.20+
- **依赖库**：
  - GLFW3（窗口管理）
  - stb（图像加载，已包含在引擎中）

## 🚀 快速开始

### 1. 下载引擎

首先，从GitHub仓库下载Boo-Engine引擎源码：

```bash
git clone https://github.com/carlsoyzy/Boo-Engine.git
cd Boo-Engine
```

### 2. 环境搭建

#### 安装Visual Studio 2022

1. 访问[Visual Studio官网](https://visualstudio.microsoft.com/zh-hans/downloads/)
2. 下载并安装Visual Studio 2022 Community版本
3. 安装时选择「使用C++的桌面开发」工作负载

#### 安装CMake

1. 访问[CMake官网](https://cmake.org/download/)
2. 下载并安装最新版本的CMake
3. 确保将CMake添加到系统环境变量

### 3. 创建Windows项目

Boo-Engine提供了便捷的项目创建工具，让你可以快速搭建Windows平台的开发环境。

#### 步骤1：运行创建工具

在引擎根目录执行以下命令：

```bash
python boo-engine.py
```

#### 步骤2：选择平台

在菜单中选择「2. Windows」平台：

```
请选择目标平台：
  1. Android - 适用于Android移动应用开发
  2. Windows - 适用于Windows桌面应用开发
  3. macOS - 适用于苹果Mac电脑应用开发

请输入选项编号 (1-3) [推荐: 2]: 2
```

#### 步骤3：选择项目目录

输入你想要创建项目的绝对路径：

```
请输入项目创建目录（绝对路径）: D:\Projects
绝对路径: D:\Projects
✅ 目录验证通过: D:\Projects
```

#### 步骤4：输入项目名称

输入你想要创建的项目名称：

```
请输入项目名称: MyGame
✅ 项目名称验证通过: MyGame
项目完整路径: D:\Projects\MyGame
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
- 复制Windows平台相关文件

完成后会显示：

```
🎉 Windows 项目创建完成！

📂 项目名称: MyGame
📍 项目位置: D:\Projects\MyGame

📁 项目结构:
  MyGame/
  ├── engine/          # 引擎源代码
  ├── ai/              # AI 模块
  └── [平台文件]        # Windows 平台相关文件
```

## 🔧 构建项目

### 方法1：使用构建脚本

项目创建完成后，可以使用提供的构建脚本来编译项目：

1. 进入项目目录：
   ```bash
   cd D:\Projects\MyGame
   ```

2. 运行构建脚本：
   ```bash
   build.bat
   ```

   脚本会自动：
   - 执行CMakeLists.py更新配置
   - 创建build目录
   - 运行CMake配置生成VS解决方案

## 🎮 运行项目

编译完成后，可以通过以下方式运行项目：
1. **设置启动项**：
  - 解决方案下，设置启动项（默认是Boo-Engine）
2. **从Visual Studio运行**：
   - 点击「调试」→「开始执行(不调试)」

3. **手动运行**：
   - 在build目录中找到生成的可执行文件
   - 双击运行

![](https://files.mdnice.com/user/3069/781f3b10-43f9-4e4d-8ce8-a9b8bf0db040.png)

## 📁 项目结构

创建的Windows项目具有以下结构：

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
├── build.bat        # 构建脚本
└── README.md        # 项目说明
```

## 📞 技术支持

如果遇到问题，可以通过以下方式获取帮助：

- **QQ群**：1091366480
- **微信**：Tidewalker_16



