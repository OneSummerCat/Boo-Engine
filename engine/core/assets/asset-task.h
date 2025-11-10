
#pragma once
#include <string>
#include <functional>

class Asset;
class Shader;
class AssetsManager;
class AssetCache;

struct AssetLoadResult
{
    /**
     * @brief 所有资产数量
     */
    int all = 0;
    /**
     * @brief 已完成资产数量
     */
    int complete = 0;
    /**
     * @brief 加载进度
     */
    float progress = 0.0f;
};
enum class AssetTaskType
{
    /**
     * @brief 资产加载任务状态-异步加载
     */
    Sync,
    /**
     * @brief 资产加载任务状态-同步加载-一次
     */
    AsyncOnce,
    /**
     * @brief 资产加载任务状态-同步加载-列表
     */
    AsyncList,
};
/**
 * @brief 资产加载任务
 */
class AssetTask
{
private:
    AssetsManager *_mgr;
    /**
     * @brief 资产缓存
     */
    AssetCache *_cache;
    /**
     * @brief 资产路径
     */
    std::string _path;
    /**
     * @brief 资产加载任务类型
     */
    AssetTaskType _type;
    /**
     * @brief 是否完成加载
     */
    bool _isComplete = false;
    /**
     * @brief 资产加载回调
     */
    std::function<void()> _callbackOnce;
    /**
     * @brief 资产加载回调
     */
    std::function<void(const int complete, const int all, const float progress)> _callbackList;
    AssetLoadResult *_result;

    /**
     * @brief 创建贴图
     *
     * @param resKey 资产键值
     * @param fullPath 资产路径
     */
    void _createTexture(const std::string resKey, const std::string fullPath);
    /**
     * @brief 创建GLSL着色器
     *
     * @param resKey 资产键值
     * @param fullPath 资产路径
     */
    void _createGlslShader(const std::string resKey, const std::string fullPath);
    /**
     * @brief 创建SPIR-V着色器
     *
     * @param resKey 资产键值
     * @param fullPath 资产路径
     */
    void _createSpirvShader(const std::string resKey, const std::string fullPath);

    /**
     * @brief 加载完成
     */
    void _loadComplete();
    /**
     * @brief 加载错误
     */
    void _loadError();

public:
    AssetTask(AssetsManager *mgr, AssetCache *cache);
    Asset *load(const std::string &path);
    void loadAsync(const std::string &path, std::function<void()> callback);
    void loadSync(const std::string path, AssetLoadResult *result, std::function<void(const int complete, const int all, const float progress)> callback);
    void run();
    bool isComplete()
    {
        return this->_isComplete;
    }
    ~AssetTask();
};