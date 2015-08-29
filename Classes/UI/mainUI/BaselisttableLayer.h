#pragma once


#include "BaseLayer.h"
#include "cocos2d.h" 
#include "SimpleAudioEngine.h" 
#include <queue>
#include "AchievementHelper.h"
#include "extensions/cocos-ext.h"
#include "GoodsHelper.h"
#include "MarketHelper.h"
#include "ActorHelper.h"
#include "MsgDefine.h"
#include "TollgateHelper.h"
#include "Uistruct.h"
#include "EmailHelper.h"
USING_NS_CC;
USING_NS_CC_EXT; 

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;


class BaselisttableLayer :public Layer, public TableViewDataSource, public TableViewDelegate
{
public:
	
	CREATE_FUNC(BaselisttableLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	BaselisttableLayer(void);
	~BaselisttableLayer(void);
	void setDataType(int type);
	void achive_Callback(Ref* sender);
	void shop_Callback(Ref* sender);

	
public:
	
	
	virtual void scrollViewDidScroll(ScrollView* view) {};
    virtual void scrollViewDidZoom(ScrollView* view) {}

    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
	
	int type;
	int layerspace;
	bool isVetical;

	
	TableView *mTableView;

	Size size;
    int itemnum;
	void setMtabletype(int type,Size size,int itemnum,ScrollView::Direction direction,TableView::VerticalFillOrder order,int space);// space 是二项之间的间隔
	void addDataimg(TableViewCell *cell, ssize_t idx);

	void toolsale_Callback(Ref* sender);
	void toolsdata(EventCustom* evt);


	void linqu_Callback(Ref* sender); //

	void juese_Callback(Ref* sender); //升级
	void juese_Callback1(Ref* sender);// 购买

	void buy_Callback(Ref* sender);
	void shengji_Callback(Ref* sender);

	void gongju_Callback(Ref* sender);
	vector<SaleItem> tools;
	vector<Achieve> acives;
	//关卡列表
    //vector<TollgateItem> tollgate;


	Map<int,Menu*> ac_button;
	void aciveChange(int id);

	void playAnimationByname(Node *node,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscale);
	void playAnimationBynamenode(Node *node,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscale,Vec2 v2,int zorder);

	void callfunc_AnimateEnd(Armature *, MovementEventType, const std::string&);

	
private:


	
	//道具，成就，商城
	
    Map<int,Label*> p_label;
	Map<int,Sprite*> p_labelsp;
	Map<int,Sprite*> p_chagesp;
	Map<int,Label*> p_clabel;
	int cp_lable;
	int sale_id;
	
	vector<SaleItem> sales;


	//角色,宠物，魔法物
	vector<SaleItem> heros;
	MenuItemImage* hero_button;
	int hero_ID;
	Map<int,Sprite*> hero_back;
	Map<int,Sprite*> hero_select;
	

	vector<SaleItem> pets;
	MenuItemImage* pet_button;
	int buy_id;
	int pet_ID;
	int pet_seletid;
	Map<int,Sprite*> pet_back;
	Map<int,Sprite*> pet_select;
	Map<int,Sprite*> petmoney_back;
	Map<int,MenuItemImage*> button_back;

	vector<SaleItem> magis;
	int magic_ID;
	int magic_selectd;
	Map<int,Sprite*> magic_back;
	Map<int,Sprite*> magic_select;
	Map<int,Sprite*> magicmoney_back;
	MenuItemImage* pbutton;
	Map<int,MenuItemImage*> mg_back;


	void drow_rolelist(TableViewCell *cell, ssize_t idx);
	void drow_chongwulist(TableViewCell *cell, ssize_t idx);
	void drow_gongjugwulist(TableViewCell *cell, ssize_t idx);


	//道具，成就，商城
	void drow_Toolist(TableViewCell *cell, ssize_t idx);


	void drow_Acivelist(TableViewCell *cell, ssize_t idx);
	void drow_Shoplist(TableViewCell *cell, ssize_t idx);
	void drow_tolllist(TableViewCell *cell, ssize_t idx);

	void updatetime(float time);

	float getDelwithint(int number,int iswidth);
//	void checkShouCangJiaAchievement();
	float Actual_x;
	float Actual_y;

	
public:

	// net 排行榜
	
	void drow_Sportslist(TableViewCell *cell, ssize_t idx);
	int net_tag;
	void setTextb(MenuItemImage* node,string str,int fontsize,Color3B b);
	int upname;
	int timeupname;
	void RankMsg();
	vector<RankUser> netSports;
	NetPalyermsg msgNet;
	vector<RBonusInfo>  bounds;


	Label* load;
	void modify_callback(Ref* ref);

	//消息 msg
	vector<Email> eamils;
	vector<MailInfo_layer> mails;
	void drow_msg(TableViewCell *cell, ssize_t idx);
	void setMails_list(vector<MailInfo_layer> &mails);
	void ckWarld(Ref* ref);
	void rmoveeamil(int id);

	Email getEmailid(int id);

	
	Map<int,Sprite*> yidus;
	queue<int> yd_ques;

	Label* myname;
	
	void laName(float dt);

	string name_u;
	void spcall(Node* node,bool mr);
private:
	 EventListenerCustom* buy_lr;
	 int listentag;
};

