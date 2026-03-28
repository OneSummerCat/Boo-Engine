# 事件系统 API

## 核心方法

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `subscribe(const std::string& eventName, const EventCallback& callback)` | 订阅事件 | eventName: 事件名称<br>callback: 事件回调函数 | void |
| `subscribe(const std::string& eventName, int priority, const EventCallback& callback)` | 订阅事件（带优先级） | eventName: 事件名称<br>priority: 优先级（值越小优先级越高）<br>callback: 事件回调函数 | void |
| `unsubscribe(const std::string& eventName, const EventCallback& callback)` | 取消订阅事件 | eventName: 事件名称<br>callback: 事件回调函数 | void |
| `publish(const std::string& eventName, const EventData& data)` | 发布事件 | eventName: 事件名称<br>data: 事件数据 | void |
| `hasSubscribers(const std::string& eventName)` | 检查事件是否有订阅者 | eventName: 事件名称 | bool: 是否有订阅者 |

## EventData 类

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `setInt(const std::string& key, int value)` | 设置整数类型的事件数据 | key: 键名<br>value: 整数值 | void |
| `setFloat(const std::string& key, float value)` | 设置浮点数类型的事件数据 | key: 键名<br>value: 浮点数值 | void |
| `setBool(const std::string& key, bool value)` | 设置布尔类型的事件数据 | key: 键名<br>value: 布尔值 | void |
| `setString(const std::string& key, const std::string& value)` | 设置字符串类型的事件数据 | key: 键名<br>value: 字符串值 | void |
| `setVec2(const std::string& key, const Vec2& value)` | 设置Vec2类型的事件数据 | key: 键名<br>value: Vec2值 | void |
| `setVec3(const std::string& key, const Vec3& value)` | 设置Vec3类型的事件数据 | key: 键名<br>value: Vec3值 | void |
| `getInt(const std::string& key, int defaultValue = 0)` | 获取整数类型的事件数据 | key: 键名<br>defaultValue: 默认值 | int: 整数值 |
| `getFloat(const std::string& key, float defaultValue = 0.0f)` | 获取浮点数类型的事件数据 | key: 键名<br>defaultValue: 默认值 | float: 浮点数值 |
| `getBool(const std::string& key, bool defaultValue = false)` | 获取布尔类型的事件数据 | key: 键名<br>defaultValue: 默认值 | bool: 布尔值 |
| `getString(const std::string& key, const std::string& defaultValue = "")` | 获取字符串类型的事件数据 | key: 键名<br>defaultValue: 默认值 | std::string: 字符串值 |
| `getVec2(const std::string& key, const Vec2& defaultValue = Vec2())` | 获取Vec2类型的事件数据 | key: 键名<br>defaultValue: 默认值 | Vec2: Vec2值 |
| `getVec3(const std::string& key, const Vec3& defaultValue = Vec3())` | 获取Vec3类型的事件数据 | key: 键名<br>defaultValue: 默认值 | Vec3: Vec3值 |
| `hasKey(const std::string& key)` | 检查是否存在指定键名的数据 | key: 键名 | bool: 是否存在 |