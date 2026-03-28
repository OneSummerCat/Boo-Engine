# Pixel Art Template API

## 1. Python 像素提取脚本 API

### 函数签名
```python
def extract_pixels(image_path, interval=5, max_size=100):
    """
    从图片中提取像素数据
    :param image_path: 图片路径
    :param interval: 采样间隔
    :param max_size: 最大尺寸
    :return: 二值像素数据
    """

def generate_cpp_code(data, variable_name="pixelData"):
    """
    生成C++代码
    :param data: 像素数据
    :param variable_name: 变量名
    :return: C++代码字符串
    """
```

## 2. C++ 核心 API

### PixelArt 类
```cpp
class PixelArt {
private:
    void* m_scene;
    
public:
    PixelArt();
    ~PixelArt();
    
    // 创建像素艺术场景
    void createScene();
    
    // 设置像素数据
    void setPixelData(int** data, int width, int height);
    
    // 设置方块大小
    void setBlockSize(int size);
    
    // 设置方块颜色
    void setBlockColor(float r, float g, float b, float a);
};
```

## 3. 主要方法

### createScene()
- 功能：创建像素艺术场景
- 参数：无
- 返回值：无

### setPixelData(int** data, int width, int height)
- 功能：设置像素数据并渲染
- 参数：
  - data: 二维数组，存储像素数据（1表示需要显示，0表示不显示）
  - width: 数据宽度
  - height: 数据高度
- 返回值：无

### setBlockSize(int size)
- 功能：设置方块大小
- 参数：
  - size: 方块大小（像素）
- 返回值：无

### setBlockColor(float r, float g, float b, float a)
- 功能：设置方块颜色
- 参数：
  - r: 红色通道值（0-1）
  - g: 绿色通道值（0-1）
  - b: 蓝色通道值（0-1）
  - a: 透明度值（0-1）
- 返回值：无

## 4. 错误处理

### 1. 项目配置错误
如果遇到项目配置错误，确保：
- CMakeLists.txt 文件正确配置
- 所有必要的头文件和源文件都已包含
- Boo Engine 正确集成到项目中

### 2. Python 脚本错误
如果运行 Python 脚本时遇到错误，检查：
- 已安装 Python 3
- 已安装 OpenCV (`pip install opencv-python`)
- 图片路径正确
- 图片文件可访问

### 3. 渲染问题
如果像素艺术渲染不正确：
- 检查像素数据是否正确生成
- 验证方块大小是否适合屏幕分辨率
- 确保 Boo Engine 正确初始化
- 检查控制台中的错误消息