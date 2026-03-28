#include "gltf-asset.h"
#include "../../log.h"

namespace Boo
{
    GLTFAsset::GLTFAsset()
    {
        this->_type = AssetType::GLTF;
        this->_meshAssets.clear();
    }
    GLTFAsset::GLTFAsset(std::string uuid) : Asset(uuid)
    {
        this->_type = AssetType::GLTF;
        this->_meshAssets.clear();
    }
    GLTFAsset::GLTFAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name)
    {
        this->_type = AssetType::GLTF;
        this->_meshAssets.clear();
    }
    void GLTFAsset::create(std::unique_ptr<fastgltf::Asset> m_asset)
    {
        this->m_asset = std::move(m_asset);
        this->_parseScenes();
    }
    void GLTFAsset::_parseScenes()
    {
        // 检查是否有场景
        if (m_asset->scenes.empty())
        {
            LOGW("No scenes in asset");
            return;
        }
        // 解析场景
        // 为每个场景加载数据
        for (size_t i = 0; i < m_asset->scenes.size(); ++i)
        {
            const auto &scene = m_asset->scenes[i];
            std::string sceneName = std::string(scene.name);
            GLTFNode root{};
            root.name = sceneName;
            root.local = Mat4::identity();
            root.meshUuid = "";
            for (size_t nodeIdx : scene.nodeIndices)
            {
                this->_parseNode(root, nodeIdx);
            }
            this->_nodes.push_back(std::move(root));
        }
    }
    void GLTFAsset::_parseNode(GLTFNode &parent, size_t nodeIdx)
    {
        if (nodeIdx >= m_asset->nodes.size())
        {
            return;
        }
        const fastgltf::Node &node = m_asset->nodes[nodeIdx];
        GLTFNode nodeData{};
        nodeData.name = std::string(node.name);
        nodeData.local = Mat4::identity();
        nodeData.meshUuid = "";
        fastgltf::math::fmat4x4 matrix = fastgltf::getTransformMatrix(node);
        this->_fmatToMat4(nodeData.local, matrix);
        this->_parseMeshes(node, nodeData);
        parent.children.push_back(nodeData);
        for (size_t childIdx : node.children)
        {
            this->_parseNode(nodeData, childIdx);
        }
    }
    /**
     * @brief 解析网格
     *
     * @param meshIndex 网格索引
     * @param nodeData 节点数据
     */
    void GLTFAsset::_parseMeshes(const fastgltf::Node &node, GLTFNode &nodeData)
    {
        const size_t meshIndex = *node.meshIndex;
        if (meshIndex >= m_asset->meshes.size())
        {
            nodeData.meshUuid = "";
            return;
        }
        if (!node.meshIndex.has_value())
        {
            nodeData.meshUuid = "";
            return;
        }
        const fastgltf::Mesh &mesh = m_asset->meshes[meshIndex];
        const std::string meshName = std::string(mesh.name);
        // 解析所有子网格数据
        std::vector<MeshPrimitive> primitives;
        int index = 0;
        for (const auto &primitive : mesh.primitives)
        {
            MeshPrimitive primitiveData{};
            this->_parsePrimitive(index, primitive, primitiveData);
            if (primitiveData._indices.empty())
            {
                continue;
            }
            index++;
            primitives.push_back(primitiveData);
        }
        // std::cout << "Mesh: idx=" << meshIndex << ", name=" << meshName << ", primitives=" << index << std::endl;
        // 创建网格资产
        std::string meshUuid = this->_uuid + "_" + meshName + ":" + std::to_string(meshIndex);
        nodeData.meshUuid = meshUuid;
        MeshAsset *meshAsset = new MeshAsset(meshUuid, "", meshName);
        meshAsset->create(primitives);
        // std::cout << "MeshAsset: idx=" << meshIndex << ", name=" << meshName << ", primitives=" << index << std::endl;
        this->_meshAssets.push_back(meshAsset);

    }
    /**
     * @brief 解析子网格
     *
     * @param primitive 子网格数据
     * @param primitiveData 子网格数据
     */
    void GLTFAsset::_parsePrimitive(int index, const fastgltf::Primitive &primitive, MeshPrimitive &primitiveData)
    {
        primitiveData.index = index;
        auto *positionAttribute = primitive.findAttribute("POSITION");
        if (positionAttribute == primitive.attributes.end())
        {
            LOGW("No position attribute in primitive");
            return;
        }
        primitiveData._positions.clear();
        primitiveData._normals.clear();
        primitiveData._uvs.clear();
        primitiveData._uvs1.clear();
        primitiveData._uvs2.clear();
        primitiveData._colors.clear();
        primitiveData._tangents.clear();
        primitiveData._indices.clear();
        // 1. 读取位置数据
        const auto &positionAccessor = m_asset->accessors[positionAttribute->accessorIndex];
        auto positionIterable = fastgltf::iterateAccessor<fastgltf::math::fvec3>(
            *m_asset,
            positionAccessor);

        for (const auto &position : positionIterable)
        {
            // 3个顶点float坐标
            primitiveData._positions.push_back(position.x());
            primitiveData._positions.push_back(position.y());
            primitiveData._positions.push_back(position.z());
            // 3个法线float坐标
            primitiveData._normals.push_back(0.0f);
            primitiveData._normals.push_back(0.0f);
            primitiveData._normals.push_back(1.0f);
            // 2个uv坐标坐标
            primitiveData._uvs.push_back(0.0f);
            primitiveData._uvs.push_back(0.0f);
            // 2个uv坐标坐标
            primitiveData._uvs1.push_back(0.0f);
            primitiveData._uvs1.push_back(0.0f);
            // 2个uv坐标坐标
            primitiveData._uvs2.push_back(0.0f);
            primitiveData._uvs2.push_back(0.0f);
            // 4个颜色float坐标
            primitiveData._colors.push_back(1.0f);
            primitiveData._colors.push_back(1.0f);
            primitiveData._colors.push_back(1.0f);
            primitiveData._colors.push_back(1.0f);
            // 4个切线float坐标
            primitiveData._tangents.push_back(0.0f);
            primitiveData._tangents.push_back(0.0f);
            primitiveData._tangents.push_back(0.0f);
            primitiveData._tangents.push_back(0.0f);
        }

        // 2. 读取法线数据（如果存在）
        auto normIter = primitive.findAttribute("NORMAL");
        if (normIter != primitive.attributes.end())
        {
            const auto &normAccessor = m_asset->accessors[normIter->accessorIndex];
            auto normals = fastgltf::iterateAccessor<fastgltf::math::fvec3>(
                *m_asset, normAccessor);

            size_t nIdx = 0;
            for (auto norm : normals)
            {
                primitiveData._normals[nIdx * 3 + 0] = norm.x();
                primitiveData._normals[nIdx * 3 + 1] = norm.y();
                primitiveData._normals[nIdx * 3 + 2] = norm.z();
                nIdx++;
            }
        }
        // 3. 读取纹理坐标（如果存在）
        auto texIter = primitive.findAttribute("TEXCOORD_0");
        if (texIter != primitive.attributes.end())
        {
            const auto &texAccessor = m_asset->accessors[texIter->accessorIndex];
            auto texCoords = fastgltf::iterateAccessor<fastgltf::math::fvec2>(
                *m_asset, texAccessor);

            size_t tIdx = 0;
            for (auto tex : texCoords)
            {
                primitiveData._uvs[tIdx * 2 + 0] = tex.x();
                primitiveData._uvs[tIdx * 2 + 1] = tex.y();
                tIdx++;
            }
        }
        // 4. 读取切线数据（如果存在）
        auto tanIter = primitive.findAttribute("TANGENT");
        if (tanIter != primitive.attributes.end())
        {
            const auto &tanAccessor = m_asset->accessors[tanIter->accessorIndex];
            auto tangents = fastgltf::iterateAccessor<fastgltf::math::fvec4>(
                *m_asset, tanAccessor);

            size_t tanIdx = 0;
            for (auto tan : tangents)
            {
                primitiveData._tangents[tanIdx * 4 + 0] = tan.x();
                primitiveData._tangents[tanIdx * 4 + 1] = tan.y();
                primitiveData._tangents[tanIdx * 4 + 2] = tan.z();
                primitiveData._tangents[tanIdx * 4 + 3] = tan.w(); // w 分量不变
                tanIdx++;
            }
        }
        // 6. 读取索引数据（重要！）
        if (primitive.indicesAccessor.has_value())
        {
            size_t idxAccessorIdx = primitive.indicesAccessor.value();
            if (idxAccessorIdx < m_asset->accessors.size())
            {
                const auto &idxAccessor = m_asset->accessors[idxAccessorIdx];

                switch (idxAccessor.componentType)
                {
                case fastgltf::ComponentType::UnsignedByte:
                {
                    auto indices = fastgltf::iterateAccessor<uint8_t>(*m_asset, idxAccessor);
                    for (auto index : indices)
                    {
                        primitiveData._indices.push_back(index);
                    }
                    break;
                }
                case fastgltf::ComponentType::UnsignedShort:
                {
                    auto indices = fastgltf::iterateAccessor<uint16_t>(*m_asset, idxAccessor);
                    for (auto index : indices)
                    {
                        primitiveData._indices.push_back(index);
                    }
                    break;
                }
                case fastgltf::ComponentType::UnsignedInt:
                {
                    auto indices = fastgltf::iterateAccessor<uint32_t>(*m_asset, idxAccessor);
                    for (auto index : indices)
                    {
                        primitiveData._indices.push_back(index);
                    }
                    break;
                }
                default:
                    LOGW("Unsupported index type");
                    break;
                }
            }
        }
        else
        {
            // 没有索引数据，生成顺序索引（三角形列表）
            size_t vertexCount = positionAccessor.count;
            for (size_t i = 0; i < vertexCount; i += 3)
            {
                primitiveData._indices.push_back(i);
                primitiveData._indices.push_back(i + 1);
                primitiveData._indices.push_back(i + 2);
            }
        }
    }

    void GLTFAsset::_fmatToMat4(Mat4 &mat, const fastgltf::math::fmat4x4 &fmat)
    {
        std::array<float, 16> result;
        for (int i = 0; i < 4; ++i)
        {
            result[i * 4 + 0] = fmat[i][0];
            result[i * 4 + 1] = fmat[i][1];
            result[i * 4 + 2] = fmat[i][2];
            result[i * 4 + 3] = fmat[i][3];
        }
        mat.set(result);
    }
    const std::vector<MeshAsset*> &GLTFAsset::getMeshAssets()
    {
        return this->_meshAssets;
    }
    void GLTFAsset::destroy()
    {
    }
    GLTFAsset::~GLTFAsset()
    {
    }
}
