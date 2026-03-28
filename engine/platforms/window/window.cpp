
#include "window.h"
#include "../../boo.h"
#include "../../core/game.h"

const int MIN_WIDTH = 640;
const int MIN_HEIGHT = 360;

Window::Window()
{
	this->_width = 0;
	this->_height = 0;
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
	this->_window = nullptr;
#endif
}

void Window::init()
{
	this->_initAssetsRoot();
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
	if (!glfwInit())
	{
		return;
	}
	// 获取显示器的视频模式
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	// 获取显示器的视频模式
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	float width = mode->width * 0.7f;
	width = width < MIN_WIDTH ? MIN_WIDTH : width;
	float height = mode->height * 0.7f;
	height = height < MIN_HEIGHT ? MIN_HEIGHT : height;

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // 窗口不可调整大小
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	const int C_GLFW_TRANSPARENT_FRAMEBUFFER = 131082;
	glfwWindowHint(C_GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

	// 创建窗口
	this->_window = glfwCreateWindow(width, height, "", nullptr, nullptr);
	if (!this->_window)
	{
		glfwTerminate();
		return;
	}
	glfwSetWindowSizeLimits(this->_window, MIN_WIDTH, MIN_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowUserPointer(this->_window, this);
	glfwSetCursorPosCallback(this->_window, Window::cursorPosCallback);
	glfwSetMouseButtonCallback(this->_window, Window::mouseButtonCallback);
	glfwSetKeyCallback(this->_window, Window::keyCallback);
	glfwSetWindowSizeCallback(this->_window, Window::windowSizeCallback);
	this->onWindowSize();
#endif
}
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
GLFWwindow *Window::getWindow()
{
	return this->_window;
}
void Window::_initAssetsRoot()
{
#if defined(BOO_PLATFORM_WINDOWS)
	//SetConsoleOutputCP(CP_UTF8);
	//SetConsoleCP(CP_UTF8); // 也设置输入编码为 UTF-8，保持统一

	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);
	this->_assetsRoot = std::filesystem::path(exePath).parent_path().string();
	std::cout << "assets path:" << this->_assetsRoot << std::endl;
#else defined(BOO_PLATFORM_MACOS)
	uint32_t size = 0;
	_NSGetExecutablePath(nullptr, &size);
	std::vector<char> buffer(size);
	if (_NSGetExecutablePath(buffer.data(), &size) == 0)
	{
		try
		{
			std::string appPath = std::string(buffer.data());
			this->_assetsRoot = std::filesystem::path(appPath).parent_path().string();
			std::cout << "assets path:" << this->_assetsRoot << std::endl;
		}
		catch (const std::filesystem::filesystem_error &ex)
		{
			std::cerr << "file system error: " << ex.what() << std::endl;
		}
	}
#endif
}

void Window::cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
	void *userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr)
	{
		return;
	}
	Window *manager = static_cast<Window *>(userPointer);
	if (manager == nullptr)
	{
		return;
	}
	manager->onCursorPos(xpos, ypos);
}
void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	void *userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr)
	{
		return;
	}
	Window *manager = static_cast<Window *>(userPointer);
	if (manager == nullptr)
	{
		return;
	}
	manager->onMouseButton(button, action, mods);
}
void Window::windowSizeCallback(GLFWwindow *window, int width, int height)
{
	void *userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr)
	{
		return;
	}
	Window *manager = static_cast<Window *>(userPointer);
	if (manager == nullptr) // 检查非空
	{
		return;
	}
	manager->onWindowSize();
};
void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	void *userPointer = glfwGetWindowUserPointer(window);
	if (userPointer == nullptr)
	{
		return;
	}
	Window *manager = static_cast<Window *>(userPointer);
	if (manager == nullptr) // 检查非空
	{
		return;
	}
	manager->onKey(key, scancode, action, mods);
}

void Window::onCursorPos(double xpos, double ypos)
{
	if (Boo::game == nullptr)
		return;
	// 通知引擎更新鼠标位置
	Boo::game->updateMousePos(xpos, ypos);
};
void Window::onMouseButton(int button, int action, int mods)
{
	if (Boo::game == nullptr)
		return;
	// 通知引擎更新鼠标状态
	Boo::game->updateMouseState(button, action, mods);
};
void Window::onKey(int key, int scancode, int action, int mods)
{
	if (Boo::game == nullptr)
		return;
	// 通知引擎更新键盘状态
	Boo::game->updateKeyState(key, scancode, action, mods);
}
void Window::onWindowSize()
{
	glfwGetWindowSize(this->_window, &this->_width, &this->_height);
	// std::cout << "onWindowSize: width:" << this->_width << " height:" << this->_height << std::endl;
	if (Boo::game == nullptr)
		return;
	Boo::game->resizeView(this->_width, this->_height);
};
bool Window::isRunning()
{
	return !glfwWindowShouldClose(this->_window);
}
void Window::tick()
{
	glfwPollEvents();
}

void Window::destroy()
{
	glfwDestroyWindow(this->_window);
	glfwTerminate();
}
#endif
Window::~Window()
{
}
