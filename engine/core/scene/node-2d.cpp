#include "node-2d.h"
#include "../../boo.h"
#include "../../log.h"
#include "../component/component-factory.h"
#include "../renderer/ui/ui-renderer.h"
// #include "../util/util-api.h"
// #include "../input/input.h"

namespace Boo
{
	Node2D::Node2D(const std::string name, const std::string uuid)
	{
		this->_groupID = uint32_t(NodeGroup::Node2D);
		this->_name = name;
		this->_layer = NodeLayer::Node2D;
		this->_uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
		this->_anchor.set(0.5, 0.5);
		this->_size.set(200, 200);
		this->_active = true;
		this->_isActiveInHierarchy = false;
		this->_position.set(0.0f, 0.0f, 0.0f);
		this->_scale.set(1.0f, 1.0f, 1.0f);
		this->_eulerAngles.set(0.0f, 0.0f, 0.0f);
		this->_rotation.set(0.0f, 0.0f, 0.0f, 1.0f);
		this->_worldPosition.set(0.0f, 0.0f, 0.0f);
		this->_worldScale.set(1.0f, 1.0f, 1.0f);
		this->_worldRotation.set(0.0f, 0.0f, 0.0f, 1.0f);
		this->_localMatrix = Mat4::identity();
		this->_worldMatrix = Mat4::identity();
		this->_uiWorldMatrix = Mat4::identity();
		this->_worldTransformFlag = static_cast<uint32_t>(NodeTransformFlag::ALL_FLAG);
		this->_frameTransformFlag = static_cast<uint32_t>(NodeTransformFlag::ALL_FLAG);
		this->_parent = nullptr;
		this->_uiRenderer = nullptr;
	}

	/**
	 * 2d 节点的锚点
	 * @param x
	 * @param y
	 */
	void Node2D::setAnchor(float x, float y)
	{
		if (this->_anchor.getX() == x && this->_anchor.getY() == y)
		{
			return;
		}
		this->_anchor.set(x, y);
		this->_updateWorldTransformFlag(NodeTransformFlag::ANCHOR_FLAG);
	}
	const Vec2 &Node2D::getAnchor()
	{
		return this->_anchor;
	}
	/**
	 * 2d 节点的大小
	 * @param width
	 * @param height
	 */
	void Node2D::setSize(float width, float height)
	{
		if (this->_size.getWidth() == width && this->_size.getHeight() == height)
		{
			return;
		}
		this->_size.set(width, height);
		this->_updateWorldTransformFlag(NodeTransformFlag::SIZE_FLAG);
	}
	const Size &Node2D::getSize()
	{
		return this->_size;
	}
	void Node2D::setPosition(float x, float y, float z)
	{
		Node::setPosition(x, y, z);
	}
	void Node2D::setWorldPosition(float x, float y, float z)
	{
		Node::setWorldPosition(x, y, z);
	}
	void Node2D::setScale(float x, float y, float z)
	{
		Node::setScale(x, y, z);
	}
	void Node2D::setWorldScale(float x, float y, float z)
	{
		Node::setWorldScale(x, y, z);
	}
	void Node2D::setEulerAngles(float x, float y, float z)
	{
		Node::setEulerAngles(x, y, z);
	}
	void Node2D::setRotation(float x, float y, float z, float w)
	{
		Node::setRotation(x, y, z, w);
	}
	void Node2D::setWorldRotation(float x, float y, float z, float w)
	{
		Node::setWorldRotation(x, y, z, w);
	}
	/**
	 * 2d 节点的世界变换矩阵
	 */
	void Node2D::_updateWorldTransform()
	{
		Node::_updateWorldTransform();
		// 坐标
		float x = this->_worldMatrix.getM30() + (0.5 - this->_anchor.getX()) * this->_size.getWidth();
		float y = this->_worldMatrix.getM31() + (0.5 - this->_anchor.getY()) * this->_size.getHeight();
		// 尺寸
		float width = this->_worldMatrix.getM00() * this->_size.getWidth();
		float height = this->_worldMatrix.getM11() * this->_size.getHeight();
		this->_uiWorldMatrix.setM30(x);
		this->_uiWorldMatrix.setM31(y);
		this->_uiWorldMatrix.setM00(width); // 宽高和缩放进行相乘
		this->_uiWorldMatrix.setM11(height);
	}
	const Mat4 &Node2D::getUIWorldMatrix()
	{
		return this->_uiWorldMatrix;
	}
	void Node2D::setActive(bool active)
	{
		Node::setActive(active);
	}

	Component *Node2D::addComponent(std::string name, std::string uuid)
	{
		Component *component = ComponentFactory::getInstance().createComponent(name, this, uuid);
		if (component == nullptr)
		{
			// std::cout << name << ":Component Not register" << std::endl;
			LOGW("[Node2D]:addComponent:: %s, %s, Component Not register", name.c_str(), uuid.c_str());
			return nullptr;
		}
		if (component->layer() == NodeLayer::Node3D)
		{
			// std::cout << name << ":Component add fail,node type is Node3D" << std::endl;
			LOGW("[Node2D]:addComponent:: %s, %s, Component add fail,node type is Node3D", name.c_str(), uuid.c_str());
			delete component;
			component = nullptr;
			return nullptr;
		}
		if (dynamic_cast<UIRenderer *>(component) != nullptr)
		{
			if (this->_uiRenderer == nullptr)
			{
				this->_uiRenderer = dynamic_cast<UIRenderer *>(component);
			}
			else
			{
				LOGW("[Node2D]:addComponent:: %s, %s, Component add fail,node already has UIRenderer", name.c_str(), uuid.c_str());
				delete component;
				component = nullptr;
				return nullptr;
			}
		}
		this->_components.push_back(component);

		// LOGI("Node2D::addComponent: %s, %s", this->getName().c_str(), component->getName().c_str());
		if (this->_parent != nullptr)
		{
			component->setNodeActiveInHierarchy(this->_isActiveInHierarchy);
		}
		return component;
	}
	/**
	 * 获取UI渲染组件
	 */
	UIRenderer *Node2D::getUIRenderer()
	{
		return this->_uiRenderer;
	}

	void Node2D::update(float dt)
	{
		Node::update(dt);
	}
	void Node2D::lateUpdate(float dt)
	{
		Node::lateUpdate(dt);
	}
	void Node2D::clearNodeFrameFlag()
	{
		Node::clearNodeFrameFlag();
	}
	// void Node2D::offNodeInputEvent(int inputID)
	// {
	// 	Boo::game->input()->offNodeInputEvent(this, inputID);
	// }
	// void Node2D::offAllNodeInputEvent()
	// {
	// 	Boo::game->input()->offAllNodeInputEvent(this);
	// }
	bool Node2D::inHitMask(float x, float y)
	{
		const Mat4 &uiMat = this->getUIWorldMatrix();
		float _x = uiMat.getM30();
		float _y = uiMat.getM31();
		float _width = uiMat.getM00();
		float _height = uiMat.getM11();
		if (x >= _x - _width / 2.0 && x <= _x + _width / 2.0 && y >= _y - _height / 2.0 && y <= _y + _height / 2.0)
		{
			return true;
		}
		return false;
	}
	/**
	 * 2d 节点的点击事件
	 * @param x
	 * @param y
	 * @return true
	 * @return false
	 */
	bool Node2D::inHitOnNode(float x, float y)
	{
		// UI Mask属于特殊情况，必须在ui-node里边，并且同时在ui-mask里边
		const Mat4 &uiMat = this->getUIWorldMatrix();
		float _x = uiMat.getM30();
		float _y = uiMat.getM31();
		float _width = uiMat.getM00();
		float _height = uiMat.getM11();

		float _left = _x - _width / 2.0;
		float _right = _x + _width / 2.0;
		float _top = _y - _height / 2.0;
		float _bottom = _y + _height / 2.0;
		if (x >= _left && x <= _right && y >= _top && y <= _bottom)
		{
			return true;
		}
		return false;
	}
	void Node2D::destroy()
	{
		Node::destroy();
	}
	Node2D::~Node2D()
	{
		// LOGI("[Node2D]:~destructor:: %s", this->_name.c_str());
	}

} // namespace Boo