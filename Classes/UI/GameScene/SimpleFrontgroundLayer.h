//============================================================
//* 游戏场景内（一共五层结构）
//* 第七层 ：路面前的层
//* 功能 ：简单滚动 
//============================================================


#pragma once

#include "cocos2d.h"
#include "BaseSprite.h"
#include "ScrollForeverNode.h"
#include "GameSceneConfigManager.h"
using namespace cocos2d;


class SimpleFrontgroundLayer : public Layer
{
public:

	enum _ScrollNodeTag
	{
		iTag_ScrollNode1 = 10,
		iTag_ScrollNode2,
		iTag_ScrollNode3,

		iTag_PiaoFuWuNodeStart = 1000, 

	};



	SimpleFrontgroundLayer();
	virtual ~SimpleFrontgroundLayer();

	virtual bool init();

	CREATE_FUNC(SimpleFrontgroundLayer);


	void refreshBackGround();
	void setScrollEnable(bool e);
	ScrollForeverNode* getBGLayer_JinJing();//近
	ScrollForeverNode* getBGLayer_ZhongJing();//中
	ScrollForeverNode* getBGLayer_YuanJing();//远

protected:
	void refreshBackGround(EventCustom* pMsg, bool bFisrt = false);
	void updateBackGround(EventCustom* pMsg);
	void update(float dt);
private:

	Size					m_Winsize;	
	int						m_iRunTime;

	BackgroundList*			m_pInfo; 

	vector<Sprite*>			m_vJinJingZhuangShiWu;
	EventListenerCustom* m_refreshListener;
};