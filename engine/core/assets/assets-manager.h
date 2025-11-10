#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
enum class AssetType;
class Asset;
class AssetCache;
class AssetLoad;
struct AssetLoadResult;

// 基础图片资源
class AssetsManager
{
private:
	/**
	 * @brief 资产根目录
	 */
	std::string _root;

	/**
	 * @brief 资产加载器
	 */
	AssetLoad *_assetLoad;

	void _initRoot();

public:
	AssetsManager();
	/**
	 * @brief 初始化资产管理器
	 */
	void init();

	const std::string &root()
	{
		return this->_root;
	}
	AssetLoad *assetLoad()
	{
		return this->_assetLoad;
	}
	/**
	 * @brief 加载资产
	 * @param path 资产路径
	 * @return Asset 资产对象
	 */
	Asset *load(const std::string &path);
	void loadAsync(const std::string &path, std::function<void()> callback);
	/**
	 * @brief 异步加载资产列表
	 * @param paths 资产路径列表
	 */
	void loadList(const std::vector<std::string> &paths);
	void loadListAsync(const std::vector<std::string> &paths, std::function<void(const int complete, const int all, const float progress)> callback);

	Asset *get(const std::string &path);


	void update(float deltaTime);
	// /**
	//  * @brief 异步加载资产
	//  * @param path 资产路径
	//  */
	//
	~AssetsManager();
};