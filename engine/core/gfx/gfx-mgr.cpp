
#define GLFW_INCLUDE_VULKAN
#include "gfx.h"
#include "gfx-mgr.h"
#include "gfx-context.h"
#include "gfx-renderer.h"

#include "base/gfx-material.h"
#include "base/gfx-mesh.h"
#include "base/gfx-render-texture.h"
#include "../../log.h"

GfxMgr *GfxMgr::getInstance()
{
    static GfxMgr _instance;
    return &_instance;
}

GfxMgr::GfxMgr()
{
    uint32_t instanceVersion = 0;
    VkResult result = vkEnumerateInstanceVersion(&instanceVersion);
    if (result == VK_SUCCESS)
    {
        uint32_t major = VK_VERSION_MAJOR(instanceVersion);
        uint32_t minor = VK_VERSION_MINOR(instanceVersion);
        uint32_t patch = VK_VERSION_PATCH(instanceVersion);
        LOGI("Gfx :: Vulkan Instance (Driver) Version:%d.%d.%d", major, minor, patch);
    }
}

void GfxMgr::init(Window *window)
{
    Gfx::_context = new GfxContext();
    Gfx::_context->init(window);
    this->_initRenderer();
}
void GfxMgr::init(Android *android)
{
    Gfx::_context = new GfxContext();
    Gfx::_context->init(android);
    this->_initRenderer();
}
void GfxMgr::_initRenderer()
{
    Gfx::_renderer = new GfxRenderer();
    Gfx::_renderer->init();
}
void GfxMgr::resize(int width, int height)
{
    Gfx::_viewWidth = width;
    Gfx::_viewHeight = height;
}
void GfxMgr::update(float dt)
{
    Gfx::_time += dt;
    Gfx::_drawCount = 0;
    this->_preFrameTime = std::chrono::high_resolution_clock::now();
    Gfx::_context->frameFencesPrepare(this->_currentFrame);
    /* // 可用的图像的索引 */
    uint32_t imageIndex;
    /**
     * 从交换链申请一个可渲染的图像
     * 通过 _imageAvailableSemaphores[_currentFrame] 信号量，通知 GPU："必须等这个信号量触发后，才能开始渲染该图像"。
     */
    VkResult result1 = Gfx::_context->frameAcquireNextImage(&imageIndex, this->_currentFrame);
    /*  // 如果交换链已过期（如窗口大小改变），会返回 VK_ERROR_OUT_OF_DATE_KHR，触发重建交换链 */
    if (result1 == VK_ERROR_OUT_OF_DATE_KHR || result1 == VK_SUBOPTIMAL_KHR)
    {
        std::cout << "Gfx : Render :: update:'VK_ERROR_OUT_OF_DATE_KHR',The window size might have changed, and the swap chain needs to be recreated." << std::endl;
        this->resetSwapChain();
        return;
    }
    /* // 检查当前索引图像是否被其他帧使用，若已被使用，则等待其他帧完成 */
    Gfx::_context->frameWaitImageInUse(imageIndex, this->_currentFrame);
    Gfx::_renderer->frameRendererBefore();
    /*  // 准备渲染buffer */
    std::vector<VkCommandBuffer> commandBuffers;
    Gfx::_renderer->frameRenderer(imageIndex, commandBuffers);
    /*  // 提交渲染命令 */
    Gfx::_context->frameSubmitCommands(imageIndex, commandBuffers, this->_currentFrame);
    /*  // 显示图像 */
    VkResult result2 = Gfx::_context->framePresentFrame(imageIndex, this->_currentFrame);
    if (result2 == VK_ERROR_OUT_OF_DATE_KHR || result2 == VK_SUBOPTIMAL_KHR)
    {
        Gfx::_renderer->frameRendererAfter();
        this->resetSwapChain();
        return;
    }
    else if (result2 != VK_SUCCESS)
    {
        std::cout << "Gfx : Render :: Failed to present swap chain imag22222e!" << std::endl;
    }
    /*  // 0 1 0 1 0 1 */
    /**
     * 帧0	提交命令，信号量A	开始渲染帧0
     * 帧1	提交命令，信号量B	渲染帧0完成，开始渲染帧1
     * 帧0	等待帧0栅栏，信号量A复用	渲染帧1完成，显示帧0
     */
    this->_currentFrame = (this->_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    Gfx::_renderer->frameRendererAfter();
    // 计算当前帧的渲染提交时间
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - this->_preFrameTime);
    this->_preFrameTime = currentTime;
    long long renderTimeNano = duration.count(); // 这是纳秒 ，中间还有微妙，需要转换为毫秒
    Gfx::_renderTime = renderTimeNano / 1000.0f / 1000.0f;
}
/**
 * 关键时刻手动重置交换链
 */
void GfxMgr::resetSwapChain()
{
    vkDeviceWaitIdle(Gfx::_context->getVkDevice()); /*  // 等待所有操作完成 */
    Gfx::_renderer->_cleanRendererState();
    Gfx::_context->cleanSwapChain();
    /*  // 后重置 */
    Gfx::_context->resetSwapChain();
    Gfx::_renderer->_resetRendererState();
    std::cout << "GfxMgr :reset swap chain end..." << std::endl;
}

void GfxMgr::createPipeline(std::string pipelineName, GfxRendererState rendererState)
{
    Gfx::_renderer->createPipeline(pipelineName, rendererState);
}

GfxTexture* GfxMgr::createTexture(std::string textureUuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels, GfxTextureFormat format)
{
    return Gfx::_renderer->createTexture(textureUuid, width, height, channels, pixels, format);
}
void GfxMgr::destroyTexture(GfxTexture *texture)
{
    Gfx::_renderer->destroyTexture(texture);
}

GfxShader* GfxMgr::createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, int> &macros)
{
    return Gfx::_renderer->createGlslShader(shaderName, shaderType, data, macros);
}
GfxShader* GfxMgr::createSpirvShader(const std::string &shaderName, const std::vector<uint32_t> &data)
{
    return Gfx::_renderer->createSpirvShader(shaderName, data);
}
void GfxMgr::destroyShader(GfxShader *shader)
{
    Gfx::_renderer->destroyShader(shader);
}
GfxMesh* GfxMgr::createMesh(std::string meshUuid, const std::vector<float> &_positions, const std::vector<float> &_normals, const std::vector<float> &_uvs, const std::vector<float> &_uvs1, const std::vector<float> &_uvs2, const std::vector<float> &_colors, const std::vector<float> &_tangents, const std::vector<int> &_indices)
{
    return Gfx::_renderer->createMesh(meshUuid, _positions, _normals, _uvs, _uvs1, _uvs2, _colors, _tangents, _indices);
}
void GfxMgr::destroyMesh(GfxMesh *mesh)
{
    Gfx::_renderer->destroyMesh(mesh);
}

GfxRenderTexture* GfxMgr::createRenderTexture(std::string renderTextureUuid, uint32_t width, uint32_t height)
{
    return Gfx::_renderer->createRenderTexture(renderTextureUuid, width, height);
}
void GfxMgr::destroyRenderTexture(GfxRenderTexture *renderTexture)
{
    Gfx::_renderer->destroyRenderTexture(renderTexture);
}



void GfxMgr::initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture, int priority)
{
    Gfx::_renderer->initRenderQueue(renderId, renderTexture, priority);
}
void GfxMgr::setRenderQueuePriority(std::string renderId, int priority)
{
    Gfx::_renderer->setRenderQueuePriority(renderId, priority);
}
void GfxMgr::delRenderQueue(std::string renderId)
{
    Gfx::_renderer->delRenderQueue(renderId);
}
void GfxMgr::submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen)
{
    Gfx::_renderer->submitRenderData(renderId, viewMatrix, projMatrix, isOnScreen);
}
void GfxMgr::submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh)
{
    Gfx::_renderer->submitRenderObject(renderId, material, mesh);
}
std::vector<char> GfxMgr::readShaderFile(const std::string &filename)
{
    /*   // ate: 表示从文件末未开始读取
      // binary: 表示以二进制方式读取 */
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    /*    // std::cout << filename << std::endl; */
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }
    /*  // tellg()返回当前定位指针的位置，也代表着输入流的大小。 */
    size_t fileSize = (size_t)file.tellg();
    /*  // std::cout << fileSize << std::endl; */
    std::vector<char> buffer(fileSize);
    /*  // seekg()是对输入流的操作 g是get缩写，0是代表从开头读起。 */
    file.seekg(0);
    /* // 读入到Buffer当中 */
    file.read(buffer.data(), fileSize);
    file.close();
    /* // std::cout << "read file success: " << filename << std::endl; */
    return buffer;
}

VkResult GfxMgr::createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size, void *data = nullptr)
{
    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.usage = usageFlags;
    bufferCreateInfo.size = size;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkCreateBuffer(Gfx::_context->getVkDevice(), &bufferCreateInfo, nullptr, buffer);

    VkMemoryRequirements memReqs;
    VkMemoryAllocateInfo memAlloc{};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkGetBufferMemoryRequirements(Gfx::_context->getVkDevice(), *buffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, memoryPropertyFlags);
    vkAllocateMemory(Gfx::_context->getVkDevice(), &memAlloc, nullptr, memory);

    if (data != nullptr)
    {
        void *mapped;
        vkMapMemory(Gfx::_context->getVkDevice(), *memory, 0, size, 0, &mapped);
        memcpy(mapped, data, size);
        vkUnmapMemory(Gfx::_context->getVkDevice(), *memory);
    }

    vkBindBufferMemory(Gfx::_context->getVkDevice(), *buffer, *memory, 0);
    return VK_SUCCESS;
}
uint32_t GfxMgr::getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(Gfx::_context->getPhysicalDevice(), &deviceMemoryProperties);
    for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((typeBits & 1) == 1)
        {
            if ((deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }
        typeBits >>= 1;
    }
    return 0;
}
GfxMgr::~GfxMgr()
{
}