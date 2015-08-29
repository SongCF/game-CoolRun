//============================================================
//* 动画管理类
//* 配置的动画
//============================================================

#pragma once

#include "cocos2d.h"
#include "TriggerConfigManager.h"

class UnitActionHelper
{
private:
// 	UnitActionHelper();
// 	static UnitActionHelper* m_sUnitHelper;
public:
//	static UnitActionHelper* getHelper();
	static void runActionByActionId(cocos2d::Node* target,  unsigned int actionID, const Trigger* trigger = nullptr);
	static void runActionByTrigger(cocos2d::Node* target, const Trigger* trigger);
	//	static void runActionByTriggerId(cocos2d::Node* target, unsigned int triggerId);
};