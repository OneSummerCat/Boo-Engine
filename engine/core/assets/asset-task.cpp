
#include "asset-task.h"
#include <filesystem>
#include <iostream>
#include "texture.h"
#include "shader.h"
#include "assets-manager.h"
#include "asset-cache.h"
#include "../utils/time-util.h"

AssetTask::AssetTask(AssetsManager *mgr, AssetCache *cache)
{
	this->_mgr = mgr;
	this->_cache = cache;
}
Asset *AssetTask::load(const std::string &path)
{
	this->_path = path;
	return this->run();
}
void AssetTask::load(const std::string path, AssetLoadResult *result)
{
	this->_path = path;
}

Asset *AssetTask::run()
{
	std::filesystem::path fullPath = std::filesystem::path(this->_mgr->root()) / this->_path;
	if (!std::filesystem::exists(fullPath))
	{
		std::cerr << "AssetLoad:No such file or directory:" << fullPath << std::endl;
		return nullptr;
	}
	if (!std::filesystem::is_regular_file(fullPath))
	{
		std::cerr << "AssetLoad:Not a regular file:" << fullPath << std::endl;
		return nullptr;
	}
	std::filesystem::path path = std::filesystem::relative(fullPath, std::filesystem::path(this->_mgr->root()));
	std::string resKey = path.generic_string();
	
	std::string extension = std::filesystem::path(fullPath).extension().string();
	if (extension == ".png" || extension == ".PNG" || extension == ".jpg" || extension == ".JPG")
	{
		// 贴图
		Texture *texture = new Texture(resKey, fullPath.generic_string());
		return texture;
	}
	else if (extension == ".vert" || extension == ".frag")
	{
		// 着色器
		Shader *shader = new Shader(resKey, fullPath.generic_string());
		shader->loadGlsl();
		return shader;
	}
	else if (extension == ".spv")
	{
		// SPIR-V
		Shader *shader = new Shader(resKey, fullPath.generic_string());
		shader->loadSpv();
		return shader;
	}
	return nullptr;
}

AssetTask::~AssetTask()
{
}