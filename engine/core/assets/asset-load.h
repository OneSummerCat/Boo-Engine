#pragma once
#include <string>
#include <vector>
#include <functional>
#include "asset-cache.h"
#include "asset-task.h"

class Asset;
class Shader;
class AssetsManager;



class AssetLoad
{
private:
    AssetsManager *_mgr;
    /**
     * @brief 资产缓存
     */
    AssetCache *_cache;
    /**
     * @brief 最大加载数量
     */
    int MAX_LOAD_COUNT = 3;

    std::vector<AssetTask> _tasks;

public:
    AssetLoad(AssetsManager *mgr);
    Asset *load(const std::string path);

    template <typename T, typename Func>
    void loadAsync(const std::string &path, Func callback, T *instance)
    {
        std::filesystem::path key = std::filesystem::path(path);
        std::string normPath = key.generic_string();
        Asset *asset = this->_cache->getAsset(normPath);
        if (asset != nullptr)
        {
            (instance->*callback)();
            return;
        }
        AssetTask task(this->_mgr, this->_cache);
        task.loadAsync(normPath, callback, instance);
        this->_tasks.push_back(task);
    }
    template <typename T, typename Func>
    void loadListAsync(const std::vector<std::string> &paths, Func callback, T *instance)
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
            task.loadASync(normPath, result, callback, instance);
            this->_tasks.push_back(task);
        }
        // 所有任务完成
        if (result->complete >= result->all)
        {
            int complete = result->complete;
            int all = result->all;
            float progress = (float)complete / (float)all;
            (instance->*callback)(complete, all, progress);
            delete result;
        }
    }

    Asset *getAsset(const std::string &path);

    /**
     * @brief 更新加载任务
     * @param deltaTime 时间间隔
     */
    void update(float deltaTime);
    void updateLoadTasks();
    ~AssetLoad();
};
