# Boo Engine 跨平台构建文档

## 📋 目录

1. **环境要求**
2. **macOS 构建步骤**
3. **Windows 构建步骤**
4. **构建产物说明**
5. **常见问题解决**

## 1. 环境要求
| 平台 | 必需工具 |
|------|----------|
| macOS | CMake 3.15+、Xcode 命令行工具 |
| Windows | CMake 3.15+、 Visual Studio |


## 2. macOS 构建步骤

### 2.1 含三方头文件（默认）
```bash
# 创建构建目录
# 配置 CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 构建项目
cmake --build build

# 安装到 dist 目录
cmake --install build --prefix ./dist
```

### 2.2 不含三方头文件
```bash
# 配置 CMake（关闭三方头文件安装）
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBOO_INSTALL_THIRDPARTY_HEADERS=OFF

# 构建项目
cmake --build build

# 安装到 dist 目录
cmake --install build --prefix ./dist
```


## 3. Windows 构建步骤

### 3.1 Visual Studio 构建（生成 .lib 文件）
```powershell
# 创建构建目录
# mkdir build
# cd build
# 配置 CMake（指定 Visual Studio 生成器）
# Visual Studio 2022 示例
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
# 构建项目（Release 配置）
cmake --build build --config Release
# 安装到 dist 目录
cmake --install build --prefix ./dist --config Release
```


## 4. 构建产物说明

### 4.1 生成的文件结构
```
dist/
├── include/
│   ├── engine/           # 引擎自身头文件
│   │   ├── core/
│   │   ├── platforms/
│   │   ├── boo.h
│   │   ├── engine.h
│   │   └── log.h
│   └── libs/             # 三方库头文件（仅当 BOO_INSTALL_THIRDPARTY_HEADERS=ON 时）
│       ├── fastgltf/
│       ├── freetype2/
│       ├── glfw/
│       ├── glslang/
│       ├── nlohmann/
│       ├── simdjson/
│       ├── stb/
│       ├── volk/
│       └── vulkan/
├── libs/                 # 预编译库
│   ├── glfw/             # GLFW 库
│   ├── freetype2/        # FreeType 库
│   ├── glslang/          # GLSLang 库
│   └── libBooEngine.a    # MinGW 生成的静态库
│       或
│       BooEngine.lib      # Visual Studio 生成的静态库
└── CMakeLists.txt        # 示例 CMake 配置文件
```

### 4.2 平台特定文件
- **macOS**：`libBooEngine.a`（静态库）
- **Windows (Visual Studio)**：`BooEngine.lib`（静态库）