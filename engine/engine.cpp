#include "engine.h"
#include "boo.h"
#include "log.h"
#include "platforms/platform.h"
#include "platforms/window/window.h"   //windows下的窗口封装
#include "platforms/android/android.h" //android下的窗口封装
#include "core/game.h"

Engine::Engine()
{
	Boo::game = new Boo::Game();
}
void Engine::init(Window *window,int uiDesignWidth,int uiDesignHeight,Boo::UIDesignFitMode fitMode)
{
	LOGI("[Engine]:INIT WINDOW");
	Boo::game->init(window, uiDesignWidth, uiDesignHeight, fitMode);
}
void Engine::init(Android *android,int uiDesignWidth,int uiDesignHeight,Boo::UIDesignFitMode fitMode)
{
	LOGI("[Engine]:INIT ANDROID");
	Boo::game->init(android, uiDesignWidth, uiDesignHeight, fitMode);
}
/**
 * @brief 引擎更新
 */
void Engine::tick()
{
	if (Boo::game == nullptr)
		return;
	Boo::game->tick();
}

Engine::~Engine()
{
}
