#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "../utils/json-util.h"

class GfxMaterial;

class MaterialAsset : public Asset
{
private:
    GfxMaterial *_gfxMaterial;
    std::vector<std::string> _textures;

private:
    // UI和3D材质
    int _render = 0;
    /**
     * @brief 顶点着色器id
     */
    std::string _vert;
    /**
     * @brief 片段着色器id
     */
    std::string _frag;
    /**
     * 多边形模式
     */
    int _polygonMode = 0;
    /**
     * 剔除模式
     */
    int _cullMode = 0;
    /**
     * 深度测试
     * 开关
     */
    int _depthTest = 0;
    /**
     * 深度写入
     * 开关
     */
    int _depthWrite = 0;
    /**
     * 深度比较操作
     */
    int _depthCompareOp = 0;

    /**
     * 模板测试
     * 开关
     */
    int _stencilTest = 0;
    /**
     * 正面三角形（逆时针）的Stencil操作
     */
    int _stencilFrontCompareOp = 0;
    int _stencilFrontFailOp = 0;
    int _stencilFrontDepthFailOp = 0;
    int _stencilFrontPassOp = 0;
    uint32_t _stencilFrontCompareMask = 0xFFFFFFFF;
    uint32_t _stencilFrontWriteMask = 0xFFFFFFFF;
    uint32_t _stencilFrontRreference = 0;
    /**
     * 反面三角形（顺时针）的Stencil操作
     */
    int _stencilBackCompareOp = 0;
    int _stencilBackFailOp = 0;
    int _stencilBackDepthFailOp = 0;
    int _stencilBackPassOp = 0;
    uint32_t _stencilBackCompareMask = 0xFFFFFFFF;
    uint32_t _stencilBackWriteMask = 0xFFFFFFFF;
    uint32_t _stencilBackRreference = 0;

    /**
     * 颜色缓和
     * 开关
     */
    int _colorBlend = 0;
    /**
     * 混合模式
     */
    /**
     * 源颜色混合因子
     */
    int _srcColorBlendFactor = 0;
    /**
     * 目标颜色混合因子
     */
    int _dstColorBlendFactor = 0;
    /**
     * 颜色混合操作
     */
    int _colorBlendOp = 0;
    /**
     * 源alpha混合因子
     */
    int _srcAlphaBlendFactor = 0;
    /**
     * 目标alpha混合因子
     */
    int _dstAlphaBlendFactor = 0;
    /**
     * alpha混合操作
     */
    int _alphaBlendOp = 0;

    void _initProperties(json &materialJson);
    void _initPipelineStruct();

protected:
public:
    MaterialAsset();
    MaterialAsset(const std::string uuid);

    void create(std::string path) override;
    void create(json &materialData);
    void create(MaterialAsset *mtl);
    void setTexture(const std::string &texture);

    int getRender() { return _render; }
    std::string getVert() { return _vert; }
    std::string getFrag() { return _frag; }
    int getPolygonMode() { return _polygonMode; }
    int getCullMode() { return _cullMode; }
    int getDepthTest() { return _depthTest; }
    int getDepthWrite() { return _depthWrite; }
    int getDepthCompareOp() { return _depthCompareOp; }
    int getStencilTest() { return _stencilTest; }
    int getStencilFrontCompareOp() { return _stencilFrontCompareOp; }
    int getStencilFrontFailOp() { return _stencilFrontFailOp; }
    int getStencilFrontDepthFailOp() { return _stencilFrontDepthFailOp; }
    int getStencilFrontPassOp() { return _stencilFrontPassOp; }
    uint32_t getStencilFrontCompareMask() { return _stencilFrontCompareMask; }
    uint32_t getStencilFrontWriteMask() { return _stencilFrontWriteMask; }
    uint32_t getStencilFrontRreference() { return _stencilFrontRreference; }
    int getStencilBackCompareOp() { return _stencilBackCompareOp; }
    int getStencilBackFailOp() { return _stencilBackFailOp; }
    int getStencilBackDepthFailOp() { return _stencilBackDepthFailOp; }
    int getStencilBackPassOp() { return _stencilBackPassOp; }
    uint32_t getStencilBackCompareMask() { return _stencilBackCompareMask; }
    uint32_t getStencilBackWriteMask() { return _stencilBackWriteMask; }
    uint32_t getStencilBackRreference() { return _stencilBackRreference; }
    int getColorBlend() { return _colorBlend; }
    int getSrcColorBlendFactor() { return _srcColorBlendFactor; }
    int getDstColorBlendFactor() { return _dstColorBlendFactor; }
    int getColorBlendOp() { return _colorBlendOp; }
    int getSrcAlphaBlendFactor() { return _srcAlphaBlendFactor; }
    int getDstAlphaBlendFactor() { return _dstAlphaBlendFactor; }
    int getAlphaBlendOp() { return _alphaBlendOp; }

    GfxMaterial *getGfxMaterial() { return _gfxMaterial; }
    /**
     * @brief 创建图形资源
     * 参数 宏定义
     */
    virtual void destroy() override;
    ~MaterialAsset() {}
};

// void createUITest();
// void createUIMaskTest(int maskType);
// void createMaskUITest();

// /**
//  * @brief 管线状态
//  */
// json _pipeline = json::object();
// /**
//  * @brief 存储纹理
//  * 键值对 纹理名称-纹理id
//  * {
//  *  "index": 0,
//  *  "key": "texture0"
//  *  "value": "texture0"
//  * }
//  */
// json _textures = json::array();
// /**
//  * @brief 推送常量
//  * {
//  *  "index": 0,
//  *  "key": "pushConstant0"
//  *  "value": "pushConstant0"
//  * }
//  */
// json _pushConstants = json::array();
