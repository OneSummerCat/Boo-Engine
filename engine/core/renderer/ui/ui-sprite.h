#pragma once
#include <string>
#include "ui-renderer.h"
#include "../../component/component-register.h"
#include "../../math/math-api.h"

namespace Boo
{
    class Node;
    class Camera;
    class MaterialAsset;
    class TextureAsset;
    class MeshAsset;

    enum class SizeMode
    {
        /**
         * @brief 图片的原始尺寸精灵的大小与节点的大小相同
         */
        Raw,
        /**
         * @brief 自定义精灵的大小
         */
        Custom,
    };

    class UISprite : public UIRenderer
    {
    private:
        /**
         * @brief 精灵的材质
         */
        MaterialAsset *_materialAsset;
        /**
         * @brief 精灵的纹理
         */
        TextureAsset *_textureAsset;
        /**
         * @brief 精灵的网格
         */
        MeshAsset *_meshAsset;
        Color _color;
        SizeMode sizeMode;

        void _createDefaultMaterial();
        void _createDefaultMesh();

    public:
        UISprite(std::string name, Node *node, std::string uuid = "");

        void Awake() override;
        void Enable() override;

        /**
         * @brief 设置渲染器的颜色
         *
         * @param color
         */
        void setColor(Color &color);
        void setColor(std::string color);
        void setAlpha(float alpha);
        void setColor(float r, float g, float b, float a);
        void setSizeMode(SizeMode sizeMode);

        /**
         * @brief 设置渲染器的纹理
         *
         * @param texture
         */
        void setTexture(std::string texture);
        void setTexture(TextureAsset *texture);

        /**
         * @brief 设置渲染器的材质
         *
         * @param material
         */
        void setMaterial(std::string material);
        void setMaterial(MaterialAsset *material);

        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void Render(Camera *camera) override;
        void Disable() override;
        void destroy() override;
        ~UISprite() override;
    };
    REGISTER_COMPONENT(UISprite, "UI Sprite Component")
} // namespace Boo
