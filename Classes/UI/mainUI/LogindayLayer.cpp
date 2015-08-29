#include "LogindayLayer.h"

#include "protocol_message.pb.h"
#include "NetmsgManger.h"
#include "ImageHelper.h"
#include "UnitConfigManager.h"

LogindayLayer::LogindayLayer(void)
{
}


LogindayLayer::~LogindayLayer(void)
{

	

}



bool LogindayLayer::init(){

	if(!Layer::init()){

		return false;
	}
	isRegist=false;
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	
	currentrsp=0;
	iskd=true;

	
	//spp=Sprite::create("ui_zhuye_guanqiamoshi_guanqia_weijiesuo.png");
	//addChild(spp);

	
	jiemian();

	int id=PlayerInfoHelper::getHelper()->getUserId();
	
	if(id==0){
		//得注册
		NetmsgManger* manager=NetmsgManger::getNetManager();
		manager->setMsg_Center(std::bind(&LogindayLayer::Regist_return,this,std::placeholders::_1));
		manager->isregist();
	    isRegist=true;
	}else{
		//NetmsgManger* manager=NetmsgManger::getNetManager();
		//manager->setMsg_Center(std::bind(&LogindayLayer::Regist_return,this,std::placeholders::_1));
	    //manager->sendLoginday();	
		NetmsgManger::getNetManager()->sendLoginday();
		//schedule(schedule_selector(LogindayLayer::cs_login), 0.5f);
	}
	
	return true;
}
void LogindayLayer::onEnter(){

    Msgcallback();
	Layer::onEnter();

}

void LogindayLayer::onExit(){
    

	NetmsgManger::getNetManager()->setMsgc_null();
	w_l.clear();
	wards.clear();
	NetmsgManger::getNetManager()->wards.clear();
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	unschedule(schedule_selector(LogindayLayer::cs_login));
	Layer::onExit();
}


float LogindayLayer::getDelwithint(float number,int iswidth){

	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
    return dealnumber;

}



void LogindayLayer::loadInterphase(bool bl){
    


	
	
	if (bl==false)
	{

		text->setString(FX::StringsMap::getValue("weilianjie"));
	   
	}else{

		
		text->setVisible(false);

	   auto spv=Sprite::create("ui_zhuye_guanqiamoshi_guanqia_weijiesuo.png");

       float jg1=(scal9->getContentSize().width-(spv->getContentSize().width*0.7)*4-80)/3;
	   float start1=(scal9->getContentSize().width- jg1*3+spv->getContentSize().width*4*0.7f)/2;
	   float jg2=jg1;
		
		int size=wards.size();
		for(int i=0;i<size;i++){

			
			
			if(i<4){

				auto sp=Sprite::create("ui_zhuye_guanqiamoshi_guanqia_weijiesuo.png");				
				sp->setScale(0.7f);
				sp->setPosition(Vec2(40+sp->getContentSize().width*0.7f*i+jg1*i+sp->getContentSize().width*0.7f/2,scal9->getContentSize().height*0.7));
				scal9->addChild(sp,100);
				
				w_l.insert(i,sp);
				char c[100];
                int k=i+1;
                sprintf(c,"d%d",k);

				
				auto day=Label::createWithSystemFont(FX::StringsMap::getValue(c),"Arial",26);
				day->setPosition(Vec2(sp->getContentSize().width/2,sp->getContentSize().height*0.89));
				sp->addChild(day);
	            
				
				string img_name=UnitConfigManager::getManager()->getOneUnit(wards.at(i).type)->imageFile;
				auto wup=Sprite::create(img_name);// ImageHelper::getUnitSpriteUIScene(wards.at(i).type);//ImageHelper::getUnitSpriteRunningScene(wards.at(i).type); //Sprite::create(typeimgstr);

				wup->setPosition(Vec2(sp->getContentSize().width/2,sp->getContentSize().height*0.5));
				wup->setScale(1.8f);
				sp->addChild(wup);
				
				
				auto countl=Label::createWithSystemFont(__String::createWithFormat("%d",wards.at(i).count)->getCString(),"Arial",30);
				countl->setPosition(Vec2(sp->getContentSize().width/2,sp->getContentSize().height*0.18));
				countl->setColor(Color3B(0,108,191));
				sp->addChild(countl);
				

			    if(wards.at(i).enable==true&&wards.at(i).is_get==true){

					auto countl=Label::createWithSystemFont(FX::StringsMap::getValue("yilinju"),"Arial",40);
				    countl->setPosition(Vec2(sp->getContentSize().width/2,sp->getContentSize().height/2));
				    countl->setColor(Color3B(255,33,33));
					countl->setRotation(-45.0f);
				    sp->addChild(countl);
					
				}
				
				

	    	}else{

				

				auto sp=Sprite::create("ui_zhuye_guanqiamoshi_guanqia_weijiesuo.png");
              
				sp->setScale(0.7f);
				sp->setPosition(Vec2(145+sp->getContentSize().width*0.7f*(i-4)+jg2*(i-4)+sp->getContentSize().width*0.7f/2,scal9->getContentSize().height*0.3));
				scal9->addChild(sp,100);

				w_l.insert(i,sp);
				char c[100];
                int k=i+1;
                sprintf(c,"d%d",k);

				auto day=Label::createWithSystemFont(FX::StringsMap::getValue(c),"Arial",26);
				day->setPosition(Vec2(sp->getContentSize().width/2,sp->getContentSize().height*0.89));
				sp->addChild(day);


				

				string img_name=UnitConfigManager::getManager()->getOneUnit(wards.at(i).type)->imageFile;
				auto wup=Sprite::create(img_name);
				//auto wup=Sprite::create(UnitConfigManager::getManager()->getOneUnit(wards.at(i).type)->imageFile) ;  //ImageHelper::getUnitSpriteUIScene(wards.at(i).type);  //ImageHelper::getUnitSpriteRunningScene(wards.at(i).type);//Sprite::create(typeimgstr);
				 if(wup->getContentSize().width>0){
					wup->setPosition(Vec2(sp->getContentSize().width/2,sp->getContentSize().height*0.5));
					if(wards.at(i).type==UnitID::unitID_Gold||wards.at(i).type==UnitID::unitID_Diamond){
					   wup->setScale(1.8f);
					}
					sp->addChild(wup,100);
				 }
				
                
				auto countl=Label::createWithSystemFont(__String::createWithFormat("%d",wards.at(i).count)->getCString(),"Arial",30);
				countl->setPosition(Vec2(sp->getContentSize().width/2,sp->getContentSize().height*0.18));
				countl->setColor(Color3B(0,108,191));
				sp->addChild(countl);


				//未领取
				if(wards.at(i).enable==true&&wards.at(i).is_get==true){

					auto countl=Label::createWithSystemFont(FX::StringsMap::getValue("yilinju"),"Arial",40);
				    countl->setPosition(Vec2(sp->getContentSize().width/2,sp->getContentSize().height/2));
				    countl->setColor(Color3B(255,33,33));
					countl->setRotation(-45.0f);
				    sp->addChild(countl);

				}
				
			}
			
		}
		
		auto lqwarld=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(LogindayLayer::getWarldCallback,this));
	    lqwarld->setPosition(Vec2(getContentSize().width/2,scal9->getPositionY()-scal9->getContentSize().height/2));

		auto label=Label::createWithSystemFont(FX::StringsMap::getValue("linju"),"Arial",30);
		label->setPosition(Vec2(lqwarld->getContentSize().width/2,lqwarld->getContentSize().height/2));
		label->setColor(Color3B(0,0,0));
	    lqwarld->addChild(label);

	    auto menu = Menu::create(lqwarld,nullptr);
        menu->setPosition(Vec2::ZERO);
	    addChild(menu, 2);
		

	}
}



void LogindayLayer::getWarldCallback(cocos2d::Ref* pSender){


	

	warld_Id=-1;
	wl_id=-1;

	for(size_t i=0;i<wards.size();i++){

		Warld  wd=wards.at(i);
		if(wd.enable==true&&wd.is_get==false){
			warld_Id=wd.id;
			idw=wd;
			wl_id=i;
			break;
		}

	}

	if(warld_Id!=-1){
		 NetmsgManger* manager=NetmsgManger::getNetManager();
		 manager->setMsg_Center(std::bind(&LogindayLayer::Regist_return,this,std::placeholders::_1));
		 manager->linquDay(warld_Id);
	}else{
        
		auto layer=BaseLayer::create();
		layer->Tishistring("linquS");
		Director::getInstance()->getRunningScene()->addChild(layer);
	}

	

}

//界面
void LogindayLayer::jiemian(){

	 
	this->setContentSize(Size(Actual_x,Actual_y-getDelwithint(Tophight,1)));
    
	scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setContentSize(Size( getContentSize().width*0.8,getContentSize().height*0.8)); // 459,365
	scal9->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
	addChild(scal9,1);

    
	std::string tt= FX::StringsMap::getValue("shujujiazz")+"...";
	text=Label::createWithSystemFont(tt, "Arial", 34);
	text->setTag(1025);
	text->setColor(Color3B(0,0,0));
	text->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
	addChild(text,100);

}

void LogindayLayer::Regist_return(int tag){

	
	if(tag==0){
		text->setString(FX::StringsMap::getValue("weilianjie"));
	}else if(tag==1){
		NetmsgManger::getNetManager()->sendLoginday();
	}else if(tag==5){ //获得登陆成功

		if(wards.size()>0){
			return;
		}
		wards=NetmsgManger::getNetManager()->wards;
		loadInterphase(true);

	}else if(tag==4){//获得登陆失败
		loadInterphase(false);
	}else if(tag==7){//领取成功
		        if(warld_Id!=-1){
					auto layer=BaseLayer::create();
					layer->Tishistring("linquC");
					Director::getInstance()->getRunningScene()->addChild(layer);
					//GoodsHelper::getHelper()->changeOneUnitCount(idw.id,idw.count);
					BagDataManager::getManager()->changeOneUnitCount(idw.type,idw.count);
					if(w_l.at(wl_id)!=nullptr){
						auto countl=Label::createWithSystemFont(FX::StringsMap::getValue("yilinju"),"Arial",40);
						countl->setPosition(Vec2(w_l.at(wl_id)->getContentSize().width/2,w_l.at(wl_id)->getContentSize().height/2));
						countl->setColor(Color3B(255,33,33));
						countl->setRotation(-45.0f);
						w_l.at(wl_id)->addChild(countl);
						auto action1 = Sequence::create(
		                ScaleTo::create(0.5,1.0f),
                        CallFunc::create(
                             // lambda
                             [&](){
                                  ;
                             }  ),
							nullptr);
	                    countl->runAction(action1);

					}
		        }else{
					auto layer=BaseLayer::create();
					layer->Tishistring("linquS");
					Director::getInstance()->getRunningScene()->addChild(layer);
			  }
			        iskd=true;
	}else if(tag==6){//领取失败
					auto layer=BaseLayer::create();
					layer->Tishistring("linquS");
					Director::getInstance()->getRunningScene()->addChild(layer);
					iskd=true;
	}

	
}

void LogindayLayer::Msgcallback(){
 
	auto login_callBack       =  [&](EventCustom* evt)  
                            {  
								MsgData* data=(MsgData*)evt->getUserData();
								Regist_return(data->value);
                            };  
	auto Login_changevent=EventListenerCustom::create(Logoinmsg,login_callBack);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(Login_changevent,1);  
	
}

void LogindayLayer::cs_login(float dt){

	if(NetmsgManger::getNetManager()->wards.size()>0){

		wards=NetmsgManger::getNetManager()->wards;
		loadInterphase(true);
		unschedule(schedule_selector(LogindayLayer::cs_login));
	}

}