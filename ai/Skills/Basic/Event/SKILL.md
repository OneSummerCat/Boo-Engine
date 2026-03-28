---
name: "event"
description: "事件系统，处理引擎和游戏事件的发布订阅。"
---

# 事件系统

事件系统负责处理引擎和游戏事件的发布订阅，允许不同系统之间进行松耦合的通信。通过事件系统，你可以发布事件并订阅感兴趣的事件。

## 核心功能

### 事件管理
- **事件订阅**：订阅感兴趣的事件并指定处理函数
- **事件发布**：发布自定义事件并传递事件数据
- **事件参数**：支持传递各种类型的事件参数
- **事件优先级**：管理事件处理的优先级

## 主要接口

### 事件订阅
```cpp
// 订阅事件
int subscribe(const std::string& eventName, std::function<void(const EventData&)> callback);

// 订阅事件（带优先级）
int subscribe(const std::string& eventName, std::function<void(const EventData&)> callback, int priority);

// 取消订阅
void unsubscribe(const std::string& eventName, int subscriptionId);
```

### 事件发布
```cpp
// 发布事件
void publish(const std::string& eventName, const EventData& data);

// 发布事件（无参数）
void publish(const std::string& eventName);
```

### 事件数据
```cpp
// EventData 类
class EventData {
public:
    // 设置参数
    void setInt(const std::string& key, int value);
    void setFloat(const std::string& key, float value);
    void setBool(const std::string& key, bool value);
    void setString(const std::string& key, const std::string& value);
    void setObject(const std::string& key, void* value);
    
    // 获取参数
    int getInt(const std::string& key, int defaultValue = 0) const;
    float getFloat(const std::string& key, float defaultValue = 0.0f) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    void* getObject(const std::string& key, void* defaultValue = nullptr) const;
};
```

## 使用示例

### 基本用法

```cpp
#include "engine/boo.h"

void setupEvents() {
    // 订阅事件
    int subscriptionId = Boo::event->subscribe("PlayerJump", [](const Boo::EventData& data) {
        // 处理玩家跳跃事件
        float jumpHeight = data.getFloat("jumpHeight", 0.0f);
        LOGI("玩家跳跃了！高度：%.2f", jumpHeight);
    });
    
    // 发布事件
    Boo::EventData jumpData;
    jumpData.setFloat("jumpHeight", 10.0f);
    jumpData.setString("playerName", "Player1");
    Boo::event->publish("PlayerJump", jumpData);
    
    // 取消订阅
    Boo::event->unsubscribe("PlayerJump", subscriptionId);
}
```

### 带优先级的订阅

```cpp
// 高优先级订阅（优先级值越小，优先级越高）
Boo::event->subscribe("GameStart", [](const Boo::EventData& data) {
    LOGI("高优先级处理游戏开始事件");
}, 1);

// 低优先级订阅
Boo::event->subscribe("GameStart", [](const Boo::EventData& data) {
    LOGI("低优先级处理游戏开始事件");
}, 10);

// 发布事件（高优先级的处理函数会先执行）
Boo::event->publish("GameStart");
```

## 事件系统架构

### 核心组件
- **Event**：事件系统的核心类，负责事件的发布和订阅
- **EventData**：事件数据类，用于传递事件参数

### 事件流程
1. **订阅阶段**：系统或游戏代码通过 `subscribe` 方法订阅感兴趣的事件
2. **发布阶段**：当事件发生时，通过 `publish` 方法发布事件
3. **处理阶段**：所有订阅该事件的处理函数按照优先级顺序执行

## 最佳实践

1. **事件命名**：使用清晰、描述性的事件名称，如 "PlayerJump"、"GameStart" 等
2. **事件数据**：只传递必要的事件数据，避免传递过多数据
3. **事件优先级**：合理设置事件处理的优先级，确保重要的处理逻辑优先执行
4. **取消订阅**：不再需要的事件订阅应及时取消，避免内存泄漏
5. **事件粒度**：事件粒度应适中，避免过于细粒度或过于粗粒度的事件

## 常见问题

- **事件未被触发**：检查事件名称是否正确，以及是否正确发布了事件
- **事件处理函数未执行**：检查订阅是否成功，以及事件名称是否匹配
- **事件数据获取错误**：确保使用正确的键名和数据类型获取事件数据
- **内存泄漏**：确保及时取消不再需要的事件订阅

## 总结

事件系统是 Boo 引擎中实现系统间通信的重要机制，它提供了一种松耦合的方式来处理引擎和游戏事件。通过事件系统，你可以：

- 实现系统间的解耦，提高代码的可维护性
- 处理游戏中的各种事件，如玩家输入、游戏状态变化等
- 传递事件相关的数据，支持复杂的事件处理逻辑
- 管理事件处理的优先级，确保重要的逻辑优先执行

事件系统的设计理念是"发布-订阅"模式，通过这种模式，不同系统之间可以通过事件进行通信，而不需要直接依赖彼此，从而提高代码的模块化和可扩展性。