#include "profiler.h"

#include <sstream>
#include <iomanip>

#include "../../boo.h"
#include "../assets/assets-manager.h"
#include "../gfx/gfx.h"
#include "../gfx/gfx-mgr.h"
#include "../gfx/base/gfx-render-texture.h"

namespace Boo
{
    Profiler::Profiler()
    {
    }
    void Profiler::init()
    {
        this->_uuid = UuidUtil::generateUUID();
        this->_width = 200;
        this->_height = 130;
        this->_matView = Mat4::identity();
        this->_matProj = Mat4::identity();
        // 渲染性能分析窗口
        GfxMgr::getInstance()->createRenderQueue(this->_uuid, 999999, this->_width * 5, this->_height * 5);
        this->_createDefaultMeshAsset();
        this->_initBg();
        this->_initFps();
        this->_initRenderTime();
        this->_initRenderCount();
        this->_initObjectCount();
    }
    void Profiler::_createDefaultMeshAsset()
    {
        this->_defaultMeshAsset = new MeshAsset();
        MeshPrimitive primitive;
        primitive.index = 0;
        primitive.type = 0;
        primitive._positions = {-0.5f, 0.5f, 0.0f,
                                -0.5f, -0.5f, 0.0f,
                                0.5f, -0.5f, 0.0f,
                                0.5f, 0.5f, 0.0f};
        primitive._uvs = {0.0f, 0.0f,
                          0.0f, 1.0f,
                          1.0f, 1.0f,
                          1.0f, 0.0f};
        primitive._indices = {0, 1, 2, 0, 2, 3};
        this->_defaultMeshAsset->create({primitive});
    }

    void Profiler::_initBg()
    {
        this->_bgMltAsset = this->_createMaterialAsset();
        this->_bgMat = Mat4::identity();
        this->_bgMat.setM0(this->_width);
        this->_bgMat.setM5(this->_height);
        this->_bgMat.setM15(1.0f);
    }
    void Profiler::_initFps()
    {
        // FPS
        this->_fpsMltAsset = this->_createMaterialAsset();
        this->_fpsNumMltAsset = this->_createMaterialAsset();
        this->_fpsMeshAsset = this->_createMeshAsset();
        this->_fpsMeshNumAsset = this->_createMeshAsset();
        int width, height;
        this->_setText(this->_fpsTitle, this->_fpsMeshAsset, this->_fpsMltAsset, width, height);
        this->_setFontTransform(5, 5, width, height, 25, this->_fpsMat);
    }
    void Profiler::_initRenderTime()
    {
        this->_renderTitleMeshAsset = this->_createMeshAsset();
        this->_renderTimeMeshAsset = this->_createMeshAsset();
        this->_renderTitleMltAsset = this->_createMaterialAsset();
        this->_renderTimeMltAsset = this->_createMaterialAsset();
        this->_renderTitleMat = Mat4::identity();
        this->_renderTimeMat = Mat4::identity();
        int width, height;
        this->_setText(this->_renderTimeTitle, this->_renderTitleMeshAsset, this->_renderTitleMltAsset, width, height);
        this->_setFontTransform(5, 35, width, height, 25, this->_renderTitleMat);
    }
    void Profiler::_initRenderCount()
    {
        this->_renderCountTitleMeshAsset = this->_createMeshAsset();
        this->_renderCountMeshAsset = this->_createMeshAsset();
        this->_renderCountTitleMltAsset = this->_createMaterialAsset();
        this->_renderCountMltAsset = this->_createMaterialAsset();
        this->_renderCountMat = Mat4::identity();
        int width, height;
        this->_setText(this->_renderCountTitle, this->_renderCountTitleMeshAsset, this->_renderCountTitleMltAsset, width, height);
        this->_setFontTransform(5, 65, width, height, 25, this->_renderCountTitleMat);
    }
    void Profiler::_initObjectCount()
    {
        this->_objectCountTitleMltAsset = this->_createMaterialAsset();
        this->_objectCountTitleMeshAsset = this->_createMeshAsset();
        this->_objectCountMeshAsset = this->_createMeshAsset();
        this->_objectCountMltAsset = this->_createMaterialAsset();
        this->_objectCountMat = Mat4::identity();
        int width, height;
        this->_setText(this->_objectCountTitle, this->_objectCountTitleMeshAsset, this->_objectCountTitleMltAsset, width, height);
        this->_setFontTransform(5, 95, width, height, 25, this->_objectCountTitleMat);
    }

    void Profiler::openDebug()
    {
        this->_isShowDebug = true;
    }
    void Profiler::closeDebug()
    {
        this->_isShowDebug = false;
    }
    void Profiler::addObjectCount(int count)
    {
        this->_objectCount += count;
    }
    void Profiler::render()
    {
        if (!this->_isShowDebug)
        {
            return;
        }
        this->_matView.identity();
        float scaleX = this->_width / (float)view->getWidth();
        float scaleY = this->_height / (float)view->getHeight();
        float posX = (-view->getWidth() * 0.5f + this->_width * 0.5f * 0.7f) * scaleX;
        float posY = (view->getHeight() * 0.5f - this->_height * 0.5f * 0.7f) * scaleY;
        this->_matView.setM0(scaleX * 0.7f);
        this->_matView.setM5(scaleY * 0.7f);
        this->_matView.setM11(1);
        this->_matView.setM12(posX);
        this->_matView.setM13(posY);
        // 投影矩阵
        float left = -(float)this->_width / 2.0f;
        float right = (float)this->_width / 2.0f;
        float top = (float)this->_height / 2.0f;
        float bottom = -(float)this->_height / 2.0f;
        Mat4::ortho(this->_matProj, left, right, bottom, top, -1000.0f, 1000.0f, -1.0f, 0);
        std::array<float, 4> cameraPosition = {0.0f, 0.0f, 0.0f, 0.0f};
        GfxMgr::getInstance()->submitRenderData(this->_uuid, this->_matView.data(), this->_matProj.data(), true, cameraPosition);
        // 渲染背景
        this->_refreshBg();
        // 渲染FPS
        this->_refreshFps();
        // 上一帧渲染时间
        this->_refreshRenderTime();
        // 上一帧渲染提交次数
        this->_refreshRenderCount();
        // 当前帧渲染物体数量
        this->_refreshObjectCount();

        this->_objectCount = 0;
    }
    void Profiler::_refreshBg()
    {
        // 渲染背景
        this->_submitRenderObject(this->_bgMltAsset, nullptr, this->_bgMat, 1.0, 1.0, 1.0, 0.05);
    }
    void Profiler::_refreshFps()
    {
        // 渲染FPS 标题
        this->_submitRenderObject(this->_fpsMltAsset, this->_fpsMeshAsset, this->_fpsMat, 1.0f, 0.0f, 0.0f, 1.0);
        // 渲染FPS 数值
        if (this->_fpsNum != game->getFps())
        {
            this->_fpsNum = game->getFps();
            int width, height;
            this->_setText(std::to_string(this->_fpsNum), this->_fpsMeshNumAsset, this->_fpsNumMltAsset, width, height);
            this->_setFontTransform(50, 5, width, height, 25, this->_fpsNumMat);
        }
        this->_submitRenderObject(this->_fpsNumMltAsset, this->_fpsMeshNumAsset, this->_fpsNumMat, 1.0f, 0.0f, 0.0f, 1.0);
    }
    void Profiler::_refreshRenderTime()
    {
        this->_submitRenderObject(this->_renderTitleMltAsset, this->_renderTitleMeshAsset, this->_renderTitleMat, 1.0f, 0.0f, 0.0f, 1.0);
        // 渲染时间
        int width, height;
        std::ostringstream oss;
        // oss << std::fixed << std::setprecision(2) << Gfx::_submitTime;
        oss << std::fixed << std::setprecision(2) << Gfx::_frameTime;
        // std::cout << std::to_string(Gfx::_submitTime) << std::endl;
        std::string result = oss.str() + "ms";
        this->_setText(result, this->_renderTimeMeshAsset, this->_renderTimeMltAsset, width, height);
        this->_setFontTransform(120, 35, width, height, 25, this->_renderTimeMat);
        this->_submitRenderObject(this->_renderTimeMltAsset, this->_renderTimeMeshAsset, this->_renderTimeMat, 1.0f, 0.0f, 0.0f, 1.0);
    }
    void Profiler::_refreshRenderCount()
    {
        this->_submitRenderObject(this->_renderCountTitleMltAsset, this->_renderCountTitleMeshAsset, this->_renderCountTitleMat, 1.0f, 0.0f, 0.0f, 1.0);
        // 渲染提交次数
        int width, height;
        // std::ostringstream oss;
        // oss << std::fixed << std::setprecision(2) << Gfx::_drawCount;
        // std::string result = oss.str();
        this->_setText(std::to_string(Gfx::_drawCount), this->_renderCountMeshAsset, this->_renderCountMltAsset, width, height);
        this->_setFontTransform(130, 65, width, height, 25, this->_renderCountMat);
        this->_submitRenderObject(this->_renderCountMltAsset, this->_renderCountMeshAsset, this->_renderCountMat, 1.0f, 0.0f, 0.0f, 1.0);
    }
    void Profiler::_refreshObjectCount()
    {
        this->_submitRenderObject(this->_objectCountTitleMltAsset, this->_objectCountTitleMeshAsset, this->_objectCountTitleMat, 1.0f, 0.0f, 0.0f, 1.0);
        // 渲染物体数量
        int width, height;
        this->_setText(std::to_string(this->_objectCount), this->_objectCountMeshAsset, this->_objectCountMltAsset, width, height);
        this->_setFontTransform(170, 95, width, height, 25, this->_objectCountMat);
        this->_submitRenderObject(this->_objectCountMltAsset, this->_objectCountMeshAsset, this->_objectCountMat, 1.0f, 0.0f, 0.0f, 1.0);
    }

    MeshAsset *Profiler::_createMeshAsset()
    {
        MeshAsset *meshAsset = new MeshAsset(UuidUtil::generateUUID());
        MeshPrimitive primitive;
        primitive.index = 0;
        primitive.type = 0;
        // primitive._positions = {-0.5f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f};
        // primitive._uvs = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        // primitive._indices = {0, 1, 2, 0, 2, 3};
        std::vector<float> positions = {};
        std::vector<float> uvs = {};
        std::vector<uint32_t> indices = {};
        uint32_t MAX_SIZE = 50; // 最大字符数
        for (int i = 0; i < MAX_SIZE; i++)
        {
            // 每个字符两个三角形,6个顶点
            // 第一个三角形
            // 顶点0 左上角
            positions.push_back(-0.5f);
            positions.push_back(0.5f);
            positions.push_back(0.0f);
            uvs.push_back(0.0f);
            uvs.push_back(0.0f);
            // 顶点1 左下角
            positions.push_back(-0.5f);
            positions.push_back(-0.5f);
            positions.push_back(0.0f);
            uvs.push_back(0.0f);
            uvs.push_back(1.0f);
            // 顶点2 右下角
            positions.push_back(0.5f);
            positions.push_back(-0.5f);
            positions.push_back(0.0f);
            uvs.push_back(1.0f);
            uvs.push_back(1.0f);
            // 第二个三角形
            // 顶点3 左上角
            positions.push_back(-0.5f);
            positions.push_back(0.5f);
            positions.push_back(0.0f);
            uvs.push_back(0.0f);
            uvs.push_back(0.0f);
            // 顶点4 右下角
            positions.push_back(0.5f);
            positions.push_back(-0.5f);
            positions.push_back(0.0f);
            uvs.push_back(1.0f);
            uvs.push_back(1.0f);
            // 顶点5 右上角
            positions.push_back(0.5f);
            positions.push_back(0.5f);
            positions.push_back(0.0f);
            uvs.push_back(1.0f);
            uvs.push_back(0.0f);
        }
        uint32_t vertexCount = positions.size() / 3;
        for (int i = 0; i < vertexCount; i++)
        {
            indices.push_back(i * 3);
            indices.push_back(i * 3 + 1);
            indices.push_back(i * 3 + 2);
        }
        primitive._positions = positions;
        primitive._uvs = uvs;
        primitive._indices = indices;
        meshAsset->createDynamic({primitive});
        return meshAsset;
    }
    MaterialAsset *Profiler::_createMaterialAsset()
    {
        MaterialAsset *materialAsset = new MaterialAsset(UuidUtil::generateUUID());
        MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetsManager->getAsset(AssetBuiltinMaterial::UI));
        materialAsset->create(mtl);
        return materialAsset;
    }
    void Profiler::_setText(std::string text, MeshAsset *meshAsset, MaterialAsset *materialAsset, int &width, int &height)
    {
        if (meshAsset == nullptr || materialAsset == nullptr || text.empty())
        {
            return;
        }
        TextLayoutResult result = FreetypeMgr::getInstance()->create(text, 50);
        MeshPrimitive primitive;
        primitive._positions = result.batches[0].positions;
        primitive._uvs = result.batches[0].uvs;
        primitive._indices = result.batches[0].indices;
        meshAsset->updateDynamicSubMesh(0, primitive);
        materialAsset->setTexture(result.batches[0].texture);
        width = result.width;
        height = result.height;
    }
    void Profiler::_setFontTransform(int x, int y, int width, int height, int fontSize, Mat4 &mat)
    {
        mat.identity();
        float rate = fontSize / 50.f;
        float _width = width * rate;
        float _height = height * rate;
        float _x = -this->_width * 0.5f + _width * 0.5f + x;
        float _y = this->_height * 0.5f - _height * 0.5f - y;

        mat.setM0(_width);
        mat.setM5(_height);
        mat.setM11(1);
        mat.setM12(_x);
        mat.setM13(_y);
    }
    void Profiler::_submitRenderObject(MaterialAsset *materialAsset, MeshAsset *meshAsset, Mat4 &mat, float colorR, float colorG, float colorB, float colorA)
    {
        if (materialAsset == nullptr)
        {
            return;
        }
        materialAsset->setModelWorldMatrix(mat.data());
        materialAsset->setUIColor(colorR, colorG, colorB, colorA);
        if (meshAsset == nullptr)
        {
            GfxMgr::getInstance()->submitRenderObject(this->_uuid, materialAsset->getGfxMaterial(), this->_defaultMeshAsset->getGfxMesh(0));
        }
        else
        {
            GfxMgr::getInstance()->submitRenderObject(this->_uuid, materialAsset->getGfxMaterial(), meshAsset->getGfxMesh(0));
        }
    }
} // namespace Boo
