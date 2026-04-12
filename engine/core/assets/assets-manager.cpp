#include "assets-manager.h"
#include "../../log.h"

namespace Boo
{

	AssetsManager::AssetsManager() : _assetsLoad(new AssetLoad()),
									 _assetsCache(new AssetCache()),
									 _assetsBuiltin(new AssetBuiltin())

	{
	}

#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
	void AssetsManager::setAssetsRoot(const std::string &assetsRoot)
	{
		this->_assetsRoot = assetsRoot;
	}
#endif
#if defined(BOO_PLATFORM_ANDROID)
	void AssetsManager::setAndroidAssetsManager(AAssetManager *androidAssetsManager)
	{
		this->_androidAssetsManager = androidAssetsManager;
	}
#endif
	void AssetsManager::init()
	{
		this->_assetsBuiltin->init();
	}
	const std::string &AssetsManager::getAssetsRoot()
	{
		return this->_assetsRoot;
	}

	AssetLoad *AssetsManager::getAssetsLoad()
	{
		return this->_assetsLoad;
	}
	AssetCache *AssetsManager::getAssetsCache()
	{
		return this->_assetsCache;
	}
	void AssetsManager::setMaxLoadCount(int count)
	{
	}
	Asset *AssetsManager::loadAsset(const std::string &path)
	{
		return this->_assetsLoad->loadAsset(path);
	}

	Asset *AssetsManager::getAsset(const std::string &path, bool loadIfNotFound)
	{
		if (loadIfNotFound) {
			return this->_assetsLoad->loadAsset(path);
		}
		return this->_assetsCache->getAsset(path);
	}
	void AssetsManager::unloadAsset(const std::string &path)
	{
		LOGI("The current function is for implementation"); // 当前功能为实现,后续会根据需求进行修改
	}
	void AssetsManager::update(float deltaTime)
	{
		this->_assetsLoad->update(deltaTime);
	}

	AssetsManager::~AssetsManager()
	{
	}

} // namespace Boo