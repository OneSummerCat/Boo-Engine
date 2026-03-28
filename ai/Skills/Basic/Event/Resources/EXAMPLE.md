# 事件系统示例

## 示例 1: 基本事件订阅与发布

```cpp
#include "engine/boo.h"

class MyGame {
public:
    void init() {
        // 订阅玩家跳跃事件
        Boo::event->subscribe("PlayerJump", [](const Boo::EventData& data) {
            float jumpHeight = data.getFloat("jumpHeight", 0.0f);
            LOGI("玩家跳跃了！跳跃高度: %.2f", jumpHeight);
        });
        
        // 订阅玩家得分事件
        Boo::event->subscribe("PlayerScore", [](const Boo::EventData& data) {
            int score = data.getInt("score", 0);
            std::string playerName = data.getString("playerName", "Unknown");
            LOGI("%s 得分: %d", playerName.c_str(), score);
        });
    }
    
    void update(float deltaTime) {
        // 模拟玩家跳跃
        if (Boo::game->input->getKey(Boo::KeyCode::SPACE)) {
            Boo::EventData jumpData;
            jumpData.setFloat("jumpHeight", 10.0f);
            Boo::event->publish("PlayerJump", jumpData);
        }
        
        // 模拟玩家得分
        if (Boo::game->input->getKey(Boo::KeyCode::E)) {
            Boo::EventData scoreData;
            scoreData.setInt("score", 100);
            scoreData.setString("playerName", "Player1");
            Boo::event->publish("PlayerScore", scoreData);
        }
    }
};
```

## 示例 2: 带优先级的事件订阅

```cpp
#include "engine/boo.h"

class MyGame {
public:
    void init() {
        // 高优先级订阅者（优先级值越小，优先级越高）
        Boo::event->subscribe("GameStart", 1, [](const Boo::EventData& data) {
            LOGI("高优先级订阅者: 游戏开始前的准备工作");
        });
        
        // 普通优先级订阅者
        Boo::event->subscribe("GameStart", 5, [](const Boo::EventData& data) {
            LOGI("普通优先级订阅者: 游戏开始");
        });
        
        // 低优先级订阅者
        Boo::event->subscribe("GameStart", 10, [](const Boo::EventData& data) {
            LOGI("低优先级订阅者: 游戏开始后的清理工作");
        });
    }
    
    void startGame() {
        // 发布游戏开始事件
        Boo::EventData gameStartData;
        gameStartData.setString("level", "1");
        Boo::event->publish("GameStart", gameStartData);
    }
};
```

## 示例 3: 取消事件订阅

```cpp
#include "engine/boo.h"

class MyGame {
private:
    // 存储回调函数的引用
    std::function<void(const Boo::EventData&)> onPlayerJump;
    
public:
    void init() {
        // 创建回调函数
        onPlayerJump = [](const Boo::EventData& data) {
            LOGI("玩家跳跃了！");
        };
        
        // 订阅事件
        Boo::event->subscribe("PlayerJump", onPlayerJump);
    }
    
    void cleanup() {
        // 取消订阅事件
        Boo::event->unsubscribe("PlayerJump", onPlayerJump);
        LOGI("已取消玩家跳跃事件的订阅");
    }
};
```