#include "gfx-builtin-pipeline-ui.h"
#include "../../gfx.h"
#include "../../gfx-context.h"
#include "../../gfx-renderer.h"
#include "../../base/gfx-pipeline.h"
#include "../../base/gfx-shader.h"
#include "../../base/gfx-render-pass.h"
#include "../../../log.h"

GfxBuiltinPipelineUI::GfxBuiltinPipelineUI(const std::string &name) : GfxBuiltinPipeline(name)
{
}
void GfxBuiltinPipelineUI::_createPipeline()
{
    GfxBuiltinPipeline::_createPipeline();
}
void GfxBuiltinPipelineUI::_initShaderState()
{
    GfxBuiltinPipeline::_initShaderState();
}
/**
 * @brief 初始化顶点输入状态
 * 基础的顶点输入状态，只包含位置和纹理坐标
 */
void GfxBuiltinPipelineUI::_initVertexInputState()
{
        // 初始化顶点输入属性描述
    this->_vInputBindDescription = {};
    this->_vInputBindDescription.binding = 0;
    this->_vInputBindDescription.stride = sizeof(float) * (3 + 2);
    this->_vInputBindDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    // 初始化实例输入属性描述
    this->_vInputBindInstanceDescription = {};
    this->_vInputBindInstanceDescription.binding = 1;
    this->_vInputBindInstanceDescription.stride = sizeof(float) * (16 + 4); // 16个float(模型变换矩阵) + 4个float（颜色）
    this->_vInputBindInstanceDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

    this->_vertexInputAttributes = {
        {
            // 顶点坐标
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = 0,
        },
        {
            // 顶点纹理坐标
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = sizeof(float) * 3,
        },
        //-----实例
        // 矩阵第一行 (location 2)
        {
            .location = 2,
            .binding = 1, // 从绑定1读取！
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = 0,
        },
        // 矩阵第二行 (location 3)
        {
            .location = 3,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 4,
        },
        // 矩阵第三行 (location 4)
        {
            .location = 4,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 8,
        },
        // 矩阵第四行 (location 5)
        {
            .location = 5,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 12,
        },
        // 颜色 (location 6)
        {
            .location = 6,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 16,
        },
    };

    this->_vertexInputBindings.push_back(this->_vInputBindDescription);
    this->_vertexInputBindings.push_back(this->_vInputBindInstanceDescription);

    this->_vertexInputInfo = {};
    this->_vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    this->_vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)this->_vertexInputBindings.size();
    this->_vertexInputInfo.pVertexBindingDescriptions = this->_vertexInputBindings.data();
    this->_vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)this->_vertexInputAttributes.size();
    this->_vertexInputInfo.pVertexAttributeDescriptions = this->_vertexInputAttributes.data();
}
void GfxBuiltinPipelineUI::_initInputAssemblyState()
{
    GfxBuiltinPipeline::_initInputAssemblyState();
}
void GfxBuiltinPipelineUI::_initDynamicState()
{
    GfxBuiltinPipeline::_initDynamicState();
}
void GfxBuiltinPipelineUI::_initViewportState()
{
    GfxBuiltinPipeline::_initViewportState();
}
void GfxBuiltinPipelineUI::_initRasterizationState()
{
    GfxBuiltinPipeline::_initRasterizationState();
}

void GfxBuiltinPipelineUI::_initMultisampleState()
{
    GfxBuiltinPipeline::_initMultisampleState();
}
void GfxBuiltinPipelineUI::_initDepthStencilState()
{
    GfxBuiltinPipeline::_initDepthStencilState();
}
void GfxBuiltinPipelineUI::_initColorBlendState()
{
    GfxBuiltinPipeline::_initColorBlendState();
}
void GfxBuiltinPipelineUI::_initPipelineLayout()
{
    GfxBuiltinPipeline::_initPipelineLayout();
}
void GfxBuiltinPipelineUI::_initPipeline()
{
    GfxBuiltinPipeline::_initPipeline();
}

GfxBuiltinPipelineUI::~GfxBuiltinPipelineUI()
{
}