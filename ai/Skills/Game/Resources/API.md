# Game 类核心 API 文档

## 初始化与启动

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `init(Window *window, int uiDesignWidth, int uiDesignHeight, UIDesignFitMode fitMode)` | `void` | 初始化游戏（窗口平台），设置设计分辨率和适配模式 |
| `init(Android *android, int uiDesignWidth, int uiDesignHeight, UIDesignFitMode fitMode)` | `void` | 初始化游戏（Android平台），设置设计分辨率和适配模式 |

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

## 游戏循环

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `tick()` | `void` | 游戏主循环，处理更新和渲染 |

## 资源管理

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `addCompClearCaches(Component *comp)` | `void` | 添加组件到清理缓存 |
| `addNodeClearCaches(Node *node)` | `void` | 添加节点到清理缓存 |