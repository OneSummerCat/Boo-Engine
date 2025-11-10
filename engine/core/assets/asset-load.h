#pragma once
#include <string>
#include <vector>
#include <functional>

#include "asset-task.h"

class Asset;
class Shader;
class AssetsManager;

class AssetCache;

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
    void loadAsync(const std::string &path, std::function<void()> callback);
    //
    // void loadList(const std::vector<std::string> &paths);
    // void loadAsync(const std::string &path, std::function<void(const int complete, const int all, const float progress)> callback);
    
    void loadListAsync(const std::vector<std::string> &paths, std::function<void(const int complete, const int all, const float progress)> callback);

    Asset *getAsset(const std::string &path);

    /**
     * @brief 更新加载任务
     * @param deltaTime 时间间隔
     */
    void update(float deltaTime);
    void updateLoadTasks();
    ~AssetLoad();
};
