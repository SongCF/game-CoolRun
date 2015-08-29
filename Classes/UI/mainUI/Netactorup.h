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

class Netactorup:public Layer
{
public:
	Netactorup(void);
	~Netactorup(void);


	

	CREATE_FUNC(Netactorup);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	float getDelwithint(float number,int iswidth);


public:
	//
	void setactorId(int id);
	void loadData();
	//
	void inintjiemian();
	void drawSx(Node* node,int dj);
	//功能函数
	int getMjyuan();
	int getsjyuan();
	void callback_sj(EventCustom* evt);

	int getxj(int dj);
	int getsm(int dj);
	string getjc(int dj);

	//button 回调
	void yjmjCallback(Ref* ref);
	void sjCallback(Ref* ref);


private:

	float Actual_x;
	float Actual_y;
	int actorid;
	Actor hero;
	int current_herolevel;
	EventListenerCustom* sj;
};

