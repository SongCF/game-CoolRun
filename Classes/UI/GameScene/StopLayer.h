//============================================================
//* 暂停层
//* 功能 ：游戏中暂停
//============================================================


#pragma once

#include "cocos2d.h"

using namespace cocos2d;

#define TagName_StopLayer "STOP_LAYER"

class StopLayer : public Layer
{
public:

	enum BlackLayerType
	{
		Type_Start = 1,
		Type_End,
	};

	enum Tag
	{
		Tag_BlackLayer_Up = 1,
		Tag_BlackLayer_Down
	};
	
public:

	CREATE_FUNC(StopLayer);

	virtual bool init();
	void setLastScene(Scene* pScene);

	virtual void onExit();


	static void checkAchievement();
	static void checkFightPowerAchievement();
		
	void showEndBlackLayer();
	void showStartBlackLayer(bool bShowGoBtn);
	void showBlackLayerRemove(const std::function<void(Node*)> &func);

	void showPauseLayer();
	void showFightPlayDeadLayer();
	void showCommonPlayDeadLayer();
	void showTollgateOverLayer(bool bSucceed);
	void removeSelf();
	void showBlackLayer(BlackLayerType type, const std::function<void(Node*)> &func, bool bShowAni, bool bRemove);
	void rebegin(Ref* pNode);
	void onResume(Ref* pObj);
protected:
	void onMainScene(Ref* pObj);
	void reallyResumeGame();
	void showDaoJiShiAnimation();
	
	void updateDeadLayer();
	void showEndlessAccountLayer();
	void showTollgateAccountLayer();
	void showShopLayer();
	void onRebornMakeSure(Ref* obj);
	void update(float dt);
protected:
	Size m_winSize;
	Scene* m_pVisitScene;
};


class WaitingLayer : public Layer
{
public:
	WaitingLayer();
	~WaitingLayer();

	enum WaitingLayer_Type
	{
		WaitingLayer_Type_SiWangChongCi = 1,
	};

	enum iTag
	{
		iTag_MainSpr = 1,
		iTag_ProgressTimerBg,
		iTag_ProgressTimer,
	};

public:

	static WaitingLayer* createWithType(WaitingLayer_Type type);

	bool initWithType(WaitingLayer_Type type);

private:
	WaitingLayer_Type		m_CurWaitingLayer_Type;

};