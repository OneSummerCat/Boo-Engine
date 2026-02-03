# 平台：Windows
## 运行
### 1. 执行vs项目前，先执行build.bat创建符号链接
    - 创建虚拟目录
    - mklink /J "editor" "../../editor"
    - mklink /J "engine" "../../engine"
    - 创建编辑器和引擎的虚拟目录指向真实目录，方便在 Visual Studio 中直接打开项目
### 2. 执行vs项目，双击.sln文件打开
### 3. C++ 语言标准
    - 属性-常规-C++语言标准： ISO C++20 标准 (/std:c++20)
### 4. 资源目录
    - 属性-生成事件-后期生成事件-命令行
```
if exist "$(TargetDir)resources" rmdir /s /q "$(TargetDir)resources"
if exist "$(ProjectDir)engine\resources" (
    xcopy /Y /I /E "$(ProjectDir)engine\resources\*" "$(TargetDir)resources\"
)

if exist "$(TargetDir)res" rmdir /s /q "$(TargetDir)res"
if exist "$(ProjectDir)editor\res" (
    xcopy /Y /I /E "$(ProjectDir)editor\res\*" "$(TargetDir)res\"
)
```
### 5. 三方库
1. glfw
    - 属性-C/C++-常规-附加包含目录：$(SolutionDir)libs/glfw/include
    - 属性-链接-常规-附加库目录：$(SolutionDir)libs/glfw/lib-vc2022
    - 属性-链接-输入-附加依赖项：glfw3.lib
2. stb
    - 属性-C/C++-常规-附加包含目录：$(SolutionDir)/engine/libs/stb
3. vulkan(现在本地安装vulkan-sdk)
    - 属性-C/C++-常规-附加包含目录：E:\software\vulkan\Include
    - 属性-链接-常规-附加库目录：E:\software\vulkan\Lib
    - 属性-链接-输入-附加依赖项：vulkan-1.lib;shaderc_combinedd.lib
4. freetype
    - 属性-C/C++-常规-附加包含目录：$(SolutionDir)/libs/freetype2/include
    - 属性-链接-常规-附加库目录：$(SolutionDir)/libs/freetype2/lib
    - 属性-链接-输入-附加依赖项：freetype.lib
5. nativefiledialog
    - 属性-C/C++-常规-附加包含目录：$(SolutionDir)/libs/nativefiledialog/include
    - 属性-链接-常规-附加库目录：$(SolutionDir)/libs/nativefiledialog/lib
    - 属性-链接-输入-附加依赖项：nfd.lib
