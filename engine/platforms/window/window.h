#pragma once
#include <iostream>
#include <functional>
#include <mutex>
#include <atomic>

#include "../platform.h"

#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
#include <GLFW/glfw3.h>
#endif
class Window
{
private:
	int _width;
	int _height;

private:
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
	GLFWwindow *_window = nullptr;
#endif

public:
	Window();
	void init();
	const int getWidth()
	{
		return this->_width;
	}
	const int getHeight()
	{
		return this->_height;
	}
	~Window();

public:
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
	GLFWwindow *getWindow();
	static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void windowSizeCallback(GLFWwindow *window, int width, int height);
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

	void onCursorPos(double xpos, double ypos);
	void onMouseButton(int button, int action, int mods);
	void onKey(int key, int scancode, int action, int mods);
	void onWindowSize();
	bool isRunning();
	void tick();
	void destroy();
#endif
};
