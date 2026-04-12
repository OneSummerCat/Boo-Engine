#pragma once
#include "gfx-builtin-batch.h"

class GfxBuiltinRenderer;
class GfxBuiltinPipeline;
class GfxRenderTexture;
class GfxTexture;
class GfxMaterial;
class GfxMesh;
class GfxBuffer;

class GfxBuiltinBatchUI : public GfxBuiltinBatch
{
protected:

    void _stencilTest(VkCommandBuffer &queueCommandBuffer, const GfxRendererState &rendererState);
    void _setViewportScissor(VkCommandBuffer &queueCommandBuffer) override;
    void _bindUniformBuffer() override;
    void _bindDescriptorSets(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline, GfxBuffer *ubo) override;
    void _bindVertexIndicesBuffers(VkCommandBuffer &queueCommandBuffer) override;
    void _drawIndexed(VkCommandBuffer &queueCommandBuffer) override;
    
public:
    GfxBuiltinBatchUI();
    virtual void addObject(const std::vector<char> &instanceData);
    void render(VkCommandBuffer &queueCommandBuffer) override;
    ~GfxBuiltinBatchUI();
};
