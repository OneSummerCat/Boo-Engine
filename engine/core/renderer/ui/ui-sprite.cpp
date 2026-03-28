#include "ui-sprite.h"
#include "../../../boo.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../renderer/camera.h"
#include "../../assets/assets-manager.h"
#include "../../../log.h"

namespace Boo
{

    UISprite::UISprite(std::string name, Node *node, std::string uuid) : UIRenderer(name, node, uuid)
    {
        this->sizeMode = SizeMode::Custom;
        // 默认纹理
        this->_textureAsset = nullptr;
        // 默认UI材质
        this->_materialAsset = new MaterialAsset(AssetBuiltinMaterial::UI,"","");
        MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetsManager->getAsset(AssetBuiltinMaterial::UI));
        this->_materialAsset->create(mtl);
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
        this->_setColor(color.getR(), color.getG(), color.getB(), color.getA());
    }
    void UISprite::setColor(std::string color)
    {
        if (color == this->_color.hexString())
        {
            return;
        }
        Color c(color);
        this->_setColor(c.getR(), c.getG(), c.getB(), c.getA());
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
        this->_setColor(r, g, b, a);
    }

    void UISprite::setAlpha(float alpha)
    {
        if (alpha == this->_color.getA())
            return;
        Color c(this->_color);
        c.setA(alpha);
        this->_setColor(c.getR(), c.getG(), c.getB(), c.getA());
    }
    void UISprite::setMaterial(std::string material)
    {
        MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetsManager->getAsset(material));
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
        this->_setMaterial(material);
    }

    void UISprite::setTexture(std::string path)
    {
        TextureAsset *texture = dynamic_cast<TextureAsset *>(assetsManager->getAsset(path));
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
        // this->_materialAsset->setTexture("");
    }
    void UISprite::setRenderTexture()
    {
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
        UIRenderer::Render(camera);
    }
    void UISprite::Disable()
    {
        UIRenderer::Disable();
    }

    void UISprite::destroy()
    {
        UIRenderer::destroy();
    }
    UISprite::~UISprite()
    {
    }

} // namespace Boo