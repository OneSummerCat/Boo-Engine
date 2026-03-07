# Boo 命名空间核心 API 文档

## 全局实例 API

### Boo::game - Game* 游戏核心实例

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `openScene(Scene* scene)` | `void` | 打开指定场景 |
| `destroyScene()` | `void` | 销毁当前场景 |
| `getScene()` | `Scene*` | 获取当前场景 |

### Boo::event - Event* 事件系统

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `on(std::string name, std::function<void()> callback)` | `void` | 添加事件监听器 |
| `emit(std::string name)` | `void` | 触发指定事件 |

### Boo::view - View* 视图系统

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `resize(int width, int height)` | `void` | 调整视口大小 |
| `getWidth()` | `int` | 获取视口宽度 |
| `getHeight()` | `int` | 获取视口高度 |
| `getDesignWidth()` | `int` | 获取设计宽度 |
| `getDesignHeight()` | `int` | 获取设计高度 |
| `getDesignFitMode()` | `UIDesignFitMode` | 获取设计适配模式 |
| `getFitMatrix()` | `Mat4` | 获取适配矩阵 |

### Boo::assetsManager - AssetsManager* 资产管理系统

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `loadAsset(const std::string& path)` | `Asset*` | 加载指定路径的资产 |
| `getAsset(const std::string& path)` | `Asset*` | 获取已加载的资产 |
| `unloadAsset(const std::string& path)` | `void` | 卸载指定资产 |


## 核心类

### 场景管理
- `Boo::Scene` - 场景类，游戏世界的根容器
- `Boo::Node` - 基础节点类
- `Boo::Node2D` - 2D节点类，继承自Node
- `Boo::Node3D` - 3D节点类，独立类（暂未继承自Node）

### 数学
- `Boo::Vec2` - 2D向量
- `Boo::Vec3` - 3D向量
- `Boo::Mat4` - 4x4矩阵
- `Boo::Quat` - 四元数
- `Boo::Color` - 颜色类
- `Boo::Size` - 尺寸类

### 资产
- `Boo::Asset` - 基础资产类
- `Boo::TextureAsset` - 纹理资产
- `Boo::MaterialAsset` - 材质资产
- `Boo::ShaderAsset` - 着色器资产
- `Boo::SceneAsset` - 场景资产
- `Boo::MeshAsset` - 网格资产

### 组件
- `Boo::Component` - 基础组件类
- `Boo::ComponentFactory` - 组件工厂

### 渲染
- `Boo::Renderer` - 渲染器
- `Boo::Camera` - 相机类

### 工具
- `Boo::FileUtil` - 文件工具
- `Boo::UUIDUtil` - UUID工具
- `Boo::TimeUtil` - 时间工具
