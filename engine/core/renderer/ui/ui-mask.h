#pragma once
#include <string>
#include <iostream>
#include "ui-renderer.h"
#include "../../component/component-register.h"
#include "../../scene/node.h"
class GfxMesh;
namespace Boo
{
    class Node;
    class Camera;
    class TextureAsset;
    class MaterialAsset;

    /**
     * mask 组件继承与ui-renderer组件
     * 后续优化
     */

    class UIMask : public UIRenderer
    {
    private:
        MaterialAsset *_addMaterialAsset;
        MaterialAsset *_subMaterialAsset;
        GfxMesh *_maskMesh;

        void _updateNodeMask();

    public:
        UIMask(std::string name, Node *node, std::string uuid = "");
        void Awake() override;
        void Enable() override;

        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void Render(Camera *camera) override;
        void lateRender(Camera *camera);
        void Disable() override;
        void destroy() override;
        ~UIMask() override;
    };
    REGISTER_COMPONENT(UIMask, "UI Mask Component")
} // namespace Boo
