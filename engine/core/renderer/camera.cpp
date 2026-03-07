#include "camera.h"
#include "../../boo.h"
#include "../../log.h"
#include "../gfx/gfx-mgr.h"
#include "../gfx/base/gfx-render-texture.h"

namespace Boo
{

    Camera::Camera(std::string name, Node *node, std::string uuid) : Component(name, node, uuid),
                                                                     _priority(0),
                                                                     _groupIDs(0),
                                                                     _type(0),
                                                                     _fieldOfView(45.0f),
                                                                     _nearClip(0.1f),
                                                                     _farClip(100.0f),
                                                                     _orthoHeight(1.0f),
                                                                     _isOnScreen(true),
                                                                     _renderTexture(nullptr)

    {
    }
    void Camera::Awake()
    {
        Component::Awake();
        this->_matView = Mat4::identity();
        this->_matProj = Mat4::identity();
        this->_matProj.setM00(2.0f / (float)view->getWidth());
        this->_matProj.setM11(2.0f / (float)view->getHeight());
        this->_createRenderPipeline();
    }
    void Camera::_createRenderPipeline()
    {
        this->_renderTexture = new GfxRenderTexture(this->_uuid, view->getWidth(), view->getHeight());
        GfxMgr::getInstance()->initRenderQueue(this->_uuid, this->_renderTexture);
    }
    void Camera::Enable()
    {
        Component::Enable();
        renderer->mountCamera(this);
    }
    void Camera::setIsOnScreen(bool isOnScreen)
    {
        this->_isOnScreen = isOnScreen;
    }
    bool Camera::getIsOnScreen()
    {
        return this->_isOnScreen;
    }
    void Camera::updateViewSize()
    {
        if (this->_renderTexture == nullptr)
        {
            return;
        }
        if (this->_renderTexture->getWidth() == view->getWidth() && this->_renderTexture->getHeight() == view->getHeight())
        {
            return;
        }
        this->_renderTexture->resize(view->getWidth(), view->getHeight());
        this->_matProj.setM00(2.0f / (float)view->getWidth());
        this->_matProj.setM11(2.0f / (float)view->getHeight());
    }
    void Camera::setPriority(int priority)
    {
        this->_priority = priority;
    }
    int Camera::getPriority()
    {
        return this->_priority;
    }
    void Camera::setGroupIDs(int groupIDs)
    {
        this->_groupIDs = groupIDs;
    }
    void Camera::addGroupID(int groupID)
    {
        this->_groupIDs |= groupID;
    }
    int Camera::getGroupIDs()
    {
        return this->_groupIDs;
    }
    void Camera::Update(float deltaTime)
    {
        Component::Update(deltaTime);
    }
    void Camera::LateUpdate(float deltaTime)
    {
        Component::LateUpdate(deltaTime);
    }
    void Camera::Render()
    {
        GfxMgr::getInstance()->submitRenderData(this->_uuid, this->_matView.data(), this->_matProj.data(), this->_isOnScreen);
    }
    void Camera::Disable()
    {
        Component::Disable();
        renderer->unmountCamera(this);
    }
    void Camera::destroy()
    {
        Component::destroy();
        this->_renderTexture->destroy();
        GfxMgr::getInstance()->delRenderQueue(this->_uuid);
    }
    Camera::~Camera()
    {
        std::cout << "Camera::~destructor" << this->_node->getName() << std::endl;
    }

} // namespace Boo
