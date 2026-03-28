# Game 类使用示例

## 场景管理示例

### 创建和打开场景

```cpp
#include "engine/boo.h"

// 创建并打开场景
void openMainScene() {
    // 创建场景实例
    Boo::Scene* scene = new Boo::Scene("MainScene");
     // 打开场景（new Scene后必须紧接着调用open）
    Boo::game->openScene(scene);
    // 在场景中添加节点
    // ...
}

// 获取当前打开场景
Boo::Scene* getCurrentScene() {
    return Boo::game->getScene();
}
```

## 调度系统示例

### 重复执行的定时任务

```cpp
#include "engine/boo.h"

class GameManager {
public:
    GameManager() {
        // 每帧执行一次更新
        _updateId = Boo::game->schedule(&GameManager::update, this, 0.0f);
        
        // 每2秒执行一次定时任务
        _timerId = Boo::game->schedule(&GameManager::onTimer, this, 2.0f);
    }
    
    ~GameManager() {
        // 取消定时任务
        Boo::game->unschedule(_updateId);
        Boo::game->unschedule(_timerId);
    }
    
    void update() {
        // 每帧更新逻辑
        std::cout << "Updating game logic..." << std::endl;
    }
    
    void onTimer() {
        // 定时执行的逻辑
        std::cout << "Timer triggered!" << std::endl;
    }
    
private:
    int _updateId;
    int _timerId;
};

// 使用示例
GameManager* gameManager = new GameManager();
```

### 一次性定时任务

```cpp
#include "engine/boo.h"

class Countdown {
public:
    void startCountdown() {
        // 5秒后执行一次
        Boo::game->scheduleOnce(&Countdown::onCountdownEnd, this, 5.0f);
        std::cout << "Countdown started!" << std::endl;
    }
    
    void onCountdownEnd() {
        std::cout << "Countdown ended!" << std::endl;
    }
};

// 使用示例
Countdown countdown;
countdown.startCountdown();
```

## 视图调整示例

### 处理窗口大小变化

```cpp
#include "engine/boo.h"

// 窗口大小变化回调
void onWindowSizeChanged(GLFWwindow* window, int width, int height) {
    // 调整游戏视图大小
    Boo::game->resizeView(width, height);
    std::cout << "Window resized to: " << width << "x" << height << std::endl;
}

// 注册窗口大小变化回调（以GLFW为例）
void setupWindowSizeCallback(GLFWwindow* window) {
    glfwSetWindowSizeCallback(window, onWindowSizeChanged);
}
```

## 完整游戏示例

### 简单的2D游戏示例

```cpp
#include "engine/boo.h"

class SimpleGame {
public:
    void init() {
        // 创建场景
        _scene = new Boo::Scene("GameScene");
         // 打开场景（new Scene后必须紧接着调用open）
        Boo::game->openScene(_scene);
        
        // 创建玩家节点
        _player = new Boo::Node2D();
        _player->setPosition(Boo::Vector2(100, 100));
        _player->setName("Player");
        _scene->addChild(_player);
        
        // 为玩家添加精灵组件
        Boo::Component* playerSpriteComp = _player->addComponent("UISprite");
        _playerSprite = dynamic_cast<Boo::UISprite*>(playerSpriteComp);
        if (_playerSprite) {
            _playerSprite->setColor(1.0f, 0.0f, 0.0f, 1.0f); // 红色
            _playerSprite->setTexture("player.png");
        }
        
        // 创建敌人节点
        _enemy = new Boo::Node2D();
        _enemy->setPosition(Boo::Vector2(300, 100));
        _enemy->setName("Enemy");
        _scene->addChild(_enemy);
        
        // 为敌人添加精灵组件
        Boo::Component* enemySpriteComp = _enemy->addComponent("UISprite");
        _enemySprite = dynamic_cast<Boo::UISprite*>(enemySpriteComp);
        if (_enemySprite) {
            _enemySprite->setColor(0.0f, 0.0f, 1.0f, 1.0f); // 蓝色
            _enemySprite->setTexture("enemy.png");
        }
        
       
        
        // 设置调度任务
        _updateId = Boo::game->schedule(&SimpleGame::update, this, 0.0f);
    }
    
    void update() {
        // 更新玩家位置
        Boo::Vector2 playerPos = _player->getPosition();
        playerPos.x += 1.0f;
        if (playerPos.x > 800) {
            playerPos.x = 0;
        }
        _player->setPosition(playerPos);
        
        // 更新敌人位置
        Boo::Vector2 enemyPos = _enemy->getPosition();
        enemyPos.x -= 1.0f;
        if (enemyPos.x < 0) {
            enemyPos.x = 800;
        }
        _enemy->setPosition(enemyPos);
    }
    
private:
    Boo::Scene* _scene;
    Boo::Node2D* _player;
    Boo::Node2D* _enemy;
    Boo::UISprite* _playerSprite;
    Boo::UISprite* _enemySprite;
    int _updateId;
};

int main() {
    // 创建窗口
    Boo::Window* window = new Boo::Window();
    window->init();
    
    // 初始化游戏
    Boo::game->init(window, 800, 600, Boo::UIDesignFitMode::None);
    
    // 初始化游戏逻辑
    SimpleGame game;
    game.init();
    
    // 游戏主循环
    while (!window->shouldClose()) {
        window->pollEvents();
        Boo::game->tick();
        window->swapBuffers();
    }
    
    return 0;
}
```