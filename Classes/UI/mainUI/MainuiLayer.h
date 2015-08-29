#pragma once
#include "cocos2d.h"
#include "BaseLayer.h"
#include "TopLayer.h"
#include <stack>
#include <iostream>
#include <queue>
#include "TopLayer.h"
#include "Rolelayer.h"
#include "Toolayer.h"
#include "MsgDefine.h"
#include "GoodsHelper.h"
#include "StartLayer.h"
#include "CutomTableshop.h"
#include "LogindayLayer.h"

using namespace std;
USING_NS_CC;

class MainuiLayer :public Layer
{
public:
	
	CREATE_FUNC(MainuiLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;

	stack<int> lp;
	MainuiLayer(void);
	void setTopLayer(TopLayer* p);
	void setCenterLayer(Layer* p,int idx);
	Rolelayer* getCenterLayer();
	void gotoTollgateLayer();
	void backFromTollgateLayer();
	
	void removeLayer(int tag);
	void setTophight(float tophight);

	void setGold_label();
	void setdiamoud_label();
	void layerChange(LayerData data);
	~MainuiLayer(void);

	void acived();
	bool isroot;
	void joinGk();

	void joinHero();
	void joinMagic();

	void dynamic_active();

	void  setJiemian_box();
	
	Scale9Sprite* scal9;
	void showShake(Node* sp);
	void Close_button(Ref* ref);
    void Yinyueset_button(Ref* ref);
	void Shieldtouched();

	void spcall(Node* node,bool mr);
	

	void setNodesfalse();
	void setNodestrue();
	
public:

	float tophight;
	TopLayer* top;
	Layer* center;
	Layer* m_tollgateLayer;

	void allCallback(Ref* sender);
	void returnCallback(Ref* sender);
	
	void pushLayer(int p);

	Ref				*m_oTarget;
	SEL_CallFuncO	m_hCallBackHandler;

	inline void setCallBackHandler(Ref *obj,SEL_CallFuncO handler)
	{
		m_oTarget = obj;
		m_hCallBackHandler = handler;
	}
		


    

private:
	void Registeventalls();
	int getDelwithint(int number,int iswidth);
	float Actual_x;
	float Actual_y;

	vector<EventListenerCustom*> events;
};

