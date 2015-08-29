#pragma once


#include "BaseLayer.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h" 
#include "MsgDefine.h"
#include "TollgateHelper.h"
#include "GameDirector.h"

USING_NS_CC;
USING_NS_CC_EXT;


class CutomTableshop :public Layer,public ScrollViewDelegate  

{
public:
	CutomTableshop(void);
	~CutomTableshop(void);

	float Actual_x;
	float Actual_y;
	
	CREATE_FUNC(CutomTableshop);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	int getDelwithint(int number,int iswidth);


   cocos2d::extension::ScrollView* scrollView;
   void scrollViewDidScroll(cocos2d::extension::ScrollView* view);  
   void scrollViewDidZoom(cocos2d::extension::ScrollView* view);  
   void scrollViewMoveOver(cocos2d::extension::ScrollView* view); 
   void Shieldtouched();

   void drowGold(Layer* node,Vec2 p);
   void drowJiewel(Layer* node,Vec2 p);

   void drowgk_mode(Layer* node,Vec2 p,int idx);

   void returnPa(Ref* sender);

   void gold_Callback(Ref* sender);
   void jiewl_Callback(Ref* sender);

   void setGold();
   void setJiewl();

   //关卡列表
//   vector<TollgateItem> tollgate;

   inline void setCallBackHandler(Ref *obj,SEL_CallFuncO handler)
	{
		m_oTarget = obj;
		m_hCallBackHandler = handler;
	}

   Ref				*m_oTarget;
   SEL_CallFuncO	m_hCallBackHandler;


   void img_Callback(Ref* sender);
   void shuoming_Callback(Ref* sender);
   void join_Callback(Ref* sender);
   void again_Callback(Ref* sender);
   void linqu_Callback(Ref* sender);

   Map<int,MenuItemImage*> liqu_back;
private:
	  Vector<Sprite*> sp_vec;//声明一个容器  
	  Vec2 m_touchPoint;  
      int m_curPage;
	  int nodes;
	  Layer* scroll_layer;
};