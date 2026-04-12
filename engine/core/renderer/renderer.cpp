
#include "renderer.h"
#include "../../boo.h"
#include "../../log.h"

namespace Boo
{

    Renderer::Renderer(/* args */)
    {
    }
    void Renderer::init()
    {
    }
    void Renderer::mountCamera(Camera *camera)
    {
        if (this->_cameras.find(camera->getUuid()) != this->_cameras.end())
        {
            return;
        }
        LOGI("[Renderer]:mount camera: %s", camera->getUuid().c_str());
        this->_cameras[camera->getUuid()] = camera;
    }
    void Renderer::unmountCamera(Camera *camera)
    {
        if (this->_cameras.find(camera->getUuid()) == this->_cameras.end())
        {
            return;
        }
        this->_cameras.erase(camera->getUuid());
        LOGI("[Renderer]:unmount camera: %s", camera->getUuid().c_str());
    }
    void Renderer::clearCameras()
    {
        this->_cameras.clear();
    }
    void Renderer::updateViewSize()
    {
        for (auto &camera : this->_cameras)
        {
            camera.second->updateViewSize();
        }
    }
    void Renderer::render(Scene *scene)
    {
        // LOGI("[Renderer]:render 1: %s", scene->getName().c_str());
        // 相机排序 按照从小到大优先级
        std::vector<Camera *> sortedCameras;
        for (auto &camera : this->_cameras)
        {
            sortedCameras.push_back(camera.second);
        }
        std::sort(sortedCameras.begin(), sortedCameras.end(), [](Camera *a, Camera *b)
                  { return a->getPriority() < b->getPriority(); });
        // LOGI("[Renderer]:render 2 size: %d", sortedCameras.size());
        for (auto camera : sortedCameras)
        {
            this->_renderCameras(camera, scene);
        }
    }
    void Renderer::_renderCameras(Camera *camera, Scene *scene)
    {
        // LOGI("[Renderer]:_renderCameras 1: %s", camera->getName().c_str());
        if (camera == nullptr)
        {
            return;
        }
        // LOGI("[Renderer]:_renderCameras 2: %s", camera->getName().c_str());
        if (scene == nullptr)
        {
            return;
        }
        // LOGI("[Renderer]:_renderCameras 3: %s", camera->getName().c_str());
        if (!camera->isEnabledInHierarchy())
        {
            return;
        }
        // LOGI("[Renderer]:_renderCameras 4: %s", camera->getName().c_str());
        camera->Render();
        // LOGI("[Renderer]:_renderCameras 5: %s", camera->getName().c_str());
        //std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
        this->_walkNode3D(camera, scene->getRoot3D());
        /*std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
        float frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
        LOGI("render frame 3D submit object duration: %f", frameDuration);*/
        this->_walkNode2D(camera, scene->getRoot2D());
    }
    void Renderer::_walkNode3D(Camera *camera, Node3D *node)
    {
        // std::cout << "node: " << node->getName().c_str() << std::endl;
        // std::cout << "node groupID: " << node->getGroupID() << std::endl;
        // std::cout << "camera groupIDs: " << camera->getGroupIDs() << std::endl;
        if (node == nullptr)
        {
            return;
        }
        if (!node->getActiveInHierarchy())
        {
            return;
        }
        if (!(node->getGroupID() & camera->getGroupIDs()))
        {
            return;
        }
        Vec3 worldScale = node->getWorldScale();
        if (worldScale.getX() == 0 || worldScale.getY() == 0)
        {
            // 缩放为0 不渲染
            return;
        }
        MeshRenderer *meshRenderer = node->getMeshRenderer();
        if (meshRenderer != nullptr && meshRenderer->isEnabledInHierarchy())
        {
            meshRenderer->Render(camera);
        }
        std::vector<Node *> &nodes = node->getChildren();
        for (auto node : nodes)
        {
            this->_walkNode3D(camera, dynamic_cast<Node3D *>(node));
        }
    }
    void Renderer::_walkNode2D(Camera *camera, Node2D *node)
    {
        if (node == nullptr)
        {
            return;
        }
        if (!node->getActiveInHierarchy())
        {
            return;
        }
        if (!(node->getGroupID() & camera->getGroupIDs()))
        {
            return;
        }
        Vec3 worldScale = node->getWorldScale();
        if (worldScale.getX() == 0 || worldScale.getY() == 0)
        {
            // 缩放为0 不渲染
            return;
        }
        UIRenderer *uiRenderer = node->getUIRenderer();
        if (uiRenderer != nullptr && uiRenderer->isEnabledInHierarchy())
        {
            uiRenderer->Render(camera);
            std::vector<Node *> &nodes = node->getChildren();
            for (auto node : nodes)
            {
                this->_walkNode2D(camera, dynamic_cast<Node2D *>(node));
            }
            UIMask *uiMask = dynamic_cast<UIMask *>(uiRenderer);
            if (uiMask != nullptr)
            {
                uiMask->lateRender(camera);
            }
        }
        std::vector<Node *> &nodes = node->getChildren();
        for (auto node : nodes)
        {
            this->_walkNode2D(camera, dynamic_cast<Node2D *>(node));
        }
    }
    Renderer::~Renderer()
    {
    }

} // namespace Boo
