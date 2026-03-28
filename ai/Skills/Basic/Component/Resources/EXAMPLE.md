# 组件系统使用示例

## 内置组件使用

### 相机组件

```cpp
#include "engine/boo.h"

void setupCamera() {
    Boo::Node3D* cameraNode = new Boo::Node3D();
    cameraNode->setPosition(Boo::Vector3(0, 0, 10));
    cameraNode->setName("MainCamera");

    Boo::Component* cameraComp = cameraNode->addComponent("Camera");
    Boo::Camera* camera = dynamic_cast<Boo::Camera*>(cameraComp);
    if (camera) {
    }
}
```

### UI组件

```cpp
#include "engine/boo.h"

void setupUI() {
    Boo::Node2D* uiRoot = new Boo::Node2D();
    uiRoot->setName("UIRoot");

    Boo::Component* backgroundComp = uiRoot->addComponent("UISprite");
    Boo::UISprite* background = dynamic_cast<Boo::UISprite*>(backgroundComp);
    if (background) {
        background->setColor(0.1f, 0.1f, 0.1f, 1.0f);
        background->setTexture(Boo::AssetBuiltinTexture::Default);
    }

    Boo::Component* playerSpriteComp = uiRoot->addComponent("UISprite");
    Boo::UISprite* playerSprite = dynamic_cast<Boo::UISprite*>(playerSpriteComp);
    if (playerSprite) {
        playerSprite->setPosition(Boo::Vector2(100, 100));
        playerSprite->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        playerSprite->setTexture(Boo::AssetBuiltinTexture::Default);
    }

    Boo::Component* scoreTextComp = uiRoot->addComponent("UIText");
    Boo::UIText* scoreText = dynamic_cast<Boo::UIText*>(scoreTextComp);
    if (scoreText) {
        scoreText->setPosition(Boo::Vector2(700, 500));
        scoreText->setText("Score: 0");
        scoreText->setSize(24);
        scoreText->setLineHeight(30);
        scoreText->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    Boo::Component* maskComp = uiRoot->addComponent("UIMask");
    Boo::UIMask* mask = dynamic_cast<Boo::UIMask*>(maskComp);
    if (mask) {
        mask->setPosition(Boo::Vector2(300, 200));
        mask->setAlpha(0.5f);
    }
}
```

## 创建自定义组件

### 1. 定义自定义组件

#### 头文件 (my-component.h)

```cpp
#pragma once
#include "engine/core/component/component.h"
#include "engine/core/component/component-register.h"

class GameLogic : public Boo::Component
{
private:
    int score;
    float timer;

public:
    GameLogic(std::string name, Boo::Node *node, std::string uuid = "");

    void Awake() override;
    void Enable() override;
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Disable() override;
    void destroy() override;
    ~GameLogic() override;

    void addScore(int value);
    int getScore() const;
};

REGISTER_COMPONENT(GameLogic, "Game Logic Component")
```

#### 源文件 (my-component.cpp)

```cpp
#include "my-component.h"

GameLogic::GameLogic(std::string name, Boo::Node *node, std::string uuid)
    : Boo::Component(name, node, uuid), score(0), timer(0.0f) {
}

void GameLogic::Awake() {
    auto node = getNode();
    
    auto sprite = new Boo::Node2D("Background");
    node->addChild(sprite);
    sprite->setPosition(100, 100, 0);
    sprite->setSize(200, 200);
    
    auto spriteComp = dynamic_cast<Boo::UISprite*>(sprite->addComponent("UISprite"));
    if (spriteComp) {
        spriteComp->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        spriteComp->setTexture(Boo::AssetBuiltinTexture::Default);
    }
}

void GameLogic::Enable() {
}

void GameLogic::Update(float deltaTime) {
    timer += deltaTime;
    if (timer >= 1.0f) {
        timer = 0.0f;
        addScore(10);
    }
}

void GameLogic::LateUpdate(float deltaTime) {
}

void GameLogic::Disable() {
}

void GameLogic::destroy() {
    Boo::Component::destroy();
}

void GameLogic::addScore(int value) {
    score += value;
}

int GameLogic::getScore() const {
    return score;
}

GameLogic::~GameLogic() {
}
```

### 2. 使用自定义组件

```cpp
#include "my-component.h"

void setupGameLogic() {
    Boo::Node2D* gameNode = new Boo::Node2D("GameManager");
    root2D->addChild(gameNode);

    Boo::Component* comp = gameNode->addComponent("GameLogic");
    GameLogic* gameLogic = dynamic_cast<GameLogic*>(comp);
    if (gameLogic) {
        gameLogic->addScore(100);
    }
}
```

**注意**：在组件内部创建子节点时，应使用 `getNode()` 获取组件所在的节点，而不是使用 `root2D`。这样组件更加独立和可复用。
