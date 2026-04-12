
#include "asset-task.h"
#include <filesystem>
#include <iostream>
#include "texture-asset.h"
#include "shader-asset.h"
#include "assets-manager.h"
#include "gltf-asset.h"
#include "../../boo.h"
#include "../../log.h"
#include "../../platforms/platform.h"
#include "../../libs/stb/stb_image.h"
#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
// #include <fastgltf/glm_element_traits.hpp>

namespace Boo
{
	AssetTask::AssetTask()
	{
	}
	Asset *AssetTask::load(const std::string &assetPath)
	{
		if (this->_isTextureAsset(assetPath))
		{
			return this->_createTexture(assetPath);
		}
		else if (this->_isShaderVertAsset(assetPath))
		{
			return this->_createShader(assetPath, ShaderVertexAsset);
		}
		else if (this->_isShaderFragAsset(assetPath))
		{
			return this->_createShader(assetPath, ShaderFragmentAsset);
		}
		else if (this->_isMaterialAsset(assetPath))
		{
			return this->_createMaterial(assetPath);
		}
		else if (this->_isGLBAsset(assetPath))
		{
			return this->_createGLBAsset(assetPath);
		}
		return nullptr;
	}
	bool AssetTask::_isTextureAsset(const std::string &assetPath)
	{
		std::string assetExtension = std::filesystem::path(assetPath).extension().string();
		return assetExtension == ".png" || assetExtension == ".PNG" || assetExtension == ".jpg" || assetExtension == ".JPG" || assetExtension == ".jpeg" || assetExtension == ".JPEG";
	}
	bool AssetTask::_isShaderVertAsset(const std::string &assetPath)
	{
		std::string assetExtension = std::filesystem::path(assetPath).extension().string();
		return assetExtension == ".vert" || assetExtension == ".VERT";
	}
	bool AssetTask::_isShaderFragAsset(const std::string &assetPath)
	{
		std::string assetExtension = std::filesystem::path(assetPath).extension().string();
		return assetExtension == ".frag" || assetExtension == ".FRAG";
	}
	bool AssetTask::_isMaterialAsset(const std::string &assetPath)
	{
		std::string assetExtension = std::filesystem::path(assetPath).extension().string();
		return assetExtension == ".mtl" || assetExtension == ".MTL";
	}
	bool AssetTask::_isGLBAsset(const std::string &assetPath)
	{
		std::string assetExtension = std::filesystem::path(assetPath).extension().string();
		return assetExtension == ".glb" || assetExtension == ".GLB";
		// assetExtension == ".gltf" || assetExtension == ".GLTF" ||
	}

	Asset *AssetTask::_createTexture(const std::string &assetPath)
	{
		
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
		std::filesystem::path fullPath = (std::filesystem::path(assetsManager->getAssetsRoot()) / "assets" / assetPath).generic_string();
		if (!std::filesystem::exists(fullPath))
		{
			LOGW("AssetLoad:No such file or directory: %s", fullPath.c_str());
			return nullptr;
		}
		if (!std::filesystem::is_regular_file(fullPath))
		{
			LOGW("AssetLoad:Not a regular file: %s", fullPath.c_str());
			return nullptr;
		}
		std::string name = std::filesystem::path(fullPath).stem().string();
		int _width = 0;
		int _height = 0;
		int _channels = 0;
		const void *_pixels = stbi_load(fullPath.string().c_str(), &_width, &_height, &_channels, STBI_rgb_alpha);
		if (_pixels == nullptr)
		{
			LOGW("Failed to load TextureAsset: %s", fullPath.c_str());
			return nullptr;
		}
		_channels = STBI_rgb_alpha;
		std::vector<uint8_t> pixelsVector = std::vector<uint8_t>(static_cast<const uint8_t *>(_pixels), static_cast<const uint8_t *>(_pixels) + (_width * _height * _channels));
		stbi_image_free((void *)_pixels);

		TextureAsset *texture = new TextureAsset(assetPath ,fullPath.string(), name);
		texture->create(_width, _height, _channels, pixelsVector, GfxTextureFormat::R8G8B8A8_SRGB);
		return texture;
#elif defined(BOO_PLATFORM_ANDROID)

#endif
		return nullptr;
	}

	Asset *AssetTask::_createShader(const std::string &assetPath, const std::string &type)
	{
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
		std::filesystem::path fullPath = (std::filesystem::path(assetsManager->getAssetsRoot()) / "assets" / assetPath).generic_string();
		if (!std::filesystem::exists(fullPath))
		{
			LOGW("AssetLoad:No such file or directory: %s", fullPath.c_str());
			return nullptr;
		}
		if (!std::filesystem::is_regular_file(fullPath))
		{
			LOGW("AssetLoad:Not a regular file: %s", fullPath.c_str());
			return nullptr;
		}
		std::string name = std::filesystem::path(fullPath).stem().string();
		std::ifstream file(fullPath, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			LOGW("AssetLoad:Failed to load ShaderAsset: %s", fullPath.c_str());
			return nullptr;
		}
		std::string glslData="";
		size_t fileSize = static_cast<size_t>(file.tellg());
		glslData.resize(fileSize);
		file.seekg(0);
		file.read((char *)glslData.data(), fileSize);
		file.close();

		ShaderAsset *shaderAsset = new ShaderAsset(assetPath, fullPath.string(), name);
		shaderAsset->create(type,glslData, {});
		return shaderAsset;
#elif defined(BOO_PLATFORM_ANDROID)

#endif
		return nullptr;
	}
	Asset *AssetTask::_createMaterial(const std::string &assetPath)
	{
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
		std::filesystem::path fullPath = (std::filesystem::path(assetsManager->getAssetsRoot()) / "assets" / assetPath).generic_string();
		if (!std::filesystem::exists(fullPath))
		{
			LOGW("AssetLoad:No such file or directory: %s", fullPath.c_str());
			return nullptr;
		}
		if (!std::filesystem::is_regular_file(fullPath))
		{
			LOGW("AssetLoad:Not a regular file: %s", fullPath.c_str());
			return nullptr;
		}
		json materialJson = FileUtil::readJsonFromText(fullPath.string());
		if (materialJson.is_null() || !materialJson.is_object() || !materialJson.contains("layer") || !materialJson.contains("type") || !materialJson.contains("vert") || !materialJson.contains("frag"))
		{
			LOGW("AssetLoad:Failed to load MaterialAsset: %s", fullPath.c_str());
			return nullptr;
		}
		std::string name = std::filesystem::path(fullPath).stem().string();
		MaterialAsset *material = new MaterialAsset(assetPath, fullPath.string(), name);
		material->create(materialJson);
		return material;
#elif defined(BOO_PLATFORM_ANDROID)

#endif
		return nullptr;
	}
	Asset *AssetTask::_createGLBAsset(const std::string &assetPath)
	{
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
		std::filesystem::path fullPath = (std::filesystem::path(assetsManager->getAssetsRoot()) / "assets" / assetPath).generic_string();
		if (!std::filesystem::exists(fullPath))
		{
			LOGW("AssetLoad:No such file or directory: %s", fullPath.c_str());
			return nullptr;
		}
		if (!std::filesystem::is_regular_file(fullPath))
		{
			LOGW("AssetLoad:Not a regular file: %s", fullPath.c_str());
			return nullptr;
		}
		std::chrono::high_resolution_clock::time_point time1 = std::chrono::high_resolution_clock::now();
		std::string name = std::filesystem::path(fullPath).stem().string();
		auto bufferResult = fastgltf::GltfDataBuffer::FromPath(fullPath);
		if (bufferResult.error() != fastgltf::Error::None)
		{
			// std::cerr << "Failed to load file: " << fastgltf::getErrorString(bufferResult.error()) << std::endl;
			LOGE("Failed to load GLTFAsset: %s", fullPath.c_str());
			return nullptr;
		}
		std::chrono::high_resolution_clock::time_point time2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> frameDuration = std::chrono::duration_cast<std::chrono::duration<double>>(time2 - time1);
		LOGI("[AssetTask : create] :: loadGLBAsset %s duration: %f", name.c_str(), frameDuration.count() / 1000);

		// 2. 配置解析器选项
		fastgltf::Parser parser;
		// 设置解析选项：加载 GLB 缓冲区数据，启用网格优化压缩解压
		auto options = fastgltf::Options::LoadGLBBuffers |
					   fastgltf::Options::DecomposeNodeMatrices;
		//  fastgltf::Options::LoadExternalBuffers | 不支持读取并加载外部 .bin 文件的内容
		auto &dataGetter = bufferResult.get();
		auto result = parser.loadGltf(dataGetter, fullPath, options);
		if (result.error() != fastgltf::Error::None)
		{
			LOGE("Failed to parse glTFAsset %d %s", result.error(), fullPath.c_str());
			return nullptr;
		}
		std::chrono::high_resolution_clock::time_point time3 = std::chrono::high_resolution_clock::now();
		float parseDuration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2).count() / 1000000.0f;
		LOGI("[AssetTask : create] :: parse loadGltf %s duration: %f", name.c_str(), parseDuration1);

		std::unique_ptr<fastgltf::Asset> m_asset = std::make_unique<fastgltf::Asset>(std::move(result.get()));
		std::chrono::high_resolution_clock::time_point time4 = std::chrono::high_resolution_clock::now();
		float parseDuration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(time4 - time3).count() / 1000000.0f;
		LOGI("[AssetTask : create] :: parse move asset %s duration: %f", name.c_str(), parseDuration2);

		GLTFAsset *glTFAsset = new GLTFAsset(assetPath, fullPath.string(), name);
		glTFAsset->create(std::move(m_asset));
		return glTFAsset;
#elif defined(BOO_PLATFORM_ANDROID)

#endif
		return nullptr;
	}

	AssetTask::~AssetTask()
	{
	}
}