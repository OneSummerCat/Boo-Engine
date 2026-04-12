#include "ui-renderer.h"
#include <filesystem>
#include "../../../boo.h"
#include "../../../log.h"
#include "../../gfx/gfx.h"
#include "../../gfx/gfx-mgr.h"
#include "../../scene/node.h"
#include "../../gfx/base/gfx-mesh.h"
#include "../../gfx/base/gfx-material.h"

namespace Boo
{

	UIRenderer::UIRenderer(std::string name, Node *node, std::string uuid) : Component(name, node, uuid),
																			 _uiViewMatrix(Mat4::identity())
	{
		this->_layer = ComponentLayer::Node2D;
		this->_node2D = dynamic_cast<Node2D *>(node);
	}
	void UIRenderer::Awake()
	{
		Component::Awake();
	}
	void UIRenderer::Enable()
	{
		Component::Enable();
	}
	void UIRenderer::Update(float deltaTime)
	{
		Component::Update(deltaTime);
	}
	void UIRenderer::LateUpdate(float deltaTime)
	{
		Component::LateUpdate(deltaTime);
	}
	bool UIRenderer::_canRenderer(Camera *camera)
	{
		if (camera == nullptr)
		{
			return false; // 相机为空
		}
		if (this->_node2D == nullptr)
		{
			return false; // 节点不是Node2D类型
		}
		if (this->_node2D->getSize().getHeight() <= 0 || this->_node2D->getSize().getWidth() <= 0)
		{
			return false; // 节点不可见
		}
		return true;
	}
	void UIRenderer::Render(Camera *camera)
	{	
		 
	}

	void UIRenderer::Disable()
	{
		Component::Disable();
	}
	void UIRenderer::destroy()
	{
		Component::destroy();
	}

	UIRenderer::~UIRenderer()
	{
	}

} // namespace Boo
// void UIRenderer::_setColor(float r, float g, float b, float a)
	// {
	// 	// 透明度会影响到子节点的透明度
	// 	if (r == this->_color.getR() && g == this->_color.getG() && b == this->_color.getB() && a == this->_color.getA())
	// 	{
	// 		return;
	// 	}
	// 	this->_color.set(r, g, b, a);
	// 	this->_materialAsset->setUIColor(r, g, b, a);
	// 	if (!this->_isEnabledInHierarchy)
	// 		return;
	// }
	// const Color &UIRenderer::getColor()
	// {
	// 	return this->_color;
	// }

	// void UIRenderer::_setMaterial(MaterialAsset *mtl)
	// {
	// 	if (mtl == nullptr)
	// 	{
	// 		LOGW("UIRenderer::setMaterial: mtl is nullptr");
	// 		return;
	// 	}
	// 	if (this->_materialAsset != nullptr && this->_materialAsset->getUuid() == mtl->getUuid())
	// 	{
	// 		LOGW("UIRenderer::setMaterial: mtl is same as _materialAsset");
	// 		return;
	// 	}
	// 	this->_materialAsset->create(mtl);
	// }