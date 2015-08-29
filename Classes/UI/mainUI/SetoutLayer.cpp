#include "SetoutLayer.h"
#include "GameDirector.h"
#include "ActiveValueHelper.h"


SetoutLayer::SetoutLayer(void)
{
}


SetoutLayer::~SetoutLayer(void)
{
}


bool SetoutLayer::init(){

	if(!Layer::init()){
		return false;
	}
	setoutInit();
	return true;
}
void SetoutLayer::onEnter(){

	Layer::onEnter();
}
void SetoutLayer::onExit(){
    
	Layer::onExit();
}


float SetoutLayer::getDelwithint(float number,int iswidth){
	  
	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;
	  
}

void SetoutLayer::setoutInit(){

	 //所需数据初始化
	 Actorinint();
	 //界面布局
	 Drawinterface();
	 //消息回调
	 rigistdaoju();
}



void SetoutLayer::Actorinint(){

	hero=ActorHelper::getHelper()->getOneHero(PlayerInfoHelper::getHelper()->getDeafaultHero());
	pet=ActorHelper::getHelper()->getOnePet(PlayerInfoHelper::getHelper()->getDeafaultPet());
	magic=(BagItem*)GoodsHelper::getHelper()->getOneItem(PlayerInfoHelper::getHelper()->getDeafaultMagicItem());
}


void SetoutLayer::Drawinterface(){

	 
	 Size visibleSize = Director::getInstance()->getWinSize();
	 Actual_x=visibleSize.width;
	 Actual_y=visibleSize.height;
	 this->setContentSize(Size(Actual_x,(Design_hight-Tophight)*(Actual_y/Design_hight)));
	 

	 float x_width= getDelwithint(0,0);

	 //魔法物 人物 宠物
	string hero_str=ImageHelper::getUnitSpriteUIScene(hero.unit.ID)->getName();
	auto hero_default=MenuItemImage::create(hero_str,hero_str,CC_CALLBACK_1(SetoutLayer::juesebuttonCallback,this));
	hero_default->setNormalImage(ImageHelper::getUnitSpriteUIScene(hero.unit.ID));
	hero_default->setSelectedImage(ImageHelper::getUnitSpriteUIScene(hero.unit.ID));
	hero_default->setScale(0.85f);
	hero_default->setAnchorPoint(Vec2(0.5,0.0));
	
	string namekl=hero_default->getName();
	hero_default->setPosition(Vec2(getContentSize().width*0.25+x_width,getContentSize().height*0.4));
	hero_default->setOpacity(0.0f);
	string animationData="lanbenben";
	if(hero.unit.ID==unitID_Actor1){
		animationData="lanbenben";
	}else if(hero.unit.ID==unitID_Actor2){
		animationData="lanbaba";
		hero_default->setPosition(Vec2(getContentSize().width*0.25+x_width,getContentSize().height*0.2));
	}

	playAnimationByname(this,"huxidonghua.ExportJson","huxidonghua",animationData,true,false,Vec2(hero_default->getPositionX(),hero_default->getPositionY()+hero_default->getContentSize().width*0.85f));




	MenuItemImage* hero_default1=nullptr;
	if(pet.unit.ID!=unitID_None){

		string hero_str=ImageHelper::getUnitSpriteUIScene(pet.unit.ID)->getName();
		CCLOG("---->%s",hero_str.c_str());
		hero_default1=MenuItemImage::create(hero_str,hero_str,CC_CALLBACK_1(SetoutLayer::cw_buttonCallback,this));
		hero_default1->setNormalImage(ImageHelper::getUnitSpriteUIScene(pet.unit.ID));
		hero_default1->setSelectedImage(ImageHelper::getUnitSpriteUIScene(pet.unit.ID));
	    hero_default1->setAnchorPoint(Vec2(0.0f,0.0f));
		hero_default1->setOpacity(0.0f);
	    string namekl=hero_default1->getName();
		float pet_y=hero_default1->getPositionX()+hero_default1->getContentSize().width/2+hero_default1->getContentSize().width/2;


		string animationData="limao";
		float pet_yh=getContentSize().height*0.3;
		float pet_xh=getContentSize().width*0.3+x_width;
		if(pet.unit.ID==unitID_PiPiZhu){
			animationData="limao";
			pet_y=hero_default1->getPositionX()+hero_default1->getContentSize().width/1.5;
			pet_yh=getContentSize().height*0.28;
			pet_xh=(getContentSize().width)*0.32+x_width;
		}else if(pet.unit.ID==unitID_XiXiaoYang ){
			animationData="xiaoxiong";
			pet_y=hero_default1->getPositionX()+hero_default1->getContentSize().width/2;
			pet_yh=getContentSize().height*0.28;
			pet_xh=(getContentSize().width)*0.32+x_width;
		}else if(pet.unit.ID==unitID_BuGuNiao	){
			animationData="maotouying";
			pet_y=hero_default1->getPositionX()+hero_default1->getContentSize().width/2;
			pet_yh=getContentSize().height*0.3;
			pet_xh=(getContentSize().width)*0.32+x_width;
		}
		hero_default1->setPosition(Vec2(pet_xh,pet_yh));
	     
		playAnimationByname(this,"huxidonghua.ExportJson","huxidonghua",animationData,true,false,Vec2(hero_default1->getPositionX()+hero_default1->getPositionX()*0.3f-37,hero_default1->getPositionY()+hero_default1->getPositionY()*0.4f));
	}


	MenuItemImage* hero_default2=nullptr;
	if(magic!=nullptr){
		string hero_str=ImageHelper::getUnitSpriteUIScene(magic->id,magic->value)->getName();
		hero_default2=MenuItemImage::create(hero_str,hero_str,CC_CALLBACK_1(SetoutLayer::gj_buttonCallback,this));
		hero_default2->setNormalImage(ImageHelper::getUnitSpriteUIScene(magic->id,magic->value));
		hero_default2->setSelectedImage(ImageHelper::getUnitSpriteUIScene(magic->id,magic->value));
		hero_default2->setScale(0.9f);
		hero_default2->setOpacity(0.0f);

		if(hero.unit.ID==unitID_Actor2){
		   hero_default2->setPosition(Vec2((getContentSize().width)*0.125+x_width,getContentSize().height*0.9));
		}else{
		   hero_default2->setPosition(Vec2((getContentSize().width)*0.15+x_width,getContentSize().height*0.9));

		}
		string animationData="xingxing";
		
		if(magic->id==unitID_FaGuangWuXing){
		   animationData="xingxing";
	    }else if(magic->id==unitID_JiXingHuoJian){
		   animationData="danzai";
	    }else if(magic->id==unitID_MoLiYingHuo){
		   animationData="mofayinghuo";
	   }
		playAnimationByname(this,"huxidonghua.ExportJson","huxidonghua",animationData,true,false,Vec2(hero_default2->getPositionX(),hero_default2->getPositionY()));
	}


	scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setContentSize(Size(499,395));// 459,365
	scal9->setPosition(Vec2(getContentSize().width*0.7,getContentSize().height*0.65));
	addChild(scal9,1);

	auto ts_sm=Sprite::create("ui_anniu_zhuye_daoju.png");
	ts_sm->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height));
	scal9->addChild(ts_sm,100);


	sp=Sprite::create("ui_sence_zanting_bg_tiao.png");
	sp->setPosition(Vec2(scal9->getContentSize().width/2,0));
	scal9->addChild(sp,100);
	sp->setVisible(false);

	auto label_xs=Label::createWithSystemFont("","Arial",24);
	label_xs->setPosition(Vec2(sp->getContentSize().width/2,sp->getContentSize().height/2));
	//label_xs->setColor(Color3B(255,33,33));
	sp->addChild(label_xs,1,100);




	list=BaselisttableLayer::create();
	list->setMtabletype(Daoju_list,Size(499,100),3,cocos2d::extension::ScrollView::Direction::VERTICAL,TableView::VerticalFillOrder::TOP_DOWN,0);
	list->ignoreAnchorPointForPosition(false);
	list->setAnchorPoint(Vec2(0.5f,0.5f));
	list->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.5));
	scal9->addChild(list,99);
    

	auto wj_button=MenuItemImage::create("anniu_zhuye_chufa_wujin.png","anniu_zhuye_chufa_wujin.png",CC_CALLBACK_1(SetoutLayer::wj_buttonCallback,this));
	wj_button->setPosition(Vec2(scal9->getPositionX(),wj_button->getContentSize().height/2));


	auto menu=Menu::create(hero_default,hero_default1,hero_default2,nullptr);
	menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 100);

    auto menu1=Menu::create(wj_button,nullptr);
	menu1->setPosition(Vec2::ZERO);
    this->addChild(menu1, 100);

	{
		auto teachMenu = MenuItemFont::create("Teach", [](Ref* obj){
			GameDirector::getDirector()->loadingToRunningScene(RunningMode::teach);
		});
		teachMenu->setPosition(Vec2(Director::getInstance()->getWinSize().width-100, 50));
		teachMenu->setColor(Color3B::RED);
		menu1->addChild(teachMenu);
	}
}



//动画播放-----》

void SetoutLayer::playAnimationByname(Node *bs,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscal,Vec2 p){
	  
	auto armature =Armature::create(filename.c_str());
	if(isloop==true){
		armature->getAnimation()->play(animationName.c_str(),-1,1);
	}else{
	   armature->getAnimation()->play(animationName.c_str());
	   armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(SetoutLayer::callfunc_AnimateEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	armature->removeFromParentAndCleanup(true);
	if(isscal==true){
	   armature->setScale(1.25);

	}
	
	armature->setPosition(p);
	armature->setTag(100);
	armature->setLocalZOrder(100);
	bs->addChild(armature);
	 

}

void SetoutLayer::callfunc_AnimateEnd(Armature *ar, MovementEventType movementType, const std::string& movementID){

	 ar->removeFromParent();
	 Label* p=(Label*)sp->getChildByTag(100);
	 const  Unit* unit=GoodsHelper::getHelper()->getUnitByID(GoodsHelper::getHelper()->getRandomMarketItem());//  data.errorCode);
	 string ms=unit->name+": "+__String::createWithFormat("%d",tag_unit)->getCString();
	 p->setString(ms);
	 GoodsHelper::getHelper()->changeOneUnitCount(unit->ID,tag_unit);


}



//按钮触发
void SetoutLayer::juesebuttonCallback(Ref* ref){

	 
}
void SetoutLayer::cw_buttonCallback(Ref* ref){


}
void SetoutLayer::gj_buttonCallback(Ref* ref){


}


void  SetoutLayer::wj_buttonCallback(Ref* ref){

	int iCount = PlayerInfoDataManager::getManager()->getOnePlayerInfo(key_playGameCount);
	PlayerInfoDataManager::getManager()->setOnePlayerInfo(key_playGameCount, iCount+1);

	Achieve* info = nullptr;
	//检测痴狂相关成就
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiKuangI);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiKuangI, 1);
	}
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiKuangII);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiKuangII, 1);
	}
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiKuangIII);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiKuangIII, 1);
	}
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiKuangIV);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiKuangIV, 1);
	}

	 FX::Music::getInstance()->buttonClicked();
	 GameDirector::getDirector()->loadingToRunningScene(RunningMode::endless);

}



void SetoutLayer::rigistdaoju(){
	  

	 

}


void SetoutLayer::randxiaog(unsigned int tag){

	if(sp->isVisible()==false){
		sp->setVisible(true);
	}
	Label* p=(Label*)sp->getChildByTag(100);
	p->setString("");
	//播放动画
    tag_unit=tag;
	playAnimationByname(sp,"Run_shengji_xiaoguo.ExportJson","Run_shengji_xiaoguo","xiaoguo",false,false,Vec2(getDelwithint(310+sp->getContentSize().width/2,1),sp->getContentSize().height/2-getDelwithint(25,1)));

}

