#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include "asset-load.h"
enum class AssetType;
class Asset;
class AssetCache;
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
	template <typename T, typename Func>
	void loadAsync(const std::string &path, Func callback, T *instance)
	{
		this->_assetLoad->loadAsync(path, callback, instance);
	}
	template <typename T, typename Func>
	void loadListAsync(const std::vector<std::string> &paths, Func callback, T *instance)
	{
		this->_assetLoad->loadListAsync(paths, callback, instance);
	}

	Asset *get(const std::string &path);

	void update(float deltaTime);
	// /**
	//  * @brief 异步加载资产
	//  * @param path 资产路径
	//  */
	//
	~AssetsManager();
};




// // ============================================
//   // 模拟资源类
//   // ============================================
//   struct Asset {
//       std::string path;
//       Asset(const std::string& p) : path(p) {
//           std::cout << "[Asset] 加载: " << path << std::endl;
//       }
//   };

//   // ============================================
//   // 底层异步加载器
//   // ============================================
//   class AssetLoader {
//   public:
//       void loadAsync(const std::string& path, std::function<void(Asset*)> callback) {
//           // 模拟异步加载
//           std::thread([path, callback]() {
//               std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟耗时
//               auto* asset = new Asset(path);
//               callback(asset);
//               delete asset;
//           }).detach();
//       }
//   };

//   // ============================================
//   // 带守卫的异步加载管理器
//   // ============================================
//   class LoaderManager {
//   private:
//       AssetLoader* _assetLoader;

//   public:
//       LoaderManager() : _assetLoader(new AssetLoader()) {}
//       ~LoaderManager() { delete _assetLoader; }

//       // 方案三：使用守卫对象
//       template <typename T, typename Func, typename Guard>
//       void loadAsync(const std::string& path,
//                      Func callback,
//                      T* instance,
//                      std::shared_ptr<Guard> guard)
//       {
//           std::weak_ptr<Guard> weak_guard = guard;

//           _assetLoader->loadAsync(path, [weak_guard, instance, callback](Asset* result) {
//               if (auto locked = weak_guard.lock()) {
//                   // ✅ 守卫还在，说明对象还活着
//                   std::cout << "[回调] 守卫有效，执行回调" << std::endl;
//                   (instance->*callback)(result);
//               } else {
//                   // ⚠️ 守卫已失效，对象已销毁
//                   std::cout << "[回调] 守卫失效，跳过回调（避免崩溃）" << std::endl;
//               }
//           });
//       }
//   };

//   // ============================================
//   // 使用守卫的类
//   // ============================================
//   class MyClass {
//   private:
//       int _value = 42;
//       std::shared_ptr<int> guard_; // 守卫对象（哨兵）

//   public:
//       MyClass() : guard_(std::make_shared<int>(0)) {
//           std::cout << "[构造] MyClass" << std::endl;
//       }

//       ~MyClass() {
//           std::cout << "[析构] MyClass - 开始" << std::endl;
//           guard_.reset(); // ✅ 让守卫失效
//           std::cout << "[析构] MyClass - 守卫已重置" << std::endl;
//       }

//       // 回调函数
//       void onTextureLoaded(Asset* asset) {
//           std::cout << "[MyClass::onTextureLoaded] 资源加载完成: " << asset->path
//                     << ", _value = " << _value << std::endl;
//       }

//       void onModelLoaded(Asset* asset) {
//           std::cout << "[MyClass::onModelLoaded] 模型加载完成: " << asset->path
//                     << ", _value = " << _value << std::endl;
//       }

//       // 启动异步加载
//       void startLoad(LoaderManager& loader) {
//           std::cout << "[MyClass] 开始异步加载..." << std::endl;

//           // 传入 this、成员函数指针、守卫对象
//           loader.loadAsync("texture.png", &MyClass::onTextureLoaded, this, guard_);
//           loader.loadAsync("model.obj", &MyClass::onModelLoaded, this, guard_);
//       }
//   };

//   // ============================================
//   // 测试用例
//   // ============================================
//   int main() {
//       LoaderManager loader;

//       std::cout << "========== 测试1: 对象存活，回调正常执行 ==========" << std::endl;
//       {
//           MyClass obj;
//           obj.startLoad(loader);
//           std::this_thread::sleep_for(std::chrono::milliseconds(200)); // 等待加载完成
//       }

//       std::cout << "\n========== 测试2: 对象提前销毁，回调被跳过 ==========" << std::endl;
//       {
//           MyClass obj;
//           obj.startLoad(loader);
//           // 立即销毁对象，不等待加载完成
//       }
//       std::this_thread::sleep_for(std::chrono::milliseconds(200)); // 等待异步回调尝试执行

//       std::cout << "\n========== 测试3: 多个对象独立管理 ==========" << std::endl;
//       {
//           MyClass obj1;
//           MyClass obj2;

//           obj1.startLoad(loader);
//           std::this_thread::sleep_for(std::chrono::milliseconds(50));

//           obj2.startLoad(loader);
//           std::this_thread::sleep_for(std::chrono::milliseconds(200));
//       }

//       std::cout << "\n========== 所有测试完成 ==========" << std::endl;
//       std::this_thread::sleep_for(std::chrono::milliseconds(100));

//       return 0;
//   }
