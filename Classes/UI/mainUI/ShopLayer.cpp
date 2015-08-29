#include "ShopLayer.h"
#include "MarketHelper.h"
#include "BasicLayer.h"
#include "GameDirector.h"


#define Ui_sc_bg  "UI_shangcheng_bg.png"
#define Ui_sc_bt  "UI_shangcheng_biaoti.png"
#define Ui_sc_hbstart "UI_shangcheng_anniu_huobi.png"
#define Ui_sc_hbselect "UI_shangcheng_anniu_huobi_select.png"
#define Ui_sc_fdstart "UI_shangcheng_anniu_shiwu.png"
#define Ui_sc_fdselect "UI_shangcheng_anniu_shiwu_select.png"
#define Ui_sc_layerbacklibao "UI_shangcheng_shangpin_libao_bg.png"
#define Ui_sc_layerbacknolibao "UI_shangcheng_shangpin_bg.png"
#define Ui_sc_layer_wp   "UI_shangcheng_shangpin_mingzi_bg.png"
#define Ui_sc_layer_wp2   "UI_shangcheng_shangpin_mingzi_bg_2.png"
#define Ui_sc_zs_20   "UI_shangcheng_shangpin_zuanshi_20.png"
#define Ui_sc_zs_70   "UI_shangcheng_shangpin_zuanshi_70.png"
#define Ui_sc_zs_140   "UI_shangcheng_shangpin_zuanshi_140.png"

#define Ui_zuire_xian "UI_shangcheng_shangpin_cuxiao_zuirexiao.png"
#define Ui_zuishi_hui "UI_shangcheng_shangpin_cuxiao_zuishihui.png"

#define Ui_biaoti_shangc "UI_shangcheng_zhuangshi.png"


enum ShopTag{

	 shop_hb_tag=1000,
	 shop_fd_tag,
	 shop_close_tag,
	 shop_moneylayer_tag,
	 shop_foodlayer_tag,
	 shop_buy_tag,

};

ShopLayer::ShopLayer(void)
	: m_pClocseCallback(nullptr)
{
}


ShopLayer::~ShopLayer(void)
{
}



bool ShopLayer::init(){
	  

	if(!Layer::init()){
		 return false;
	}
	
	dataInint();
	drawShop();
	return true;

}
void ShopLayer::onEnter(){

	
	auto callBack       =  [&](EventCustom* evt)  
                            {  

								

								 GameDirector::getDirector()->hideWaiting();
								 BagItem item;
								 if(sales.size()>0){
									 item=sales.top();
									 sales.pop();
								 }
								 MsgData* data=(MsgData*)evt->getUserData();
								 if(data->errorCode==Rsp_Success){
				                        
									 auto basic=BasicLayer::create();
									 basic->buySuccecs(item);
									 Director::getInstance()->getRunningScene()->addChild(basic,2000);


								 }else if(data->errorCode==Rsp_Gold_UnEnough){
										//调转到商店页面
									   //qxgmjb 
									 auto basic=BasicLayer::create();
									 basic->buystr("qxgmjb ");
									 Director::getInstance()->getRunningScene()->addChild(basic,2000);

								 }else if(data->errorCode==Rsp_Jewel_UnEnough){
										//调转到商店页面qxgmzs
									 auto basic=BasicLayer::create();
									 basic->buystr("qxgmzs");
									 Director::getInstance()->getRunningScene()->addChild(basic,2000);
								 }else if(data->errorCode==Rsp_RedJewel_UnEnough){
									 ////调转到商店页面qxgmzs
									 //auto basic=BasicLayer::create();
									 //basic->buystr("qxgmzs");
									 //Director::getInstance()->getRunningScene()->addChild(basic,2000);

								 }else if(data->errorCode==Rsp_Need_UnEnough){
									 int a = 0;

								 }else if(data->errorCode==Rsp_DontHaveThing){
										//没有该物品
								 }else if(data->errorCode==Rsp_NotSupport){
									 auto basic=BasicLayer::create();
									 basic->buystr("xjsdk");
									 Director::getInstance()->getRunningScene()->addChild(basic,2000);
								 }
                             };  
	buy_listener=EventListenerCustom::create(MSG_MarketBuyGoods_Rsp,callBack);
	_eventDispatcher->addEventListenerWithFixedPriority(buy_listener,1);  
	
	Layer::onEnter();
}
void ShopLayer::onExit(){
	
	_eventDispatcher->removeEventListener(buy_listener);
	Layer::onExit();
}



float ShopLayer::getDelwithint(float number,int iswidth){

	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;

}

void ShopLayer::dataInint(){

	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	shops=MarketHelper::getHelper()->getAllChargeItems();
	for(size_t i=0;i<shops->size();i++){
		int id=shops->at(i).unitSaleID;
		if(id>=unitID_Gold&&id<=unitID_Diamond){
			shop_moneys.push_back(shops->at(i));
		}else{
			shop_foods.push_back(shops->at(i));
		}
	}


}

void ShopLayer::drawMoney(int tag,vector<SaleItem> shop_moneys){
	 
	 //#define Ui_sc_layerbacklibao "UI_shangcheng_shangpin_libao_bg.png" #define Ui_sc_layerbacknolibao "UI_shangcheng_shangpin_bg.pngg"
	 shop_back->removeChildByTag(shop_moneylayer_tag);
	 auto p=Layer::create();
	 p->setTag(shop_moneylayer_tag);
	 p->setContentSize(shop_back->getContentSize());
	 float x,y,jg;
	 int k=0;
	 for(size_t i=0;i<shop_moneys.size();i++){

		 string img_back=Ui_sc_layerbacknolibao;
		 k=i;
		
		
		 y=p->getContentSize().height*0.62;
		
		 auto m=Sprite::create(img_back);
		 if(k>=3){
			 k-=3;
			 y=p->getContentSize().height*0.27;
		 } 
		 float width_b=m->getContentSize().width;
		 jg=(p->getContentSize().width-width_b*3)/4;
		 x=jg*(k+1)+k*width_b+width_b*0.5;
		 m->setPosition(Vec2(x,y));
		 p->addChild(m);

		 /*
		 #define Ui_zuire_xian "UI_shangcheng_shangpin_cuxiao_zuirexiao.png"
         #define Ui_zuishi_hui "UI_shangcheng_shangpin_cuxiao_zuishihui.png"
		 */
		 if(i==2&&tag==0){
		    auto z_rx=Sprite::create(Ui_zuire_xian);
			z_rx->setPosition(Vec2(m->getContentSize().width*0.95,m->getContentSize().height*0.35));
			m->addChild(z_rx);
			z_rx->setLocalZOrder(100);
		 }
		 if(i==5&&tag==0){
		    auto z_sh=Sprite::create(Ui_zuishi_hui);
			z_sh->setPosition(Vec2(m->getContentSize().width*0.95,m->getContentSize().height*0.35));
			m->addChild(z_sh);
			z_sh->setLocalZOrder(100);
		 }


		 //卖的什么 Ui_sc_layer_wp
		 Sprite* sale_thing= nullptr;
		 if((i<=1)&& tag==0){
			sale_thing=Sprite::create(Ui_sc_layer_wp2);
		 }else{
			sale_thing=Sprite::create(Ui_sc_layer_wp);
		 }
			
		 //if(i==0){
		 //   sale_thing->setPosition(Vec2(m->getContentSize().width/2,m->getContentSize().height*0.7));
		 //}else{
			//sale_thing->setPosition(Vec2(m->getContentSize().width/2,m->getContentSize().height*0.85));
		 //}
		 sale_thing->setPosition(Vec2(m->getContentSize().width/2,m->getContentSize().height*0.85));
		 m->addChild(sale_thing);
		 //Label::createWithSystemFont(hero.unit.name,"minijiankatong.ttf",40);
		 string name_sale=__String::createWithFormat("%d",shop_moneys.at(i).saleCount)->getCString();
		 name_sale+=GoodsHelper::getHelper()->getUnitByID(shop_moneys.at(i).unitSaleID)->name;
		 auto label_str=Label::createWithSystemFont(name_sale,"minijiankatong.ttf",30);
		 label_str->setPosition(Vec2(sale_thing->getContentSize().width/2,sale_thing->getContentSize().height/2));
		 sale_thing->addChild(label_str);

		 string my_img;
		 if(tag==0){
			 my_img="UI_zhuye_jinbilan_zuanshi.png";
			 if(shop_moneys.at(i).unitSaleID==unitID_Gold){
				 my_img="UI_zhuye_jinbilan_jinbi.png";
			 }else if(shop_moneys.at(i).unitSaleID==unitID_Diamond){
				 my_img="UI_zhuye_jinbilan_zuanshi.png";
			 }

			 if(i==0){
				 if(shop_moneys.at(i).unitSaleID==unitID_Diamond){
					 my_img=Ui_sc_zs_20;
				 }
			 }else if(i==1){
				  if(shop_moneys.at(i).unitSaleID==unitID_Diamond){
					 my_img=Ui_sc_zs_70;
				 }
			 }else if(i==2){
				   if(shop_moneys.at(i).unitSaleID==unitID_Diamond){
					 my_img=Ui_sc_zs_140;
				 }
			 }
		 }else if(tag==1){

			/*
		    unitID_TangGuo			= 94, //糖果
			unitID_GuoDong			= 95, //甜筒
			unitID_DanGao			= 96, //蛋糕
			unitID_RandomItem		= 97, //商店随机道具（随机宝箱）
			unitID_Gold				= 98, //金币
			unitID_Diamond			= 99, //钻石
		    unitID_RMB				= 100, //人名币
			*/
			const Unit* unit=GoodsHelper::getHelper()->getUnitByID(shop_moneys.at(i).unitSaleID);
			my_img=unit->imageFile;
			 if(i==0){
				 if(shop_moneys.at(i).unitSaleID==unitID_TangGuo){
					 my_img="UI_shangcheng_shangpin_shiwu_tangguo_5.png";//"UI_shangcheng_shangpin_shiwu_dangao_5.png";
				 }
			 }else if(i==1){
				  if(shop_moneys.at(i).unitSaleID==unitID_GuoDong){
					 my_img=my_img="UI_shangcheng_shangpin_shiwu_tiantong_5.png";;// "UI_shangcheng_shangpin_shiwu_tiantong_5.png";//UI_shangcheng_shangpin_shiwu_dangao_40.png";
				 }
			 }else if(i==2){
				   if(shop_moneys.at(i).unitSaleID==unitID_DanGao){
					 my_img="UI_shangcheng_shangpin_shiwu_dangao_5.png";;
				 }
			 }else if(i==3){
				  if(shop_moneys.at(i).unitSaleID==unitID_TangGuo){
					 my_img="UI_shangcheng_shangpin_shiwu_tangguo_40.png";//
				 }
			 }else if(i==4){
				   if(shop_moneys.at(i).unitSaleID== unitID_GuoDong){
					my_img="UI_shangcheng_shangpin_shiwu_tiantong_40.png";//";
				 }
			 }else if(i==5){
				   if(shop_moneys.at(i).unitSaleID==unitID_DanGao){
					 my_img="UI_shangcheng_shangpin_shiwu_dangao_40.png";//";
				 }
			 }
		 }
		 auto money_imgsp=Sprite::create(my_img);
		 if(i==0){
		    money_imgsp->setPosition(Vec2(m->getContentSize().width/2,m->getContentSize().height*0.4));
		 }else{
			money_imgsp->setPosition(Vec2(m->getContentSize().width/2,m->getContentSize().height*0.5));
		 }
		 m->addChild(money_imgsp);
		 //购买
		 auto p_buy=MenuItemImage::create("UI_shangcheng_anniu_goumai.png","UI_shangcheng_anniu_goumai.png",CC_CALLBACK_1(ShopLayer::allcallback,this));
		 p_buy->setPosition(Vec2(m->getContentSize().width/2,p_buy->getContentSize().height*0.25));
		 fillOuty(p_buy,"UI_shangcheng_anniu_goumai.png");

		 SaleItem* p_sale=new SaleItem();
		 p_sale->payPrice=shop_moneys.at(i).payPrice;
		 p_sale->saleCount=shop_moneys.at(i).saleCount;
		 p_sale->unitPayID=shop_moneys.at(i).unitPayID;
		 p_sale->unitSaleID=shop_moneys.at(i).unitSaleID;
		 p_sale->saleID=shop_moneys.at(i).saleID;
		 p_buy->setUserData((void*)p_sale);
		 p_buy->setTag(shop_buy_tag);
		 {
			     string name_sale=__String::createWithFormat("%d",shop_moneys.at(i).payPrice)->getCString();
				 if(shop_moneys.at(i).unitPayID==unitID_RMB){
			         name_sale+=FX::StringsMap::getValue("yuan");
                     auto label_str=Label::createWithSystemFont(name_sale,"minijiankatong.ttf",30);
					 label_str->setPosition(Vec2(p_buy->getContentSize().width/2,p_buy->getContentSize().height/2));
					 p_buy->addChild(label_str);
				 }
				 if(shop_moneys.at(i).unitPayID==unitID_Gold	){
					  auto label_str=Label::createWithSystemFont(name_sale,"minijiankatong.ttf",30);
					  label_str->setAnchorPoint(Vec2(0.5,0.5));
					  auto p=Sprite::create("UI_zhuye_shangdian_jinbi.png");
					  float mx,my;
					  mx=(p_buy->getContentSize().width-label_str->getContentSize().width-p->getContentSize().width)/2+label_str->getContentSize().width/2;
					  my=p_buy->getContentSize().height/2;
					  label_str->setPosition(Vec2(mx,my));
					  p->setPosition(Vec2(mx+label_str->getContentSize().width/2+p->getContentSize().width/2,my));
					  p_buy->addChild(label_str);
					  p_buy->addChild(p);

				 }else if(shop_moneys.at(i).unitPayID==unitID_Diamond){
					  auto label_str=Label::createWithSystemFont(name_sale,"minijiankatong.ttf",30);
					  auto p=Sprite::create("UI_zhuye_shangdian_zuanshi.png");
					  float mx,my;
					  mx=(p_buy->getContentSize().width-label_str->getContentSize().width-p->getContentSize().width)/2+label_str->getContentSize().width/2;
					  my=p_buy->getContentSize().height/2;
					  label_str->setPosition(Vec2(mx,my));
					  p->setPosition(Vec2(mx+label_str->getContentSize().width/2+p->getContentSize().width/2,my));
					  p_buy->addChild(label_str);
					  p_buy->addChild(p);
				 }else if(shop_moneys.at(i).unitPayID==unitID_RedJewel){
					  auto label_str=Label::createWithSystemFont(name_sale,"minijiankatong.ttf",30);
					  auto p=Sprite::create("daoju_lvbaoshi.png");
					  p->setScale(0.7);
					  float mx,my;
					  mx=(p_buy->getContentSize().width-label_str->getContentSize().width-p->getContentSize().width)/2+label_str->getContentSize().width/2;
					  my=p_buy->getContentSize().height/2;
					  label_str->setPosition(Vec2(mx,my));
					  p->setPosition(Vec2(mx+label_str->getContentSize().width/2+p->getContentSize().width/2,my));
					  p_buy->addChild(label_str);
					  p_buy->addChild(p);
				 }
		 }
		 auto menu=Menu::create(p_buy,nullptr);
		 menu->setPosition(Vec2::ZERO);
		 m->addChild(menu);
	 }
	 shop_back->addChild(p);
}

void ShopLayer::drawShop(){

	//shop_hb_tag, shop_fd_tag, shop_close_tag,
	shop_back=Sprite::create(Ui_sc_bg);
	shop_back->setPosition(Vec2(Actual_x/2,Actual_y/2-30));
	addChild(shop_back);


	auto biaoti=Sprite::create(Ui_biaoti_shangc);
	biaoti->setPosition(Vec2(shop_back->getContentSize().width/2,shop_back->getContentSize().height*0.865));
	shop_back->addChild(biaoti);

	auto shop_bt=Sprite::create(Ui_sc_bt);
	shop_bt->setPosition(Vec2(Actual_x/2,shop_back->getContentSize().height/2+
		shop_back->getPositionY()));
	shop_bt->setLocalZOrder(90);
	addChild(shop_bt);
	
	//默认货币商城 sprite
	drawMoney(0,shop_moneys);
	
	huob_b=MenuItemImage::create(Ui_sc_hbstart,Ui_sc_hbselect,CC_CALLBACK_1(ShopLayer::allcallback,this));
	huob_b->setLocalZOrder(100);
	huob_b->setTag(shop_hb_tag);
	huob_b->setPosition(Vec2(shop_back->getContentSize().width*0.42,shop_back->getContentSize().height*0.865));

	fd_b=MenuItemImage::create(Ui_sc_fdstart,Ui_sc_fdselect,CC_CALLBACK_1(ShopLayer::allcallback,this));
	fd_b->setLocalZOrder(100);
	fd_b->setTag(shop_fd_tag);
	fd_b->setPosition(Vec2(huob_b->getPositionX()+huob_b->getContentSize().width/2+fd_b->getContentSize().width/2,shop_back->getContentSize().height*0.865));

	auto closeshop=MenuItemImage::create("UI_jiaose_anniu_guanbi.png","UI_jiaose_anniu_guanbi.png",CC_CALLBACK_1(ShopLayer::allcallback,this));
	closeshop->setPosition(Vec2(shop_back->getContentSize().width-closeshop->getContentSize().width/2,shop_back->getContentSize().height-closeshop->getContentSize().height/4));
	closeshop->setTag(shop_close_tag);
	fillOuty(closeshop,"UI_jiaose_anniu_guanbi.png");


	auto menu=Menu::create(huob_b,fd_b,closeshop,nullptr);
	menu->setPosition(Vec2::ZERO);
	menu->setLocalZOrder(100);
	shop_back->addChild(menu);
    setChanged(0);

}

void ShopLayer::allcallback(Ref* ref){

	MenuItemImage* p=(MenuItemImage*)ref;
	int tag=(int)p->getTag();
	current_oprate=tag;
    FX::Music::getInstance()->buttonClicked();
	if(tag==shop_buy_tag){
		saleitem=(SaleItem*)p->getUserData();
		BagItem item;
		item.id=saleitem->unitSaleID;
		item.value=saleitem->saleCount;
		sales.push(item);
		
	}
	if(tag==shop_hb_tag||tag==shop_fd_tag){

        if(tag==shop_hb_tag){
		  drawMoney(0,shop_moneys);
		  setChanged(0);
		}else if(tag==shop_fd_tag){
		  drawMoney(1,shop_foods);
		  setChanged(1);
		}
	}else {
		 if(current_oprate==shop_close_tag){
			if (m_pClocseCallback)
			{
				m_pClocseCallback();
			}
			else
			{
				EventCustom event(Layerchange);
				LayerData data;
				data.current=Layertag::gold_tag_close;
				data.push=0;
				event.setUserData(&data);
				Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
			}
			this->getParent()->removeFromParent();

		}else if(current_oprate==shop_buy_tag){
			int id=saleitem->saleID;
			MarketHelper::getHelper()->buyItem(id);
		}

	}
}

void ShopLayer::setChanged(int id){
	 
	if(id==0){
	   huob_b->setSelectedImage(Sprite::create(Ui_sc_hbstart));
	   huob_b->setNormalImage(Sprite::create(Ui_sc_hbselect));
	   fd_b->setSelectedImage(Sprite::create(Ui_sc_fdselect));
	   fd_b->setNormalImage(Sprite::create(Ui_sc_fdstart));
	}else if(id==1){
	   huob_b->setSelectedImage(Sprite::create(Ui_sc_hbselect));
	   huob_b->setNormalImage(Sprite::create(Ui_sc_hbstart));
	   fd_b->setSelectedImage(Sprite::create(Ui_sc_fdstart));
	   fd_b->setNormalImage(Sprite::create(Ui_sc_fdselect));
	}
}

void ShopLayer::bAction(MenuItemImage* node){
	
}


void ShopLayer::callsp(Node* node,bool mr){

	
	
}



void ShopLayer::Shieldtouched(){

	/*
	//事件
	auto listen=EventListenerTouchOneByOne::create();
	listen->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false   
    listen->onTouchBegan = [this](Touch* touch, Event* event){    


		m_touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView()); 

        return true;  

        return true;    
    };     
    listen->onTouchMoved = [this](Touch* touch, Event* event){       
    };     
     
    listen->onTouchEnded = [this](Touch* touch, Event* event){     

		

		Vec2 endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());  
        float distance = endPoint.x - m_touchPoint.x;  
		if (fabs(distance) > 50){  
			Size visibaleSize = Size(getDelwithint(930,0),getDelwithint(470,1));
				

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

				CCLOG("adjustPos.x---->%f",adjustPos.x);
				scrollView->setContentOffset(adjustPos,true);
				setWeizhi(m_curPage);
	            
         }  
    };  
	EventDispatcher* dispatch=Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen,this);
	//
	ignoreAnchorPointForPosition(false);
	setAnchorPoint(Vec2(0.5,0.5));
	setPosition(Vec2(Actual_x/2,Actual_y/2));
	setContentSize(Size(Actual_x,Actual_y));
	*/
}



void ShopLayer::drowGold(Layer* node,Vec2 p){



	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(p);
	scal9->setContentSize(Size(getDelwithint(25,0)*37.4,getDelwithint(21,1)*22.4));// 
	node->addChild(scal9);

	

	auto top=Sprite::create("ui_shangcheng_bg_dinglan.png");
	top->setAnchorPoint(Vec2(0.5,0));
	top->setScaleX((scal9->getContentSize().width-getDelwithint(10,1))/top->getContentSize().width);
	top->setPosition(Vec2(scal9->getPositionX(),scal9->getPositionY()+scal9->getContentSize().height/2-top->getContentSize().height-getDelwithint(5,1)));
	node->addChild(top);

	
	
	float backx=0.0f;
	float backy=0.0f;

	Size size=Size(getDelwithint(200,0),getDelwithint(350,1));

	for(int i=1;i<=4;i++){
	
		
		auto back=Sprite::create("ui_shangcheng_shangpin_bg_shuoming.png");

		//if(i==1){

			//backx=getDelwithint(25,1)+back->getContentSize().width/2+(i-1)*back->getContentSize().width;
		//}else{
		float spcae= (scal9->getContentSize().width-getDelwithint(50,0)-back->getContentSize().width*4)/3;

		backx=getDelwithint(25,0)+back->getContentSize().width/2+(i-1)*back->getContentSize().width+spcae*(i-1);

		//}

		back->setPosition(Vec2(backx,getDelwithint(90,1)+back->getContentSize().height/2));
		scal9->addChild(back);

		auto back1=Sprite::create("ui_shangcheng_shangpin_bg.png");
		back1->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height-back1->getContentSize().height/2));
		back->addChild(back1);

		auto Daoju=Sprite::create(StringUtils::format("ui_shangcheng_shangpin_jinbi_%d.png",i));
		Daoju->setPosition(Vec2(back1->getContentSize().width/2,back1->getContentSize().height/2));
		back1->addChild(Daoju);

		string str_zds="6000";
		string str_yh="";
		if(i==1){
			str_zds="6000";
		}else if(i==2){
			str_zds="12000";
			str_yh=FX::StringsMap::getValue("Zhengsong")+"6000"+FX::StringsMap::getValue("Gold");
		}else if(i==3){
			str_zds="";
			str_yh=FX::StringsMap::getValue("Yongjiutequan");
		}else if(i==4){
			str_zds="27000";
			str_yh=FX::StringsMap::getValue("Zhengsong")+"18000"+FX::StringsMap::getValue("Gold");
		}

		if(i!=3){
			str_zds+=FX::StringsMap::getValue("Gold");
		}
		auto Zhids=Label::createWithSystemFont(str_zds,"Arial",28);
		Zhids->setColor(Color3B(185,21,77));
		Zhids->setPosition(Vec2(Daoju->getContentSize().width/2,10));
		Daoju->addChild(Zhids);

		auto shuoming=Label::createWithSystemFont(str_yh,"Arial",24,Size(170,0),TextHAlignment::CENTER);
		shuoming->setColor(Color3B(185,21,77));
		shuoming->setPosition(Vec2( back->getContentSize().width/2,back->getContentSize().height*0.15));
		back->addChild(shuoming);


		auto button=MenuItemImage::create("ui_anniu_tongyong_goumai.png","ui_anniu_tongyong_goumai_select.png",CC_CALLBACK_1(ShopLayer::gold_Callback,this));
		button->setPosition(Vec2(back->getPositionX(),size.height-back->getContentSize().height-button->getContentSize().height/2));
		button->setUserData((void*)i);
		int yuan_m=(i)*2;
		if(i==4){
			yuan_m=9;
		}
		string yuan=__String::createWithFormat("%d",yuan_m)->getCString()+FX::StringsMap::getValue("yuan");
		auto buy=Label::createWithSystemFont(yuan,"Arial",30);
		buy->setPosition(Vec2(button->getContentSize().width/2,button->getContentSize().height/2));
		button->addChild(buy);

		auto menu = Menu::create(button,NULL);
		menu->setPosition(Vec2::ZERO);
		scal9->addChild(menu, 1);
	}
}

void ShopLayer::drowJiewel(Layer* node,Vec2 p){


	 
	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(p);
	scal9->setContentSize(Size(getDelwithint(25,0)*37.4,getDelwithint(21,1)*22.4));// 
	node->addChild(scal9);

	
	auto top=Sprite::create("ui_shangcheng_bg_dinglan.png");
	top->setAnchorPoint(Vec2(0.5,0));
	top->setScaleX((scal9->getContentSize().width-getDelwithint(10,1))/top->getContentSize().width);
	top->setPosition(Vec2(scal9->getPositionX(),scal9->getPositionY()+scal9->getContentSize().height/2-top->getContentSize().height-getDelwithint(5,1)));
	node->addChild(top);
    
	
	
	float backx=0.0f;
	float backy=0.0f;

	Size size=Size(getDelwithint(200,0),getDelwithint(350,1));

	for(int i=1;i<=4;i++){
	
		auto back=Sprite::create("ui_shangcheng_shangpin_bg_shuoming.png");
		float spcae= (scal9->getContentSize().width-getDelwithint(50,0)-back->getContentSize().width*4)/3;

		backx=getDelwithint(25,0)+back->getContentSize().width/2+(i-1)*back->getContentSize().width+spcae*(i-1);

		back->setPosition(Vec2(backx,getDelwithint(90,1)+back->getContentSize().height/2));
		scal9->addChild(back);

		auto back1=Sprite::create("ui_shangcheng_shangpin_bg.png");
		back1->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height-back1->getContentSize().height/2));
		back->addChild(back1);

		string str_zds="6000";
		string str_yh="";
		if(i==1){
			str_zds="20";
		}else if(i==2){
			str_zds="40";
			str_yh=FX::StringsMap::getValue("Zhengsong")+"10"+FX::StringsMap::getValue("Zs");
		}else if(i==3){
			str_zds="90";
			str_yh=FX::StringsMap::getValue("Zhengsong")+"50"+FX::StringsMap::getValue("Zs");
		}else if(i==4){
			str_zds="190";
			str_yh=FX::StringsMap::getValue("Zhengsong")+"190"+FX::StringsMap::getValue("Zs");
		}

		str_zds+=FX::StringsMap::getValue("Zs");
		auto Daoju=Sprite::create(StringUtils::format("ui_shangcheng_shangpin_zuanshi_%d.png",i));
		Daoju->setPosition(Vec2(back1->getContentSize().width/2,back1->getContentSize().height/2));
		back1->addChild(Daoju);

		auto Zhids=Label::createWithSystemFont(str_zds,"Arial",28);
		Zhids->setColor(Color3B(185,21,77));
		Zhids->setPosition(Vec2(Daoju->getContentSize().width/2,10));
		Daoju->addChild(Zhids);


		auto shuoming=Label::createWithSystemFont(str_yh,"Arial",24,Size(170,0),TextHAlignment::CENTER);
		shuoming->setColor(Color3B(185,21,77));
		shuoming->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height*0.15));
		back->addChild(shuoming);


		auto button=MenuItemImage::create("ui_anniu_tongyong_goumai.png","ui_anniu_tongyong_goumai_select.png",CC_CALLBACK_1(ShopLayer::jiewl_Callback,this));

		button->setPosition(Vec2(back->getPositionX(),size.height-back->getContentSize().height-button->getContentSize().height/2));
		button->setUserData((void*)i);

		int yuan_m=2;
		if(i<3){
			yuan_m=i*2;
		}else if(i==3){
			yuan_m=9;
		}else if(i==4){
			yuan_m=19;
		}

		string yuan=__String::createWithFormat("%d",yuan_m)->getCString()+FX::StringsMap::getValue("yuan");
		auto buy=Label::createWithSystemFont(yuan,"Arial",30);
		buy->setPosition(Vec2(button->getContentSize().width/2,button->getContentSize().height/2));
		button->addChild(buy);

		auto menu = Menu::create(button,NULL);
		menu->setPosition(Vec2::ZERO);
		scal9->addChild(menu, 1);
	}






}



void ShopLayer::drowSiwu(Layer* node,Vec2 p){

	 
	auto scal9=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9->setPosition(p);
	scal9->setContentSize(Size(getDelwithint(25,0)*37.4,getDelwithint(21,1)*22.4));// 
	node->addChild(scal9);
    
	float backx=0.0f;
	float backy=0.0f;

	Size size=Size(getDelwithint(200,0),getDelwithint(350,1));

	for(int i=1;i<=3;i++){
	
		auto back=Sprite::create("ui_zhuye_guanqiamoshi_guanqia_weijiesuo.png");
		float spcae= (scal9->getContentSize().width-getDelwithint(200,0)-back->getContentSize().width*3)/2;

		backx=getDelwithint(100,0)+back->getContentSize().width/2+(i-1)*back->getContentSize().width+spcae*(i-1);

		back->setPosition(Vec2(backx,getDelwithint(90,1)+back->getContentSize().height*0.7));
		scal9->addChild(back);

		string str_name=FX::StringsMap::getValue("ydtg");
		string str_zds="20";
		string str_yh="";
		if(i==1){
			str_zds="20";
			str_yh=FX::StringsMap::getValue("nh")+"1000"+FX::StringsMap::getValue("tg");
			str_name=FX::StringsMap::getValue("ydtg");
		}else if(i==2){
			str_zds="40";
			str_yh=FX::StringsMap::getValue("nh")+"1000"+FX::StringsMap::getValue("gd");
			str_name=FX::StringsMap::getValue("ydgd");
		}else if(i==3){
			str_zds="90";
			str_yh=FX::StringsMap::getValue("nh")+"1000"+FX::StringsMap::getValue("dg");
			str_name=FX::StringsMap::getValue("yddg");

		}
        
		auto l_name=Label::createWithSystemFont(str_name,"Arial",30);
		l_name->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height*0.9));
		back->addChild(l_name);


		auto sp_w=Sprite::create("ui_shangcheng_shangpin_jinbi_2.png");
		sp_w->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height*0.5));
		back->addChild(sp_w);



		
		auto shuoming=Label::createWithSystemFont(str_yh,"Arial",24,Size(170,0),TextHAlignment::CENTER);
		shuoming->setColor(Color3B(185,21,77));
		shuoming->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height*0.7));
		back->addChild(shuoming);

		//
		str_zds+=FX::StringsMap::getValue("Zs");
		auto zs_label=Label::createWithSystemFont(str_zds,"Arial",24);
		zs_label->setColor(Color3B(185,21,77));
		zs_label->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height*0.15));
		back->addChild(zs_label);

		auto button=MenuItemImage::create("ui_anniu_tongyong_goumai.png","ui_anniu_tongyong_goumai_select.png",CC_CALLBACK_1(ShopLayer::jiewl_Callback,this));
		button->setPosition(Vec2(back->getPositionX(),scal9->getContentSize().height*0.125));
		button->setUserData((void*)i);

		int yuan_m=2;
		if(i<3){
			yuan_m=i*2;
		}else if(i==3){
			yuan_m=9;
		}else if(i==4){
			yuan_m=19;
		}
        
		string yuan=__String::createWithFormat("%d",yuan_m)->getCString()+FX::StringsMap::getValue("yuan");
		auto buy=Label::createWithSystemFont(yuan,"Arial",30);
		buy->setPosition(Vec2(button->getContentSize().width/2,button->getContentSize().height/2));
		button->addChild(buy);

		auto menu = Menu::create(button,NULL);
		menu->setPosition(Vec2::ZERO);
		scal9->addChild(menu, 1);
	}
	 

}





void ShopLayer::returnPa(Ref* sender){

	this->getParent()->removeFromParent();

}



void ShopLayer::gold_Callback(Ref* sender){

	 


}


void ShopLayer::jiewl_Callback(Ref* sender){

	 
	  

}


void ShopLayer::buttonSw(Ref* ref){

	Vec2 p;
	int m=nodes;
	Size visibaleSize = Size(getDelwithint(930,0),getDelwithint(470,1));
	if(issw==false){
		p=Vec2(-visibaleSize.width*2,0);
		m=nodes;
	}else{
		p=Vec2(0,0);
		m=1;
	}
	scrollView->setContentOffset(p,true);
	setWeizhi(m);
	m_curPage=m;
}


void ShopLayer::buttonMy(Ref* ref){


	Vec2 p;
	int m=nodes;
	Size visibaleSize = Size(getDelwithint(930,0),getDelwithint(470,1));
	if(issw==false){
		p=Vec2(0,0);
		m=1;
	}else{
		p=Vec2(-visibaleSize.width*2,0);
		m=nodes;
	}
	scrollView->setContentOffset(p,true);
	setWeizhi(m);
	m_curPage=m;
}

 void ShopLayer::setWeizhi(int wz){

	 	for(int i=1;i<=nodes;i++){
			 if(i==wz){
					    sp_vec.at(i-1)->setTexture(Sprite::create("ui_shangcheng_fenye_select.png")->getTexture());
		     }else{
					    sp_vec.at(i-1)->setTexture(Sprite::create("ui_shangcheng_fenye.png")->getTexture());
			 }
	    }

 }

 void ShopLayer::setGold(){
	 
	 drawMoney(0,shop_moneys);
	 setChanged(0);	
 }
 void ShopLayer::setJiewl(){

	 drawMoney(0,shop_moneys);
	 setChanged(0);	
 }

 void ShopLayer::setSw(){
	 
	 drawMoney(1,shop_foods);
     setChanged(1);
 }


void ShopLayer::setTextb(MenuItemImage* node,string str,int fontsize,Color3B b){
	 
	auto label=Label::createWithSystemFont(FX::StringsMap::getValue(str),"Arial",fontsize);
	label->setPosition(Vec2(node->getContentSize().width/2,node->getContentSize().height/2));
	label->setColor(b);
	node->addChild(label);

}


void ShopLayer::fillOuty(MenuItemImage* p,string imgfile){
	 
	auto s=Sprite::create(imgfile);
	s->setScale(1.05f);
	p->setSelectedImage(s);
}
