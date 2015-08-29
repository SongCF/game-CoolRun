#include "StartLayer.h"
#include "BaselisttableLayer.h"
#include "MainuiLayer.h"
#include "Uiscene.h"
#include "Preload.h"
#include "StringsMap.h"
#include "Music.h"
#include "AccountsLayer.h"
#include "HeroSprite.h"
#include "PlayerInfoHelper.h"
//#include "CutomTableshop.h"
#include "protocol.h"
#include "EditBoxTest.h"
#include "PlayerNameFactory.h"
#include "Uistruct.h"
#include "ShopLayer.h"
#include "ActiveValueHelper.h"

#define  ipstr "192.168.2.11"

static  int  Dk=23577; 

bool StartLayer::init(){
	if(!Layer::init()){
		return false;
	}

	
 
	yidian=0;
	
	
	shifouyi=false;
	start_c=false;

	na=0;
	guanxuiao=0;
	gegewu=0;
	bitao=0;
	aniu=0;
    inintStartLayer();

	//
	return true;
}


void StartLayer::onEnter(){

	Layer::onEnter();
	
}
void StartLayer::onExit(){
	
	this->unscheduleAllSelectors();
	Layer::onExit();
}


StartLayer::StartLayer(void)
{
	
}


StartLayer::~StartLayer(void)
{
	
}


void StartLayer::inintStartLayer(){

	

	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	
	auto back=Sprite::create("ui_kaishi_bg.jpg");
	back->setPosition(Vec2(Actual_x/2,Actual_y/2));
	addChild(back);
	
	auto backbom=Sprite::create("ui_kaishi_bgbom.png");
	backbom->setPosition(Vec2(Actual_x/2,backbom->getContentSize().height/2));
	addChild(backbom,200);


	

	CCLOG("---->%s",PlayerInfoHelper::getHelper()->getPlayerName().c_str());
	if(PlayerInfoHelper::getHelper()->getPlayerName().size()==0){
         //名字输入
		 write_nameBox();
	}else{
		 setAftername();
	}

	



}


void StartLayer::write_nameBox(){
	


	scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(400,200));// 
	addChild(scal9);



	auto bg=Sprite::create("ui_zhuye_biaoti.png");
	bg->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.65));
	scal9->addChild(bg);


	write_name=EditBoxTest::create();
	write_name->shuru_name("Shuruqu");
	write_name->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height/2));
	bg->addChild(write_name);


	auto need_label=Label::createWithSystemFont(FX::StringsMap::getValue("Shuruxinyinichen"),"Arial",24);
	need_label->setColor(Color3B(0,0,0));
	need_label->setPosition(Vec2(40+need_label->getContentSize().width/2,scal9->getContentSize().height*0.28));
	scal9->addChild(need_label);

	
	auto mody_butoon=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(StartLayer::write_Button,this));
	mody_butoon->setPosition(Vec2(scal9->getContentSize().width,scal9->getContentSize().height/2));
	this->setTextb(mody_butoon,"qding",30,Color3B(0,0,0));
	


	auto menu = Menu::create(mody_butoon,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu, 125);

	showShake(scal9);

	 
}

void StartLayer::setJiemian_box(){
	 


	

	scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(400,250));// 
	addChild(scal9,1025);


	Shieldtouched(scal9);

	auto set_img=Sprite::create("ui_anniu_tongyong_zhuye_lanse.png");
	set_img->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height));
	scal9->addChild(set_img,1025);

	auto set_text=Label::createWithSystemFont(FX::StringsMap::getValue("set"),"Arial",24);
	set_text->setColor(Color3B(0,0,0));
	set_text->setPosition(Vec2(set_img->getContentSize().width/2,set_img->getContentSize().height/2));
	set_img->addChild(set_text);

	
	
	string yiyue_str="Guangbi";
	string yinyue_b="ui_anniu_tongyong_zhuye_lanse.png";
	string yinyue_bc="ui_anniu_tongyong_zhuye_lanse_select.png";
	/*

	MusicLock,
		MusciEffectLock,
		SpecialEffectLock,
	*/
	
	if( PlayerInfoHelper::getHelper()->getGameLock(PlayerInfoHelper::MusicLock)==false){
		yiyue_str="Guangbi";
	    yinyue_b="ui_anniu_tongyong_zhuye_lanse.png";
	    yinyue_bc="ui_anniu_tongyong_zhuye_lanse_select.png";
	}else{
		yiyue_str="Kaiqi";
		yinyue_b="ui_anniu_tongyong_zhuye_lanse_select.png";
	    yinyue_bc="ui_anniu_tongyong_zhuye_lanse.png";
	}

	string yixiao_str="Guangbi";
	string yinxiao_b="ui_anniu_tongyong_zhuye_lanse.png";
	string yinxiao_bc="ui_anniu_tongyong_zhuye_lanse_select.png";
	if(PlayerInfoHelper::getHelper()->getGameLock(PlayerInfoHelper::MusciEffectLock)==false){
		 yixiao_str="Guangbi";
		 yinxiao_b="ui_anniu_tongyong_zhuye_lanse.png";
	     yinxiao_bc="ui_anniu_tongyong_zhuye_lanse_select.png";
	}else{
		 yixiao_str="Kaiqi";
		 yinxiao_b="ui_anniu_tongyong_zhuye_lanse_select.png";
	     yinxiao_bc="ui_anniu_tongyong_zhuye_lanse.png";
	}
	
	string texiao_str="Guangbi";
	string texiao_b="ui_anniu_tongyong_zhuye_lanse.png";
	string texiao_bc="ui_anniu_tongyong_zhuye_lanse_select.png";
	if(PlayerInfoHelper::getHelper()->getGameLock(PlayerInfoHelper::SpecialEffectLock)==false){
		texiao_str="Guangbi";
		texiao_b="ui_anniu_tongyong_zhuye_lanse.png";
	    texiao_bc="ui_anniu_tongyong_zhuye_lanse_select.png";
	}else{
		texiao_str="Kaiqi";
		texiao_b="ui_anniu_tongyong_zhuye_lanse_select.png";
	    texiao_bc="ui_anniu_tongyong_zhuye_lanse.png";
	}

	//音乐
	auto label_yy=Label::createWithSystemFont(FX::StringsMap::getValue("Yinyue"),"Arial",30);
	label_yy->setColor(Color3B(255,33,33));
	label_yy->setPosition(Vec2(scal9->getContentSize().width*0.2,scal9->getContentSize().height*0.7));
	scal9->addChild(label_yy);
	auto yy_b=MenuItemImage::create(yinyue_b,yinyue_bc,CC_CALLBACK_1(StartLayer::Yinyueset_button,this));
	yy_b->setPosition(Vec2(scal9->getContentSize().width*0.6,label_yy->getPositionY()));

    
	Label* yy_bl=Label::createWithSystemFont(FX::StringsMap::getValue(yiyue_str),"Arial",30);
	yy_bl->setPosition(Vec2(yy_b->getContentSize().width/2,yy_b->getContentSize().height/2));
	yy_b->addChild(yy_bl,1,100);
	yy_b->setScale(0.85f);
	yy_b->setTag(100);



	

	//音效
	auto label_yx=Label::createWithSystemFont(FX::StringsMap::getValue("Yinxiao"),"Arial",30);
	label_yx->setColor(Color3B(255,33,33));
	label_yx->setPosition(Vec2(scal9->getContentSize().width*0.2,scal9->getContentSize().height*0.5));
	scal9->addChild(label_yx);
	auto yy_x=MenuItemImage::create(yinxiao_b,yinxiao_bc,CC_CALLBACK_1(StartLayer::Yinyueset_button,this));
	yy_x->setPosition(Vec2(scal9->getContentSize().width*0.6,label_yx->getPositionY()));

   

	Label* yy_xl=Label::createWithSystemFont(FX::StringsMap::getValue(yixiao_str),"Arial",30);
	yy_xl->setPosition(Vec2(yy_x->getContentSize().width/2,yy_b->getContentSize().height/2));
	yy_x->addChild(yy_xl,1,100);
	yy_x->setScale(0.85f);
	yy_x->setTag(101);

    
	//特效
	auto label_tx=Label::createWithSystemFont(FX::StringsMap::getValue("Texiao"),"Arial",30);
	label_tx->setColor(Color3B(255,33,33));
	label_tx->setPosition(Vec2(scal9->getContentSize().width*0.2,scal9->getContentSize().height*0.3));
	scal9->addChild(label_tx);
	auto yy_t=MenuItemImage::create(texiao_b,texiao_bc,CC_CALLBACK_1(StartLayer::Yinyueset_button,this));
	yy_t->setPosition(Vec2(scal9->getContentSize().width*0.6,label_tx->getPositionY()));
	Label* yy_tl=Label::createWithSystemFont(FX::StringsMap::getValue(texiao_str),"Arial",30);
	yy_tl->setPosition(Vec2(yy_t->getContentSize().width/2,yy_t->getContentSize().height/2));
	yy_t->addChild(yy_tl,1,100);
	yy_t->setScale(0.85f);
	yy_t->setTag(102);

	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(StartLayer::Close_button,this));
	close->setPosition(Vec2(scal9->getContentSize().width- close->getContentSize().width/2,scal9->getContentSize().height-close->getContentSize().height/2));

	auto menu = Menu::create(close,yy_b,yy_x,yy_t,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu,1025);


	showShake(scal9);
}

void StartLayer::Close_button(Ref* ref){

	scal9->removeFromParent();
	shifouyi=false;
}

void StartLayer::Yinyueset_button(Ref* ref){

	

	MenuItemImage* p=(MenuItemImage*)ref;
	int tag=p->getTag();
	Label* p_l=(Label*)p->getChildByTag(100);
	string name=p_l->getString();

	string Kq=FX::StringsMap::getValue("Kaiqi");
	string Gp=FX::StringsMap::getValue("Guangbi");

	auto Kq_img=Sprite::create("ui_anniu_tongyong_zhuye_lanse_select.png");
	auto Gp_img=Sprite::create("ui_anniu_tongyong_zhuye_lanse.png");

	
	if(name.compare(Gp)==0){ // 
		p_l->setString(Kq);
		p->setNormalImage(Kq_img);
		p->setSelectedImage(Gp_img);
	}else{
		p_l->setString(Gp);
		p->setNormalImage(Gp_img);
		p->setSelectedImage(Kq_img);
	}

	if(tag==100){
       
		if(name.compare(Gp)==0){ // 
			FX::Music::getInstance()->setMusicEnable(false);
			PlayerInfoHelper::getHelper()->setGameLock(PlayerInfoHelper::MusicLock,true);
	    }else{
			FX::Music::getInstance()->setMusicEnable(true);
			PlayerInfoHelper::getHelper()->setGameLock(PlayerInfoHelper::MusicLock,false);
		}
        
	}else if(tag==101){
		if(name.compare(Gp)==0){ // 
			PlayerInfoHelper::getHelper()->setGameLock(PlayerInfoHelper::MusciEffectLock,true);
	    }else{
			PlayerInfoHelper::getHelper()->setGameLock(PlayerInfoHelper::MusciEffectLock,false);
		}

	}else if(tag==102){
		if(name.compare(Gp)==0){ // 
			PlayerInfoHelper::getHelper()->setGameLock(PlayerInfoHelper::SpecialEffectLock,true);
	    }else{
			PlayerInfoHelper::getHelper()->setGameLock(PlayerInfoHelper::SpecialEffectLock,false);
		}

	}
}

void StartLayer::write_Button(Ref* ref){


	string name_sr=write_name->context;
	auto cs_namesize=Label::createWithSystemFont(write_name->context,"Arial",24);
	int size=cs_namesize->getContentSize().width;

	auto ctname=Label::createWithSystemFont(FX::StringsMap::getValue("Bzszzm"),"Arial",24);
	int bzsize=ctname->getContentSize().width;

	if(ctname->getContentSize().width<cs_namesize->getContentSize().width){
		write_name->_editName->setText(FX::StringsMap::getValue("zuiduoliugehanzi").c_str());
	}else if(cs_namesize->getContentSize().width==0){
		write_name->_editName->setPlaceHolder(FX::StringsMap::getValue("mingzibunengweikong").c_str());
	}else{

		PlayerInfoHelper::getHelper()->setPlayerName(name_sr);
		scal9->removeFromParent();
		setAftername();
	}

	  
	 
}

void StartLayer::showShake(Node* sp){

	 sp->setScale(0.2f);
	 sp->runAction(Sequence::create(
		FadeOut::create(0),
		Spawn::create(FadeIn::create(0.05f),
		ScaleTo::create(0.1f,1.05f),
		nullptr),
		
		ScaleTo::create(0.1f,1.0f),
		nullptr));
}

void StartLayer::setAftername(){

    
	this->Shieldtouched();
	playAnimationByname(this,"JinRu_donghua.ExportJson","JinRu_donghua","BiaoTi_ChuXian",false,false,Vec2(Actual_x/2,Actual_y/2));
    
	
	
	schedule(schedule_selector(StartLayer::guangXiao),1.8f);

	this->button=MenuItemImage::create("ui_anniu_kaishiyouxi_bg.png","ui_anniu_kaishiyouxi_bg.png",CC_CALLBACK_1(StartLayer::startGameCallback,this));
	button->setPosition(Vec2(Actual_x/2,getDelwithint(30,1)+button->getContentSize().height/2));

	auto buimg= Sprite::create("ui_anniu_kaishiyouxi_bg.png");
	buimg->setColor(Colorzdy);
	button->setSelectedImage(buimg);
	button->setVisible(false);

	regist=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(StartLayer::registCallback,this));
	regist->setPosition(Vec2(regist->getContentSize().width/2,regist->getContentSize().height/2));
	if(PlayerInfoHelper::getHelper()->getUserId()>0){
		//regist->setVisible(false);
	}

	//设置 活动中心
	auto set=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(StartLayer::set_buttonCallback,this));
	set->setPosition(Vec2(set->getContentSize().width/2,set->getContentSize().height/2));
	this->setTextb(set,"set",30,Color3B(0,0,0));
	

	//
	auto hd_zing=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(StartLayer::hd_buttonCallback,this));
	hd_zing->setPosition(Vec2(getContentSize().width-hd_zing->getContentSize().width/2,hd_zing->getContentSize().height/2));
	this->setTextb(hd_zing,"Active_hd",30,Color3B(0,0,0));

	Menu* menu=Menu::create(button,regist,set,hd_zing,nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu,1000);
	startAcitve();
	
	
	schedule(schedule_selector(StartLayer::gegewuChu), 3.0f);

	 
}

void StartLayer::set_buttonCallback(Ref* ref){

	if(shifouyi==true){
		return;
	}
	shifouyi=true;
	setJiemian_box();

}
void StartLayer::hd_buttonCallback(Ref* ref){

}

void StartLayer::startGameCallback(cocos2d::Ref* pSender){

	 if(start_c==false){
	    Gomain();
	    start_c=true;
	 }
}





void StartLayer::updatetime(float time){

	
	
}


void StartLayer::biti2action(){

	
}


void StartLayer::randflicker(){



}


void StartLayer::runHero(Node* sender){

	
	
	//run->setPosition(p3);
	//run->runAction(Sequence::create( MoveTo::create(6.0f,p4),CallFunc::create( CC_CALLBACK_0(StartLayer::runHero,this,nullptr)),nullptr));
	run->removeFromParent();
	if( yidian!=1){
		yidian=1;
	    button->setVisible(true);
		button->setOpacity(0.0f);
		playAnimationByname(this,"JinRu_donghua.ExportJson","JinRu_donghua","ANniu_Chuxian",false,false,Vec2(button->getPositionX(),button->getPositionY()+button->getContentSize().height));
	}
	

}

void StartLayer::runHeromyself(Node* sender){
	
	hero->removeFromParent();
	//hero->setPosition(p1);
	//hero->runAction(Sequence::create( MoveTo::create(6.0f,p2),CallFunc::create( CC_CALLBACK_0(StartLayer::runHeromyself,this,nullptr)),nullptr));
}

void StartLayer::sgstart(Node* sender){

	
	Sprite* bt_view=(Sprite*)bt->getChildByTag(rand()%8+1);
	bt_view->runAction(Sequence::create(Spawn::create(FadeIn::create(1.0f),ScaleTo::create(1.0f,5.0f),nullptr),Spawn::create(FadeOut::create(0.1f),ScaleTo::create(0.1f,0.1f),nullptr),
		CallFuncN::create( CC_CALLBACK_0(StartLayer::sgstart,this,nullptr)),nullptr));
											
}


void StartLayer::Gomain(){


	
	CCLOG("-----> worinima---->");
	FX::Music::getInstance()->buttonClicked();
	this->stopAllActions();
	auto scene=Uiscene::createScene();
	Director::getInstance()->replaceScene(scene);//TransitionZoomFlipAngular::create(1.5f, scene, TransitionScene::Orientation::LEFT_OVER));
	
	/*
//	auto scene=Scene::create();
	//AccountsLayer* layer=AccountsLayer::create();
	//CutomTableshop* layer=CutomTableshop::create();
	//layer->Shieldtouched();
	//layer->setGold();
	//layer->inintAccoutsLayer(201,120,12,123,1200);
//	CutomTableshop* layer=CutomTableshop::create();
//	scene->addChild(layer);
//	Director::getInstance()->replaceScene(scene);
	*/
}


int StartLayer::getDelwithint(int number,int iswidth){

	int dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;
}


void StartLayer::playAnimationByname(Node *bs,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscale,Vec2 p){

	 
	auto armature =Armature::create(filename.c_str());
	if(isloop==true){
		armature->getAnimation()->play(animationName.c_str(),-1,1);
	}else{

	   armature->getAnimation()->play(animationName.c_str());
	   if(animationName.compare("ANniu_Chuxian")==0){
		  armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(StartLayer::anniu_AnimateEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	   }else{
	     armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(StartLayer::biaoti_AnimateEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	   }
	}
	armature->removeFromParentAndCleanup(true);
	if(isscale==true){
		
		
	}
	if(animationName.compare("Animation1")==0){
		
	}else if(animationName.compare("gegewu")==0){
		
	}
	armature->setPosition(p);
	//armature->setTag(100);
	bs->addChild(armature,1000);

}

void StartLayer::playAnimationByname(Node *bs,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscal){

	auto armature =Armature::create(filename.c_str());
	if(isloop==true){
		armature->getAnimation()->play(animationName.c_str(),-1,1);
	}else{
	   armature->getAnimation()->play(animationName.c_str());
	   armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(StartLayer::callfunc_AnimateEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		
	}
	armature->removeFromParentAndCleanup(true);
	if(isscal==true){
		
		
	}
	if(animationName.compare("Animation1")==0){
		
	}else if(animationName.compare("gegewu")==0){
		
	}
	armature->setPosition(Vec2(bs->getContentSize().width/2,bs->getContentSize().height/2));
	armature->setTag(100);
	bs->addChild(armature);
	 
	
}
	
void StartLayer::callfunc_AnimateEnd(Armature *ar, MovementEventType, const std::string&){

	 ar->removeFromParent();


	
}


void StartLayer::updatehero(float time){

	//auto armature =Armature::create("gegewu");
	run=Armature::create("gegewu");//Sprite::create("gegewu_pao_1.png");
	Vec2 p3=Vec2(-300,getDelwithint(100,1)+run->getContentSize().height/2);
	Vec2 p4=Vec2(Actual_x+run->getContentSize().width*2-200,getDelwithint(100,1)+run->getContentSize().height/2);
	
	run->getAnimation()->play("gegewu",-1,1);
	
	run->setPosition(p3);
	addChild(run,1);
	
	run->runAction(Sequence::create( MoveTo::create(4.0f,p4),CallFunc::create(CC_CALLBACK_0(StartLayer::runHero,this,nullptr)),nullptr));


}



void StartLayer::Shieldtouched(){
     
	//事件
	auto listen=EventListenerTouchOneByOne::create();
	//listen->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false   
    listen->onTouchBegan = [this](Touch* touch, Event* event){    

		if(yidian==0){
			yidian=1;
			button->setVisible(true);
			button->setOpacity(0.0f);
			playAnimationByname(this,"JinRu_donghua.ExportJson","JinRu_donghua","ANniu_Chuxian",false,false,Vec2(button->getPositionX(),button->getPositionY()+button->getContentSize().height));
		}

        return true;    
    };     
    listen->onTouchMoved = [](Touch* touch, Event* event){       
    };     
     
    listen->onTouchEnded = [=](Touch* touch, Event* event){     
    };  
	EventDispatcher* dispatch=Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen,this);
	 


}


void  StartLayer::startAcitve(){

	  lanmeimei=Armature::create("nanhai_tao");
	  lanmeimei->getAnimation()->play("nvhai_zou",-1,1);

	  lanbaba=Armature::create("nanhai_tao");
	  lanbaba->getAnimation()->play("nanhai_zou",-1,1);

	  Vec2  p1=Vec2(Actual_x+lanmeimei->getContentSize().width/2,getDelwithint(100,1)+lanmeimei->getContentSize().height/2);
	  Vec2  p2=Vec2(Actual_x/2-lanmeimei->getContentSize().width/2,getDelwithint(100,1)+lanmeimei->getContentSize().height/2);

	  Vec2  p3=Vec2(Actual_x+lanmeimei->getContentSize().width+lanmeimei->getContentSize().width/2,getDelwithint(100,1)+lanbaba->getContentSize().height/2);
	  Vec2  p4=Vec2(Actual_x/2-lanmeimei->getContentSize().width/2+lanmeimei->getContentSize().width,getDelwithint(100,1)+lanbaba->getContentSize().height/2);
	  
	  lanmeimei->setPosition(p1);
	  lanbaba->setPosition(p3);	  


	  addChild(lanmeimei,100);
	  addChild(lanbaba,100);



	  lanmeimei->runAction(Sequence::create( MoveTo::create(4.0f,p2),CallFunc::create(CC_CALLBACK_0(StartLayer::runNvzou,this,nullptr)),nullptr));
	  lanbaba->runAction(Sequence::create( MoveTo::create(4.0f,p4),CallFunc::create(CC_CALLBACK_0(StartLayer::runManzou,this,nullptr)),nullptr));

}



void StartLayer::runNvzou(Node* sender){
	  
	  lanmeimei->getAnimation()->play("nvhai_jingya",-1,-1);

}

void StartLayer::runManzou(Node* sender){
	 
	  lanbaba->getAnimation()->play("nanhai_jingya",-1,-1);
	  
	  na=-1;
	  schedule(schedule_selector(StartLayer::naNv), 0.3f);
	  
}



void StartLayer::runManzou1(Node* sender){
	
	lanbaba->removeFromParent();

}
void StartLayer::runNvzou1(Node* sender){
	
	lanmeimei->removeFromParent();
}

void StartLayer::naNv(float dt){

	  na=1;
	  unschedule(schedule_selector(StartLayer::naNv));
	  Vec2  p11=Vec2(Actual_x+lanmeimei->getContentSize().width/2,getDelwithint(100,1)+lanmeimei->getContentSize().height/2);
	  lanmeimei->getAnimation()->play("nvhai_tao",-1,1);
	  lanmeimei->runAction(Sequence::create( MoveTo::create(2.0f,p11),CallFunc::create(CC_CALLBACK_0(StartLayer::runNvzou1,this,nullptr)),nullptr));


	  Vec2  p33=Vec2(Actual_x+lanmeimei->getContentSize().width+lanmeimei->getContentSize().width/2,getDelwithint(100,1)+lanbaba->getContentSize().height/2);
	  lanbaba->getAnimation()->play("nanhai_tao",-1,1);
	  lanbaba->runAction(Sequence::create( MoveTo::create(2.0f,p33),CallFunc::create(CC_CALLBACK_0(StartLayer::runManzou1,this,nullptr)),nullptr));
}

void StartLayer::gegewuChu(float dt){

	
	gegewu=1;
	 unschedule(schedule_selector(StartLayer::gegewuChu));
	 updatehero(0.5f);
}






void StartLayer::callfunc_Nvhai(Armature *ar, MovementEventType, const std::string&){

	


}

void StartLayer::callfunc_Naihai(Armature *ar, MovementEventType, const std::string&){
	  
	 

}

void StartLayer::biaoti_AnimateEnd(Armature *ar, MovementEventType, const std::string&){

	 
	 CCLOG("--->over");
	 biaoti=ar;
	
	 bitao=-1;
	 schedule(schedule_selector(StartLayer::bitaoT), 0.5f);



	 
}
void StartLayer::bitaoT(float dt){

	
	bitao=1;
	 unschedule(schedule_selector(StartLayer::bitaoT));
	 biaoti->removeFromParent();
	 Armature* top=Armature::create("JinRu_donghua");//Sprite::create("gegewu_pao_1.png");
	 top->getAnimation()->play("BiaoTi_ChiXu");
	 top->setPosition(Vec2(Actual_x/2,Actual_y/2));
	 addChild(top,100);

	


}

void StartLayer::guangXiao(float dt){


	
	 guanxuiao=1;
	 unschedule(schedule_selector(StartLayer::guangXiao));
	 auto guang=Sprite::create("ui_kaishi_xiaoguo_liuguang.png");
	 guang->setScale(2.2f);
	 guang->setPosition(Vec2(Actual_x/2,Actual_y/2+getDelwithint(100,1)));
	 guang->runAction(RepeatForever::create(RotateBy::create(6.0f,360.0f)));
	 addChild(guang,80);

}


void StartLayer::anniuChixu(float dt){

	  aniu=1;
	  unschedule(schedule_selector(StartLayer::anniuChixu));
	  anniu->removeFromParent();
	  Armature* top=Armature::create("JinRu_donghua");//Sprite::create("gegewu_pao_1.png");
	  top->getAnimation()->play("AnNiu_Chixu");
	  top->setPosition(Vec2(button->getPositionX(),button->getPositionY()+button->getContentSize().height));
	  addChild(top,1000);

}

void StartLayer::anniu_AnimateEnd(Armature *ar, MovementEventType, const std::string&){

	  anniu=ar;
	  aniu=-1;
	  schedule(schedule_selector(StartLayer::anniuChixu), 0.5f);

}

//注册
void StartLayer::registCallback(cocos2d::Ref* pSender){

	 
	
}






void StartLayer::setTextb(MenuItemImage* node,string str,int fontsize,Color3B b){

	auto label=Label::createWithSystemFont(FX::StringsMap::getValue(str),"Arial",fontsize);
	label->setPosition(Vec2(node->getContentSize().width/2,node->getContentSize().height/2));
	label->setColor(b);
	node->addChild(label);

}

void StartLayer::Shieldtouched(Node* node){

	 //事件
	auto listen=EventListenerTouchOneByOne::create();
	listen->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false   
    listen->onTouchBegan = [](Touch* touch, Event* event){    
        return true;    
    };     
    listen->onTouchMoved = [](Touch* touch, Event* event){       
    };     
     
    listen->onTouchEnded = [=](Touch* touch, Event* event){     
    };  
	EventDispatcher* dispatch=Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen,node);


}

void StartLayer::csFloat(float dt){

	
	NetmsgManger::getNetManager()->updateNickname("jkliui");
    NetmsgManger::getNetManager()->loadNetRank();
}