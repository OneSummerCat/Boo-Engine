#include "ui-sprite.h"
#include "../../../boo.h"
#include "../../../log.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../renderer/camera.h"
#include "../../assets/assets-manager.h"
#include "../../gfx/gfx.h"
#include "../../gfx/gfx-mgr.h"
#include "../../gfx/base/gfx-mesh.h"
#include "../../gfx/base/gfx-material.h"

namespace Boo
{

    UISprite::UISprite(std::string name, Node *node, std::string uuid) : UIRenderer(name, node, uuid)
    {
        this->sizeMode = SizeMode::Custom;
        // 默认纹理
        this->_textureAsset = nullptr;
        // 默认UI材质
        this->_createDefaultMaterial();
        // 默认UI网格
        this->_createDefaultMesh();
    }
    void UISprite::_createDefaultMaterial()
    {
        this->_materialAsset = new MaterialAsset(AssetBuiltinMaterial::UI, "", "");
        MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetsManager->getAsset(AssetBuiltinMaterial::UI));
        this->_materialAsset->create(mtl);
    }

    void UISprite::_createDefaultMesh()
    {
        this->_meshAsset = new MeshAsset();
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
        this->_meshAsset->create({primitive});
    }

    void UISprite::Awake()
    {
        UIRenderer::Awake();
    }

    void UISprite::Enable()
    {
        UIRenderer::Enable();
    }

    void UISprite::setColor(Color &color)
    {
        if (color.getR() == this->_color.getR() &&
            color.getG() == this->_color.getG() &&
            color.getB() == this->_color.getB() &&
            color.getA() == this->_color.getA())
        {
            return;
        }
        this->setColor(color.getR(), color.getG(), color.getB(), color.getA());
    }
    void UISprite::setColor(std::string color)
    {
        if (color == this->_color.hexString())
        {
            return;
        }
        Color c(color);
        this->setColor(c.getR(), c.getG(), c.getB(), c.getA());
    }
    void UISprite::setAlpha(float alpha)
    {
        if (alpha == this->_color.getA())
            return;
        Color c(this->_color);
        c.setA(alpha);
        this->setColor(c.getR(), c.getG(), c.getB(), c.getA());
    }
    void UISprite::setColor(float r, float g, float b, float a)
    {
        if (r == this->_color.getR() &&
            g == this->_color.getG() &&
            b == this->_color.getB() &&
            a == this->_color.getA())
        {
            return;
        }
        this->_color.set(r, g, b, a);
        this->_materialAsset->setUIColor(r, g, b, a);
    }

    void UISprite::setMaterial(std::string material)
    {
        MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetsManager->getAsset(material,true));
        if (mtl == nullptr)
        {
            LOGW("[UISprite]:setMaterial: material %s not found", material.c_str());
            return;
        }
        this->setMaterial(mtl);
    }
    void UISprite::setMaterial(MaterialAsset *material)
    {
        if (material == nullptr)
        {
            LOGW("[UISprite]:setMaterial: material %s not found", material->getName().c_str());
            return;
        }
        this->_materialAsset->create(material);
    }

    void UISprite::setTexture(std::string path)
    {
        TextureAsset *texture = dynamic_cast<TextureAsset *>(assetsManager->getAsset(path,true));
        if (texture == nullptr)
        {
            LOGW("[UISprite]:setTexture: texture %s not found", path.c_str());
            return;
        }
        this->setTexture(texture);
    }
    void UISprite::setTexture(TextureAsset *texture)
    {
        if (this->_textureAsset == texture)
        {
            return;
        }
        this->_textureAsset = texture;
        if (this->_textureAsset == nullptr)
        {
            return;
        }
        if (this->sizeMode == SizeMode::Raw)
        {
            float width = this->_textureAsset->getWidth();
            float height = this->_textureAsset->getHeight();
            this->_node2D->removeSizeLock(Node2DSizeLock::SpriteRaw);
            this->_node2D->setSize(width, height);
            this->_node2D->addSizeLock(Node2DSizeLock::SpriteRaw);
        }
        if (this->_materialAsset == nullptr)
        {
            return;
        }
        this->_materialAsset->setTexture(texture);
    }

    void UISprite::setSizeMode(SizeMode sizeMode)
    {
        if (this->sizeMode == sizeMode)
        {
            return;
        }
        this->sizeMode = sizeMode;
        if (this->sizeMode == SizeMode::Raw)
        {
            if (this->_textureAsset != nullptr)
            {
                float width = this->_textureAsset->getWidth();
                float height = this->_textureAsset->getHeight();
                this->_node2D->removeSizeLock(Node2DSizeLock::SpriteRaw);
                this->_node2D->setSize(width, height);
            }
            this->_node2D->addSizeLock(Node2DSizeLock::SpriteRaw);
        }
        else
        {
            this->_node2D->removeSizeLock(Node2DSizeLock::SpriteRaw);
        }
    }

    void UISprite::Update(float deltaTime)
    {
        UIRenderer::Update(deltaTime);
    }
    void UISprite::LateUpdate(float deltaTime)
    {
        UIRenderer::LateUpdate(deltaTime);
    }
    void UISprite::Render(Camera *camera)
    {
        if (!this->_canRenderer(camera))
        {
            return;
        }
        if (this->_materialAsset == nullptr)
        {
            return;
        }
        if (this->_meshAsset == nullptr)
        {
            return;
        }
        UIRenderer::Render(camera);
        // 后续添加节点层级alpha测试
        const Mat4 &matrix = this->_node2D->getUIWorldMatrix();
        Mat4::multiply(matrix, view->getFitMatrix(), this->_uiViewMatrix);
        // 更新世界矩阵
        this->_materialAsset->setModelWorldMatrix(this->_uiViewMatrix.data());
        GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), this->_materialAsset->getGfxMaterial(), this->_meshAsset->getGfxMesh(0));
        // 增加渲染物体数量
        profiler->addObjectCount(1);
    }
    void UISprite::Disable()
    {
        UIRenderer::Disable();
    }

    void UISprite::destroy()
    {
        UIRenderer::destroy();
        delete this->_materialAsset;
        delete this->_meshAsset;
        this->_materialAsset = nullptr;
        this->_meshAsset = nullptr;
    }
    UISprite::~UISprite()
    {
    }

} // namespace Boo