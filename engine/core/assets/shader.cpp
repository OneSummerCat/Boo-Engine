#include "shader.h"
#include "../gfx/gfx-mgr.h"
Shader::Shader(const std::string key, const std::string type,const std::string path) : Asset(key, path)
{
    this->_shaderType = type;
    this->_load();
}
void Shader::_load()
{
    std::ifstream file(this->_path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << this->_path << std::endl;
        return;
    }
    // tellg()返回当前定位指针的位置，也代表着输入流的大小。
    size_t fileSize = static_cast<size_t>(file.tellg());
    // 创建缓冲区并读取文件
    this->_shaderData.resize(fileSize);
    file.seekg(0);
    file.read(this->_shaderData.data(), fileSize);
    file.close();
    // GfxMgr::getInstance()->createShader(this->_key, this->_data);
    // // std::cout << "Loaded shader: " << this->_key << " from path: " << this->_data << std::endl;
    // // GfxMgr::getInstance()->createTexture(this->_key, this->_width, this->_height, this->_channels, &this->_pixelsVector);
}
void Shader::destroy()
{
}
