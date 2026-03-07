#pragma once
#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include <iostream>

#include "asset.h"
#include "asset-cache.h"
#include "asset-task.h"

namespace Boo
{

    class AssetLoad
    {
    private:
        /**
         * @brief 最大加载数量
         */
        int _MAX_LOAD_COUNT = 30;

        int _TaskNextID = 0;
        std::vector<AssetTask> _tasks;

    public:
        AssetLoad();
        void setMaxLoadCount(int count)
        {
            this->_MAX_LOAD_COUNT = count;
        };
        Asset *loadAsset(const std::string &path);
        /**
         * @brief 更新加载任务
         * @param deltaTime 时间间隔
         */
        void update(float deltaTime);
        void updateLoadTasks();
        ~AssetLoad();
    };

} // namespace Boo
