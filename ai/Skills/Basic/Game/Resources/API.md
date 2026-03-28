# Game 类核心 API 文档

## 场景管理

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `getScene()` | `Scene*` | 获取当前场景 |
| `openScene(Scene *scene)` | `void` | 打开指定场景 |
| `destroyScene()` | `void` | 销毁当前场景 |

## 视图管理

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `resizeView(const int width, const int height)` | `void` | 调整视图大小 |

## 调度系统

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `schedule(Func func, T *instance, float interval)` | `int` | 安排定时任务（重复执行），返回任务ID |
| `scheduleOnce(Func func, T *instance, float interval)` | `int` | 安排定时任务（执行一次），返回任务ID |
| `unschedule(int scheduleID)` | `void` | 取消指定ID的定时任务 |

## 输入处理

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `updateMouseState(int button, int action, int mods)` | `void` | 更新鼠标按钮状态 |
| `updateMousePos(double xpos, double ypos)` | `void` | 更新鼠标位置 |
| `updateKeyState(int key, int scancode, int action, int mods)` | `void` | 更新键盘按键状态 |

