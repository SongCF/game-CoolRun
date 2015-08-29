#pragma once

#include "BaseLayer.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "MsgDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ShopLayer :public Layer//,public ScrollViewDelegate  
{
public:
	ShopLayer(void);
	~ShopLayer(void);

public:
	float Actual_x;
	float Actual_y;
	
	CREATE_FUNC(ShopLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	float getDelwithint(float number,int iswidth);

   
   cocos2d::extension::ScrollView* scrollView;
   /*
   void scrollViewDidScroll(cocos2d::extension::ScrollView* view);  
   void scrollViewDidZoom(cocos2d::extension::ScrollView* view);  
   void scrollViewMoveOver(cocos2d::extension::ScrollView* view); 
   */
   void Shieldtouched();

   void drowGold(Layer* node,Vec2 p);
   void drowJiewel(Layer* node,Vec2 p);
   void drowSiwu(Layer* node,Vec2 p);

   void returnPa(Ref* sender);

   void gold_Callback(Ref* sender);
   void jiewl_Callback(Ref* sender);

   void setGold();
   void setJiewl();
   void setSw();

   void setTextb(MenuItemImage* node,string str,int fontsize,Color3B b);
   //call back
   void buttonSw(Ref* ref);
   void buttonMy(Ref* ref);

   //gongneng
   void setWeizhi(int wz);

   void setCloseCallback(std::function<void()> callback){m_pClocseCallback = callback;};

   inline void setCallBackHandler(Ref *obj,SEL_CallFuncO handler)
	{
		m_oTarget = obj;
		m_hCallBackHandler = handler;
	}

   Ref				*m_oTarget;
   SEL_CallFuncO	m_hCallBackHandler;

   bool issw;
//v-one draw

   void dataInint();
   void drawShop();


   	void allcallback(Ref* ref);
	void bAction(MenuItemImage* node);
	void callsp(Node* node,bool mr);
	void drawMoney(int tag,vector<SaleItem> shop_moneys);
	void drawFood();
	void setChanged(int id);
	void fillOuty(MenuItemImage* p,string imgfile);

private:
	  
	  const vector<SaleItem>* shops;
	  vector<SaleItem> shop_moneys;
	  vector<SaleItem> shop_foods;

	  Vector<Sprite*> sp_vec;//声明一个容器  
	  Vec2 m_touchPoint;  
      int m_curPage;
	  Layer* scroll_layer;
	  int nodes;
	  int cubjid;
	  int current_oprate;
	  Sprite* shop_back;
	  SaleItem* saleitem;
	  MenuItemImage* huob_b;
	  MenuItemImage* fd_b;

	  EventListenerCustom* buy_listener;
	  stack<BagItem> sales;


	  std::function<void()>		m_pClocseCallback;

};

