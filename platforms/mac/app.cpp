#include <iostream>
#include "engine/engine.h"
#include "engine/platforms/window/window.h"
#include "src/launch.h"
#include "engine/boo.h"

int main()
{
    Window *window = new Window();
    window->init();
    Engine *engine = new Engine();
    engine->init(window,1920,1080,Boo::UIDesignFitMode::Height);
    Launch *launch = new Launch();
    launch->init();
    while (window->isRunning())
	{
		window->tick();
		engine->tick();
	}
    delete window;
    window = nullptr;
    delete engine;
    engine = nullptr;
    delete launch;
    launch = nullptr;
    return 0;
}
