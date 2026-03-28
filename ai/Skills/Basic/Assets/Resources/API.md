# 资产管理系统 API

## 核心方法

### 纹理加载

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `loadTexture(const std::string& path)` | 加载纹理资产 | path: 纹理文件路径 | TextureAsset*: 纹理资产指针 |
| `getTexture(const std::string& path)` | 获取已加载的纹理资产 | path: 纹理文件路径 | TextureAsset*: 纹理资产指针 |
| `unloadTexture(TextureAsset* texture)` | 卸载纹理资产 | texture: 纹理资产指针 | void |

### 材质加载

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `loadMaterial(const std::string& path)` | 加载材质资产 | path: 材质文件路径 | MaterialAsset*: 材质资产指针 |
| `getMaterial(const std::string& path)` | 获取已加载的材质资产 | path: 材质文件路径 | MaterialAsset*: 材质资产指针 |
| `unloadMaterial(MaterialAsset* material)` | 卸载材质资产 | material: 材质资产指针 | void |

### 着色器加载

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `loadShader(const std::string& path)` | 加载着色器资产 | path: 着色器文件路径 | ShaderAsset*: 着色器资产指针 |
| `getShader(const std::string& path)` | 获取已加载的着色器资产 | path: 着色器文件路径 | ShaderAsset*: 着色器资产指针 |
| `unloadShader(ShaderAsset* shader)` | 卸载着色器资产 | shader: 着色器资产指针 | void |

### 场景加载

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `loadScene(const std::string& path)` | 加载场景资产 | path: 场景文件路径 | SceneAsset*: 场景资产指针 |
| `getScene(const std::string& path)` | 获取已加载的场景资产 | path: 场景文件路径 | SceneAsset*: 场景资产指针 |
| `unloadScene(SceneAsset* scene)` | 卸载场景资产 | scene: 场景资产指针 | void |

### 网格加载

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `loadMesh(const std::string& path)` | 加载网格资产 | path: 网格文件路径 | MeshAsset*: 网格资产指针 |
| `getMesh(const std::string& path)` | 获取已加载的网格资产 | path: 网格文件路径 | MeshAsset*: 网格资产指针 |
| `unloadMesh(MeshAsset* mesh)` | 卸载网格资产 | mesh: 网格资产指针 | void |

### 通用方法

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `getAsset(const std::string& path)` | 获取任意类型的资产 | path: 资产文件路径 | Asset*: 资产指针 |
| `unloadAsset(Asset* asset)` | 卸载任意类型的资产 | asset: 资产指针 | void |
| `clearCache()` | 清空所有资产缓存 | 无 | void |
| `getLoadedAssetsCount()` | 获取已加载的资产数量 | 无 | int: 资产数量 |

## 资产类

### Asset（基础资产类）

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `getPath()` | 获取资产路径 | 无 | std::string: 资产路径 |
| `getUUID()` | 获取资产UUID | 无 | std::string: 资产UUID |
| `getRefCount()` | 获取资产引用计数 | 无 | int: 引用计数 |
| `addRef()` | 增加资产引用计数 | 无 | void |
| `release()` | 减少资产引用计数，当引用计数为0时自动卸载 | 无 | void |

### TextureAsset（纹理资产）

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `getWidth()` | 获取纹理宽度 | 无 | int: 宽度 |
| `getHeight()` | 获取纹理高度 | 无 | int: 高度 |
| `getTextureId()` | 获取纹理ID | 无 | unsigned int: 纹理ID |

### MaterialAsset（材质资产）

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `getShader()` | 获取材质使用的着色器 | 无 | ShaderAsset*: 着色器资产指针 |
| `setTexture(const std::string& name, TextureAsset* texture)` | 设置材质纹理 | name: 纹理名称<br>texture: 纹理资产指针 | void |
| `setFloat(const std::string& name, float value)` | 设置材质浮点参数 | name: 参数名称<br>value: 浮点值 | void |
| `setVec3(const std::string& name, const Vec3& value)` | 设置材质向量参数 | name: 参数名称<br>value: 向量值 | void |

### SceneAsset（场景资产）

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `getScene()` | 获取场景实例 | 无 | Scene*: 场景指针 |