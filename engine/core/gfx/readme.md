# 验证层(元层"（Meta-Layer）)
- 在较新的 Vulkan SDK（1.1.106.0+）中，Khronos 将多个独立的验证层合并成了一个统一的层：VK_LAYER_KHRONOS_validation
## 验证层功能
独立验证层                                      包含的功能 
VK_LAYER_GOOGLE_threading                      线程安全检查 
VK_LAYER_GOOGLE_unique_objects                 对象唯一性检查 
VK_LAYER_KHRONOS_validation                    参数验证 
VK_LAYER_LUNARG_assistant_layer                API使用建议 
VK_LAYER_LUNARG_core_validation                核心验证 
VK_LAYER_LUNARG_device_simulation              设备模拟 
VK_LAYER_LUNARG_monitor                        帧率监控 
VK_LAYER_LUNARG_screenshot                     截图功能 
VK_LAYER_LUNARG_vktrace                        API追踪

# 扩展
- 实例扩展：与 Vulkan 实例相关的功能，在创建 VkInstance 时加载。 作用于整个程序
- 设备扩展：与特定物理设备相关的功能，需要在创建 VkDevice 时加载。 作用于具体GPU
## 实例扩展
扩展名                                                       作用 
VK_KHR_SURFACE_EXTENSION_NAME                               通用表面支持（窗口表面） 
VK_KHR_SWAPCHAIN_EXTENSION_NAME                             交换链支持（ 注意：也可作为设备扩展 ） 
VK_EXT_DEBUG_UTILS_EXTENSION_NAME                           调试工具，生成更友好的调试信息 
VK_EXT_DEBUG_REPORT_EXTENSION_NAME                          旧版调试报告扩展 
VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME      查询GPU详细信息 
VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME               macOS/iOS便携性支持 
VK_KHR_ANDROID_SURFACE_EXTENSION_NAME                       Android原生表面 
VK_KHR_WIN32_SURFACE_EXTENSION_NAME                         Windows原生表面 
VK_MVK_MACOS_SURFACE_EXTENSION_NAME                         macOS原生表面（MoltenVK）
## 设备扩展
扩展名 作用 VK_KHR_SWAPCHAIN_EXTENSION_NAME                   交换链，显示图像到屏幕 
VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME                     动态渲染，无需渲染通道 
VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME                动态状态（视口、裁剪矩形等）
VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME            动态顶点输入 
VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME                       推送描述符 
VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME                着色器绘制参数 
VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME                    时间线信号量 
VK_KHR_MULTIVIEW_EXTENSION_NAME                             多视图渲染


# 渲染状态
```
Blend:1
DepthTest:0  深度测试
    UI:不启用深度测试
    Mask:不启用深度测试
DepthWrite:0  深度写入
    UI:不启用深度测试
    Mask:不启用深度测试
StencilTest:0  缓冲区的深度处理
    UI:不启用深度测试
    Mask:启用
ADD:
    compareOp: VK_COMPARE_OP_ALWAYS  总是通过测试
    passOp: VK_STENCIL_OP_INCREMENT_AND_WRAP  测试通过：值+1
SUBTRACT:
    compareOp: VK_COMPARE_OP_ALWAYS  总是通过测试
    passOp: VK_STENCIL_OP_DECREMENT_AND_WRAP  测试通过：值-1
REPLACE:
    compareOp: VK_COMPARE_OP_ALWAYS  总是通过测试
    passOp: VK_STENCIL_OP_REPLACE  测试通过：替换为参考值
front: 正面三角形（逆时针）的Stencil操作
    compareOp: VK_COMPARE_OP_ALWAYS  总是通过测试
    failOp: VK_STENCIL_OP_KEEP  测试失败：保持原值
    depthFailOp: VK_STENCIL_OP_KEEP  深度测试失败：保持原值
    passOp: VK_STENCIL_OP_INCREMENT_AND_WRAP  测试通过：值+1
    compareMask: 0xFF  比较所有位
    writeMask: 0xFF  写入所有位
    reference: 1  参考值（会被动态覆盖）
back: 背面三角形（顺时针）的Stencil操作
    compareOp: VK_COMPARE_OP_ALWAYS  总是通过测试
    failOp: VK_STENCIL_OP_KEEP  测试失败：保持原值
    depthFailOp: VK_STENCIL_OP_KEEP  深度测试失败：保持原值
    passOp: VK_STENCIL_OP_DECREMENT_AND_WRAP  测试通过：值-1
    compareMask: 0xFF  比较所有位
    writeMask: 0xFF  写入所有位
    reference: 1  参考值（会被动态覆盖）
DepthCompareOp
    VK_COMPARE_OP_NEVER 永远不通过深度测试
    VK_COMPARE_OP_LESS  当前深度 < 缓冲区深度 → 通过  标准深度测试，近处物体遮挡远处物体
    VK_COMPARE_OP_EQUAL  当前深度 = 缓冲区深度 → 通过  特殊效果，如深度相等高亮
    VK_COMPARE_OP_LESS_OR_EQUAL  当前深度 ≤ 缓冲区深度 → 通过  阴影映射、某些后处理效果
    VK_COMPARE_OP_GREATER  当前深度 > 缓冲区深度 → 通过  反转深度测试、特殊效果
    VK_COMPARE_OP_NOT_EQUAL  当前深度 ≠ 缓冲区深度 → 通过  轮廓效果、剔除特定深度
    VK_COMPARE_OP_GREATER_OR_EQUAL  当前深度 ≥ 缓冲区深度 → 通过  反射、门户效果
    VK_COMPARE_OP_ALWAYS  永远通过深度测试  UI渲染、天空盒、透明物体

vert:"resources/shader/ui/ui-mask.vert.spv"
frag:"resources/shader/ui/ui-mask.frag.spv"
rasterizerDiscardEnable:0   // 启用光栅化丢弃，将所有片段都丢弃
polygonMode:
    VK_POLYGON_MODE_FILL   // 填充模式
    VK_POLYGON_MODE_LINE（线框模式）
    VK_POLYGON_MODE_POINT（点模式）
cullMode:
    VK_CULL_MODE_BACK_BIT   // 背面剔除
    VK_CULL_MODE_FRONT_BIT  // 正面剔除
    VK_CULL_MODE_NONE       // 不剔除
frontFace:
    VK_FRONT_FACE_COUNTER_CLOCKWISE   // 逆时针为正面
    VK_FRONT_FACE_CLOCKWISE          // 顺时针为正面

```
# 多个实例缓冲区
```
// Binding 1: 变换矩阵数据（固定大小 16 * sizeof(float)）
this->_vInputBindInstanceDescription[0].binding = 1;
this->_vInputBindInstanceDescription[0].stride = sizeof(float) * 16;
this->_vInputBindInstanceDescription[0].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

// Binding 2: 颜色数据（固定大小 4 * sizeof(float)）
this->_vInputBindInstanceDescription[1].binding = 2;
this->_vInputBindInstanceDescription[1].stride = sizeof(float) * 4;
this->_vInputBindInstanceDescription[1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

// Binding 3: 动态数据（使用最大长度或单独处理）
this->_vInputBindInstanceDescription[2].binding = 3;
this->_vInputBindInstanceDescription[2].stride = maxDynamicDataSize;
this->_vInputBindInstanceDescription[2].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
```

# 纹理格式
sRGB：VK_FORMAT_R8G8B8A8_SRGB 用于颜色纹理（UI、漫反射贴图等），颜色会更准确
UNORM：VK_FORMAT_R8G8B8A8_UNORM 用于数据纹理（法线贴图、高度图等）或需要线性计算的情况
R8_UNORM：VK_FORMAT_R8_UNORM 用于单通道纹理（如深度纹理）

# 图像格式
这三个是Vulkan中的图像格式，主要区别在于通道数和用途：
格式对比
格式	                                通道数	                每像素字节	             说明
VK_FORMAT_R32G32B32A32_SFLOAT	       RGBA (4通道)	           16字节	                红、绿、蓝、Alpha，各32位浮点
VK_FORMAT_R32G32B32_SFLOAT	           RGB (3通道)	           12字节	                红、绿、蓝，各32位浮点
VK_FORMAT_R32G32_SFLOAT	               RG (2通道)	           8字节	                红、绿，各32位浮点
性能考虑
格式	                                带宽消耗	                                适用场景
R32G32B32A32_SFLOAT                    最高	                                       对精度要求极高的场景，避免频繁使用
R32G32B32_SFLOAT                 	   中等	                                       通用3D数据，但注意某些GPU对此格式支持不佳
R32G32_SFLOAT                 	       最低	                                       UV坐标等2D数据的最佳选择
对于颜色渲染目标，通常不需要R32G32B32A32_SFLOAT的精度：
HDR渲染：使用 VK_FORMAT_R16G16B16A16_SFLOAT (8字节)
常规颜色：使用 VK_FORMAT_R8G8B8A8_UNORM (4字节)
对于顶点数据：
位置：可使用 VK_FORMAT_R32G32B32_SFLOAT 或压缩为 R16G16B16_SFLOAT
UV：使用 VK_FORMAT_R32G32_SFLOAT 或 R16G16_SFLOAT
法线：可使用 VK_FORMAT_R32G32B32_SFLOAT 或压缩为 R10G10B10A2_UNORM