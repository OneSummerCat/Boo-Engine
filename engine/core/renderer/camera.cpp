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
        this->_renderTexture = GfxMgr::getInstance()->createRenderTexture(this->_uuid, view->getWidth(), view->getHeight());
        GfxMgr::getInstance()->initRenderQueue(this->_uuid, this->_renderTexture,this->_priority);
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
        GfxMgr::getInstance()->submitRenderData(this->_uuid, this->_matView.data(), this->_matProj.data(), this->_isOnScreen);
    }
    void Camera::_updateViewMatrix()
    {
        Vec3 _localForward{0, 0, -1.0};
        Vec3 _localRight{1, 0, 0};
        Vec3 _localUp{0, 1, 0};
        // 从世界矩阵中提取位置和旋转信息
        const Mat4 &worldMat = this->_node->getWorldMatrix();
        Vec3 position{};
        Mat4::getPosition(worldMat, position);
        Quat rotation{};
        Mat4::getRotation(worldMat, rotation);

        Vec3 _worldForward{0, 0, 0.0};
        Vec3 _worldRight{0, 0, 0};
        Vec3 _worldUp{0, 0, 0};
        Vec3::transformQuat(_worldForward, _localForward, rotation);
        Vec3::transformQuat(_worldRight, _localRight, rotation);
        Vec3::transformQuat(_worldUp, _localUp, rotation);
        // 构建视图矩阵（行主序）
        // 第一行：right 轴
        this->_matView.setM00(_worldRight.getX());
        this->_matView.setM01(_worldRight.getY());
        this->_matView.setM02(_worldRight.getZ());
        this->_matView.setM03(-Vec3::dot(_worldRight, position));

        // 第二行：up 轴
        this->_matView.setM10(_worldUp.getX());
        this->_matView.setM11(_worldUp.getY());
        this->_matView.setM12(_worldUp.getZ());
        this->_matView.setM13(-Vec3::dot(_worldUp, position));

        // 第三行：-forward 轴
        this->_matView.setM20(-_worldForward.getX());
        this->_matView.setM21(-_worldForward.getY());
        this->_matView.setM22(-_worldForward.getZ());
        this->_matView.setM23(Vec3::dot(_worldForward, position));

        // 第四行
        this->_matView.setM30(0.0f);
        this->_matView.setM31(0.0f);
        this->_matView.setM32(0.0f);
        this->_matView.setM33(1.0f);
    }
    void Camera::_updateProjectionMatrix()
    {
        Mat4 worldMat = this->_node->getWorldMatrix();
        if (this->_projection == CameraProjection::Ortho)
        {
            this->_matProj.setM00(2.0f / (float)view->getWidth());
            this->_matProj.setM11(2.0f / (float)view->getHeight());
            // 摄像机看向 -Z 方向（OpenGL风格） 近平面在 Z = -near，远平面在 Z = -far
            // 公式变成：
            // proj.m[2][2] = -2.0f / (far - near);  // 负号是因为方向相反
            this->_matProj.setM22(-2.0f / (this->_farClip - this->_nearClip));
            this->_matProj.setM30(0.0f);
            this->_matProj.setM31(0.0f);
            this->_matProj.setM32(0.0f);
            this->_matProj.setM33(1.0f);
        }
        else if (this->_projection == CameraProjection::Perspective)
        {
            float tanHalfFov = tan(this->_fov * 3.14159f / 360.0f); // 视角的一半的正切值
            float range = this->_farClip - this->_nearClip;
            float aspectRatio = (float)view->getWidth() / (float)view->getHeight();
            this->_matProj.setM00(1.0f / (aspectRatio * tanHalfFov));
            this->_matProj.setM11(1.0f / (tanHalfFov));
            this->_matProj.setM22(-(this->_farClip + this->_nearClip) / range);
            this->_matProj.setM23(-1.0f);
            this->_matProj.setM32(-2.0f * this->_farClip * this->_nearClip / range);
            this->_matProj.setM33(0.0f);
        }
    }
    void Camera::Disable()
    {
        Component::Disable();
        renderer->unmountCamera(this);
    }
    void Camera::destroy()
    {
        Component::destroy();
        GfxMgr::getInstance()->delRenderQueue(this->_uuid);
        GfxMgr::getInstance()->destroyRenderTexture(this->_renderTexture);
        this->_renderTexture = nullptr;
    }
    Camera::~Camera()
    {
        std::cout << "Camera::~destructor" << this->_node->getName() << std::endl;
    }

} // namespace Boo
