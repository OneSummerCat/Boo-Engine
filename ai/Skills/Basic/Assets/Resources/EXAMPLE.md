# 资产管理系统示例

## 示例 1: 加载和使用纹理

```cpp
#include "engine/boo.h"

class MyGame {
private:
    Boo::TextureAsset* playerTexture;
    Boo::TextureAsset* backgroundTexture;
    
public:
    void init() {
        // 加载纹理
        playerTexture = Boo::assetsManager->loadTexture("assets/textures/player.png");
        backgroundTexture = Boo::assetsManager->loadTexture("assets/textures/background.png");
        
        if (playerTexture) {
            LOGI("玩家纹理加载成功，尺寸: %dx%d", playerTexture->getWidth(), playerTexture->getHeight());
        } else {
            LOGE("玩家纹理加载失败");
        }
    }
    
    void update(float deltaTime) {
        // 使用纹理（在渲染代码中）
        // 这里只是示例，实际渲染代码会更复杂
    }
    
    void cleanup() {
        // 释放纹理
        if (playerTexture) {
            playerTexture->release();
        }
        if (backgroundTexture) {
            backgroundTexture->release();
        }
    }
};
```

## 示例 2: 加载和使用材质

```cpp
#include "engine/boo.h"

class MyGame {
private:
    Boo::MaterialAsset* playerMaterial;
    Boo::TextureAsset* playerTexture;
    
public:
    void init() {
        // 加载纹理
        playerTexture = Boo::assetsManager->loadTexture("assets/textures/player.png");
        
        // 加载材质
        playerMaterial = Boo::assetsManager->loadMaterial("assets/materials/player.material");
        
        if (playerMaterial && playerTexture) {
            // 设置材质纹理
            playerMaterial->setTexture("diffuseTexture", playerTexture);
            // 设置材质参数
            playerMaterial->setFloat("shininess", 32.0f);
            playerMaterial->setVec3("specularColor", Boo::Vec3(1.0f, 1.0f, 1.0f));
            LOGI("材质加载和配置成功");
        } else {
            LOGE("材质或纹理加载失败");
        }
    }
    
    void cleanup() {
        // 释放资源
        if (playerMaterial) {
            playerMaterial->release();
        }
        if (playerTexture) {
            playerTexture->release();
        }
    }
};
```

## 示例 3: 加载和使用场景

```cpp
#include "engine/boo.h"

class MyGame {
private:
    Boo::SceneAsset* levelScene;
    
public:
    void init() {
        // 加载场景
        levelScene = Boo::assetsManager->loadScene("assets/scenes/level1.scene");
        
        if (levelScene) {
            Boo::Scene* scene = levelScene->getScene();
            if (scene) {
                // 设置当前场景
                Boo::game->setScene(scene);
                LOGI("场景加载成功");
            } else {
                LOGE("场景实例获取失败");
            }
        } else {
            LOGE("场景加载失败");
        }
    }
    
    void cleanup() {
        // 释放场景
        if (levelScene) {
            levelScene->release();
        }
    }
};
```

## 示例 4: 资产缓存管理

```cpp
#include "engine/boo.h"

class MyGame {
public:
    void init() {
        // 加载多个资产
        Boo::assetsManager->loadTexture("assets/textures/player.png");
        Boo::assetsManager->loadTexture("assets/textures/enemy.png");
        Boo::assetsManager->loadTexture("assets/textures/background.png");
        
        // 获取已加载的资产数量
        int assetCount = Boo::assetsManager->getLoadedAssetsCount();
        LOGI("已加载的资产数量: %d", assetCount);
        
        // 获取已加载的纹理（从缓存中）
        Boo::TextureAsset* playerTexture = Boo::assetsManager->getTexture("assets/textures/player.png");
        if (playerTexture) {
            LOGI("从缓存中获取玩家纹理成功");
        }
    }
    
    void cleanup() {
        // 清空所有资产缓存
        Boo::assetsManager->clearCache();
        LOGI("资产缓存已清空");
    }
};
```