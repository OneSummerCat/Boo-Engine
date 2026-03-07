# Boo 命名空间核心文档

## 1. 概述

Boo 是引擎的主命名空间，提供对各个核心模块的统一访问。通过 Boo 命名空间，开发者可以方便地使用引擎的所有核心功能，构建完整的游戏应用。

## 2. 核心模块

### 2.1 全局实例

Boo 命名空间提供了以下全局实例，用于访问各个核心系统：

| 实例 | 类型 | 描述 |
|------|------|------|
| `Boo::game` | `Game*` | 游戏核心实例，负责游戏的初始化、更新、渲染和场景管理 |
| `Boo::event` | `Event*` | 事件系统，处理引擎和游戏事件的发布订阅 |
| `Boo::view` | `View*` | 视图系统，管理窗口尺寸和视口 |
| `Boo::assetsManager` | `AssetsManager*` | 资产管理系统，负责加载和管理游戏资源 |
| `Boo::renderer` | `Renderer*` | 渲染系统，负责图形渲染 |

### 2.2 核心类

Boo 命名空间包含以下核心类：

#### 场景管理
- `Boo::Scene` - 场景类，游戏世界的根容器
- `Boo::Node` - 基础节点类
- `Boo::Node2D` - 2D节点类，继承自Node
- `Boo::Node3D` - 3D节点类，独立类（暂未继承自Node）

#### 数学
- `Boo::Vec2` - 2D向量
- `Boo::Vec3` - 3D向量
- `Boo::Mat4` - 4x4矩阵
- `Boo::Quat` - 四元数
- `Boo::Color` - 颜色类
- `Boo::Size` - 尺寸类

#### 资产
- `Boo::Asset` - 基础资产类
- `Boo::TextureAsset` - 纹理资产
- `Boo::MaterialAsset` - 材质资产
- `Boo::ShaderAsset` - 着色器资产
- `Boo::SceneAsset` - 场景资产
- `Boo::MeshAsset` - 网格资产

#### 组件
- `Boo::Component` - 基础组件类
- `Boo::ComponentFactory` - 组件工厂

#### 渲染
- `Boo::Renderer` - 渲染器
- `Boo::Camera` - 相机类

#### 工具
- `Boo::FileUtil` - 文件工具
- `Boo::UUIDUtil` - UUID工具
- `Boo::TimeUtil` - 时间工具

## 3. API 参考

### 3.1 全局实例 API

#### Boo::game - Game* 游戏核心实例

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `openScene(Scene* scene)` | `void` | 打开指定场景 |
| `destroyScene()` | `void` | 销毁当前场景 |
| `getScene()` | `Scene*` | 获取当前场景 |

#### Boo::event - Event* 事件系统

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `on(EventType type, std::function<void()> callback)` | `void` | 添加事件监听器 |
| `trigger(EventType type)` | `void` | 触发指定事件 |

#### Boo::view - View* 视图系统

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `resize(int width, int height)` | `void` | 调整视口大小 |
| `getWidth()` | `int` | 获取视口宽度 |
| `getHeight()` | `int` | 获取视口高度 |
| `getDesignWidth()` | `int` | 获取设计宽度 |
| `getDesignHeight()` | `int` | 获取设计高度 |
| `getDesignFitMode()` | `UIDesignFitMode` | 获取设计适配模式 |
| `getFitMatrix()` | `Mat4` | 获取适配矩阵 |


#### Boo::assetsManager - AssetsManager* 资产管理系统

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `loadAsset(const std::string& path)` | `Asset*` | 加载指定路径的资产 |
| `getAsset(const std::string& path)` | `Asset*` | 获取已加载的资产 |
| `unloadAsset(const std::string& path)` | `void` | 卸载指定资产 |



## 4. 使用示例

### 4.1 全局实例调用示例

#### 游戏核心实例

```cpp
// 打开场景
Boo::Scene* scene = new Boo::Scene("MainScene");
Boo::game->openScene(scene);

// 销毁场景
Boo::game->destroyScene();

// 获取当前场景
Boo::Scene* currentScene = Boo::game->getScene();
```

#### 事件系统

```cpp
// 监听引擎启动事件
Boo::event->on(Boo::EventEngine_Launch, []() {
    LOGI("Engine launched");
});

// 监听键盘事件
Boo::event->on(Boo::EventKey_Press, []() {
    LOGI("Key pressed");
});
```

#### 视图系统

```cpp
// 调整视口大小
Boo::view->resize(1920, 1080);

// 获取视口宽度和高度
int width = Boo::view->getWidth();
int height = Boo::view->getHeight();

// 获取设计宽度和高度
int designWidth = Boo::view->getDesignWidth();
int designHeight = Boo::view->getDesignHeight();

// 获取设计适配模式
Boo::UIDesignFitMode fitMode = Boo::view->getDesignFitMode();

// 获取适配矩阵
Boo::Mat4 fitMatrix = Boo::view->getFitMatrix();
```

#### 资产管理系统

```cpp
// 加载纹理
Boo::Asset* textureAsset = Boo::assetsManager->loadAsset("textures/player.png");

// 加载材质
Boo::Asset* materialAsset = Boo::assetsManager->loadAsset("materials/player.mat");
```

## 5. 命名空间结构

```
Boo 命名空间
├── 全局实例
│   ├── Game* game              // 游戏核心实例
│   ├── Event* event            // 事件系统
│   ├── View* view              // 视图系统
│   ├── AssetsManager* assetsManager  // 资产管理
│   └── Renderer* renderer      // 渲染系统
│
├── 场景管理
│   ├── Scene                   // 场景类
│   ├── Node                    // 基础节点类
│   ├── Node2D                  // 2D节点类
│   └── Node3D                  // 3D节点类
│
├── 数学库
│   ├── Vec2, Vec3              // 向量类
│   ├── Mat4                    // 矩阵类
│   ├── Quat                    // 四元数类
│   ├── Color                   // 颜色类
│   └── Size                    // 尺寸类
│
├── 资产系统
│   ├── Asset                   // 基础资产类
│   ├── TextureAsset            // 纹理资产
│   ├── MaterialAsset           // 材质资产
│   ├── ShaderAsset             // 着色器资产
│   ├── MeshAsset               // 网格资产
│   └── SceneAsset              // 场景资产
│
├── 组件系统
│   ├── Component               // 基础组件类
│   └── ComponentFactory        // 组件工厂
│
├── 渲染系统
│   ├── Renderer                // 渲染器
│   └── Camera                  // 相机
│
└── 工具类
    ├── FileUtil                // 文件工具
    ├── UUIDUtil                // UUID工具
    └── TimeUtil                // 时间工具
```

## 6. 引擎头文件

所有引擎功能通过统一头文件引入：

```cpp
#include "engine/boo.h"      // 包含所有核心功能
#include "engine/log.h"      // 日志系统（LOGI, LOGW, LOGE等）
```

## 7. 总结

Boo 命名空间为引擎提供了统一的访问入口，通过全局实例和核心类，开发者可以方便地使用引擎的所有功能。本文档提供了Boo命名空间的核心信息，包括全局实例、核心类、API参考和使用示例，帮助开发者快速上手引擎开发。

对于更详细的模块级文档，请参考各个模块的专门文档。