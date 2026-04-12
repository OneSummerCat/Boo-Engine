#pragma once
#include "node.h"

namespace Boo
{
    class MeshRenderer;

    class Node3D : public Node
    {
    private:
        MeshRenderer *_meshRenderer;

    protected:
        void _updateWorldTransform() override;

    public:
        Node3D(const std::string name, const std::string uuid = "");
        void setPosition(float x, float y, float z) override;
        void setWorldPosition(float x, float y, float z) override;
        void setScale(float x, float y, float z) override;
        void setWorldScale(float x, float y, float z) override;
        void setEulerAngles(float x, float y, float z) override;
        void setRotation(float x, float y, float z, float w) override;
        void setWorldRotation(float x, float y, float z, float w) override;
        void setActive(bool active) override;
        /**
         * 添加组件
         */
        Component *addComponent(std::string name, std::string uuid = "") override;
        /**
         * 获取UI渲染组件
         */
        MeshRenderer *getMeshRenderer();
        /**
		 * 获取世界矩阵的逆转置矩阵
		 */
		const Mat4 &getWorldMatrixIT();
        void update(float deltaTime) override;
        void lateUpdate(float deltaTime) override;
        void clearNodeFrameFlag() override;
        void destroy() override;
        ~Node3D();
    };

} // namespace Boo

#include "node-3d-impl.h"
