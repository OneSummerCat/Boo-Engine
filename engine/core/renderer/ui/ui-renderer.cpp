#include "ui-renderer.h"
#include <filesystem>
#include "../../../boo.h"
#include "../../../log.h"
#include "../../gfx/gfx.h"
#include "../../gfx/gfx-mgr.h"
#include "../../gfx/base/gfx-mesh.h"
#include "../../scene/node.h"

namespace Boo
{

	UIRenderer::UIRenderer(std::string name, Node *node, std::string uuid) : Component(name, node, uuid),
																			 _textureAsset(nullptr),
																			 _materialAsset(nullptr),
																			 _color(1.0f, 1.0f, 1.0f, 1.0f),
																			 _uiViewMatrix(Mat4::identity())
	{
		this->_layer = NodeLayer::Node2D;
		this->_materialAsset = new MaterialAsset();
	}
	void UIRenderer::Awake()
	{
		Component::Awake();
	}
	void UIRenderer::Enable()
	{
		Component::Enable();
	}
	const int UIRenderer::getGroupID()
	{
		return this->_node->getGroupID();
	}
	void UIRenderer::_setColor(float r, float g, float b, float a)
	{
		// 透明度会影响到子节点的透明度
		if (r == this->_color.getR() && g == this->_color.getG() && b == this->_color.getB() && a == this->_color.getA())
		{
			return;
		}
		this->_color.set(r, g, b, a);
		if (!this->_isEnabledInHierarchy)
			return;
	}

	void UIRenderer::_setMaterial(MaterialAsset *mtl)
	{
		if (mtl == nullptr)
		{
			std::cout << "UIRenderer::setMaterial: mtl is nullptr" << std::endl;
			return;
		}
		if (this->_materialAsset != nullptr && this->_materialAsset->getUuid() == mtl->getUuid())
		{
			return;
		}
		this->_materialAsset->create(mtl);
	}
	void UIRenderer::_setTexture(TextureAsset *texture)
	{
		if (texture == nullptr)
		{
			LOGW("UIRenderer::setTexture: texture is nullptr");
			return;
		}
		if (this->_textureAsset != nullptr && this->_textureAsset->getUuid() == texture->getUuid())
		{
			return;
		}
		this->_textureAsset = texture;
		this->_materialAsset->setTexture(texture->getUuid());
	}
	void UIRenderer::Update(float deltaTime)
	{
		Component::Update(deltaTime);
	}
	void UIRenderer::LateUpdate(float deltaTime)
	{
		Component::LateUpdate(deltaTime);
	}
	void UIRenderer::Render(Camera *camera)
	{
		if (camera == nullptr)
		{
			return; // 相机为空
		}
		Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
		if (node2D == nullptr)
		{
			return; // 节点不是Node2D类型
		}
		if (node2D->getSize().getHeight() <= 0 || node2D->getSize().getWidth() <= 0)
		{
			return; // 节点不可见
		}
		if (this->_materialAsset == nullptr)
		{
			return; // 节点没有设置材质
		}
		// 提交渲染对象
		this->_instanceData.clear();
		this->_instanceData.reserve(16 + 4);
		// 1. 先添加模型矩阵 (16个float)
		const Mat4 &matrix = node2D->getUIWorldMatrix();
		// std::cout << "node2D name: " << node2D->getName() << std::endl;
		// std::cout << "matrix1: " << matrix.getM00() << std::endl;
		// std::cout << "matrix5: " << matrix.getM11() << std::endl;
		// std::cout << "matrix12: " << matrix.getM30() << std::endl;
		// std::cout << "matrix13: " << matrix.getM31() << std::endl;
		Mat4::multiply(matrix, view->getFitMatrix(), this->_uiViewMatrix);
		// std::cout << "uiViewMatrix1: " << this->_uiViewMatrix.getM00() << std::endl;
		// std::cout << "uiViewMatrix5: " << this->_uiViewMatrix.getM11() << std::endl;
		// std::cout << "uiViewMatrix12: " << this->_uiViewMatrix.getM30() << std::endl;
		// std::cout << "uiViewMatrix13: " << this->_uiViewMatrix.getM31() << std::endl;
		// _instanceData.insert(_instanceData.end(), matrix.begin(), matrix.end());
		_instanceData.insert(_instanceData.end(), this->_uiViewMatrix.data().begin(), this->_uiViewMatrix.data().end());
		// 2. 再添加颜色 (4个float)
		_instanceData.insert(_instanceData.end(), {_color.getR(), _color.getG(), _color.getB(), _color.getA()});
		GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), this->_materialAsset->getGfxMaterial(), Gfx::uiTestMesh, this->_instanceData);
	}

	void UIRenderer::Disable()
	{
		Component::Disable();
	}
	void UIRenderer::destroy()
	{
		Component::destroy();
		// LOGI("[UIRenderer]:destroy: %s", this->_node->getName().c_str());
		this->_materialAsset->destroy();
		this->_textureAsset = nullptr;
	}

	UIRenderer::~UIRenderer()
	{
	}

} // namespace Boo
