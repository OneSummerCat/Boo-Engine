#include "gfx-renderer.h"
#include "gfx.h"
#include "gfx-context.h"
#include "base/gfx-shader.h"
#include "base/gfx-texture.h"
#include "base/gfx-render-pass.h"
#include "base/gfx-material.h"
#include "base/gfx-mesh.h"
#include "base/gfx-buffer-ubo.h"
#include "base/gfx-buffer-instance.h"
#include "base/gfx-render-texture.h"
#include "default/gfx-renderer-default.h"
#include "builtin/gfx-renderer-builtin.h"

#include "../math/mat4.h"

GfxRenderer::GfxRenderer()
{
    // 所有ui 默认绑定4个采样器
    if (Gfx::uiTestMesh == nullptr)
    {
        Gfx::uiTestMesh = new GfxMesh("789abcde-f012-34a5-b678-901234567890");
        Gfx::uiTestMesh->setInputVertices({-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
                                           -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
                                           0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
                                           0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
                                          {0, 1, 2, 0, 2, 3});
    }

    this->_defaultRenderer = new GfxRendererDefault("default");
    this->_builtinRenderer = new GfxRendererBuiltin("builtin");
}
void GfxRenderer::init()
{
    Gfx::bufferUBO = new GfxBufferUBO();
    Gfx::bufferInstance = new GfxBufferInstance();
    this->_defaultRenderer->init();
    this->_builtinRenderer->init();
}

void GfxRenderer::createPipeline(std::string name, GfxPipelineStruct pipelineStruct)
{
    this->_builtinRenderer->createPipeline(name, pipelineStruct);
}

void GfxRenderer::createTexture(std::string textureUuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels)
{
    if (Gfx::textures.find(textureUuid) == Gfx::textures.end())
    {
        GfxTexture *texture = new GfxTexture(textureUuid, pixels, width, height, channels);
        Gfx::textures[textureUuid] = texture;
    }
    else
    {
        std::cout << "Gfx : Renderer :: createTexture:uuid:" << textureUuid << " already exists" << std::endl;
    }
}
void GfxRenderer::insertTexture(std::string textureUuid, GfxTexture *texture)
{
    if (Gfx::textures.find(textureUuid) != Gfx::textures.end())
    {
        std::cout << "Gfx : Renderer :: insertTexture:uuid:" << textureUuid << " already exists" << std::endl;
        return;
    }
    Gfx::textures[textureUuid] = texture;
}
void GfxRenderer::destroyTexture(std::string textureUuid)
{
    if (Gfx::textures.find(textureUuid) != Gfx::textures.end())
    {
        // 加入销毁队列
        this->_destroyTextureCaches.push_back(Gfx::textures[textureUuid]);
        Gfx::textures.erase(textureUuid);
    }
}
bool GfxRenderer::isExistTexture(std::string textureUuid)
{
    return Gfx::textures.find(textureUuid) != Gfx::textures.end();
}
GfxTexture *GfxRenderer::getTexture(std::string uuid)
{
    if (uuid.empty())
    {
        return nullptr;
    }
    if (Gfx::textures.find(uuid) == Gfx::textures.end())
    {
        std::cout << "Gfx : Renderer :: Texture not found:" << uuid << std::endl;
        return nullptr;
    }
    return Gfx::textures.at(uuid);
}
void GfxRenderer::createSpirvShader(const std::string &shaderName, const std::vector<uint32_t> &data)
{
    if (Gfx::shaders.find(shaderName) != Gfx::shaders.end())
    {
        std::cout << "Gfx : Renderer :: Shader already exists: " << shaderName << std::endl;
        return;
    }
    GfxShader *shader = new GfxShader(shaderName);
    shader->createShaderModule(data);
    Gfx::shaders[shaderName] = shader;
}
void GfxRenderer::createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, std::string> &macros)
{
    // 生成唯一的缓存键：shaderName + 宏定义
    std::stringstream cacheKey;
    cacheKey << shaderName;
    if (!macros.empty())
    {
        cacheKey << "[";
        bool first = true;
        for (const auto &[key, value] : macros)
        {
            if (!first)
            {
                cacheKey << "|";
            }
            cacheKey << key << ":" << value;
            first = false;
        }
        cacheKey << "]";
    }

    std::string finalCacheKey = cacheKey.str();
    //  检查是否已存在
    if (Gfx::shaders.find(finalCacheKey) != Gfx::shaders.end())
    {
        std::cout << "Gfx : Renderer :: Shader already exists: " << finalCacheKey << std::endl;
        return;
    }
    // 创建着色器
    try
    {
        std::vector<uint32_t> spirvCode = this->_compileShaderGlslToSpirv(shaderType, finalCacheKey, data, macros);
        GfxShader *shader = new GfxShader(finalCacheKey);
        shader->createShaderModule(spirvCode);
        Gfx::shaders[finalCacheKey] = shader;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to create shader '" << finalCacheKey << "': " << e.what() << std::endl;
        // 可以考虑抛出异常或返回错误码
    }
}

void GfxRenderer::destroyShader(std::string shaderName)
{
    if (Gfx::shaders.find(shaderName) != Gfx::shaders.end())
    {
        // 加入销毁队列
        this->_destroyShaderCaches.push_back(Gfx::shaders[shaderName]);
        Gfx::shaders.erase(shaderName);
    }
}
std::vector<uint32_t> GfxRenderer::_compileShaderGlslToSpirv(const std::string &type, const std::string &cacheKey, const std::string &source, const std::map<std::string, std::string> &macros)
{
    shaderc::Compiler _compiler;
    // 配置编译选项
    shaderc::CompileOptions compileOptions;
    // 设置目标环境
    compileOptions.SetTargetEnvironment(
        shaderc_target_env_vulkan,
        shaderc_env_version_vulkan_1_0);
    // 优化级别
    compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
    // 生成调试信息
    compileOptions.SetGenerateDebugInfo();
    // 添加宏定义
    // 添加通用宏定义
    compileOptions.AddMacroDefinition("GL_SPIRV", "1");
    compileOptions.AddMacroDefinition("VULKAN", "100");
    for (const auto &[key, value] : macros)
    {
        compileOptions.AddMacroDefinition(key, value);
    }
    // compileOptions.AddMacroDefinition("GL_SPIRV", "1");
    // compileOptions.AddMacroDefinition("VULKAN", "100");

    shaderc::SpvCompilationResult result;
    if (type == "Vert")
    {
        result = _compiler.CompileGlslToSpv(
            source.c_str(), shaderc_vertex_shader, cacheKey.c_str(), compileOptions);
    }
    else if (type == "Frag")
    {
        result = _compiler.CompileGlslToSpv(
            source.c_str(), shaderc_fragment_shader, cacheKey.c_str(), compileOptions);
    }
    else
    {
        throw std::runtime_error("Shader type not supported");
    }
    shaderc_compilation_status status = result.GetCompilationStatus();
    if (status != shaderc_compilation_status_success)
    {
        std::string errorMsg = "Shader compilation failed:\n";
        errorMsg += "File: " + cacheKey + "\n";
        errorMsg += "Error: " + result.GetErrorMessage();
        errorMsg += "Status: " + std::to_string(status);

        throw std::runtime_error(errorMsg);
    }
    // 输出警告信息
    if (result.GetNumWarnings() > 0)
    {
        std::cout << "Gfx : Renderer :: Shader compilation warnings for " << cacheKey << ":\n"
                  << result.GetErrorMessage() << std::endl;
    }
    std::vector<uint32_t> spirvCode(result.cbegin(), result.cend());
    std::cout << "Gfx : Renderer :: Successfully compiled " << cacheKey
              << " (" << spirvCode.size() << " SPIR-V words)" << std::endl;
    return spirvCode;
}
void GfxRenderer::initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture)
{
    this->_builtinRenderer->initRenderQueue(renderId, renderTexture);
}
void GfxRenderer::delRenderQueue(std::string renderId)
{
    this->_builtinRenderer->delRenderQueue(renderId);
}
void GfxRenderer::submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen)
{
    this->_builtinRenderer->submitRenderData(renderId, viewMatrix, projMatrix, isOnScreen);
}
void GfxRenderer::submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh, std::vector<float> &instanceData)
{
    this->_builtinRenderer->submitRenderObject(renderId, material, mesh, instanceData);
}
/**
 * @brief 渲染前处理
 * 上一帧渲染完成，清空渲染目标，准备新帧渲染
 */
void GfxRenderer::frameRendererBefore()
{
    Gfx::bufferUBO->clear();
    Gfx::bufferInstance->clear();
    this->_defaultRenderer->frameRendererBefore();
    this->_builtinRenderer->frameRendererBefore();
    // 清空销毁队列
    this->_clearDestroyCaches();
}
void GfxRenderer::frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
    // 先获取上一帧的离屏渲染输出提交到默认队列
    std::vector<std::string> pipelineOutds;
    this->_builtinRenderer->getOffScreenOutds(pipelineOutds);
    this->_defaultRenderer->frameRenderer(imageIndex, commandBuffers, pipelineOutds);

    // 离屏渲染队列
    this->_builtinRenderer->frameRenderer(imageIndex, commandBuffers);
}
void GfxRenderer::frameRendererAfter()
{
    this->_defaultRenderer->frameRendererAfter();
    this->_builtinRenderer->frameRendererAfter();
}
void GfxRenderer::_clearDestroyCaches()
{
    // 清空销毁纹理队列
    for (auto &texture : this->_destroyTextureCaches)
    {
        texture->destroy();
        delete texture;
        texture = nullptr;
    }
    this->_destroyTextureCaches.clear();
    // 清空销毁着色器队列
    for (auto &shader : this->_destroyShaderCaches)
    {
        shader->destroy();
        delete shader;
        shader = nullptr;
    }
    this->_destroyShaderCaches.clear();
}
void GfxRenderer::_cleanRendererState()
{
    this->_defaultRenderer->_cleanRendererState();
}
void GfxRenderer::_resetRendererState()
{
    this->_defaultRenderer->_resetRendererState();
}


GfxRenderer::~GfxRenderer()
{
}