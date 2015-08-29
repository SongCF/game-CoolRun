#pragma once

#include "cocos2d.h"
#include "extensions/cocos-ext.h" 

#include "BaseLayer.h"
#include "string.h"
#include "BaselisttableLayer.h"
#include "HeroSprite.h"

#include "protocol_message.pb.h"
#include "NetmsgManger.h"
#include <iostream> 
#include <cmath>

USING_NS_CC;
USING_NS_CC_EXT; 

class StartLayer :public Layer
{
public:
	
	
	CREATE_FUNC(StartLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	

	int yidian;
	StartLayer(void);
	void inintStartLayer();
	~StartLayer(void);
	
	 void updatetime(float time);
	 //标题动画
	 void biti2action();
	 void randflicker();
	 void Gomain();
	 Sprite* bt;
	void playAnimationByname(Node *node,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscale);

	void playAnimationByname(Node *node,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscale,Vec2 p);
	
	void callfunc_AnimateEnd(Armature *, MovementEventType, const std::string&);

	void callfunc_Nvhai(Armature *, MovementEventType, const std::string&);

	void callfunc_Naihai(Armature *, MovementEventType, const std::string&);
   

	void biaoti_AnimateEnd(Armature *, MovementEventType, const std::string&);
	void anniu_AnimateEnd(Armature *, MovementEventType, const std::string&);
	Sprite* hero;
	Armature* run;

	Armature* lanmeimei;
	Armature* lanbaba;
	Armature* lanbenben;
	Armature *biaoti;
	Armature *anniu;
	void runHero(Node* sender);
	void runHeromyself(Node* sender);
	void updatehero(float time);

	void runManzou(Node* sender);
	void runNvzou(Node* sender);

	void runManzou1(Node* sender);
	void runNvzou1(Node* sender);

	Vec2 p1;
	Vec2 p2;

	Vec2 p3;
	Vec2 p4;

	void Shieldtouched();
	MenuItemImage* button;
	void  startAcitve();

	void naNv(float dt);
	void gegewuChu(float dt);
	void bitaoT(float dt);
	void guangXiao(float dt);
	void anniuChixu(float dt);
	int geTime;

	void csFloat(float dt);
private:
	MenuItemImage* startmenu;
	void startGameCallback(cocos2d::Ref* pSender);
	
	void sgstart(Node* sender);
    float Actual_x;
	float Actual_y;
	int getDelwithint(int number,int iswidth);
public:

	void registCallback(cocos2d::Ref* pSender);
	

	MenuItemImage* regist;
	//
	void setAftername();
	void set_buttonCallback(Ref* ref);
	void hd_buttonCallback(Ref* ref);
	void write_Button(Ref* ref);
	EditBoxTest *write_name;
	void write_nameBox();
	void showShake(Node* sp);
	Scale9Sprite* scal9;
	//
    void  setJiemian_box();
	void Close_button(Ref* ref);
    void Yinyueset_button(Ref* ref);

	
	int na;
	int guanxuiao;
	int gegewu;
	int bitao;
	int aniu;
	bool  shifouyi;
	void Shieldtouched(Node* node);
	void setTextb(MenuItemImage* node,string str,int fontsize,Color3B b);
	bool start_c;
};