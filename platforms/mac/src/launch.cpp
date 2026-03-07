#include "launch.h"
#include "../engine/boo.h"
#include "../engine/log.h"

Launch::Launch() {
}

Launch::~Launch() {
   
}

void Launch::init() {
    // 注册引擎启动事件回调
    Boo::event->on(Boo::EventEngine_Launch, [this]() {
        this->onLaunch();
    });
}

void Launch::onLaunch() {
    LOGI("Launch::onLaunch");
    
}