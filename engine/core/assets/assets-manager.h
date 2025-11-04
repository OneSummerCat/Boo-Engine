#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>

enum class AssetType
{
    None,
    Image,
    Audio,
    Font,
    Shader,
    Material,
};
struct AssetPath
{
    /**
     * @brief 资产路径
     * @param path 绝对路径
     */
    std::string path;
    /**
     * @brief 资产类型
     */
    AssetType type;
};

class AssetsManager
{
private:
    /**
     * @brief 资产根目录
     */
    std::string _root;
    /**
     * @brief 资产映射表
     */
    std::unordered_map<std::string, AssetPath> _assetsMap;
    /**
     * @brief 最大加载数量
     */
    int MAX_LOAD_COUNT = 30;
    void _initRoot();
public:
	AssetsManager();
	~AssetsManager();
};