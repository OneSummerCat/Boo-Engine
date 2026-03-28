#include "mesh-renderer.h"
#include "../../../boo.h"
#include "../../../log.h"
#include "../../assets/mesh-asset.h"
#include "../../assets/material-asset.h"

namespace Boo
{
    MeshRenderer::MeshRenderer(std::string name, Node *node, std::string uuid) : Component(name, node, uuid),
                                                                                 _meshAsset(nullptr),
                                                                                 _materials({})
    {
        this->_layer = ComponentLayer::Node3D;
        this->_node3D = dynamic_cast<Node3D *>(node);
    }

    void MeshRenderer::Awake()
    {
        Component::Awake();
        this->_meshAsset = nullptr;
        this->_materials.clear();
        this->_materials.resize(1);
    }
    void MeshRenderer::Enable()
    {
        Component::Enable();
    }
    void MeshRenderer::setMeshAsset(MeshAsset *meshAsset)
    {
        if (this->_meshAsset != nullptr)
        {
            LOGW("MeshRenderer::setMeshAsset: meshAsset is not nullptr");
            return;
        }
        // std::cout << "meshAsset name: " << meshAsset->getName() << std::endl;
        this->_meshAsset = meshAsset;
        this->_materials.resize(meshAsset->getSubMeshCount());
    }
    void MeshRenderer::setMaterialAsset(std::string materialName)
    {
        Asset *asset = assetsManager->getAssetsCache()->getAsset(materialName);
        MaterialAsset *materialAsset = dynamic_cast<MaterialAsset *>(asset);
        if (materialAsset == nullptr)
        {
            LOGW("MeshRenderer::setMaterialAsset: materialAsset is nullptr");
            return;
        }
        this->setMaterialAsset(0, materialAsset);
    }
    void MeshRenderer::setMaterialAsset(MaterialAsset *materialAsset)
    {
        this->setMaterialAsset(0, materialAsset);
    }
    void MeshRenderer::setMaterialAsset(int index, std::string materialName)
    {
        Asset *asset = assetsManager->getAssetsCache()->getAsset(materialName);
        MaterialAsset *materialAsset = dynamic_cast<MaterialAsset *>(asset);
        if (materialAsset == nullptr)
        {
            LOGW("MeshRenderer::setMaterialAsset: materialAsset is nullptr");
            return;
        }
        this->setMaterialAsset(index, materialAsset);
    }
    void MeshRenderer::setMaterialAsset(int index, MaterialAsset *materialAsset)
    {
        if (this->_meshAsset == nullptr)
        {
            LOGW("MeshRenderer::setMaterialAsset: meshAsset is nullptr");
            return;
        }
        if (index < 0 || index >= this->_materials.size())
        {
            LOGW("MeshRenderer::setMaterialAsset: index out of range");
            return;
        }
        this->_materials[index] = materialAsset;
    }
    void MeshRenderer::Update(float deltaTime)
    {
        Component::Update(deltaTime);
    }
    void MeshRenderer::LateUpdate(float deltaTime)
    {
        Component::LateUpdate(deltaTime);
    }
    /**
     * @brief 组件渲染函数
     * 当前节点组件以及子节点组件的lateUpdate执行结束,render渲染函数开始
     */
    void MeshRenderer::Render(Camera *camera)
    {
        const Mat4 &matrix = this->_node3D->getWorldMatrix();
        // 更新世界矩阵
        this->_materials[0]->setModelWorldMatrix(matrix.data());
        // std::cout << "MeshRenderer Render: "  << std::endl;

        GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), this->_materials[0]->getGfxMaterial(), this->_meshAsset->getGfxMesh(0));
        // 增加渲染物体数量
        profiler->addObjectCount(1);
    }
    void MeshRenderer::Disable()
    {
        Component::Disable();
    }
    void MeshRenderer::destroy()
    {
        Component::destroy();
    }
    MeshRenderer::~MeshRenderer()
    {
    }
} // namespace Boo
