#include "material-asset.h"
#include "texture-asset.h"
#include "../../log.h"
#include "../../boo.h"
#include "../gfx/gfx-mgr.h"

namespace Boo
{
    MaterialAsset::MaterialAsset() : Asset()
    {
        this->_type = AssetType::Material;
        this->_uuid = UuidUtil::generateUUID();
        this->_gfxMaterial = new GfxMaterial();
        this->_originData = json::object();
    }
    MaterialAsset::MaterialAsset(std::string uuid) : Asset(uuid)
    {
        this->_type = AssetType::Material;
        this->_gfxMaterial = new GfxMaterial();
        this->_originData = json::object();
    }
    MaterialAsset::MaterialAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name)
    {
        this->_type = AssetType::Material;
        this->_gfxMaterial = new GfxMaterial();
        this->_originData = json::object();
    }
    void MaterialAsset::create(MaterialAsset *mtl)
    {
        if (mtl == nullptr)
        {
            LOGE("MaterialAsset::create(MaterialAsset *mtl) mtl is nullptr");
            return;
        }
        this->create(mtl->getOriginData());
    }
    void MaterialAsset::create(const json &materialData)
    {
        this->_originData = materialData;
        this->_parse();
    }
    void MaterialAsset::_parse()
    {

        int renderer = this->_originData["renderer"].get<int>();
        if (renderer != int(RendererCategory::_UI) && renderer != int(RendererCategory::_3D))
        {
            LOGE("MaterialAsset::_parse(json &materialData) render is not UI or 3D");
            return;
        }
        std::string vert = this->_originData["vert"].get<std::string>();
        if (vert.empty())
        {
            LOGE("MaterialAsset::_parse(json &materialData) vert is empty");
            return;
        }
        Asset *vertAsset = assetsManager->loadAsset(vert);
        if (vertAsset == nullptr)
        {
            LOGE("MaterialAsset::_parse(json &materialData) vert is not found");
            this->_originData.clear();
            return;
        }
        std::string frag = this->_originData["frag"].get<std::string>();
        if (frag.empty())
        {
            LOGE("MaterialAsset::_parse(json &materialData) frag is empty");
            return;
        }
        Asset *fragAsset = assetsManager->loadAsset(frag);
        if (fragAsset == nullptr)
        {
            LOGE("MaterialAsset::_parse(json &materialData) frag is not found");
            this->_originData.clear();
            return;
        }

        this->_gfxMaterial->create(GfxRendererCategory(renderer), vert, frag);
        this->_parseProperties();
        this->_parseTextures();
        this->_parseRendererState();
    }
    void MaterialAsset::_parseProperties()
    {
        if (!this->_originData.contains("properties"))
        {
            LOGW("MaterialAsset::_parseProperties() properties is not found");
            return;
        }
        this->_properties.clear();
        json &properties = this->_originData["properties"];
        if (properties.is_array())
        {
            for (int i = 0; i < properties.size(); i++)
            {
                json &param = properties[i];
                std::string name = param["name"];
                std::string type = param["type"];
                if (type == "int")
                {
                    int value = param["value"].get<int>();
                    GfxMaterialDataBlock block = {};
                    block.offset = 0;
                    block.size = sizeof(int);
                    block.type = GfxMaterialPropertyType::Int;
                    block.data = &value;
                }
                else if (type == "float")
                {
                    float value = param["value"].get<float>();
                    GfxMaterialDataBlock block = {};
                    block.offset = 0;
                    block.size = sizeof(float);
                    block.type = GfxMaterialPropertyType::Float;
                    block.data = &value;
                }
                else if (type == "vec2")
                {
                    float x = param["value"][0];
                    float y = param["value"][1];
                    float value[2] = {x, y};
                    GfxMaterialDataBlock block = {};
                    block.offset = 0;
                    block.size = sizeof(value);
                    block.type = GfxMaterialPropertyType::Vec2;
                    block.data = value;
                }
                else if (type == "vec3")
                {
                    float x = param["value"][0];
                    float y = param["value"][1];
                    float z = param["value"][2];
                    float value[3] = {x, y, z};
                    GfxMaterialDataBlock block = {};
                    block.offset = 0;
                    block.size = sizeof(value);
                    block.type = GfxMaterialPropertyType::Vec3;
                    block.data = value;
                }
                else if (type == "vec4")
                {
                    float x = param["value"][0];
                    float y = param["value"][1];
                    float z = param["value"][2];
                    float w = param["value"][3];
                    float value[4] = {x, y, z, w};
                    GfxMaterialDataBlock block = {};
                    block.offset = 0;
                    block.size = sizeof(value);
                    block.type = GfxMaterialPropertyType::Vec4;
                    block.data = value;
                }
            }
        }
        this->_gfxMaterial->setProperties(this->_properties);
    }
    void MaterialAsset::_parseTextures()
    {
        if (!this->_originData.contains("textures"))
        {
            return;
        }
        json &textures = this->_originData["textures"];
        if (textures.is_object())
        {
            for (auto &texture : textures.items()) // items() 返回键值对迭代器
            {
                std::string key = texture.key(); // ✅ 正确
                json &data = texture.value();    // ✅ 正确
                if (!data.contains("binding") || !data.contains("path"))
                {
                    LOGW("MaterialAsset::_parseTextures() textures %s is not found binding or path", key.c_str());
                    continue;
                }
                int binding = data["binding"];
                std::string path = data["path"];
                // 加载关联图片资产
                Asset *textureAsset = assetsManager->loadAsset(path);
                if (binding <= 0)
                {
                    LOGW("MaterialAsset::_parseTextures() textures %s binding is invalid", key.c_str());
                    continue;
                }
                MaterialTextureBlock textureBlock = {};
                textureBlock.binding = binding;
                textureBlock.key = key;
                textureBlock.path = path;
                this->_textures[key] = textureBlock;
                this->_gfxMaterial->setTexture(binding - 1, path);
            }
        }
    }
    void MaterialAsset::_parseRendererState()
    {
        // 多边形模式
        if (this->_originData.contains("polygonMode"))
        {
            GfxRendererStatePolygonMode polygonMode = (GfxRendererStatePolygonMode)this->_originData["polygonMode"].get<int>();
            this->_gfxMaterial->setPolygonMode(polygonMode);
        }
        // 剔除模式
        if (this->_originData.contains("cullMode"))
        {
            GfxRendererStateCullMode cullMode = (GfxRendererStateCullMode)this->_originData["cullMode"].get<int>();
            this->_gfxMaterial->setCullMode(cullMode);
        }
        // 深度测试
        if (this->_originData.contains("depthTest"))
        {
            this->_gfxMaterial->setDepthTest(this->_originData["depthTest"].get<int>());
        }
        // 深度写入
        if (this->_originData.contains("depthWrite"))
        {
            this->_gfxMaterial->setDepthWrite(this->_originData["depthWrite"].get<int>());
        }
        // 深度比较操作
        if (this->_originData.contains("depthCompareOp"))
        {
            GfxRendererStateCompareOp compareOp = (GfxRendererStateCompareOp)this->_originData["depthCompareOp"].get<int>();
            this->_gfxMaterial->setDepthCompareOp(compareOp);
        }
        // 模板测试
        if (this->_originData.contains("stencilTest"))
        {
            this->_gfxMaterial->setStencilTest(this->_originData["stencilTest"].get<int>());
        }
        // 正面三角形（逆时针）的Stencil操作
        if (this->_originData.contains("stencilFrontCompareOp"))
        {
            GfxRendererStateCompareOp compareOp = (GfxRendererStateCompareOp)this->_originData["stencilFrontCompareOp"].get<int>();
            this->_gfxMaterial->setStencilFrontCompareOp(compareOp);
        }
        // 正面三角形（逆时针）的Stencil操作
        if (this->_originData.contains("stencilFrontFailOp"))
        {
            GfxRendererStateStencilOp failOp = (GfxRendererStateStencilOp)this->_originData["stencilFrontFailOp"].get<int>();
            this->_gfxMaterial->setStencilFrontFailOp(failOp);
        }
        // 正面三角形（逆时针）的Stencil操作
        if (this->_originData.contains("stencilFrontDepthFailOp"))
        {
            GfxRendererStateStencilOp depthFailOp = (GfxRendererStateStencilOp)this->_originData["stencilFrontDepthFailOp"].get<int>();
            this->_gfxMaterial->setStencilFrontDepthFailOp(depthFailOp);
        }
        // 正面三角形（逆时针）的Stencil操作
        if (this->_originData.contains("stencilFrontPassOp"))
        {
            GfxRendererStateStencilOp passOp = (GfxRendererStateStencilOp)this->_originData["stencilFrontPassOp"].get<int>();
            this->_gfxMaterial->setStencilFrontPassOp(passOp);
        }
        // // 正面三角形（逆时针）的Stencil操作
        // if(originData.contains("stencilFrontCompareMask")){
        //     this->_rendererState.stencilFrontCompareMask = originData["stencilFrontCompareMask"];
        // }else{
        //     this->_rendererState.stencilFrontCompareMask = 0xFFFFFFFF;
        // }
        // // 正面三角形（逆时针）的Stencil操作
        // if(originData.contains("stencilFrontWriteMask")){
        //     this->_rendererState.stencilFrontWriteMask = originData["stencilFrontWriteMask"];
        // }else{
        //     this->_rendererState.stencilFrontWriteMask = 0xFFFFFFFF;
        // }
        // // 正面三角形（逆时针）的Stencil操作
        // if(originData.contains("stencilFrontRreference")){
        //     this->_rendererState.stencilFrontRreference = originData["stencilFrontRreference"];
        // }else{
        //     this->_rendererState.stencilFrontRreference = 0;
        // }
        // 反面三角形（顺时针）的Stencil操作
        if (this->_originData.contains("stencilBackCompareOp"))
        {
            GfxRendererStateCompareOp compareOp = (GfxRendererStateCompareOp)this->_originData["stencilBackCompareOp"].get<int>();
            this->_gfxMaterial->setStencilBackCompareOp(compareOp);
        }
        // 反面三角形（顺时针）的Stencil操作
        if (this->_originData.contains("stencilBackFailOp"))
        {
            GfxRendererStateStencilOp failOp = (GfxRendererStateStencilOp)this->_originData["stencilBackFailOp"].get<int>();
            this->_gfxMaterial->setStencilBackFailOp(failOp);
        }
        // 反面三角形（顺时针）的Stencil操作
        if (this->_originData.contains("stencilBackDepthFailOp"))
        {
            GfxRendererStateStencilOp depthFailOp = (GfxRendererStateStencilOp)this->_originData["stencilBackDepthFailOp"].get<int>();
            this->_gfxMaterial->setStencilBackDepthFailOp(depthFailOp);
        }
        // 正面三角形（逆时针）的Stencil操作
        if (this->_originData.contains("stencilBackPassOp"))
        {
            GfxRendererStateStencilOp passOp = (GfxRendererStateStencilOp)this->_originData["stencilBackPassOp"].get<int>();
            this->_gfxMaterial->setStencilBackPassOp(passOp);
        }
        // 反面三角形（顺时针）的Stencil操作
        // if(originData.contains("stencilBackCompareMask")){
        //     this->_rendererState.stencilBackCompareMask = originData["stencilBackCompareMask"];
        // }else{
        //     this->_rendererState.stencilBackCompareMask = 0xFFFFFFFF;
        // }
        // // 反面三角形（顺时针）的Stencil操作
        // if(originData.contains("stencilBackWriteMask")){
        //     this->_rendererState.stencilBackWriteMask = originData["stencilBackWriteMask"];
        // }else{
        //     this->_rendererState.stencilBackWriteMask = 0xFFFFFFFF;
        // }
        // // 反面三角形（顺时针）的Stencil操作
        // if(originData.contains("stencilBackRreference")){
        //     this->_rendererState.stencilBackRreference = originData["stencilBackRreference"];
        // }else{
        //     this->_rendererState.stencilBackRreference = 0;
        // }

        // 颜色缓和
        // 开关
        if (this->_originData.contains("colorBlend"))
        {
            this->_gfxMaterial->setColorBlend((int)this->_originData["colorBlend"]);
        }
        // 源颜色混合因子
        if (this->_originData.contains("srcColorBlendFactor"))
        {
            GfxRendererStateColorBlendFactor srcColorBlendFactor = (GfxRendererStateColorBlendFactor)this->_originData["srcColorBlendFactor"].get<int>();
            this->_gfxMaterial->setSrcColorBlendFactor(srcColorBlendFactor);
        }
        // 目标颜色混合因子
        if (this->_originData.contains("dstColorBlendFactor"))
        {
            GfxRendererStateColorBlendFactor dstColorBlendFactor = (GfxRendererStateColorBlendFactor)this->_originData["dstColorBlendFactor"].get<int>();
            this->_gfxMaterial->setDstColorBlendFactor(dstColorBlendFactor);
        }
        // 颜色混合操作
        if (this->_originData.contains("colorBlendOp"))
        {
            GfxRendererStateColorBlendOp colorBlendOp = (GfxRendererStateColorBlendOp)this->_originData["colorBlendOp"].get<int>();
            this->_gfxMaterial->setColorBlendOp(colorBlendOp);
        }
        // 源alpha混合因子
        if (this->_originData.contains("srcAlphaBlendFactor"))
        {
            GfxRendererStateColorBlendFactor srcAlphaBlendFactor = (GfxRendererStateColorBlendFactor)this->_originData["srcAlphaBlendFactor"].get<int>();
            this->_gfxMaterial->setSrcAlphaBlendFactor(srcAlphaBlendFactor);
        }
        // 目标alpha混合因子
        if (this->_originData.contains("dstAlphaBlendFactor"))
        {
            GfxRendererStateColorBlendFactor dstAlphaBlendFactor = (GfxRendererStateColorBlendFactor)this->_originData["dstAlphaBlendFactor"].get<int>();
            this->_gfxMaterial->setDstAlphaBlendFactor(dstAlphaBlendFactor);
        }
        // alpha混合操作
        if (this->_originData.contains("alphaBlendOp"))
        {
            GfxRendererStateColorBlendOp alphaBlendOp = (GfxRendererStateColorBlendOp)this->_originData["alphaBlendOp"].get<int>();
            this->_gfxMaterial->setAlphaBlendOp(alphaBlendOp);
        }
    }

    void MaterialAsset::setModelWorldMatrix(const std::array<float, 16> &modelMatrix)
    {
        this->_gfxMaterial->setModelWorldMatrix(modelMatrix);
    }
    void MaterialAsset::setUIColor(float r, float g, float b, float w)
    {
        this->_gfxMaterial->setUIColor(r, g, b, w);
    }
    void MaterialAsset::setTexture(const std::string &key, TextureAsset *texture)
    {
        if (texture == nullptr)
        {
            LOGE("MaterialAsset::setTexture(const std::string &key, TextureAsset *texture) texture is nullptr");
            return;
        }
        if (this->_textures.find(key) == this->_textures.end())
        {
            LOGW("MaterialAsset::setTexture(const std::string &key, TextureAsset *texture) key %s is not found", key.c_str());
            return;
        }
        MaterialTextureBlock &textureBlock = this->_textures[key];
        int binding = textureBlock.binding;
        this->_gfxMaterial->setTexture(binding, texture->getUuid());
    }

    const json &MaterialAsset::getOriginData()
    {
        return this->_originData;
    }
    GfxMaterial *MaterialAsset::getGfxMaterial()
    {
        return this->_gfxMaterial;
    }
    void MaterialAsset::destroy()
    {
    }

} // namespace Boo