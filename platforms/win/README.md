# 平台：Windows
## 创建虚拟目录
mklink /J "editor" "../../editor"
mklink /J "engine" "../../engine"
- 创建编辑器和引擎的虚拟目录指向真实目录，方便在 Visual Studio 中直接打开项目