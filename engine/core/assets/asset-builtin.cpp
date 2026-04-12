#include "asset-builtin.h"
#include "../../boo.h"
#include "asset-struct.h"

namespace Boo
{

    AssetBuiltin::AssetBuiltin()
    {
    }
    void AssetBuiltin::init()
    {
        this->_initDefaultTextureLogo();
        this->_initDefaultShaderUI();
        this->_initDefaultShaderUnlit();
        this->_initDefaultMaterialUI();
        this->_initDefaultMaterialUnlit();
    }
    void AssetBuiltin::_initDefaultTextureLogo()
    {
        TextureAsset *texture = new TextureAsset(AssetBuiltinTexture::Default, AssetBuiltinTexture::Default, AssetBuiltinTexture::Default);
        texture->create(GfxTextureDefault, sizeof(GfxTextureDefault));
        assetsManager->getAssetsCache()->addAsset(AssetBuiltinTexture::Default, texture);

        // 默认splash纹理
        TextureAsset *splash = new TextureAsset(AssetBuiltinTexture::Splash, AssetBuiltinTexture::Splash, AssetBuiltinTexture::Splash);
        splash->create(GfxTextureSplash, sizeof(GfxTextureSplash));
        assetsManager->getAssetsCache()->addAsset(AssetBuiltinTexture::Splash, splash);
    }
    void AssetBuiltin::_initDefaultShaderUI()
    {
        ShaderAsset *vert = new ShaderAsset(AssetBuiltinShader::UIVert, AssetBuiltinShader::UIVert, AssetBuiltinShader::UIVert);
        // vert->create(ShaderVertexAsset, std::string(AssetBuiltinShaderUIVert()), {});
        std::vector<uint32_t> vertSPV(std::begin(AssetBuiltinShaderUIVert), std::end(AssetBuiltinShaderUIVert));
        vert->create(vertSPV);
        assetsManager->getAssetsCache()->addAsset(AssetBuiltinShader::UIVert, vert);
        ShaderAsset *frag = new ShaderAsset(AssetBuiltinShader::UIFrag, AssetBuiltinShader::UIFrag, AssetBuiltinShader::UIFrag);
        // frag->create(ShaderFragmentAsset, std::string(AssetBuiltinShaderUIFrag()), {});
        std::vector<uint32_t> fragSPV(std::begin(AssetBuiltinShaderUIFrag), std::end(AssetBuiltinShaderUIFrag));
        frag->create(fragSPV);
        assetsManager->getAssetsCache()->addAsset(AssetBuiltinShader::UIFrag, frag);
    }
    void AssetBuiltin::_initDefaultShaderUnlit()
    {
        ShaderAsset *vert = new ShaderAsset(AssetBuiltinShader::UnlitVert, AssetBuiltinShader::UnlitVert, AssetBuiltinShader::UnlitVert);
        // vert->create(ShaderVertexAsset, std::string(AssetBuiltinShaderUnlitVert()), {});
        std::vector<uint32_t> vertSPV(std::begin(AssetBuiltinShaderUnlitVert), std::end(AssetBuiltinShaderUnlitVert));
        vert->create(vertSPV);
        assetsManager->getAssetsCache()->addAsset(AssetBuiltinShader::UnlitVert, vert);
        ShaderAsset *frag = new ShaderAsset(AssetBuiltinShader::UnlitFrag, AssetBuiltinShader::UnlitFrag, AssetBuiltinShader::UnlitFrag);
        // frag->create(ShaderFragmentAsset, std::string(AssetBuiltinShaderUnlitFrag()), {});
        std::vector<uint32_t> fragSPV(std::begin(AssetBuiltinShaderUnlitFrag), std::end(AssetBuiltinShaderUnlitFrag));
        frag->create(fragSPV);
        assetsManager->getAssetsCache()->addAsset(AssetBuiltinShader::UnlitFrag, frag);
    }
    void AssetBuiltin::_initDefaultMaterialUI()
    {
        MaterialAsset *materialUI = new MaterialAsset(AssetBuiltinMaterial::UI, AssetBuiltinMaterial::UI, AssetBuiltinMaterial::UI);
        json uiMaterial = json::parse(AssetBuiltinMaterialUI);
        materialUI->create(uiMaterial);
        assetsManager->getAssetsCache()->addAsset(AssetBuiltinMaterial::UI, materialUI);
    }
    void AssetBuiltin::_initDefaultMaterialUnlit()
    {
        MaterialAsset *materialUnlit = new MaterialAsset(AssetBuiltinMaterial::Unlit, AssetBuiltinMaterial::Unlit, AssetBuiltinMaterial::Unlit);
        json unlitMaterial = json::parse(AssetBuiltinMaterialUnlit);
        materialUnlit->create(unlitMaterial);
        assetsManager->getAssetsCache()->addAsset(AssetBuiltinMaterial::Unlit, materialUnlit);
    }

} // namespace Boo
//         // 默认UI遮罩-add材质
//         std::string jsonMaskAdd = R"({
//         "render": 0,
//         "vert": "builtin-ui-mask.vert",
//         "frag": "builtin-ui-mask.frag",
//         "polygonMode": 0,
//         "cullMode": 0,
//         "depthTest": 0,
//         "depthWrite": 0,
//         "depthCompareOp": 7,
//         "stencilTest": 1,
//         "stencilFrontCompareOp": 7,
//         "stencilFrontFailOp": 0,
//         "stencilFrontDepthFailOp": 0,
//         "stencilFrontPassOp": 3,
//         "stencilBackCompareOp": 7,
//         "stencilBackFailOp": 0,
//         "stencilBackDepthFailOp": 0,
//         "stencilBackPassOp": 3,
//         "colorBlend": 0,
//         "srcColorBlendFactor": 0,
//         "dstColorBlendFactor": 0,
//         "colorBlendOp": 0,
//         "srcAlphaBlendFactor": 0,
//         "dstAlphaBlendFactor": 0,
//         "alphaBlendOp": 0,
//         "pushConstant": [],
//         "textures": []
//     })";
//         // 默认UI遮罩-sub材质
//         std::string jsonMaskSub = R"({
//         "render": 0,
//         "vert": "builtin-ui-mask.vert",
//         "frag": "builtin-ui-mask.frag",
//         "polygonMode": 0,
//         "cullMode": 0,
//         "depthTest": 0,
//         "depthWrite": 0,
//         "depthCompareOp": 7,
//         "stencilTest": 1,
//         "stencilFrontCompareOp": 7,
//         "stencilFrontFailOp": 0,
//         "stencilFrontDepthFailOp": 0,
//         "stencilFrontPassOp": 4,
//         "stencilBackCompareOp": 7,
//         "stencilBackFailOp": 0,
//         "stencilBackDepthFailOp": 0,
//         "stencilBackPassOp": 4,
//         "colorBlend": 0,
//         "srcColorBlendFactor": 0,
//         "dstColorBlendFactor": 0,
//         "colorBlendOp": 0,
//         "srcAlphaBlendFactor": 0,
//         "dstAlphaBlendFactor": 0,
//         "alphaBlendOp": 0,
//         "pushConstant": [],
//         "textures": []
//     })";