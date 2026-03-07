#include "android.h"
#include "../../boo.h"
#include "../../core/game.h"
Android::Android()
{
    this->_width = 0;
    this->_height = 0;
}
#if defined(BOO_PLATFORM_ANDROID)
Android::Android(ANativeWindow *g_nativeWindow, int width, int height)
{
    this->_nativeWindow = g_nativeWindow;
    this->_width = width;
    this->_height = height;
}
#endif
void Android::resize(int width, int height)
{
    this->_width = width;
    this->_height = height;
    Boo::game->resizeView(this->_width, this->_height);
}
void Android::tick()
{
  
}
Android::~Android()
{
}
