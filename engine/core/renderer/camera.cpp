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
                                                                     _projection(CameraProjection::Perspective),
                                                                     _fov(60.0f),
                                                                     _nearClip(0.1f),
                                                                     _farClip(2000.0f),
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
        this->_updateViewMatrix();
        this->_updateProjectionMatrix();
        this->_createRenderPipeline();
    }
    void Camera::_createRenderPipeline()
    {
        // this->_renderTexture = GfxMgr::getInstance()->createRenderTexture(this->_uuid, view->getWidth(), view->getHeight());
        GfxMgr::getInstance()->createRenderQueue(this->_uuid, this->_priority, view->getWidth(), view->getHeight());
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
        GfxMgr::getInstance()->resizeRenderQueue(this->_uuid, view->getWidth(), view->getHeight());
        this->_updateProjectionMatrix();
    }
    void Camera::setPriority(int priority)
    {
        this->_priority = priority;
        GfxMgr::getInstance()->setRenderQueuePriority(this->_uuid, this->_priority);
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
    void Camera::addGroupID(NodeGroup groupID)
    {
        this->_groupIDs |= (int)groupID;
    }

    void Camera::setProjection(CameraProjection projection)
    {
        this->_projection = projection;
        this->_updateProjectionMatrix();
    }
    void Camera::Update(float deltaTime)
    {
        Component::Update(deltaTime);
    }
    void Camera::LateUpdate(float deltaTime)
    {
        Component::LateUpdate(deltaTime);
        if (this->_node->hasFrameTransformFlag())
        {
            this->_updateViewMatrix();
        }
    }
    void Camera::Render()
    {
        this->_updateViewMatrix();
        std::array<float, 4> cameraPosition = {this->_node->getPosition().getX(), this->_node->getPosition().getY(), this->_node->getPosition().getZ(), 0.0f};
        GfxMgr::getInstance()->submitRenderData(this->_uuid, this->_matView.data(), this->_matProj.data(), this->_isOnScreen, cameraPosition);
    }
    void Camera::_updateViewMatrix()
    {
        // 1. 获取节点世界矩阵的逆作为初始视图矩阵
        const Mat4 &worldMat = this->_node->getWorldMatrix();
        Mat4::inverse(worldMat, this->_matView);
        // 2. 从逆矩阵中提取 forward 向量（第2列的负值）
        // 列主序: m[col*4+row]
        // m02 = 第0列第2行, m06 = 第1列第2行, m10 = 第2列第2行
        Vec3 forward;
        forward.setX(-this->_matView.getM2());
        forward.setY(-this->_matView.getM6());
        forward.setZ(-this->_matView.getM10());

        // 3. 移除缩放: matView = matView × ScaleMatrix
        const Vec3 &worldScale = this->_node->getWorldScale();
        Mat4 scaleMat;
        scaleMat.scale(worldScale);
        Mat4 tempMat;
        Mat4::multiply(this->_matView, scaleMat, tempMat);
        this->_matView = tempMat;
       
    }
    void Camera::_updateProjectionMatrix()
    {
        Mat4 worldMat = this->_node->getWorldMatrix();
        if (this->_projection == CameraProjection::Ortho)
        {
            float left = -(float)view->getWidth() / 2.0f;
            float right = (float)view->getWidth() / 2.0f;
            float bottom = -(float)view->getHeight() / 2.0f;
            float top = (float)view->getHeight() / 2.0f;
            this->_nearClip = -1000.0f;
            this->_farClip = 1000.0f;
            Mat4::ortho(this->_matProj, left, right, bottom, top, this->_nearClip, this->_farClip,  -1.0f, 0);
        }
        else if (this->_projection == CameraProjection::Perspective)
        {
            float fovRad = this->_fov * 3.14159f / 180.0f;
            float aspectRatio = (float)view->getWidth() / (float)view->getHeight();
            Mat4::perspective(this->_matProj, fovRad, aspectRatio, this->_nearClip, this->_farClip,  -1.0f, 0);
        }
    }
    // -1 0 1
    // 1 0 1
    // -1 0 1
    void Camera::Disable()
    {
        Component::Disable();
        renderer->unmountCamera(this);
    }
    void Camera::destroy()
    {
        Component::destroy();
        GfxMgr::getInstance()->delRenderQueue(this->_uuid);
        this->_renderTexture = nullptr;
    }
    Camera::~Camera()
    {
    }

} // namespace Boo
