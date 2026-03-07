# Boo 命名空间使用示例

## 全局实例调用示例

### 1. Boo::game - 游戏核心实例

```cpp
// 打开场景
Boo::Scene* scene = new Boo::Scene("MainScene");
Boo::game->openScene(scene);

// 销毁场景
Boo::game->destroyScene();

// 获取当前场景
Boo::Scene* currentScene = Boo::game->getScene();
```

### 2. Boo::event - 事件系统

```cpp
// 监听引擎启动事件
Boo::event->on(Boo::EventEngine_Launch, []() {
    LOGI("Engine launched");
});

// 监听键盘事件
Boo::event->on(Boo::EventKey_Press, []() {
    LOGI("Key pressed");
});

// 监听窗口大小变化事件
Boo::event->on(Boo::EventWindow_Resize, []() {
    LOGI("Window resized");
});

// 触发自定义事件
Boo::event->trigger(Boo::EventCustom);
```

### 3. Boo::view - 视图系统

```cpp
// 调整视口大小
Boo::view->resize(1920, 1080);

// 获取视口宽度和高度
int width = Boo::view->getWidth();
int height = Boo::view->getHeight();
LOGI("View dimensions: %dx%d", width, height);

// 获取设计宽度和高度
int designWidth = Boo::view->getDesignWidth();
int designHeight = Boo::view->getDesignHeight();
LOGI("Design dimensions: %dx%d", designWidth, designHeight);

// 获取设计适配模式
Boo::UIDesignFitMode fitMode = Boo::view->getDesignFitMode();
LOGI("Design fit mode: %d", (int)fitMode);

// 获取适配矩阵
Boo::Mat4 fitMatrix = Boo::view->getFitMatrix();
LOGI("Fit matrix obtained");
```

### 4. Boo::assetsManager - 资产管理系统

```cpp
// 加载纹理
Boo::Asset* textureAsset = Boo::assetsManager->loadAsset("textures/player.png");

// 加载材质
Boo::Asset* materialAsset = Boo::assetsManager->loadAsset("materials/player.mat");

// 加载着色器
Boo::Asset* shaderAsset = Boo::assetsManager->loadAsset("shaders/default.vert");

// 获取已加载的资产
Boo::Asset* existingAsset = Boo::assetsManager->getAsset("textures/player.png");

// 卸载资产
Boo::assetsManager->unloadAsset("textures/player.png");
```
