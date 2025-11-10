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
    std::cout << "load asset " << normPath << " cost :" << TimeUtil::nowTime() - time << " ms" << std::endl;
    return asset2;
}

void AssetLoad::loadAsync(const std::string &path, std::function<void()> callback)
{
    std::filesystem::path key = std::filesystem::path(path);
    std::string normPath = key.generic_string();
    Asset *asset = this->_cache->getAsset(normPath);
    if (asset != nullptr)
    {
        callback();
        return;
    }
    AssetTask task(this->_mgr, this->_cache);
    task.loadAsync(normPath, callback);
    this->_tasks.push_back(task);
}
void AssetLoad::loadListAsync(const std::vector<std::string> &paths, std::function<void(const int complete, const int all, const float progress)> callback)
{
    AssetLoadResult *result = new AssetLoadResult();
    result->all = paths.size();
    for (const std::string &path : paths)
    {
        std::filesystem::path key = std::filesystem::path(path);
        std::string normPath = key.generic_string();
        Asset *asset = this->_cache->getAsset(normPath);
        if (asset != nullptr)
        {
            result->complete++;
            continue;
        }
        AssetTask task(this->_mgr, this->_cache);
        task.loadSync(normPath, result, callback);
        this->_tasks.push_back(task);
    }
    // 所有任务完成
    if (result->complete >= result->all)
    {
        int complete = result->complete;
        int all = result->all;
        float progress = (float)complete / (float)all;
        callback(complete, all, progress);
        delete result;
    }
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

/**
 * @brief 更新加载任务
 * @param deltaTime 时间间隔
 */
void AssetLoad::update(float deltaTime)
{
    this->updateLoadTasks();
}
void AssetLoad::updateLoadTasks()
{
    // 删除已经完成的任务
    auto it = std::remove_if(this->_tasks.begin(), this->_tasks.end(),
                             [](AssetTask &task)
                             {
                                 return task.isComplete();
                             });

    this->_tasks.erase(it, this->_tasks.end());
    // 执行剩余任务的run
    int loadCount = 0;
    for (AssetTask &task : this->_tasks)
    {
        task.run();
        loadCount++;
        if (loadCount >= this->MAX_LOAD_COUNT)
        {
            break;
        }
    }
}

AssetLoad::~AssetLoad()
{
}
