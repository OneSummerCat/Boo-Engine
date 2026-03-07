#pragma once
#include <string>
#include "ui-renderer.h"
#include "../../component/component-register.h"

namespace Boo
{
    class Node;
    class Camera;
    class TextureAsset;
    class MaterialAsset;

    class UISprite : public UIRenderer
    {
    private:

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
        void setColor(float r, float g, float b, float a);
        /**
         * @brief 设置渲染器的透明度
         *
         * @param alpha
         */
        void setAlpha(float alpha);
        /**
         * @brief 设置渲染器的纹理
         *
         * @param texture
         */
        void setTexture(std::string texture);
        void setTexture(TextureAsset *texture);
        /**
         * @brief 设置渲染器的渲染目标
         *
         * @param renderTexture
         */
        void setRenderTexture();

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
