# Game 类使用示例

## 基本初始化与主循环

### 窗口平台初始化

```cpp
#include "engine/boo.h"

int main() {
    // 创建窗口
    Window* window = new Window();
    window->init();
    
    // 初始化游戏，设置设计分辨率和适配模式
    // 设计分辨率：1920x1080，适配模式：None
    Boo::game->init(window, 1920, 1080, Boo::UIDesignFitMode::None);
    
    // 游戏主循环
    while (!window->shouldClose()) {
        // 处理窗口事件
        window->pollEvents();
        
        // 游戏逻辑更新
        Boo::game->tick();
        
        // 交换缓冲区
        window->swapBuffers();
    }
    
    return 0;
}
```

### Android平台初始化

```cpp
#include "engine/boo.h"

// Android平台初始化函数
void android_main(struct android_app* state) {
    // 初始化Android应用
    Android* android = new Android(state);
    android->init();
    
    // 初始化游戏，设置设计分辨率和适配模式
    // 设计分辨率：1280x720，适配模式：None
    Boo::game->init(android, 1280, 720, Boo::UIDesignFitMode::None);
    
    // 游戏主循环
    while (android->isRunning()) {
        // 处理Android事件
        android->pollEvents();
        
        // 游戏逻辑更新
        Boo::game->tick();
    }
}
```

## 场景管理示例

### 创建和切换场景

```cpp
#include "engine/boo.h"

// 创建场景
Boo::Scene* createMainScene() {
    // 创建场景实例
    Boo::Scene* scene = new Boo::Scene("MainScene");
    
    // 在场景中添加节点
    // ...
    
    return scene;
}

// 切换场景
void switchToMainScene() {
    // 创建主场景
    Boo::Scene* mainScene = createMainScene();
    
    // 打开场景
    Boo::game->openScene(mainScene);
}

// 获取当前场景
Boo::Scene* getCurrentScene() {
    return Boo::game->getScene();
}

// 销毁当前场景
void destroyCurrentScene() {
    Boo::game->destroyScene();
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

## 输入处理示例

### 鼠标事件处理

```cpp
#include "engine/boo.h"

// 鼠标按钮事件回调
void onMouseButton(int button, int action, int mods) {
    // 更新鼠标状态
    Boo::game->updateMouseState(button, action, mods);
    
    // 处理鼠标事件
    if (action == 1) { // 按下
        switch (button) {
            case 0: // 左键
                std::cout << "Left mouse button pressed" << std::endl;
                break;
            case 1: // 右键
                std::cout << "Right mouse button pressed" << std::endl;
                break;
            case 2: // 中键
                std::cout << "Middle mouse button pressed" << std::endl;
                break;
        }
    }
}

// 鼠标移动事件回调
void onMouseMove(double xpos, double ypos) {
    // 更新鼠标位置
    Boo::game->updateMousePos(xpos, ypos);
    
    // 处理鼠标移动
    std::cout << "Mouse moved to: " << xpos << ", " << ypos << std::endl;
}

// 注册鼠标事件回调（以GLFW为例）
void setupMouseCallbacks(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, onMouseButton);
    glfwSetCursorPosCallback(window, onMouseMove);
}
```

### 键盘事件处理

```cpp
#include "engine/boo.h"

// 键盘事件回调
void onKey(int key, int scancode, int action, int mods) {
    // 更新键盘状态
    Boo::game->updateKeyState(key, scancode, action, mods);
    
    // 处理键盘事件
    if (action == 1) { // 按下
        switch (key) {
            case GLFW_KEY_W:
                std::cout << "W key pressed" << std::endl;
                break;
            case GLFW_KEY_A:
                std::cout << "A key pressed" << std::endl;
                break;
            case GLFW_KEY_S:
                std::cout << "S key pressed" << std::endl;
                break;
            case GLFW_KEY_D:
                std::cout << "D key pressed" << std::endl;
                break;
        }
    }
}

// 注册键盘事件回调（以GLFW为例）
void setupKeyCallback(GLFWwindow* window) {
    glfwSetKeyCallback(window, onKey);
}
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
        
        // 创建玩家节点
        _player = new Boo::Node2D();
        _player->setPosition(Boo::Vector2(100, 100));
        _player->setName("Player");
        _scene->addChild(_player);
        
        // 创建敌人节点
        _enemy = new Boo::Node2D();
        _enemy->setPosition(Boo::Vector2(300, 100));
        _enemy->setName("Enemy");
        _scene->addChild(_enemy);
        
        // 打开场景
        Boo::game->openScene(_scene);
        
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
    int _updateId;
};

int main() {
    // 创建窗口
    Window* window = new Window();
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