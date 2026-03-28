# Boo Engine 使用注意事项(AI 笔记)

## 重要纠正性问题

### 1. 场景根节点创建
- **问题**：不要手动调用 `createRoot2D()` 和 `createRoot3D()` 接口
- **原因**：引擎内部已经自动处理根节点的创建
- **正确做法**：直接使用 `scene->getRoot2D()` 或 `scene->getRoot3D()` 获取根节点

### 2. 场景打开顺序
- **问题**：不要在设置完所有场景内容后才调用 `openScene()`
- **原因**：场景需要在创建后立即打开，以确保内部初始化正确
- **正确做法**：在 `new Scene()` 后立即调用 `Boo::game->openScene(scene)`

### 3. 2D 渲染要求
- **问题**：2D 场景中没有相机组件会导致图片无法渲染
- **原因**：渲染系统需要相机来确定视图和投影
- **正确做法**：在 `root2D` 下创建一个带有 `Camera` 组件的节点

### 4. 2D 节点尺寸设置
- **问题**：使用 `setScale()` 设置 2D 节点的尺寸
- **原因**：2D 节点的尺寸应该通过宽高设置，而不是缩放
- **正确做法**：使用 `setSize(width, height)` 方法设置 2D 节点的尺寸

### 5. 屏幕中心坐标
- **问题**：使用 `width/2, height/2` 计算屏幕中心
- **原因**：游戏引擎的锚点 (0, 0) 就是屏幕中心
- **正确做法**：直接使用 `(0, 0, 0)` 表示屏幕中心

### 6. UISprite 图片设置
- **问题**：使用 `setProperty("image", path)` 设置图片
- **原因**：UISprite 组件应该使用 `setTexture()` 方法设置图片
- **正确做法**：使用 `sprite->setTexture("path/to/image.png")` 设置图片路径
- **路径说明**：
  - `setTexture()`：图片路径是相对于 assets 目录的相对路径，**不需要包含assets**
  - `AssetsManager::loadTexture()`：需要包含 assets
- **示例**：
  - setTexture 正确：`textures/player.png`
  - setTexture 错误：`assets/textures/player.png`
  - AssetsManager：`assets/textures/player.png`
- **默认图片**：使用 `Boo::AssetBuiltinTexture::Default` 作为默认贴图
- **重要**：使用图片前，必须先调用 `Boo::assetsManager->loadAsset()` 加载图片，否则获取不到
- **加载方式**：`loadAsset()` 会根据文件后缀自动确定加载方式，比使用特定的 `loadTexture()` 更通用

### 7. 全屏设置
- **问题**：使用 `view->getWidth()` 和 `view->getHeight()` 获取窗口尺寸
- **原因**：全屏应该使用设计分辨率，view下的宽高是实际窗口尺寸
- **正确做法**：使用 `Boo::view->getDesignWidth()` 和 `Boo::view->getDesignHeight()` 获取设计分辨率
- **示例代码**：
  ```cpp
  int designWidth = Boo::view->getDesignWidth();
  int designHeight = Boo::view->getDesignHeight();
  node->setSize(designWidth, designHeight);
  ```

### 8. 组件类型转换
- **问题**：直接使用 `addComponent()` 的返回值调用组件特定方法
- **原因**：`addComponent()` 返回的是通用组件类型，需要转换为具体类型
- **正确做法**：使用 `dynamic_cast` 将组件转换为具体类型后再调用方法
- **示例代码**：
  ```cpp
  auto spriteComponent = node->addComponent("UISprite");
  auto sprite = dynamic_cast<Boo::UISprite*>(spriteComponent);
  if (sprite) {
      sprite->setTexture("claw.png");
  }
  ```

### 8. 场景和节点使用
- **问题**：继承 `Scene` 和 `Node` 类
- **原因**：没有特殊需求时，不需要继承这些类，直接使用实例即可
- **正确做法**：使用 `new Scene()` 创建场景实例，使用 `new Node2D()` 或 `new Node3D()` 创建节点实例
- **示例代码**：
  ```cpp
  // 创建场景
  auto scene = new Boo::Scene("SceneName");
  Boo::game->openScene(scene);
  
  // 创建节点
  auto node = new Boo::Node2D("NodeName");
  ```

### 9. 头文件设计
- **问题**：在头文件中使用 `void*` 类型来声明指针
- **原因**：`void*` 类型不安全，缺乏类型检查，容易导致运行时错误
- **正确做法**：使用向前声明来减少依赖，提高类型安全性
- **示例代码**：
  ```cpp
  // 推荐的头文件设计
  namespace Boo {
      class Scene;
      class Node2D;
  }
  
  class PixelLobster {
  private:
      Boo::Scene* m_scene;
      
  public:
      PixelLobster();
      ~PixelLobster();
      
      void createScene();
  };
  ```

### 10. 自定义组件注册
- **问题**：自定义组件无法被 `addComponent()` 识别
- **原因**：没有使用 `REGISTER_COMPONENT` 宏进行注册
- **正确做法**：引入头文件并使用 `REGISTER_COMPONENT` 宏注册组件
- **必须引入的头文件**：
  ```cpp
  #include "engine/core/component/component-register.h"
  ```
- **示例代码**：
  ```cpp
  #pragma once
  #include "engine/core/component/component.h"
  #include "engine/core/component/component-register.h"
  
  class GobangGame : public Boo::Component
  {
  public:
      GobangGame(std::string name, Boo::Node *node, std::string uuid = "");
      void Awake() override;
      void Enable() override;
      void Update(float deltaTime) override;
      void LateUpdate(float deltaTime) override;
      void Disable() override;
      void destroy() override;
      ~GobangGame() override;
  };
  
  REGISTER_COMPONENT(GobangGame, "Gobang Game Manager Component")
  ```

### 11. 组件内节点操作
- **问题**：在组件中使用 `root2D` 或 `getScene()->getRoot2D()` 获取根节点
- **原因**：组件应该操作自己的节点，而不是操作根节点
- **正确做法**：使用 `getNode()` 获取组件所在的节点，所有子节点都添加到该节点下
- **示例代码**：
  ```cpp
  void MyComponent::Awake() {
      auto node = getNode();
      
      auto child = new Boo::Node2D("Child");
      node->addChild(child);
      child->setPosition(100, 100, 0);
  }
  ```

## 其他注意事项

### 4. 组件添加
- 为节点添加组件时，使用 `node->addComponent("ComponentName")`
- 常用组件包括：`UISprite`、`Camera`、`UIText` 等

### 5. 坐标系统
- 2D 节点的位置基于屏幕坐标系
- **重要**：游戏引擎的锚点 (0, 0) 就是屏幕中心，不需要计算宽高的一半
- 坐标示例：(0, 0, 0) 表示屏幕中心

### 6. 资源管理
- 图片资源需要通过 `AssetsManager` 加载
- 使用 `UISprite` 组件时，可以设置图片路径

### 7. 事件系统
- 使用 `Boo::event->on()` 注册事件回调
- 常用事件：`Boo::EventEngine_Launch`、`Boo::EventEngine_Tick` 等

### 8. 生命周期管理
- 游戏对象的创建和销毁需要正确管理
- 使用智能指针或在适当时候手动清理资源

## 示例代码结构

```cpp
// 正确的场景创建和初始化流程
Scene *scene = new Scene("SceneName");
Boo::game->openScene(scene);

// 获取根节点
auto root2D = scene->getRoot2D();

// 创建相机
auto cameraNode = new Node2D("Camera");
root2D->addChild(cameraNode);
cameraNode->addComponent("Camera");

// 创建精灵
auto sprite = new Node2D("Sprite");
root2D->addChild(sprite);
sprite->addComponent("UISprite");

// 设置精灵大小（使用setSize而不是setScale）
sprite->setSize(100, 100);

// 设置精灵位置到屏幕中心（锚点(0,0)就是屏幕中心）
sprite->setPosition(0, 0, 0);
```
