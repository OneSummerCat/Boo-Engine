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
    }
    void MeshRenderer::Enable()
    {
        Component::Enable();
    }
    void MeshRenderer::setMesh(MeshAsset *meshAsset)
    {
        if (meshAsset == nullptr)
        {
            LOGW("MeshRenderer::setMeshAsset: meshAsset is nullptr");
            return;
        }
        this->_meshAsset = meshAsset;
        this->_clearMaterials();
        this->_resetMaterials();
    }
    void MeshRenderer::setMaterial(std::string materialName)
    {
        Asset *asset = assetsManager->getAsset(materialName, true);
        MaterialAsset *materialAsset = dynamic_cast<MaterialAsset *>(asset);
        if (materialAsset == nullptr)
        {
            LOGW("MeshRenderer::setMaterialAsset: materialAsset is nullptr");
            return;
        }
        this->setMaterial(0, materialAsset);
    }
    void MeshRenderer::setMaterial(MaterialAsset *materialAsset)
    {
        this->setMaterial(0, materialAsset);
    }
    void MeshRenderer::setMaterial(int index, std::string materialName)
    {
        Asset *asset = assetsManager->getAsset(materialName, true);
        MaterialAsset *materialAsset = dynamic_cast<MaterialAsset *>(asset);
        if (materialAsset == nullptr)
        {
            LOGW("MeshRenderer::setMaterialAsset: materialAsset is nullptr");
            return;
        }
        this->setMaterial(index, materialAsset);
    }
    void MeshRenderer::setMaterial(int index, MaterialAsset *materialAsset)
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
        this->_materials[index]->create(materialAsset);
    }
    std::vector<MaterialAsset *> MeshRenderer::getMaterials()
    {
        return this->_materials;
    }
    MaterialAsset *MeshRenderer::getMaterial()
    {
        return this->_materials[0];
    }
    MaterialAsset *MeshRenderer::getMaterial(int index)
    {
        if (index < 0 || index >= this->_materials.size())
        {
            LOGW("MeshRenderer::getMaterial: index out of range");
            return nullptr;
        }
        return this->_materials[index];
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
        if (camera == nullptr)
        {
            return;
        }
        if (this->_meshAsset == nullptr)
        {
            LOGW("MeshRenderer::Render: meshAsset is nullptr");
            return;
        }
        if (this->_materials.empty())
        {
            LOGW("MeshRenderer::Render: materials is empty");
            return;
        }
        const Mat4 &matrix = this->_node3D->getWorldMatrix();
        const Mat4 &worldIT = this->_node3D->getWorldMatrixIT();
        size_t subMeshCount = this->_meshAsset->getSubMeshCount();
        for (size_t i = 0; i < subMeshCount; i++)
        {
            GfxMesh *gfxMesh = this->_meshAsset->getGfxMesh(i);
            if (gfxMesh == nullptr)
            {
                continue;
            }
            if (i >= this->_materials.size())
            {
                continue;
            }
            if (this->_materials[i] == nullptr)
            {
                continue;
            }
            this->_materials[i]->setModelWorldMatrix(matrix.data());
            this->_materials[i]->setModelWorldMatrixIT(worldIT.data());
            GfxMaterial *gfxMaterial = this->_materials[i]->getGfxMaterial();
            GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), gfxMaterial, gfxMesh);
        }
        // 增加渲染物体数量
        profiler->addObjectCount(1);
    }
    void MeshRenderer::_resetMaterials()
    {
        if (this->_meshAsset == nullptr)
        {
            LOGW("MeshRenderer::_resetMaterials: meshAsset is nullptr");
            return;
        }
        MaterialAsset *defaultMaterial = dynamic_cast<MaterialAsset *>(assetsManager->getAsset("internal/materials/standard.mtl"));
        if (defaultMaterial == nullptr)
        {
            LOGW("MeshRenderer::_resetMaterials: default materialAsset is nullptr");
            return;
        }
        for (size_t i = 0; i < this->_meshAsset->getSubMeshCount(); i++)
        {
            MaterialAsset *material = new MaterialAsset();
            material->create(defaultMaterial);
            this->_materials.push_back(material);
        }
    }
    void MeshRenderer::_clearMaterials()
    {
        for (size_t i = 0; i < this->_materials.size(); i++)
        {
            this->_materials[i]->destroy();
            delete this->_materials[i];
            this->_materials[i] = nullptr;
        }
        this->_materials.clear();
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
