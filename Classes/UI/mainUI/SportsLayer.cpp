#include "SportsLayer.h"


SportsLayer::SportsLayer(void)
{
}


SportsLayer::~SportsLayer(void)
{
}


bool SportsLayer::init(){

	if(!Layer::init()){

		return false;
	}

	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
    m_curPage = 1;  
	this->Shieldtouched();

	
	return true;

}



void SportsLayer::loadbasic(){

	float width=Actual_x*0.8f;
	float hight=Actual_y*0.8f;

	Color4B color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);
	this->setContentSize(colorLayer1->getContentSize());


	auto scal8=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal8->setContentSize(Size(width,hight));

	
	scal8->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
	colorLayer1->addChild(scal8);


	jt1=Sprite::create("ui_zhuye_guanqiamoshi_guanqia_anniu_fanye.png");
	jt1->setPosition(Vec2(scal8->getPositionX()-scal8->getContentSize().width/2+jt1->getContentSize().width/2,scal8->getPositionY()));
	
	jt1->setRotationSkewY(180.0f);
	jt1->setVisible(false);
	colorLayer1->addChild(jt1,100);

	jt2=Sprite::create("ui_zhuye_guanqiamoshi_guanqia_anniu_fanye.png");
	jt2->setPosition(Vec2(scal8->getPositionX()+scal8->getContentSize().width/2-jt1->getContentSize().width/2,scal8->getPositionY()));
	//jt2->setFlippedY(180);
	jt2->setVisible(true);
	colorLayer1->addChild(jt2,100);


	scroll_layer = Layer::create();//创建scrollView中的容器层   
	scroll_layer->setPosition(Vec2(0,0));  
	scroll_layer->setAnchorPoint(Vec2(0.0f,0.0f));  
	scroll_layer->setContentSize(Size(width*2,hight));//设置容器层大小为（600，300）   


  
	scrollView = ScrollView::create(Size(width,hight),scroll_layer);//创建scrollView，显示窗口大小为(400,300)   
    scrollView->setDelegate(this);//设置委托  
	scrollView->setContentOffset(Vec2::ZERO);//充当了锚点的作用  
	scrollView->setTouchEnabled(false);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);//设置滚动方向为水平   

	scrollView->ignoreAnchorPointForPosition(false);
	scrollView->setAnchorPoint(Vec2(0.5,0.5));
    scrollView->setPosition(scal8->getPosition());  
	colorLayer1->addChild(scrollView,20);  



	
	

	/*
	text_ts=Label::createWithSystemFont(FX::StringsMap::getValue("shujujiazz"),"Arial",30);
	text_ts->setColor(Color3B(0,0,0));
	text_ts->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
	colorLayer1->addChild(text_ts,1000);
	*/
	
	auto close=MenuItemImage::create("ui_shangcheng_guanbi.png","ui_shangcheng_guanbi.png",CC_CALLBACK_1(SportsLayer::returnPa,this));
	close->setPosition(Vec2(scal8->getPositionX()+scal8->getContentSize().width/2-close->getContentSize().width/2,
		                    scal8->getPositionY()+scal8->getContentSize().height/2-close->getContentSize().height/2));


	auto menu = Menu::create(close,nullptr);
    menu->setPosition(Vec2::ZERO);
	colorLayer1->addChild(menu);
     

	for(int i=0;i<2;i++){
		if(i==0){
				drowSecond(scroll_layer,Vec2(width*i+width/2,hight/2));
		 }else{
				drowFisrt(scroll_layer,Vec2(width*i+width/2,hight/2));
		 }
	 }
}


void SportsLayer::Vesion(int vesion){

	vsion=vesion;
	if(vesion==1){
		loadbasic();
	}else if(vesion==2){
		loadNet();
	}

	

}

void SportsLayer::loadNet(){

	mark=0;
	float width=525;
	float hight=450;

	
	this->setContentSize(Size(width,hight));
    
	auto scal8=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal8->setContentSize(Size(width,hight));

	
	scal8->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
	addChild(scal8,100);


	jt1=Sprite::create("ui_zhuye_guanqiamoshi_guanqia_anniu_fanye.png");
	jt1->setPosition(Vec2(scal8->getPositionX()-scal8->getContentSize().width/2+jt1->getContentSize().width/2,scal8->getPositionY()));
	
	jt1->setRotationSkewY(180.0f);
	jt1->setVisible(false);
	scal8->addChild(jt1,100);

	jt2=Sprite::create("ui_zhuye_guanqiamoshi_guanqia_anniu_fanye.png");
	jt2->setPosition(Vec2(scal8->getPositionX()+scal8->getContentSize().width/2-jt1->getContentSize().width/2,scal8->getPositionY()));;
	jt2->setVisible(true);
	scal8->addChild(jt2,100);


	scroll_layer = Layer::create();//创建scrollView中的容器层   
	scroll_layer->setPosition(Vec2(0,0));  
	scroll_layer->setAnchorPoint(Vec2(0.0f,0.0f));  
	scroll_layer->setContentSize(Size(width*2,hight));//设置容器层大小为（600，300）   


  
	scrollView = ScrollView::create(Size(width,hight),scroll_layer);//创建scrollView，显示窗口大小为(400,300)   
    scrollView->setDelegate(this);//设置委托  
	scrollView->setContentOffset(Vec2::ZERO);//充当了锚点的作用  
	scrollView->setTouchEnabled(false);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);//设置滚动方向为水平   

	scrollView->ignoreAnchorPointForPosition(false);
	scrollView->setAnchorPoint(Vec2(0.5,0.5));
    scrollView->setPosition(scal8->getPosition());  
	scal8->addChild(scrollView,20);  

	
	auto biti2=Sprite::create(Biti_imgstr2);
	biti2->setPosition(Vec2(scal8->getContentSize().width/2,0));
	scal8->addChild(biti2,1025);

    b_label=Label::createWithSystemFont(FX::StringsMap::getValue("wdfzsm"),"Arial",24);
	b_label->setPosition(Vec2(biti2->getContentSize().width/2,biti2->getContentSize().height/2));
	b_label->setColor(Color3B(0,0,0));
	biti2->addChild(b_label);
	
	auto jj_bg=Sprite::create(Biti_imgstr);
	jj_bg->setPosition(Vec2(width/2,hight));
	addChild(jj_bg,1025);

	auto jjgz=Label::createWithSystemFont(FX::StringsMap::getValue("jinjiguizhe"),"Arial",30);
	jjgz->setColor(Color3B(0,0,0));
	jjgz->setPosition(Vec2(jj_bg->getContentSize().width/2,jj_bg->getContentSize().height/2));
	jj_bg->addChild(jjgz,100);

	for(int i=0;i<2;i++){
		if(i==0){
			    
				drowFisrt(scroll_layer,Vec2(width*i+width/2,hight/2));
		 }else{
                drowSecond(scroll_layer,Vec2(width*i+width/2,hight/2));
				
		 }
	 }

}


void SportsLayer::onEnter(){

	Layer::onEnter();
}
void SportsLayer::onExit(){

	NetmsgManger::getNetManager()->mark=-1;
	//this->unscheduleAllSelectors();

	Layer::onExit();

}

float SportsLayer::getDelwithint(float number,int iswidth){


	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	
	return dealnumber;
}




void SportsLayer::scrollViewDidScroll(cocos2d::extension::ScrollView* view){



	//在scrollView拖动时响应该函数   
  
  

}
void SportsLayer::scrollViewDidZoom(cocos2d::extension::ScrollView* view){


}
void SportsLayer::scrollViewMoveOver(cocos2d::extension::ScrollView* view){


}



void SportsLayer::Shieldtouched(){

	//事件
	auto listen=EventListenerTouchOneByOne::create();
	listen->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false   
    listen->onTouchBegan = [this](Touch* touch, Event* event){    

		m_touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView()); 
        return true;  
    };     
    listen->onTouchMoved = [this](Touch* touch, Event* event){       
    };     
     
    listen->onTouchEnded = [this](Touch* touch, Event* event){     

		

		Vec2 endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());  

		if(endPoint.x>=this->getPositionX()-getContentSize().width/2&&endPoint.x<=this->getPositionX()+getContentSize().width/2){

        float distance = endPoint.x - m_touchPoint.x;  
		if (fabs(distance) > 50){  

			float width=Actual_x*0.8f;
	        float hight=Actual_y*0.8f;

			if(vsion==1){
			   width=Actual_x*0.8f;
	           hight=Actual_y*0.8f;
			}else if(vsion==2){
			   width=525;
	           hight=450;
			}
			Size visibaleSize = Size(width,hight);
				

				 if (distance < 0)  
			   {  
				   m_curPage++;  
				}else{  
					m_curPage--;  
				}  
  
 
  
			    if (m_curPage < 1)  
				{  
					m_curPage = 1;  
				}  
  
				if (m_curPage > 2)  
				{  
					m_curPage = 2;  
				}   
				
				Vec2 origin = Director::getInstance()->getVisibleOrigin();  
				Vec2 adjustPos =Vec2(origin.x - (m_curPage - 1) * visibaleSize.width,0);  
				scrollView->setContentOffset(adjustPos,true);

				if(m_curPage>1){
					jt1->setVisible(true);
					jt2->setVisible(false);
					
					if(vsion==2)
					  b_label->setString(FX::StringsMap::getValue("dfljing"));
					  
				}else if(m_curPage<2){
					jt1->setVisible(false);
					jt2->setVisible(true);
					
					if(vsion==2)
					  b_label->setString(FX::StringsMap::getValue("wdfzsm"));
					  
				}
	           
         }
		}
    };  


	EventDispatcher* dispatch=Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen,this);

	//
	ignoreAnchorPointForPosition(false);
	setAnchorPoint(Vec2(0.5,0.5));
	setPosition(Vec2(Actual_x/2,Actual_y/2));
	setContentSize(Size(Actual_x,Actual_y));
}


void SportsLayer::drowFisrt(Layer* node,Vec2 p){

	 
	//黑色:RGB 000 

	 float width=Actual_x*0.8f;
	 float hight=Actual_y*0.8f;
	 int fontsize=22;
	 if(vsion==1){
			width=Actual_x*0.8f;
	        hight=Actual_y*0.8f;
	 }else if(vsion==2){
		 width=525;
		 hight=450;
			fontsize=20;
	 }

	 auto scal9=Layer::create();
	 scal9->setContentSize(Size(width,hight));// 
	 scal9->ignoreAnchorPointForPosition(false);
	 scal9->setAnchorPoint(Vec2(0.5,0.5));
	 scal9->setPosition(p);
	 node->addChild(scal9,1);


	 l_one=scal9;
	auto sm1=Label::createWithSystemFont(FX::StringsMap::getValue("shuoming1"),"Arial",fontsize);
	sm1->setColor(Color3B(0,0,0));
	sm1->setPosition(Vec2(sm1->getContentSize().width/2+20,hight*0.87));
	scal9->addChild(sm1);

	auto sm2=Label::createWithSystemFont(FX::StringsMap::getValue("shuoming2"),"Arial",fontsize);
	sm2->setColor(Color3B(0,0,0));
	sm2->setPosition(Vec2(sm2->getContentSize().width/2+20,hight*0.81));
	scal9->addChild(sm2);

	auto sm3=Label::createWithSystemFont(FX::StringsMap::getValue("shuoming3"),"Arial",fontsize,Size(width-40,0),TextHAlignment::LEFT);
	sm3->setColor(Color3B(0,0,0));
	sm3->setPosition(Vec2(sm3->getContentSize().width/2+20,hight*0.69));
	scal9->addChild(sm3);
	

	string name_str="";
	if(NetmsgManger::getNetManager()->mark==1){
	   name_str=NetmsgManger::getNetManager()->getMy_name();
	}else{
		schedule(schedule_selector(SportsLayer::get_myZm), 0.5f);
		rla=Label::createWithSystemFont("","Arial",24);
		rla->setColor(Color3B(0,0,0));
		rla->setPosition(Vec2(scal9->getContentSize().width/2,scal9->getContentSize().height/2));
		scal9->addChild(rla);
	}
	//我的组名
	name=Label::createWithSystemFont(name_str,"Arial",fontsize);
	name->setColor(Color3B(0,0,0));
	name->setPosition(width/2,hight*0.6);
	scal9->addChild(name);
	//  
}


void SportsLayer::drawMyself(){


	 NetPalyermsg msgp=NetmsgManger::getNetManager()->msgNet;
	 vector<RankingData> m_v=RankingListHelper::getHelper()->getOneGroupRankingList(msgp.cur_group);
	 
	 float width=Actual_x*0.8f;
	 float hight=Actual_y*0.8f;
	 int fontsize=22;
	 if(vsion==1){
			width=Actual_x*0.8f;
	        hight=Actual_y*0.8f;
	 }else if(vsion==2){
		 width=525;
		 hight=450;
			fontsize=20;
	 }


	 for(size_t i=0;i<m_v.size();i++){

		string numbers=__String::createWithFormat("%d",m_v.at(i).rankId)->getCString();
		if(i<3){
			numbers=__String::createWithFormat("%d",m_v.at(i).rankId)->getCString();
		}else{
			string sr=__String::createWithFormat("number%d",(i+1))->getCString();
			numbers=FX::StringsMap::getValue(sr);
		}

		auto number=Label::createWithSystemFont(numbers,"Arial",fontsize);
		number->setColor(Color3B(0,0,0));
		number->setPosition(Vec2(width*0.08+number->getContentSize().width/2,hight*(0.52-i*0.07)));
		l_one->addChild(number);

		//ImageHelper::getUnitSpriteRunningScene(m_v.at(i).bonus_unitId);
		auto warld_mc=Sprite::create("ui_icon_jinbi.png");
		warld_mc->setPosition(Vec2(width*0.7,hight*(0.52-i*0.07)));
		warld_mc->setScale(0.7f);
		l_one->addChild(warld_mc);

		auto number1=Label::createWithSystemFont(__String::createWithFormat("%d",m_v.at(i).bonusCount)->getCString(),"Arial",fontsize);
		number1->setColor(Color3B(0,0,0));
		number1->setPosition(Vec2(warld_mc->getPositionX()+warld_mc->getContentSize().width/2+20+number1->getContentSize().width/2,hight*(0.52-i*0.07)));
		l_one->addChild(number1);

	}
	
	





}


void SportsLayer::drowone(Layer* node,int num,int index,RankingData data){


	

	float x=0;
	float y=0;
	auto scal8=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal8->setContentSize(Size(getDelwithint(50,0),getDelwithint(200,1)));
	NetPalyermsg msgp=NetmsgManger::getNetManager()->msgNet;
	x=20+scal8->getContentSize().width/2+(5+scal8->getContentSize().width)*index;
	y=20+scal8->getContentSize().height/2+20*index;
	scal8->setPosition(Vec2(x,y));
	node->addChild(scal8);

	auto name=Label::createWithSystemFont(data.describe,"Arial",24);
	name->setColor(Color3B(0,0,0));
	name->setPosition(Vec2(scal8->getContentSize().width/2,scal8->getContentSize().height*0.65));
	scal8->addChild(name,100);
	m_l.insert(data.groupId,name);

    if(msgp.cur_group==data.groupId){
		name->setColor(Color3B(255,33,33));
	}

	auto warld_mc=ImageHelper::getUnitSpriteUIScene(data.bonus_unitId);
	warld_mc->setPosition(Vec2(scal8->getContentSize().width/2,scal8->getContentSize().height/2));
	warld_mc->setScale(0.7f);
	scal8->addChild(warld_mc,100);

	auto number1=Label::createWithSystemFont(__String::createWithFormat("%d",data.bonusCount)->getCString(),"Arial",24);
	number1->setColor(Color3B(0,0,0));
	number1->setPosition(Vec2(scal8->getContentSize().width/2,scal8->getContentSize().height*0.25));
	scal8->addChild(number1,100);



}

 void SportsLayer::drowSecond(Layer* node,Vec2 p){




	float  width=525;
	float  hight=450;
	float fontsize=20;

	 auto scal9=Layer::create();
	 scal9->setContentSize(Size(width,hight)); // 
	 scal9->ignoreAnchorPointForPosition(false);
	 //scal9->setAnchorPoint(Vec2(0.5,0.5));
	 scal9->setPosition(p);
	 node->addChild(scal9,1);
	 
	 auto grup=Label::createWithSystemFont(FX::StringsMap::getValue("groupsore"),"Arial",28);
	 grup->setColor(Color3B(0,0,0));
	 grup->setPosition(Vec2(width/2,hight*0.85));
	 scal9->addChild(grup);
	 
	 vector<RankingData> m_vecRankingList=RankingListHelper::getHelper()->getAllRankingList();
	 vector<RankingData> m_v;
	 for(auto data_m: m_vecRankingList){
		 if(data_m.rankId==1){
			 m_v.push_back(data_m);
		 }
	 }
	 int r_size=m_v.size();
	 for(int i=0;i<r_size;i++){
	  
	     drowone(scal9,r_size,i,m_v.at(i));
	}

	
 }


 void SportsLayer::returnPa(Ref* sender){


	 this->removeFromParent();

 }


 void SportsLayer::loadNetdata(){
 
	  
	  NetmsgManger* manager=NetmsgManger::getNetManager();
	  if(manager->getMy_name().size()>0&&manager->bounds.size()>0){

		  
		  for(size_t j=0;j<manager->bounds.size();j++){
			  bounds.push_back(manager->bounds.at(j));
		  }
		  loadJiemian(true);
	  }else{//从新加载---》
		  CCLOG("loat---->tectdata");
		  isloadData=true;
		  NetmsgManger::getNetManager()->iswhoLoad=2;
		  NetmsgManger::getNetManager()->load_data();
	  }


 }


 void SportsLayer::loadJiemian(bool isnet){

     

	


	 if(isnet==true){
		 float width=Actual_x*0.8f;
	     float hight=Actual_y*0.8f;
		 for(int i=0;i<2;i++){
			 if(i==0){
				drowFisrt(scroll_layer,Vec2(width*i+width/2,hight/2));
			 }else{
				drowSecond(scroll_layer,Vec2(width*i+width/2,hight/2));
			 }
	     }

	 }else{

		 text_ts->setString(FX::StringsMap::getValue("dataerror"));

	 }
 }



 


 void SportsLayer::isregist(){


	  
	  auto callBack       =  [&](EventCustom* evt)  
                            {  

								MsgData* data=(MsgData*)evt->getUserData();
								if(data->errorCode==0){//错误码
									loadJiemian(false);
								}else if(data->errorCode==1){//

									 NetmsgManger* manager=NetmsgManger::getNetManager();
	                                 if(manager->getMy_name().size()>0&&manager->bounds.size()>0){

										 for(size_t i=0;i<manager->bounds.size();i++){
											 bounds.push_back(manager->bounds.at(i));
										 }
												  loadJiemian(true);
									 }else{
										 loadJiemian(false);
									 }
								}
                            };  
	    auto Data_changevent=EventListenerCustom::create(Load_rankBounds,callBack);
	    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(Data_changevent,1);  

 }



 //得到自己的组名
string SportsLayer::getMy_name(){

	vector<RankingData> m_vecRankingList=RankingListHelper::getHelper()->getAllRankingList();
	string name;

	for(size_t i=0;i<m_vecRankingList.size();i++){
		if(NetmsgManger::getNetManager()->msgNet.cur_group==m_vecRankingList.at(i).groupId){
			name=m_vecRankingList.at(i).describe;
			break;
		}
	}
	return name;
}


void SportsLayer::setPtion_xy(Vec2 v2){

	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vec2(0.5,0.5));
	this->setPosition(v2);

}


void SportsLayer::get_myZm(float time){

	if(NetmsgManger::getNetManager()->mark==1){
		 name->setString(NetmsgManger::getNetManager()->getMy_name());
		 NetPalyermsg msgp=NetmsgManger::getNetManager()->msgNet;
		 if(msgp.cur_group!=-1){
			 drawMyself();
			 m_l.at(msgp.cur_group)->setColor(Color3B(255,33,33));
			 mark=1;
		 }
		 unschedule(schedule_selector(SportsLayer::get_myZm));
	 }

}

void SportsLayer::Czdata(){
	 
	 

}