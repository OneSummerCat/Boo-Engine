#pragma once
#include <string>
#include <vector>
#include "asset.h"
#include "../gfx/gfx-mgr.h"
#include "../gfx/base/gfx-mesh.h"

namespace Boo
{
    enum class MeshMode
    {
        Static,
        Dynamic,
    };
   
    struct MeshPrimitive
    {
        /**
         * @brief 索引
         */
        int index;
        /**
         * @brief 类型
         * 0:ui网格 1:模型网格
         */
        int type;
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
        std::vector<uint32_t> _indices;
        /**
         * @brief 网格
         */
        GfxMesh *_gfxMesh;
    };
    /**
     * @brief 网格资产
     */
    class MeshAsset : public Asset
    {
    private:
        MeshMode _meshMode;
        std::vector<MeshPrimitive> _primitives;

        void _createMesh(MeshMode meshMode, std::vector<MeshPrimitive> &primitives);
        void _prepareVertexData(MeshPrimitive &primitive,std::vector<float> &vertexData);
    public:
        MeshAsset();
        MeshAsset(std::string uuid);
        MeshAsset(std::string uuid, std::string path, std::string name);
        /**
         * @brief 创建网格
         *
         * @param primitives 网格原语
         */
        void create(std::vector<MeshPrimitive> primitives);
        /**
         * @brief 创建动态网格
         *
         * @param primitives 网格原语
         */
        void createDynamic(std::vector<MeshPrimitive> primitives);
        /**
         * @brief 更新动态网格
         *
         * @param index 索引
         * @param data 网格原语
         */
        void updateDynamicSubMesh(int index, MeshPrimitive &data);
        const size_t getSubMeshCount();
        GfxMesh *getGfxMesh(int index);
        void destroy();
        ~MeshAsset();
    };

} // namespace Boo
