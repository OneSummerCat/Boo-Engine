#include "gfx-builtin-batch-ui.h"
#include "../../gfx.h"
#include "../../gfx-context.h"
#include "../../gfx-renderer.h"
#include "../../base/gfx-mesh.h"
#include "../../base/gfx-material.h"
#include "../../base/gfx-texture.h"
#include "../../base/gfx-render-texture.h"
#include "../../base/gfx-buffer.h"
#include "../../base/gfx-buffer-ubo.h"
#include "../../base/gfx-buffer-instance.h"
#include "../gfx-builtin-renderer.h"
#include "../gfx-builtin-pipeline/gfx-builtin-pipeline.h"

GfxBuiltinBatchUI::GfxBuiltinBatchUI(GfxBuiltinRenderer *renderer, GfxRenderTexture *renderTexture, GfxMaterial *material, GfxMesh *mesh)
    : GfxBuiltinBatch(renderer, renderTexture, material, mesh)
{
}
void GfxBuiltinBatchUI::render(VkCommandBuffer &queueCommandBuffer, GfxBuffer *ubo)
{
    const GfxRendererState &pipelineState = this->_material->getRendererState();
    GfxBuiltinPipeline *pipeline = this->_renderer->getPipeline(pipelineState);
    if (pipeline == nullptr)
    {
        LOGE("[Gfx : BatchBuiltin] :: render: pipeline not found!");
        return;
    }
    this->_bindPipeline(queueCommandBuffer, pipeline);
    this->_stencilTest(queueCommandBuffer, pipelineState);
    this->_setViewportScissor(queueCommandBuffer);
    this->_bindDescriptorSets(queueCommandBuffer, pipeline, ubo);
    this->_bindVertexIndicesBuffers(queueCommandBuffer);
    this->_drawIndexed(queueCommandBuffer);
}
void GfxBuiltinBatchUI::_stencilTest(VkCommandBuffer &queueCommandBuffer, const GfxRendererState &rendererState)
{
    if (rendererState.stencilFrontPassOp == GfxRendererStateStencilOp::Increment_Add)
    {
        GfxBuiltinRenderer::StencilRef++;
        vkCmdSetStencilReference(queueCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 1);      // 增加和减少每次都是固定值1
        vkCmdSetStencilCompareMask(queueCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
        vkCmdSetStencilWriteMask(queueCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF);   // 写入所有位
    }
    else if (rendererState.stencilFrontPassOp == GfxRendererStateStencilOp::Decrement_Subtract)
    {
        GfxBuiltinRenderer::StencilRef--;
        vkCmdSetStencilReference(queueCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 1);      // 增加和减少每次都是固定值1
        vkCmdSetStencilCompareMask(queueCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
        vkCmdSetStencilWriteMask(queueCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF);   // 写入所有位
    }
    else
    {
        vkCmdSetStencilReference(queueCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, GfxBuiltinRenderer::StencilRef);
        vkCmdSetStencilCompareMask(queueCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
        vkCmdSetStencilWriteMask(queueCommandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0x00);   // 不写入模板（保持遮罩）
    }
}

void GfxBuiltinBatchUI::_setViewportScissor(VkCommandBuffer &queueCommandBuffer)
{
    GfxBuiltinBatch::_setViewportScissor(queueCommandBuffer);
}

void GfxBuiltinBatchUI::_bindDescriptorSets(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline, GfxBuffer *ubo)
{
    VkDescriptorSet descriptor = this->_renderer->getUIDescriptorSet();
    std::array<VkWriteDescriptorSet, 5> descriptorWrites{};
    // 绑定ubo
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = ubo->getBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(float) * (16 + 16 + 1); // 视图矩阵+投影矩阵+全局时间
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptor;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;
    // 绑定采样器
    std::array<VkDescriptorImageInfo, 4> imageInfos;
    GfxTexture *defaultTexture = Gfx::_textures.at("Gfx::Texture::default.png");
    for (size_t i = 0; i < 4; i++)
    {
        imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        if (i < this->_material->getTextures().size())
        {
            imageInfos[i].imageView = defaultTexture->getImageView();
            imageInfos[i].sampler = defaultTexture->getSampler();
        }
        else
        {
            if (Gfx::_textures.find(this->_material->getTextures()[i]) != Gfx::_textures.end())
            {
                GfxTexture *texture = Gfx::_textures.at(this->_material->getTextures()[i]);
                imageInfos[i].imageView = texture->getImageView();
                imageInfos[i].sampler = texture->getSampler();
            }
            else
            {
                imageInfos[i].imageView = defaultTexture->getImageView();
                imageInfos[i].sampler = defaultTexture->getSampler();
            }
        }
        descriptorWrites[i + 1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[i + 1].dstSet = descriptor;
        descriptorWrites[i + 1].dstBinding = static_cast<uint32_t>(i + 1);
        descriptorWrites[i + 1].dstArrayElement = 0;
        descriptorWrites[i + 1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[i + 1].descriptorCount = 1;
        descriptorWrites[i + 1].pImageInfo = &imageInfos[i];
    }

    vkUpdateDescriptorSets(Gfx::_context->getVkDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    vkCmdBindDescriptorSets(queueCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipelineLayout(), 0, 1, &descriptor, 0, nullptr);
}
void GfxBuiltinBatchUI::_drawIndexed(VkCommandBuffer &queueCommandBuffer)
{
    GfxBuiltinBatch::_drawIndexed(queueCommandBuffer);
}

void GfxBuiltinBatchUI::_bindVertexIndicesBuffers(VkCommandBuffer &queueCommandBuffer)
{
    GfxBuiltinBatch::_bindVertexIndicesBuffers(queueCommandBuffer);
}

GfxBuiltinBatchUI::~GfxBuiltinBatchUI()
{
}