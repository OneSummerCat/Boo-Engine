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
    Asset::Asset(std::string uuid, std::string path, std::string name) : _type(AssetType::None),
                                                                         _uuid(uuid),
                                                                         _name(name),
                                                                         _path(path)
    {
    }
    void Asset::destroy()
    {
    }
    Asset::~Asset()
    {
    }

} // namespace Boo

