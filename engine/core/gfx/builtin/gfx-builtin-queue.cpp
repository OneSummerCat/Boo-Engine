#include "gfx-builtin-queue.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../gfx-mgr.h"
#include "../base/gfx-render-pass.h"
#include "../base/gfx-render-texture.h"
#include "../base/gfx-texture.h"
#include "../base/gfx-material.h"
#include "../base/gfx-mesh.h"
#include "../base/gfx-buffer.h"
#include "../base/gfx-buffer-ubo.h"
#include "gfx-builtin-render-pass.h"
#include "gfx-builtin-pipeline/gfx-builtin-pipeline.h"
#include "gfx-builtin-batch/gfx-builtin-batch.h"
#include "gfx-builtin-batch/gfx-builtin-batch-ui.h"
#include "gfx-builtin-batch/gfx-builtin-batch-3d.h"
#include "gfx-builtin-renderer.h"
#include "../../../log.h"

GfxBuiltinQueue::GfxBuiltinQueue(GfxBuiltinRenderer *renderer, GfxRenderTexture *renderTexture)
{
    this->_renderer = renderer;
    this->_renderTexture = renderTexture;
}
void GfxBuiltinQueue::init()
{
}
void GfxBuiltinQueue::submitData(const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen)
{
    this->_viewMatrix = viewMatrix;
    this->_projMatrix = projMatrix;
    this->_isOnScreen = isOnScreen;
}
bool GfxBuiltinQueue::getIsOnScreen()
{
    return this->_isOnScreen;
}
int GfxBuiltinQueue::getPriority() const
{
    return this->_priority;
}
void GfxBuiltinQueue::setPriority(int priority)
{
    this->_priority = priority;
}
void GfxBuiltinQueue::submitObject(GfxMaterial *material, GfxMesh *mesh)
{
    if (material == nullptr || mesh == nullptr)
    {
        LOGE("[Gfx : QueueBuiltin] :: submitObject: material or mesh is nullptr");
        return;
    }
    if (this->_batches.empty())
    {
        this->_createNewBatch(material, mesh);
    }
    else
    {
        GfxBuiltinBatch *batch = this->_batches.back();
        if (!material->equals(batch->getMaterial()) || !mesh->equals(batch->getMesh()))
        {
            this->_createNewBatch(material, mesh);
        }
    }
    GfxBuiltinBatch *batch = this->_batches.back();
    batch->addObject(material->getInstanceData());
}
void GfxBuiltinQueue::_createNewBatch(GfxMaterial *material, GfxMesh *mesh)
{
    const GfxRendererState &pipelineState = material->getRendererState();
    if (pipelineState.renderer == GfxRendererCategory::_UI)
    {
        GfxBuiltinBatchUI *batch = new GfxBuiltinBatchUI(this->_renderer, this->_renderTexture, material, mesh);
        this->_batches.push_back(batch);
    }else if (pipelineState.renderer == GfxRendererCategory::_3D)
    {
        GfxBuiltinBatch3D *batch = new GfxBuiltinBatch3D(this->_renderer, this->_renderTexture, material, mesh);
        this->_batches.push_back(batch);
    }else {
        LOGE("[Gfx : QueueBuiltin] :: _createNewBatch: pipelineState.renderer is not supported");
    }
}

void GfxBuiltinQueue::render(std::vector<VkCommandBuffer> &commandBuffers)
{
    this->_resetCommandBuffer();
    this->_beginCommandBuffer();
    this->_beginRenderPass();
    this->_bindUniformBuffer();

    for (size_t i = 0; i < this->_batches.size(); i++)
    {
        GfxBuiltinBatch *batch = this->_batches[i];
        batch->render(this->_renderTexture->getCommandBuffer(), this->_ubo);
    }
    // 渲染结束
    vkCmdEndRenderPass(this->_renderTexture->getCommandBuffer());
    // 结束渲染pass
    if (vkEndCommandBuffer(this->_renderTexture->getCommandBuffer()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer!");
    }
    commandBuffers.push_back(this->_renderTexture->getCommandBuffer());
    // 清空批次
    for (size_t i = 0; i < this->_batches.size(); i++)
    {
        GfxBuiltinBatch *batch = this->_batches[i];
        batch->destroy();
        delete batch;
        batch = nullptr;
    }
    this->_batches.clear();
}
void GfxBuiltinQueue::_resetCommandBuffer()
{
    vkResetCommandBuffer(this->_renderTexture->getCommandBuffer(), 0);
}
void GfxBuiltinQueue::_beginCommandBuffer()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(this->_renderTexture->getCommandBuffer(), &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }
}
void GfxBuiltinQueue::_beginRenderPass()
{
    GfxBuiltinRenderPass *pass = this->_renderer->getRenderPass();
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.framebuffer = this->_renderTexture->getFramebuffer();
    renderPassInfo.renderArea.extent = {this->_renderTexture->getWidth(), this->_renderTexture->getHeight()};
    renderPassInfo.renderPass = pass->getVKRenderPass();
    renderPassInfo.renderArea.offset = {0, 0};

    std::array<VkClearValue, 2> clearValues = {}; /*  // 至少4个，因为最高索引是3 */
    clearValues[0].color = {{0.0f, 1.0f, 0.0f, 0.0f}};
    clearValues[1].depthStencil = {1.0f, 0}; /* // 深度=1.0f，模板=0 */
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(this->_renderTexture->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}
GfxRenderTexture *GfxBuiltinQueue::getRenderTexture()
{
    return this->_renderTexture;
}
void GfxBuiltinQueue::_bindUniformBuffer()
{
    this->_ubo = Gfx::_bufferUBO->getBuffer(16 + 16 + 1);
    this->_ubo->setIsOccupied(true);
    // 提交视图矩阵和投影矩阵
    memcpy(this->_ubo->getMappedData(), this->_viewMatrix.data(), sizeof(float) * 16);
    memcpy((char *)this->_ubo->getMappedData() + sizeof(float) * 16, this->_projMatrix.data(), sizeof(float) * 16);
    // 提交全局时间
    memcpy((char *)this->_ubo->getMappedData() + sizeof(float) * 16 * 2, &Gfx::_time, sizeof(float));
}

void GfxBuiltinQueue::_clean()
{
}
void GfxBuiltinQueue::_reset()
{
}
void GfxBuiltinQueue::destroy()
{
    // 清空批次
    for (size_t i = 0; i < this->_batches.size(); i++)
    {
        GfxBuiltinBatch *batch = this->_batches[i];
        batch->destroy();
        delete batch;
        batch = nullptr;
    }
    this->_batches.clear();
    this->_renderer = nullptr;
    this->_renderTexture = nullptr;
}

GfxBuiltinQueue::~GfxBuiltinQueue()
{
}
