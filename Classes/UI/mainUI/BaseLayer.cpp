#include "BaseLayer.h"
#include "ShopLayer.h"
#include "RiskdataManager.h"
#include "ActiveValueHelper.h"
#include "BasicLayer.h"
#include "UnitConfigManager.h"
#include "GameDirector.h"

BaseLayer::BaseLayer(void)
{
	
}

BaseLayer::BaseLayer(std::string msg){
  
	  
	  
}

BaseLayer::~BaseLayer(void)
{

}

/**
 * 静态单例变量初始化
 */
BaseLayer* BaseLayer::baselayer = nullptr;


BaseLayer* BaseLayer::getBaseLayer() {

	if(baselayer==nullptr){
	   BaseLayer *pRet =BaseLayer::create();
	 
	   pRet->setTag(1025);
	   pRet->Shieldtouched();
	   baselayer=pRet;
	}
	return baselayer;
}






void  BaseLayer::money_no(int tag){

	  
	  auto listen=EventListenerTouchOneByOne::create();
	  listen->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false   
	  listen->onTouchBegan = [](Touch* touch, Event* event){   
			return true;
	  };
	  EventDispatcher* dispatch=Director::getInstance()->getEventDispatcher();
	  dispatch->addEventListenerWithSceneGraphPriority(listen,this);
	  
	  
	  string name="goldnoenough";
	  if(tag==unitID_Diamond){
		  name="zsnoenough";
	  }else if(tag==unitID_Gold){
		  name="goldnoenough";
	  }else{
		  name="swnoenough";
	  }

	  color =Color4B(0,0,0,100);
	  LayerColor *colorLayer1 = LayerColor::create(color);
	  colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	  addChild(colorLayer1,1);

	  auto scal9=Sprite::create("UI_tanchukuang.png");
	  scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	  colorLayer1->addChild(scal9);


	  auto label=Label::createWithSystemFont(FX::StringsMap::getValue(name),"minijiankatong.ttf",40);
	  label->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.8));
	  scal9->addChild(label,100);

	  //购买
	 auto gm_button=MenuItemImage::create("UI_youxizhong_fuhuo_anniu_bg.png","UI_youxizhong_fuhuo_anniu_bg.png",
                         CC_CALLBACK_1(BaseLayer::Gosd,this));
	 gm_button->setPosition(Vec2(scal9->getContentSize().width/2-10-gm_button->getContentSize().width/2,scal9->getContentSize().height*0.4));
	 gm_button->setUserData((void*)tag);

	 auto gm_l=Label::createWithSystemFont(FX::StringsMap::getValue("gom1"),"minijiankatong.ttf",30);
	 gm_l->setPosition(Vec2(gm_button->getContentSize().width/2,gm_button->getContentSize().height/2));
	 gm_button->addChild(gm_l);

	//取消
     auto qx_button=MenuItemImage::create("UI_youxizhong_fuhuo_anniu_bg.png","UI_youxizhong_fuhuo_anniu_bg.png",
                         CC_CALLBACK_1(BaseLayer::returnPa,this));

	 qx_button->setPosition(Vec2(scal9->getContentSize().width/2+10+qx_button->getContentSize().width/2,scal9->getContentSize().height*0.4));
	 auto gm_2=Label::createWithSystemFont(FX::StringsMap::getValue("qx1"),"minijiankatong.ttf",30);
	 gm_2->setPosition(Vec2(qx_button->getContentSize().width/2,qx_button->getContentSize().height/2));
	 qx_button->addChild(gm_2);


	 auto menu = Menu::create(gm_button,qx_button,nullptr);
     menu->setPosition(Vec2::ZERO);
	 scal9->addChild(menu, 1);
	 showShake(scal9);

}

//购买框
void BaseLayer::craeteLayer(){

	

	auto back=Sprite::create("shangpin_goumai_tishikuang.png");
	back->setPosition(Vec2(Actual_x/2,Actual_y/2));
	addChild(back);

	auto gm_label=Label::createWithSystemFont("gold no need is buy?", "minijiankatong.ttf", 24);
	gm_label->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height/4*3));
	back->addChild(gm_label,1);

	//购买
	 auto gm_button=MenuItemImage::create("xuanze_zhuang_bei_queding.png","xuanze_zhuang_bei_queding_select.png",
                         CC_CALLBACK_1(BaseLayer::Gosd,this));
	 gm_button->setPosition(Vec2(back->getContentSize().width/2-gm_button->getContentSize().width/2-50*(Actual_x/Design_width),back->getContentSize().height/2));
	
	//取消
     auto qx_button=MenuItemImage::create("xuanze_zhuang_bei_quxiao.png","xuanze_zhuang_bei_quxiao_select.png",
                         CC_CALLBACK_1(BaseLayer::returnPa,this));
	 qx_button->setPosition(Vec2(back->getContentSize().width/2+qx_button->getContentSize().width/2+50*(Actual_x/Design_width),back->getContentSize().height/2));


	 auto menu = Menu::create(gm_button,qx_button,nullptr);
     menu->setPosition(Vec2::ZERO);
	 back->addChild(menu, 1);


}


//金币商店提示框
void BaseLayer::goldbox(std::function<void()> call_back ){


	auto listen=EventListenerTouchOneByOne::create();
	listen->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false   
	listen->onTouchBegan = [](Touch* touch, Event* event){   
		return true;
	};
	EventDispatcher* dispatch=Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen,this);


	EventCustom event(Layerchange);
	LayerData data;
	data.current=Layertag::gold_tag;
	data.push=0;
	event.setUserData(&data);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	ShopLayer* gold=ShopLayer::create();
	gold->setGold();
	if (call_back != nullptr)
	{
		gold->setCloseCallback(call_back);
	}
	addChild(gold,100, game_returntag);
	showShake(gold);

	


}

void BaseLayer::shopsW(){



	EventCustom event(Layerchange);
	LayerData data;
	data.current=Layertag::gold_tag;
	data.push=0;
	event.setUserData(&data);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	ShopLayer* gold=ShopLayer::create();
	gold->setSw();
	addChild(gold,100);

	showShake(gold);

}

	//钻石提示框
void BaseLayer::jiewelbox(){



	EventCustom event(Layerchange);
	LayerData data;
	data.current=Layertag::gold_tag;
	data.push=0;
	event.setUserData(&data);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);
	ShopLayer* gold=ShopLayer::create();
	gold->setJiewl();
	addChild(gold,100);
	showShake(gold);
}

void BaseLayer::Swbox(){

	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	ShopLayer* gold=ShopLayer::create();
	gold->setSw();
	colorLayer1->addChild(gold);

	showShake(gold);

}


void  BaseLayer::Achievementpromptbox_net(unsigned int id){

	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(getDelwithint(25,0)*18.12,getDelwithint(21,1)*15.4));// 
	colorLayer1->addChild(scal9);

	auto top=Sprite::create("ui_sence_zanting_bg_tiao.png");
	top->setScaleX(0.85f);
	top->setAnchorPoint(Vec2(0.5,0));
	top->setScaleX(scal9->getContentSize().width/top->getContentSize().width);
	top->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height-top->getContentSize().height));

	auto top_label=Label::createWithSystemFont(FX::StringsMap::getValue("daochengjiaoli"),"minijiankatong.ttf",30);
	top_label->setPosition(Vec2(top->getContentSize().width/2,top->getContentSize().height/2));
	top->addChild(top_label);
	scal9->addChild(top,100);

	auto aciveBox=Sprite::create("ui_chengjiu_wancheng.png");
	aciveBox->setPosition(Vec2(scal9->getContentSize().width/2,top->getPositionY()-top->getContentSize().height*0.85/2-aciveBox->getContentSize().height/4));
	scal9->addChild(aciveBox,100);
	 
	string name=AchievementHelper::getHelper()->getAchieveByID(id)->name;
	string dscrible=AchievementHelper::getHelper()->getAchieveByID(id)->describe;

	auto labelname=Label::createWithSystemFont(name,"minijiankatong.ttf",24);
	labelname->setColor(Color3B(172,65,0));
	labelname->setPosition(Vec2(labelname->getContentSize().width/2+getDelwithint(25,1),aciveBox->getContentSize().height-labelname->getContentSize().height/2-getDelwithint(20,1)));
	aciveBox->addChild(labelname);

	
	auto dscriblable=Label::createWithSystemFont(dscrible,"minijiankatong.ttf",24);
	dscriblable->setColor(Color3B(172,65,0));
	dscriblable->setPosition(Vec2(dscriblable->getContentSize().width/2+getDelwithint(20,1),dscriblable->getContentSize().height/2+getDelwithint(20,1)));
	aciveBox->addChild(dscriblable);

    
	auto yidc=Label::createWithSystemFont(FX::StringsMap::getValue("yidaocheng"),"minijiankatong.ttf",24);
	yidc->setColor(Color3B(172,65,0));
	yidc->setPosition(Vec2(yidc->getContentSize().width/2+10,scal9->getContentSize().height*0.43));
	scal9->addChild(yidc,100);


	float f_value=0;//
	

	vector<Achieve> chives=*AchievementHelper::getHelper()->getAllAchievement();
	for(size_t i=0;i<chives.size();i++){
		if(AchievementHelper::getHelper()->getAchieveByID(chives.at(i).ID)->isOver==true){
			f_value++;
		}
	}

	float need_Experience=AchievementHelper::getHelper()->getAllAchievement()->size();
	//进度条
		auto tiLiBarBoarder =Sprite::create("ui_zhuye_jiaoseshengji_nengli_jinyan_bg.png");
		scal9->addChild(tiLiBarBoarder,100);
		tiLiBarBoarder->setPosition(Vec2(yidc->getPositionX()+yidc->getContentSize().width/2+tiLiBarBoarder->getContentSize().width/2,
			yidc->getPositionY()));
	    auto progress = ProgressTimer::create(Sprite::create("ui_zhuye_jiaoseshengji_nengli_jinyan.png"));


	    progress->setType(ProgressTimer::Type::BAR);
	    progress->setMidpoint(Vec2(0,0));
	    progress->setBarChangeRate(Vec2(1, 0));
		tiLiBarBoarder->addChild(progress, 2, 101);
		progress->setPosition(Vec2(tiLiBarBoarder->getContentSize().width/2, tiLiBarBoarder->getContentSize().height/2));


		float prent=0.0;
		if(f_value-0==0){
		  prent=0;
		}else{
		  prent=(f_value/need_Experience)*100.0f;
		}

		progress->setPercentage(prent);

	
		string pre=__String::createWithFormat("%.02f",prent)->getCString();
	pre+="%";

	auto precent_lable=Label::createWithSystemFont(pre,"minijiankatong.ttf",24);
	precent_lable->setColor(Color3B(172,65,0));
	precent_lable->setPosition(Vec2(tiLiBarBoarder->getContentSize().width/2+tiLiBarBoarder->getPositionX()+precent_lable->getContentSize().width/2,
		tiLiBarBoarder->getPositionY()));

	scal9->addChild(precent_lable,100);


	auto rewar_lable=Label::createWithSystemFont(FX::StringsMap::getValue("jl"),"minijiankatong.ttf",24);
	rewar_lable->setVisible(false);
	rewar_lable->setColor(Color3B(172,65,0));
	rewar_lable->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.4));
	scal9->addChild(rewar_lable,100);


	string hd=FX::StringsMap::getValue("achivegold")+__String::createWithFormat("%d",AchievementHelper::getHelper()->getAchieveByID(id)->count)->getCString();

	int w_id=AchievementHelper::getHelper()->getAchieveByID(id)->award_unitID;

	

	if(w_id==98){

		hd=FX::StringsMap::getValue("achivegold")+__String::createWithFormat("%d",AchievementHelper::getHelper()->getAchieveByID(id)->count)->getCString();
	}else if(w_id==99){

		hd=FX::StringsMap::getValue("achivezs")+__String::createWithFormat("%d",AchievementHelper::getHelper()->getAchieveByID(id)->count)->getCString();
	}else{

		hd=FX::StringsMap::getValue("get_d")+UnitConfigManager::getManager()->getOneUnit(w_id)->name+__String::createWithFormat("%d",AchievementHelper::getHelper()->getAchieveByID(id)->count)->getCString();
	}
	
	
	auto text_lable=Label::createWithSystemFont(hd,"minijiankatong.ttf",24,Size(400,0),TextHAlignment::CENTER);
	text_lable->setColor(Color3B(172,65,0));
	text_lable->setPosition(Vec2(scal9->getContentSize().width/2,rewar_lable->getPositionY()-rewar_lable->getContentSize().height));
	scal9->addChild(text_lable,100);
     
    auto qd_button=MenuItemImage::create("ui_anniu_tongyong_zhuye.png","ui_anniu_tongyong_zhuye_select.png",
                         CC_CALLBACK_1(BaseLayer::reward_net,this));
	qd_button->setAnchorPoint(Vec2(0.5,0));
	qd_button->setPosition(Vec2(scal9->getContentSize().width/2,0));
	
	qd_button->setUserData((void*)id);
	auto qd_label=Label::createWithSystemFont(FX::StringsMap::getValue("linqu"),"minijiankatong.ttf",30);
	qd_label->setPosition(Vec2(qd_button->getContentSize().width/2,qd_button->getContentSize().height/2));
	qd_button->addChild(qd_label);
	auto menu = Menu::create(qd_button,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu,100);
	showShake(scal9);
}


//成就提示框

void BaseLayer::Achievementpromptbox(unsigned int id){

	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(getDelwithint(25,0)*18.12,getDelwithint(21,1)*15.4));// 
	colorLayer1->addChild(scal9);

	auto top=Sprite::create("ui_sence_zanting_bg_tiao.png");
	top->setScaleX(0.85f);
	top->setAnchorPoint(Vec2(0.5,0));
	top->setScaleX(scal9->getContentSize().width/top->getContentSize().width);
	top->setPosition(Vec2(Actual_x/2,scal9->getPositionY()+scal9->getContentSize().height/2-top->getContentSize().height));

	auto top_label=Label::createWithSystemFont(FX::StringsMap::getValue("daochengjiaoli"),"minijiankatong.ttf",30);
	top_label->setPosition(Vec2(top->getContentSize().width/2,top->getContentSize().height/2));
	top->addChild(top_label);
	colorLayer1->addChild(top);

	auto aciveBox=Sprite::create("ui_chengjiu_wancheng.png");
	aciveBox->setPosition(Vec2(Actual_x/2,top->getPositionY()-top->getContentSize().height));
	colorLayer1->addChild(aciveBox);
	 
	string name=AchievementHelper::getHelper()->getAchieveByID(id)->name;
	string dscrible=AchievementHelper::getHelper()->getAchieveByID(id)->describe;

	auto labelname=Label::createWithSystemFont(name,"minijiankatong.ttf",24);
	labelname->setColor(Color3B(172,65,0));
	labelname->setPosition(Vec2(labelname->getContentSize().width/2+getDelwithint(25,1),aciveBox->getContentSize().height-labelname->getContentSize().height/2-getDelwithint(20,1)));
	aciveBox->addChild(labelname);

	
	auto dscriblable=Label::createWithSystemFont(dscrible,"minijiankatong.ttf",24);
	dscriblable->setColor(Color3B(172,65,0));
	dscriblable->setPosition(Vec2(dscriblable->getContentSize().width/2+getDelwithint(20,1),dscriblable->getContentSize().height/2+getDelwithint(20,1)));
	aciveBox->addChild(dscriblable);

    
	auto yidc=Label::createWithSystemFont(FX::StringsMap::getValue("yidaocheng"),"minijiankatong.ttf",24);
	yidc->setColor(Color3B(172,65,0));
	yidc->setPosition(Vec2(getDelwithint(266,0)+yidc->getContentSize().width/2,aciveBox->getPositionY()-aciveBox->getContentSize().height/2-yidc->getContentSize().height/2));
	colorLayer1->addChild(yidc);
	float f_value=0;//
	
	vector<Achieve> chives=*AchievementHelper::getHelper()->getAllAchievement();
	for(size_t i=0;i<chives.size();i++){
		if(AchievementHelper::getHelper()->getAchieveByID(chives.at(i).ID)->isOver==true){
			f_value++;
		}
	}

	float need_Experience=AchievementHelper::getHelper()->getAllAchievement()->size();

	//进度条
		auto tiLiBarBoarder =Sprite::create("ui_zhuye_jiaoseshengji_nengli_jinyan_bg.png");
		colorLayer1->addChild(tiLiBarBoarder,2);
		tiLiBarBoarder->setPosition(Vec2(yidc->getPositionX()+yidc->getContentSize().width/2+tiLiBarBoarder->getContentSize().width/2,
			yidc->getPositionY()));
	    auto progress = ProgressTimer::create(Sprite::create("ui_zhuye_jiaoseshengji_nengli_jinyan.png"));
	    progress->setType(ProgressTimer::Type::BAR);
	    progress->setMidpoint(Vec2(0,0));
	    progress->setBarChangeRate(Vec2(1, 0));
		tiLiBarBoarder->addChild(progress, 2, 101);
		progress->setPosition(Vec2(tiLiBarBoarder->getContentSize().width/2, tiLiBarBoarder->getContentSize().height/2));


		float prent=0.0;
		if(f_value-0==0){
		  prent=0;
		}else{
		  prent=(f_value/need_Experience)*100.0f;
		}

		progress->setPercentage(prent);

	
		string pre=__String::createWithFormat("%.02f",prent)->getCString();
	pre+="%";

	auto precent_lable=Label::createWithSystemFont(pre,"minijiankatong.ttf",24);
	precent_lable->setColor(Color3B(172,65,0));
	precent_lable->setPosition(Vec2(tiLiBarBoarder->getContentSize().width/2+tiLiBarBoarder->getPositionX()+precent_lable->getContentSize().width/2,
		aciveBox->getPositionY()-aciveBox->getContentSize().height/2-precent_lable->getContentSize().height/2));

	colorLayer1->addChild(precent_lable);


	auto rewar_lable=Label::createWithSystemFont(FX::StringsMap::getValue("jl"),"minijiankatong.ttf",24);
	rewar_lable->setVisible(false);
	rewar_lable->setColor(Color3B(172,65,0));
	rewar_lable->setPosition(Vec2(getDelwithint(266,0)+rewar_lable->getContentSize().width/2,yidc->getPositionY()-yidc->getContentSize().height/2-rewar_lable->getContentSize().height/2));
	colorLayer1->addChild(rewar_lable);


	string hd=FX::StringsMap::getValue("achivegold")+__String::createWithFormat("%d",AchievementHelper::getHelper()->getAchieveByID(id)->count)->getCString();

	int w_id=AchievementHelper::getHelper()->getAchieveByID(id)->award_unitID;
	if(w_id==98){

		hd=FX::StringsMap::getValue("achivegold")+__String::createWithFormat("%d",AchievementHelper::getHelper()->getAchieveByID(id)->count)->getCString();
	}else if(w_id==99){

		hd=FX::StringsMap::getValue("achivezs")+__String::createWithFormat("%d",AchievementHelper::getHelper()->getAchieveByID(id)->count)->getCString();
	}
	
	
	auto text_lable=Label::createWithSystemFont(hd,"minijiankatong.ttf",24,Size(400,0),TextHAlignment::CENTER);
	text_lable->setColor(Color3B(172,65,0));
	text_lable->setPosition(Vec2(Actual_x/2,rewar_lable->getPositionY()-rewar_lable->getContentSize().height));
	colorLayer1->addChild(text_lable);



    auto qd_button=MenuItemImage::create("ui_anniu_tongyong_zhuye.png","ui_anniu_tongyong_zhuye_select.png",
                         CC_CALLBACK_1(BaseLayer::reward,this));
	qd_button->setAnchorPoint(Vec2(0.5,0));
	qd_button->setPosition(Vec2(Actual_x/2,scal9->getPositionY()-scal9->getContentSize().height/2-qd_button->getContentSize().height/2));
	
	qd_button->setUserData((void*)id);
	auto qd_label=Label::createWithSystemFont(FX::StringsMap::getValue("linqu"),"minijiankatong.ttf",30);
	qd_label->setPosition(Vec2(qd_button->getContentSize().width/2,qd_button->getContentSize().height/2));
	qd_button->addChild(qd_label);



	auto menu = Menu::create(qd_button,nullptr);
    menu->setPosition(Vec2::ZERO);
	colorLayer1->addChild(menu, 2);
	
}


void BaseLayer::closeGame(){


	this->Shieldtouched();
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(getDelwithint(25,0)*18.12,getDelwithint(21,1)*15.4));// 
	colorLayer1->addChild(scal9);

	string need=FX::StringsMap::getValue("closegame");
	
	
	auto label=Label::createWithSystemFont(need,"minijiankatong.ttf",30);
	label->setPosition(Vec2(Actual_x/2,scal9->getPositionY()+scal9->getContentSize().height/2-label->getContentSize().height/2));
	colorLayer1->addChild(label);
	
	
	auto qd_button=MenuItemImage::create("ui_anniu_tongyong_zhuye.png","ui_anniu_tongyong_zhuye_select.png",
		CC_CALLBACK_1(BaseLayer::Gameover,this));
	qd_button->setPosition(Vec2(Actual_x/2-qd_button->getContentSize().width,scal9->getPositionY()-2*qd_button->getContentSize().height));
	
	

	
	auto lable_1=Label::createWithSystemFont(FX::StringsMap::getValue("qungding"),"minijiankatong.ttf",30);
	lable_1->setPosition(Vec2(qd_button->getContentSize().width/2,qd_button->getContentSize().height/2));
	qd_button->addChild(lable_1);

	
	auto qx_button=MenuItemImage::create("ui_anniu_tongyong_zhuye.png","ui_anniu_tongyong_zhuye_select.png",
		CC_CALLBACK_1(BaseLayer::returnPa,this));
	qx_button->setPosition(Vec2(Actual_x/2+qx_button->getContentSize().width,scal9->getPositionY()-2*qx_button->getContentSize().height));

	auto lable2=Label::createWithSystemFont(FX::StringsMap::getValue("cacle"),"minijiankatong.ttf",30);
	lable2->setPosition(Vec2(qx_button->getContentSize().width/2,qx_button->getContentSize().height/2));
	qx_button->addChild(lable2);



	auto menu = Menu::create(qd_button,qx_button,nullptr);
    menu->setPosition(Vec2::ZERO);
	colorLayer1->addChild(menu, 2);


}


void BaseLayer::gkmodebox(){


	this->Shieldtouched();
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(getDelwithint(25,0)*18.12,getDelwithint(21,1)*15.4));// 
	colorLayer1->addChild(scal9);

	
	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BaseLayer::returnPa,this));
	close->setPosition(Vec2(scal9->getPositionX()+scal9->getContentSize().width/2-close->getContentSize().width/2,scal9->getPositionY()+scal9->getContentSize().height/2-close->getContentSize().height/2));
	
	auto renwu=Sprite::create("lanmeimei.png");
	renwu->setPosition(Actual_x/2+scal9->getContentSize().width/3+renwu->getContentSize().width/2+getDelwithint(20,1),scal9->getPositionY());
	colorLayer1->addChild(renwu);



	auto youDao=Label::createWithSystemFont(FX::StringsMap::getValue("gk_ts"),"minijiankatong.ttf",30,Size(25*16,0),TextHAlignment::LEFT);
	youDao->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height/2));
	youDao->setColor(Color3B(172,65,0));
	scal9->addChild(youDao);


	auto menu = Menu::create(close,nullptr);
    menu->setPosition(Vec2::ZERO);
	colorLayer1->addChild(menu, 2);

}

void BaseLayer::goShop(int id,unsigned int wupingId){


	this->setTag(1000);
	CCLOG("--->shop--->");
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(getDelwithint(25,0)*18.12,getDelwithint(21,1)*15.4));// 
	colorLayer1->addChild(scal9);

	
	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BaseLayer::returnPa,this));
	close->setPosition(Vec2(scal9->getPositionX()+scal9->getContentSize().width/2-close->getContentSize().width/2,scal9->getPositionY()+scal9->getContentSize().height/2-close->getContentSize().height/2));
	
	auto renwu=Sprite::create("lanmeimei.png");
	renwu->setPosition(Actual_x/2+scal9->getContentSize().width/3+renwu->getContentSize().width/2+getDelwithint(20,1),scal9->getPositionY());
	colorLayer1->addChild(renwu);


	//lqgold

	string need=FX::StringsMap::getValue("lqgold");
	string need1=FX::StringsMap::getValue("sdk");
	if(id==98){
		need=FX::StringsMap::getValue("lqgold");
	}else if(id==99){
		need=FX::StringsMap::getValue("lqgold");

	}else if(id==100){
		need=FX::StringsMap::getValue("lqhero");
		need1=FX::StringsMap::getValue("sdkhero");

	}else if(id==101){
		need=FX::StringsMap::getValue("lqcw");
		need1=FX::StringsMap::getValue("sdkpet");
	}else if(id==102){
		need=FX::StringsMap::getValue("lqmagical");
		need1=FX::StringsMap::getValue("sdkmagic");
	}else if(id==103){ //开启关卡
		//Kaiqigk
		need=FX::StringsMap::getValue("Kaiqigk");
		need1=FX::StringsMap::getValue("sdkmode");

	}

	auto youDao=Label::createWithSystemFont(need1,"minijiankatong.ttf",30,Size(25*16,0),TextHAlignment::LEFT);
	youDao->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height/2));
	youDao->setColor(Color3B(172,65,0));
	scal9->addChild(youDao);

	Label* lqts;
	if(id==102){

	    lqts=Label::createWithSystemFont(need,"minijiankatong.ttf",24);

	}else{
		lqts=Label::createWithSystemFont(need,"minijiankatong.ttf",26);
	}
	auto lqbutton=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse.png",CC_CALLBACK_1(BaseLayer::gouMai,this));
	lqbutton->setPosition(Vec2(Actual_x/2,scal9->getPositionY()-scal9->getContentSize().height/2));
	lqbutton->setUserData((void*)wupingId);



	lqts->setPosition(Vec2(lqbutton->getContentSize().width/2,lqbutton->getContentSize().height/2));
	lqbutton->addChild(lqts);


	auto menu = Menu::create(close,lqbutton,nullptr);
    menu->setPosition(Vec2::ZERO);
	colorLayer1->addChild(menu, 2);

	//showShake();
	
}


void BaseLayer::Gosd(Ref* sender){
	
	
	FX::Music::getInstance()->buttonClicked();
	MenuItemImage* p=(MenuItemImage*)sender;
	int tag=(int)(long long)p->getUserData();

	if(tag==unitID_Gold	){
		BaseLayer* layer=BaseLayer::create();
		layer->goldbox();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}else if(tag==unitID_Diamond){
		BaseLayer* layer=BaseLayer::create();
		layer->jiewelbox();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}else{

		BaseLayer* layer=BaseLayer::create();
		layer->Swbox();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}
	this->removeFromParent();
    
}
void BaseLayer::reward_net(Ref* sender){


	FX::Music::getInstance()->buttonClicked();
	MenuItemImage* p=(MenuItemImage*)sender;
	int id=(int)(long long)p->getUserData();
	AchievementHelper::getHelper()->getAchievement(id);
	MsgCenter();
	this->removeFromParent();
	

}


void BaseLayer::reward(Ref* sender){
   
	FX::Music::getInstance()->buttonClicked();
	MenuItemImage* p=(MenuItemImage*)sender;
	int id=(int)(long long)p->getUserData();

	//if(id!=10001){
	   AchievementHelper::getHelper()->getAchievement(id);
	//}
	this->removeFromParent();

	

	
    EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
	EventCustom event(Changeacive);
	MsgData data;
    data.errorCode = 0;
    data.value =id;
	event.setUserData(&data);
	eventDspc->dispatchEvent(&event);

	


}
void BaseLayer::freep(){

	if(Director::getInstance()->getRunningScene()->getChildByTag(1025)!=nullptr){
		Director::getInstance()->getRunningScene()->removeChildByTag(1025);
	    baselayer=nullptr;
	}
}
void BaseLayer::returnPa(Ref* sender){


	MenuItemImage* p=(MenuItemImage*)sender;
	FX::Music::getInstance()->buttonClicked();
	this->removeFromParent();
    

}


int BaseLayer::getDelwithint(int number,int iswidth){

	int dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;
}


void BaseLayer::onEnter(){

	Layer::onEnter();
}
void BaseLayer::onExit(){
	
	Layer::onExit();
}





bool BaseLayer::init(){

	if(!Layer::init()){
		return false;
	}
	state=-1;
	
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	yinyue_state=false;
    yinxiao_state=false;
	texiao_state=false;
	return true;
}


void BaseLayer::goSop(Ref* sender){


	FX::Music::getInstance()->buttonClicked();
	this->removeFromParent();

	MenuItemImage* p=(MenuItemImage*)sender;
	int id=(int)(long long)p->getUserData();

	if(id==98){
		
	   BaseLayer* shop=BaseLayer::create();
	   shop->goldbox();
	   shop->Shieldtouched();
	   Director::getInstance()->getRunningScene()->addChild(shop);

	}else if(id==99){

		BaseLayer* shop=BaseLayer::create();
	    shop->jiewelbox();
	    shop->Shieldtouched();
	    Director::getInstance()->getRunningScene()->addChild(shop);

	}
	

}


void BaseLayer::Gameover(Ref* sender){

	FX::Music::getInstance()->buttonClicked();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	  MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
      return;
    #endif

    Director::getInstance()->end();
      #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
      exit(0);
    #endif



}


void BaseLayer::shopclose(Ref* sender){
	FX::Music::getInstance()->buttonClicked();


}


//接入sdk

void BaseLayer::gouMai(Ref* sender){

	FX::Music::getInstance()->buttonClicked();
	MenuItemImage* p=(MenuItemImage*)sender;
	int id=(int)(long long)p->getUserData();
	CCLOG("shop----->%d",id);

	if(id!=0){
		MarketHelper::getHelper()->buyItem(id);
	}
}


void BaseLayer::showShake(Node* sp){

	
	sp->setScale(0.2f);
	sp->runAction(Sequence::create(
		FadeOut::create(0),
		Spawn::create(FadeIn::create(0.05f),
		ScaleTo::create(0.1f,1.05f),
		nullptr),
		
		ScaleTo::create(0.1f,1.0f),
		nullptr));

	
}

//网络

void BaseLayer::Netcontact(){
     

	
	this->removeAllChildren();
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);


	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(getDelwithint(25,0)*18.12,getDelwithint(21,1)*15.4));// 
	colorLayer1->addChild(scal9);


	auto wait=Sprite::create("wait.png");
	wait->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height/2));
	wait->setColor(Color3B(255,33,33));
	scal9->addChild(wait);
	

	wait->runAction(
		  RepeatForever::create(RotateBy::create(2,360))
		);


	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BaseLayer::returnPa,this));
	close->setPosition(Vec2(scal9->getContentSize().width-close->getContentSize().width/2,scal9->getContentSize().height-close->getContentSize().height/2));

    
	auto menu = Menu::create(close,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu,1000);
}



void BaseLayer::Regists(){

	
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);


	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(getDelwithint(25,0)*18.12,getDelwithint(21,1)*15.4));// 
	colorLayer1->addChild(scal9);

	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BaseLayer::returnPa,this));
	close->setPosition(Vec2(scal9->getPositionX()+scal9->getContentSize().width/2-close->getContentSize().width/2,scal9->getPositionY()+scal9->getContentSize().height/2-close->getContentSize().height/2));

	auto sl=Label::createWithSystemFont(FX::StringsMap::getValue("zhuceC"),"minijiankatong.ttf",30);
	sl->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height/2));
	scal9->addChild(sl);



	auto menu = Menu::create(close,nullptr);
    menu->setPosition(Vec2::ZERO);
	colorLayer1->addChild(menu, 2);
}

void BaseLayer::Registf(){

	
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);


	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(getDelwithint(25,0)*18.12,getDelwithint(21,1)*15.4));// 
	colorLayer1->addChild(scal9);

	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BaseLayer::returnPa,this));
	close->setPosition(Vec2(scal9->getPositionX()+scal9->getContentSize().width/2-close->getContentSize().width/2,scal9->getPositionY()+scal9->getContentSize().height/2-close->getContentSize().height/2));


	auto fl=Label::createWithSystemFont(FX::StringsMap::getValue("zhuceS"),"minijiankatong.ttf",30);
	fl->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height/2));
	scal9->addChild(fl);


	auto menu = Menu::create(close,nullptr);
    menu->setPosition(Vec2::ZERO);
	colorLayer1->addChild(menu, 2);
}


void BaseLayer::Tishistring(string str){

	this->Shieldtouched();

	this->removeAllChildren();
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);


	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(getDelwithint(25,0)*18.12,getDelwithint(21,1)*15.4));// 
	colorLayer1->addChild(scal9);

	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BaseLayer::returnPa,this));
	close->setPosition(Vec2(scal9->getPositionX()+scal9->getContentSize().width/2-close->getContentSize().width/2,scal9->getPositionY()+scal9->getContentSize().height/2-close->getContentSize().height/2));

	if(str.compare("nicknameS")==0){
		close->setUserData((void*)100);
	}


	auto fl=Label::createWithSystemFont(FX::StringsMap::getValue(str),"minijiankatong.ttf",30);
	fl->setColor(Colorzdy);
	fl->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height/2));
	scal9->addChild(fl);


	auto menu = Menu::create(close,nullptr);
    menu->setPosition(Vec2::ZERO);
	colorLayer1->addChild(menu, 2);


}


void BaseLayer::modify_nameBox(){


	

	Shieldtouched();
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);


	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(400,200));// 
	colorLayer1->addChild(scal9);



	auto bg=Sprite::create("ui_zhuye_biaoti.png");
	bg->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.65));
	//bg->setVisible(false);
	scal9->addChild(bg);


	mody_name=EditBoxTest::create();
	mody_name->update_namebox();
	mody_name->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.65));
	scal9->addChild(mody_name);


	auto need_label=Label::createWithSystemFont(FX::StringsMap::getValue("needmony"),"minijiankatong.ttf",24);
	need_label->setColor(Color3B(0,0,0));
	need_label->setPosition(Vec2(40+need_label->getContentSize().width/2,scal9->getContentSize().height*0.3));
	scal9->addChild(need_label);


	auto jiewl_img=Sprite::create("ui_icon_zuanshi.png");
	jiewl_img->setPosition(Vec2(need_label->getPositionX()+need_label->getContentSize().width/2+20+jiewl_img->getContentSize().width/2,scal9->getContentSize().height*0.3));
	scal9->addChild(jiewl_img);

	auto number_label=Label::createWithSystemFont("50","minijiankatong.ttf",24);
	number_label->setColor(Color3B(255,33,33));
	number_label->setPosition(Vec2(jiewl_img->getContentSize().width/2+20+jiewl_img->getPositionX(),scal9->getContentSize().height*0.3));	
	scal9->addChild(number_label);

	auto mody_butoon=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(BaseLayer::mody_nameYes,this));
	mody_butoon->setPosition(Vec2(scal9->getContentSize().width*0.8,scal9->getContentSize().height*0.3));
	this->setTextb(mody_butoon,"qding",30,Color3B(0,0,0));



	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BaseLayer::updateName,this));
	close->setPosition(Vec2(scal9->getContentSize().width- close->getContentSize().width/2,scal9->getContentSize().height-close->getContentSize().height/2));

	auto menu = Menu::create(mody_butoon,close,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu, 2);

	showShake(scal9);
}

void BaseLayer::setTextb(MenuItemImage* node,string str,int fontsize,Color3B b){

	auto label=Label::createWithSystemFont(FX::StringsMap::getValue(str),"minijiankatong.ttf",fontsize);
	label->setPosition(Vec2(node->getContentSize().width/2,node->getContentSize().height/2));
	label->setColor(b);
	node->addChild(label);
}


void BaseLayer::mody_nameYes(Ref* ref){


	
	auto cs_namesize=Label::createWithSystemFont(mody_name->context,"minijiankatong.ttf",24);
	auto ctname=Label::createWithSystemFont(FX::StringsMap::getValue("Bzszzm"),"minijiankatong.ttf",24);
	if(ctname->getContentSize().width<cs_namesize->getContentSize().width){
		mody_name->_editName->setPlaceHolder(FX::StringsMap::getValue("zuiduoliugehanzi").c_str());
	}else if(cs_namesize->getContentSize().width==0){
		mody_name->_editName->setPlaceHolder(FX::StringsMap::getValue("mingzibunengweikong").c_str());
	}else{
		NetmsgManger* manager=NetmsgManger::getNetManager();
		manager->updateNickname(mody_name->context);
		manager->playername=mody_name->context;
	}
	this->removeFromParent();

}

void BaseLayer::huoD_ward(int Mc,vector<ItInfo> ifos,string Zm){



	ward_type=ifos.at(0).type;
	ward_num=ifos.at(0).count;
    
	Shieldtouched();
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);


	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(400,200));// 
	colorLayer1->addChild(scal9);
	 


	Sprite* bt_img=Sprite::create("ui_anniu_tongyong_zhuye_lanse.png");
	bt_img->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height+5));
	scal9->addChild(bt_img,1000);


	auto layername=Label::createWithSystemFont(FX::StringsMap::getValue("Jjjl"),"minijiankatong.ttf",24);
	layername->setColor(Color3B(0,0,0));
	layername->setPosition(Vec2(bt_img->getContentSize().width/2,bt_img->getContentSize().height/2));
    bt_img->addChild(layername,1000);
    
	string bzzm=FX::StringsMap::getValue("Bzszzm")+": "+Zm;
	auto Bzszzm=Label::createWithSystemFont(bzzm,"minijiankatong.ttf",24);
	Bzszzm->setColor(Color3B(0,0,0));
	Bzszzm->setPosition(Vec2(40+Bzszzm->getContentSize().width/2,scal9->getContentSize().height*0.8));
	scal9->addChild(Bzszzm,1000);


	string qdmc=FX::StringsMap::getValue("Qdemc")+":";
	auto qdmclabel=Label::createWithSystemFont(qdmc,"minijiankatong.ttf",22);
	qdmclabel->setColor(Color3B(0,0,0));
	qdmclabel->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.65));
	scal9->addChild(qdmclabel);

	auto qdm_nclabel=Label::createWithSystemFont(__String::createWithFormat("%d",Mc)->getCString(),"minijiankatong.ttf",30);
	qdm_nclabel->setColor(Color3B(0,0,0));
	qdm_nclabel->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.50));
	scal9->addChild(qdm_nclabel);


	string hd_l=FX::StringsMap::getValue("Hdjl")+":";
	auto hd_label=Label::createWithSystemFont(hd_l,"minijiankatong.ttf",24);
	hd_label->setColor(Color3B(0,0,0));
	hd_label->setPosition(Vec2(scal9->getContentSize().width*0.15+hd_label->getContentSize().width/2,scal9->getContentSize().height*0.3));
	scal9->addChild(hd_label);

	string jinb="ui_icon_jinbi.png";
	
	if(ward_type==UnitID::unitID_Gold){
		jinb="ui_icon_jinbi.png";
	}else if(ward_type==UnitID::unitID_Diamond){
		jinb="ui_icon_zuanshi.png";
	}

	auto w_p=Sprite::create(jinb);
	w_p->setScale(0.7f);
	w_p->setPosition(scal9->getContentSize().width/2,hd_label->getPositionY());
	scal9->addChild(w_p);

	auto hd_labelnumber=Label::createWithSystemFont(__String::createWithFormat("%d",ward_num)->getCString(),"minijiankatong.ttf",24);
	hd_labelnumber->setColor(Color3B(255,33,33));
	hd_labelnumber->setPosition(w_p->getContentSize().width/2+w_p->getPositionX()+hd_labelnumber->getContentSize().width/2,w_p->getPositionY());
	scal9->addChild(hd_labelnumber);


	if(Mc<=10){

		auto label_jj=Label::createWithSystemFont(FX::StringsMap::getValue("Jjggjb"),"minijiankatong.ttf",24);
		label_jj->setPosition(Vec2(scal9->getContentSize().width*0.8,scal9->getContentSize().height*0.6));
		label_jj->setColor(Color3B(255,33,33));
		label_jj->setRotation(-35.0f);
		scal9->addChild(label_jj,1000);
	}
   
	auto linqu_butoon=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(BaseLayer::lingqu_netBack,this));
	linqu_butoon->setPosition(Vec2(scal9->getContentSize().width*0.5,10));


	auto menu = Menu::create(linqu_butoon,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu, 2);

	showShake(scal9);




}

void BaseLayer::huoD_ward(int Mc,int type,int ward,string Zm){

	  
    ward_type=type;
	ward_num=ward;


	Shieldtouched();
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);


	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(400,200));// 
	colorLayer1->addChild(scal9);
	 


	Sprite* bt_img=Sprite::create("ui_anniu_tongyong_zhuye_lanse.png");
	bt_img->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height+5));
	scal9->addChild(bt_img,1000);


	auto layername=Label::createWithSystemFont(FX::StringsMap::getValue("Jjjl"),"minijiankatong.ttf",24);
	layername->setColor(Color3B(0,0,0));
	layername->setPosition(Vec2(bt_img->getContentSize().width/2,bt_img->getContentSize().height/2));
    bt_img->addChild(layername,1000);



	


	string bzzm=FX::StringsMap::getValue("Bzszzm")+": "+Zm;
	auto Bzszzm=Label::createWithSystemFont(bzzm,"minijiankatong.ttf",24);
	Bzszzm->setColor(Color3B(0,0,0));
	Bzszzm->setPosition(Vec2(40+Bzszzm->getContentSize().width/2,scal9->getContentSize().height*0.8));
	scal9->addChild(Bzszzm,1000);


	string qdmc=FX::StringsMap::getValue("Qdemc")+":";
	auto qdmclabel=Label::createWithSystemFont(qdmc,"minijiankatong.ttf",22);
	qdmclabel->setColor(Color3B(0,0,0));
	qdmclabel->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.65));
	scal9->addChild(qdmclabel);

	auto qdm_nclabel=Label::createWithSystemFont(__String::createWithFormat("%d",Mc)->getCString(),"minijiankatong.ttf",30);
	qdm_nclabel->setColor(Color3B(0,0,0));
	qdm_nclabel->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.50));
	scal9->addChild(qdm_nclabel);


	string hd_l=FX::StringsMap::getValue("Hdjl")+":";
	auto hd_label=Label::createWithSystemFont(hd_l,"minijiankatong.ttf",24);
	hd_label->setColor(Color3B(0,0,0));
	hd_label->setPosition(Vec2(scal9->getContentSize().width*0.15+hd_label->getContentSize().width/2,scal9->getContentSize().height*0.3));
	scal9->addChild(hd_label);

	string jinb="ui_icon_jinbi.png";
	
	if(type==UnitID::unitID_Gold){
		jinb="ui_icon_jinbi.png";
	}else if(type==UnitID::unitID_Diamond){
		jinb="ui_icon_zuanshi.png";
	}


	auto w_p=Sprite::create(jinb);
	w_p->setScale(0.7f);
	w_p->setPosition(scal9->getContentSize().width/2,hd_label->getPositionY());
	scal9->addChild(w_p);

	auto hd_labelnumber=Label::createWithSystemFont(__String::createWithFormat("%d",ward)->getCString(),"minijiankatong.ttf",24);
	hd_labelnumber->setColor(Color3B(255,33,33));
	hd_labelnumber->setPosition(w_p->getContentSize().width/2+w_p->getPositionX()+hd_labelnumber->getContentSize().width/2,w_p->getPositionY());
	scal9->addChild(hd_labelnumber);


	if(Mc<=10){

		auto label_jj=Label::createWithSystemFont(FX::StringsMap::getValue("Jjggjb"),"minijiankatong.ttf",24);
		label_jj->setPosition(Vec2(scal9->getContentSize().width*0.8,scal9->getContentSize().height*0.6));
		label_jj->setColor(Color3B(255,33,33));
		label_jj->setRotation(-35.0f);
		scal9->addChild(label_jj,1000);
	}


	auto linqu_butoon=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(BaseLayer::lingqu_netBack,this));
	linqu_butoon->setPosition(Vec2(scal9->getContentSize().width*0.5,10));
	
	auto l_q=Label::createWithSystemFont(FX::StringsMap::getValue("qungding"),"minijiankatong.ttf",24);
	l_q->setPosition(Vec2(linqu_butoon->getContentSize().width/2,linqu_butoon->getContentSize().height/2));
	linqu_butoon->addChild(l_q);

	auto menu = Menu::create(linqu_butoon,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu, 2);

	showShake(scal9);

}


void BaseLayer::lingqu_netBack(Ref* ref){
	 
    
	//BagDataManager::getManager()->changeOneUnitCount(ward_type,ward_num);
	this->removeFromParent();
     

}

void BaseLayer::return_Msg(int tag){

	 
	 if(tag==1){
		 this->Tishistring("nicknameS");
		 BagDataManager::getManager()->changeOneUnitCount(unitID_Diamond,update_jiwel);
	 }else if(tag==0){
		 BaseLayer* layer=BaseLayer::create();
		 layer->Tishistring("nicknameF");
		 Director::getInstance()->getRunningScene()->addChild(layer);

	 }

}

void BaseLayer::updateName(Ref* ref){

	 
	FX::Music::getInstance()->buttonClicked();
	this->removeFromParent();
	
	

}

void BaseLayer::returnmody_name(Ref* sender){


	FX::Music::getInstance()->buttonClicked();

	EventCustom event(Huoyuelinjian);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	this->removeFromParent();
	NetmsgManger::getNetManager()->setMsgc_null();
	NetmsgManger::getNetManager()->Csmsgis_null();
	
	
}


//活跃框
void BaseLayer::risk_box(){



	


	Shieldtouched();
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(850,500));// 
	colorLayer1->addChild(scal9);
	 

	auto hy_img=Sprite::create(Biti_imgstr);
	hy_img->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height));
	scal9->addChild(hy_img,100);
    
	auto hy_text=Label::createWithSystemFont(FX::StringsMap::getValue("hyd"),"minijiankatong.ttf",24);
	hy_text->setColor(Color3B(0,0,0));
	hy_text->setPosition(Vec2(hy_img->getContentSize().width/2,hy_img->getContentSize().height/2));
	hy_img->addChild(hy_text);
    
	auto jm=Label::createWithSystemFont(FX::StringsMap::getValue("jim"),"minijiankatong.ttf",24);
	jm->setPosition(Vec2(scal9->getContentSize().width*0.3,scal9->getContentSize().height*0.85));
	jm->setColor(Color3B(0,0,0));
	scal9->addChild(jm);

	auto yibai=Label::createWithSystemFont(FX::StringsMap::getValue("yibaidian"),"minijiankatong.ttf",24);
	yibai->setPosition(Vec2(jm->getContentSize().width/2+jm->getPositionX()+yibai->getContentSize().width/2,jm->getPositionY()));
	yibai->setColor(Color3B(255,33,33));
	scal9->addChild(yibai);

	auto dhyg=Label::createWithSystemFont(FX::StringsMap::getValue("hyzkydhyg"),"minijiankatong.ttf",24);
	dhyg->setPosition(yibai->getPositionX()+yibai->getContentSize().width/2+dhyg->getContentSize().width/2,jm->getPositionY());
	dhyg->setColor(Color3B(0,0,0));
	scal9->addChild(dhyg,100);


	string yy_text=FX::StringsMap::getValue("yylb")+"!";
	auto yylb=Label::createWithSystemFont(yy_text,"minijiankatong.ttf",24);
	yylb->setPosition(Vec2(dhyg->getPositionX()+dhyg->getContentSize().width/2+yylb->getContentSize().width/2,jm->getPositionY()));
	yylb->setColor(Color3B(255,10,10));
	scal9->addChild(yylb);

	//进度条
    auto tiLiBarBoarder =Sprite::create("ui_zhuye_jiaoseshengji_nengli_jinyan_bg.png");
	scal9->addChild(tiLiBarBoarder,2);
	tiLiBarBoarder->setPosition(Vec2(scal9->getContentSize().width/2,
		                             scal9->getContentSize().height*0.75));



	    progress = ProgressTimer::create(Sprite::create("ui_zhuye_jiaoseshengji_nengli_jinyan.png"));
	    progress->setType(ProgressTimer::Type::BAR);
	    progress->setMidpoint(Vec2(0,0));
	    progress->setBarChangeRate(Vec2(1, 0));
		tiLiBarBoarder->addChild(progress, 2, 101);
		progress->setPosition(Vec2(tiLiBarBoarder->getContentSize().width/2, tiLiBarBoarder->getContentSize().height/2));

		float prent=0.0;
		float q_x=(float)ActiveValueHelper::getHelper()->getHasActiveCount();
		float h_x=100;
		prent=(q_x/h_x)*100.0f;
		progress->setPercentage(prent);


		string bfb_str=__String::createWithFormat("%d",(int)q_x)->getCString();
		bfb_str+="/";
		bfb_str+=__String::createWithFormat("%d",(int)h_x)->getCString();

		b_l=Label::createWithSystemFont(bfb_str,"minijiankatong.ttf",24);
		b_l->setColor(Color3B(0,0,0));
		b_l->setPosition(Vec2(tiLiBarBoarder->getContentSize().width/2,tiLiBarBoarder->getContentSize().height/2));
		tiLiBarBoarder->addChild(b_l,100);


	//800 238
	auto bom=Scale9Sprite::create("ui_zhuye_daojukuang2.png");
	bom->setContentSize(Size(800,238));
	bom->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.4));
	scal9->addChild(bom,100);


	auto hqfont=MenuItemFont::create(FX::StringsMap::getValue("hqhyzff"),nullptr);
	hqfont->setPosition(Vec2(bom->getContentSize().width/2,bom->getContentSize().height));
	hqfont->setColor(Color3B(0,0,0));
	hqfont->setFontSize(24);
	bom->addChild(hqfont,100);
    
	vector<Active> risk= ActiveValueHelper::getHelper()->getAllActive(); 
	int hang=risk.size()/3;
	if(risk.size()%3==0){
		hang=risk.size()/3;
	}else{
		hang=risk.size()/3+1;
	}

	for(int i=0;i<hang;i++){

		for(int j=0;j<3;j++){

			int idx=i*3+j;
			if(idx<risk.size()){

			  float y=bom->getContentSize().height-(i+1)*(bom->getContentSize().height/(hang+1));
			  float x=(j)*(bom->getContentSize().width/3);
			  string text= __String::createWithFormat("%d",risk.at(idx).id)->getCString();
			         text+=". ";
					 text+=risk.at(idx).describe+"+";
					 text+=__String::createWithFormat("%d",risk.at(idx).value)->getCString();
					 auto label=Label::createWithSystemFont(text,"minijiankatong.ttf",24);
					 label->ignoreAnchorPointForPosition(false);
					 label->setAnchorPoint(Vec2(0,0.5));

					 label->setPosition(x,y);
					 label->setColor(Color3B(0,0,0));
					 bom->addChild(label,100);
			}

		}

	}


	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BaseLayer::returnmody_name,this));
	close->setPosition(Vec2(scal9->getContentSize().width- close->getContentSize().width/2,scal9->getContentSize().height-close->getContentSize().height/2));
	auto menu = Menu::create(close,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu, 2);
	showShake(scal9);
	//判断是否 可以领取随机随机包
	if(ActiveValueHelper::getHelper()->canGetGift()){
		   
		auto layer=BasicLayer::create();
		layer->active_warldbox(ActiveValueHelper::getHelper()->getOneGift_useActiveNum());
		Director::getInstance()->getRunningScene()->addChild(layer,1000);
		this->Conset();	
	}
}


void BaseLayer::active_warldbox(BagItem bag){

	Shieldtouched();
	bag_l.id=bag.id;
	bag_l.value=bag.value;

	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(320,220));// 
	colorLayer1->addChild(scal9);
	ww=scal9;

	auto renwu=Sprite::create("lanmeimei.png");
	renwu->setScale(0.5f);
	renwu->setPosition(scal9->getContentSize().width,scal9->getContentSize().height/2);
	scal9->addChild(renwu,100);


	auto label_sm=Label::createWithSystemFont(FX::StringsMap::getValue("jjpmjl"),"minijiankatong.ttf",30);
	label_sm->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.8));
	label_sm->setColor(Color3B(255,33,33));
	scal9->addChild(label_sm);

	//
	const Unit *unit=GoodsHelper::getHelper()->getUnitByID(bag.id);
	auto sp_i=Sprite::create(unit->imageFile);
	sp_i->setPosition(Vec2(40+sp_i->getContentSize().width/2,scal9->getContentSize().height-10-sp_i->getContentSize().height/2));
	scal9->addChild(sp_i,100);


	auto label=Label::createWithSystemFont(__String::createWithFormat("%d",bag.value)->getCString(),"minijiankatong.ttf",24);
	label->setColor(Color3B(255,33,33));
	label->setPosition(Vec2(sp_i->getPositionX(),scal9->getContentSize().height*0.4));
	scal9->addChild(label,100);

	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BaseLayer::daoju_reward2,this));
	close->setPosition(Vec2(scal9->getContentSize().width-close->getContentSize().width/2,scal9->getContentSize().height-close->getContentSize().height/2));
    
	
    
	GoodsHelper::getHelper()->changeOneUnitCount(bag.id,bag.value);
	auto labl=Label::createWithSystemFont(FX::StringsMap::getValue("yilinju"),"minijiankatong.ttf",24);
	labl->setColor(Color3B(255,33,33));
	labl->setRotation(-45.0f);
	labl->setPosition(Vec2(scal9->getContentSize().width*0.6,scal9->getContentSize().height*0.4));
	scal9->addChild(labl,100);
	

	auto menu = Menu::create(close,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu, 2);
	showShake(scal9);
}

void BaseLayer::linqu_active(Ref* ref){
	  
	 //
	 MenuItemImage* p=(MenuItemImage*)ref;
	 auto label=Label::createWithSystemFont(FX::StringsMap::getValue("yilinju"),"minijiankatong.ttf",24);
	 label->setColor(Color3B(255,33,33));
	 label->setRotation(-45.0f);
	 label->setPosition(Vec2(ww->getContentSize().width*0.6,ww->getContentSize().height*0.4));
	 ww->addChild(label,100);
	 p->setVisible(false);
}

void BaseLayer::Conset(){

	float prent=0.0;
    float q_x=(float)ActiveValueHelper::getHelper()->getHasActiveCount();
    float h_x=100;
    prent=(q_x/h_x)*100.0f;
    progress->setPercentage(prent);

	string bfb_str=__String::createWithFormat("%d",(int)q_x)->getCString();
	bfb_str+="/";
    bfb_str+=__String::createWithFormat("%d",(int)h_x)->getCString();
	b_l->setString(bfb_str);
}

void BaseLayer::daoju_reward2(Ref* sender){

	
	this->removeFromParent();
}

void BaseLayer::daoju_reward(Ref* sender){

	if(ActiveValueHelper::getHelper()->canGetGift()){
		auto layer=BaseLayer::create();
		layer->active_warldbox(ActiveValueHelper::getHelper()->getOneGift_useActiveNum());
		Director::getInstance()->getRunningScene()->addChild(layer,1000);
	}
	this->removeFromParent();
}



void  BaseLayer::setJiemian_box(){


	
	 
}



void BaseLayer::Close_button(Ref* ref){

	this->removeFromParent();

}

void BaseLayer::Yinyueset_button(Ref* ref){

	
}


void BaseLayer::drawSetmusic(){

	
	setOparatetag(yinyue_set_tag);
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);
	 
	auto blayer=Layer::create();
	addChild(blayer,100);
	blayer->setTag(blayertag);
	//yinyue_o_tag, yinxiao_o_tag,
	auto s_kimg=MenuItemImage::create("UI_denglu_shezhi_bg.png","UI_denglu_shezhi_bg.png",CC_CALLBACK_1(BaseLayer::xinshouCallback,this));
	s_kimg->setPosition(Vec2(Actual_x/2,Actual_y*0.64));
	s_kimg->setTag(yinyue_o_tag);
	
	


    
	auto yinyue=Sprite::create("UI_denglu_shezhi_yinyue.png");
	float skx=s_kimg->getPositionX()-s_kimg->getContentSize().width/2-50+yinyue->getContentSize().width/2;
	float sky=s_kimg->getPositionY();
	yinyue->setTag(yinyue_qian_kuan);
	yinyue->setPosition(Vec2(skx,sky));
	blayer->addChild(yinyue);

	auto ms_k=Sprite::create("UI_denglu_shezhi_gou_bg.png");
	ms_k->setPosition(Vec2(s_kimg->getPositionX()+s_kimg->getContentSize().width/2-ms_k->getContentSize().width/2+30,s_kimg->getPositionY()));
	ms_k->setTag(yinyue_hou_kuan);
	blayer->addChild(ms_k);

	string str_yinyue=FX::StringsMap::getValue("yinyuek");
	if(PlayerInfoHelper::getHelper()->getGameLock(PlayerInfoHelper::MusicLock)==false){

		auto yy_x=Sprite::create("UI_denglu_shezhi_guanbi.png");
		yy_x->setPosition(Vec2(yinyue->getContentSize().width/2,yinyue->getContentSize().height/2));
		yy_x->setTag(100);
		yinyue->addChild(yy_x);
		str_yinyue=FX::StringsMap::getValue("yinyueg");
		FX::Music::getInstance()->setMusicEnable(false);
	}else{
		auto yy_j=Sprite::create("UI_denglu_shezhi_gou.png");
		yy_j->setPosition(Vec2(ms_k->getContentSize().width/2,ms_k->getContentSize().height/2));
		yy_j->setTag(100);
		ms_k->addChild(yy_j);
		FX::Music::getInstance()->setMusicEnable(true);
	}

	Label* y_p=Label::createWithSystemFont(str_yinyue,"minijiankatong.ttf",40);
	y_p->setPosition(Vec2(s_kimg->getPositionX(),s_kimg->getPositionY()));
	y_p->setTag(yinyueone_b);
	y_p->setColor(Color3B(254,218,185));
	y_p->setLocalZOrder(100);
	blayer->addChild(y_p);

	Label* y_p2=Label::createWithSystemFont(str_yinyue,"minijiankatong.ttf",40);
	y_p2->setPosition(Vec2(s_kimg->getPositionX(),s_kimg->getPositionY()-1));
	
	y_p2->setTag(yinyuetwo_b);
	y_p2->setLocalZOrder(90);
	blayer->addChild(y_p2);


	auto x_kimg=MenuItemImage::create("UI_denglu_shezhi_bg.png","UI_denglu_shezhi_bg.png",CC_CALLBACK_1(BaseLayer::xinshouCallback,this));
	x_kimg->setPosition(Vec2(Actual_x/2,Actual_y*0.43));
	x_kimg->setTag(yinxiao_o_tag);
	



	auto yinxiao=Sprite::create("UI_denglu_shezhi_yinxiao.png");
	yinxiao->setTag(yinxiao_qian_kuan);
	yinxiao->setPosition(Vec2(x_kimg->getPositionX()-x_kimg->getContentSize().width/2-50+yinxiao->getContentSize().width/2,x_kimg->getPositionY()));
	blayer->addChild(yinxiao);


	auto ms_kx=Sprite::create("UI_denglu_shezhi_gou_bg.png");
	ms_kx->setTag(yinxiao_hou_kuan);
	ms_kx->setPosition(Vec2(x_kimg->getPositionX()+x_kimg->getContentSize().width/2-ms_kx->getContentSize().width/2+30,x_kimg->getPositionY()));
	blayer->addChild(ms_kx,100);

	string str_xiao=FX::StringsMap::getValue("yinxiaok");
	if(PlayerInfoHelper::getHelper()->getGameLock(PlayerInfoHelper::MusciEffectLock)==false){
		auto yy_xdom=Sprite::create("UI_denglu_shezhi_guanbi.png");
		yy_xdom->setPosition(Vec2(yinxiao->getContentSize().width/2,yinxiao->getContentSize().height/2));
		yy_xdom->setTag(100);
		yinxiao->addChild(yy_xdom);
		str_xiao=FX::StringsMap::getValue("yinxiaog");
	}else{
		auto yy_jdom=Sprite::create("UI_denglu_shezhi_gou.png");
		yy_jdom->setPosition(Vec2(ms_kx->getContentSize().width/2,ms_kx->getContentSize().height/2));
		yy_jdom->setTag(100);
		ms_kx->addChild(yy_jdom);
	}
	Label* x_p=Label::createWithSystemFont(str_xiao,"minijiankatong.ttf",40);
	x_p->setTag(yinxiao_one_tag);
	x_p->setPosition(Vec2(x_kimg->getPositionX(),x_kimg->getPositionY()));
	x_p->setLocalZOrder(100);
	x_p->setColor(Color3B(254,218,185));
	blayer->addChild(x_p);

	Label* x_p2=Label::createWithSystemFont(str_xiao,"minijiankatong.ttf",40);
	x_p2->setTag(yinxiao_two_tag);
	x_p2->setPosition(Vec2(x_kimg->getPositionX(),x_kimg->getPositionY()-1));
	x_p2->setLocalZOrder(90);
	
	blayer->addChild(x_p2);


	// close_set_layer_tag,xinshou_yindao_tag,

	auto close_tagsp=MenuItemImage::create("UI_jiaose_anniu_guanbi.png","UI_jiaose_anniu_guanbi.png",CC_CALLBACK_1(BaseLayer::xinshouCallback,this));
	close_tagsp->setTag(close_set_layer_tag);
	close_tagsp->setPosition(Vec2(Actual_x-close_tagsp->getContentSize().width/2,Actual_y-close_tagsp->getContentSize().height/2));
	fillOuty(close_tagsp,"UI_jiaose_anniu_guanbi.png");
	



	//xinshoujianxue
	auto xinshou_sp=MenuItemImage::create("UI_jiaose_anniu_shengji_manji.png","UI_jiaose_anniu_shengji_manji.png",CC_CALLBACK_1(BaseLayer::xinshouCallback,this));
	xinshou_sp->setTag(xinshou_yindao_tag);
	xinshou_sp->setPosition(Vec2(Actual_x/2,Actual_y*0.23));
	fillOuty(xinshou_sp,"UI_jiaose_anniu_shengji_manji.png");


	auto menu=Menu::create(x_kimg,s_kimg,close_tagsp,xinshou_sp,nullptr);
	menu->setPosition(Vec2::ZERO);
	menu->setTag(menu_tag);
	blayer->addChild(menu,-10);


    
	Label* xinshoulable=Label::createWithSystemFont(FX::StringsMap::getValue("xinshoujianxue"),"minijiankatong.ttf",30);
	xinshoulable->setPosition(Vec2(xinshou_sp->getContentSize().width/2,xinshou_sp->getContentSize().height/2));
	xinshoulable->setLocalZOrder(100);
	xinshoulable->setColor(Color3B(254,218,185));
	xinshou_sp->addChild(xinshoulable);

	Label* xinshoulable1=Label::createWithSystemFont(FX::StringsMap::getValue("xinshoujianxue"),"minijiankatong.ttf",30);
	xinshoulable1->setLocalZOrder(90);
	xinshoulable1->setPosition(Vec2(xinshou_sp->getContentSize().width/2,xinshou_sp->getContentSize().height/2-1));
	xinshou_sp->addChild(xinshoulable1);
}

void BaseLayer::fillOuty(MenuItemImage* p,string imgfile){

	auto s=Sprite::create(imgfile);
	s->setScale(1.05f);
	p->setSelectedImage(s);
}

void BaseLayer::xinshouCallback(Ref* ref){

	 FX::Music::getInstance()->buttonClicked();
	 MenuItemImage* p=(MenuItemImage*)ref;
	 if(p->getTag()==xinshou_yindao_tag){
	   GameDirector::getDirector()->loadingToRunningScene(RunningMode::teach);
	 }else if(p->getTag()==close_set_layer_tag){

		   EventCustom event(Layerchange);
		   LayerData data;
		   data.current=Layertag::gold_tag_close;
		   data.push=0;
		   event.setUserData(&data);
		   Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
		   this->removeFromParent();
	 }else if(p->getTag()==yinyue_o_tag){
					 
		             //Sprite* m=(Sprite*)this->getChildByTag(blayertag)->getChildByTag(menu_tag)->getChildByTag(state);
					 Sprite* n=(Sprite*)this->getChildByTag(blayertag)->getChildByTag(yinyue_qian_kuan);
					 Sprite* mn=(Sprite*)this->getChildByTag(blayertag)->getChildByTag(yinyue_hou_kuan);

								Label* lo=(Label*)this->getChildByTag(blayertag)->getChildByTag(yinyueone_b);
								Label* lt=(Label*)this->getChildByTag(blayertag)->getChildByTag(yinyuetwo_b);
								if(PlayerInfoHelper::getHelper()->getGameLock(PlayerInfoHelper::MusicLock)==true){

									lo->setString(FX::StringsMap::getValue("yinyueg"));
									lt->setString(FX::StringsMap::getValue("yinyueg"));
									auto yy_xdom=Sprite::create("UI_denglu_shezhi_guanbi.png");
									yy_xdom->setPosition(Vec2(n->getContentSize().width/2,n->getContentSize().height/2));
									yy_xdom->setTag(100);
									n->addChild(yy_xdom);
									mn->removeChildByTag(100);
									FX::Music::getInstance()->setMusicEnable(false);
								    PlayerInfoHelper::getHelper()->setGameLock(PlayerInfoHelper::MusicLock,false);
								}else{

									n->removeChildByTag(100);
									lo->setString(FX::StringsMap::getValue("yinyuek"));
									lt->setString(FX::StringsMap::getValue("yinyuek"));
									auto yy_jdom=Sprite::create("UI_denglu_shezhi_gou.png");
									yy_jdom->setPosition(Vec2(mn->getContentSize().width/2,mn->getContentSize().height/2));
									yy_jdom->setTag(100);
									mn->addChild(yy_jdom);
									FX::Music::getInstance()->setMusicEnable(true);
								    PlayerInfoHelper::getHelper()->setGameLock(PlayerInfoHelper::MusicLock,true);
								}
		}else if(p->getTag()==yinxiao_o_tag){

					             /*
								    yinxiao_qian_kuan,
	                                yinxiao_hou_kuan,
								 */
								 //Sprite* m=(Sprite*)this->getChildByTag(blayertag)->getChildByTag(menu_tag)->getChildByTag(state);
								 Sprite* n=(Sprite*)this->getChildByTag(blayertag)->getChildByTag(yinxiao_qian_kuan);
								 Sprite* mn=(Sprite*)this->getChildByTag(blayertag)->getChildByTag(yinxiao_hou_kuan);

					
								Label* lo=(Label*)this->getChildByTag(blayertag)->getChildByTag(yinxiao_one_tag);
								Label* lt=(Label*)this->getChildByTag(blayertag)->getChildByTag(yinxiao_two_tag);
								if(PlayerInfoHelper::getHelper()->getGameLock(PlayerInfoHelper::MusciEffectLock)==true){

									lo->setString(FX::StringsMap::getValue("yinxiaog"));
									lt->setString(FX::StringsMap::getValue("yinxiaog"));
									auto yy_xdom=Sprite::create("UI_denglu_shezhi_guanbi.png");
									yy_xdom->setPosition(Vec2(n->getContentSize().width/2,n->getContentSize().height/2));
									yy_xdom->setTag(100);
									n->addChild(yy_xdom);
									mn->removeChildByTag(100);
									PlayerInfoHelper::getHelper()->setGameLock(PlayerInfoHelper::MusciEffectLock,false);
								}else{

									n->removeChildByTag(100);
									lo->setString(FX::StringsMap::getValue("yinxiaok"));
									lt->setString(FX::StringsMap::getValue("yinxiaok"));
									auto yy_jdom=Sprite::create("UI_denglu_shezhi_gou.png");
									yy_jdom->setPosition(Vec2(mn->getContentSize().width/2,mn->getContentSize().height/2));
									yy_jdom->setTag(100);
									mn->addChild(yy_jdom);
									PlayerInfoHelper::getHelper()->setGameLock(PlayerInfoHelper::MusciEffectLock,true);
								}

	  }

}

int BaseLayer::getOparatetag(){

	 return opratetag;
}
void BaseLayer::setOparatetag(int tag){

	 opratetag=tag;
}

bool BaseLayer::isContactpoint(Vec2 p,Sprite* sp){
	  
	  float widthl=sp->getPositionX()-sp->getContentSize().width/2;
	  float widthr=sp->getPositionX()+sp->getContentSize().width/2;
	  float hightx=sp->getPositionY()-sp->getContentSize().height/2;
	  float hights=sp->getPositionY()+sp->getContentSize().height/2;
	  if(widthl<p.x&&p.x<widthr&&hightx<p.y&&p.y<hights){
		  FX::Music::getInstance()->buttonClicked();
		  return true;
	  }else{
		  return false;
	  }
}

void BaseLayer::Shieldtouched(){

	
	//
}