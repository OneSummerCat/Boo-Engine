#include "shader-asset.h"
#include <filesystem>
#include "../gfx/gfx-mgr.h"
#include "../gfx/base/gfx-shader.h"

namespace Boo
{
    ShaderAsset::ShaderAsset()
    {
        this->_type = AssetType::Shader;
    }
    ShaderAsset::ShaderAsset(std::string uuid) : Asset(uuid)
    {
        this->_type = AssetType::Shader;
    }

    ShaderAsset::ShaderAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name),
                                                                                     _glslData("")
    {
        this->_type = AssetType::Shader;
    }

    void ShaderAsset::create(const std::string &type, const std::string &glslData, const std::map<std::string, int> &macros)
    {
        this->_glslData = glslData;
        this->gfxShader = GfxMgr::getInstance()->createGlslShader(this->_uuid, type, this->_glslData, macros);
    }
    // void ShaderAsset::create(const std::vector<uint32_t> &data)
    // {
        //     this->gfxShader = GfxMgr::getInstance()->createSpirvShader(this->_uuid, data);
    // }

    void ShaderAsset::destroy()
    {
        GfxMgr::getInstance()->destroyShader(this->gfxShader);
        this->gfxShader = nullptr;
    }

} // namespace Boo
