#pragma once
#include <string>
#include <vector>
#include "asset.h"
#include "../gfx/gfx-mgr.h"
#include "../gfx/base/gfx-mesh.h"

namespace Boo
{
    struct MeshPrimitive
    {
        int index;
        /**
         * @brief 顶点位置
         */
        std::vector<float> _positions;
        /**
         * @brief 顶点法线
         */
        std::vector<float> _normals;
        /**
         * @brief 顶点纹理坐标
         */
        std::vector<float> _uvs;
        /**
         * @brief 顶点纹理坐标
         */
        std::vector<float> _uvs1;
        /**
         * @brief 顶点纹理坐标
         */
        std::vector<float> _uvs2;
        /**
         * @brief 顶点颜色
         */
        std::vector<float> _colors;
        /**
         * @brief 顶点切线
         */
        std::vector<float> _tangents;
        /**
         * @brief 顶点索引
         */
        std::vector<int> _indices;
    };
    /**
     * @brief 网格资产
     */
    class MeshAsset : public Asset
    {
        std::vector<MeshPrimitive> _primitives;
        std::vector<GfxMesh*> _subGfxMeshs;

    public:
        MeshAsset();
        MeshAsset(std::string uuid);
        MeshAsset(std::string uuid, std::string path, std::string name);
        void create(std::vector<MeshPrimitive> primitives);
        const size_t getSubMeshCount();
        GfxMesh* getGfxMesh(int index);
        void destroy();
        ~MeshAsset();
    };

} // namespace Boo
