#include "asset.h"

namespace Boo
{

    Asset::Asset() : _type(AssetType::None),
                     _uuid(UuidUtil::generateUUID()),
                     _name(""),
                     _path("")
    {
    }
    Asset::Asset(std::string uuid) : _type(AssetType::None),
                                     _uuid(uuid),
                                     _name(""),
                                     _path("")
    {
    }
    void Asset::create(std::string path)
    {
        this->_path = path;
        // 获取资源文件名作为资源名
        this->_name = std::filesystem::path(path).stem().string();
    }
    void Asset::destroy()
    {
    }
    Asset::~Asset()
    {
    }

} // namespace Boo
