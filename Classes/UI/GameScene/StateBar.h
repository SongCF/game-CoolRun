//============================================================
//* 游戏场景内（一共五层结构）
//* 第六层 ：玩家状态层
//* 功能 ：展示血量、分值、跑的距离等等
//============================================================


#pragma once

#include "cocos2d.h"
#include "UnitDefine.h"
#include "TollgateHelper.h"
#include <iostream>
#include <vector>
using namespace std;
using namespace cocos2d;


class StateBar : public Layer
{
	
public:

	enum Attr_Tag
	{
		Tag_StartSpeed = 11,
		Tag_StartProtect,
		Tag_DieSpeed,
		Tag_Reborn,
		Tag_ControlBgSpr = 50,
		Tag_ScoreValue = 1000,
		Tag_ScoreValue1,
		Tag_XingSpr = 1001,
		Tag_HPNode = 2000,

		Tag_DaoJu_Protect = 3000,

		Tag_DaoJu_Count_Bg = 4000,
		Tag_DaoJu_Menu,
		Tag_DaoJu_Label,

		Tag_DeadLayer = 5000,
	};
	CREATE_FUNC(StateBar);

	bool init();
	int getProtectTimes(){return m_iUseProtectTimes;}
	int getPauseTimes(){return m_iPauseTimes;}

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float dt) override;

	void onPause(Ref* obj);
	void onProtect(Ref* obj);
	void onResume(Ref* obj);
	void onMainScene(Ref* obj);

	void updateGoldBarValue();
	void startGoldValue(bool refreshValue=true);
	bool getGoldValueEnable(){return m_bStartScoreControl;}
	void addGoldValue(float v){m_fGoldScoreValue += v;}

	void onMsgHeroWin(EventCustom * msg);
	void onMsgHeroDead(EventCustom* msg);

	void tollgateModeEnd(bool arrivalEndPos);
	static TollgateInfor::TollgatePassLevel calculateTollgatePassLevel(int gotJewel, int totalJewel);

	void onRebornMakeSure(Ref* obj);
	void showShopLayer();
	void onBuyResultRsp(EventCustom* msg);
	void showDaoJiShiAnimation();

	void showScoreBar(bool isShow);

	void showHadGoods();

	void startEcffect(Attr_Tag tag);

	void setProtectMenuEnable(bool e);
	void setProtectMenuVisable(bool isVisable);

	ProgressTimer* getScoreProgressTimer() {return m_controlSprite;}
	
	void updateHPIconCount(int iHP);

protected:
	void initDebugMenuBar();
	void initDebugInfo();
	Sprite* createDaoJu(int iTag);
	void setDaoJuMenuEnable(int iTag, bool e);
	void setDaoJuMenuVisable(int iTag, bool isVisable);
protected:
	bool		m_bStartScoreControl;
	LabelAtlas *m_scoreLabel;
//	LabelAtlas *m_meterLabel;

	Size m_winSize;
	float m_fGoldScoreValue;
	ProgressTimer		*m_controlSprite;

	Unit		m_speedUnit;
	Unit		m_dieSpeedUnit;
	vector<MenuItem*>	m_lMenuItemList;

	vector<EventListenerCustom*>				m_lListenerList;

	int  m_iUseProtectTimes;
	int m_iPauseTimes;
};