#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

class GfxShader
{
private:
    std::string _uuid;
    VkShaderModule _shaderModule;

public:
    GfxShader( const std::string &uuid);
    void createShaderModule(const std::vector<uint32_t> &code);
    VkShaderModule getShaderModule();
    const std::string & getUuid();

    void destroy();
    ~GfxShader();
};
