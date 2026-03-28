# Pixel Art Template Example

## 1. 准备工作

1. **获取参考图片**：准备需要转换的图片，无需存放到项目目录中。

2. **获取图片路径**：在聊天中提供图片的完整路径。

## 2. 提取像素数据

### 运行像素提取脚本

```bash
python ai/Skills/Template/CreatePixelArt/Scripts/pixel_extractor.py <图片完整路径>
```

### 生成的C++代码示例

```cpp
const int pixelDataWidth = 70;
const int pixelDataHeight = 70;
int pixelData[70][70] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // ... 其他行数据 ...
};
```

## 3. C++ 实现示例

### 主入口文件 (`src/<示例名称>/<示例名称>.cpp`)

```cpp
#include "<示例名称>/<示例名称>.h"

// 像素数据（由Python脚本生成）
const int pixelDataWidth = 70;
const int pixelDataHeight = 70;
int pixelData[70][70] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // ... 其他行数据 ...
};

int main() {
    // 创建像素艺术实例
    PixelArt pixelArt;
    
    // 创建场景
    pixelArt.createScene();
    
    // 转换数据格式
    int** data = new int*[pixelDataHeight];
    for (int i = 0; i < pixelDataHeight; i++) {
        data[i] = pixelData[i];
    }
    
    // 设置像素数据
    pixelArt.setPixelData(data, pixelDataWidth, pixelDataHeight);
    
    // 清理
    delete[] data;
    
    return 0;
}
```

### 像素艺术类实现 (`src/<示例名称>/<示例名称>.cpp`)

```cpp
#include "<示例名称>/<示例名称>.h"
#include <vector>

PixelArt::PixelArt() : m_scene(nullptr) {
}

PixelArt::~PixelArt() {
    // 析构函数
}

// 创建像素艺术场景
void PixelArt::createScene() {
    // 创建场景
    m_scene = new Boo::Scene("PixelArtScene");
    
    // 场景创建后立即打开
    Boo::game->openScene(m_scene);
    
    auto root2D = static_cast<Boo::Node2D*>(m_scene->getRoot2D());
    
    auto cameraNode = new Boo::Node2D("Camera");
    root2D->addChild(cameraNode);
    cameraNode->addComponent("Camera");
}

// 设置像素数据
void PixelArt::setPixelData(int** data, int width, int height) {
    if (!m_scene) {
        createScene();
    }
    
    auto root2D = static_cast<Boo::Node2D*>(m_scene->getRoot2D());
    
    // 默认方块大小
    int blockSize = 10;
    
    // 计算起始位置（居中）
    float startX = - (width * blockSize) / 2.0f;
    float startY = (height * blockSize) / 2.0f;
    
    // 绘制像素
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (data[row][col] == 1) {
                auto blockNode = new Boo::Node2D("PixelBlock" + std::to_string(row) + "_" + std::to_string(col));
                root2D->addChild(blockNode);
                blockNode->setSize(blockSize, blockSize);
                
                float x = startX + col * blockSize + blockSize / 2.0f;
                float y = startY - row * blockSize - blockSize / 2.0f;
                blockNode->setPosition(x, y, 0);
                
                auto spriteComponent = blockNode->addComponent("UISprite");
                auto sprite = dynamic_cast<Boo::UISprite*>(spriteComponent);
                if (sprite) {
                    sprite->setColor(1.0f, 0.0f, 0.0f, 1.0f); // 默认红色
                    sprite->setTexture(Boo::AssetBuiltinTexture::Default);
                }
            }
        }
    }
}

// 设置方块大小
void PixelArt::setBlockSize(int size) {
    // 实现方块大小设置
}

// 设置方块颜色
void PixelArt::setBlockColor(float r, float g, float b, float a) {
    // 实现方块颜色设置
}
```

## 4. 编译和运行

### 编译项目

```bash
mkdir -p build
cd build
cmake ..
make
```

### 运行项目

```bash
./<项目名称>
```

## 5. 预期效果

运行后，屏幕上会显示一个由小方块组成的像素艺术图像，例如龙虾。方块大小为10x10像素，颜色为默认的红色，图像居中显示在屏幕上。

## 6. 扩展示例

### 自定义方块大小和颜色

```cpp
#include "<示例名称>/<示例名称>.h"

// 像素数据（由Python脚本生成）
const int pixelDataWidth = 70;
const int pixelDataHeight = 70;
int pixelData[70][70] = {
    // ... 数据内容 ...
};

int main() {
    // 创建像素艺术实例
    PixelArt pixelArt;
    
    // 创建场景
    pixelArt.createScene();
    
    // 转换数据格式
    int** data = new int*[pixelDataHeight];
    for (int i = 0; i < pixelDataHeight; i++) {
        data[i] = pixelData[i];
    }
    
    // 设置方块大小
    pixelArt.setBlockSize(8);
    
    // 设置方块颜色（蓝色）
    pixelArt.setBlockColor(0.0f, 0.0f, 1.0f, 1.0f);
    
    // 设置像素数据
    pixelArt.setPixelData(data, pixelDataWidth, pixelDataHeight);
    
    // 清理
    delete[] data;
    
    return 0;
}
```

### 处理不同尺寸的图像

对于不同尺寸的图像，Python脚本会自动缩放数据到最大100x100的网格，确保在屏幕上能够合理显示。

例如，对于一张500x300的图片，脚本会生成一个约100x60的网格数据。