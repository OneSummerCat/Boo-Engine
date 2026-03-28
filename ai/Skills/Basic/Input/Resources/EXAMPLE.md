# 输入系统示例

## 示例 1: 基本键盘输入

```cpp
#include "engine/boo.h"

class MyGame {
public:
    void update(float deltaTime) {
        // 检测空格键是否被按下
        if (Boo::game->input->getKey(Boo::KeyCode::SPACE)) {
            LOGI("空格键被按下");
        }
        
        // 检测方向键
        if (Boo::game->input->getKey(Boo::KeyCode::UP)) {
            LOGI("上方向键被按下");
        }
        if (Boo::game->input->getKey(Boo::KeyCode::DOWN)) {
            LOGI("下方向键被按下");
        }
        if (Boo::game->input->getKey(Boo::KeyCode::LEFT)) {
            LOGI("左方向键被按下");
        }
        if (Boo::game->input->getKey(Boo::KeyCode::RIGHT)) {
            LOGI("右方向键被按下");
        }
    }
};
```

## 示例 2: 鼠标输入

```cpp
#include "engine/boo.h"

class MyGame {
public:
    void update(float deltaTime) {
        // 获取鼠标位置
        Boo::Vec2 mousePos = Boo::game->input->getMousePosition();
        LOGI("鼠标位置: (%.2f, %.2f)", mousePos.x, mousePos.y);
        
        // 检测鼠标左键
        if (Boo::game->input->getMouseButton(Boo::MouseButton::LEFT)) {
            LOGI("鼠标左键被按下");
        }
        
        // 检测鼠标滚轮
        Boo::Vec2 scroll = Boo::game->input->getMouseScroll();
        if (scroll.y != 0) {
            LOGI("鼠标滚轮: %.2f", scroll.y);
        }
    }
};
```

## 示例 3: 触摸输入

```cpp
#include "engine/boo.h"

class MyGame {
public:
    void update(float deltaTime) {
        // 获取触摸点数量
        int touchCount = Boo::game->input->getTouchCount();
        if (touchCount > 0) {
            LOGI("触摸点数量: %d", touchCount);
            
            // 获取第一个触摸点的位置
            Boo::Vec2 touchPos = Boo::game->input->getTouchPosition(0);
            LOGI("触摸位置: (%.2f, %.2f)", touchPos.x, touchPos.y);
        }
    }
};
```