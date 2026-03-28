#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "../gfx/base/gfx-material.h"
namespace Boo
{
    enum class RendererCategory
    {
        _UI = 0,
        _3D = 1,
    };
    enum class RendererType
    {
        _Opaque = 0,
        _Transparent = 1,
    };
    class TextureAsset;
    struct MaterialTextureBlock
    {
        int binding;
        std::string key;
        std::string path;
    };
    class MaterialAsset : public Asset
    {
    private:
        json _originData;
        GfxMaterial *_gfxMaterial;
        std::vector<GfxMaterialDataBlock> _properties;
        std::map<std::string, MaterialTextureBlock> _textures;
        void _parse();
        void _parseProperties();
        void _parseTextures();
        void _parseRendererState();
    public:
        MaterialAsset();
        MaterialAsset(std::string uuid);
        MaterialAsset(std::string uuid, std::string path, std::string name);

        void create(const json &materialData);
        void create(MaterialAsset *mtl);
        const json &getOriginData();

        /**
         * @brief 更新模型矩阵
         * @param modelMatrix 模型矩阵
         */
        void setModelWorldMatrix(const std::array<float, 16> &modelMatrix);
        void setUIColor(float r, float g, float b, float w);
        void setTexture(const std::string &key, TextureAsset *texture);
        
        /**
         * @brief 获取图形材质
         * @return GfxMaterial* 图形材质
         */
        GfxMaterial *getGfxMaterial();
        /**
         * @brief 创建图形资源
         * 参数 宏定义
         */
        virtual void destroy() override;
        ~MaterialAsset() {}
    };

} // namespace Boo
