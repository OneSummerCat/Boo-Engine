#include "asset.h"
Asset::Asset(const std::string key, const std::string path)
{
    this->_key = key;
    this->_path = path;
    this->_type = AssetType::None;
}

Asset::~Asset()
{
}
