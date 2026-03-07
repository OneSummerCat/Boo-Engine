#include "boo.h"
#include <iostream>
namespace Boo
{
	Game *game = nullptr;
	/**
	 * @brief 事件系统
	 */
	Event *event = nullptr;
	/**
	 * @brief 视图系统
	 */
	View *view = nullptr;
	/**
	 * @brief 资产系统
	 */
	AssetsManager *assetsManager = nullptr;
	/**
	 * @brief 渲染系统
	 */
	Renderer *renderer = nullptr;

	// /**
	//  * @brief 字体系统
	//  */
	// FreetypeMgr *fontMgr = nullptr;
	// /**
	//  * @brief 输入系统
	//  */
	// Input *input = nullptr;
	
}