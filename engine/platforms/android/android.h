#pragma once
#include "../platform.h"
#if defined(BOO_PLATFORM_ANDROID)
#include <android/native_window.h>
#endif

class Android
{
private:
    int _width;
    int _height;

private:
#if defined(BOO_PLATFORM_ANDROID)
    ANativeWindow *_nativeWindow = nullptr;
#endif
public:
    Android();
#if defined(BOO_PLATFORM_ANDROID)
    Android(ANativeWindow *g_nativeWindow, int width, int height);
    ANativeWindow *getNativeWindow()
    {
        return this->_nativeWindow;
    }
#endif
    void init();
    const int getWidth()
    {
        return this->_width;
    }
    const int getHeight()
    {
        return this->_height;
    }
    void resize(int width, int height);
    void tick();
    ~Android();
};
