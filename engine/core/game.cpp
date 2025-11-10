#include "game.h"
#include "gfx/gfx-mgr.h"
#include "global/event.h"
#include "assets/assets-manager.h"

#include "component/component-factory.h"
#include "renderer/ui/ui-sprite.h"
#include "alpha/alpha.h"
// #include "renderer/ui/sprite.h"
// #include "renderer/ui/text.h"
// #include "ui/ui-widget.h"
// #include "ui/ui-tree/node-tree.h"
// #include "ui/ui-tree/file-tree.h"
// #include "ui/ui-mask.h"
Game::Game() : _assetsManager(nullptr),
			   _curScene(nullptr)
{
	this->_view.width = 1280;
	this->_view.height = 720;
}
Game *Game::getInstance()
{
	static Game instance;
	return &instance;
}
void Game::init()
{
	this->_initEvent();
	this->_initInput();
	this->_initFont();
	this->_initComponents();
	this->_initAssets();
	this->_initAlpha();
}

void Game::_initEvent()
{
	this->_event = new Event();
}
void Game::_initInput()
{
	/* InputMgr::getInstance()->init();*/
}
void Game::_initFont()
{
	// FreetypeMgr::getInstance()->init();
}

void Game::_initComponents()
{
	this->_componentFactory = new ComponentFactory();
	this->_componentFactory->registerClass<UISprite>("UISprite");
	/*
	  this->_componentFactory->registerClass<Text>("Text");
	  this->_componentFactory->registerClass<UIWidget>("UIWidget");
	  this->_componentFactory->registerClass<NodeTree>("NodeTree");
	  this->_componentFactory->registerClass<FileTree>("FileTree");
	  this->_componentFactory->registerClass<UIMask>("UIMask");*/
}
void Game::_initAssets()
{
	std::cout << "INIT ASSETS MGR" << std::endl;
	this->_assetsManager = new AssetsManager();
	this->_assetsManager->init();
}
void Game::_initAlpha()
{
	Alpha *alpha = new Alpha("Editor-Alpha");
	this->_curScene = dynamic_cast<Scene *>(alpha);
}

void Game::setView(int width, int height)
{
	this->_view.width = width;
	this->_view.height = height;
}
template <typename T, typename Func>
int Game::schedule(Func func, T *instance, float interval)
{
	int id = this->_scheduleNextID_++;
	this->_schedules[id] = {func, instance, interval, 0.0f, false};
	return id;
}
template <typename T, typename Func>
int Game::scheduleOnce(Func func, T *instance, float interval)
{
	int id = this->_scheduleNextID_++;
	this->_schedules[id] = {func, instance, interval, 0.0f, true};
	return id;
}
void Game::unschedule(int scheduleID)
{
	this->_schedules.erase(scheduleID);
}



void Game::update(float dt)
{
	if (this->_curScene)
	{
		this->_curScene->update(dt);
	}
	// Update schedules
	this->_updateSchedules(dt);
	if (this->_curScene)
	{
		this->_curScene->lateUpdate(dt);
	}
	if (this->_curScene)
	{
		this->_curScene->render();
	}
	if (this->_curScene)
	{
		this->_curScene->clearNodeFrameFlag();
	}
}
void Game::_updateSchedules(float dt)
{
	for (auto it = this->_schedules.begin(); it != this->_schedules.end();)
	{
		ScheduleInfo &info = it->second;
		info.time += dt;
		if (info.time >= info.interval)
		{
			info.time = 0.0f;
			info.func();
			if (info.isOnce)
			{
				it = this->_schedules.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			++it;
		}
	}
}


Game::~Game()
{
}