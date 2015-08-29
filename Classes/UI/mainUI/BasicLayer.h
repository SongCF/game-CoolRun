#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h" 
#include "string.h"
#include "AchievementHelper.h"
#include "MarketHelper.h"
#include "FuncTools.h"
#include "PlayerInfoHelper.h"
#include "ActorHelper.h"
#include "GoodsHelper.h"
#include "BaselisttableLayer.h"
#include "StringsMap.h"
#include "ImageHelper.h"
#include "Music.h"
#include "cocostudio/CocoStudio.h"
#include "NetmsgManger.h"
#include "EditBoxTest.h"
#include "BaseLayer.h"
#include "Uistruct.h"
#include "ID_Define.h"
#include "ActiveValueHelper.h"

USING_NS_CC;
USING_NS_CC_EXT; 


#define Design_width 960
#define Design_hight 640




class BasicLayer:public Layer
{
public:

	BasicLayer(void);
	~BasicLayer(void);
	CREATE_FUNC(BasicLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
public:
	float Actual_x;
	float Actual_y;
	float getDelwithint(float number,int iswidth);
	BaselisttableLayer* plist;
	//活跃框奖励
	void active_warldbox(BagItem unit);
	void Shieldtouched();
	void showShake(Node* sp);
	void daoju_reward(Ref* sender);
	void linqu_active(Ref* ref);
	void maojitishi(int tag);
	void Cannokaifang(string title = "",string desc="");
	void shuoming(string sm);
	void cancel_callback(Ref* ref);
	void Close_callback(Ref* ref);
	void Chankanyoujian(Email* ifo,BaselisttableLayer* p);
	void closelinquW(Ref* ref);
	void linquW(Ref* ref);
	void setTextb(MenuItemImage* node,string str,int fontsize,Color3B b);
	

	Scale9Sprite* ww;
	BagItem bag_l;
	Color4B color; 
	ProgressTimer* progress;
	Label* b_l;
	int eamilid;
	SpriteBatchNode* nodes;
	Vector<Sprite*> sps;
public:

	void wardBox(int id);
	void linWard(Ref* ref);
	float fgRangeRand( float min, float m );
	void chankan(Node* node);
	void buySuccecs(BagItem item,string str="goumaicg");
	void buystr(string tishi,string desc="temp");

	LayerColor *colorLayer2;
	int wdId;
};
