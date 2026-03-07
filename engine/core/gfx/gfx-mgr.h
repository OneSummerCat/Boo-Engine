#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include "base/gfx-pipeline-struct.h"
#include "../../platforms/window/window.h"
#include "../../platforms/android/android.h"

class GfxRenderTexture;
class GfxMaterial;
class GfxMesh;

class GfxMgr
{
private:
	GfxMgr(const GfxMgr &) = delete;			/* // 禁用拷贝构造 */
	GfxMgr &operator=(const GfxMgr &) = delete; /* // 禁用赋值操作符 */
	GfxMgr();
	~GfxMgr();
	size_t _currentFrame = 0;

	void _initRenderer();
public:
	static GfxMgr *getInstance();
	/**
	 * 初始化
	 */
	void init(Window *window);
	void init(Android *android);
	// void setWindow();
	// void setAndroid(Android *android);
	
	void update(float dt);
	void resize(int width,int height);
	void resetSwapChain();

	void createPipeline(std::string pipelineName, GfxPipelineStruct pipelineStruct);
	/**
	 * @brief 创建纹理
	 *
	 * @param textureUuid 纹理UUID
	 * @param width 宽度
	 * @param height 高度
	 * @param channels 通道数
	 * @param pixels 像素数据
	 */
	void createTexture(std::string texture, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels);
	/**
	 * @brief 销毁纹理
	 *
	 * @param textureUuid 纹理UUID
	 */
	void destroyTexture(std::string texture);
	/**
	 * @brief 判断纹理是否存在
	 *
	 * @param texture 纹理UUID
	 * @return true 存在
	 * @return false 不存在
	 */
	bool isExistTexture(std::string texture);

	/**
	 * @brief 创建着色器
	 *
	 * @param shaderName 着色器名称
	 * @param buffer 着色器字节码
	 */
	void createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, std::string> &macros);
	/**
	 * @brief 创建SPIR-V着色器
	 */
	void createSpirvShader(const std::string &shaderName, const std::vector<uint32_t> &data);
	void destroyShader(std::string shaderName);

	/**
	 * @brief 初始化渲染队列
	 *
	 * @param renderId 渲染队列ID
	 * @param viewMat 视图矩阵
	 * @param projMat 投影矩阵
	 */
	void initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture);
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
	void submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh, std::vector<float> &instanceData);
	/**
	 * @brief 提交渲染对象
	 * @param id 物体ID
	 */

	VkResult createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size, void *data);
	uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties);
	std::vector<char> readShaderFile(const std::string &filename);
};