//============================================================
//* 游戏场景内（一共五层结构）
//* 第五层 ：玩家输入层（键盘事件）
//* 功能 ：玩家输入（按键盘）之后的逻辑响应 ---》 键盘模拟触摸
//============================================================


#pragma once

#include "cocos2d.h"

using namespace cocos2d;


class GameInKeyboardLayer : public Layer
{
public:
	CREATE_FUNC(GameInKeyboardLayer);
	GameInKeyboardLayer();
	bool init();

	virtual void onEnter();
	virtual void onExit();
	void checkIsTouchingLeft(float dt);
	EventListenerKeyboard* m_listener;
};