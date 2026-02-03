#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <iostream>
#include <filesystem>

#include "asset-struct.h"
#include "asset-load.h"
#include "asset-cache.h"
#include "assst-builtin.h"
#include "texture-asset.h"
#include "material-asset.h"
#include "shader-asset.h"

// 基础图片资源
class AssetsManager
{
private:
	/**
	 * @brief 资产根目录
	 */
	std::string _assetsRoot;

	/**
	 * @brief 资产加载器
	 */
	AssetLoad *_assetsLoad;
	/**
	 * @brief 资产缓存
	 */
	AssetCache *_assetsCache;

	AssetBuiltin *_assetsBuiltin;


public:
	AssetsManager();
	/**
	 * @brief 初始化资产管理器
	 */
	void init();
	
	void setAssetsRoot(const std::string &root);
	const std::string &getAssetsRoot();
	void setMaxLoadCount(int count);
	AssetLoad *getAssetsLoad();
	AssetCache *getAssetsCache();

	Asset *loadAsset(const std::string &uuid);
	Asset *getAsset(const std::string &uuid);

	void update(float deltaTime);

	~AssetsManager();
};