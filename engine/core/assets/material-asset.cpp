#include "material-asset.h"
#include "../gfx/gfx-mgr.h"
#include "../utils/uuid-util.h"
#include "../gfx/base/gfx-pipeline-struct.h"
#include "../gfx/base/gfx-push-constants.h"
#include "../gfx/base/gfx-mesh.h"
#include "../gfx/base/gfx-material.h"
#include "../utils/file-util.h"
#include "../utils/json-util.h"

MaterialAsset::MaterialAsset() : Asset()
{
    this->_type = AssetType::Material;
    this->_gfxMaterial = new GfxMaterial();
}
MaterialAsset::MaterialAsset(const std::string uuid) : Asset(uuid)
{
    this->_type = AssetType::Material;
    this->_gfxMaterial = new GfxMaterial();
}
void MaterialAsset::create(std::string path)
{
    Asset::create(path);
    json materialJson = FileUtil::readJsonFromText(this->_path);
    this->_initProperties(materialJson);
    // 初始化管线结构体
    this->_initPipelineStruct();
}

void MaterialAsset::create(json &materialData)
{
    this->_initProperties(materialData);
    // 初始化管线结构体
    this->_initPipelineStruct();
}
void MaterialAsset::create(MaterialAsset *mtl)
{
    this->_render = mtl->getRender();
    this->_vert = mtl->getVert();
    this->_frag = mtl->getFrag();
    // 多边形模式
    this->_polygonMode = mtl->getPolygonMode();
    // 剔除模式
    this->_cullMode = mtl->getCullMode();
    // 深度测试
    this->_depthTest = mtl->getDepthTest();
    // 深度写入
    this->_depthWrite = mtl->getDepthWrite();
    // 深度比较操作
    this->_depthCompareOp = mtl->getDepthCompareOp();
    // 模板测试
    this->_stencilTest = mtl->getStencilTest();
    // 模板比较操作
    this->_stencilFrontCompareOp = mtl->getStencilFrontCompareOp();
    // 模板失败操作
    this->_stencilFrontFailOp = mtl->getStencilFrontFailOp();
    // 模板深度失败操作
    this->_stencilFrontDepthFailOp = mtl->getStencilFrontDepthFailOp();
    // 模板通过操作
    this->_stencilFrontPassOp = mtl->getStencilFrontPassOp();
    // 模板比较掩码
    this->_stencilFrontCompareMask = mtl->getStencilFrontCompareMask();
    // 模板写入掩码
    this->_stencilFrontWriteMask = mtl->getStencilFrontWriteMask();
    // 模板参考值
    this->_stencilFrontRreference = mtl->getStencilFrontRreference();
    // 模板比较操作
    this->_stencilBackCompareOp = mtl->getStencilBackCompareOp();
    // 模板失败操作
    this->_stencilBackFailOp = mtl->getStencilBackFailOp();
    // 模板深度失败操作
    this->_stencilBackDepthFailOp = mtl->getStencilBackDepthFailOp();
    // 模板通过操作
    this->_stencilBackPassOp = mtl->getStencilBackPassOp();
    // 模板比较掩码
    this->_stencilBackCompareMask = mtl->getStencilBackCompareMask();
    // 模板写入掩码
    this->_stencilBackWriteMask = mtl->getStencilBackWriteMask();
    // 模板参考值
    this->_stencilBackRreference = mtl->getStencilBackRreference();
    // 颜色混合
    this->_colorBlend = mtl->getColorBlend();
    // 源颜色混合因子
    this->_srcColorBlendFactor = mtl->getSrcColorBlendFactor();
    // 目标颜色混合因子
    this->_dstColorBlendFactor = mtl->getDstColorBlendFactor();
    // 颜色混合操作
    this->_colorBlendOp = mtl->getColorBlendOp();
    // 目标Alpha混合因子
    this->_dstAlphaBlendFactor = mtl->getDstAlphaBlendFactor();
    // 颜色混合操作
    this->_colorBlendOp = mtl->getColorBlendOp();
    // 初始化管线结构体
    this->_initPipelineStruct();
}
void MaterialAsset::_initProperties(json &materialJson)
{
    this->_render = materialJson["render"].get<int>();
   
    if (materialJson.contains("vert"))
    {
        this->_vert = materialJson["vert"].get<std::string>();
        if (this->_vert.empty())
        {
            // this->_vert = this->_render == 0 ? "builtin-ui.vert" : "builtin-vert.vert";
            this->_vert = "builtin-ui.vert";
        }
    }
    if (materialJson.contains("frag"))
    {
        this->_frag = materialJson["frag"].get<std::string>();
        if (this->_frag.empty())
        {
            this->_frag = this->_render == 0 ? "builtin-ui.frag" : "builtin-frag.frag";
        }
    }
    // 多边形模式
    if (materialJson.contains("polygonMode"))
    {
        this->_polygonMode = materialJson["polygonMode"].get<int>();
    }
    else
    {
        this->_polygonMode = static_cast<int>(GfxPipelinePolygonMode::Fill);
    }
    // 剔除模式
    if (materialJson.contains("cullMode"))
    {
        this->_cullMode = materialJson["cullMode"].get<int>();
    }
    else
    {
        this->_cullMode = 0;
    }
    // 深度测试
    if (materialJson.contains("depthTest"))
    {
        this->_depthTest = materialJson["depthTest"].get<int>();
    }
    else
    {
        this->_depthTest = 0;
    }
    // 深度写入
    if (materialJson.contains("depthWrite"))
    {
        this->_depthWrite = materialJson["depthWrite"].get<int>();
    }
    else
    {
        this->_depthWrite = 0;
    }
    // 深度比较操作
    if (materialJson.contains("depthCompareOp"))
    {
        this->_depthCompareOp = materialJson["depthCompareOp"].get<int>();
    }
    else
    {
        this->_depthCompareOp = static_cast<int>(GfxPipelineCompareOp::Always);
    }

    if (materialJson.contains("stencilTest"))
    {
        this->_stencilTest = materialJson["stencilTest"].get<int>();
    }
    else
    {
        this->_stencilTest = 0;
    }
    // 模版测试 比较操作
    if (materialJson.contains("stencilFrontCompareOp"))
    {
        this->_stencilFrontCompareOp = materialJson["stencilFrontCompareOp"].get<int>();
    }
    else
    {
        this->_stencilFrontCompareOp = static_cast<int>(GfxPipelineCompareOp::Equal);
    }
    // 模版测试 失败操作
    if (materialJson.contains("stencilFrontFailOp"))
    {
        this->_stencilFrontFailOp = materialJson["stencilFrontFailOp"].get<int>();
    }
    else
    {
        this->_stencilFrontFailOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    }
    // 模版测试 深度失败操作
    if (materialJson.contains("stencilFrontDepthFailOp"))
    {
        this->_stencilFrontDepthFailOp = materialJson["stencilFrontDepthFailOp"].get<int>();
    }
    else
    {
        this->_stencilFrontDepthFailOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    }
    // 模版测试 通过操作
    if (materialJson.contains("stencilFrontPassOp"))
    {
        this->_stencilFrontPassOp = materialJson["stencilFrontPassOp"].get<int>();
    }
    else
    {
        this->_stencilFrontPassOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    }
    // 模版测试 比较掩码
    if (materialJson.contains("stencilBackCompareOp"))
    {
        this->_stencilBackCompareOp = materialJson["stencilBackCompareOp"].get<int>();
    }
    else
    {
        this->_stencilBackCompareOp = static_cast<int>(GfxPipelineCompareOp::Equal);
    }
    // 模版测试 写入掩码
    if (materialJson.contains("stencilBackFailOp"))
    {
        this->_stencilBackFailOp = materialJson["stencilBackFailOp"].get<int>();
    }
    else
    {
        this->_stencilBackFailOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    }
    // 模版测试 深度失败操作
    if (materialJson.contains("stencilBackDepthFailOp"))
    {
        this->_stencilBackDepthFailOp = materialJson["stencilBackDepthFailOp"].get<int>();
    }
    else
    {
        this->_stencilBackDepthFailOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    }
    // 模版测试 通过操作
    if (materialJson.contains("stencilBackPassOp"))
    {
        this->_stencilBackPassOp = materialJson["stencilBackPassOp"].get<int>();
    }
    else
    {
        this->_stencilBackPassOp = static_cast<int>(GfxPipelineStencilOp::Keep);
    }
    // 颜色混合 开启
    if (materialJson.contains("colorBlend"))
    {
        this->_colorBlend = materialJson["colorBlend"].get<int>();
    }
    else
    {
        this->_colorBlend = 0;
    }
    // 颜色混合 源因子
    if (materialJson.contains("srcColorBlendFactor"))
    {
        this->_srcColorBlendFactor = materialJson["srcColorBlendFactor"].get<int>();
    }
    else
    {
        this->_srcColorBlendFactor = 4;
    }
    // 颜色混合 目标因子
    if (materialJson.contains("dstColorBlendFactor"))
    {
        this->_dstColorBlendFactor = materialJson["dstColorBlendFactor"].get<int>();
    }
    else
    {
        this->_dstColorBlendFactor = 6;
    }
    // 颜色混合 混合操作
    if (materialJson.contains("colorBlendOp"))
    {
        this->_colorBlendOp = materialJson["colorBlendOp"].get<int>();
    }
    else
    {
        this->_colorBlendOp = 0;
    }
    // 透明度混合 源因子
    if (materialJson.contains("srcAlphaBlendFactor"))
    {
        this->_srcAlphaBlendFactor = materialJson["srcAlphaBlendFactor"].get<int>();
    }
    else
    {
        this->_srcAlphaBlendFactor = 0;
    }
    // 透明度混合 目标因子
    if (materialJson.contains("dstAlphaBlendFactor"))
    {
        this->_dstAlphaBlendFactor = materialJson["dstAlphaBlendFactor"].get<int>();
    }
    else
    {
        this->_dstAlphaBlendFactor = 6;
    }
    // 透明度混合 混合操作
    if (materialJson.contains("alphaBlendOp"))
    {
        this->_alphaBlendOp = materialJson["alphaBlendOp"].get<int>();
    }
    else
    {
        this->_alphaBlendOp = 0;
    }
}
void MaterialAsset::_initPipelineStruct()
{
    GfxPipelineStruct uiPipeline = {};
    uiPipeline.render = this->_render;
    uiPipeline.vert = this->_vert;
    uiPipeline.frag = this->_frag;
    // 多边形模式 填充
    uiPipeline.polygonMode = static_cast<GfxPipelinePolygonMode>(this->_polygonMode);
    // 剔除模式 背面
    uiPipeline.cullMode = static_cast<GfxPipelineCullMode>(this->_cullMode);
    // 深度测试 开启
    uiPipeline.depthTest = this->_depthTest;
    // 深度写入 开启
    uiPipeline.depthWrite = this->_depthWrite;
    // 深度比较操作 小于等于
    uiPipeline.depthCompareOp = static_cast<GfxPipelineCompareOp>(this->_depthCompareOp);

    // 模版测试 启用（用于UI遮罩）
    uiPipeline.stencilTest = this->_stencilTest;
    uiPipeline.stencilFrontCompareOp = static_cast<GfxPipelineCompareOp>(this->_stencilFrontCompareOp);     // 只在模板值相等时绘制
    uiPipeline.stencilFrontFailOp = static_cast<GfxPipelineStencilOp>(this->_stencilFrontFailOp);           // 测试失败：保持
    uiPipeline.stencilFrontDepthFailOp = static_cast<GfxPipelineStencilOp>(this->_stencilFrontDepthFailOp); // 深度 失败：保持
    uiPipeline.stencilFrontPassOp = static_cast<GfxPipelineStencilOp>(this->_stencilFrontPassOp);           // 测试通过：保持（不修改模板值）
    uiPipeline.stencilBackCompareOp = static_cast<GfxPipelineCompareOp>(this->_stencilBackCompareOp);
    uiPipeline.stencilBackFailOp = static_cast<GfxPipelineStencilOp>(this->_stencilBackFailOp);
    uiPipeline.stencilBackDepthFailOp = static_cast<GfxPipelineStencilOp>(this->_stencilBackDepthFailOp);
    uiPipeline.stencilBackPassOp = static_cast<GfxPipelineStencilOp>(this->_stencilBackPassOp);
    // 颜色混合 开启
    uiPipeline.colorBlend = this->_colorBlend;
    uiPipeline.srcColorBlendFactor = static_cast<GfxPipelineColorBlendFactor>(this->_srcColorBlendFactor);
    uiPipeline.dstColorBlendFactor = static_cast<GfxPipelineColorBlendFactor>(this->_dstColorBlendFactor);
    uiPipeline.colorBlendOp = static_cast<GfxPipelineColorBlendOp>(this->_colorBlendOp);    
    uiPipeline.srcAlphaBlendFactor = static_cast<GfxPipelineColorBlendFactor>(this->_srcAlphaBlendFactor);
    uiPipeline.dstAlphaBlendFactor = static_cast<GfxPipelineColorBlendFactor>(this->_dstAlphaBlendFactor);
    uiPipeline.alphaBlendOp = static_cast<GfxPipelineColorBlendOp>(this->_alphaBlendOp);
    // 推送常量 开启
    uiPipeline.pushConstant = 1;
    uiPipeline.pushConstantSize = 0;

    this->_gfxMaterial->setPipelineStruct(uiPipeline);
}
void MaterialAsset::setTexture(const std::string &texture)
{
    this->_textures.resize(1);
    this->_textures[0] = texture;
    this->_gfxMaterial->setTextures(this->_textures);
}

void MaterialAsset::destroy()
{
}

// void MaterialAsset::createUITest()
// {
//     GfxPipelineStruct uiPipeline = {};
//     uiPipeline.render = 1;
//     uiPipeline.vert = "builtin-ui.vert";
//     uiPipeline.frag = "builtin-ui.frag";
//     // 多边形模式 填充
//     uiPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
//     // 剔除模式 背面
//     uiPipeline.cullMode = GfxPipelineCullMode::None;
//     // 深度测试 开启
//     uiPipeline.depthTest = 0;
//     // 深度写入 开启
//     uiPipeline.depthWrite = 0;
//     // 深度比较操作 小于等于
//     uiPipeline.depthCompareOp = GfxPipelineCompareOp::Always;

//     // 模版测试 启用（用于UI遮罩）
//     uiPipeline.stencilTest = 1;
//     uiPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Equal;  // 只在模板值相等时绘制
//     uiPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;      // 测试失败：保持
//     uiPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep; // 深度失败：保持
//     uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Keep;      // 测试通过：保持（不修改模板值）
//     uiPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Equal;
//     uiPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
//     uiPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
//     uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Keep;
//     // 颜色混合 开启
//     uiPipeline.colorBlend = 1;
//     uiPipeline.srcColorBlendFactor = GfxPipelineColorBlendFactor::SrcAlpha;
//     uiPipeline.dstColorBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
//     uiPipeline.colorBlendOp = GfxPipelineColorBlendOp::Add;
//     uiPipeline.srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
//     uiPipeline.dstAlphaBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
//     uiPipeline.alphaBlendOp = GfxPipelineColorBlendOp::Add;
//     // 推送常量 开启
//     uiPipeline.pushConstant = 1;
//     uiPipeline.pushConstantSize = 0;

//     this->_gfxMaterial = new GfxMaterial();
//     this->_gfxMaterial->setPipelineStruct(uiPipeline);
//     std::vector<std::string> aaas;
//     aaas.push_back("_private/default.png");
//     this->_gfxMaterial->setTextures(aaas);
// }
// void MaterialAsset::createUIMaskTest(int maskType)
// {
//     GfxPipelineStruct uiPipeline = {};
//     uiPipeline.render = 1;
//     uiPipeline.vert = "builtin-ui-mask.vert";
//     uiPipeline.frag = "builtin-ui-mask.frag";
//     // 多边形模式 填充
//     uiPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
//     // 剔除模式 背面
//     uiPipeline.cullMode = GfxPipelineCullMode::None;
//     // 深度测试 开启
//     uiPipeline.depthTest = 0;
//     // 深度写入 开启
//     uiPipeline.depthWrite = 0;
//     // 深度比较操作 小于等于
//     uiPipeline.depthCompareOp = GfxPipelineCompareOp::Always;

//     // 模版测试 启用（用于UI遮罩）
//     uiPipeline.stencilTest = 1;
//     uiPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Always; // 只在模板值相等时绘制
//     uiPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;      // 测试失败：保持
//     uiPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep; // 深度：保持
//     if (maskType == 0)
//     {
//         uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Increment_Add; // 测试通过：保持（不修改模板值）
//     }
//     else
//     {
//         uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Decrement_Subtract;
//     }
//     uiPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Always;
//     uiPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
//     uiPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
//     if (maskType == 0)
//     {
//         uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Increment_Add;
//     }
//     else
//     {
//         uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Decrement_Subtract;
//     }
//     // 颜色混合 开启
//     uiPipeline.colorBlend = 0;
//     // 推送常量 开启
//     uiPipeline.pushConstant = 1;
//     uiPipeline.pushConstantSize = 0;

//     this->_gfxMaterial = new GfxMaterial();
//     this->_gfxMaterial->setPipelineStruct(uiPipeline);
//     std::vector<std::string> aaas;
//     aaas.push_back("_private/default.png");
//     this->_gfxMaterial->setTextures(aaas);
// }
// void MaterialAsset::createMaskUITest()
// {
//     GfxPipelineStruct uiPipeline = {};
//     uiPipeline.render = 1;
//     uiPipeline.vert = "builtin-ui-mask-test.vert";
//     uiPipeline.frag = "builtin-ui-mask-test.frag";
//     // 多边形模式 填充
//     uiPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
//     // 剔除模式 背面
//     uiPipeline.cullMode = GfxPipelineCullMode::None;
//     // 深度测试 开启
//     uiPipeline.depthTest = 0;
//     // 深度写入 开启
//     uiPipeline.depthWrite = 0;
//     // 深度比较操作 小于等于
//     uiPipeline.depthCompareOp = GfxPipelineCompareOp::Always;

//     // 模版测试 启用（用于UI遮罩）
//     uiPipeline.stencilTest = 1;
//     uiPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Equal;  // 只在模板值相等时绘制
//     uiPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;      // 测试失败：保持
//     uiPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep; // 深度失败：保持
//     uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Keep;      // 测试通过：保持（不修改模板值）
//     uiPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Equal;
//     uiPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
//     uiPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
//     uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Keep;
//     // 颜色混合 开启
//     uiPipeline.colorBlend = 1;
//     uiPipeline.srcColorBlendFactor = GfxPipelineColorBlendFactor::SrcAlpha;
//     uiPipeline.dstColorBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
//     uiPipeline.colorBlendOp = GfxPipelineColorBlendOp::Add;
//     uiPipeline.srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
//     uiPipeline.dstAlphaBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
//     uiPipeline.alphaBlendOp = GfxPipelineColorBlendOp::Add;
//     // 推送常量 开启
//     uiPipeline.pushConstant = 1;
//     uiPipeline.pushConstantSize = 0;

//     this->_gfxMaterial = new GfxMaterial();
//     this->_gfxMaterial->setPipelineStruct(uiPipeline);
//     std::vector<std::string> aaas;
//     aaas.push_back("_private/default.png");
//     this->_gfxMaterial->setTextures(aaas);
// }
// void MaterialAsset::setTextures(const std::vector<std::string> textures)
// {
//     this->_gfxMaterial->setTextures(textures);
// }
// void MaterialAsset::setTexture(int index, std::string texture)
// {
//     this->_gfxMaterial->setTextures({texture});
// }