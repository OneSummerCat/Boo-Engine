#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <array>
#include "../../../../log.h"
#include "../../gfx-struct.h"

class GfxBuiltinRenderer;
class GfxBuiltinPipeline;
class GfxRenderTexture;
class GfxTexture;
class GfxMaterial;
class GfxMesh;
class GfxBuffer;

class GfxBuiltinBatch
{
protected:
    GfxBuiltinRenderer *_renderer;
    GfxRenderTexture *_renderTexture;
    GfxMaterial *_material;
    GfxMesh *_mesh;

    std::vector<char> _instanceDatas;
    int _instanceCount = 0;
    /**
     * @brief 绑定渲染管线
     * 渲染第四步
     */
    void _bindPipeline(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline);
    virtual void _setViewportScissor(VkCommandBuffer &queueCommandBuffer); // // 设置视口
    virtual void _bindDescriptorSets(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline, GfxBuffer *ubo);
    virtual void _bindVertexIndicesBuffers(VkCommandBuffer &queueCommandBuffer);
    virtual void _drawIndexed(VkCommandBuffer &queueCommandBuffer);

public:
    GfxBuiltinBatch(GfxBuiltinRenderer *renderer, GfxRenderTexture *renderTexture, GfxMaterial *material, GfxMesh *mesh);
    GfxMaterial *getMaterial() const { return _material; }
    GfxMesh *getMesh() const { return _mesh; }
    void addObject(const std::vector<char> &instanceData);
    virtual void render(VkCommandBuffer &queueCommandBuffer, GfxBuffer *ubo);
    void destroy();
    virtual ~GfxBuiltinBatch();
};