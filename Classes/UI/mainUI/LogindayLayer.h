#pragma once

#include "BaseLayer.h"

#include "Uistruct.h"





class LogindayLayer:public Layer
{
public:
	LogindayLayer(void);
	~LogindayLayer(void);

	CREATE_FUNC(LogindayLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
      
	float Actual_x;
	float Actual_y;
	bool isRegist;

	void Regist_return(int tag);
public:

	Label* text;
	void jiemian();
	void loadInterphase(bool bl);
	float getDelwithint(float number,int iswidth);

	Scale9Sprite* scal9;
	//请求数据

	vector<Warld> wards;
	int warld_Id;
	Warld  idw;
	bool iskd;
	Map<int,Sprite*> w_l;
	int wl_id;

	void getWarldCallback(cocos2d::Ref* pSender);

	void Msgcallback();

	void cs_login(float dt);
private:
	
	int currentrsp;// 0 为请求登陆奖励 1为 发送领取奖励
	//Sprite * spp;
};

