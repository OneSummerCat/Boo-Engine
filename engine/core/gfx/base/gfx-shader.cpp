
#include "gfx-shader.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../../log.h"



GfxShader::GfxShader(const std::string &uuid):  _uuid(uuid)
{
}

// 要将着色器字节码在管线上使用，还需要使用VkShaderModule转换
void GfxShader::createShaderModule(const std::vector<uint32_t> &code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size() * sizeof(uint32_t);
    createInfo.pCode = code.data();
    /*VkShaderModule shaderModule;*/
    if (vkCreateShaderModule(Gfx::_context->getVkDevice(), &createInfo, nullptr, &this->_shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module!");
    }
    LOGI("[Gfx : Shader]:: create shader module success %s", this->_uuid.c_str());
}

VkShaderModule GfxShader::getShaderModule()
{
    return this->_shaderModule;
}

const std::string & GfxShader::getUuid()
{
    return this->_uuid;
}

void GfxShader::destroy(){
     if (this->_shaderModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(Gfx::_context->getVkDevice(), this->_shaderModule, nullptr);
        this->_shaderModule = VK_NULL_HANDLE;
    }
}

GfxShader::~GfxShader()
{
   
}

// void GfxShader::createShaderModule(const uint32_t *data, const uint32_t size)
// {
//     VkShaderModuleCreateInfo createInfo{};
//     createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//     createInfo.codeSize = size;
//     createInfo.pCode = data;
//     if (vkCreateShaderModule(Gfx::_context->getVkDevice(), &createInfo, nullptr, &this->_shaderModule) != VK_SUCCESS)
//     {
//         throw std::runtime_error("Failed to create shader module!");
//     }
//     std::cout << "[Gfx : Shader]:: create shader module success " << this->_name << std::endl;
// }

// void GfxShader::createShaderModule(const std::vector<char> &code)
// {
//     VkShaderModuleCreateInfo createInfo{};
//     createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//     createInfo.codeSize = code.size();
//     createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
//     if (vkCreateShaderModule(Gfx::_context->getVkDevice(), &createInfo, nullptr, &this->_shaderModule) != VK_SUCCESS)
//     {
//         throw std::runtime_error("Failed to create shader module!");
//     }
//     std::cout << "[Gfx : Shader]:: create shader module success " << this->_name << std::endl;
// }

