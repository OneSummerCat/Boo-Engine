# Pixel Art Template Forms

## 1. 输入格式

### 技能调用输入

```json
{
  "image_path": "/path/to/image.png",
  "interval": 5,
  "max_size": 100
}
```

### 参数说明

| 参数名 | 类型 | 必填 | 默认值 | 描述 |
|-------|------|------|--------|------|
| image_path | string | 是 | - | 参考图片的完整路径 |
| interval | number | 否 | 5 | 像素采样间隔 |
| max_size | number | 否 | 100 | 最大网格尺寸 |

## 2. 输出格式

### 技能调用输出

```json
{
  "cpp_code": "const int pixelDataWidth = 70;\nconst int pixelDataHeight = 70;\nint pixelData[70][70] = {\n    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},\n    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},\n    // ... 其他行数据 ...\n};\n"
}
```

### 输出说明

| 输出字段 | 类型 | 描述 |
|---------|------|------|
| cpp_code | string | 生成的C++代码，包含像素数据和必要的变量定义 |

## 3. Python 脚本输入输出

### 脚本命令

```bash
python ai/Skills/Template/CreatePixelArt/Scripts/pixel_extractor.py <图片完整路径>
```

### 脚本输出

```
=== C++ 代码 ===
const int pixelDataWidth = 70;
const int pixelDataHeight = 70;
int pixelData[70][70] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // ... 其他行数据 ...
};

代码已保存到 pixel_data.cpp
```

## 4. 错误处理输出

### 无效图片路径

```json
{
  "error": "无法读取图片: /path/to/invalid.png",
  "code": "INVALID_IMAGE"
}
```

### 缺少必要参数

```json
{
  "error": "缺少必要参数: image_path",
  "code": "MISSING_PARAMETER"
}
```

### Python 脚本错误

```json
{
  "error": "Python 脚本执行失败: 缺少 OpenCV 库",
  "code": "SCRIPT_ERROR"
}
```