#include "MsgnetLayer.h"


MsgnetLayer::MsgnetLayer(void)
{
}


MsgnetLayer::~MsgnetLayer(void)
{
}


bool MsgnetLayer::init(){

	if(!Layer::init()){
		return false;
	}
	
	isRegist=false;
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	
	currentrsp=0;
	jiemian();

	if(PlayerInfoHelper::getHelper()->getUserId()==0){
		//得注册
		NetmsgManger* manager=NetmsgManger::getNetManager();
		manager->setMsg_Center(std::bind(&MsgnetLayer::Regist_return,this,std::placeholders::_1));
		manager->isregist();
	    isRegist=true;
	}else{
	    laodData();
	}
	return true;
}
void MsgnetLayer::onEnter(){

	Layer::onEnter();
}
void MsgnetLayer::onExit(){

	EmailHelper::getHeper()->save();
	NetmsgManger::getNetManager()->setMsgc_null();
	Layer::onExit();
}


float MsgnetLayer::getDelwithint(float number,int iswidth){
	  
	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
    return dealnumber;

	
}


void MsgnetLayer::laodData(){

	 NetmsgManger* manager=NetmsgManger::getNetManager();
	 manager->setMsg_Center(std::bind(&MsgnetLayer::Regist_return,this,std::placeholders::_1));
	 manager->sendMail();
	 
}
void MsgnetLayer::inintNetmsg(bool bk){

	 
	//BaseLayer::getBaseLayer()->freep();

	if (bk==false)
	{
		text->setString(FX::StringsMap::getValue("weilianjie"));
	}else{

		//信息数据列表
		list=BaselisttableLayer::create();
		list->setMails_list(NetmsgManger::getNetManager()->mails);
		list->setMtabletype(Msgnet_list,Size(499,100),3,cocos2d::extension::ScrollView::Direction::VERTICAL,TableView::VerticalFillOrder::TOP_DOWN,0);
	    list->ignoreAnchorPointForPosition(false);
	    list->setAnchorPoint(Vec2(0.5f,0.5f));
	    list->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height*0.5));
	    scal9->addChild(list,99);
		removeChildByTag(99);

	}


}


void MsgnetLayer::jiemian(){

	this->setContentSize(Size(Actual_x,Actual_y-getDelwithint(Tophight,1)));
    
	 //width=getDelwithint(525,0);
	 // hight=getDelwithint(450,1);
	scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setContentSize(Size(getDelwithint(480,0),getDelwithint(520,1)));// 459,365
	scal9->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
	addChild(scal9,1);

	std::string tt= FX::StringsMap::getValue("shujujiazz")+"...";
	text=Label::createWithSystemFont(tt, "Arial", 34);
	text->setColor(Color3B(0,0,0));
	text->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
	text->setTag(99);

	addChild(text,100);

}

void MsgnetLayer::Regist_return(int tag){

	 if(tag==0){
		 text->setString(FX::StringsMap::getValue("weilianjie"));
	 }else if(tag==1){
		 laodData();
	 }else if(tag==3){
		 inintNetmsg(true);
	 }else if(tag==2){

		 if(EmailHelper::getHeper()->getAllEmailList(30).size()==0){
		    inintNetmsg(false);
		 }else{
			inintNetmsg(true);
		 }
	 }else if(tag==4){
		 if(EmailHelper::getHeper()->getAllEmailList(30).size()==0){
		   text->setString(FX::StringsMap::getValue("myyj"));
		 }else{
			inintNetmsg(true);
		 }
	 }
	 

}

