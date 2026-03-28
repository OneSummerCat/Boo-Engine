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
#include "default/gfx-default-renderer.h"
#include "builtin/gfx-builtin-renderer.h"

#include "../math/mat4.h"
#include "../../log.h"

GfxRenderer::GfxRenderer()
{
    // 所有ui 默认绑定4个采样器
    if (Gfx::_uiTestMesh == nullptr)
    {
        Gfx::_uiTestMesh = new GfxMesh("789abcde-f012-34a5-b678-901234567890");
        Gfx::_uiTestMesh->createUIMesh({-0.5f, 0.5f, 0.0f,
                                        -0.5f, -0.5f, 0.0f,
                                        0.5f, -0.5f, 0.0f,
                                        0.5f, 0.5f, 0.0f},
                                       {0.0f, 0.0f,
                                        0.0f, 1.0f,
                                        1.0f, 1.0f,
                                        1.0f, 0.0f},
                                       {0, 1, 2,
                                        0, 2, 3});
        // Gfx::_uiTestMesh->setInputVertices({-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        //                                    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        //                                    0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        //                                    0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
        //                                   {0, 1, 2, 0, 2, 3});
    }

    this->_defaultRenderer = new GfxDefaultRenderer("default");
    this->_builtinRenderer = new GfxBuiltinRenderer("builtin");
}
void GfxRenderer::init()
{
    Gfx::_bufferUBO = new GfxBufferUBO();
    Gfx::_bufferInstance = new GfxBufferInstance();
    this->_initDefaultTexture();
    this->_defaultRenderer->init();
    this->_builtinRenderer->init();
}
void GfxRenderer::_initDefaultTexture()
{
    const std::vector<uint8_t> pixels(GfxTextureDefault, GfxTextureDefault + sizeof(GfxTextureDefault));
    this->createTexture("Gfx::Texture::default.png", GfxTextureDefaultWidth, GfxTextureDefaultHeight, 1, &pixels, GfxTextureFormat::R8G8B8A8_SRGB);
}

void GfxRenderer::createPipeline(std::string name, GfxRendererState rendererState)
{
    this->_builtinRenderer->createPipeline(name, rendererState);
}

GfxTexture *GfxRenderer::createTexture(std::string uuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels, GfxTextureFormat format)
{
    if (Gfx::_textures.find(uuid) == Gfx::_textures.end())
    {
        VkFormat _format = VK_FORMAT_R8G8B8A8_UNORM;
        if (format == GfxTextureFormat::R8G8B8A8_SRGB)
        {
            _format = VK_FORMAT_R8G8B8A8_SRGB;
        }
        else if (format == GfxTextureFormat::R8G8B8A8_UNORM)
        {
            _format = VK_FORMAT_R8G8B8A8_UNORM;
        }
        else if (format == GfxTextureFormat::R8_UNORM)
        {
            _format = VK_FORMAT_R8_UNORM;
        }
        GfxTexture *texture = new GfxTexture(uuid);
        texture->create(pixels, width, height, channels, _format);
        Gfx::_textures[uuid] = texture;
    }
    try
    {
        return Gfx::_textures.at(uuid);
    }
    catch (const std::out_of_range &e)
    {
        LOGE("[Gfx : Renderer] :: createTexture:uuid:%s not found", uuid.c_str());
        return nullptr;
    }
}
void GfxRenderer::insertTexture(GfxTexture *texture)
{
    if (Gfx::_textures.find(texture->getUuid()) != Gfx::_textures.end())
    {
        LOGI("[Gfx : Renderer] :: insertTexture:uuid:%s already exists", texture->getUuid().c_str());
        return;
    }
    Gfx::_textures[texture->getUuid()] = texture;
}
void GfxRenderer::destroyTexture(GfxTexture *texture)
{
    if (texture == nullptr)
    {
        return;
    }
    if (Gfx::_textures.find(texture->getUuid()) != Gfx::_textures.end())
    {
        // 加入销毁队列
        this->_destroyTextureCaches.push_back(texture);
        Gfx::_textures.erase(texture->getUuid());
    }
}

GfxShader *GfxRenderer::createSpirvShader(const std::string &uuid, const std::vector<uint32_t> &data)
{
    if (Gfx::_shaders.find(uuid) == Gfx::_shaders.end())
    {
        GfxShader *shader = new GfxShader(uuid);
        shader->createShaderModule(data);
        Gfx::_shaders[uuid] = shader;
    }
    try
    {
        return Gfx::_shaders.at(uuid);
    }
    catch (const std::out_of_range &e)
    {
        LOGE("[Gfx : Renderer] :: createSpirvShader:uuid:%s not found", uuid.c_str());
        return nullptr;
    }
}
GfxShader *GfxRenderer::createGlslShader(const std::string &uuid, const std::string &shaderType, const std::string &data, const std::map<std::string, int> &macros)
{
    // 生成唯一的缓存键：shaderName + 宏定义
    std::stringstream newUuid;
    newUuid << uuid;
    if (!macros.empty())
    {
        newUuid << "[";
        bool first = true;
        for (const auto &[key, value] : macros)
        {
            if (!first)
            {
                newUuid << "|";
            }
            newUuid << key << ":" << value;
            first = false;
        }
        newUuid << "]";
    }

    //  检查是否已存在
    if (Gfx::_shaders.find(newUuid.str()) == Gfx::_shaders.end())
    {
        std::vector<uint32_t> spirvCode = this->_compileShaderGlslToSpirv(shaderType, newUuid.str(), data, macros);
        GfxShader *shader = new GfxShader(newUuid.str());
        shader->createShaderModule(spirvCode);
        Gfx::_shaders[newUuid.str()] = shader;
    }
    try
    {
        return Gfx::_shaders.at(newUuid.str());
    }
    catch (const std::out_of_range &e)
    {
        LOGE("[Gfx : Renderer] :: createGlslShader:uuid:%s not found", newUuid.str().c_str());
        return nullptr;
    }
}

void GfxRenderer::destroyShader(GfxShader *shader)
{
    if (shader == nullptr)
    {
        return;
    }
    if (Gfx::_shaders.find(shader->getUuid()) != Gfx::_shaders.end())
    {
        // 加入销毁队列
        this->_destroyShaderCaches.push_back(shader);
        Gfx::_shaders.erase(shader->getUuid());
    }
}
std::vector<uint32_t> GfxRenderer::_compileShaderGlslToSpirv(const std::string &type, const std::string &cacheKey, const std::string &source, const std::map<std::string, int> &macros)
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
        compileOptions.AddMacroDefinition(key, std::to_string(value));
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
        LOGI("[Gfx : Renderer] :: Shader compilation warnings for %s:\n%s", cacheKey.c_str(), result.GetErrorMessage().c_str());
    }
    std::vector<uint32_t> spirvCode(result.cbegin(), result.cend());
    LOGI("[Gfx : Renderer] :: Successfully compiled %s (%d SPIR-V words)", cacheKey.c_str(), (int)spirvCode.size());
    return spirvCode;
}
GfxMesh *GfxRenderer::createMesh(std::string meshUuid, const std::vector<float> &_positions, const std::vector<float> &_normals, const std::vector<float> &_uvs, const std::vector<float> &_uvs1, const std::vector<float> &_uvs2, const std::vector<float> &_colors, const std::vector<float> &_tangents, const std::vector<int> &_indices)
{
    if (Gfx::_meshes.find(meshUuid) == Gfx::_meshes.end())
    {
        // std::cout << "createMesh1: idx=" << meshUuid << std::endl;
        GfxMesh *mesh = new GfxMesh(meshUuid);
        // std::cout << "createMesh2: idx=" << meshUuid << std::endl;
        mesh->createMesh(_positions, _normals, _uvs, _uvs1, _uvs2, _colors, _tangents, _indices);
        // std::cout << "createMesh3: idx=" << meshUuid << std::endl;
        Gfx::_meshes[meshUuid] = mesh;
    }
    try
    {
        return Gfx::_meshes.at(meshUuid);
    }
    catch (const std::out_of_range &e)
    {
        LOGE("[Gfx : Renderer] :: createMesh:uuid:%s not found", meshUuid.c_str());
        return nullptr;
    }
}
void GfxRenderer::destroyMesh(GfxMesh *mesh)
{
    if (mesh == nullptr)
    {
        return;
    }
    if (Gfx::_meshes.find(mesh->getUuid()) != Gfx::_meshes.end())
    {
        // 加入销毁队列
        this->_destroyMeshCaches.push_back(mesh);
        Gfx::_meshes.erase(mesh->getUuid());
    }
}
GfxRenderTexture *GfxRenderer::createRenderTexture(std::string renderTextureUuid, uint32_t width, uint32_t height)
{
    GfxRenderTexture *renderTexture = new GfxRenderTexture(renderTextureUuid, width, height);
    return renderTexture;
}
void GfxRenderer::destroyRenderTexture(GfxRenderTexture *renderTexture)
{
    if (renderTexture == nullptr)
    {
        return;
    }
    // 加入销毁队列
    this->_destroyRenderTextureCaches.push_back(renderTexture);
}

void GfxRenderer::initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture, int priority)
{
    this->_builtinRenderer->initRenderQueue(renderId, renderTexture, priority);
}
void GfxRenderer::setRenderQueuePriority(std::string renderId, int priority)
{
    this->_builtinRenderer->setRenderQueuePriority(renderId, priority);
}
void GfxRenderer::delRenderQueue(std::string renderId)
{
    this->_builtinRenderer->delRenderQueue(renderId);
}
void GfxRenderer::submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen)
{
    this->_builtinRenderer->submitRenderData(renderId, viewMatrix, projMatrix, isOnScreen);
}
void GfxRenderer::submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh)
{
    this->_builtinRenderer->submitRenderObject(renderId, material, mesh);
}
/**
 * @brief 渲染前处理
 * 上一帧渲染完成，清空渲染目标，准备新帧渲染
 */
void GfxRenderer::frameRendererBefore()
{
    Gfx::_bufferUBO->clear();
    Gfx::_bufferInstance->clear();
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
    // 清空销毁渲染纹理队列
    for (auto &renderTexture : this->_destroyRenderTextureCaches)
    {
        renderTexture->destroy();
        delete renderTexture;
        renderTexture = nullptr;
    }
    this->_destroyRenderTextureCaches.clear();
    // 清空销毁网格队列
    for (auto &mesh : this->_destroyMeshCaches)
    {
        mesh->destroy();
        delete mesh;
        mesh = nullptr;
    }
    this->_destroyMeshCaches.clear();
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