#pragma once
#include "cocos2d.h"
#include "BaseLayer.h"
#include "BaselisttableLayer.h"
USING_NS_CC;
class Toolayer: public Layer
{
public:
	CREATE_FUNC(Toolayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	Toolayer(void);
	~Toolayer(void);
	BaselisttableLayer* list;
public:
	void inintLayer();
	//返回主页面
	void return_Main(Ref* sender);
	
 float Actual_x;
	float Actual_y;
	int getDelwithint(int number,int iswidth);
};

