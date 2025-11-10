#pragma once
#include <string>
#include <functional>
#include <unordered_map>

class Component;
class Scene;
class ComponentFactory;
class Event;
class AssetsManager;

struct View
{
    int width = 1280;
    int height = 720;
};
struct ScheduleInfo
{
    std::function<void()> func;
    void *instance;
    float interval;
    float time;
    bool isOnce;
};

class Game
{
private:
    Game();
    ~Game();
    Game(const Game &) = delete;            // 禁用拷贝构造
    Game &operator=(const Game &) = delete; // 禁用赋值操作符
    Event *_event;
    ComponentFactory *_componentFactory = nullptr;
    View _view;
    Scene *_curScene;
    AssetsManager *_assetsManager;

    uint64_t _scheduleNextID_ = 0;
    std::unordered_map<int, ScheduleInfo> _schedules;

    void _initEvent();
    void _initInput();
    void _initFont();
    void _initComponents();
    void _initAssets();
    void _initAlpha();

    void _updateSchedules(float dt);
public:
    static Game *getInstance();
    void init();

    void setView(int width, int height);
    View &view()
    {
        return this->_view;
    }
    Event *event()
    {
        return this->_event;
    }
    ComponentFactory *componentFactory()
    {
        return this->_componentFactory;
    };
    Scene *getScene()
    {
        return this->_curScene;
    }
    AssetsManager *assetsManager()
    {
        return this->_assetsManager;
    }
    template <typename T, typename Func>
    int schedule(Func func, T *instance, float interval);
    template <typename T, typename Func>
    int scheduleOnce(Func func, T *instance, float interval);
    void unschedule(int scheduleID);

    Scene *openScene(std::string sceneName);
    void update(float dt);
};
