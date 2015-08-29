#include "Veronestart.h"
#include "BasicLayer.h"



#define basic_color Color3B(0,0,0)
#define basic_color1 Color3B(254,218,185)

#define back_img "ui_kaishi_bg.jpg"
#define biao_ti  "UI_denglu_biaoti_bg.png"
#define start_game "UI_denglu_kaishiyouxi.png"
#define set_img       "UI_denglu_shezhi.png"
#define active_img   "UI_denglu_huodongzhongxin.png"

#define fanhui_img  "UI_denglu_fanhui.png"
#define kuang_tiao  "UI_denglu_shezhi_bg.png"

#define yinyue_img  "UI_denglu_shezhi_yinyue.png"
#define yinxiao_img  "UI_denglu_shezhi_yinxiao.png"
#define setmusicz_img  "UI_denglu_shezhi_gou_bg.png"
#define musicx_img    "UI_denglu_shezhi_guanbi.png"
#define musicj_img    "UI_denglu_shezhi_gou.png"

#define left_img "UI_sence_denglu_left.png"
#define right_img "UI_sence_denglu_left.png"

#define Shu_du 10
//"Run_UI_jinru.ExportJson"

#define Ui_active  "Run_UI_jinru2.ExportJson"
#define Ui_activeanidata "Run_UI_jinru2"
#define Msg_active "Active_msg"

enum Tagnumber{

	 starttag=100,
	 leftag,
	 righttag,
	 settag,
	 topkt,
	 domkt,
	 topktsonyinyue,
	 topktsonk,
	 domktsonxiao,
	 domktsonk,
	 yinyueone,
	 yinyuetwo,
	 xiaone,
	 xiaotwo,
	 backimgtag,
	 backright,

};

enum Actiontag{

	biaoti_tag,
	anniu_tag,
	guangxiao_tag,
};



Veronestart::Veronestart(void)
{
}


Veronestart::~Veronestart(void)
{
}


bool Veronestart::init(){

	if(!Layer::init()){

		return false;
	} 
	drawLayout();
    return true;
}
void Veronestart::onEnter(){


	

	Layer::onEnter();
}
void Veronestart::onExit(){

	this->stopAllActions();
	Layer::onExit();

}

float Veronestart::getDelwithint(float number,int iswidth){


	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/960);
	}else{
		dealnumber=number*(Actual_y/640);
	}
	return dealnumber;
}


void Veronestart::drawLayout(){


    Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	call_mark=0;
	mk_tag=0;
	isend=0;
	state=1;
	jianbu=2.0f;

	isdoneactive=false;

	timectol.biaoti_time=0.2f;    //标题动画
	timectol.start_time=0.5f;     //开始按钮动画
    timectol.guangxiao_time=6.0f; //光效动画
	timectol.time=0;
	for(int i=0;i<3;i++){
    	timectol.mark[i]=0;
	}
	timectol.actioname.push_back("biaoti_chuxian");
	timectol.actioname.push_back("anniu_chuxian");
	timectol.actioname.push_back("anniu_huxi");
	timectol.actioname.push_back("biaoti_huxi");
	timectol.actioname.push_back("biaoti_huxi_guangxiao");


    
	auto backimg=Sprite::create(left_img);
	backimg->setAnchorPoint(Vec2(0,0.5));
	backimg->setPosition(Vec2(0,Actual_y/2));
	
	backimg->setTag(Tagnumber::backimgtag);
	addChild(backimg);
	runsp.pushBack(backimg);

	auto backrightimg=Sprite::create(left_img);
	backrightimg->setAnchorPoint(Vec2(0,0.5));
	backrightimg->setPosition(Vec2(backrightimg->getContentSize().width,Actual_y/2));
	backrightimg->setTag(Tagnumber::backright);
    backrightimg->setFlippedX(true);
	addChild(backrightimg);
	runsp.pushBack(backrightimg);
	MovebackAction();
	schedule(schedule_selector(Veronestart::timeupdate),0.1f);
	/*
	{
	
	playAnimationByname(this,Ui_active,Ui_activeanidata,timectol.actioname[0],false);
	}

	{
	
	playAnimationByname(this,Ui_active,Ui_activeanidata,timectol.actioname[1],false);
	}

	{
	
	playAnimationByname(this,Ui_active,Ui_activeanidata,timectol.actioname[4],false);
	}
	*/

	//if(PlayerInfoHelper::getHelper()->getPlayerName().size()==0){
         //名字输入
		// write_nameBox();
	//}else{
	 //initlayer();
	//}

	
	
}

void Veronestart::playerTag(int tag){

	 
	 

}

void Veronestart::timeupdate(float dt){

	 timectol.time+=dt;
	 if(timectol.time>=timectol.biaoti_time&&timectol.mark[0]==0){
         timectol.mark[0]=1;
		 playAnimationByname(this,Ui_active,Ui_activeanidata,timectol.actioname[0],false);
	 }
	 if(timectol.time>= timectol.start_time&&timectol.mark[1]==0){
		   timectol.mark[1]=1;
		   playAnimationByname(this,Ui_active,Ui_activeanidata,timectol.actioname[1],false);
	 }
	 if(timectol.time>=timectol.guangxiao_time&&timectol.mark[2]==0){
		   timectol.mark[2]=1;
		   playAnimationByname(this,Ui_active,Ui_activeanidata,timectol.actioname[4],false);
		   unschedule(schedule_selector(Veronestart::timeupdate));
	 }
	 

}

void Veronestart::MovebackAction(){

	  for(auto m:runsp){
		  Vec2 p=Vec2(-(m->getContentSize().width),Actual_y/2);
		  float time=(m->getPositionX()-p.x)/Shu_du;
		  m->runAction(
			  Sequence::create( 
			                      MoveTo::create(time,p),CallFuncN::create(CC_CALLBACK_1(Veronestart::Movedone,this)),
								  nullptr) 
			  );
	  }
}

void Veronestart::Movedone(Node* node){

	if(Shu_du>100){
	  jianbu=(Shu_du/100)*2;
	}
	  Vec2 p=Vec2(-(node->getContentSize().width),Actual_y/2);
	  for(auto m:runsp){
		  if(m->getTag()!=node->getTag()){
			  node->setPosition(Vec2(m->getPositionX()+m->getContentSize().width-jianbu,Actual_y/2));
		  }
	  }
	 
	  float time=(node->getPositionX()-p.x)/Shu_du;
      node->runAction(
			  Sequence::create( 
			                      MoveTo::create(time,p),
                                  CallFuncN::create(CC_CALLBACK_1(Veronestart::Movedone,this)),
												nullptr) 
							    );
}


void Veronestart::initlayer(){

    alayer=Layer::create();
	addChild(alayer,100);
	//Shieldtouched();
	blayer=Layer::create();
	blayer->setPosition(Vec2(Actual_x,0));
	addChild(blayer,98);
	drawOne();
	drawTwo();
}


void Veronestart::drawOne(){

	 
	auto biaotisp=Sprite::create(biao_ti);
	biaotisp->setPosition(Vec2(Actual_x/2,Actual_y*0.7));
	alayer->addChild(biaotisp);
	 
	auto startgamesp=Sprite::create(start_game);//  MenuItemImage::create(start_game,start_game,CC_CALLBACK_1());
	startgamesp->setPosition(Actual_x/2,Actual_y*0.25);
	startgamesp->setTag(Tagnumber::starttag);
	alayer->addChild(startgamesp);
    
	auto activesp=Sprite::create(active_img);
	activesp->setVisible(false);
	activesp->setPosition(Vec2(30+activesp->getContentSize().width/2,30+activesp->getContentSize().height/2));
	activesp->setTag(Tagnumber::leftag);
	alayer->addChild(activesp);

	auto setsp=Sprite::create(set_img);
	setsp->setPosition(Vec2(Actual_x-setsp->getContentSize().width/2-30,30+activesp->getContentSize().height/2));
	setsp->setTag(Tagnumber::righttag);
	alayer->addChild(setsp);
     
}



void Veronestart::drawTwo(){


	
	auto fh_img=Sprite::create(fanhui_img);
	fh_img->setTag(Tagnumber::settag);
	fh_img->setPosition(Vec2(30+fh_img->getContentSize().width/2,30+fh_img->getContentSize().height/2));
	blayer->addChild(fh_img);


	//
	auto s_kimg=Sprite::create(kuang_tiao);
	s_kimg->setPosition(Vec2(Actual_x/2,Actual_y*0.64));
	s_kimg->setTag(Tagnumber::topkt);
	blayer->addChild(s_kimg);

	auto yinyue=Sprite::create(yinyue_img);
	float skx=s_kimg->getPositionX()-s_kimg->getContentSize().width/2-50+yinyue->getContentSize().width/2;
	float sky=s_kimg->getPositionY();
	yinyue->setPosition(Vec2(skx,sky));
	yinyue->setTag(Tagnumber::topktsonyinyue);
	blayer->addChild(yinyue);


	auto ms_k=Sprite::create(setmusicz_img);
	ms_k->setPosition(Vec2(s_kimg->getPositionX()+s_kimg->getContentSize().width/2-ms_k->getContentSize().width/2+30,s_kimg->getPositionY()));
	ms_k->setTag(Tagnumber::topktsonk);
	blayer->addChild(ms_k);

	string str_yinyue=FX::StringsMap::getValue("yinyuek");

	if(PlayerInfoHelper::getHelper()->getGameLock(PlayerInfoHelper::MusicLock)==false){

		auto yy_x=Sprite::create(musicx_img);
		yy_x->setPosition(Vec2(yinyue->getContentSize().width/2,yinyue->getContentSize().height/2));
		yy_x->setTag(100);
		yinyue->addChild(yy_x);
		str_yinyue=FX::StringsMap::getValue("yinyueg");
		FX::Music::getInstance()->setMusicEnable(false);
	}else{
		auto yy_j=Sprite::create(musicj_img);
		yy_j->setPosition(Vec2(ms_k->getContentSize().width/2,ms_k->getContentSize().height/2));
		yy_j->setTag(100);
		ms_k->addChild(yy_j);
		FX::Music::getInstance()->setMusicEnable(true);
	}

	Label* y_p=Label::createWithSystemFont(str_yinyue,"minijiankatong.ttf",40);
	y_p->setPosition(Vec2(s_kimg->getPositionX(),s_kimg->getPositionY()));
	y_p->setTag(Tagnumber::yinyueone);
	blayer->addChild(y_p);

	Label* y_p2=Label::createWithSystemFont(str_yinyue,"minijiankatong.ttf",40);
	y_p2->setPosition(Vec2(s_kimg->getPositionX(),s_kimg->getPositionY()-1));
	y_p2->setColor(basic_color1);
	y_p2->setTag(Tagnumber::yinyuetwo);
	blayer->addChild(y_p2);


	auto x_kimg=Sprite::create(kuang_tiao);
	x_kimg->setTag(Tagnumber::domkt);
	x_kimg->setPosition(Vec2(Actual_x/2,Actual_y*0.43));
	blayer->addChild(x_kimg);

	auto yinxiao=Sprite::create(yinxiao_img);
	yinxiao->setPosition(Vec2(x_kimg->getPositionX()-x_kimg->getContentSize().width/2-50+yinxiao->getContentSize().width/2,x_kimg->getPositionY()));
	yinxiao->setTag(Tagnumber::domktsonxiao);
	blayer->addChild(yinxiao);

	auto ms_kx=Sprite::create(setmusicz_img);
	ms_kx->setPosition(Vec2(x_kimg->getPositionX()+x_kimg->getContentSize().width/2-ms_kx->getContentSize().width/2+30,x_kimg->getPositionY()));
	ms_kx->setTag(Tagnumber::domktsonk);
	blayer->addChild(ms_kx);

	string str_xiao=FX::StringsMap::getValue("yinxiaok");
	if(PlayerInfoHelper::getHelper()->getGameLock(PlayerInfoHelper::MusciEffectLock)==false){

		 
		auto yy_xdom=Sprite::create(musicx_img);
		yy_xdom->setPosition(Vec2(yinxiao->getContentSize().width/2,yinxiao->getContentSize().height/2));
		yy_xdom->setTag(100);
		yinxiao->addChild(yy_xdom);
		str_xiao=FX::StringsMap::getValue("yinxiaog");

	}else{

		auto yy_jdom=Sprite::create(musicj_img);
		yy_jdom->setPosition(Vec2(ms_kx->getContentSize().width/2,ms_kx->getContentSize().height/2));
		yy_jdom->setTag(100);
		ms_kx->addChild(yy_jdom);
	}

	Label* x_p=Label::createWithSystemFont(str_xiao,"minijiankatong.ttf",40);
	x_p->setPosition(Vec2(x_kimg->getPositionX(),x_kimg->getPositionY()));
	x_p->setTag(Tagnumber::xiaone);
	blayer->addChild(x_p);

	Label* x_p2=Label::createWithSystemFont(str_xiao,"minijiankatong.ttf",40);
	x_p2->setPosition(Vec2(x_kimg->getPositionX(),x_kimg->getPositionY()-1));
	x_p2->setColor(basic_color1);
	x_p2->setTag(Tagnumber::xiaotwo);
	blayer->addChild(x_p2);

}




void Veronestart::callback(Ref* ref){
	 


}

void Veronestart::spcall(Node* node,bool mr){
	 
	 isdoneactive=true;
	 if(isend==1){
		 node->setScale(1.0f);
		 call_mark=0;
	 }

}

void Veronestart::setcall(Node* node){

	
	//Sprite* p=(Sprite*)getChildByTag(Tagnumber::backimgtag);
	//p->setTexture(Sprite::create(right_img)->getTexture());

	state=2;
	auto move = MoveTo::create(0.5f, Vec2(0,0));    
    auto move_ease_inout1 = EaseExponentialOut::create(move);
	Vector<Node *> pa= blayer->getChildren();
    for(auto m:pa){
		m->runAction(FadeIn::create(0.5));
	}
	auto swap=Spawn::create(move_ease_inout1,FadeIn::create(0.5),nullptr);
	blayer->runAction(swap);

}


void Veronestart::bsetcall(Node* node){

	
	//Sprite* p=(Sprite*)getChildByTag(Tagnumber::backimgtag);
	//p->setTexture(Sprite::create(left_img)->getTexture());

	state=1;
	auto move = MoveTo::create(0.5f, Vec2(0,0));    
    auto move_ease_inout1 = EaseExponentialOut::create(move);
	//out xiaoshi in xian xian
	 Vector<Node *> pa= alayer->getChildren();
     for(auto m:pa){
		m->runAction(FadeIn::create(0.5));
	 }
	auto swap=Spawn::create(move_ease_inout1,FadeIn::create(0.5),nullptr);
	alayer->runAction(swap);

}


 void Veronestart::Shieldtouched(){

    //unsigned int mk_tag=0;
	   //事件
	auto listen=EventListenerTouchOneByOne::create();
	listen->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false   

    listen->onTouchBegan = [&](Touch* touch, Event* event){    

		if(call_mark==1){
		 return false;
	    }
        Vec2 p=touch->getLocation();
        return true;    
    };     
    listen->onTouchMoved = [&](Touch* touch, Event* event){       
		
    };     
     
    listen->onTouchEnded = [&](Touch* touch, Event* event){     
		
	  
    };  

	EventDispatcher* dispatch=Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen,this);

	
	ignoreAnchorPointForPosition(false);
	setAnchorPoint(Vec2(0.5,0.5));
	setPosition(Vec2(Actual_x/2,Actual_y/2));
	setContentSize(Size(Actual_x,Actual_y));

 }


 bool Veronestart::isContactpoint(Vec2 p,Sprite* sp){
 
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

 void Veronestart::setTextb(MenuItemImage* node,string str,int fontsize,Color3B b){

	auto label=Label::createWithSystemFont(FX::StringsMap::getValue(str),"minijiankatong.ttf",fontsize);
	label->setPosition(Vec2(node->getContentSize().width/2,node->getContentSize().height/2));
	label->setColor(b);
	node->addChild(label); 

 }


 void Veronestart::write_Button(Ref* ref){



	string name_sr=write_name->context;
	auto cs_namesize=Label::createWithSystemFont(write_name->context,"minijiankatong.ttf",24);
	int size=cs_namesize->getContentSize().width;

	auto ctname=Label::createWithSystemFont(FX::StringsMap::getValue("Bzszzm"),"minijiankatong.ttf",24);
	int bzsize=ctname->getContentSize().width;

	if(ctname->getContentSize().width<cs_namesize->getContentSize().width){
		write_name->_editName->setText(FX::StringsMap::getValue("zuiduoliugehanzi").c_str());
	}else if(cs_namesize->getContentSize().width==0){
		write_name->_editName->setPlaceHolder(FX::StringsMap::getValue("mingzibunengweikong").c_str());
	}else{

		PlayerInfoHelper::getHelper()->setPlayerName(name_sr);
		scal9->removeFromParent();
		initlayer();
	}
}


 void Veronestart::write_nameBox(){

	  
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


	auto need_label=Label::createWithSystemFont(FX::StringsMap::getValue("Shuruxinyinichen"),"minijiankatong.ttf",24);
	need_label->setColor(Color3B(0,0,0));
	need_label->setPosition(Vec2(40+need_label->getContentSize().width/2,scal9->getContentSize().height*0.28));
	scal9->addChild(need_label);

	
	auto mody_butoon=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(Veronestart::write_Button,this));
	mody_butoon->setPosition(Vec2(scal9->getContentSize().width,scal9->getContentSize().height/2));
	this->setTextb(mody_butoon,"qding",30,Color3B(0,0,0));
	
	auto menu = Menu::create(mody_butoon,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu, 125);

	showShake(scal9);

 }

 void Veronestart::showShake(Node* sp){

	   sp->setScale(0.2f);
	   sp->runAction(Sequence::create(
		FadeOut::create(0),
		Spawn::create(FadeIn::create(0.05f),
		ScaleTo::create(0.1f,1.05f),
		nullptr),
		
		ScaleTo::create(0.1f,1.0f),
		nullptr));
 }


 int Veronestart::getTag(){

	 return rand()%200;
 }


 int Veronestart::getTag2(int parent,int (Veronestart::*p)()){


	 return parent;
 }

 void Veronestart::playAnimationByname(Node *bs,std::string jsonfile,std::string filename,std::string animationName,bool isloop){

	
	 Armature *armature=Armature::create(filename.c_str());


	  
	if(animationName.compare(timectol.actioname[0])==0){
	   armature->getAnimation()->play(animationName.c_str());
	   armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Veronestart::callfunc_AnimateEndGk, 
			 this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); 
	   armature->setPosition(Vec2(getContentSize().width/2,Actual_y*0.45));
       armature->removeFromParentAndCleanup(true);
	   bs->addChild(armature,10);
	   armature->setTag(biaoti_tag);
	   timectol.armats.push_back(armature);
	  
	}else if(animationName.compare(timectol.actioname[4])==0){
	    
		for(auto m:timectol.armats){
			if(m->getTag()==biaoti_tag){
				m->setVisible(false);
				break;
			}
		}
		armature->getAnimation()->play(animationName.c_str());
	    armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Veronestart::callfunc_AnimateEndGk, 
			 this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	    armature->setPosition(Vec2(getContentSize().width/2,Actual_y*0.45));
		armature->removeFromParentAndCleanup(true);
	    bs->addChild(armature,150);
	    armature->setTag(guangxiao_tag);
	    
		
	}else if(animationName.compare(timectol.actioname[1])==0){

	   armature->getAnimation()->play(animationName.c_str());
	   armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Veronestart::callfunc_AnimateEndGk, 
			 this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	   armature->setPosition(Vec2(getContentSize().width/2,Actual_y*0.5));
       armature->removeFromParentAndCleanup(true);
	   armature->setTag(anniu_tag);
	   bs->addChild(armature,100);
	   timectol.armats.push_back(armature);
       
	   auto p_click=MenuItemImage::create("UI_jiaose_anniu_shengji.png","UI_jiaose_anniu_shengji.png",CC_CALLBACK_1(Veronestart::allcallback,this));
	   p_click->setPosition(Vec2(getContentSize().width/2,Actual_y*0.12));
	   p_click->setScaleX(2.0f);
	   p_click->setOpacity(0);
	   auto menu=Menu::create(p_click,nullptr);
	   menu->setPosition(Vec2::ZERO);
	   addChild(menu);
	}
}

 void Veronestart::allcallback(Ref* ref){

	 MenuItemImage* p=(MenuItemImage*)ref;
	 p->setCallback(nullptr);
	  for(auto m:timectol.armats){
		  if(m->getTag()==anniu_tag){
			
			  m->stopAllActions();
			  m->setScale(1.2f);

			  int bLoged = PlayerInfoHelper::getHelper()->getOneInfo(History_AlreadyHasLogin);
			  if (bLoged)
			  {
										auto scene=Uiscene::createScene();
										Director::getInstance()->replaceScene(scene);
			  }
			  else 
			  {
										this->removeFromParent();
										GameDirector::getDirector()->loadingToRunningScene(RunningMode::teach);
										return;
              }
			  break;
		  }
	  }
 }



void Veronestart::callfunc_AnimateEndGk(Armature *ar, MovementEventType movementType, const std::string& movementID){
	  
	 if(movementID.compare(timectol.actioname[0])==0){
		 ar->getAnimation()->setMovementEventCallFunc(nullptr);
		 ar->getAnimation()->play(timectol.actioname[3]);

	 }else if(movementID.compare(timectol.actioname[4])==0){
		 ar->getAnimation()->setMovementEventCallFunc(nullptr);
		 ar->removeFromParent();
		 for(auto m:timectol.armats){
			if(m->getTag()==biaoti_tag){
				m->setVisible(true);
				break;
			}
		}
	 }else if(movementID.compare(timectol.actioname[1])==0){
		 ar->getAnimation()->setMovementEventCallFunc(nullptr);
         ar->getAnimation()->play(timectol.actioname[2]);
	 }

}