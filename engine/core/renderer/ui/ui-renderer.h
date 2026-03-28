#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <array>
#include <cstdint>

#include "../../math/math-api.h"
#include "../../component/component.h"

namespace Boo
{

    class Node;
    class Camera;
    class MaterialAsset;
    class TextureAsset;
    class Node2D;

    class UIRenderer : public Component
    {
    private:
    protected:
        Node2D *_node2D;
        MaterialAsset *_materialAsset;
        Color _color;
        Mat4 _uiViewMatrix;
        /**
         * @brief 设置渲染器的材质
         *
         */
        void _setMaterial(MaterialAsset *mtl);
        /**
         * @brief 设置渲染器的颜色
         *
         */
        void _setColor(float r, float g, float b, float a);

    public:
        UIRenderer(std::string name, Node *node, std::string uuid = "");

        void Awake() override;
        const Color &getColor();
        void Enable() override;
        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        /**
         * @brief 组件渲染函数
         * 当前节点组件以及子节点组件的lateUpdate执行结束,render渲染函数开始
         */
        virtual void Render(Camera *camera);
        void Disable() override;
        void destroy() override;

        virtual ~UIRenderer() override;
    };

} // namespace Boo
