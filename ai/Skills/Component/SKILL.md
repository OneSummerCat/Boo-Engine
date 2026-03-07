# 组件系统

组件系统是 Boo 引擎的核心功能之一，它允许开发者通过组合不同的组件来构建复杂的游戏对象。组件系统提供了一种灵活、可扩展的方式来为游戏对象添加功能。

## 核心功能

### 组件基础
- **组件生命周期**：每个组件都有完整的生命周期，包括初始化、更新、禁用和销毁
- **组件状态管理**：支持启用/禁用组件，以及在层级中的激活状态管理

### 组件工厂
- **组件创建**：通过组件名称动态创建组件实例
- **组件管理**：管理已注册的组件类型和描述

### 内置组件
- **相机组件**：负责场景的渲染，支持正交和透视投影
- **UI组件**：包括精灵、文本、遮罩等UI元素
- **渲染组件**：处理游戏对象的渲染

## 主要接口

### 组件基类
```cpp
// 组件构造函数
Component(std::string name, Node *node, std::string uuid = "");

// 生命周期方法
virtual void Awake() = 0;
virtual void Enable() = 0;
virtual void Update(float deltaTime) = 0;
virtual void LateUpdate(float deltaTime) = 0;
virtual void Disable() = 0;
virtual void destroy();

// 状态管理
bool isEnabled();
void setEnabled(bool enabled);
const bool isEnabledInHierarchy();

// 属性访问
std::string getName();
Node *getNode();
NodeLayer layer();
const std::string getUuid();
```

### 组件工厂
```cpp
// 获取组件工厂实例
static ComponentFactory &getInstance();

// 创建组件
Boo::Component *createComponent(const std::string &name, Boo::Node *node, std::string uuid);

// 获取组件描述
std::string getComponentDescription(const std::string &name);

// 检查组件是否注册
bool isComponentRegistered(const std::string &typeName) const;
```

### 内置组件

#### 相机组件
```cpp
// 构造函数
Camera(std::string name, Node *node, std::string uuid = "");

// 生命周期方法
void Awake() override;
void Enable() override;
void Update(float deltaTime) override;
void LateUpdate(float deltaTime) override;
virtual void Render();
void Disable() override;
void destroy() override;

// 属性设置
void setIsOnScreen(bool isOnScreen);
void setPriority(int priority);
void setGroupIDs(int groupIDs);
void addGroupID(int groupID);
void updateViewSize();

// 属性获取
bool getIsOnScreen();
int getPriority();
int getGroupIDs();
```

#### UI精灵组件
```cpp
// 构造函数
UISprite(std::string name, Node *node, std::string uuid = "");

// 生命周期方法
void Awake() override;
void Enable() override;
void Update(float deltaTime) override;
void LateUpdate(float deltaTime) override;
void Render(Camera *camera) override;
void Disable() override;
void destroy() override;

// 属性设置
void setColor(Color &color);
void setColor(std::string color);
void setColor(float r, float g, float b, float a);
void setAlpha(float alpha);
void setTexture(std::string texture);
void setTexture(TextureAsset *texture);
void setRenderTexture();
void setMaterial(std::string material);
void setMaterial(MaterialAsset *material);
```

## 使用示例

### 使用内置组件

```cpp
#include "engine/boo.h"

void setupComponents() {
    // 创建节点
    Boo::Node2D* node = new Boo::Node2D();
    node->setPosition(Boo::Vector2(100, 100));
    
    // 添加相机组件
    Boo::Camera* camera = new Boo::Camera("MainCamera", node);
    camera->setIsOnScreen(true);
    
    // 添加UI精灵组件
    Boo::UISprite* sprite = new Boo::UISprite("PlayerSprite", node);
    sprite->setColor(1.0f, 0.0f, 0.0f, 1.0f); // 红色
    sprite->setTexture("player.png");
    
    // 启用/禁用组件
    sprite->setEnabled(false); // 禁用精灵组件
}
```

## 组件生命周期

组件的生命周期包括以下阶段：

1. **初始化**：当组件被创建时，`Awake()` 方法被调用
2. **启用**：当组件被启用时，`Enable()` 方法被调用
3. **更新**：每一帧，`Update()` 方法被调用
4. **晚更新**：每一帧，在所有 `Update()` 方法执行后，`LateUpdate()` 方法被调用
5. **禁用**：当组件被禁用时，`Disable()` 方法被调用
6. **销毁**：当组件被销毁时，`destroy()` 方法被调用

## 组件系统架构

### 核心组件
- **Component**：所有组件的基类，定义了组件的基本接口和生命周期
- **ComponentFactory**：负责组件的创建和管理

### 内置组件
- **Camera**：相机组件，负责场景渲染
- **UIRenderer**：UI渲染器基类
- **UISprite**：UI精灵组件
- **UIText**：UI文本组件
- **UIMask**：UI遮罩组件

### 渲染系统集成
- 组件系统与渲染系统紧密集成，渲染组件负责将游戏对象渲染到屏幕上
- 相机组件作为特殊的渲染组件，负责场景的可见性和渲染目标

## 最佳实践

1. **组件职责单一**：每个组件应该只负责一个特定的功能
2. **组件通信**：组件之间通过节点或事件系统进行通信
3. **组件复用**：设计可复用的组件，提高代码的可维护性
4. **组件性能**：注意组件的性能开销，避免在 `Update()` 方法中执行复杂计算

## 常见问题

- **组件未被更新**：检查组件是否启用，以及所在节点是否激活
- **组件创建失败**：检查组件是否已注册，以及构造函数参数是否正确
- **组件生命周期异常**：确保正确实现所有生命周期方法
- **组件性能问题**：优化 `Update()` 方法，避免不必要的计算

## 总结

组件系统是 Boo 引擎的核心功能之一，它提供了一种灵活、可扩展的方式来构建游戏对象。通过组件系统，开发者可以：

- 构建复杂的游戏对象，通过组合不同的组件
- 实现代码的模块化和复用
- 管理游戏对象的生命周期
- 与渲染系统和其他系统集成

组件系统的设计理念是"组合优于继承"，通过组合不同的组件来实现复杂的功能，而不是通过继承来扩展基类。这种设计使得代码更加灵活、可维护，并且易于扩展。