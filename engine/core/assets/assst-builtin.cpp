#include "assst-builtin.h"
#include "../../boo.h"
#include "assets-manager.h"
#include "asset-cache.h"
#include "texture-asset.h"
#include "material-asset.h"
#include "../utils/json-util.h"

AssetBuiltin::AssetBuiltin()
{
}
void AssetBuiltin::init()
{
    this->_initDefaultTextures();
    this->_initDefaultMaterials();
}
void AssetBuiltin::_initDefaultTextures()
{
    // 默认纹理
    TextureAsset *texture = new TextureAsset("builtin::default.png");
    texture->create(GfxTextureDefault, sizeof(GfxTextureDefault));
    Boo::game->assetsManager()->getAssetsCache()->addAsset("builtin::default.png", texture);
    
    TextureAsset *logo = new TextureAsset("builtin::logo.png");
    logo->create(GfxTextureLogo, sizeof(GfxTextureLogo));
    Boo::game->assetsManager()->getAssetsCache()->addAsset("builtin::logo.png", logo);
}
void AssetBuiltin::_initDefaultMaterials()
{
    // 默认UI材质
    std::string jsonUI = R"({
        "render": 1,
        "vert": "builtin-ui.vert",
        "frag": "builtin-ui.frag",
        "polygonMode": 0,
        "cullMode": 0,
        "depthTest": 0,
        "depthWrite": 0,
        "depthCompareOp": 7,
        "stencilTest": 1,
        "stencilFrontCompareOp": 5,
        "stencilFrontFailOp": 0,
        "stencilFrontDepthFailOp": 0,
        "stencilFrontPassOp": 0,
        "stencilBackCompareOp": 5,
        "stencilBackFailOp": 0,
        "stencilBackDepthFailOp": 0,
        "stencilBackPassOp": 0,
        "colorBlend": 1,
        "srcColorBlendFactor": 4,
        "dstColorBlendFactor": 6,
        "colorBlendOp": 0,
        "srcAlphaBlendFactor": 0,
        "dstAlphaBlendFactor": 6,
        "alphaBlendOp": 0
    })";
    json uiMaterial = json::parse(jsonUI);
    MaterialAsset *materialUI = new MaterialAsset("builtin::ui.mtl");
    materialUI->create(uiMaterial);
    Boo::game->assetsManager()->getAssetsCache()->addAsset("builtin::ui.mtl", materialUI);

    // 默认UI遮罩-add材质
    std::string jsonMaskAdd = R"({
        "render": 1,
        "vert": "builtin-ui-mask.vert",
        "frag": "builtin-ui-mask.frag",
        "polygonMode": 0,
        "cullMode": 0,
        "depthTest": 0,
        "depthWrite": 0,
        "depthCompareOp": 7,
        "stencilTest": 1,
        "stencilFrontCompareOp": 7,
        "stencilFrontFailOp": 0,
        "stencilFrontDepthFailOp": 0,
        "stencilFrontPassOp": 3,
        "stencilBackCompareOp": 7,
        "stencilBackFailOp": 0,
        "stencilBackDepthFailOp": 0,
        "stencilBackPassOp": 3,
        "colorBlend": 0,
        "srcColorBlendFactor": 0,
        "dstColorBlendFactor": 0,
        "colorBlendOp": 0,
        "srcAlphaBlendFactor": 0,
        "dstAlphaBlendFactor": 0,
        "alphaBlendOp": 0,
        "pushConstant": [],
        "textures": []
    })";
    json uiMaskMaterial = json::parse(jsonMaskAdd);
    MaterialAsset *materialUIMaskAdd = new MaterialAsset("builtin::ui-mask-add.mtl");
    materialUIMaskAdd->create(uiMaskMaterial);
    Boo::game->assetsManager()->getAssetsCache()->addAsset("builtin::ui-mask-add.mtl", materialUIMaskAdd);
    // 默认UI遮罩-sub材质
    std::string jsonMaskSub = R"({
        "render": 1,
        "vert": "builtin-ui-mask.vert",
        "frag": "builtin-ui-mask.frag",
        "polygonMode": 0,
        "cullMode": 0,
        "depthTest": 0,
        "depthWrite": 0,
        "depthCompareOp": 7,
        "stencilTest": 1,
        "stencilFrontCompareOp": 7,
        "stencilFrontFailOp": 0,
        "stencilFrontDepthFailOp": 0,
        "stencilFrontPassOp": 4,
        "stencilBackCompareOp": 7,
        "stencilBackFailOp": 0,
        "stencilBackDepthFailOp": 0,
        "stencilBackPassOp": 4,
        "colorBlend": 0,
        "srcColorBlendFactor": 0,
        "dstColorBlendFactor": 0,
        "colorBlendOp": 0,
        "srcAlphaBlendFactor": 0,
        "dstAlphaBlendFactor": 0,
        "alphaBlendOp": 0,
        "pushConstant": [],
        "textures": []
    })";
    json uiMaskMaterialSub = json::parse(jsonMaskSub);
    MaterialAsset *materialUIMaskSub = new MaterialAsset("builtin::ui-mask-sub.mtl");
    materialUIMaskSub->create(uiMaskMaterialSub);
    Boo::game->assetsManager()->getAssetsCache()->addAsset("builtin::ui-mask-sub.mtl", materialUIMaskSub);
}
AssetBuiltin::~AssetBuiltin()
{
}
