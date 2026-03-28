#include "mesh-asset.h"
#include "../../log.h"
#include "../../boo.h"
namespace Boo
{
    MeshAsset::MeshAsset() : Asset()
    {
        this->_type = AssetType::Mesh;
    }
    MeshAsset::MeshAsset(std::string uuid) : Asset(uuid)
    {
        this->_type = AssetType::Mesh;
    }
    MeshAsset::MeshAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name)
    {
        this->_type = AssetType::Mesh;
    }
    void MeshAsset::create(std::vector<MeshPrimitive> primitives)
    {
        this->_primitives = primitives;
        this->_subGfxMeshs.clear();
        int index = 0;
        for (auto &primitive : this->_primitives)
        {
            // 顶点位置
            const std::vector<float> &_positions = primitive._positions;
            // 顶点法线
            const std::vector<float> &_normals = primitive._normals;
            // 顶点纹理坐标
            const std::vector<float> &_uvs = primitive._uvs;
            // 顶点纹理坐标
            const std::vector<float> &_uvs1 = primitive._uvs1;
            // 顶点纹理坐标
            const std::vector<float> &_uvs2 = primitive._uvs2;
            // 顶点颜色
            const std::vector<float> &_colors = primitive._colors;
            // 顶点切线
            const std::vector<float> &_tangents = primitive._tangents;
            // 顶点索引
            const std::vector<int> &_indices = primitive._indices;
            std::string gfxMeshUuid = this->_uuid + "_" + std::to_string(index);
            // std::cout << "MeshAsset: idx=" << index << ", name=" << this->_name << ", indices=" << _indices.size() << std::endl;
            GfxMesh *gfxMesh = GfxMgr::getInstance()->createMesh(gfxMeshUuid, _positions, _normals, _uvs, _uvs1, _uvs2, _colors, _tangents, _indices);
            this->_subGfxMeshs.push_back(gfxMesh);
            index++;
        }
    }
    GfxMesh* MeshAsset::getGfxMesh(int index)
    {
        if (index < 0 || index >= this->_subGfxMeshs.size())
        {
            LOGW("MeshAsset::getGfxMesh: index out of range");
            return nullptr;
        }
        return this->_subGfxMeshs[index];
    }
    const size_t MeshAsset::getSubMeshCount()
    {
        return this->_subGfxMeshs.size();
    }
    void MeshAsset::destroy()
    {
        for (auto &gfxMesh : this->_subGfxMeshs)
        {
            GfxMgr::getInstance()->destroyMesh(gfxMesh);
        }
        this->_subGfxMeshs.clear();
    }
    MeshAsset::~MeshAsset()
    {
    }
} // namespace Boo