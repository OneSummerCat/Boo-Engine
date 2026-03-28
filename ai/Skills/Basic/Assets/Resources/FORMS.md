# 资产管理系统输入输出格式

## 输入格式

### 加载纹理

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `path` | string | 纹理文件路径 | 是 |

### 加载材质

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `path` | string | 材质文件路径 | 是 |

### 加载着色器

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `path` | string | 着色器文件路径 | 是 |

### 加载场景

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `path` | string | 场景文件路径 | 是 |

### 加载网格

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `path` | string | 网格文件路径 | 是 |

### 释放资产

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `assetPath` | string | 资产文件路径 | 是 |

## 输出格式

### 加载资产

| 字段 | 类型 | 描述 |
|------|------|------|
| `success` | boolean | 是否加载成功 |
| `asset` | object | 资产信息 |
| `message` | string | 加载结果消息 |

### 资产信息

| 字段 | 类型 | 描述 |
|------|------|------|
| `path` | string | 资产路径 |
| `uuid` | string | 资产UUID |
| `type` | string | 资产类型 |
| `width` | number | 纹理宽度（仅纹理资产） |
| `height` | number | 纹理高度（仅纹理资产） |

### 释放资产

| 字段 | 类型 | 描述 |
|------|------|------|
| `success` | boolean | 是否释放成功 |
| `message` | string | 释放结果消息 |

## 示例输入输出

### 示例 1: 加载纹理

**输入**:
```json
{
  "path": "assets/textures/player.png"
}
```

**输出**:
```json
{
  "success": true,
  "asset": {
    "path": "assets/textures/player.png",
    "uuid": "12345678-1234-1234-1234-1234567890ab",
    "type": "texture",
    "width": 128,
    "height": 128
  },
  "message": "纹理加载成功"
}
```

### 示例 2: 加载材质

**输入**:
```json
{
  "path": "assets/materials/player.material"
}
```

**输出**:
```json
{
  "success": true,
  "asset": {
    "path": "assets/materials/player.material",
    "uuid": "87654321-4321-4321-4321-ba0987654321",
    "type": "material"
  },
  "message": "材质加载成功"
}
```

### 示例 3: 加载场景

**输入**:
```json
{
  "path": "assets/scenes/level1.scene"
}
```

**输出**:
```json
{
  "success": true,
  "asset": {
    "path": "assets/scenes/level1.scene",
    "uuid": "11223344-5566-7788-9900-aabbccddeeff",
    "type": "scene"
  },
  "message": "场景加载成功"
}
```

### 示例 4: 释放资产

**输入**:
```json
{
  "assetPath": "assets/textures/player.png"
}
```

**输出**:
```json
{
  "success": true,
  "message": "资产释放成功"
}
```