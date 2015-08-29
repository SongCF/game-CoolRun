#include "MainuiLayer.h"
#include "AchievementHelper.h"
#include "MsgnetLayer.h"
#include "AchieveLayer.h"
#include "RodeoLayer.h"
#include "SetoutLayer.h"
#include "NetactorLayer.h"
#include "Netactorup.h"
#include "Magicup.h"
#include "ActiveValueHelper.h"
#include "Netpet.h"
#include "AccountsLayer.h"
#include "TollgateLayer.h"

enum {

	root=0,//
	hero=1,
	herosj=2,
	pet=3,
	petsj=4,
	magic=5,
	magicsj=6,
	gkms=7,
	loginday=8,
	acivelayer=9,
	rodeoLayer=10,
	setoutact=11,
	netactor=12,
	netup=13,
	netmagicup=14,
	backtag_img,
};

#define Taglayer 100

#define main_backimg "UI_sence_zhuye_l.png"

bool MainuiLayer::init(){

	if(!Layer::init()){
		return false;
	}
	//背景图片
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	lp.push(0);

	auto back=Sprite::create(main_backimg);
	back->setAnchorPoint(Vec2(0,0.5));
	back->setPosition(Vec2(0,Actual_y/2));
	back->setTag(backtag_img);
	addChild(back);

	top=TopLayer::create();
	setTopLayer(top);
	top->setLocalZOrder(10);
	

	//默认为 角色信息显示页面
	center=Rolelayer::create();
	setCenterLayer(center,0);
	isroot=true;
	dynamic_active();
	Registeventalls();
	return true;

}


void MainuiLayer::onEnter(){
	//注册事件

	Shieldtouched();
	Layer::onEnter();
}
void MainuiLayer::onExit(){
    

	for(auto m:events){
		Director::getInstance()->getEventDispatcher()->removeEventListener(m);
	}
    Layer::onExit();
}



MainuiLayer::MainuiLayer(void)
	:m_tollgateLayer(nullptr)
{
	
}


MainuiLayer::~MainuiLayer(void)
{   
	if (m_tollgateLayer) m_tollgateLayer->release();
	
}


void MainuiLayer::setTophight(float tophight){

	this->tophight=tophight;

}

void MainuiLayer::setTopLayer(TopLayer* p){

	top->setCallBackHandler(this,SEL_CallFuncO(&MainuiLayer::returnCallback));
	addChild(top,2);
	top->setPosition(Vec2(0,Actual_y-Tophight*(Actual_y/Design_hight)));
}
void MainuiLayer::setCenterLayer(Layer* p,int idx){

	 
	Vec2 p2=Vec2(Actual_x,0);
	if(rand()%2==0){
		p2.x=-Actual_x;
	}

	
	addChild(p);
	p->setTag(Taglayer);
	p->setPosition(p2);
	p->setLocalZOrder(100);
	auto move = MoveTo::create(0.5f, Vec2(0,0));    
	auto move_ease_inout1 = EaseExponentialOut::create(move);
		//out xiaoshi in xian xian
	Vector<Node *> pa= p->getChildren();
	for(auto m:pa){
			m->runAction(FadeIn::create(0.15f));
	}
	auto swap=Spawn::create(move_ease_inout1,FadeIn::create(0.15f),nullptr);
	auto seq=Sequence::create(swap,CallFuncN::create(CC_CALLBACK_0(MainuiLayer::spcall,this,p,true)),nullptr);
	p->runAction(seq);
}

Rolelayer* MainuiLayer::getCenterLayer()
{
	return dynamic_cast<Rolelayer*>(this->getChildByTag(Taglayer));
}





void MainuiLayer::setGold_label(){

	if(top!=nullptr)
	   top->gold_label->setString(String::createWithFormat("%d",GoodsHelper::getHelper()->getGoldNum())->getCString());
}


void MainuiLayer::setdiamoud_label(){
	 
	
	if(top!=nullptr)
	   top->diamoud_label->setString(String::createWithFormat("%d",GoodsHelper::getHelper()->getDiamondNum())->getCString());
 
}  



void MainuiLayer::allCallback(Ref* sender){


	MenuItemImage *node=(MenuItemImage*)sender;

	
	


}






void MainuiLayer::layerChange(LayerData data){


	removeLayer(data.current);
	if(data.current!=0){
		isroot=false;
		top->isGk=false;
	}else{
	}
	
	top->setpBtsfase();
	top->setAllfalse();

	top->setAchive();
	top->setPactive();
	top->setAlltrue();

	if(data.current==0){

		
		top->setpBtstrue();
		
		Rolelayer* ly=Rolelayer::create();
		setCenterLayer(ly,data.current);
		isroot=true;

		
		
	}else if(data.current==5){
	
		
		Toolayer* layer=Toolayer::create();
		setCenterLayer(layer,data.current);
		
	}else if(data.current==6){

		
		CutomTableshop* layer=CutomTableshop::create();
		layer->ignoreAnchorPointForPosition(false);
	    layer->setAnchorPoint(Vec2(0.5f,0.5f));
	    layer->setPosition(Vec2(Actual_x/2,Actual_y/2-getDelwithint(30,1)));
		addChild(layer,1,Taglayer);
		 
	}else if(data.current==7){
		
		LogindayLayer* layer=LogindayLayer::create();
		setCenterLayer(layer,data.current);
	}else if(data.current==8){
		
		MsgnetLayer* layer=MsgnetLayer::create();
		setCenterLayer(layer,data.current);
	}else if(data.current==9){
		top->setpBtstrue();
		
		setNodesfalse();
		
		AchieveLayer* layer=AchieveLayer::create();
		layer->ignoreAnchorPointForPosition(false);
		layer->setPosition(Vec2(0,0));
		Director::getInstance()->getRunningScene()->addChild(layer);
	}else if(data.current==10){
		
		RodeoLayer* layer=RodeoLayer::create();
		setCenterLayer(layer,data.current);
	}else if(data.current==11){
		
		 SetoutLayer* layer=SetoutLayer::create();
         setCenterLayer(layer,data.current);
	}else if(data.current==12){
		
		 auto layer=NetactorLayer::create();
         setCenterLayer(layer,data.current);
	}else if(data.current==13){
		
		 auto layer=Netactorup::create();
		 layer->setactorId(data.heroid);
         setCenterLayer(layer,data.current);
	}else if(data.current==14){
		
		auto layer=Magicup::create();
		layer->setmagicId(PlayerInfoHelper::getHelper()->getDeafaultMagicItem());
        setCenterLayer(layer,data.current);
	}else if(data.current==15){
		
		auto layer=Netpet::create();
        setCenterLayer(layer,data.current);
	}

	
	
}


void MainuiLayer::pushLayer(int p){

	
	if(lp.empty()){
	  lp.push(p);
	}else{

		if(lp.top()==p){

		}else{
			lp.push(p);
		}
	}

}


void MainuiLayer::Registeventalls(){


	 auto callBack_layerc      =  [&](EventCustom* evt)  
                            {  
								MsgData* data=(MsgData*)evt->getUserData();
								if(data->value==12){
									joinHero();
								}else if(data->value==14){
                                    joinMagic();
								}
                            };  
	 auto  layer_changeventgame=EventListenerCustom::create(UI_LayerChanged,callBack_layerc);
	 Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(layer_changeventgame,1);  
	 events.push_back(layer_changeventgame);

	 auto callBack       =  [&](EventCustom* evt)  
                            {  
								MsgData* data=(MsgData*)evt->getUserData();
								this->setGold_label();
								this->setdiamoud_label();
                            };  
	auto Gold_changevent=EventListenerCustom::create(MSG_BagDataChanged,callBack);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(Gold_changevent,1);  
	events.push_back(Gold_changevent);


	auto LayercallBack       =  [&](EventCustom* evt)  
                            {  
								LayerData data=*(LayerData*)evt->getUserData();
								if(data.current==100){
									setJiemian_box();
								}else if(data.current==101){
									Node* p=(Node*)this->getChildByTag(backtag_img);
									if(data.push==-1){
										auto move = MoveTo::create(0.5f, Vec2(p->getPositionX()-(p->getContentSize().width-Actual_x),p->getPositionY()));    
			                            auto move_ease_inout0 = EaseExponentialOut::create(move);
										p->runAction(move_ease_inout0);
									}else if(data.push==1){
										auto move = MoveTo::create(0.5f, Vec2(p->getPositionX()+(p->getContentSize().width-Actual_x),p->getPositionY()));    
			                            auto move_ease_inout0 = EaseExponentialOut::create(move);
										p->runAction(move_ease_inout0);										
									}
								}else if(data.current==102){ //人物返回界面
									returnCallback(nullptr);
								}else if(data.current==103){ //宠物返回界面
                                    returnCallback(nullptr);
								}else if(data.current==104){ //魔殿返回界面
                                    returnCallback(nullptr);
								}else if(data.current==105){// 成就界面
									setNodestrue();
									top->setpBtstrue();
								}else if(data.current==Layertag::gold_tag){
									setNodesfalse();
								}else if(data.current==Layertag::gold_tag_close){
									setNodestrue();
								}else if(data.current==Layertag::zs_tag){
									setNodesfalse();
								}else if(data.current==Layertag::zs_tag_close){
									setNodestrue();
								}else{
									 this->layerChange(data);
									 this->pushLayer(data.push);
								}
                            };  
	auto layer_changevent=EventListenerCustom::create(Layerchange,LayercallBack);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(layer_changevent,1);  
	events.push_back(layer_changevent);


	auto toTollgatelayer=EventListenerCustom::create(MSG_TollgateLayer_comein,[=](EventCustom* evt){
		this->gotoTollgateLayer();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(toTollgatelayer,1);  
	events.push_back(toTollgatelayer);
	auto backfromTollgatelayer=EventListenerCustom::create(MSG_TollgateLayer_Close,[=](EventCustom* evt){
		this->backFromTollgateLayer();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(backfromTollgatelayer,1);  
	events.push_back(backfromTollgatelayer);
}


void MainuiLayer::returnCallback(Ref* sender){

	
	top->setAchive();
	LayerData data;
	if(isroot==false){
		if(lp.size()>0){
		
			if(lp.size()==1){
				unsigned  int p=(unsigned int)lp.top();
				  data.current=p;
		 		  layerChange(data);
			}else{
				unsigned  int p=(unsigned int)lp.top();
				data.current=p;
				layerChange(data);
				lp.pop();

			}

		}
	
	}else{

		 BaseLayer* shop=BaseLayer::create();
	     shop->closeGame();
	     Director::getInstance()->getRunningScene()->addChild(shop); 
	}

}


void MainuiLayer::removeLayer(int tag){

	if(tag==9){
		return;
	}
	if(getChildByTag(Taglayer)!=nullptr){
        removeChildByTag(Taglayer,true);
	}
}

void MainuiLayer::spcall(Node* node,bool mr){
     
	 //node->removeFromParent();
	top->setAlltrue();

}


void MainuiLayer::setNodesfalse(){

	Vector<Node*> nodes=this->getChildren();
	for(auto m:nodes){
		if(m->getTag()!=backtag_img)
		   m->setVisible(false);
	}

}

void MainuiLayer::setNodestrue(){

	top->setAchive();
	Vector<Node*> nodes=this->getChildren();
	for(auto m:nodes){
		m->setVisible(true);
	}
	top->setAlltrue();
}


int MainuiLayer::getDelwithint(int number,int iswidth){

	int dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;
}


void MainuiLayer::acived(){

	 
	vector<unsigned int> list;
	AchievementHelper::getHelper()->getNewAchievementList(list);

	//for(int i=0;i<list.size();i++){

	  BaseLayer* layer=BaseLayer::create();
	  layer->Shieldtouched();
	  layer->Achievementpromptbox(10001);//list.at(10001));
	  layer->setAnchorPoint(Vec2(0.5,0.5));

	  addChild(layer);

	//}



}


void MainuiLayer::dynamic_active(){


}

void MainuiLayer::joinGk(){
	 
	//removeLayer();
    LayerData data;
	data.current=6;
	data.push=0;
	this->layerChange(data);
    this->pushLayer(data.push);

}

void MainuiLayer::joinHero(){

	LayerData data;
	data.current=12;
	data.push=0;
    this->pushLayer(data.push);

	removeLayer(data.current);
	if(data.current!=0){
		isroot=false;
		top->isGk=false;
	
	}else{
	
	}
	
	top->setpBtsfase();
	top->setAchive();
	top->setPactive();

	
    auto layer=NetactorLayer::create();
	layer->goshenji();
    setCenterLayer(layer,data.current);
}

void MainuiLayer::joinMagic(){

	LayerData data;
	data.current=14;
	data.push=0;
    this->pushLayer(data.push);

	removeLayer(data.current);
	if(data.current!=0){
		isroot=false;
		top->isGk=false;
	}else{
	}
	
	top->setpBtsfase();
	top->setAchive();
	top->setPactive();

	
	auto layer=Magicup::create();
	layer->setmagicId(PlayerInfoHelper::getHelper()->getDeafaultMagicItem());
	layer->setGameFahui();
    setCenterLayer(layer,data.current);

}

void MainuiLayer::Shieldtouched(){

	 //事件
	auto listen=EventListenerTouchOneByOne::create();
	listen->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 //默认为false   
    listen->onTouchBegan = [](Touch* touch, Event* event){    
        return true;    
    };     

	EventDispatcher* dispatch=Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen,this);


}

void MainuiLayer::setJiemian_box(){
	 
	auto layer=BaseLayer::create();
	layer->drawSetmusic();
	Director::getInstance()->getRunningScene()->addChild(layer);
	setNodesfalse();
}

void MainuiLayer::Yinyueset_button(Ref* ref){

	
}


void MainuiLayer::showShake(Node* sp){

	 sp->setScale(0.2f);
	 sp->runAction(Sequence::create(
		FadeOut::create(0),
		Spawn::create(FadeIn::create(0.05f),
		ScaleTo::create(0.1f,1.05f),
		nullptr),
		
		ScaleTo::create(0.1f,1.0f),
		nullptr));


}




void MainuiLayer::Close_button(Ref* ref){

	this->removeChildByTag(777);
	
}


void MainuiLayer::gotoTollgateLayer()
{
	Size winSize = Director::getInstance()->getWinSize();

	top->runAction(Sequence::create(
		MoveTo::create(0.25f, Vec2(top->getPositionX(), winSize.height+top->getContentSize().height)),
		nullptr));

	Rolelayer* roleLayer = getCenterLayer();
	if (roleLayer) {
		roleLayer->layerChange();
		roleLayer->setVisible(false);
	}

	if (m_tollgateLayer == nullptr){
		m_tollgateLayer = TollgateLayer::create();
		this->addChild(m_tollgateLayer);
		m_tollgateLayer->retain();
	}
	m_tollgateLayer->setPositionX(winSize.width);
	m_tollgateLayer->runAction(EaseExponentialOut::create(MoveTo::create(0.25f, Vec2(0,0))));
}
void MainuiLayer::backFromTollgateLayer()
{
	Size winSize = Director::getInstance()->getWinSize();

	top->setpBtstrue(1);
	top->runAction(Sequence::create(
		Show::create(),
		MoveTo::create(0.25f, Vec2(top->getPositionX(), Actual_y-Tophight*(Actual_y/Design_hight))),
		nullptr));

	Rolelayer* roleLayer = getCenterLayer();
	if (roleLayer) {
		roleLayer->setVisible(true);
		roleLayer->layeReturnpop();
	}

	m_tollgateLayer->runAction(Sequence::create(
		MoveBy::create(0.2f, Vec2(winSize.width, 0)),
		//RemoveSelf::create(),
		nullptr));
}