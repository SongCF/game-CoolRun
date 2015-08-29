#pragma once

#include "cocos2d.h"
#include "BaselisttableLayer.h"
#include "SportsLayer.h"

USING_NS_CC;


class RodeoLayer: public Layer
{
public:

	RodeoLayer(void);
	~RodeoLayer(void);

	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	CREATE_FUNC(RodeoLayer);

	float Actual_x;
	float Actual_y;
	float getDelwithint(float number,int iswidth);
	void ininRodeolayer();

	void Regist_return(int tag);

	void Rankmsg(bool isbd);

public: 

	Scale9Sprite*  scal9;
	//BaselisttableLayer* jj_list;
	//SportsLayer* splayer;
	void loadjiemian();

	void load_jjdata();

	Label* data_ld;
	bool isRegist;
};

