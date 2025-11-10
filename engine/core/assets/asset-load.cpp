#include "asset-load.h"
#include "asset.h"
#include "texture.h"
#include <filesystem>
#include <iostream>
#include "shader.h"
#include "assets-manager.h"
#include "asset-cache.h"
#include "asset-task.h"
#include "../utils/time-util.h"

AssetLoad::AssetLoad(AssetsManager *mgr)
{
    this->_mgr = mgr;
    this->_cache = new AssetCache();
}
Asset *AssetLoad::load(const std::string path)
{
    // 从缓存中获取资产
    std::filesystem::path key = std::filesystem::path(path);
    std::string normPath = key.generic_string();
    Asset *asset1 = this->_cache->getAsset(normPath);
    if (asset1 != nullptr)
    {
        return asset1;
    }
    // 创建加载任务
    long long time = TimeUtil::nowTime();
    AssetTask task(this->_mgr, this->_cache);
    Asset *asset2 = task.load(normPath);
    if (asset2 != nullptr)
    {
        this->_cache->addAsset(normPath, asset2);
        std::cout << "load asset " << normPath << " cost :" << TimeUtil::nowTime() - time << " ms" << std::endl;
    }
    return asset2;
}
Asset *AssetLoad::getAsset(const std::string &path)
{
    std::filesystem::path key = std::filesystem::path(path);
    std::string normPath = key.generic_string();
    Asset *asset = this->_cache->getAsset(path);
    if (asset != nullptr)
    {
        return asset;
    }
    return nullptr;
}

AssetLoad::~AssetLoad()
{
}

// Asset *AssetLoad::load(const std::string path, const std::string fullPath)
// {

// }