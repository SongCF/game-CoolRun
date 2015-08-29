#include "TopLayer.h"
#include "GoodsHelper.h"
#include "ShopLayer.h"
#include "ActiveValueHelper.h"
#include "BasicLayer.h"


#define  k_bcakimg "UI_zhuye_jinbilan_bg.png"
#define  gold_img "UI_zhuye_jinbilan_jinbi.png"
#define  jiewel_img "UI_zhuye_jinbilan_zuanshi.png"
#define  buy_img "UI_zhuye_jinbilan_anniu_jia.png"
#define  number_img "UI_shuzi_01.png"
#define  cj_img "UI_zhuye_anniu_chengjiu.png"
#define  ph_img "UI_zhuye_anniu_paihang.png"
#define  ui_tisizs "UI_zhuye_tishi_zuoshang.png"
#define  ui_tiao  "UI_zhuye_anniu_chengjiu_jindutiao.png"

enum{
	msgwork=900,
	activework,
	loginwork,
};

void TopLayer::setIsgk(){
	isGk=true;
	
}


bool TopLayer::init(){

	if(!Layer::init()){
		return false;
	}
	isGk=false;
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	mark_source=0;
	oprate_mark=0;
	inintLyaer();
	return true;
}
void TopLayer::onEnter(){

    auto callBack     =  [&](EventCustom* evt)  
                            {  
							   MsgData* data=(MsgData*)evt->getUserData();
							   if(data->value==1){//不可见
								   setpBtsfase(1);
							   }else if(data->value==2){
								   setpBtstrue(1);
							   }
                            };  
	mm_top=EventListenerCustom::create(Topmsg_ch,callBack);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mm_top,1); 
	Layer::onEnter();
}
void TopLayer::onExit(){


	Director::getInstance()->getEventDispatcher()->removeEventListener(mm_top);
	Layer::onExit();
}
TopLayer::TopLayer(void)
{
	
}

TopLayer::~TopLayer(void)
{
}


void TopLayer::inintLyaer(){


	this->setContentSize(Size(Actual_x,Tophight*(Actual_y/Design_hight)));
	
	auto goldimg=MenuItemImage::create(k_bcakimg,k_bcakimg,CC_CALLBACK_1(TopLayer::goldCallback,this));
	goldimg->setPosition(Vec2(goldimg->getContentSize().width/2+5,getContentSize().height*0.6));
	goldimg->setTag(100);

	
	//金币
	auto gold_img1=Sprite::create(gold_img);
	gold_img1->setPosition(Vec2(gold_img1->getContentSize().width/2+getDelwithint(5,0),goldimg->getContentSize().height/2-getDelwithint(3,1)));
	goldimg->addChild(gold_img1,1,0);
    

	__String *gold_strnum=__String::createWithFormat("%u",GoodsHelper::getHelper()->getGoldNum());
	gold_label=LabelAtlas::create(gold_strnum->getCString(),number_img,15, 20,'0'); // Label::createWithSystemFont(gold_strnum->getCString(), "Arial", 24);
	gold_label->setAnchorPoint(Vec2(0.5f,0.5f));
	gold_label->setPosition(Vec2(goldimg->getContentSize().width*0.6,goldimg->getContentSize().height*0.5));
	goldimg->addChild(gold_label,1,Gold_labeltag);

	

	addgold=MenuItemImage::create(buy_img,buy_img,CC_CALLBACK_1(TopLayer::goldCallback,this));
	addgold->setPosition(Vec2(goldimg->getPositionX()+goldimg->getContentSize().width/2+getDelwithint(5,0),goldimg->getPositionY()));
	addgold->setLocalZOrder(200);

	fillOuty(addgold,buy_img);
	



	auto zsimg=MenuItemImage::create(k_bcakimg,k_bcakimg,CC_CALLBACK_1(TopLayer::zsCallback,this));
	zsimg->setPosition(Vec2(goldimg->getPositionX()+goldimg->getContentSize().width/2+50+zsimg->getContentSize().width/2,goldimg->getPositionY()+5));
	zsimg->setTag(200);

	auto dimaoud_img=Sprite::create(jiewel_img);
	dimaoud_img->setPosition(Vec2(dimaoud_img->getContentSize().width/2+getDelwithint(5,0),zsimg->getContentSize().height/2-getDelwithint(5,1)));
	zsimg->addChild(dimaoud_img,1,2);
    	
	addzs=MenuItemImage::create(buy_img,buy_img,CC_CALLBACK_1(TopLayer::zsCallback,this));
	addzs->setPosition(Vec2(zsimg->getPositionX()+ zsimg->getContentSize().width/2+getDelwithint(5,0),goldimg->getPositionY()));
	addzs->setLocalZOrder(200);

	fillOuty(addzs,buy_img);

	__String *dimaoud_strnum=__String::createWithFormat("%u",GoodsHelper::getHelper()->getDiamondNum());
	diamoud_label=LabelAtlas::create(dimaoud_strnum->getCString(),number_img,15, 20,'0');//Label::createWithSystemFont(dimaoud_strnum->getCString(), "Arial", 24);
	diamoud_label->setAnchorPoint(Vec2(0.5f,0.5f));
	diamoud_label->setPosition(Vec2(zsimg->getContentSize().width*0.6,zsimg->getContentSize().height*0.5));
	zsimg->addChild(diamoud_label,1,Jewel_labeltag);
	
	//成就 排行
	auto cjbt=MenuItemImage::create(cj_img,cj_img,CC_CALLBACK_1(TopLayer::Cjiu_menu,this));
	cjbt->setPosition(Vec2(getContentSize().width*0.65,getContentSize().height*0.3));
	pbts.pushBack(cjbt);
    mtrues.push_back(cjbt->getPosition());
	mfals.push_back(Vec2(cjbt->getPositionX(),cjbt->getPositionY()+200));
	fillOuty(cjbt,cj_img);


	youe=Sprite::create(ui_tisizs);
	youe->setPosition(Vec2(10,cjbt->getContentSize().height-30));
	cjbt->addChild(youe,100);
	youe->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5,0.8f),
		                                 ScaleTo::create(0.5,1.0f),nullptr
		                                 )));
	setAchive();
	

	 progress = ProgressTimer::create(Sprite::create("UI_zhuye_anniu_chengjiu_jindutiao.png"));
	 progress->setType(ProgressTimer::Type::BAR);
	 progress->setAnchorPoint(Vec2(0.5,0));
	 progress->setMidpoint(Vec2(0,0));
	 progress->setBarChangeRate(Vec2(1, 0));
	 progress->setPosition(Vec2(cjbt->getContentSize().width/2+20,12));
	 cjbt->addChild( progress,100);
	 setPactive();

	

	auto jjpm_menu=MenuItemImage::create(ph_img,ph_img,CC_CALLBACK_1(TopLayer::JjPm_button,this));
	jjpm_menu->setPosition(Vec2(cjbt->getPositionX()+cjbt->getContentSize().width/2+20+jjpm_menu->getContentSize().width/2,getContentSize().height*0.3));
	pbts.pushBack(jjpm_menu);
	mtrues.push_back(jjpm_menu->getPosition());
	mfals.push_back(Vec2(jjpm_menu->getPositionX(),jjpm_menu->getPositionY()+200));
	fillOuty(jjpm_menu,ph_img);


	auto text_sz=Label::createWithSystemFont("--","minijiankatong.ttf", 26);
	text_sz->setColor(Color3B(184,69,13));
	text_sz->setPosition(Vec2(jjpm_menu->getContentSize().width/2+20,20));
	jjpm_menu->addChild(text_sz);
	
	auto menu = Menu::create(cjbt,jjpm_menu,nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	auto menu1 = Menu::create(goldimg,zsimg,addgold,addzs,nullptr);
    menu1->setPosition(Vec2::ZERO);
	menu1->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(menu1, 1);
	menu1->setTag(125);

	m_lefttrues.push_back(Vec2(0,0));
	m_leftfals.push_back(Vec2(0,200));
	

}


void TopLayer::fillOuty(MenuItemImage* p,string imgfile){


	auto s=Sprite::create(imgfile);
	s->setScale(1.1f);
	p->setSelectedImage(s);

}

void TopLayer::setPactive(){
	 
	 float need_Experience=AchievementHelper::getHelper()->getAllAchievement()->size();
	 float f_value=0;//
	 vector<Achieve> chives=*AchievementHelper::getHelper()->getAllAchievement();
	 for(size_t i=0;i<chives.size();i++){
			if(AchievementHelper::getHelper()->getAchieveByID(chives.at(i).ID)->isOver==true){
				f_value++;
			}
	 }
	 float x=f_value/need_Experience*100;
	 progress->setPercentage(x);
}

void TopLayer::setAchive(){

	const vector<Achieve> list=*AchievementHelper::getHelper()->getAllAchievement();
	bool ishave=true;
   
	for (size_t i=0;i<list.size();i++)
	{
		
		if(AchievementHelper::getHelper()->getAchieveByID(list.at(i).ID)->isOver&&AchievementHelper::getHelper()->getIsGotAchievement(list.at(i).ID)==false){
			ishave=false;
			break;
		}
	}
	if(ishave==false){
		youe->setVisible(true);
	}else{
		youe->setVisible(false);
	}
}

void TopLayer::setEmail(bool ys){

	if(ys==false){
	   youe->setVisible(false);
	}else{
	   youe->setVisible(true);
	}

}


void TopLayer::Conset(){

	  float q_x=(float)ActiveValueHelper::getHelper()->getHasActiveCount();
	  if(q_x<100){
		  hys->setVisible(false);
	  }else{
		  hys->setVisible(true);
	  }
}

void TopLayer::Cjiu_menu(Ref* ref){

	

	FX::Music::getInstance()->buttonClicked();
	
	int tag=9;

	EventCustom event(Layerchange);
	LayerData data;
	data.current=tag;
	data.push=0;
	event.setUserData(&data);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);


	//9
	
}



//竞技排名
void TopLayer::JjPm_button(Ref* ref){
    
	FX::Music::getInstance()->buttonClicked();
    auto p=BasicLayer::create();
	p->Cannokaifang();
	Director::getInstance()->getRunningScene()->addChild(p);
	//10
}

void TopLayer::netCallback(Ref* sender){

	MenuItemImage* p=(MenuItemImage*)sender;
	 int tag=p->getTag();

	 if(tag==msgwork){
		 
		 FX::Music::getInstance()->buttonClicked(); 
		 EventCustom event(Layerchange);
				 LayerData data;
				 data.current=8;
				 data.push=0;
				 event.setUserData(&data);
				 Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
	 }else if(tag==activework){
         
		 if(RandomGiftBagHelper::getHelper()->getGiftBagTable().size()==0){
			 NetmsgManger::getNetManager()->sendUpdateLibao();
		 }
		 auto layer=BaseLayer::create();
		 layer->risk_box();
		 Director::getInstance()->getRunningScene()->addChild(layer);

	 }else if(tag==loginwork){

		FX::Music::getInstance()->buttonClicked();
		EventCustom event(Layerchange);
				 LayerData data;
				 data.current=7;
				 data.push=0;
				 event.setUserData(&data);
				 Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
	 }
}



void TopLayer::goldCallback(cocos2d::Ref* pSender){

	FX::Music::getInstance()->buttonClicked();
	auto shop=BaseLayer::create();
	shop->goldbox();
	Director::getInstance()->getRunningScene()->addChild(shop,2000);
 }
void TopLayer::zsCallback(cocos2d::Ref* pSender){
   
	FX::Music::getInstance()->buttonClicked();
	auto shop=BaseLayer::create();
	shop->jiewelbox();
    Director::getInstance()->getRunningScene()->addChild(shop,2000);
	
	

}
float TopLayer::getDelwithint(float number,int iswidth){

	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;
}


int TopLayer::getDelwithint(int number,int iswidth){


	int dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;
}


void TopLayer::setpBtsfase(int tag){

	 for(int i=0;i<pbts.size();i++){

		     auto move = MoveTo::create(0.25f,mfals.at(i));    
			 auto move_ease_inout0 = EaseExponentialOut::create(move);
			 pbts.at(i)->runAction(move_ease_inout0);
	}

	

}
void TopLayer::setpBtstrue(int tag){

	 for(int i=0;i<pbts.size();i++){

		 
		   auto move = MoveTo::create(0.25f,mtrues.at(i));    
	       auto move_ease_inout0 = EaseExponentialOut::create(move);
		   pbts.at(i)->runAction(move_ease_inout0);
	}
	
}


void TopLayer::setTextb(MenuItemImage* node,string str,int fontsize,Color3B b){

	auto label=Label::createWithSystemFont(FX::StringsMap::getValue(str),"Arial",fontsize);
	label->setPosition(Vec2(node->getContentSize().width/2,node->getContentSize().height/2));
	label->setColor(b);
	node->addChild(label);

}








void TopLayer::setAllfalse(){
 
	 auto move = MoveTo::create(0.25f,m_leftfals.at(0));    
	 auto move_ease_inout0 = EaseExponentialOut::create(move);
	 this->getChildByTag(125)->runAction(move_ease_inout0);
}
void TopLayer::setAlltrue(){
	 auto move = MoveTo::create(0.25f,m_lefttrues.at(0));    
	 auto move_ease_inout0 = EaseExponentialOut::create(move);
	 this->getChildByTag(125)->runAction(move_ease_inout0);
}


