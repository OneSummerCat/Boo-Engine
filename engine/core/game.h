#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>
class Window;
class Android;
namespace Boo
{

    struct ScheduleInfo
    {
        std::function<void()> func;
        void *instance;
        float interval;
        float time;
        bool isOnce;
        bool clearFlag;
        ScheduleInfo()
            : func(), instance(nullptr), interval(0.0f), time(0.0f), isOnce(false), clearFlag(false)
        {
        }
        ScheduleInfo(std::function<void()> f, void *i, float in, bool once)
        {
            this->func = f;
            this->instance = i;
            this->interval = in;
            this->isOnce = once;
            this->time = 0.0f;
            this->clearFlag = false;
        }
    };

    enum class UIDesignFitMode;
    class Scene;
    class Camera;
    class Node;
    class Node3D;
    class Node2D;
    class Renderer;
    class Component;

    class Game
    {
    private:
        float _logicTime = 0.0f;
        float _renderTime = 0.0f;
        long long _deltaTime;
        int _frameRate;

        /**
         * @brief 视图是否改变
         */
        bool _viewChanged = false;
        long long _viewChangedTime = 0;
        /**
         * @brief 当前场景
         */
        Scene *_curScene;

    private:
        // 调度器相关
        uint64_t _scheduleNextID_ = 0;
        std::unordered_map<int, ScheduleInfo> _schedules;

    private:
        std::vector<int> _scheduleClearCaches;
        // 组件清理相关
        std::vector<Component *> _compClearCaches;
        // 节点清理相关
        std::vector<Node *> _nodeClearCaches;

    private:
        /**
         * @brief 初始化图形库
         */
        void _initGFX();

        void _initModules();
        /**
         * @brief 初始化视图
         * @param uiDesignWidth 设计宽度
         * @param uiDesignHeight 设计高度
         * @param fitMode 适配模式
         * @param width 实际宽度
         * @param height 实际高度
         */
        void _initView(int uiDesignWidth, int uiDesignHeight, UIDesignFitMode fitMode, int width, int height);
        /**
         * @brief 初始化事件系统
         */
        void _initEvent();
        /**
         * @brief 初始化输入系统
         */
        void _initInput();
        /**
         * @brief 初始化字体系统
         */
        void _initFont();
        /**_renderCameras
         * @brief 初始化资产系统
         */
        void _initAssets();

        void _initRenderer();

        /**
         * @brief 初始化透明度系统
         */
        void _initAlpha();

        void _update(float dt);
        void _updateSchedules(float dt);
        void _lateUpdate(float dt);
        void _render(float dt);

        void _clear();
        void _updateClearCaches();

    public:
        Game();
        ~Game();
        /**
         * @brief 初始化游戏
         * @param window 窗口
         * @param uiDesignWidth 设计宽度
         * @param uiDesignHeight 设计高度
         * @param fitMode 适配模式
         */
        void init(Window *window, int uiDesignWidth, int uiDesignHeight, UIDesignFitMode fitMode);
        void init(Android *android, int uiDesignWidth, int uiDesignHeight, UIDesignFitMode fitMode);

        Scene *getScene()
        {
            return this->_curScene;
        }
        void openScene(Scene *scene);
        void destroyScene();

        void resizeView(const int width, const int height);

        // typename T: 表示一个类型参数，通常指类的类型
        // typename Func: 表示另一个类型参数，通常指函数类型（函数指针、成员函数指针、函数对象等）
        template <typename T, typename Func>
        int schedule(Func func, T *instance, float interval)
        {
            int id = this->_scheduleNextID_++;
            auto callback = [instance, func]()
            {
                (instance->*func)();
            };
            this->_schedules[id] = ScheduleInfo(callback, instance, interval, false);

            return id;
        }
        template <typename T, typename Func>
        int scheduleOnce(Func func, T *instance, float interval)
        {
            int id = this->_scheduleNextID_++;
            auto callback = [instance, func]()
            {
                (instance->*func)();
            };
            ScheduleInfo scheduleInfo(callback, instance, interval, true);
            this->_schedules[id] = scheduleInfo;
            return id;
        }
        void unschedule(int scheduleID);

        void addCompClearCaches(Component *comp);
        void addNodeClearCaches(Node *node);

        /**
         * @brief 更新鼠标状态
         *
         * @param button 鼠标按钮 0:左键 1:右键 2:中键
         * @param action 鼠标操作 0:抬起 1:按下
         * @param mods 键盘修饰键 0:无 1:Shift 2:Ctrl 3:Alt
         */
        void updateMouseState(int button, int action, int mods);
        /**
         * @brief 更新鼠标位置
         *
         * @param xpos 鼠标X坐标
         * @param ypos 鼠标Y坐标
         */
        void updateMousePos(double xpos, double ypos);
        /**
         * @brief 更新键盘状态
         *
         * @param key 键盘键值
         * @param scancode 键盘扫描码
         * @param action 键盘操作 0:抬起 1:按下
         * @param mods 键盘修饰键 0:无 1:Shift 2:Ctrl 3:Alt
         */
        void updateKeyState(int key, int scancode, int action, int mods);

        void tick();
    };

} // namespace Boo
