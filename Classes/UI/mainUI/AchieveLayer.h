#pragma once

#include "BaseLayer.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h" 
#include "MsgDefine.h"
#include "TollgateHelper.h"
#include "GameDirector.h"

USING_NS_CC;
USING_NS_CC_EXT;


class AchieveLayer :public Layer,public ScrollViewDelegate
{
public:
	AchieveLayer(void);
	~AchieveLayer(void);

	float Actual_x;
	float Actual_y;
	
	CREATE_FUNC(AchieveLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	float getDelwithint(float number,int iswidth);

   cocos2d::extension::ScrollView* scrollView;
   void scrollViewDidScroll(cocos2d::extension::ScrollView* view);  
   void scrollViewDidZoom(cocos2d::extension::ScrollView* view);  
   void scrollViewMoveOver(cocos2d::extension::ScrollView* view); 
   void Shieldtouched();

   vector<Achieve> acives;
   void drowgk_mode(Layer* node,Vec2 p,int idx);
   Map<int,Menu*> ac_button;
   void linqu_Callback(Ref* ref);

   //回调函数
   void Lin_Ward();
   int Lin_Id;
   void  Wangcheng();

   void allcallback(Ref* ref);
   void bAction(MenuItemImage* node);
   void callsp(Node* node,bool mr);
   void warld(const Achieve* iv);
   void spcall(Node* node,bool mr);

   
  


private:
	  Vector<Sprite*> sp_vec;//声明一个容器  
	  Vec2 m_touchPoint;  
      int m_curPage;
	  int nodes;
	  Layer* scroll_layer;
	  float back_w;
	  float back_h;
	  Sprite* shop_back;
	  int current_oprate;
	  int aciveid;
	  Map<int,Sprite*> ac_back_change;
};

