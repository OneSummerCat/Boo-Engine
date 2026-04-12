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
#include "../../component/component.h"
#include "../../component/component-register.h"
#include "../../math/math-api.h"
#include "../../assets/gltf-asset.h"

namespace Boo
{
    class Node;
    class MaterialAsset;
    class TextureAsset;
    class Node3D;
    class MeshAsset;
    class MeshRenderer;

    class GLTFComponent : public Component
    {
    private:
        Node3D *_node3D;
        GLTFAsset *_gltfAsset;

        void _parseGLTFAsset(GLTFAsset *_gltfAsset);
        void _parseGLTFNode(Node3D *root, const GLTFNode &gltfNode);
        void _parseGLTFMeshRenderer(Node3D *node3D, const std::string &meshUuid);
        MeshRenderer *_getMeshRenderer(const std::string &meshNodePath);

    public:
        GLTFComponent(std::string name, Node *node, std::string uuid = "");
        void Awake() override;
        void Enable() override;
        void setGLTFAsset(std::string gltfAssetPath);
        void setGLTFAsset(GLTFAsset *gltfAsset);
        void setMaterial(std::string meshNodePath, int meshIndex, std::string materialPath);
        void setMaterial(std::string meshNodePath, int meshIndex, MaterialAsset *material);
        /**
         * @brief 设置材质纹理
         * @param meshNodePath 网格节点路径
         * @param texturePath 纹理路径
         * @param key 纹理键
         * @param texture 纹理资产
         * 默认设置0号纹理
         */
        void setTexture(std::string meshNodePath, std::string texturePath);
        void setTexture(std::string meshNodePath, TextureAsset *texture);
        void setTexture(std::string meshNodePath, std::string key, std::string texturePath);
        void setTexture(std::string meshNodePath, std::string key, TextureAsset *texture);
        /**
         * @brief 设置材质纹理
         * @param meshNodePath 网格节点路径
         * @param texturePath 纹理路径
         * @param key 纹理键
         * @param texture 纹理资产
         */
        void setTexture(std::string meshNodePath, int mtlIndex, std::string texturePath);
        void setTexture(std::string meshNodePath, int mtlIndex, TextureAsset *texture);
        void setTexture(std::string meshNodePath, int mtlIndex, std::string key, std::string texturePath);
        void setTexture(std::string meshNodePath, int mtlIndex, std::string key, TextureAsset *texture);
        
        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void Disable() override;
        void destroy() override;
        ~GLTFComponent() override;
    };
    REGISTER_COMPONENT(GLTFComponent, "GLTF Component")
} // namespace Boo