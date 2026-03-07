#include "shader-asset.h"
#include <filesystem>
#include "../gfx/gfx-mgr.h"

namespace Boo
{

    ShaderAsset::ShaderAsset() : Asset(),
                                 _glslData("")
    {
        this->_type = AssetType::Shader;
    }
    ShaderAsset::ShaderAsset(const std::string uuid) : Asset(uuid),
                                                       _glslData("")
    {
        this->_type = AssetType::Shader;
    }
    void ShaderAsset::create(std::string path)
    {
        Asset::create(path);
    }
    void ShaderAsset::create(const uint32_t *data)
    {
        this->_spirvData = std::vector<uint32_t>(data, data + sizeof(data) / sizeof(uint32_t));
        GfxMgr::getInstance()->createSpirvShader(this->_uuid, this->_spirvData);
    }

    void ShaderAsset::destroy()
    {
        GfxMgr::getInstance()->destroyShader(this->_uuid);
    }

} // namespace Boo
