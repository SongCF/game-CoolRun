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


struct MymagicStruct
{

	MenuItemImage* sj_p;
};

struct LeavelSucess{

	Sprite* cr_djimg; //当前等级图片
	Sprite* nextcr_djimg; //下一等级图片
	Vector<Sprite*> levelAction;
	Label* current_mlabel; //当前魔力
	Label* add_label;  //增加得魔力
	Label* miaoshu_label;//魔法物描述
	Sprite* magic_djimg;
	Sprite* ml_djimg;
	float jg;
	int add_mark;
};


class Magicup: public Layer
{
public:
	Magicup(void);
	~Magicup(void);

    CREATE_FUNC(Magicup);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	float getDelwithint(float number,int iswidth);

	void setmagicId(int id);
	void inintJiemian();

	void drawSx(Node* node,int dj);

	int getpj(int dj);
	int getNextpj(int dj);

	float getmfggl();
	float getnextmfggl();

	string getCurrentleveimg();
    string getNextleveimg();
	bool isMaxlevel();

	void callback_sj(EventCustom* evt);
	Vec2 getItemoney();

	//
	int getMjyuan();
	int getsjyuan();
	//button 回调
	void yjmjCallback(Ref* ref);
	void sjCallback(Ref* ref);
	void allcallback(Ref* ref);
	void bAction(MenuItemImage* node);
	void callsp();
	int layerGame;
	void setGameFahui();

	void playAnimationByname(Node *node,std::string jsonfile,std::string filename,std::string animationName,bool isloop);
	void fillOuty(MenuItemImage* p,string imgfile);

	void levelDraw();

	void levelChangSuccess();
	void drawMaojitishi();
private:

	float Actual_x;
	float Actual_y;
	Actor magic;
	int magicid;
	int current_oprate;
	EventListenerCustom* sj;
	Sprite* back_hero;

	LeavelSucess magicDescrible;
	MymagicStruct levael_str;
};

