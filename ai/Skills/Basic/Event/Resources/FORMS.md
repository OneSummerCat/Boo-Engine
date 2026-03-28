# 事件系统输入输出格式

## 输入格式

### 订阅事件

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `eventName` | string | 事件名称 | 是 |
| `priority` | number | 事件优先级（值越小优先级越高） | 否，默认为5 |
| `callback` | string | 事件回调函数的描述 | 是 |

### 发布事件

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `eventName` | string | 事件名称 | 是 |
| `data` | object | 事件数据 | 否，默认为空 |

### 取消订阅事件

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `eventName` | string | 事件名称 | 是 |
| `callback` | string | 事件回调函数的描述 | 是 |

## 输出格式

### 订阅事件

| 字段 | 类型 | 描述 |
|------|------|------|
| `success` | boolean | 是否订阅成功 |
| `message` | string | 订阅结果消息 |

### 发布事件

| 字段 | 类型 | 描述 |
|------|------|------|
| `success` | boolean | 是否发布成功 |
| `subscriberCount` | number | 事件订阅者数量 |
| `message` | string | 发布结果消息 |

### 取消订阅事件

| 字段 | 类型 | 描述 |
|------|------|------|
| `success` | boolean | 是否取消订阅成功 |
| `message` | string | 取消订阅结果消息 |

## 示例输入输出

### 示例 1: 订阅事件

**输入**:
```json
{
  "eventName": "PlayerJump",
  "priority": 3,
  "callback": "处理玩家跳跃事件"
}
```

**输出**:
```json
{
  "success": true,
  "message": "成功订阅 PlayerJump 事件"
}
```

### 示例 2: 发布事件

**输入**:
```json
{
  "eventName": "PlayerJump",
  "data": {
    "jumpHeight": 10.0,
    "playerId": 1
  }
}
```

**输出**:
```json
{
  "success": true,
  "subscriberCount": 2,
  "message": "成功发布 PlayerJump 事件"
}
```

### 示例 3: 取消订阅事件

**输入**:
```json
{
  "eventName": "PlayerJump",
  "callback": "处理玩家跳跃事件"
}
```

**输出**:
```json
{
  "success": true,
  "message": "成功取消订阅 PlayerJump 事件"
}
```