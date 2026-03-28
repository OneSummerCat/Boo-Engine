#pragma once
#include "gfx-builtin-batch.h"

class GfxBuiltinRenderer;
class GfxBuiltinPipeline;
class GfxRenderTexture;
class GfxTexture;
class GfxMaterial;
class GfxMesh;
class GfxBuffer;

class GfxBuiltinBatch3D : public GfxBuiltinBatch
{
protected:
    void _setViewportScissor(VkCommandBuffer &queueCommandBuffer) override;
    void _bindDescriptorSets(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline, GfxBuffer *ubo) override;
    void _bindVertexIndicesBuffers(VkCommandBuffer &queueCommandBuffer) override;
    void _drawIndexed(VkCommandBuffer &queueCommandBuffer) override;
    
public:
    GfxBuiltinBatch3D(GfxBuiltinRenderer *renderer, GfxRenderTexture *renderTexture, GfxMaterial *material, GfxMesh *mesh);
    void render(VkCommandBuffer &queueCommandBuffer, GfxBuffer *ubo) override;
    ~GfxBuiltinBatch3D();
};
