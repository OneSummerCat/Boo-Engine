#include "assets-manager.h"
#include "../../log.h"

namespace Boo
{

	AssetsManager::AssetsManager() : _assetsRoot(""),
									 _assetsLoad(new AssetLoad()),
									 _assetsCache(new AssetCache()),
									 _assetsBuiltin(new AssetBuiltin())

	{
	}
	void AssetsManager::init()
	{
		this->_assetsBuiltin->init();
	}
	void AssetsManager::setAssetsRoot(const std::string &root)
	{
		this->_assetsRoot = root;
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
		this->_assetsLoad->setMaxLoadCount(count);
	}
	Asset *AssetsManager::loadAsset(const std::string &path)
	{
		return this->_assetsLoad->loadAsset(path);
	}

	Asset *AssetsManager::getAsset(const std::string &path)
	{
		return this->_assetsCache->getAsset(path);
	}
	void AssetsManager::unloadAsset(const std::string &path)
	{
		LOGI("The current function is for implementation"); //当前功能为实现,后续会根据需求进行修改
	}
	void AssetsManager::update(float deltaTime)
	{
		this->_assetsLoad->update(deltaTime);
	}

	AssetsManager::~AssetsManager()
	{
	}

} // namespace Boo