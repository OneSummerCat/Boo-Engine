#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <unordered_map>

namespace Boo
{

    class Camera;
    class Node;
    class Node2D;
    class Scene;

    class Renderer
    {
    private:
        /**
         * @brief 相机系统
         */
        std::unordered_map<std::string, Camera *> _cameras;

    private:
        void _renderCameras(Camera *camera, Scene *scene);
        void _walkNode3D(Camera *camera, Node *node);
        void _walkNode2D(Camera *camera, Node2D *node);

    public:
        Renderer(/* args */);
        void init();
        void mountCamera(Camera *camera);
        void unmountCamera(Camera *camera);
        void clearCameras();
        void updateViewSize();
        void render( Scene *scene);
        ~Renderer();
    };

} // namespace Boo
