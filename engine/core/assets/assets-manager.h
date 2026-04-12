#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <iostream>
#include <filesystem>
#include "asset.h"
#include "asset-load.h"
#include "asset-cache.h"
#include "asset-builtin.h"
#include "texture-asset.h"
#include "material-asset.h"
#include "mesh-asset.h"
#include "shader-asset.h"
#include "gltf-asset.h"


#include "../../platforms/platform.h"
#if defined(BOO_PLATFORM_ANDROID)
#include <android/asset_manager.h>
#endif
namespace Boo
{

	// 基础图片资源
	class AssetsManager
	{
	private:
		std::string _assetsRoot = "";
		/**
		 * @brief 资产加载器
		 */
		AssetLoad *_assetsLoad;
		/**
		 * @brief 资产缓存
		 */
		AssetCache *_assetsCache;

		AssetBuiltin *_assetsBuiltin;
#if defined(BOO_PLATFORM_ANDROID)
		AAssetManager *_androidAssetsManager = nullptr;
#endif
	public:
		AssetsManager();
		/**
		 * @brief 初始化资产管理器
		 */
		void init();
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
		void setAssetsRoot(const std::string &assetsRoot);
#endif
#if defined(BOO_PLATFORM_ANDROID)
		void setAndroidAssetsManager(AAssetManager *androidAssetsManager);
#endif
		const std::string &getAssetsRoot();
		void setMaxLoadCount(int count);
		AssetLoad *getAssetsLoad();
		AssetCache *getAssetsCache();

		Asset *loadAsset(const std::string &uuid);
		Asset *getAsset(const std::string &uuid, bool loadIfNotFound = false);
		void unloadAsset(const std::string &uuid);

		void update(float deltaTime);

		~AssetsManager();
	};

} // namespace Boo