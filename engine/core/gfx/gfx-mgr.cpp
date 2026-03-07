
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
    Gfx::context = new GfxContext();
    Gfx::context->init(window);
    this->_initRenderer();
}
void GfxMgr::init(Android *android)
{
    Gfx::context = new GfxContext();
    Gfx::context->init(android);
    this->_initRenderer();
}
void GfxMgr::_initRenderer()
{
   Gfx::renderer = new GfxRenderer();
   Gfx::renderer->init();
}
void GfxMgr::resize(int width, int height)
{
    Gfx::viewWidth = width;
    Gfx::viewHeight = height;
}
void GfxMgr::update(float dt)
{
    Gfx::time += dt;

    // Gfx::renderer->clearDestroyObjects();
    // std::cout << "renderer update" << std::endl;
    Gfx::context->frameFencesPrepare(this->_currentFrame);
    // std::cout << "renderer update1" << std::endl;
    /* // 可用的图像的索引 */
    uint32_t imageIndex;
    Gfx::renderer->frameRendererBefore();
    /**
     * 从交换链申请一个可渲染的图像
     * 通过 _imageAvailableSemaphores[_currentFrame] 信号量，通知 GPU："必须等这个信号量触发后，才能开始渲染该图像"。
     */
    VkResult result1 = Gfx::context->frameAcquireNextImage(&imageIndex, this->_currentFrame);
    /*  // 如果交换链已过期（如窗口大小改变），会返回 VK_ERROR_OUT_OF_DATE_KHR，触发重建交换链 */
    if (result1 == VK_ERROR_OUT_OF_DATE_KHR || result1 == VK_SUBOPTIMAL_KHR)
    {
        std::cout << "Gfx : Render :: update:'VK_ERROR_OUT_OF_DATE_KHR',The window size might have changed, and the swap chain needs to be recreated." << std::endl;
        this->resetSwapChain();
        return;
    }
    // std::cout << "renderer update2" << std::endl;
    /* // 检查当前索引图像是否被其他帧使用，若已被使用，则等待其他帧完成 */
    Gfx::context->frameWaitImageInUse(imageIndex, this->_currentFrame);
    // std::cout << "renderer update:'VK_SUCCESS',imageIndex:" << imageIndex << std::endl;
    /*  // 准备渲染buffer */
    std::vector<VkCommandBuffer> commandBuffers;
    Gfx::renderer->frameRenderer(imageIndex, commandBuffers);
    /*  // 提交渲染命令 */
    Gfx::context->frameSubmitCommands(imageIndex, commandBuffers, this->_currentFrame);
    /*  // 显示图像 */
    VkResult result2 = Gfx::context->framePresentFrame(imageIndex, this->_currentFrame);
    if (result2 == VK_ERROR_OUT_OF_DATE_KHR || result2 == VK_SUBOPTIMAL_KHR)
    {
        Gfx::renderer->frameRendererAfter();
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
    Gfx::renderer->frameRendererAfter();
}
/**
 * 关键时刻手动重置交换链
 */
void GfxMgr::resetSwapChain()
{
    vkDeviceWaitIdle(Gfx::context->getVkDevice()); /*  // 等待所有操作完成 */
    Gfx::renderer->_cleanRendererState();
    Gfx::context->cleanSwapChain();
    /*  // 后重置 */
    Gfx::context->resetSwapChain();
    Gfx::renderer->_resetRendererState();
    std::cout << "GfxMgr :reset swap chain end..." << std::endl;
}

void GfxMgr::createPipeline(std::string pipelineName, GfxPipelineStruct pipelineStruct)
{
    Gfx::renderer->createPipeline(pipelineName, pipelineStruct);
}

void GfxMgr::createTexture(std::string texture, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels)
{
    Gfx::renderer->createTexture(texture, width, height, channels, pixels);
}
void GfxMgr::destroyTexture(std::string texture)
{
    Gfx::renderer->destroyTexture(texture);
}
bool GfxMgr::isExistTexture(std::string texture)
{
    return Gfx::renderer->isExistTexture(texture);
}
void GfxMgr::createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, std::string> &macros)
{
    Gfx::renderer->createGlslShader(shaderName, shaderType, data, macros);
}
void GfxMgr::createSpirvShader(const std::string &shaderName, const std::vector<uint32_t> &data)
{
    Gfx::renderer->createSpirvShader(shaderName, data);
}
void GfxMgr::destroyShader(std::string shaderName)
{
    Gfx::renderer->destroyShader(shaderName);
}

void GfxMgr::initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture)
{
    Gfx::renderer->initRenderQueue(renderId, renderTexture);
}
void GfxMgr::delRenderQueue(std::string renderId)
{
    Gfx::renderer->delRenderQueue(renderId);
}
void GfxMgr::submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen)
{
    Gfx::renderer->submitRenderData(renderId, viewMatrix, projMatrix, isOnScreen);
}
void GfxMgr::submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh, std::vector<float> &instanceData)
{
    Gfx::renderer->submitRenderObject(renderId, material, mesh, instanceData);
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
    vkCreateBuffer(Gfx::context->getVkDevice(), &bufferCreateInfo, nullptr, buffer);

    VkMemoryRequirements memReqs;
    VkMemoryAllocateInfo memAlloc{};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkGetBufferMemoryRequirements(Gfx::context->getVkDevice(), *buffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, memoryPropertyFlags);
    vkAllocateMemory(Gfx::context->getVkDevice(), &memAlloc, nullptr, memory);

    if (data != nullptr)
    {
        void *mapped;
        vkMapMemory(Gfx::context->getVkDevice(), *memory, 0, size, 0, &mapped);
        memcpy(mapped, data, size);
        vkUnmapMemory(Gfx::context->getVkDevice(), *memory);
    }

    vkBindBufferMemory(Gfx::context->getVkDevice(), *buffer, *memory, 0);
    return VK_SUCCESS;
}
uint32_t GfxMgr::getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(Gfx::context->getPhysicalDevice(), &deviceMemoryProperties);
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