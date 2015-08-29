#pragma once

#include "cocos2d.h"
#include "BaseLayer.h"
#include "PlayerInfoHelper.h"
#include "GoodsHelper.h"
#include "ActorHelper.h"
#include "BaselisttableLayer.h"
#include "StringsMap.h"
#include "ImageHelper.h"
#include <queue>
#include "SimpleAudioEngine.h" 
#include "extensions/cocos-ext.h"
#include "MsgDefine.h"

USING_NS_CC;
USING_NS_CC_EXT; 

enum Acountag{

	 next_ganme_tag,
	 diaowo_tag,
	 color_tag,
	 toptag,
	 domtag,
	 shouqutag,
	 menutag,
	 backtag,

	 
	 star_bg1_tag,
	 star_bg2_tag,
	 star_bg3_tag,
	 facebook_tag,
	 selectlevel_tag,
	 restart_tag,
	 nextlevel_tag,
};

struct AcoundsBag{
	  
	   int id;
	   int value;
	   int weight;

};

class AccountsLayer: public Layer
{
public:
	CREATE_FUNC(AccountsLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
    void inintAccoutsLayer(unsigned int heroid,unsigned int lsmax, unsigned int biaoxianfen,unsigned int distance,unsigned int thegold);
	AccountsLayer(void);
    void allcallback(Ref* ref);
	void updateGold(float time);
	//void checkAchievement();
	void fillOuty(MenuItemImage* p,string imgfile);
	void clearjinlin();
	BagItem getBywarldId(int id);
	AcoundsBag getAcounds(int id,int value,int weight);
	~AccountsLayer(void);
public:

    std::pair<int,vector<AcoundsBag>> getPair(int tag,vector<AcoundsBag> bag);
	vector<std::pair<int,vector<AcoundsBag>>> acounts;
	int goldidx;
	unsigned int gold_numbers;
	unsigned int lsmax;
	unsigned int biaoxianfen;
	unsigned int distance;
	unsigned int thegold;
	string defalut_j;
	int wardId;
	bool isLinward;
	
	Label* gold_one;
	int current_oprate;

	
	BagItem pbag;
	int counts;
	float Actual_x;
	float Actual_y;
};





//挑战关卡结算界面

class TollgateAccountLayer : public Layer
{
public:
	TollgateAccountLayer();
	~TollgateAccountLayer();

	static TollgateAccountLayer* create(unsigned int jewel, TollgateInfor::TollgatePassLevel passLv, TollgateInfor::TollgatePassLevel oldPassLv);

	bool init(unsigned int jewel, TollgateInfor::TollgatePassLevel passLv, TollgateInfor::TollgatePassLevel oldPassLv);
	virtual void onEnter() override;
	virtual void onExit() override;

	void menuCallback(Ref* ref);
	int randStarCount();
	float randScale();
	float randRotation();
	PointArray* randStarPoint(int iCount);
	void showRandStars(Node* pParent);
	void showStarAnimation();
private:
	Sprite* m_pUp;
	TollgateInfor::TollgatePassLevel	m_passLv;
	bool    m_bUp;
};