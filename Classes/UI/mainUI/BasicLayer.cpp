#include "BasicLayer.h"
#include "GoodsHelper.h"
#include "GameDirector.h"

#define tck_img  "UI_tanchukuang.png"

BasicLayer::BasicLayer(void)
{
}


BasicLayer::~BasicLayer(void)
{

}

bool BasicLayer::init(){

	if(!Layer::init()){
		return false;
	}
	this->Shieldtouched();
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	return true;
}

void BasicLayer::onEnter(){

	Layer::onEnter();
}

void BasicLayer::onExit(){
     
	
	Layer::onExit();
}

float BasicLayer::getDelwithint(float number,int iswidth){

	  float dealnumber;
	  if(iswidth==0){
		 dealnumber=number*(Actual_x/Design_width);
	  }else{
		 dealnumber=number*(Actual_y/Design_hight);
	  }
	  return dealnumber;
}


void BasicLayer::active_warldbox(BagItem bag){

	
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

	//
	const Unit *unit=GoodsHelper::getHelper()->getUnitByID(bag.id);
	auto sp_i=Sprite::create(unit->imageFile);
	sp_i->setPosition(Vec2(40+sp_i->getContentSize().width/2,scal9->getContentSize().height-10-sp_i->getContentSize().height/2));
	scal9->addChild(sp_i,100);


	auto label=Label::createWithSystemFont(__String::createWithFormat("%d",bag.value)->getCString(),"Arial",24);
	label->setColor(Color3B(255,33,33));
	label->setPosition(Vec2(sp_i->getPositionX(),scal9->getContentSize().height*0.4));
	scal9->addChild(label,100);

	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BasicLayer::daoju_reward,this));
	close->setPosition(Vec2(scal9->getContentSize().width-close->getContentSize().width/2,scal9->getContentSize().height-close->getContentSize().height/2));
    
	auto lqbutton=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(BasicLayer::linqu_active,this));
	lqbutton->setPosition(Vec2(scal9->getContentSize().width/2,0));
	//lqbutton->setUserData((void*)&bag);
	Label* lqts=Label::createWithSystemFont(FX::StringsMap::getValue("linqujl"),"Arial",24);
	lqts->setPosition(Vec2(lqbutton->getContentSize().width/2,lqbutton->getContentSize().height/2));
	lqbutton->addChild(lqts);


	GoodsHelper::getHelper()->changeOneUnitCount(bag.id,bag.value);
	auto labl=Label::createWithSystemFont(FX::StringsMap::getValue("yilinju"),"Arial",24);
	labl->setColor(Color3B(255,33,33));
	labl->setRotation(-45.0f);
	labl->setPosition(Vec2(scal9->getContentSize().width*0.6,scal9->getContentSize().height*0.4));
	scal9->addChild(labl,100);
	

	auto menu = Menu::create(close,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu, 2);
	showShake(scal9);
}


void BasicLayer::Shieldtouched(){

	//事件
	auto listen=EventListenerTouchOneByOne::create();
	listen->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false   
    listen->onTouchBegan = [](Touch* touch, Event* event){    
        
        return true;    
    };     
    listen->onTouchMoved = [](Touch* touch, Event* event){       
    };     
     
    listen->onTouchEnded = [=](Touch* touch, Event* event){    
		this->removeFromParent();
    };  
	EventDispatcher* dispatch=Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen,this);

	//
	ignoreAnchorPointForPosition(false);
	setAnchorPoint(Vec2(0.5,0.5));
	setPosition(Vec2(Actual_x/2,Actual_y/2));
	setContentSize(Size(Actual_x,Actual_y));
}


void BasicLayer::showShake(Node* sp){

	sp->setScale(0.2f);
	sp->runAction(Sequence::create(
		FadeOut::create(0),
		Spawn::create(FadeIn::create(0.05f),
		ScaleTo::create(0.1f,1.05f),
		nullptr),
		
		ScaleTo::create(0.1f,1.0f),
		nullptr));

	
}


void BasicLayer::daoju_reward(Ref* sender){


	if(ActiveValueHelper::getHelper()->canGetGift()){
		auto layer=BasicLayer::create();
		layer->active_warldbox(ActiveValueHelper::getHelper()->getOneGift_useActiveNum());
		Director::getInstance()->getRunningScene()->addChild(layer,1000);

		EventCustom event(Huoyuelinjian);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	}
	this->removeFromParent();
}

void BasicLayer::linqu_active(Ref* ref){

	 MenuItemImage* p=(MenuItemImage*)ref;
	 auto label=Label::createWithSystemFont(FX::StringsMap::getValue("yilinju"),"Arial",24);
	 label->setColor(Color3B(255,33,33));
	 label->setRotation(-45.0f);
	 label->setPosition(Vec2(ww->getContentSize().width*0.6,ww->getContentSize().height*0.4));
	 ww->addChild(label,100);
	 p->setVisible(false);

}


void BasicLayer::Cannokaifang(string title ,string desc){

	if (title == "" || desc == "")
	{
		title = "eao";
		desc = "ggnwkf";
	}
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);
	 
	auto p=Sprite::create(tck_img);
	p->setPosition(Vec2(Actual_x/2,Actual_y/2));
	colorLayer1->addChild(p);

	
	auto h_name=Label::createWithSystemFont(FX::StringsMap::getValue(title),"minijiankatong.ttf",40);
	h_name->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.8));
	h_name->setColor(Color3B(254,218,185));
	h_name->setTag(100);
	p->addChild(h_name);

	auto h_name1=Label::createWithSystemFont(FX::StringsMap::getValue(title),"minijiankatong.ttf",40);
	h_name1->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.8-2));
	h_name1->setTag(90);
	p->addChild(h_name1);
	


	auto h_name2=Label::createWithSystemFont(FX::StringsMap::getValue(desc),"minijiankatong.ttf",30);
	h_name2->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.5));
	h_name2->setColor(Color3B(254,218,185));
	h_name2->setTag(100);
	p->addChild(h_name2);

	auto h_name3=Label::createWithSystemFont(FX::StringsMap::getValue(desc),"minijiankatong.ttf",30);
	h_name3->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.5-2));
	h_name3->setTag(90);
	p->addChild(h_name3);

	showShake(p);
}


void BasicLayer::maojitishi(int tag){

	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);
	 
	auto p=Sprite::create(tck_img);
	p->setPosition(Vec2(Actual_x/2,Actual_y/2));
	colorLayer1->addChild(p);

	
	auto h_name=Label::createWithSystemFont(FX::StringsMap::getValue("pefect"),"minijiankatong.ttf",40);
	h_name->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.8));
	h_name->setLocalZOrder(100);
	p->addChild(h_name);


	auto h_name0=Label::createWithSystemFont(FX::StringsMap::getValue("pefect"),"minijiankatong.ttf",40);
	h_name0->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.8-1));
	h_name0->setLocalZOrder(90);
	p->addChild(h_name0);



	string  shuoming=FX::StringsMap::getValue("yijiaorenwusdzgj");
	if(tag==102){
		shuoming=FX::StringsMap::getValue("yijiaorenwusdzgj");
	}else if(tag==103){
		shuoming=FX::StringsMap::getValue("yijiaopetsdzgj");
	}else if(tag==104){
		shuoming=FX::StringsMap::getValue("yijiaomagicsdzgj");
	}
	auto h_name1=Label::createWithSystemFont(shuoming,"minijiankatong.ttf",30);
	h_name1->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.4));
	h_name1->setColor(Color3B(254,218,185));
	h_name1->setLocalZOrder(100);
	p->addChild(h_name1);

	auto h_name2=Label::createWithSystemFont(shuoming,"minijiankatong.ttf",30);
	h_name2->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.4-1));
	h_name2->setLocalZOrder(90);
	p->addChild(h_name2);


    showShake(p);

}

void BasicLayer::shuoming(string sm){

	 
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(320,220));// 
	colorLayer1->addChild(scal9);
	 
	auto sm_label=Label::createWithSystemFont(sm,"Arial",24,Size(280,0),TextHAlignment::CENTER);
	sm_label->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height/2));
	sm_label->setColor(Color3B(0,0,0));
	scal9->addChild(sm_label);

    auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(BasicLayer::Close_callback,this));
	close->setPosition(Vec2(scal9->getContentSize().width- close->getContentSize().width/2,scal9->getContentSize().height-close->getContentSize().height/2));

	auto menu = Menu::create(close,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu, 2);


}

void BasicLayer::Close_callback(Ref* ref){
	this->removeFromParent();
}



void BasicLayer::cancel_callback(Ref* ref){

}


void BasicLayer::Chankanyoujian(Email* ifo,BaselisttableLayer* pp){


	this->plist=pp;
	eamilid=ifo->id;

	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(Vec2(Actual_x/2,Actual_y/2));
	scal9->setContentSize(Size(320,220));// 
	colorLayer1->addChild(scal9);
    

	string img_str="ui_chengjiu_weiwancheng.png";//  ui_chengjiu_wancheng1.png
		string type_txt=FX::StringsMap::getValue("Typemsg1");
		string opratefs="chakan";
		if(ifo->type==Email::ET_SystemBonus){
			img_str="ui_chengjiu_wancheng1.png";
			type_txt=FX::StringsMap::getValue("Typemsg1");
		}else if(ifo->type==Email::ET_SystemNotice){
			type_txt=FX::StringsMap::getValue("Typemsg2");
		}else if(ifo->type==Email::ET_FriendChat){
			type_txt=FX::StringsMap::getValue("Typemsg3");
		}

	auto labelname=Label::createWithSystemFont(type_txt,"Arial",24);
	labelname->setColor(Color3B(255,33,33));
	labelname->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.85));
	scal9->addChild(labelname,100);

	auto dscriblable=Label::createWithSystemFont(ifo->content,"Arial",24,Size(300,0),TextHAlignment::CENTER);
	dscriblable->setColor(Color3B(172,65,0));
	dscriblable->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height/2));
	scal9->addChild(dscriblable,100);

	

	MenuItemImage* p=MenuItemImage::create();
	p->setPosition(Vec2(scal9->getContentSize().width- p->getContentSize().width/2,scal9->getContentSize().height-p->getContentSize().height/2));
	p->setCallback(CC_CALLBACK_1(BasicLayer::closelinquW,this));
	p->setNormalImage(Sprite::create("ui_shangcheng_guanbi.png"));
	p->setSelectedImage(Sprite::create("ui_shangcheng_guanbi.png"));
	

	if(ifo->type==Email::ET_SystemBonus){

		auto str=Label::createWithSystemFont(FX::StringsMap::getValue("yifasong"),"Arial",24);
		str->setColor(Color3B(255,33,33));
		str->setPosition(Vec2(scal9->getContentSize().width*0.3,scal9->getContentSize().height*0.3));
		scal9->addChild(str,100);

		if(ifo->bonus_unitID>0){
		auto m_img=ImageHelper::getUnitSpriteUIScene(ifo->bonus_unitID);
	    m_img->setPosition(Vec2(scal9->getContentSize().width*0.5,scal9->getContentSize().height*0.3));
		scal9->addChild(m_img,100);

		auto count_l=Label::createWithSystemFont(__String::createWithFormat("%d",ifo->bonusCount)->getCString(),"Arial",24);
		count_l->setColor(Color3B(255,33,33));
		count_l->setPosition(Vec2(scal9->getContentSize().width*0.7,scal9->getContentSize().height*0.3));
		scal9->addChild(count_l,100);
		GoodsHelper::getHelper()->changeOneUnitCount(ifo->bonus_unitID,ifo->bonusCount);

		}
	}
	auto menu = Menu::create(p,nullptr);
    menu->setPosition(Vec2::ZERO);
	scal9->addChild(menu,2);
	showShake(scal9);
}

void BasicLayer::closelinquW(Ref* ref){

	plist->rmoveeamil(eamilid);
	this->removeFromParent();

}

void BasicLayer::linquW(Ref* ref){
	 
	MenuItemImage* p=(MenuItemImage*)ref;
	Email* ifo=(Email*)p->getUserData();


} 

void BasicLayer::setTextb(MenuItemImage* node,string str,int fontsize,Color3B b){

	auto label=Label::createWithSystemFont(FX::StringsMap::getValue(str),"Arial",fontsize);
	label->setPosition(Vec2(node->getContentSize().width/2,node->getContentSize().height/2));
	label->setColor(b);
	node->addChild(label);
}


void BasicLayer::wardBox(int id){

	  //"bao.png"
	
 
	wdId=id;
	color =Color4B(0,0,0,100);
	colorLayer2 = LayerColor::create(color);
	colorLayer2->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer2,1);


    
	
	auto bsp=Sprite::create("daoju_baoxiang3.png");
	bsp->setPosition(Vec2(Actual_x/2,Actual_y*0.8));
	colorLayer2->addChild(bsp);

	
	

	BagItem item;

	if(RandomGiftBagHelper::getHelper()->getGiftBagTable().size()>wdId){
	   item=RandomGiftBagHelper::getHelper()->getGiftBagTable().at(wdId).first;
	}else{
	   //{"unitID":510,"value":1,"randomWeight":20},{"unitID":98,"value":3000,"randomWeight":10},{"unitID":505,"value":5,"randomWeight":5},{"unitID":510,"value":2,"randomWeight":5}

       if(wdId==5){
		   item.id=unitID_Gold;
		   item.value=3000;
	   }else if(wdId==6){
		   item.id=unitID_KaiChangChongCi;
		   item.value=5;
	   }else if(wdId==7){
		   item.id=unitID_HuDun;
		   item.value=5;
	   }else if(wdId==8){
		    item.id=unitID_Gold;
		    item.value=5000;
	   }
	}
	const Unit* unit=GoodsHelper::getHelper()->getUnitByID(item.id);
	string miaoshu=unit->name+" * "+__String::createWithFormat("%d",item.value)->getCString();

	auto daoju=ImageHelper::getUnitSpriteRunningScene(unit->ID);// Sprite::create("ui_icon_jinbi.png");
	daoju->setPosition(Vec2(Actual_x/2,Actual_y/2));
	colorLayer2->addChild(daoju);

	auto mlabel=Label::createWithSystemFont(miaoshu,"Arial",30);
	mlabel->setPosition(Vec2(Actual_x/2,daoju->getPositionY()-daoju->getContentSize().height/2-mlabel->getContentSize().height/2));
	colorLayer2->addChild(mlabel);
	
	auto button=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(BasicLayer::linWard,this));
	this->setTextb(button,"linju",30,Color3B(0,0,0));
	button->setPosition(Vec2(Actual_x/2,Actual_y*0.3));

	auto menu=Menu::create(button,nullptr);
	menu->setPosition(Vec2::ZERO);
	colorLayer2->addChild(menu);


}

void BasicLayer::linWard(Ref* ref){


	BagItem item;
	if(RandomGiftBagHelper::getHelper()->getGiftBagTable().size()>wdId){
		item=RandomGiftBagHelper::getHelper()->getGiftBagTable().at(wdId).first;
	}else{
	     if(wdId==5){
		   item.id=unitID_Gold;
		   item.value=3000;
	   }else if(wdId==6){
		   item.id=unitID_KaiChangChongCi;
		   item.value=5;
	   }else if(wdId==7){
		   item.id=unitID_HuDun;
		   item.value=5;
	   }else if(wdId==8){
		    item.id=unitID_Gold;
		    item.value=5000;
	   }
	}
	GoodsHelper::getHelper()->changeOneUnitCount(item.id,item.value);
	this->removeFromParent();
	GameDirector::getDirector()->gotoUIScene();
}


float BasicLayer::fgRangeRand(  float min, float max ){

	
    float rnd = CCRANDOM_0_1();
    return rnd*(max-min)+min;

}

void BasicLayer::chankan(Node* node){


	nodes->removeFromParent();

	auto daoju=Sprite::create("ui_icon_jinbi.png");
	daoju->setPosition(Vec2(Actual_x/2,Actual_y/2));
	colorLayer2->addChild(daoju);


	BagItem item;

	if(RandomGiftBagHelper::getHelper()->getGiftBagTable().size()>wdId){
		item=RandomGiftBagHelper::getHelper()->getGiftBagTable().at(wdId).first;
	}else{
	     if(wdId==5){
		   item.id=unitID_Gold;
		   item.value=3000;
	   }else if(wdId==6){
		   item.id=unitID_KaiChangChongCi;
		   item.value=5;
	   }else if(wdId==7){
		   item.id=unitID_HuDun;
		   item.value=5;
	   }else if(wdId==8){
		    item.id=unitID_Gold;
		    item.value=5000;
	   }
	}

	
	const Unit* unit=GoodsHelper::getHelper()->getUnitByID(item.id);
	string miaoshu=unit->name+" * "+__String::createWithFormat("%d",item.value)->getCString();

	auto mlabel=Label::createWithSystemFont(miaoshu,"Arial",30);
	mlabel->setPosition(Vec2(Actual_x/2,daoju->getPositionY()-daoju->getContentSize().height/2-mlabel->getContentSize().height/2));
	colorLayer2->addChild(mlabel);
	
	auto button=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(BasicLayer::linWard,this));
	this->setTextb(button,"linju",30,Color3B(0,0,0));
	button->setPosition(Vec2(Actual_x/2,Actual_y*0.3));

	auto menu=Menu::create(button,nullptr);
	menu->setPosition(Vec2::ZERO);
	colorLayer2->addChild(menu);

}



void BasicLayer::buySuccecs(BagItem item,string str){

	  
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);
	 
	auto p=Sprite::create(tck_img);
	p->setPosition(Vec2(Actual_x/2,Actual_y/2));
	colorLayer1->addChild(p);
    
	auto h_name=Label::createWithSystemFont(FX::StringsMap::getValue(str),"minijiankatong.ttf",40);
	h_name->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.8));
	h_name->setColor(Color3B(254,218,185));
	h_name->setTag(100);
	p->addChild(h_name);

	auto h_name1=Label::createWithSystemFont(FX::StringsMap::getValue(str),"minijiankatong.ttf",40);
	h_name1->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.8-2));
	h_name1->setTag(90);
	p->addChild(h_name1);
	

	const Unit* unit=GoodsHelper::getHelper()->getUnitByID(item.id);
	string hd_str=FX::StringsMap::getValue("huode")+":";
	auto h_name2=Label::createWithSystemFont(hd_str,"minijiankatong.ttf",30);
	h_name2->setColor(Color3B(254,218,185));
	h_name2->setTag(100);
	p->addChild(h_name2);

	auto wuping=Sprite::create(unit->imageFile);
	wuping->setAnchorPoint(Vec2(0,0.5));
	p->addChild(wuping);
    
	auto num_gold=Label::createWithSystemFont(__String::createWithFormat("%d",item.value)->getCString(),"minijiankatong.ttf",30);
	num_gold->setAnchorPoint(Vec2(0,0.5));
	num_gold->setColor(Color3B(254,218,185));
	num_gold->setTag(100);
	p->addChild(num_gold);

	float px,py;
	px=(p->getContentSize().width-(h_name2->getContentSize().width+wuping->getContentSize().width+num_gold->getContentSize().width))/2;
	py=p->getContentSize().height*0.35;

	h_name2->setAnchorPoint(Vec2(0,0.5));
	h_name2->setPosition(Vec2(px,py));

	wuping->setPosition(Vec2(h_name2->getPositionX()+h_name2->getContentSize().width,py));
	num_gold->setPosition(Vec2(wuping->getPositionX()+wuping->getContentSize().width,py));
	showShake(p);
	  

}


void BasicLayer::buystr(string tishi,string desc){

	 
	color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);
	 
	auto p=Sprite::create(tck_img);
	p->setPosition(Vec2(Actual_x/2,Actual_y/2));
	colorLayer1->addChild(p);
    
	auto h_name=Label::createWithSystemFont(FX::StringsMap::getValue(tishi),"minijiankatong.ttf",40);
	h_name->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.8));
	h_name->setColor(Color3B(254,218,185));
	h_name->setTag(100);
	p->addChild(h_name);

	if(desc != "temp"){
		auto h_name_desc=Label::createWithSystemFont(FX::StringsMap::getValue(desc),"minijiankatong.ttf",40);
		h_name_desc->setPosition(Vec2(p->getContentSize().width/2,p->getContentSize().height*0.4));
		h_name_desc->setColor(Color3B(254,218,185));
		h_name_desc->setTag(100);
		p->addChild(h_name_desc);
	}


}