#include "game.h"
#include "../boo.h"
#include "../log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb/stb_image_write.h"
#include "../platforms/window/window.h"
#include "../platforms/android/android.h"
#include "../boo.h"
#include "gfx/gfx-mgr.h"
#include "alpha/alpha.h"

namespace Boo
{

	Game::Game() : _logicTime(TimeUtil::nowTime()),
				   _renderTime(TimeUtil::nowTime()),
				   _deltaTime(TimeUtil::nowTime()),
				   _frameRate(60),
				   _viewChanged(false),
				   _viewChangedTime(0.0f),
				   _curScene(nullptr),
				   _scheduleNextID_(0)
	{
	}
	void Game::init(Window *window, int uiDesignWidth, int uiDesignHeight, UIDesignFitMode fitMode)
	{
		LOGI("[Game]:INIT WINDOW");
		GfxMgr::getInstance()->init(window);
		this->_initView(uiDesignWidth, uiDesignHeight, fitMode,window->getWidth(), window->getHeight());
		this->_initModules();
	}
	void Game::init(Android *android, int uiDesignWidth, int uiDesignHeight, UIDesignFitMode fitMode)
	{
		LOGI("[Game]:INIT ANDROID");
		GfxMgr::getInstance()->init(android);
		this->_initView(uiDesignWidth, uiDesignHeight, fitMode, android->getWidth(), android->getHeight());
		this->_initModules();
	}

	void Game::_initModules()
	{
		this->_initEvent();
		this->_initInput();
		this->_initFont();
		this->_initAssets();
		this->_initRenderer();
		this->_initAlpha();
	}
	void Game::_initView(int uiDesignWidth, int uiDesignHeight, UIDesignFitMode fitMode, int width, int height)
	{
		LOGI("[Game]:INIT VIEW: %d %d", width, height);
		view = new View(uiDesignWidth, uiDesignHeight, fitMode,width, height);
	}
	void Game::_initEvent()
	{
		LOGI("[Game]:INIT EVENT");
		event = new Event();
	}
	void Game::_initInput()
	{
		LOGI("[Game]:INIT INPUT");
		// input = new Input();
		// input->init();
	}
	void Game::_initFont()
	{
		LOGI("[Game]:INIT FONT MGR");
		// Boo::fontMgr = new FreetypeMgr();
		// Boo::fontMgr->init();
	}
	void Game::_initAssets()
	{
		LOGI("[Game]:INIT ASSETS MGR");
		assetsManager = new AssetsManager();
		assetsManager->init();
	}
	void Game::_initRenderer()
	{
		LOGI("[Game]:INIT RENDERER");
		renderer = new Renderer();
		renderer->init();
	}
	void Game::_initAlpha()
	{
		LOGI("[Game]:INIT ALPHA");
		Alpha *alpha = new Alpha("AlphaScene");
		this->openScene(alpha);
	}
	void Game::resizeView(const int width, const int height)
	{
		if (view->getWidth() == width && view->getHeight() == height)
		{
			return;
		}
		LOGI("[Game]:resize view: %d %d", width, height);
		view->resize(width, height);
		this->_viewChanged = true;
		this->_viewChangedTime = TimeUtil::nowTime();
		GfxMgr::getInstance()->resize(width, height);
	}
	/**
	 * @brief 取消调度
	 * 手动直接取消指定ID的调度
	 * @param scheduleID 调度ID
	 */
	void Game::unschedule(int scheduleID)
	{
		this->_schedules[scheduleID].clearFlag = true;
		this->_scheduleClearCaches.push_back(scheduleID);
	}

	void Game::openScene(Scene *scene)
	{
		// 销毁之前的场景
		this->destroyScene();
		LOGI("[Game]:OPEN SCENE: %s", scene->getName().c_str());
		if (scene == nullptr)
		{
			return;
		}
		this->_curScene = scene;
		this->_curScene->launch();
	}
	void Game::destroyScene()
	{
		if (this->_curScene)
		{
			LOGI("[Game]:destroy scene: %s", this->_curScene->getName().c_str());
			renderer->clearCameras();
			this->_curScene->destroy();
			this->_curScene = nullptr;
		}
	}
	void Game::addCompClearCaches(Component *comp)
	{
		this->_compClearCaches.push_back(comp);
	}
	void Game::addNodeClearCaches(Node *node)
	{
		this->_nodeClearCaches.push_back(node);
	}

	void Game::tick()
	{
		long long deltaTime = TimeUtil::nowTime();
		long long t = deltaTime - this->_deltaTime;
		if (t > (1000.0f / this->_frameRate))
		{
			float dt = t / 1000.0f;
			this->_deltaTime = deltaTime;
			long long start = TimeUtil::nowTime();
			this->_update(dt);
			// LOGI("[Game]:update");
			this->_lateUpdate(dt);
			// LOGI("[Game]:lateUpdate");
			this->_logicTime = TimeUtil::nowTime() - start;
			start = TimeUtil::nowTime();
			this->_render(dt);
			// LOGI("[Game]:render");
			this->_renderTime = TimeUtil::nowTime() - start;
			this->_clear();
		}
	}
	void Game::_update(float dt)
	{
		if (this->_curScene)
		{
			this->_curScene->update(dt);
		}
		this->_updateSchedules(dt);
		if (assetsManager)
		{
			assetsManager->update(dt);
		}
	}
	void Game::_lateUpdate(float dt)
	{
		if (this->_curScene)
		{
			this->_curScene->lateUpdate(dt);
		}
	}
	void Game::_render(float dt)
	{
		// 重置相机渲染纹理大小
		if (this->_viewChanged)
		{
			if (TimeUtil::nowTime() - this->_viewChangedTime >= 100)
			{
				this->_viewChanged = false;
				renderer->updateViewSize();
			}
		}
		// 更新渲染器
		renderer->render(this->_curScene);
		GfxMgr::getInstance()->update(dt);
	}

	void Game::_clear()
	{
		if (this->_curScene)
		{
			this->_curScene->clearNodeFrameFlag();
		}
		this->_updateClearCaches();
	}

	void Game::_updateSchedules(float dt)
	{
		// 待修复
		for (auto it = this->_schedules.begin(); it != this->_schedules.end(); ++it)
		{
			if (it->second.clearFlag)
			{
				// 已经标记清除，跳过
				continue;
			}
			ScheduleInfo &info = it->second;
			if (info.instance == nullptr || info.func == nullptr)
			{
				// 实例或函数为空，标记清除
				info.clearFlag = true;
				this->_scheduleClearCaches.push_back(it->first);
				continue;
			}
			info.time += dt;
			if (info.time >= info.interval)
			{
				info.time = 0.0f;
				info.func();
				bool isOnce = info.isOnce;
				if (isOnce)
				{
					// 一次性调度，清除
					info.clearFlag = true;
					this->_scheduleClearCaches.push_back(it->first);
				}
			}
		}
	}
	void Game::_updateClearCaches()
	{
		// 清除计数器
		for (auto &scheduleId : this->_scheduleClearCaches)
		{
			this->_schedules.erase(scheduleId);
		}
		this->_scheduleClearCaches.clear();

		// 清除组件缓存
		for (auto &comp : this->_compClearCaches)
		{
			// std::cout << "Game::_updateClearCaches: clear comp: " << comp->getNode()->getName() << std::endl;
			if (comp != nullptr)
			{
				delete comp;
			}
			comp = nullptr;
		}
		this->_compClearCaches.clear();
		// 清除节点缓存
		for (auto &node : this->_nodeClearCaches)
		{
			// std::cout << "Game::_updateClearCaches: clear node: " << node << "   " << node->getName() << std::endl;
			if (node != nullptr)
			{
				delete node;
			}
			node = nullptr;
		}
		this->_nodeClearCaches.clear();
	}
	/**
	 * @brief 鼠标按钮事件
	 * @param button 鼠标按钮 0: 左键 1: 右键 2: 中键
	 * @param action 事件动作 GLFW_PRESS: 按下 GLFW_RELEASE: 释放
	 * @param mods 按键修饰符
	 */
	void Game::updateMouseState(int button, int action, int mods)
	{
		// Boo::input->onMouseButton(button, action, mods);
	}
	void Game::updateMousePos(double xpos, double ypos)
	{
		// Boo::input->onCursorPos(xpos, ypos);
	}
	void Game::updateKeyState(int key, int scancode, int action, int mods)
	{
		// Boo::input->onKey(key, scancode, action, mods);
	}

	Game::~Game()
	{
	}

} // namespace Boo