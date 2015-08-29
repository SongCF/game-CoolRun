#pragma once
#include "cocos2d.h"
#include "BaseLayer.h"
#include "PlayerInfoHelper.h"
#include "ActorHelper.h"
#include "GoodsHelper.h"
#include "BaselisttableLayer.h"
#include "StringsMap.h"
#include "ImageHelper.h"


struct Needunit{

	   int type;
	   int count;
	   string imgfile;

};

struct Addrdata{

	   float score;
	   float gold;
	   float maget;
	   float pwer;

	   bool  protect;
	   bool  rush;

	   Addrdata(){

		   score=0;
		   gold=0;
		   maget=0;
		   pwer=0;

		   protect=false;
		   rush=false;
	   }

};


struct  Netchange
{

	Sprite* back_hero;//不变
	Label* h_name;

	Sprite* suo_img;
	int suo_mark;

	Sprite* need_img;
	int need_mark;

	Label* percent_label;
	int pecent_mark;

    Sprite* kuang_img;
	int kuang_mark;

	ProgressTimer* progress;
	int progress_mark;

	Sprite* dj_img;
	Vector<Sprite*> sps;
	float jg;

	Vector<Label*> labelss;
	Label* f_s[4];
	int f_smark[4];

	Label* cf_s[4];
	int cf_smark[4];

	Label* jiaotou[4];
	int jiaotou_mark[4];

	Label* acf_s[4];
	int acf_smark[4];

	Label* miaoshu_label;
	int miaoshu_mark;

	Label* num_food[3];
	Vector<Layer*>  layers;
	int laysers_mark;

	int wy_mark;
	int mj_mark;

	int armate_mark;

	Vector<Sprite*> msjs;

	

};
USING_NS_CC;
using namespace cocostudio; 


class Netpet: public Layer
{
public:
	Netpet(void);
	~Netpet(void);

	CREATE_FUNC(Netpet);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	float getDelwithint(float number,int iswidth);

	
public:

	//绘图
	void initjiemian();
	void topjiemian();
	void refreshPetImage(Node* node, unsigned int petId);
	void centerjiemian();
	void drawPetlist();
	//gongnneg

	void inintData();
	Addrdata getCurentaddrdata();
	Addrdata getCurentandnextdata();
	Needunit getneedUnit();
    bool isUnlockpet();  //是否已解锁
	bool isMaxlevel(); //是否满级
	//根据id获取当前喂养进度
	int getCujd();
	int getCunum();
	string nextwyname();
	string getAttr(int level);
    
	//callback  函数
	void  wy_callback0(Ref* ref);
	void  wy_callback1(Ref* ref);
	void  wy_callback2(Ref* ref);

	void fh_buttonCallback(Ref* ref);


	void  wy_callback(Ref* ref);
	void  hq_callback(Ref* ref);

	//Vone_2014-12.4 
	void allcallback(Ref* ref);
	void bAction(MenuItemImage* node);
	void callsp();
	
	void toolsdata(EventCustom* evt);
	void drawaddrPet(Sprite* p,float m1,float m2,string name,int mindex);
	void drawfood(string imgfile,Layer* layer);

	void playAnimationByname(Node *node,std::string jsonfile,std::string filename,std::string animationName,bool isloop);

	void fillOuty(MenuItemImage* p,string imgfile);

	void levaDarw();
	void levelChange(int tag=0);
	void changesudrawaddrPet(float mdats,float cdata,string name,int mindex);
	void changedbyId();
	void drawMaojimenu();

private:

	float Actual_x;
	float Actual_y;
	int cId;
	int currentselect;
	//所有宠物
    vector<Actor> pets;
	Actor pet;
	int current_petlevel;
	bool l_is;
	int current_oprate;
	int x;
	int current;
	vector<EventListenerCustom*> netpetsevents;
	

	Netchange netchange_sucess;
};


 class Petlist :public Layer, public TableViewDataSource, public TableViewDelegate
{
public:

	Petlist(void);
	CREATE_FUNC(Petlist);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	
	virtual void scrollViewDidScroll(ScrollView* view) {};
    virtual void scrollViewDidZoom(ScrollView* view) {}

    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
	void drawpets(TableViewCell *cell, ssize_t idx);
	

	~Petlist(void);

	//callback

	void all_callback(Ref* ref);
	void bAc(Node* node);
	void callp(Node* node,bool mr);


private:
     
	TableView *mTableView;
	Size size;
	vector<Actor> pets;
	int current_oprate;
 };
