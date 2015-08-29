#pragma once
#include "cocos2d.h"
#include "BaseLayer.h"
#include "protocol_message.pb.h"
USING_NS_CC;

class TopLayer :public Layer
{
public:
	CREATE_FUNC(TopLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	TopLayer(void);
	~TopLayer(void);

	Ref				*m_oTarget;
	SEL_CallFuncO	m_hCallBackHandler;

	inline void setCallBackHandler(Ref *obj,SEL_CallFuncO handler)
	{
		m_oTarget = obj;
		m_hCallBackHandler = handler;
	}
public:

	
	void inintLyaer();
	LabelAtlas *gold_label;
	LabelAtlas *diamoud_label;
	MenuItemImage* yinyue_button;
	bool music;
	void goldCallback(cocos2d::Ref* pSender);
	void zsCallback(cocos2d::Ref* pSender);
	bool isGk;
	void setIsgk();

	void setpBtsfase(int tag=0);
	void setpBtstrue(int tag=0);
	//net
	void Regist_return(int tag);
	//set 设置活跃可领取
	Sprite* hys;
	Sprite* youe;
	void setEmail(bool ys);
	void Conset();
	void Emailtx();
	void setAchive();
	void setPactive();
    
	void setAllfalse();
	void setAlltrue();
	void fillOuty(MenuItemImage* p,string imgfile);
private:
	//设置
	
	
	void netCallback(Ref* sender);
	//成就
	void Cjiu_menu(Ref* ref);
	void JjPm_button(Ref* ref);

    float Actual_x;
	float Actual_y;
	int getDelwithint(int number,int iswidth);
	float getDelwithint(float number,int iswidth);

	Vector<MenuItemImage*> pbts;
	void setTextb(MenuItemImage* node,string str,int fontsize,Color3B b);

	/*
	ClippingNode* holesClipper; //裁剪节点
    Node* holesStencil;         //模板节点
    Node* holes;//底板节点
	Sprite * m;
	*/

	int mark_source;
	int oprate_mark;

	MenuItemImage* addgold;
	MenuItemImage* addzs;

	ProgressTimer* progress;

	EventListenerCustom* mm_top;

    vector<Vec2> mtrues;
	vector<Vec2> mfals;
	
	vector<Vec2> m_lefttrues;
	vector<Vec2> m_leftfals;
};

