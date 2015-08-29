#pragma once

#include "cocos2d.h"
#include "BaseLayer.h"
#include "PlayerInfoHelper.h"
#include "ActorHelper.h"
#include "GoodsHelper.h"
#include "BaselisttableLayer.h"
#include "StringsMap.h"
#include "cocostudio/CocoStudio.h"
#include <mutex>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"


USING_NS_CC;



// struct  Xiaoaction{
// 
// 	Armature* xx_action;
// 	int xxtime[2];
// 	bool change;
// 	int time;
// 
// 
// };


class Rolelayer :public Layer
{
public:
	
    Rolelayer();
	CREATE_FUNC(Rolelayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
    void initData();
	float getDelwithint(float number,int iswidth);
	void inintLayerNet();
	void allCallbackm(Ref* ref);
	void go_layer(int tag);
	void drawshoplayer();
	void layerChange();
	void layeReturnpop();
	void spcall(Node* node,bool mr);
	void fillOuty(MenuItemImage* p,string imgfile);
	void callsp(int value=0);
	void bAction(MenuItemImage* node);

	void getActorInfor(unsigned int actorId, string& arExportFile, string& armatureName, string& animationName);
	void getPetInfor(unsigned int petId, string& arExportFile, string& armatureName, string& animationName);
	UnitType getUnitTypeByArmatureName(string armatureName); // 只用于判断  hero  pet  magic 三种
    void playAnimationByname(Node *node,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscale,Vec2 p);
	void callfunc_AnimateEnd(Armature *ar, MovementEventType movementType, const std::string& movementID);
	void callfunc_AnimateEndGk(Armature *ar, MovementEventType movementType, const std::string& movementID);
    
	//回调函数
	 void rigistdaoju();
	~Rolelayer(void);
	void CscallBack(Ref* pSender,Control::EventType event);

	void addNodeevent();
	void Gonengweikaifan();
	void drawLeftlayer();
	void drawCenter();
	//功能
	bool isContactpoint(Vec2 p,Sprite* sp);
	void setcall(Node* node);
	void setcallfaihui(Node* node);
	void layerdaozhi();
	void updateSz(float dt);

	void setUiMove(int tag);
	int  getUimove();
	void Movedone(Node* node);


private:

	int achiveId;
	int current_oprate;
	int isdone;
	float Actual_x;
	float Actual_y;
	int bingbi;

	MenuItemImage* gobutton;
	Armature *ap;
	Actor hero;
	Actor pet;
	Actor magic;
	MenuItemImage* oprate_bt;
	vector<Layer*> jianls;
	vector<MenuItemImage*> menus;
	Layer* shop_layer;
	Layer* left_layer;
	Sprite* rand_ig;
	unsigned int state;
	int numtoch;
	int fahuimark;
	EventListenerCustom* Data_changevent;
    int layertag;
	//Xiaoaction xkongz;

	int whatMove;
	MenuItemImage* zb_menu;
	MenuItemImage* tollgate_zb_menu;
};



