---
name: "assets"
description: "资产管理系统，负责加载和管理游戏资源。"
---

# 资产管理系统

资产管理系统负责加载和管理游戏资源，包括纹理、材质、着色器、场景和网格等。通过资产管理系统，你可以方便地加载和使用各种游戏资源。

## 核心功能

### 资源加载
- **纹理加载**：加载和管理纹理资产
- **材质加载**：加载和管理材质资产
- **着色器加载**：加载和管理着色器资产
- **场景加载**：加载和管理场景资产
- **网格加载**：加载和管理网格资产

### 资源管理
- **资源缓存**：缓存已加载的资源，避免重复加载
- **引用计数**：通过引用计数管理资源的生命周期
- **资源释放**：自动或手动释放不再使用的资源

## 主要接口

### 资源加载
```cpp
// 加载纹理
TextureAsset* loadTexture(const std::string& path);

// 加载材质
MaterialAsset* loadMaterial(const std::string& path);

// 加载着色器
ShaderAsset* loadShader(const std::string& path);

// 加载场景
SceneAsset* loadScene(const std::string& path);

// 加载网格
MeshAsset* loadMesh(const std::string& path);
```

### 资源管理
```cpp
// 获取资产
Asset* getAsset(const std::string& path);

// 释放资产
void releaseAsset(Asset* asset);

// 释放所有资产
void releaseAllAssets();

// 获取资产引用计数
int getAssetRefCount(const std::string& path);

// 检查资产是否存在
bool hasAsset(const std::string& path);
```

## 资产类层次结构

```cpp
// 基础资产类
class Asset {
public:
    virtual ~Asset() = default;
    
    // 获取资产路径
    const std::string& getPath() const;
    
    // 获取资产类型
    virtual std::string getType() const = 0;
    
    // 增加引用计数
    void addRef();
    
    // 减少引用计数
    void release();
    
    // 获取引用计数
    int getRefCount() const;
};

// 纹理资产
class TextureAsset : public Asset {
public:
    // 获取纹理ID
    unsigned int getTextureId() const;
    
    // 获取纹理尺寸
    Size getSize() const;
    
    // 获取纹理类型
    std::string getType() const override;
};

// 材质资产
class MaterialAsset : public Asset {
public:
    // 获取材质 uniforms
    const std::map<std::string, UniformValue>& getUniforms() const;
    
    // 设置材质 uniform
    void setUniform(const std::string& name, const UniformValue& value);
    
    // 获取材质类型
    std::string getType() const override;
};

// 着色器资产
class ShaderAsset : public Asset {
public:
    // 获取着色器程序ID
    unsigned int getProgramId() const;
    
    // 获取着色器类型
    std::string getType() const override;
};

// 场景资产
class SceneAsset : public Asset {
public:
    // 获取场景数据
    const SceneData& getSceneData() const;
    
    // 获取场景类型
    std::string getType() const override;
};

// 网格资产
class MeshAsset : public Asset {
public:
    // 获取网格顶点数据
    const MeshData& getMeshData() const;
    
    // 获取网格类型
    std::string getType() const override;
};
```

## 使用示例

### 基本资源加载

```cpp
#include "engine/boo.h"

void loadGameResources() {
    // 加载纹理
    Boo::TextureAsset* playerTexture = Boo::assetsManager->loadTexture("assets/textures/player.png");
    if (playerTexture) {
        LOGI("成功加载玩家纹理: %s", playerTexture->getPath().c_str());
    }
    
    // 加载材质
    Boo::MaterialAsset* playerMaterial = Boo::assetsManager->loadMaterial("assets/materials/player.material");
    if (playerMaterial) {
        LOGI("成功加载玩家材质: %s", playerMaterial->getPath().c_str());
    }
    
    // 加载场景
    Boo::SceneAsset* levelScene = Boo::assetsManager->loadScene("assets/scenes/level1.scene");
    if (levelScene) {
        LOGI("成功加载场景: %s", levelScene->getPath().c_str());
    }
    
    // 使用完资源后释放
    // 注意：如果资源被其他对象引用，引用计数会大于1，此时不会真正释放
    Boo::assetsManager->releaseAsset(playerTexture);
    Boo::assetsManager->releaseAsset(playerMaterial);
    Boo::assetsManager->releaseAsset(levelScene);
}
```

### 资源管理

```cpp
void manageResources() {
    // 检查资源是否存在
    if (Boo::assetsManager->hasAsset("assets/textures/player.png")) {
        LOGI("玩家纹理已存在");
    } else {
        LOGI("玩家纹理不存在，需要加载");
    }
    
    // 加载纹理
    Boo::TextureAsset* texture = Boo::assetsManager->loadTexture("assets/textures/player.png");
    
    // 获取引用计数
    int refCount = Boo::assetsManager->getAssetRefCount("assets/textures/player.png");
    LOGI("玩家纹理引用计数: %d", refCount);
    
    // 释放所有资源
    // 注意：在游戏结束或场景切换时使用
    // Boo::assetsManager->releaseAllAssets();
}
```

## 资产管理系统架构

### 核心组件
- **AssetsManager**：资产管理系统的核心类，负责资源的加载和管理
- **Asset**：所有资产的基类，定义了资产的基本接口和引用计数机制
- **TextureAsset**、**MaterialAsset**、**ShaderAsset**、**SceneAsset**、**MeshAsset**：具体的资产类型

### 资源加载流程
1. **资源请求**：游戏代码通过 `load*` 方法请求加载资源
2. **资源检查**：检查资源是否已在缓存中
3. **资源加载**：如果资源不在缓存中，从磁盘加载资源
4. **资源缓存**：将加载的资源添加到缓存中
5. **引用计数**：增加资源的引用计数
6. **资源返回**：返回加载的资源指针

### 资源释放流程
1. **释放请求**：游戏代码通过 `releaseAsset` 方法请求释放资源
2. **引用计数减少**：减少资源的引用计数
3. **资源检查**：检查引用计数是否为0
4. **资源释放**：如果引用计数为0，释放资源内存并从缓存中移除

## 最佳实践

1. **资源路径**：使用相对路径加载资源，确保跨平台兼容性
2. **资源管理**：及时释放不再使用的资源，避免内存泄漏
3. **资源预加载**：在游戏加载时预加载常用资源，减少游戏运行时的加载延迟
4. **资源复用**：尽量复用相同的资源，避免重复加载
5. **资源监控**：监控资源的引用计数，确保资源正确释放

## 常见问题

- **资源加载失败**：检查资源路径是否正确，以及资源文件是否存在
- **内存泄漏**：确保及时释放不再使用的资源，检查引用计数是否正确
- **资源重复加载**：使用 `hasAsset` 方法检查资源是否已加载
- **资源释放错误**：确保只释放自己加载的资源，避免释放被其他对象使用的资源

## 总结

资产管理系统是 Boo 引擎中负责资源加载和管理的重要组件，它提供了统一的接口来加载和管理各种游戏资源。通过资产管理系统，你可以：

- 方便地加载和使用各种游戏资源
- 避免重复加载资源，提高性能
- 自动管理资源的生命周期，减少内存泄漏
- 集中管理所有游戏资源，提高代码的可维护性

资产管理系统的设计理念是提供简洁、高效的资源管理机制，使开发者能够专注于游戏逻辑的实现，而不必关心底层的资源加载和管理细节。