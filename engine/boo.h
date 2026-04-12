#pragma once
#include <chrono>
// 包含核心系统头文件
#include "core/game.h"
#include "core/event/event.h"
#include "core/view/view.h"
#include "core/assets/assets-manager.h"
#include "core/renderer/renderer.h"
#include "core/renderer/camera.h"
#include "core/profiler/profiler.h"

// 包含场景管理头文件
#include "core/scene/scene-api.h"

// 包含数学头文件
#include "core/math/math-api.h"

// 包含组件头文件
#include "core/component/component.h"
#include "core/component/component-register.h"

// 包含渲染头文件
#include "core/renderer/renderer-api.h"

// 包含工具头文件
#include "core/util/util-api.h"



namespace Boo
{
	enum class UIDesignFitMode
    {
		/**
		 * 无适配(宽度和高度都不变,常用于全屏游戏)
		 */
        None,
        /**
         * 宽适配(宽度不变，高度按比例缩放,超出部分裁剪,常用于竖屏游戏)
         */
        Width,
        /**
         * 高适配(高度不变，宽度按比例缩放,超出部分裁剪,常用于横屏游戏)
         */
        Height,
    };
	extern Game *game;
	/**
	 * @brief 事件系统
	 */
	extern Event *event;
	/**
	 * @brief 视图系统
	 */
	extern View *view;
	/**
	 * @brief 资产系统
	 */
	extern AssetsManager *assetsManager;
	/**
	 * @brief 渲染系统
	 */
	extern Renderer *renderer;
	/**
	 * @brief 性能分析系统
	 */
	extern Profiler *profiler;

	// /**
	//  * @brief 字体系统
	//  */
	// extern FreetypeMgr *fontMgr;
	// /**
	//  * @brief 输入系统
	//  */
	// extern Input *input;
}