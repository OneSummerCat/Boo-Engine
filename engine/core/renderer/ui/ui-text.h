#pragma once
#include <string>
#include <iostream>
#include "ui-renderer.h"
#include "../../font/freetype-mgr.h"
#include "../../component/component-register.h"
#include "../../math/math-api.h"

namespace Boo
{

    class MaterialAsset;
    class TextureAsset;
    class MeshAsset;
    class Camera;
    class Size;
    class MeshAsset;

    class UIText : public UIRenderer
    {

    private:
        std::string _text;
        int _fontSize;
        int _lineHeight;
        Color _color;
        MeshAsset *_meshAsset;
        MaterialAsset *_materialAsset;

        void _createDefaultMaterial();
        void _createDefaultMesh();

    private:
        int _realWidth;
        int _realHeight;
        int _fontRealSize;

        void _updateNodeSize();

    public:
        UIText(std::string name, Node *node, std::string uuid = "");

        void Awake() override;
        void Enable() override;

        void setText(std::string text);
        void setSize(int fontSize);
        void setLineHeight(int lineHeight);

        void setColor(Color &color);
        void setColor(std::string color);
        void setColor(float r, float g, float b, float a);
        void setAlpha(float alpha);
        void setMaterial(std::string material);
        void setMaterial(MaterialAsset *material);

        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void Render(Camera *camera) override;
        void Disable() override;
        void destroy() override;
        ~UIText() override;
    };
    // 注册宏调用
    REGISTER_COMPONENT(UIText, "UI Text Component")
} // namespace Boo
