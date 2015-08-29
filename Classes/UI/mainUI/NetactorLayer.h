#pragma once

#include "cocos2d.h"
#include "BaseLayer.h"
#include "PlayerInfoHelper.h"
#include "ActorHelper.h"
#include "GoodsHelper.h"
#include "BaselisttableLayer.h"
#include "StringsMap.h"
#include "ImageHelper.h"


USING_NS_CC;

using namespace cocostudio; 

struct Herodata{

	MenuItemImage* sj_p;
};

struct  XiaoactionN{

	Armature* xx_action;
	int xxtime[2];
	bool change;
	int time;

};

class NetactorLayer: public Layer
{
public:

	NetactorLayer(void);
	~NetactorLayer(void);

	CREATE_FUNC(NetactorLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;

	float Actual_x;
	float Actual_y;

	float getDelwithint(float number,int iswidth);
	void refreshHeroImage(Node* node, unsigned int heroId);
    
public:

	//界面初始化  属性界面
	void goshenji();
	void Drawloadboundary();
	void Drawequal();
	void Drawnoequal();
	void dataInint(int id);
	void DrawButtonInfo(int type ,MenuItemImage* p,int count);
	// 升级界面
    
	//bool 函数 是否已选折
	bool isMaxlevel();
	bool isSelect(int id);
	int  getXixi(int id);
	int  getSm(int id);
	int  getMaxsm();
	int  getNextsm();
	int  getSuipian(UnitID ID);
	int  getMaxSuipian();
	bool isUnlockhero(int id);
	string hetheromisoshu(int id);
	string hetheroTese(int id);
	string getHeroGjl(int id);
	string getHeronextGjl(int id);
	string getneedNum();
	string getExsonneedNum();

	int  actorType();
	int  actorType2(int id);
	int  getType2(int id);
	int  getHowtype();
	int  getYuan();

	Vec2 getItemoney();

	bool  getmy_moneyislonh(int id);
	int   getchashenme(int id);

	//按钮效应
	void  level_callback(Ref* ref);
	void  buy_callback(Ref* ref);
	void  select_callback(Ref* ref);
	void  allcallback(Ref* ref);
	void  unlockHeroCallback(Ref* ref);

	//actione
	void bAction(MenuItemImage* node);
	void callsp();
	void callback_sj(EventCustom* evt);

	//数据初始化
	void getAllherosdata();
	void datamanger();
	static void checkShouCangJiaAchievement();
	void drawaddrPet(Sprite* p,float m1,float m2,string name);
	
	
	void playAnimationByname(Node *node,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscale,Vec2 p);
	void callfunc_AnimateEndGk(Armature *ar, MovementEventType movementType, const std::string& movementID);

	void fillOuty(MenuItemImage* p,string imgfile);

public:
	Vector<MenuItem*> images; 
	int oprate_fanxiangtag;
	void setGameTag();
	void levelDraw();
	void levalChange();
	void levalScucss();

	void drawMaojitishi();


private: 

	int cId;
	int currentselect;
	Actor hero;
	vector<SaleItem> heros;
	pair<int,int> gold_suipian;//金币 碎片
	int  islevel;
	int current_oprate;
	vector<EventListenerCustom*> netactorEvents;
	 Sprite* back_hero;
	//XiaoactionN xkongz;

	Label* ggl_label;
	Label* next_ggl;
	vector<Sprite*> djsps;
	vector<Sprite*> helps;

	ProgressTimer* suipian_progress;

	Herodata m_herodata;
 };



 class Herolist :public Layer, public TableViewDataSource, public TableViewDelegate
{
public:

	Herolist(void);
	CREATE_FUNC(Herolist);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	
	virtual void scrollViewDidScroll(ScrollView* view) {};
    virtual void scrollViewDidZoom(ScrollView* view) {}

    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
	void drawheros(TableViewCell *cell, ssize_t idx);
	~Herolist(void);
	//callback
	void all_callback(Ref* ref);
	void bAc(MenuItemImage* node);
	void callp(Node* node,bool mr);

	
private:
     
	TableView *mTableView;
	Size size;
	vector<SaleItem> heros;
	int current_oprate;
	
 };
