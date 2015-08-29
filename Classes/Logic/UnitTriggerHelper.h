//============================================================
//* 触发器管理类
//* 配置的触发器
//============================================================

#pragma once

#include "cocos2d.h"
#include "TriggerConfigManager.h"

enum configTriggerID
{
	TriggerId_BossMake = 7,  //这里只定义了一个，只有这个程序里需要用到判断。
};



class UnitTriggerHelper
{
private:
// 	UnitTriggerHelper();
// 	static UnitTriggerHelper* m_sTriggerHelper;
public:
//	static UnitTriggerHelper* getHelper();
	static const Trigger* getOneTrigger(unsigned int triggerId);
	static void runTriggerActionById(cocos2d::Node* target, unsigned int trigger);
};