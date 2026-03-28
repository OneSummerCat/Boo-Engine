# 输入系统 API

## 核心方法

### 键盘输入

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `getKey(KeyCode key)` | 检测按键是否被按下 | key: 按键代码 | bool: 是否按下 |
| `getKeyDown(KeyCode key)` | 检测按键是否刚刚被按下 | key: 按键代码 | bool: 是否刚刚按下 |
| `getKeyUp(KeyCode key)` | 检测按键是否刚刚被释放 | key: 按键代码 | bool: 是否刚刚释放 |

### 鼠标输入

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `getMousePosition()` | 获取鼠标位置 | 无 | Vec2: 鼠标坐标 |
| `getMouseButton(MouseButton button)` | 检测鼠标按键是否被按下 | button: 鼠标按键 | bool: 是否按下 |
| `getMouseButtonDown(MouseButton button)` | 检测鼠标按键是否刚刚被按下 | button: 鼠标按键 | bool: 是否刚刚按下 |
| `getMouseButtonUp(MouseButton button)` | 检测鼠标按键是否刚刚被释放 | button: 鼠标按键 | bool: 是否刚刚释放 |
| `getMouseScroll()` | 获取鼠标滚轮增量 | 无 | Vec2: 滚轮增量 |

### 触摸输入

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `getTouchCount()` | 获取当前触摸点数量 | 无 | int: 触摸点数量 |
| `getTouchPosition(int index)` | 获取指定触摸点的位置 | index: 触摸点索引 | Vec2: 触摸点坐标 |
| `getTouchDelta(int index)` | 获取指定触摸点的移动增量 | index: 触摸点索引 | Vec2: 移动增量 |

## 枚举类型

### KeyCode

| 值 | 描述 |
|-----|------|
| `SPACE` | 空格键 |
| `A` | A键 |
| `B` | B键 |
| `C` | C键 |
| `D` | D键 |
| `E` | E键 |
| `F` | F键 |
| `G` | G键 |
| `H` | H键 |
| `I` | I键 |
| `J` | J键 |
| `K` | K键 |
| `L` | L键 |
| `M` | M键 |
| `N` | N键 |
| `O` | O键 |
| `P` | P键 |
| `Q` | Q键 |
| `R` | R键 |
| `S` | S键 |
| `T` | T键 |
| `U` | U键 |
| `V` | V键 |
| `W` | W键 |
| `X` | X键 |
| `Y` | Y键 |
| `Z` | Z键 |
| `UP` | 上方向键 |
| `DOWN` | 下方向键 |
| `LEFT` | 左方向键 |
| `RIGHT` | 右方向键 |
| `ENTER` | 回车键 |
| `ESCAPE` | 转义键 |
| `SHIFT` | Shift键 |
| `CTRL` | Ctrl键 |
| `ALT` | Alt键 |

### MouseButton

| 值 | 描述 |
|-----|------|
| `LEFT` | 鼠标左键 |
| `RIGHT` | 鼠标右键 |
| `MIDDLE` | 鼠标中键 |