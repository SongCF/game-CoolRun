#include "AchieveLayer.h"
#include "GoodsHelper.h"
#include "BasicLayer.h"

#define  achive_back_img "UI_shangcheng_bg.png"
#define  achive_bt_img   "UI_chengjiu_biaoti.png"

#define  acive_select_img "UI_chengjiu_fenye_select.png"
#define  acive_fenye_img "UI_chengjiu_fenye.png"

#define  acive_mubiao_back_img "UI_chengjiu_neirong_bg_weiwancheng.png"
#define  acive_yidaoc_back_img "UI_chengjiu_neirong_bg_weilingqu.png"
#define  acive_yilinqu_back_img "UI_chengjiu_neirong_bg_yiwancheng.png"

#define acive_namemb_back "UI_chengjiu_neirong_biaoti_weiwancheng.png"
#define acive_nameyidac_back "UI_chengjiu_neirong_biaoti_yiwancheng.png"

enum AciveTag{
	 Linqu_tag,
	 Close_tag,
	 acive_close_tag,
};

AchieveLayer::AchieveLayer(void)
{
}


AchieveLayer::~AchieveLayer(void)
{
}


bool AchieveLayer::init(){
	  

	if(!Layer::init()){
		 return false;
	}

	//初始化数据

	acives=(vector<Achieve>)*AchievementHelper::getHelper()->getAllAchievement();
    nodes=0;
	int numgk=acives.size();
    if(numgk%6==0){
		nodes=numgk/6;
	}else{
		nodes=numgk/6+1;
	}

	//处理数据
	vector<Achieve> haveGet;
	vector<Achieve> unGet;
	for (size_t i = 0; i < acives.size(); i++)
	{
		bool isfinish=AchievementHelper::getHelper()->getAchieveByID(acives.at(i).ID)->isOver;
		bool islq=AchievementHelper::getHelper()->getIsGotAchievement(acives.at(i).ID);
		if (isfinish || islq)
		{
			haveGet.push_back(acives.at(i));
		}else{
			unGet.push_back(acives.at(i));
		}
	}
	sort(haveGet.begin(),haveGet.end(),[&](Achieve a,Achieve b){
		return a.ID <b.ID;
	});
	sort(unGet.begin(),unGet.end(),[&](Achieve a,Achieve b){
		return a.ID <b.ID;
	});
	acives.clear();
	acives.insert(acives.begin(),haveGet.begin(),haveGet.end());
	acives.insert(acives.end(),unGet.begin(),unGet.end());
	// float back_w;float back_h;

	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
    m_curPage = 1;  
	this->Shieldtouched();
	this->setContentSize(Size(Actual_x,Actual_y-getDelwithint(Tophight,1)));

	ignoreAnchorPointForPosition(false);
	setAnchorPoint(Vec2(0.0f,0.0f));
	//setPosition(Vec2(Actual_x/2,Actual_y/2-getDelwithint(20,1)));
	float hi_t=Actual_y-getDelwithint(Tophight,1)-20;
	//#define  achive_back_img "UI_shangcheng_bg.png" #define  achive_bt_img   "UI_chengjiu_biaoti.png"

	auto color =Color4B(0,0,0,100);
	LayerColor *colorLayer1 = LayerColor::create(color);
	colorLayer1->setContentSize(Size(Actual_x,Actual_y));
	addChild(colorLayer1,1);

	shop_back=Sprite::create(achive_back_img);
	shop_back->setPosition(Vec2(Actual_x/2,Actual_y/2-30));
	colorLayer1->addChild(shop_back);

	auto shop_bt=Sprite::create(achive_bt_img);
	shop_bt->setPosition(Vec2(Actual_x/2,shop_back->getContentSize().height/2+
		shop_back->getPositionY()));
	shop_bt->setLocalZOrder(90);
	colorLayer1->addChild(shop_bt);


	auto closeshop=MenuItemImage::create("UI_jiaose_anniu_guanbi.png","UI_jiaose_anniu_guanbi.png",CC_CALLBACK_1(AchieveLayer::allcallback,this));
	closeshop->setPosition(Vec2(shop_back->getContentSize().width-closeshop->getContentSize().width/2,shop_back->getContentSize().height-closeshop->getContentSize().height/4));
	closeshop->setTag(acive_close_tag);


	auto menu=Menu::create(closeshop,nullptr);
	menu->setPosition(Vec2::ZERO);
	menu->setLocalZOrder(100);
	shop_back->addChild(menu);

	back_w=shop_back->getContentSize().width-80;
	back_h=shop_back->getContentSize().height;

	scroll_layer = Layer::create();//创建scrollView中的容器
	scroll_layer->setContentSize(Size((shop_back->getContentSize().width-80)*nodes,back_h));//设置容器层大小为（600，300）   
	scrollView = ScrollView::create(Size(back_w,back_h),scroll_layer);//创建scrollView，显示窗口大小为(400,300)   
    scrollView->setDelegate(this);//设置委托  
	scrollView->setContentOffset(Vec2::ZERO);//充当了锚点的作用  
	scrollView->setTouchEnabled(false);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);//设置滚动方向为水平   
	scrollView->setPosition(Vec2(40,0));
	shop_back->addChild(scrollView,100);  
	//完成进度
	Wangcheng();
	float spc=getDelwithint(20,0);
	for(int i=1;i<=nodes;i++){
		auto* sp=Sprite::create(acive_fenye_img);
		float px=(shop_back->getContentSize().width-sp->getContentSize().width*nodes-spc*(nodes-1))/2;
		sp->setPosition(Vec2(px+sp->getContentSize().width/2+(i-1)*(sp->getContentSize().width+spc),back_h*0.125));
		sp_vec.pushBack(sp);
		shop_back->addChild(sp,125);
	}
	sp_vec.at(0)->setTexture(Sprite::create(acive_select_img)->getTexture());
	for(int i=0;i<nodes;i++){
		drowgk_mode(scroll_layer,Vec2(back_w*i+back_w/2,back_h/2),i+1);
	}
	return true;
}
void  AchieveLayer::Wangcheng(){

	float f_value=0;//
	vector<Achieve> chives=*AchievementHelper::getHelper()->getAllAchievement();
	for(size_t i=0;i<chives.size();i++){

		if(AchievementHelper::getHelper()->getAchieveByID(chives.at(i).ID)->isOver==true){
			f_value++;
		}
	}
	float need_Experience=AchievementHelper::getHelper()->getAllAchievement()->size();
	//进度条
	auto tiLiBarBoarder =Sprite::create("UI_chengjiu_jindutiao_bg.png");
	shop_back->addChild(tiLiBarBoarder,135);
	tiLiBarBoarder->setPosition(Vec2(shop_back->getContentSize().width/2,shop_back->getContentSize().height*0.05));

	auto progress = ProgressTimer::create(Sprite::create("UI_chengjiu_jindutiao.png"));
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


	string wc_str=__String::createWithFormat("%.1f",(f_value/need_Experience)*100)->getCString();
	wc_str+="%";
	auto wc_label=Label::createWithSystemFont(wc_str,"Arial",24);
	//wc_label->setColor(Color3B(0,108,191));
	wc_label->setPosition(Vec2(tiLiBarBoarder->getContentSize().width/2,tiLiBarBoarder->getContentSize().height/2));
	tiLiBarBoarder->addChild(wc_label,1000);




}

void AchieveLayer::onEnter(){

	Layer::onEnter();
}
void AchieveLayer::onExit(){

	Layer::onExit();
}



float AchieveLayer::getDelwithint(float number,int iswidth){

	
	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	
	return dealnumber;

}





void AchieveLayer::scrollViewDidScroll(cocos2d::extension::ScrollView* view){



	//在scrollView拖动时响应该函数   
  
  

}
void AchieveLayer::scrollViewDidZoom(cocos2d::extension::ScrollView* view){


}
void AchieveLayer::scrollViewMoveOver(cocos2d::extension::ScrollView* view){


}


void AchieveLayer::Shieldtouched(){

	//事件
	auto listen=EventListenerTouchOneByOne::create();
	listen->setSwallowTouches(false);//设置不想向下传递触摸  true是不想 默认为false   
    listen->onTouchBegan = [this](Touch* touch, Event* event){    
		m_touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView()); 
        return true;  
    };     
    listen->onTouchMoved = [this](Touch* touch, Event* event){       
    };     
    listen->onTouchEnded = [this](Touch* touch, Event* event){     

		Vec2 endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());  
        float distance = endPoint.x - m_touchPoint.x;  
		if (fabs(distance) > 50){  

			float hi_t=back_h;
			Size visibaleSize = Size(back_w,back_h);
				

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
				if (m_curPage > nodes)  
				{  
					m_curPage = nodes;  
				}   


				Vec2 origin = Director::getInstance()->getVisibleOrigin();  
				Vec2 adjustPos =Vec2(origin.x - (m_curPage - 1) * visibaleSize.width,0);  
				scrollView->setContentOffset(adjustPos,true);
				for(int i=1;i<=nodes;i++){
				   if(i==m_curPage){
					    sp_vec.at(i-1)->setTexture(Sprite::create(acive_select_img)->getTexture());
				   }else{
					    sp_vec.at(i-1)->setTexture(Sprite::create(acive_fenye_img)->getTexture());
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



void AchieveLayer::drowgk_mode(Layer* node,Vec2 p,int id){

	 
	 float hi_t=back_h;
	 auto scal9=Layer::create(); 
	 scal9->ignoreAnchorPointForPosition(false);
	 scal9->setAnchorPoint(Vec2(0.5f,0.5f));
	 scal9->setPosition(p);
	 scal9->setContentSize(Size(back_w,back_h));// 
	 node->addChild(scal9,1);
	 vector<Achieve> pgk;
	 for(size_t i=0;i<acives.size();i++){
		  if(i/6+1==id){
			  pgk.push_back(acives.at(i));
		  }
	  }
	  float x=0.0f;
	  float y=0.0f;
	  float jg=0.0f;
  
	    int k=0;
	    for(size_t i=0;i<pgk.size();i++){

					bool isfinish=AchievementHelper::getHelper()->getAchieveByID(pgk.at(i).ID)->isOver;
					bool islq=AchievementHelper::getHelper()->getIsGotAchievement(pgk.at(i).ID);
					 
			        string back=acive_mubiao_back_img;
					string name_l=acive_namemb_back;
					if(isfinish){ 
					    name_l=acive_nameyidac_back;
					}
					if(isfinish==true&&islq==false){
                        back=acive_yidaoc_back_img;
					}
					if(isfinish==true&&islq==true){
						back=acive_yilinqu_back_img;
					}

					 k=i;
					 y=scal9->getContentSize().height*0.7;
					 auto m=Sprite::create(back);
					
					 if(k>=3){
						 k-=3;
						 y=scal9->getContentSize().height*0.325;
					 } 
					 float width_b=m->getContentSize().width;
					 jg=(scal9->getContentSize().width-width_b*3)/4;
					 x=jg*(k+1)+k*width_b+width_b*0.5;
					 m->setPosition(Vec2(x,y));
					 scal9->addChild(m);


					 //成就名称
					 auto na_back=Sprite::create(name_l);
					 na_back->setPosition(Vec2(m->getContentSize().width/2,m->getContentSize().height*0.89f));
					 m->addChild(na_back);

					 auto a_name=Label::createWithSystemFont(pgk.at(i).name,"minijiankatong.ttf",30);
					 a_name->setPosition(Vec2(na_back->getContentSize().width/2,na_back->getContentSize().height*0.5f));
					 na_back->addChild(a_name);



					 if(isfinish==false){
					  //成就说明
						 string dscrible=pgk.at(i).describe;
						 auto dc_label=Label::createWithSystemFont(dscrible,"minijiankatong.ttf",24,Size(160,0),TextHAlignment::CENTER);
						 dc_label->setColor(Color3B(114,9,20));
						 dc_label->setPosition(Vec2(m->getContentSize().width/2,m->getContentSize().height*0.25));
						 m->addChild(dc_label); 	  

					 }else if(isfinish==true&&islq==false){

						  //const Achieve *p=AchievementHelper::getHelper()->getAchieveByID(pgk.at(i).);
						  //AchievementHelper::getHelper()->getAchievement(id);
						  const Unit* p= GoodsHelper::getHelper()->getUnitByID(pgk.at(i).award_unitID);
						  int num=pgk.at(i).count;
						  auto sp_rt=Sprite::create(p->imageFile);
						  string num_str="X";
						  num_str+=__String::createWithFormat("%d",num)->getCString();
						  auto str_label=Label::createWithSystemFont(num_str,"minijiankatong.ttf",30);
						  str_label->setColor(Color3B(184,69,13));

						  float x=(m->getContentSize().width-sp_rt->getContentSize().width-str_label->getContentSize().width)/2+sp_rt->getContentSize().width/2;
						  sp_rt->setPosition(Vec2(x,m->getContentSize().height/2));
						  m->addChild(sp_rt);

						  str_label->setPosition(Vec2(sp_rt->getPositionX()+sp_rt->getContentSize().width/2+str_label->getContentSize().width/2,m->getContentSize().height/2));
						  m->addChild(str_label);

						  /*
						    Linqu_tag,
	                        Close_tag,
						  */
						  auto linqu_b=MenuItemImage::create("UI_chengjiu_anniu_lingqu.png","UI_chengjiu_anniu_lingqu.png",CC_CALLBACK_1(AchieveLayer::allcallback,this));
						  linqu_b->setPosition(Vec2(m->getContentSize().width/2,linqu_b->getContentSize().height/2));
						  linqu_b->setTag(Linqu_tag);
						  linqu_b->setUserData((void*)pgk.at(i).ID);

						  auto menu=Menu::create(linqu_b,nullptr);
						  menu->setPosition(Vec2::ZERO);
						  m->addChild(menu);

						 auto youe=Sprite::create("UI_zhuye_tishi_youshang.png");
						 youe->setPosition(Vec2(m->getContentSize().width-20,m->getContentSize().height-20));
						 m->addChild(youe,100);
						 youe->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5,0.8f),
																 ScaleTo::create(0.5,1.0f),nullptr
																 )));
						 

					 }else if(isfinish==true&&islq==true){


						 auto labele28=Label::createWithSystemFont(FX::StringsMap::getValue("yidaoc"),"minijiankatong.ttf",28);
						 labele28->setPosition(Vec2(m->getContentSize().width/2,m->getContentSize().height*0.2));
						 m->addChild(labele28);
						 labele28->setLocalZOrder(100);

						 auto lablecx=Label::createWithSystemFont(FX::StringsMap::getValue("yidaoc"),"minijiankatong.ttf",28);//,Size(160,0),TextHAlignment::CENTER);
						 lablecx->setPosition(Vec2(m->getContentSize().width/2,m->getContentSize().height*0.2-2));
						 lablecx->setColor(Color3B(184,69,13));
						 lablecx->setLocalZOrder(90);
						 m->addChild(lablecx); 
					 }
					ac_back_change.insert(pgk.at(i).ID,m);
		}

}





void AchieveLayer::allcallback(Ref* ref){
    
	MenuItemImage* p=(MenuItemImage*)ref;
	int tag=(int)p->getTag();
	current_oprate=tag;
    FX::Music::getInstance()->buttonClicked();
	if(tag==Linqu_tag){
		 
		 p->setCallback(nullptr);
         aciveid=(int)(long long)p->getUserData();
	}
	bAction(p);

 }

 void AchieveLayer::bAction(MenuItemImage* node){

	 node->runAction(	
		             Sequence::create(ScaleTo::create(0.1f,1.2f),CallFuncN::create( CC_CALLBACK_0(AchieveLayer::callsp,
							  this,node,true)),nullptr)
					);
 }

 void AchieveLayer::callsp(Node* node,bool mr){

	node->setScale(1.0f);

	if(current_oprate==Linqu_tag){
		
		const Achieve* iv=AchievementHelper::getHelper()->getAchieveByID(aciveid);
		AchievementHelper::getHelper()->getAchievement(iv->ID);
		
		ac_back_change.at(aciveid)->removeAllChildren();
		ac_back_change.at(aciveid)->setTexture(Sprite::create(acive_yilinqu_back_img)->getTexture());

		 auto labele28=Label::createWithSystemFont(FX::StringsMap::getValue("yidaoc"),"minijiankatong.ttf",28);
		 labele28->setPosition(Vec2(ac_back_change.at(aciveid)->getContentSize().width/2,ac_back_change.at(aciveid)->getContentSize().height*0.2));
		 ac_back_change.at(aciveid)->addChild(labele28);
		 labele28->setLocalZOrder(100);

		 auto lablecx=Label::createWithSystemFont(FX::StringsMap::getValue("yidaoc"),"minijiankatong.ttf",28);//,Size(160,0),TextHAlignment::CENTER);
		 lablecx->setPosition(Vec2(ac_back_change.at(aciveid)->getContentSize().width/2,ac_back_change.at(aciveid)->getContentSize().height*0.2-2));
		 lablecx->setColor(Color3B(184,69,13));
		 lablecx->setLocalZOrder(90);
		 ac_back_change.at(aciveid)->addChild(lablecx);
		 warld(iv);
	}else if(current_oprate==acive_close_tag){

		   this->removeFromParent();
		   EventCustom event_m(Layerchange);
		   LayerData data_m;
		   data_m.current=105;
		   event_m.setUserData(&data_m);
		   Director::getInstance()->getEventDispatcher()->dispatchEvent(&event_m);
	}
 }

 

 void AchieveLayer::warld(const Achieve* iv){

	 const Unit* p= GoodsHelper::getHelper()->getUnitByID(iv->award_unitID);
	 int num=iv->count;
	
	 BagItem bag;
	 bag.id=p->ID;
	 bag.value=num;
	 auto basic=BasicLayer::create();
	 basic->buySuccecs(bag,"hd");
     Director::getInstance()->getRunningScene()->addChild(basic,2000);

 }

 void AchieveLayer::spcall(Node* node,bool mr){

	 node->removeFromParent();
	  
 }

 void AchieveLayer::linqu_Callback(Ref* ref){


	 MenuItemImage* p=(MenuItemImage*)ref;
	 int id=(int)(long long)p->getUserData();
	 Lin_Id=id;
	 BaseLayer* layer=BaseLayer::create();
	 layer->Shieldtouched();
	 layer->Achievementpromptbox_net(id);
	 layer->setMsg_Center(std::bind(&AchieveLayer::Lin_Ward,this));
	 Director::getInstance()->getRunningScene()->addChild(layer);
	  

 }


 void AchieveLayer::Lin_Ward(){

	 ac_button.at(Lin_Id)->removeFromParent(); 

 }


 