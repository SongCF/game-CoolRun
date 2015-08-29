#include "CutomTableshop.h"


CutomTableshop::CutomTableshop(void)
{
}


CutomTableshop::~CutomTableshop(void)
{
}



bool CutomTableshop::init(){
	  

	if(!Layer::init()){
		 return false;
	}
	
	return true;

}
void CutomTableshop::onEnter(){

	Layer::onEnter();
}
void CutomTableshop::onExit(){

	Layer::onExit();
}



int CutomTableshop::getDelwithint(int number,int iswidth){

	
	int dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	
	return dealnumber;

}





void CutomTableshop::scrollViewDidScroll(cocos2d::extension::ScrollView* view){



	//在scrollView拖动时响应该函数   
  
  

}
void CutomTableshop::scrollViewDidZoom(cocos2d::extension::ScrollView* view){


}
void CutomTableshop::scrollViewMoveOver(cocos2d::extension::ScrollView* view){


}



void CutomTableshop::Shieldtouched(){

	//事件
	auto listen=EventListenerTouchOneByOne::create();
	listen->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false   
    listen->onTouchBegan = [this](Touch* touch, Event* event){    

		m_touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView()); 
        return true;  
    };     
    listen->onTouchMoved = [this](Touch* touch, Event* event){       
    };     
     
    listen->onTouchEnded = [this](Touch* touch, Event* event){     

		

		Vec2 endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());  
        float distance = endPoint.x - m_touchPoint.x;  
		if (fabs(distance) > 50){  
			Size visibaleSize = Size(getDelwithint(930,0),getDelwithint(470,1));
				

				 if (distance < 0)  
			   {  
				   m_curPage++;  
				}else{  
					m_curPage--;  
				}  
  
 
  
			   if (m_curPage < 1)  
				{  
					m_curPage = 1;  
				}  
  
				if (m_curPage > nodes)  
				{  
					m_curPage = nodes;  
				}   
				
				Vec2 origin = Director::getInstance()->getVisibleOrigin();  
				Vec2 adjustPos =Vec2(origin.x - (m_curPage - 1) * visibaleSize.width,0);  

				
				scrollView->setContentOffset(adjustPos,true);


				for(int i=1;i<=nodes;i++){
				   if(i==m_curPage){
					    sp_vec.at(i-1)->setTexture(Sprite::create("ui_shangcheng_fenye_select.png")->getTexture());
				   }else{
					    sp_vec.at(i-1)->setTexture(Sprite::create("ui_shangcheng_fenye.png")->getTexture());
				   }
	           }
	           
         }  
    };  


	EventDispatcher* dispatch=Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen,this);

	//
	ignoreAnchorPointForPosition(false);
	setAnchorPoint(Vec2(0.5,0.5));
	setPosition(Vec2(Actual_x/2,Actual_y/2));
	setContentSize(Size(Actual_x,Actual_y));
}



void CutomTableshop::drowGold(Layer* node,Vec2 p){



	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	setPosition(p);
	setContentSize(Size(getDelwithint(25,0)*37.4,getDelwithint(21,1)*22.4));// 
	node->addChild(scal9);

	

	auto top=Sprite::create("ui_shangcheng_bg_dinglan.png");
	top->setAnchorPoint(Vec2(0.5,0));
	top->setScaleX((getContentSize().width-getDelwithint(10,1))/top->getContentSize().width);
	top->setPosition(Vec2(getPositionX(),getPositionY()+getContentSize().height/2-top->getContentSize().height-getDelwithint(5,1)));
	node->addChild(top);

	
	auto closeshop=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi_select.png",CC_CALLBACK_1(CutomTableshop::returnPa,this));
	closeshop->setPosition(Vec2(getPositionX()+getContentSize().width/2-closeshop->getContentSize().width/2,getPositionY()+getContentSize().height/2-closeshop->getContentSize().height/2));
	

	Menu* menu=Menu::create(closeshop,nullptr);
	menu->setPosition(Vec2::ZERO);
	node->addChild(menu);
	
	float backx=0.0f;
	float backy=0.0f;

	Size size=Size(getDelwithint(200,0),getDelwithint(350,1));

	for(int i=1;i<=4;i++){
	
		
		auto back=Sprite::create("ui_shangcheng_shangpin_bg_shuoming.png");

		//if(i==1){

			//backx=getDelwithint(25,1)+back->getContentSize().width/2+(i-1)*back->getContentSize().width;
		//}else{
		float spcae= (getContentSize().width-getDelwithint(50,0)-back->getContentSize().width*4)/3;

		backx=getDelwithint(25,0)+back->getContentSize().width/2+(i-1)*back->getContentSize().width+spcae*(i-1);

		//}

		back->setPosition(Vec2(backx,getDelwithint(90,1)+back->getContentSize().height/2));
		addChild(back);

		auto back1=Sprite::create("ui_shangcheng_shangpin_bg.png");
		back1->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height-back1->getContentSize().height/2));
		back->addChild(back1);

		auto Daoju=Sprite::create(StringUtils::format("ui_shangcheng_shangpin_jinbi_%d.png",i));
		Daoju->setPosition(Vec2(back1->getContentSize().width/2,back1->getContentSize().height/2));
		back1->addChild(Daoju);

		auto shuoming=Label::createWithSystemFont(FX::StringsMap::getValue("shopgold1"),"Arial",24,Size(170,0),TextHAlignment::CENTER);
		shuoming->setColor(Color3B(185,21,77));
		shuoming->setPosition(Vec2(size.width/2,(back->getContentSize().height-back1->getContentSize().height))/2);

		back->addChild(shuoming);


		auto button=MenuItemImage::create("ui_anniu_tongyong_goumai.png","ui_anniu_tongyong_goumai_select.png",CC_CALLBACK_1(CutomTableshop::gold_Callback,this));

		button->setPosition(Vec2(back->getPositionX(),size.height-back->getContentSize().height-button->getContentSize().height/2));
		button->setUserData((void*)i);


		string yuan=__String::createWithFormat("%d",(i)*12)->getCString()+FX::StringsMap::getValue("yuan");
		auto buy=Label::createWithSystemFont(yuan,"Arial",30);
		buy->setPosition(Vec2(button->getContentSize().width/2,button->getContentSize().height/2));
		button->addChild(buy);

		auto menu = Menu::create(button,nullptr);
		menu->setPosition(Vec2::ZERO);
		addChild(menu, 1);
	}
}

void CutomTableshop::drowJiewel(Layer* node,Vec2 p){


	 
	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	setPosition(p);
	setContentSize(Size(getDelwithint(25,0)*37.4,getDelwithint(21,1)*22.4));// 
	node->addChild(scal9);

	

	auto top=Sprite::create("ui_shangcheng_bg_dinglan.png");
	top->setAnchorPoint(Vec2(0.5,0));
	top->setScaleX((getContentSize().width-getDelwithint(10,1))/top->getContentSize().width);
	top->setPosition(Vec2(getPositionX(),getPositionY()+getContentSize().height/2-top->getContentSize().height-getDelwithint(5,1)));
	node->addChild(top);

	
	auto closeshop=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi_select.png",CC_CALLBACK_1(CutomTableshop::returnPa,this));
	closeshop->setPosition(Vec2(getPositionX()+getContentSize().width/2-closeshop->getContentSize().width/2,getPositionY()+getContentSize().height/2-closeshop->getContentSize().height/2));
	

	Menu* menu=Menu::create(closeshop,nullptr);
	menu->setPosition(Vec2::ZERO);
	node->addChild(menu);
	
	float backx=0.0f;
	float backy=0.0f;

	Size size=Size(getDelwithint(200,0),getDelwithint(350,1));

	for(int i=1;i<=4;i++){
	
		auto back=Sprite::create("ui_shangcheng_shangpin_bg_shuoming.png");
		float spcae= (getContentSize().width-getDelwithint(50,0)-back->getContentSize().width*4)/3;

		backx=getDelwithint(25,0)+back->getContentSize().width/2+(i-1)*back->getContentSize().width+spcae*(i-1);

		back->setPosition(Vec2(backx,getDelwithint(90,1)+back->getContentSize().height/2));
		addChild(back);

		auto back1=Sprite::create("ui_shangcheng_shangpin_bg.png");
		back1->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height-back1->getContentSize().height/2));
		back->addChild(back1);

		auto Daoju=Sprite::create(StringUtils::format("ui_shangcheng_shangpin_zuanshi_%d.png",i));
		Daoju->setPosition(Vec2(back1->getContentSize().width/2,back1->getContentSize().height/2));
		back1->addChild(Daoju);

		auto shuoming=Label::createWithSystemFont(FX::StringsMap::getValue("shopgold1"),"Arial",24,Size(170,0),TextHAlignment::CENTER);
		shuoming->setColor(Color3B(185,21,77));
		shuoming->setPosition(Vec2(size.width/2,(back->getContentSize().height-back1->getContentSize().height))/2);

		back->addChild(shuoming);


		auto button=MenuItemImage::create("ui_anniu_tongyong_goumai.png","ui_anniu_tongyong_goumai_select.png",CC_CALLBACK_1(CutomTableshop::jiewl_Callback,this));

		button->setPosition(Vec2(back->getPositionX(),size.height-back->getContentSize().height-button->getContentSize().height/2));
		button->setUserData((void*)i);


		string yuan=__String::createWithFormat("%d",(i)*12)->getCString()+FX::StringsMap::getValue("yuan");
		auto buy=Label::createWithSystemFont(yuan,"Arial",30);
		buy->setPosition(Vec2(button->getContentSize().width/2,button->getContentSize().height/2));
		button->addChild(buy);

		auto menu = Menu::create(button,nullptr);
		menu->setPosition(Vec2::ZERO);
		addChild(menu, 1);
	}






}





void CutomTableshop::returnPa(Ref* sender){

	Node *node = Node::create();
	node->setTag(1);
	(m_oTarget->*m_hCallBackHandler)(node);
	 
    EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
	EventCustom event(MSG_MarketClose);
	//MsgData data;
	//data.errorCode = 0;
	//event.setUserData(&data);
	eventDspc->dispatchEvent(&event);

}



void CutomTableshop::gold_Callback(Ref* sender){




}


void CutomTableshop::jiewl_Callback(Ref* sender){


	  

}

 void CutomTableshop::setGold(){

	 drowGold(scroll_layer,Vec2(getDelwithint(930/2,0),getDelwithint(470/2,1)));
	 drowJiewel(scroll_layer,Vec2(getDelwithint(930/2+930,0),getDelwithint(470/2,1)));


 }
 void CutomTableshop::setJiewl(){
	
	 drowJiewel(scroll_layer,Vec2(getDelwithint(930/2,0),getDelwithint(470/2,1)));
	 drowGold(scroll_layer,Vec2(getDelwithint(930/2+930,0),getDelwithint(470/2,1)));
 }



 void CutomTableshop::drowgk_mode(Layer* node,Vec2 p,int id){


	  

}
                             

							                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
 void CutomTableshop::img_Callback(Ref* sender){

	



	
	

 }

 void CutomTableshop::shuoming_Callback(Ref* sender){



	
	

 }



 //已解锁 未通关
  void CutomTableshop::join_Callback(Ref* sender){

	



  }


   void CutomTableshop::again_Callback(Ref* sender){
	    
	 
     
	    

   }


  void CutomTableshop::linqu_Callback(Ref* sender){
	    
	   


		
   }