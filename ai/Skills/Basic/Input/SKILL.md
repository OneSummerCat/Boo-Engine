---
name: "input"
description: "输入系统，处理键盘、鼠标和触摸输入。"
---

# 输入系统

输入系统负责处理玩家的各种输入，包括键盘、鼠标和触摸输入。通过输入系统，你可以检测按键状态、鼠标位置和触摸事件。

## 核心功能

### 键盘输入
- **按键状态检测**：检测键盘按键的按下和释放状态
- **按键映射**：支持自定义按键映射
- **键盘事件**：处理键盘按下和释放事件

### 鼠标输入
- **鼠标位置**：获取鼠标在屏幕和游戏世界中的位置
- **鼠标按键**：检测鼠标按键的按下和释放状态
- **鼠标移动**：处理鼠标移动事件

### 触摸输入
- **触摸位置**：获取触摸点的位置
- **触摸状态**：检测触摸的开始、移动和结束
- **多点触摸**：支持多点触摸输入

## 主要接口

### 键盘输入
```cpp
// 检测按键是否按下
bool getKey(KeyCode key);

// 检测按键是否刚刚按下（只在按下的第一帧返回true）
bool getKeyDown(KeyCode key);

// 检测按键是否刚刚释放（只在释放的第一帧返回true）
bool getKeyUp(KeyCode key);

// 获取按键的持续按下时间
float getKeyDuration(KeyCode key);
```

### 鼠标输入
```cpp
// 获取鼠标位置（屏幕坐标）
Vec2 getMousePosition();

// 获取鼠标位置（游戏世界坐标）
Vec2 getMouseWorldPosition();

// 检测鼠标按键是否按下
bool getMouseButton(MouseButton button);

// 检测鼠标按键是否刚刚按下
bool getMouseButtonDown(MouseButton button);

// 检测鼠标按键是否刚刚释放
bool getMouseButtonUp(MouseButton button);

// 获取鼠标滚轮 delta
float getMouseScrollDelta();
```

### 触摸输入
```cpp
// 获取触摸点数量
int getTouchCount();

// 获取指定触摸点的位置
Vec2 getTouchPosition(int index);

// 获取指定触摸点的世界位置
Vec2 getTouchWorldPosition(int index);

// 检测触摸是否刚刚开始
bool getTouchDown(int index);

// 检测触摸是否刚刚结束
bool getTouchUp(int index);

// 获取触摸的移动距离
Vec2 getTouchDelta(int index);
```

### 输入事件
```cpp
// 更新键盘状态
void updateKeyState(int key, int scancode, int action, int mods);

// 更新鼠标状态
void updateMouseState(int button, int action, int mods);

// 更新鼠标位置
void updateMousePos(double xpos, double ypos);

// 更新鼠标滚轮
void updateMouseScroll(double xoffset, double yoffset);

// 更新触摸状态
void updateTouchState(int touchId, int action, double xpos, double ypos);
```

## 枚举类型

### 按键码
```cpp
enum class KeyCode {
    // 字母键
    A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    
    // 数字键
    NUM_0 = 48, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
    
    // 功能键
    ESCAPE = 256, ENTER, TAB, BACKSPACE, INSERT, DELETE, RIGHT, LEFT, DOWN, UP,
    PAGE_UP, PAGE_DOWN, HOME, END,
    
    // 功能键 F1-F12
    F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    
    // 特殊键
    SPACE = 32, SHIFT = 340, CONTROL = 341, ALT = 342,
};
```

### 鼠标按键
```cpp
enum class MouseButton {
    LEFT = 0,    // 鼠标左键
    RIGHT = 1,   // 鼠标右键
    MIDDLE = 2,  // 鼠标中键
};
```

## 使用示例

### 基本输入检测

```cpp
#include "engine/boo.h"

void handleInput() {
    // 检测键盘输入
    if (Boo::game->input->getKey(Boo::KeyCode::SPACE)) {
        // 空格键被按下，执行跳跃
        LOGI("跳跃！");
    }
    
    if (Boo::game->input->getKeyDown(Boo::KeyCode::E)) {
        // E键刚刚被按下，执行交互
        LOGI("交互！");
    }
    
    // 检测鼠标输入
    if (Boo::game->input->getMouseButton(Boo::MouseButton::LEFT)) {
        // 鼠标左键被按下，执行攻击
        LOGI("攻击！");
    }
    
    // 获取鼠标位置
    Boo::Vec2 mousePos = Boo::game->input->getMousePosition();
    LOGI("鼠标位置: (%.2f, %.2f)", mousePos.x, mousePos.y);
    
    // 获取鼠标世界位置
    Boo::Vec2 worldPos = Boo::game->input->getMouseWorldPosition();
    LOGI("鼠标世界位置: (%.2f, %.2f)", worldPos.x, worldPos.y);
}
```

### 触摸输入处理

```cpp
void handleTouchInput() {
    // 检测触摸输入
    int touchCount = Boo::game->input->getTouchCount();
    if (touchCount > 0) {
        // 获取第一个触摸点的位置
        Boo::Vec2 touchPos = Boo::game->input->getTouchPosition(0);
        LOGI("触摸位置: (%.2f, %.2f)", touchPos.x, touchPos.y);
        
        // 检测触摸是否刚刚开始
        if (Boo::game->input->getTouchDown(0)) {
            LOGI("触摸开始！");
        }
        
        // 检测触摸是否刚刚结束
        if (Boo::game->input->getTouchUp(0)) {
            LOGI("触摸结束！");
        }
    }
}
```

## 输入系统架构

### 核心组件
- **Input**：输入系统的核心类，负责处理所有输入事件
- **InputManager**：管理输入设备和输入状态

### 输入流程
1. **输入事件捕获**：从操作系统捕获原始输入事件
2. **输入状态更新**：根据原始事件更新输入状态
3. **输入查询**：游戏代码通过输入系统查询输入状态
4. **输入处理**：根据输入状态执行相应的游戏逻辑

## 最佳实践

1. **输入映射**：使用枚举或常量定义输入按键，便于统一管理
2. **输入检测**：在游戏的更新循环中检测输入，确保及时响应
3. **输入状态**：区分持续按下和刚刚按下/释放的状态
4. **多平台适配**：考虑不同平台的输入差异，如PC端的键盘鼠标和移动端的触摸
5. **输入反馈**：为重要的输入操作提供视觉或听觉反馈

## 常见问题

- **输入无响应**：检查输入系统是否正确初始化，以及输入事件是否正确传递
- **触摸位置偏差**：检查触摸坐标转换是否正确，特别是屏幕坐标到世界坐标的转换
- **按键重复触发**：使用 `getKeyDown` 和 `getKeyUp` 来检测单次按键事件
- **输入延迟**：优化输入处理逻辑，避免在输入检测中执行耗时操作

## 总结

输入系统是 Boo 引擎中处理玩家交互的重要组件，它提供了统一的接口来处理键盘、鼠标和触摸输入。通过输入系统，你可以：

- 检测玩家的各种输入操作
- 实现游戏中的交互逻辑
- 支持不同平台的输入设备
- 提供流畅的玩家体验

输入系统的设计理念是提供简洁、统一的输入接口，使开发者能够专注于游戏逻辑的实现，而不必关心底层的输入处理细节。