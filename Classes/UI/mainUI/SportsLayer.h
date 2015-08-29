#pragma once

#include "BaseLayer.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h" 
#include "MsgDefine.h"
#include "TollgateHelper.h"
#include "GameDirector.h"
#include <queue>
#include "Uistruct.h"
#include "RankingListHelper.h"

USING_NS_CC;
USING_NS_CC_EXT;






class SportsLayer :public Layer,public ScrollViewDelegate  
{
public:
	SportsLayer(void);
	~SportsLayer(void);

public:


	float Actual_x;
	float Actual_y;
	
	CREATE_FUNC(SportsLayer);



	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	float getDelwithint(float number,int iswidth);

	void loadbasic();
    void loadNet();

   void Vesion(int vesion);

   int vsion;


   cocos2d::extension::ScrollView* scrollView;

   void scrollViewDidScroll(cocos2d::extension::ScrollView* view);  
   void scrollViewDidZoom(cocos2d::extension::ScrollView* view);  
   void scrollViewMoveOver(cocos2d::extension::ScrollView* view); 
   void Shieldtouched();


   void drowFisrt(Layer* node,Vec2 p);
   void drowSecond(Layer* node,Vec2 p);

   void drowone(Layer* node,int num,int index,RankingData data);
  

   void returnPa(Ref* sender);
  

   //关卡列表
  

   inline void setCallBackHandler(Ref *obj,SEL_CallFuncO handler)
	{
		m_oTarget = obj;
		m_hCallBackHandler = handler;
	}

   Ref				*m_oTarget;
   SEL_CallFuncO	m_hCallBackHandler;

   /*
   void img_Callback(Ref* sender);
  */
   Sprite* jt1;
   Sprite* jt2;
   void loadNetdata();
   void loadJiemian(bool isnet);
   Label* text_ts;
   
   //获取我的组名
   string getMy_name();
   
   Map<int,Label*> m_l;
   Label* p_name;
   vector<RBonusInfo>  bounds;
   void isregist();
   bool isloadData;


   void setPtion_xy(Vec2 v2);
   Label* b_label;

   Label* name;
   Layer* l_one;
   void get_myZm(float time);
   void drawMyself();

   void Czdata();
   int mark;
 private:
	  Vector<Sprite*> sp_vec;//声明一个容器  
	  Vec2 m_touchPoint;  
      int m_curPage;
	  int nodes;
	  Layer* scroll_layer;
	  Label* rla;

	  

};

