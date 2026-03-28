#include "gfx-material.h"
#include "../../../log.h"
#include "../gfx-mgr.h"
#include "../gfx.h"
#include "../gfx-context.h"

GfxMaterial::GfxMaterial()
{
    this->_rendererState = {};
    this->_instanceBlocks = {};
    this->_propertyBlocks = {};
    this->_textures = {};
}
void GfxMaterial::create(GfxRendererCategory type, const std::string &vert, const std::string &frag)
{
    if (type == GfxRendererCategory::_UI)
    {
        this->_rendererState.renderer = GfxRendererCategory::_UI;
        this->_textures.resize(4);
        this->_initUIInstanceDatas();
    }
    else if (type == GfxRendererCategory::_3D)
    {
        this->_rendererState.renderer = GfxRendererCategory::_3D;
        this->_textures.resize(10);
        this->_init3DInstanceDatas();
    }
    else
    {
        this->_rendererState.geom = "nosupport";
    }
    this->_rendererState.vert = vert;
    this->_rendererState.frag = frag;
    this->_rendererState.pushConstant=1;
}
void GfxMaterial::_initUIInstanceDatas()
{
    // 世界矩阵
    GfxMaterialDataBlock worldMatrix = {};
    worldMatrix.type = GfxMaterialPropertyType::Mat4;
    worldMatrix.key = "worldMat4";
    worldMatrix.offset = 0;
    worldMatrix.size = sizeof(float) * 16;
    this->_instanceBlocks[worldMatrix.key] = worldMatrix;
    // 颜色
    GfxMaterialDataBlock color = {};
    color.type = GfxMaterialPropertyType::Vec4;
    color.key = "color";
    color.offset = sizeof(float) * 16;
    color.size = sizeof(float) * 4;
    this->_instanceBlocks[color.key] = color;

    size_t reservedSize = sizeof(float) * 16 + sizeof(float) * 4;
    this->_instanceData.resize(reservedSize);
    // memset 是 C/C++ 中的一个内存设置函数，用于将一块内存区域的所有字节设置为指定的值。
    memset(this->_instanceData.data(), 0, reservedSize);
    this->setUIColor(1.0f, 1.0f, 1.0f, 1.0f);
}
void GfxMaterial::_init3DInstanceDatas()
{
    // 世界矩阵
    GfxMaterialDataBlock worldMatrix = {};
    worldMatrix.type = GfxMaterialPropertyType::Mat4;
    worldMatrix.key = "worldMat4";
    worldMatrix.offset = 0;
    worldMatrix.size = sizeof(float) * 16;
    this->_instanceBlocks[worldMatrix.key] = worldMatrix;
    size_t reservedSize = sizeof(float) * 16;
    this->_instanceData.resize(reservedSize);
    // memset 是 C/C++ 中的一个内存设置函数，用于将一块内存区域的所有字节设置为指定的值。
    memset(this->_instanceData.data(), 1, reservedSize);
}


void GfxMaterial::setProperties(std::vector<GfxMaterialDataBlock> &properties)
{
    size_t offset = 0;
    this->_propertySize = 0;
    this->_propertyKey = "t";
    for (auto &property : properties)
    {
        if (property.type == GfxMaterialPropertyType::Int)
        {
            this->_addBytes(this->_propertyData, property.data, property.size);
        }
        else if (property.type == GfxMaterialPropertyType::Float)
        {
            this->_addBytes(this->_propertyData, property.data, property.size);
        }
        else if (property.type == GfxMaterialPropertyType::Vec2)
        {
            this->_addBytes(this->_propertyData, property.data, property.size);
        }
        else if (property.type == GfxMaterialPropertyType::Vec3)
        {
            this->_addBytes(this->_propertyData, property.data, property.size);
        }
        else if (property.type == GfxMaterialPropertyType::Vec4)
        {
            property.size = sizeof(property.data);
            this->_addBytes(this->_propertyData, property.data, property.size);
        }
        offset += property.size;
        property.offset = offset;
        this->_propertyBlocks[property.key] = property;
        this->_propertySize += property.size;
        this->_propertyKey += std::string(property.key);
    }
    this->_rendererState.pushConstantKey = this->_propertyKey;
    this->_rendererState.pushConstantSize = this->_propertySize;
}
void GfxMaterial::setModelWorldMatrix(const std::array<float, 16> &modelWorldMatrix)
{
    memcpy(this->_instanceData.data(), modelWorldMatrix.data(), sizeof(modelWorldMatrix));
}
void GfxMaterial::setUIColor(const float r, const float g, const float b, const float a)
{
    if (this->_rendererState.renderer != GfxRendererCategory::_UI)
    {
        return;
    }
    float color[4] = {r, g, b, a};
    memcpy(this->_instanceData.data() + sizeof(float) * 16, color, sizeof(float) * 4);
}
void GfxMaterial::setTexture(const int index, const std::string &texture)
{
    if (index >= this->_textures.size())
    {
        return;
    }
    this->_textures[index] = texture;
}
void GfxMaterial::setPropertyInt(const std::string &key, const int value)
{
    memcpy(this->_propertyData.data() + this->_propertyBlocks[key].offset, &value, sizeof(int));
}
void GfxMaterial::setPropertyFloat(const std::string &key, const float value)
{
    memcpy(this->_propertyData.data() + this->_propertyBlocks[key].offset, &value, sizeof(float));
}
void GfxMaterial::setPropertyVec2(const std::string &key, const float x, const float y)
{
    float value[2] = {x, y};
    memcpy(this->_propertyData.data() + this->_propertyBlocks[key].offset, value, sizeof(float) * 2);
}
void GfxMaterial::setPropertyVec3(const std::string &key, const float x, const float y, const float z)
{
    float value[3] = {x, y, z};
    memcpy(this->_propertyData.data() + this->_propertyBlocks[key].offset, value, sizeof(float) * 3);
}
void GfxMaterial::setPropertyVec4(const std::string &key, const float x, const float y, const float z, const float w)
{
    float value[4] = {x, y, z, w};
    memcpy(this->_propertyData.data() + this->_propertyBlocks[key].offset, value, sizeof(float) * 4);
}

// 直接添加字节数据（最底层的方法）
void GfxMaterial::_addBytes(std::vector<char> &_originData, const void *data, size_t size)
{
    size_t oldSize = _originData.size();
    _originData.resize(oldSize + size);
    memcpy(_originData.data() + oldSize, data, size);
}
void GfxMaterial::setPolygonMode(GfxRendererStatePolygonMode polygonMode)
{
    this->_rendererState.polygonMode = polygonMode;
}
void GfxMaterial::setCullMode(GfxRendererStateCullMode cullMode)
{
    this->_rendererState.cullMode = cullMode;
}
void GfxMaterial::setDepthTest(int depthTest)
{
    this->_rendererState.depthTest = depthTest;
}
void GfxMaterial::setDepthWrite(int depthWrite)
{
    this->_rendererState.depthWrite = depthWrite;
}
void GfxMaterial::setDepthCompareOp(GfxRendererStateCompareOp depthCompareOp)
{
    this->_rendererState.depthCompareOp = depthCompareOp;
}
void GfxMaterial::setStencilTest(int stencilTest)
{
    this->_rendererState.stencilTest = stencilTest;
}
void GfxMaterial::setStencilFrontCompareOp(GfxRendererStateCompareOp stencilFrontCompareOp)
{
    this->_rendererState.stencilFrontCompareOp = stencilFrontCompareOp;
}
void GfxMaterial::setStencilFrontFailOp(GfxRendererStateStencilOp stencilFrontFailOp)
{
    this->_rendererState.stencilFrontFailOp = stencilFrontFailOp;
}
void GfxMaterial::setStencilFrontDepthFailOp(GfxRendererStateStencilOp stencilFrontDepthFailOp)
{
    this->_rendererState.stencilFrontDepthFailOp = stencilFrontDepthFailOp;
}
void GfxMaterial::setStencilFrontPassOp(GfxRendererStateStencilOp stencilFrontPassOp)
{
    this->_rendererState.stencilFrontPassOp = stencilFrontPassOp;
}
void GfxMaterial::setStencilFrontCompareMask(uint32_t _stencilFrontCompareMask)
{
    this->_rendererState.stencilFrontCompareMask = _stencilFrontCompareMask;
}
void GfxMaterial::setStencilFrontWriteMask(uint32_t _stencilFrontWriteMask)
{
    this->_rendererState.stencilFrontWriteMask = _stencilFrontWriteMask;
}
void GfxMaterial::setStencilFrontRreference(uint32_t _stencilFrontRreference)
{
    this->_rendererState.stencilFrontRreference = _stencilFrontRreference;
}
void GfxMaterial::setStencilBackCompareOp(GfxRendererStateCompareOp stencilBackCompareOp)
{
    this->_rendererState.stencilBackCompareOp = stencilBackCompareOp;
}
void GfxMaterial::setStencilBackFailOp(GfxRendererStateStencilOp stencilBackFailOp)
{
    this->_rendererState.stencilBackFailOp = stencilBackFailOp;
}
void GfxMaterial::setStencilBackDepthFailOp(GfxRendererStateStencilOp stencilBackDepthFailOp)
{
    this->_rendererState.stencilBackDepthFailOp = stencilBackDepthFailOp;
}
void GfxMaterial::setStencilBackPassOp(GfxRendererStateStencilOp stencilBackPassOp)
{
    this->_rendererState.stencilBackPassOp = stencilBackPassOp;
}
void GfxMaterial::setStencilBackCompareMask(uint32_t _stencilBackCompareMask)
{
    this->_rendererState.stencilBackCompareMask = _stencilBackCompareMask;
}
void GfxMaterial::setStencilBackWriteMask(uint32_t _stencilBackWriteMask)
{
    this->_rendererState.stencilBackWriteMask = _stencilBackWriteMask;
}
void GfxMaterial::setStencilBackRreference(uint32_t _stencilBackRreference)
{
    this->_rendererState.stencilBackRreference = _stencilBackRreference;
}
void GfxMaterial::setColorBlend(int colorBlend)
{
    this->_rendererState.colorBlend = colorBlend;
}
void GfxMaterial::setSrcColorBlendFactor(GfxRendererStateColorBlendFactor srcColorBlendFactor)
{
    this->_rendererState.srcColorBlendFactor = srcColorBlendFactor;
}
void GfxMaterial::setDstColorBlendFactor(GfxRendererStateColorBlendFactor dstColorBlendFactor)
{
    this->_rendererState.dstColorBlendFactor = dstColorBlendFactor;
}
void GfxMaterial::setColorBlendOp(GfxRendererStateColorBlendOp colorBlendOp)
{
    this->_rendererState.colorBlendOp = colorBlendOp;
}
void GfxMaterial::setSrcAlphaBlendFactor(GfxRendererStateColorBlendFactor srcAlphaBlendFactor)
{
    this->_rendererState.srcAlphaBlendFactor = srcAlphaBlendFactor;
}
void GfxMaterial::setDstAlphaBlendFactor(GfxRendererStateColorBlendFactor dstAlphaBlendFactor)
{
    this->_rendererState.dstAlphaBlendFactor = dstAlphaBlendFactor;
}
void GfxMaterial::setAlphaBlendOp(GfxRendererStateColorBlendOp alphaBlendOp)
{
    this->_rendererState.alphaBlendOp = alphaBlendOp;
}
/**
 * @brief 比较材质是否相等
 *
 * @param material
 * @return true
 * @return false
 */
bool GfxMaterial::equals(const GfxMaterial *material) const
{
    // if (this->_rendererState.generateKey() != material->_rendererState.generateKey())
    // {
    //     return false;
    // }
    // if (this->_textures.size() != material->_textures.size())
    // {
    //     return false;
    // }
    // for (int i = 0; i < this->_textures.size(); i++)
    // {
    //     if (this->_textures[i] != material->_textures[i])
    //     {
    //         return false;
    //     }
    // }
    // return true;
    return false;
}
const std::vector<std::string> &GfxMaterial::getTextures() const
{
    return this->_textures;
}
GfxRendererState GfxMaterial::getRendererState() const
{
    return this->_rendererState;
}
const std::vector<char> &GfxMaterial::getInstanceData() const
{
    return this->_instanceData;
}
// void GfxMaterial::testSyncGfxPipeline()
// {
//     std::cout << "testSyncGfxPipeline" << std::endl;
//     GfxMgr::getInstance()->createPipeline(this->_rendererState.generateKey(), this->_rendererState);
// }

// // 更新字节数据（基于偏移量的更新）
// void GfxMaterial::_updateBytes(size_t offset, const void *data, size_t size)
// {
//     // if (offset + size > this->_instanceData.size())
//     // {
//     //     // 越界保护
//     //     LOGE("Buffer overflow: offset=%zu, size=%zu, buffer_size=%zu",
//     //          offset, size, this->_instanceData.size());
//     //     return;
//     // }
//     // memcpy(this->_instanceData.data() + offset, data, size);
// }

// void GfxMaterial::create(const json &originData)
// {
//     std::cout<<"create material"<<this<<std::endl;
//     this->_parseRendererState(originData);
//     this->_parseCustomProperties(originData);
//     this->_parseTextures(originData);
//     GfxMgr::getInstance()->createPipeline(this->_rendererState.generateKey(), this->_rendererState);
//     std::cout<<"create pipeline"<<std::endl;
// }
// void GfxMaterial::_parseRendererState(const json &originData)
// {
//     std::cout<<"parse renderer state json:"<<originData.dump()<<std::endl;
//     int renderer = originData["renderer"].get<int>();
//     std::cout<<"parse renderer state11111"<<std::endl;
//     this->_rendererState.renderer = renderer;
//     this->_rendererState.vert = originData["vert"].get<std::string>();
//     this->_rendererState.frag = originData["frag"].get<std::string>();
//     this->_rendererState.geom = "nosupport";
//     // 多边形模式
//     if (originData.contains("polygonMode"))
//     {
//         this->_rendererState.polygonMode = originData["polygonMode"];
//     }
//     // 剔除模式
//     if (originData.contains("cullMode"))
//     {
//         this->_rendererState.cullMode = originData["cullMode"];
//     }
//     // 深度测试
//     if (originData.contains("depthTest"))
//     {
//         this->_rendererState.depthTest = originData["depthTest"];
//     }
//     // 深度写入
//     if (originData.contains("depthWrite"))
//     {
//         this->_rendererState.depthWrite = originData["depthWrite"];
//     }
//     std::cout<<"parse renderer state22222"<<std::endl;
//     // 深度比较操作
//     if (originData.contains("depthCompareOp"))
//     {
//         this->_rendererState.depthCompareOp = originData["depthCompareOp"];
//     }
//     // 模板测试
//     if (originData.contains("stencilTest"))
//     {
//         this->_rendererState.stencilTest = originData["stencilTest"];
//     }

//     // 正面三角形（逆时针）的Stencil操作
//     if (originData.contains("stencilFrontCompareOp"))
//     {
//         this->_rendererState.stencilFrontCompareOp = originData["stencilFrontCompareOp"];
//     }
//     // 正面三角形（逆时针）的Stencil操作
//     if (originData.contains("stencilFrontFailOp"))
//     {
//         this->_rendererState.stencilFrontFailOp = originData["stencilFrontFailOp"];
//     }
//     // 正面三角形（逆时针）的Stencil操作
//     if (originData.contains("stencilFrontDepthFailOp"))
//     {
//         this->_rendererState.stencilFrontDepthFailOp = originData["stencilFrontDepthFailOp"];
//     }
//     // 正面三角形（逆时针）的Stencil操作
//     if (originData.contains("stencilFrontPassOp"))
//     {
//         this->_rendererState.stencilFrontPassOp = originData["stencilFrontPassOp"];
//     }
//     std::cout<<"parse renderer state33333"<<std::endl;
//     // // 正面三角形（逆时针）的Stencil操作
//     // if(originData.contains("stencilFrontCompareMask")){
//     //     this->_rendererState.stencilFrontCompareMask = originData["stencilFrontCompareMask"];
//     // }else{
//     //     this->_rendererState.stencilFrontCompareMask = 0xFFFFFFFF;
//     // }
//     // // 正面三角形（逆时针）的Stencil操作
//     // if(originData.contains("stencilFrontWriteMask")){
//     //     this->_rendererState.stencilFrontWriteMask = originData["stencilFrontWriteMask"];
//     // }else{
//     //     this->_rendererState.stencilFrontWriteMask = 0xFFFFFFFF;
//     // }
//     // // 正面三角形（逆时针）的Stencil操作
//     // if(originData.contains("stencilFrontRreference")){
//     //     this->_rendererState.stencilFrontRreference = originData["stencilFrontRreference"];
//     // }else{
//     //     this->_rendererState.stencilFrontRreference = 0;
//     // }
//     // 反面三角形（顺时针）的Stencil操作
//     if (originData.contains("stencilBackCompareOp"))
//     {
//         this->_rendererState.stencilBackCompareOp = originData["stencilBackCompareOp"];
//     }
//     // 反面三角形（顺时针）的Stencil操作
//     if (originData.contains("stencilBackFailOp"))
//     {
//         this->_rendererState.stencilBackFailOp = originData["stencilBackFailOp"];
//     }
//     // 反面三角形（顺时针）的Stencil操作
//     if (originData.contains("stencilBackDepthFailOp"))
//     {
//         this->_rendererState.stencilBackDepthFailOp = originData["stencilBackDepthFailOp"];
//     }
//     // 正面三角形（逆时针）的Stencil操作
//     if (originData.contains("stencilBackPassOp"))
//     {
//         this->_rendererState.stencilBackPassOp = originData["stencilBackPassOp"];
//     }
//     // // 反面三角形（顺时针）的Stencil操作
//     // if(originData.contains("stencilBackCompareMask")){
//     //     this->_rendererState.stencilBackCompareMask = originData["stencilBackCompareMask"];
//     // }else{
//     //     this->_rendererState.stencilBackCompareMask = 0xFFFFFFFF;
//     // }
//     // // 反面三角形（顺时针）的Stencil操作
//     // if(originData.contains("stencilBackWriteMask")){
//     //     this->_rendererState.stencilBackWriteMask = originData["stencilBackWriteMask"];
//     // }else{
//     //     this->_rendererState.stencilBackWriteMask = 0xFFFFFFFF;
//     // }
//     // // 反面三角形（顺时针）的Stencil操作
//     // if(originData.contains("stencilBackRreference")){
//     //     this->_rendererState.stencilBackRreference = originData["stencilBackRreference"];
//     // }else{
//     //     this->_rendererState.stencilBackRreference = 0;
//     // }

//     // 颜色缓和
//     // 开关
//     if (originData.contains("colorBlend"))
//     {
//         this->_rendererState.colorBlend = originData["colorBlend"];
//     }
//     // 源颜色混合因子
//     if (originData.contains("srcColorBlendFactor"))
//     {
//         this->_rendererState.srcColorBlendFactor = originData["srcColorBlendFactor"];
//     }
//     // 目标颜色混合因子
//     if (originData.contains("dstColorBlendFactor"))
//     {
//         this->_rendererState.dstColorBlendFactor = originData["dstColorBlendFactor"];
//     }
//     // 颜色混合操作
//     if (originData.contains("colorBlendOp"))
//     {
//         this->_rendererState.colorBlendOp = originData["colorBlendOp"];
//     }
//     // 源alpha混合因子
//     if (originData.contains("srcAlphaBlendFactor"))
//     {
//         this->_rendererState.srcAlphaBlendFactor = originData["srcAlphaBlendFactor"];
//     }
//     // 目标alpha混合因子
//     if (originData.contains("dstAlphaBlendFactor"))
//     {
//         this->_rendererState.dstAlphaBlendFactor = originData["dstAlphaBlendFactor"];
//     }
//     // alpha混合操作
//     if (originData.contains("alphaBlendOp"))
//     {
//         this->_rendererState.alphaBlendOp = originData["alphaBlendOp"];
//     }
//     std::cout<<"parse renderer state11111"<<std::endl;
// }
// void GfxMaterial::_parseCustomProperties(const json &originData)
// {
//     // std::cout<<"parse custom properties"<<std::endl;
//     // int renderer = originData["renderer"].get<int>();
//     // size_t reservedSize = 0;
//     // if (renderer == (int)GfxRendererCategory::_UI)
//     // {
//     //     reservedSize = 16 * sizeof(float) + 5 * sizeof(int) + 5 * 4 * sizeof(float);
//     //     this->_instanceData.resize(reservedSize);
//     //     // 16个float(矩阵)，5个int，5个vec4数组
//     //     // 初始化所有字节为0
//     //     // memset 是 C/C++ 中的一个内存设置函数，用于将一块内存区域的所有字节设置为指定的值。
//     //     memset(this->_instanceData.data(), 0, reservedSize);
//     // }
//     // else if (renderer == (int)GfxRendererCategory::_3D)
//     // {
//     //     // 16个float(矩阵)，10个int，10个vec4数组
//     //     reservedSize = 16 * sizeof(float) + 10 * sizeof(int) + 10 * 4 * sizeof(float);
//     //     this->_instanceData.resize(reservedSize);
//     //     // 初始化所有字节为0
//     //     memset(this->_instanceData.data(), 0, reservedSize);
//     // }
//     // size_t currentOffset = 0;
//     // json &properties = originData["properties"];
//     // // 添加模型矩阵
//     // float modelMatrix[16] = {
//     //     1.0f, 0.0f, 0.0f, 0.0f,
//     //     0.0f, 1.0f, 0.0f, 0.0f,
//     //     0.0f, 0.0f, 1.0f, 0.0f,
//     //     0.0f, 0.0f, 0.0f, 1.0f};
//     // this->_updateBytes(currentOffset, modelMatrix, sizeof(modelMatrix));
//     // currentOffset += sizeof(modelMatrix);
//     // if (properties.is_array())
//     // {
//     //     for (int i = 0; i < properties.size(); i++)
//     //     {
//     //         json &param = properties[i];
//     //         std::string name = param["name"];
//     //         std::string type = param["type"];
//     //         if (type == "int")
//     //         {
//     //             int value = param["value"];
//     //             this->_updateBytes(currentOffset, &value, sizeof(int));
//     //             GfxMaterialPropertyBlock block = {currentOffset, sizeof(int), type};
//     //             this->_blocks[name] = block;
//     //             currentOffset += sizeof(int);
//     //         }
//     //         else if (type == "float")
//     //         {
//     //             float value = param["value"];
//     //             this->_updateBytes(currentOffset, &value, sizeof(float));
//     //             GfxMaterialPropertyBlock block = {currentOffset, sizeof(float), type};
//     //             this->_blocks[name] = block;
//     //             currentOffset += sizeof(float);
//     //         }
//     //         else if (type == "vec2")
//     //         {
//     //             float x = param["value"][0];
//     //             float y = param["value"][1];
//     //             float value[2] = {x, y};
//     //             this->_updateBytes(currentOffset, &value, sizeof(value));
//     //             GfxMaterialPropertyBlock block = {currentOffset, sizeof(value), type};
//     //             this->_blocks[name] = block;
//     //             currentOffset += sizeof(value);
//     //         }
//     //         else if (type == "vec3")
//     //         {
//     //             float x = param["value"][0];
//     //             float y = param["value"][1];
//     //             float z = param["value"][2];
//     //             float value[3] = {x, y, z};
//     //             this->_updateBytes(currentOffset, &value, sizeof(value));
//     //             GfxMaterialPropertyBlock block = {currentOffset, sizeof(value), type};
//     //             this->_blocks[name] = block;
//     //             currentOffset += sizeof(value);
//     //         }
//     //         else if (type == "vec4")
//     //         {
//     //             float x = param["value"][0];
//     //             float y = param["value"][1];
//     //             float z = param["value"][2];
//     //             float w = param["value"][3];
//     //             float value[4] = {x, y, z, w};
//     //             this->_updateBytes(currentOffset, &value, sizeof(value));
//     //             GfxMaterialPropertyBlock block = {currentOffset, sizeof(value), type};
//     //             this->_blocks[name] = block;
//     //             currentOffset += sizeof(value);
//     //         }
//     //     }
//     // }
// }
// void GfxMaterial::_parseTextures(const json &originData)
// {
//     int renderer = originData["renderer"].get<int>();
//     if (renderer == (int)GfxRendererCategory::_UI)
//     {
//         this->_textures.resize(4);
//     }
//     else if (renderer == (int)GfxRendererCategory::_3D)
//     {
//         this->_textures.resize(10);
//     }
//     // json &textures = originData["textures"];
//     // if (textures.is_object())
//     // {
//     //     for (auto &texture : textures.items()) // items() 返回键值对迭代器
//     //     {
//     //         std::string key = texture.key(); // ✅ 正确
//     //         json &data = texture.value();    // ✅ 正确
//     //         int binding = data["binding"];
//     //         std::string path = data["path"];
//     //         if (binding > 1 && binding <= this->_textures.size())
//     //         {
//     //             this->_textures[binding - 1] = path;
//     //         }
//     //     }
//     // }
// }

// void GfxMaterial::updateModelMatrix(const std::array<float, 16> &modelMatrix)
// {
//     //  std::cout << "updateModelMatrix()" << std::endl;
//     // std::cout << "Interpret as model matrix:" << std::endl;
//     // const float* floatData1 = reinterpret_cast<const float*>(this->_instanceData.data());
//     // size_t floatCount1 = this->_instanceData.size() / sizeof(float);

//     // for (size_t i = 0; i < floatCount1; ++i) {
//     //     std::cout << "float[" << i << "] = " << floatData1[i] << std::endl;
//     // }
//     // std::cout<<"bbbbbb"<<std::endl;

//     memcpy(this->_instanceData.data(), modelMatrix.data(), sizeof(modelMatrix));

//     // const float* floatData = reinterpret_cast<const float*>(this->_instanceData.data());
//     // size_t floatCount = this->_instanceData.size() / sizeof(float);

//     // for (size_t i = 0; i < floatCount; ++i) {
//     //     std::cout << "float[" << i << "] = " << floatData[i] << std::endl;
//     // }
//     // std::cout<<"ccccc"<<std::endl;
// }
// void GfxMaterial::updatePropertyInt(const std::string &key, const int value)
// {
//     std::cout<<"updatePropertyInt() key: "<<key<<", value: "<<value<<std::endl;
//     if (this->_blocks.find(key) == this->_blocks.end())
//     {
//         return;
//     }
//     GfxMaterialPropertyBlock block = this->_blocks[key];
//     if (block.type != "int")
//     {
//         return;
//     }
//     memcpy(this->_instanceData.data() + block.offset, &value, sizeof(int));
// }
// void GfxMaterial::updatePropertyFloat(const std::string &key, const float value)
// {
//     std::cout<<"updatePropertyFloat() key: "<<key<<", value: "<<value<<std::endl;
//     if (this->_blocks.find(key) == this->_blocks.end())
//     {
//         return;
//     }
//     GfxMaterialPropertyBlock block = this->_blocks[key];
//     if (block.type != "float")
//     {
//         return;
//     }
//     memcpy(this->_instanceData.data() + block.offset, &value, sizeof(float));
// }
// void GfxMaterial::updatePropertyVec2(const std::string &key, const float x, const float y)
// {
//     std::cout<<"updatePropertyVec2() key: "<<key<<", x: "<<x<<", y: "<<y<<std::endl;
//     if (this->_blocks.find(key) == this->_blocks.end())
//     {
//         return;
//     }
//     GfxMaterialPropertyBlock block = this->_blocks[key];
//     if (block.type != "vec2")
//     {
//         return;
//     }
//     memcpy(this->_instanceData.data() + block.offset, &x, sizeof(float));
//     memcpy(this->_instanceData.data() + block.offset + sizeof(float), &y, sizeof(float));
// }
// void GfxMaterial::updatePropertyVec3(const std::string &key, const float x, const float y, const float z)
// {
//     std::cout<<"updatePropertyVec3() key: "<<key<<", x: "<<x<<", y: "<<y<<", z: "<<z<<std::endl;
//     if (this->_blocks.find(key) == this->_blocks.end())
//     {
//         return;
//     }
//     GfxMaterialPropertyBlock block = this->_blocks[key];
//     if (block.type != "vec3")
//     {
//         return;
//     }
//     memcpy(this->_instanceData.data() + block.offset, &x, sizeof(float));
//     memcpy(this->_instanceData.data() + block.offset + sizeof(float), &y, sizeof(float));
//     memcpy(this->_instanceData.data() + block.offset + sizeof(float) * 2, &z, sizeof(float));
// }
// void GfxMaterial::updatePropertyVec4(const std::string &key, const float x, const float y, const float z, const float w)
// {
//     if (this->_blocks.find(key) == this->_blocks.end())
//     {
//         std::cout << "updatePropertyVec4() key: " << key << " not found" << std::endl;
//         return;
//     }
//     GfxMaterialPropertyBlock block = this->_blocks[key];
//     if (block.type != "vec4")
//     {
//         std::cout << "updatePropertyVec4() key: " << key << " type: " << block.type << " not vec4" << std::endl;
//         return;
//     }
//     memcpy(this->_instanceData.data() + block.offset, &x, sizeof(float));
//     memcpy(this->_instanceData.data() + block.offset + sizeof(float), &y, sizeof(float));
//     memcpy(this->_instanceData.data() + block.offset + sizeof(float) * 2, &z, sizeof(float));
//     memcpy(this->_instanceData.data() + block.offset + sizeof(float) * 3, &w, sizeof(float));
//     // std::cout << "updatePropertyVec4() key: " << key << ", x: " << x << ", y: " << y << ", z: " << z << ", w: " << w << std::endl;
//     // std::cout << "Interpret as float:" << std::endl;
//     // const float* floatData = reinterpret_cast<const float*>(this->_instanceData.data());
//     // size_t floatCount = this->_instanceData.size() / sizeof(float);

//     // for (size_t i = 0; i < floatCount; ++i) {
//     //     std::cout << "float[" << i << "] = " << floatData[i] << std::endl;
//     // }
//     // std::cout<<"aaaaaaa"<<std::endl;
// }
// void GfxMaterial::updateTexture(const int binding, const std::string &texture)
// {
//     if (binding < 1 || binding > this->_textures.size())
//     {
//         return;
//     }
//     if (texture.empty())
//     {
//         return;
//     }
//     this->_textures[binding - 1] = texture;
// }

// const std::vector<char> &GfxMaterial::getInstanceData() const
// {
//     return this->_instanceData;
// }
// GfxRendererCategory GfxMaterial::getRendererState() const
// {
//     return (GfxRendererCategory)this->_rendererState.renderer;
// }

GfxMaterial::~GfxMaterial()
{
}