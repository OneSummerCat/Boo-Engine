#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include "gfx-struct.h"
#include "../../platforms/window/window.h"
#include "../../platforms/android/android.h"

class GfxRenderTexture;
class GfxMaterial;
class GfxMesh;
class GfxShader;
class GfxTexture;

class GfxMgr
{
private:
	GfxMgr(const GfxMgr &) = delete;			/* // 禁用拷贝构造 */
	GfxMgr &operator=(const GfxMgr &) = delete; /* // 禁用赋值操作符 */
	GfxMgr();
	~GfxMgr();
	size_t _currentFrame = 0;
	std::chrono::high_resolution_clock::time_point _preFrameTime;
	void _initRenderer();

public:
	static GfxMgr *getInstance();
	/**
	 * 初始化
	 */
	void init(Window *window);
	void init(Android *android);

	void update(float dt);
	void resize(int width, int height);
	void resetSwapChain();

	void createPipeline(std::string pipelineName, GfxRendererState rendererState);
	/**
	 * @brief 创建纹理
	 *
	 * @param textureUuid 纹理UUID
	 * @param width 宽度
	 * @param height 高度
	 * @param channels 通道数
	 * @param pixels 像素数据
	 */
	GfxTexture *createTexture(std::string uuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels, GfxTextureFormat format);
	/**
	 * @brief 销毁纹理
	 *
	 * @param textureUuid 纹理UUID
	 */
	void destroyTexture(GfxTexture *texture);


	/**
	 * @brief 创建着色器
	 *
	 * @param shaderName 着色器名称
	 * @param buffer 着色器字节码
	 */
	GfxShader *createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, int> &macros);
	/**
	 * @brief 创建SPIR-V着色器
	 */
	GfxShader *createSpirvShader(const std::string &shaderName, const std::vector<uint32_t> &data);
	void destroyShader(GfxShader *shader);
	/**
	 * @brief 创建网格
	 *
	 * @param meshUuid 网格UUID
	 * @param positions 顶点位置
	 * @param normals 法线
	 * @param uvs 纹理坐标
	 * @param uvs1 纹理坐标1
	 * @param uvs2 纹理坐标2
	 * @param colors 颜色
	 * @param tangents 切线
	 * @param indices 索引数据
	 */
	GfxMesh *createMesh(std::string meshUuid, const std::vector<float> &_positions, const std::vector<float> &_normals, const std::vector<float> &_uvs, const std::vector<float> &_uvs1, const std::vector<float> &_uvs2, const std::vector<float> &_colors, const std::vector<float> &_tangents, const std::vector<int> &_indices);
	void destroyMesh(GfxMesh *mesh);

	/**
	 * @brief 创建渲染纹理
	 *
	 * @param renderTextureUuid 渲染纹理UUID
	 * @param width 宽度
	 * @param height 高度
	 * @param channels 通道数
	 * @param pixels 像素数据
	 */
	GfxRenderTexture *createRenderTexture(std::string renderTextureUuid, uint32_t width, uint32_t height);
	void destroyRenderTexture(GfxRenderTexture *renderTexture);

	/**
	 * @brief 初始化渲染队列
	 *
	 * @param renderId 渲染队列ID
	 * @param viewMat 视图矩阵
	 * @param projMat 投影矩阵
	 */
	void initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture, int priority);
	void setRenderQueuePriority(std::string renderId, int priority);
	/**
	 * @brief 销毁渲染队列
	 *
	 * @param renderId 渲染队列ID
	 */
	void delRenderQueue(std::string renderId);

	void submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen);
	/**
	 * @brief 提交渲染对象
	 *
	 * @param renderId 渲染队列ID
	 * @param pass 渲染通道名称
	 * @param pipeline 管线名称
	 * @param vertices 顶点数据
	 * @param indices 索引数据
	 */
	void submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh);
	/**
	 * @brief 提交渲染对象
	 * @param id 物体ID
	 */

	VkResult createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size, void *data);
	uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties);
	std::vector<char> readShaderFile(const std::string &filename);
};