#pragma once
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <array>
#include <chrono>
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
    GfxBuffer *_ubo;
    std::array<float, 16> _viewMatrix;
    std::array<float, 16> _projMatrix;
    std::array<float, 4> _cameraPos;

    std::vector<char> _instanceDatas;
    int _instanceCount = 0;

	virtual void _bindUniformBuffer();
    void _bindPipeline(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline);
    virtual void _setViewportScissor(VkCommandBuffer &queueCommandBuffer); // // 设置视口
    virtual void _bindDescriptorSets(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline, GfxBuffer *ubo);
    virtual void _bindVertexIndicesBuffers(VkCommandBuffer &queueCommandBuffer);
    virtual void _drawIndexed(VkCommandBuffer &queueCommandBuffer);
    void _clear();

public:
    GfxBuiltinBatch();
    void init(GfxBuiltinRenderer *renderer, GfxRenderTexture *renderTexture, GfxMaterial *material, GfxMesh *mesh, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, const std::array<float, 4> &cameraPosition);
    GfxMaterial *getMaterial() const { return _material; }
    GfxMesh *getMesh() const { return _mesh; }
    virtual void addObject(const std::vector<char> &instanceData);
    virtual void render(VkCommandBuffer &queueCommandBuffer);
    void destroy();
    virtual ~GfxBuiltinBatch();
};