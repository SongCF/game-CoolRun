#pragma once

#include "cocos2d.h"
#include "BaseLayer.h"
#include "PlayerInfoHelper.h"
#include "ActorHelper.h"
#include "GoodsHelper.h"
#include "BaselisttableLayer.h"
#include "StringsMap.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

using namespace cocostudio; 

class SetoutLayer: public Layer
{
public:
	SetoutLayer(void);
	~SetoutLayer(void);

	CREATE_FUNC(SetoutLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	float getDelwithint(float number,int iswidth);
public:

	//界面初始化
	void setoutInit();
	void Actorinint();	 
	void Drawinterface();

	//动画播发器
	void playAnimationByname(Node *node,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscale,Vec2 p);
	void callfunc_AnimateEnd(Armature *ar, MovementEventType movementType, const std::string& movementID);

	//按钮触发器
	void juesebuttonCallback(Ref* ref);
	void cw_buttonCallback(Ref* ref);
	void gj_buttonCallback(Ref* ref);

	void  wj_buttonCallback(Ref* ref);

	//xiaoxihuidiang
	void rigistdaoju();
	void randxiaog(unsigned int tag);
	


private:


	float Actual_x;
	float Actual_y;

	Actor hero;
	Actor pet;
	BagItem* magic;
	
	BaselisttableLayer* list;
	Scale9Sprite* scal9;
	Sprite* sp;


	Armature*		m_pArmature;
	string			m_armatureFileName;
	unsigned int tag_unit;
};

