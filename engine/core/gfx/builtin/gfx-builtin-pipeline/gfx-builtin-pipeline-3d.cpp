#include "gfx-builtin-pipeline-3d.h"
#include "../../gfx.h"
#include "../../gfx-context.h"
#include "../../gfx-renderer.h"
#include "../../base/gfx-pipeline.h"
#include "../../base/gfx-shader.h"
#include "../../base/gfx-render-pass.h"
#include "../../../log.h"

GfxBuiltinPipeline3D::GfxBuiltinPipeline3D(const std::string &name) : GfxBuiltinPipeline(name)
{
}
void GfxBuiltinPipeline3D::_createPipeline()
{
    GfxBuiltinPipeline::_createPipeline();
}
void GfxBuiltinPipeline3D::_initShaderState()
{
    GfxBuiltinPipeline::_initShaderState();
}
/**
 * @brief 初始化顶点输入状态
 * 基础的顶点输入状态，只包含位置和纹理坐标
 */
void GfxBuiltinPipeline3D::_initVertexInputState()
{
    // 初始化顶点输入属性描述
    this->_vInputBindDescription = {};
    this->_vInputBindDescription.binding = 0;
    // 顶点坐标 +顶点法线 + 纹理坐标+纹理坐标1+纹理坐标2+顶点颜色+顶点切线
    this->_vInputBindDescription.stride = sizeof(float) * (3 + 3 + 2 + 2 + 2 + 4 + 4);
    this->_vInputBindDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    // 初始化实例输入属性描述
    this->_vInputBindInstanceDescription = {};
    this->_vInputBindInstanceDescription.binding = 1;
    this->_vInputBindInstanceDescription.stride = sizeof(float) * (16 + 16); // 16个float(模型变换矩阵)+16个float(转置矩阵变换矩阵)
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
            // 顶点法线
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = sizeof(float) * 3,
        },
        {
            // 顶点纹理坐标
            .location = 2,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = sizeof(float) * (3 + 3),
        },
        {
            // 顶点纹理坐标1
            .location = 3,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = sizeof(float) * (3 + 3 + 2),
        },
        {
            // 顶点纹理坐标2
            .location = 4,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = sizeof(float) * (3 + 3 + 2 + 2),
        },
        {
            // 顶点颜色
            .location = 5,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * (3 + 3 + 2 + 2 + 2),
        },
        {
            // 顶点切线
            .location = 6,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = sizeof(float) * (3 + 3 + 2 + 2 + 2 + 4),
        },
        //-----实例
        // 矩阵第一行 (location 2)
        {
            .location = 7,
            .binding = 1, // 从绑定1读取！
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = 0,
        },
        // 矩阵第二行 (location 3)
        {
            .location = 8,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 4,
        },
        // 矩阵第三行 (location 4)
        {
            .location = 9,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 8,
        },
        // 矩阵第四行 (location 5)
        {
            .location = 10,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 12,
        },
        //-----模型逆转置矩阵
        // 矩阵第一行 (location 2)
        {
            .location = 11,
            .binding = 1, // 从绑定1读取！
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 16,
        },
        // 矩阵第二行 (location 3)
        {
            .location = 12,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 20,
        },
        // 矩阵第三行 (location 4)
        {
            .location = 13,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 24,
        },
        // 矩阵第四行 (location 5)
        {
            .location = 14,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 28,
        }};

    this->_vertexInputBindings.push_back(this->_vInputBindDescription);
    this->_vertexInputBindings.push_back(this->_vInputBindInstanceDescription);

    this->_vertexInputInfo = {};
    this->_vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    this->_vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)this->_vertexInputBindings.size();
    this->_vertexInputInfo.pVertexBindingDescriptions = this->_vertexInputBindings.data();
    this->_vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)this->_vertexInputAttributes.size();
    this->_vertexInputInfo.pVertexAttributeDescriptions = this->_vertexInputAttributes.data();
}
void GfxBuiltinPipeline3D::_initInputAssemblyState()
{
    GfxBuiltinPipeline::_initInputAssemblyState();
}
void GfxBuiltinPipeline3D::_initDynamicState()
{
    GfxBuiltinPipeline::_initDynamicState();
}
void GfxBuiltinPipeline3D::_initViewportState()
{
    GfxBuiltinPipeline::_initViewportState();
}
void GfxBuiltinPipeline3D::_initRasterizationState()
{
    GfxBuiltinPipeline::_initRasterizationState();
}

void GfxBuiltinPipeline3D::_initMultisampleState()
{
    GfxBuiltinPipeline::_initMultisampleState();
}
void GfxBuiltinPipeline3D::_initDepthStencilState()
{
    GfxBuiltinPipeline::_initDepthStencilState();
}
void GfxBuiltinPipeline3D::_initColorBlendState()
{
    GfxBuiltinPipeline::_initColorBlendState();
}
void GfxBuiltinPipeline3D::_initPipelineLayout()
{
    GfxBuiltinPipeline::_initPipelineLayout();
}
void GfxBuiltinPipeline3D::_initPipeline()
{
    GfxBuiltinPipeline::_initPipeline();
}

GfxBuiltinPipeline3D::~GfxBuiltinPipeline3D()
{
}