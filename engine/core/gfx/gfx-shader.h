#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
class GfxContext;

class GfxShader
{
private:
    GfxContext *_context;
    std::string _name;
    VkShaderModule _shaderModule;

    VkShaderModule _createShaderModule(const std::vector<char> &code);
public:
    GfxShader(GfxContext *context, const std::string &name);



    VkShaderModule getShaderModule(){
        return this->_shaderModule;
    }
    ~GfxShader();
};
