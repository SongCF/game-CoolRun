#include "RodeoLayer.h"



RodeoLayer::RodeoLayer(void)
{
}


RodeoLayer::~RodeoLayer(void)
{
}

bool RodeoLayer::init(){

	if(!Layer::init()){
		return false;
	}
	
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	loadjiemian();

	

	return true;
}
void RodeoLayer::onEnter(){

	Layer::onEnter();
}
void RodeoLayer::onExit(){

	NetmsgManger::getNetManager()->setMsgc_null();
	Layer::onExit();
}


void RodeoLayer::ininRodeolayer(){

	
    loadjiemian();
}


float RodeoLayer::getDelwithint(float number,int iswidth){

	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
    return dealnumber;

}


void RodeoLayer::loadjiemian(){

	 
	this->setContentSize(Size(Actual_x,(Design_hight-Tophight)*(Actual_y/Design_hight)));

	auto splayer=SportsLayer::create();
	splayer->Vesion(2);
	splayer->setPtion_xy(Vec2(splayer->getContentSize().width/2+10,getContentSize().height/2));
	addChild(splayer);
	

	//float  width=getDelwithint(525,0);
	//float  hight=getDelwithint(450,1);
	scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setContentSize(Size(getDelwithint(415,0),450));// 459,365
	scal9->setPosition(Vec2(splayer->getContentSize().width/2+splayer->getPositionX()+scal9->getContentSize().width/2+10,getContentSize().height/2));
	addChild(scal9,1);
	

	auto jj_img=Sprite::create(Biti_imgstr);
	jj_img->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height));
	scal9->addChild(jj_img,100);

	auto jj_pm=Label::createWithSystemFont(FX::StringsMap::getValue("Jingjipaiming"),"Arial",30);
	jj_pm->setPosition(Vec2(jj_img->getContentSize().width/2,jj_img->getContentSize().height/2));
	jj_pm->setColor(Color3B(0,0,0));
	jj_img->addChild(jj_pm);



	auto l_wimg=Sprite::create(Biti_imgstr3);
	l_wimg->setPosition(Vec2(scal9->getContentSize().width/2,l_wimg->getContentSize().height/2));
	scal9->addChild(l_wimg,100);

	string time=FX::StringsMap::getValue("julijiesuan")+__String::createWithFormat("%d",NetmsgManger::getNetManager()->DaySearch())->getCString()+FX::StringsMap::getValue("xiaoshi");
	auto ti_label=Label::createWithSystemFont(time,"Arial",30);
	ti_label->setColor(Color3B(255,33,33));
	ti_label->setPosition(Vec2(l_wimg->getContentSize().width/2,l_wimg->getContentSize().height/2));
	l_wimg->addChild(ti_label);



	string ld_text=FX::StringsMap::getValue("dataload")+"...";
	data_ld=Label::createWithSystemFont(ld_text,"Arial",30);
	data_ld->setColor(Color3B(0,0,0));
	data_ld->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height/2));
	scal9->addChild(data_ld,1000);
	data_ld->setTag(1000);


	if(PlayerInfoHelper::getHelper()->getUserId()==0){

		NetmsgManger* manager=NetmsgManger::getNetManager();
		manager->setMsg_Center(std::bind(&RodeoLayer::Regist_return,this,std::placeholders::_1));
		manager->isregist();
	    isRegist=true;
	}else{

		Rankmsg(true);
	}
	
}


void RodeoLayer::load_jjdata(){
	     
	    scal9->removeChildByTag(1000);
	   
	    auto jj_list=BaselisttableLayer::create();
		jj_list->ignoreAnchorPointForPosition(false);
		jj_list->setAnchorPoint(Vec2(0.5f,0.5f));
		jj_list->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.5));
		scal9->addChild(jj_list,99);
		jj_list->setMtabletype(Sports_list,Size(getDelwithint(425,0),100),3,cocos2d::extension::ScrollView::Direction::VERTICAL,TableView::VerticalFillOrder::TOP_DOWN,0);
		

}


void RodeoLayer::Rankmsg(bool isbd){

	 if(isbd){
		NetmsgManger* manager=NetmsgManger::getNetManager();
		manager->setMsg_Center(std::bind(&RodeoLayer::Regist_return,this,std::placeholders::_1));
		manager->loadNetRank();
	 }else{
        NetmsgManger::getNetManager()->loadNetRank();
	 }
}

void RodeoLayer::Regist_return(int tag){

	 if(tag==0){
		 data_ld->setString(FX::StringsMap::getValue("weilianjie"));
	 }else if(tag==1){
		 Rankmsg(false);
	 }else if(tag==2){
		 load_jjdata();
	 }else if(tag==3){
		 data_ld->setString(FX::StringsMap::getValue("dataerror"));
	 }

}


