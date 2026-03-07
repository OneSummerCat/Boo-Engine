# 组件系统使用示例

## 内置组件使用

### 相机组件

```cpp
#include "engine/boo.h"

void setupCamera() {
    // 创建相机节点
    Boo::Node3D* cameraNode = new Boo::Node3D();
    cameraNode->setPosition(Boo::Vector3(0, 0, 10));
    cameraNode->setName("MainCamera");
    
    // 创建相机组件
    // 相机组件会自动设置默认值并挂载到渲染器
    Boo::Camera* camera = new Boo::Camera("MainCamera", cameraNode);
}
```

### UI组件

```cpp
#include "engine/boo.h"

void setupUI() {
    // 创建UI根节点
    Boo::Node2D* uiRoot = new Boo::Node2D();
    uiRoot->setName("UIRoot");
    
    // 创建背景精灵
    Boo::UISprite* background = new Boo::UISprite("Background", uiRoot);
    background->setColor(0.1f, 0.1f, 0.1f, 1.0f); // 深灰色
    background->setTexture(Boo::AssetBuiltinTexture::Default); // 使用默认纹理
    
    // 创建玩家精灵
    Boo::UISprite* playerSprite = new Boo::UISprite("PlayerSprite", uiRoot);
    playerSprite->setPosition(Boo::Vector2(100, 100));
    playerSprite->setColor(1.0f, 0.0f, 0.0f, 1.0f); // 红色
    playerSprite->setTexture(Boo::AssetBuiltinTexture::Default); // 使用默认纹理
    
    // 创建文本
    Boo::UIText* scoreText = new Boo::UIText("ScoreText", uiRoot);
    scoreText->setPosition(Boo::Vector2(700, 500));
    scoreText->setText("Score: 0");
    scoreText->setFontSize(24);
    scoreText->setColor(1.0f, 1.0f, 1.0f, 1.0f); // 白色
    
    // 创建遮罩
    Boo::UIMask* mask = new Boo::UIMask("Mask", uiRoot);
    mask->setPosition(Boo::Vector2(300, 200));
    mask->setAlpha(0.5f);
}
```


