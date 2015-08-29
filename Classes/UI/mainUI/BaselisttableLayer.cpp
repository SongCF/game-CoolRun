#include "BaselisttableLayer.h"
#include "StringsMap.h"
#include "PlayerInfoHelper.h"
#include "GameDirector.h"
#include "ActiveValueHelper.h"
#include "BasicLayer.h"


#define daojuw_img "UI_zhuye_shangdian_shangpin_bg_wu.png" 
#define daojuy_img "UI_zhuye_shangdian_shangpin_bg_yiyou.png" 
#define daojubuy_img "UI_zhuye_shangdian_anniu_goumai.png"
#define daojushuzi_img "UI_zhuye_tishi_shangpinshuliang.png" 
#define daoju_gold_img  "UI_zhuye_shangdian_jinbi.png"
#define daoju_jiewel_img "UI_zhuye_shangdian_zuanshi.png" 



bool BaselisttableLayer::init(){

	if(!Layer::init()){
		return false;
	}
	upname=0;
    RankMsg();
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	return true;
}
void BaselisttableLayer::onEnter(){

	
	Layer::onEnter();
	listentag=0;
	auto callBack       =  [&](EventCustom* evt)  
	{  
		this->toolsdata(evt);
	};  
	buy_lr=EventListenerCustom::create(MSG_MarketBuyGoods_Rsp,callBack);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(buy_lr,1);  
	
}
void BaselisttableLayer::onExit(){
    
    
	Director::getInstance()->getEventDispatcher()->removeEventListener(buy_lr);
	Layer::onExit();
}


void BaselisttableLayer::aciveChange(int id){


	
	if(ac_button.at(id)->getParent()!=nullptr&&ac_button.at(id)!=nullptr){
	   ac_button.at(id)->removeFromParent();
	}
	

}

BaselisttableLayer::BaselisttableLayer(void)
{


	
	
	
	
	
	
}


BaselisttableLayer::~BaselisttableLayer(void)
{

	
}

void BaselisttableLayer::tableCellTouched(TableView* table, TableViewCell* cell){



	


	if(type==Daoju_list){
		
	}else if(type==Chengjiu_list){
		
	}else if(type==Shangchen_list){
		
	}else if(type==Juese_list){
	
		
		int ID=(int)(long long)(cell->getUserData());
		
		if(ActorHelper::getHelper()->getIsUnlockItem(ID)==true){

			 //  
			hero_back.at(ID)->stopAllActions();
			if(hero_back.at(ID)->getChildByTag(100)!=nullptr){
			    hero_back.at(ID)->removeChildByTag(100);
			}
			
			if(ID!=hero_ID){
				
				hero_back.at(hero_ID)->setTexture(Sprite::create("ui_zhuye_jiaoseshengji_bg.png")->getTexture());
				hero_select.at(hero_ID)->setVisible(false);
			
				playAnimationByname(hero_back.at(hero_ID),"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","weixuanzhong_xiaoguo",true,false);
			}
			hero_ID=ID;

			PlayerInfoHelper::getHelper()->setDeafaultHero(ID);
			int m=0;
			
			
		    hero_back.at(ID)->setTexture(Sprite::create("ui_zhuye_jiaoseshengji_bg_select.png")->getTexture());
		    hero_select.at(ID)->setVisible(true);
			//hero_select.at(ID)->runAction(Sequence::create(ScaleTo::create(0.1f,0.5f),ScaleTo::create(0.1f,1.0f),nullptr));
     		playAnimationByname(hero_back.at(ID),"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","xuanzhongxiaoguo",false,false);
			
		}else{

			 BaseLayer* shop=BaseLayer::create();
	         shop->goShop(100,ID); //领取角色
	         shop->Shieldtouched();
	         Director::getInstance()->getRunningScene()->addChild(shop);

			 
		

	   }

		

		
	}else if(type==Chongwu_list){
		int ID=(int)(long long)cell->getUserData();
		

		if(ActorHelper::getHelper()->getIsUnlockItem(ID)==true){

			 //  
	        
			pet_back.at(ID)->stopAllActions();
			pet_back.at(ID)->removeChildByTag(100);
		    
			if(ID!=pet_ID){
			  pet_back.at(pet_ID)->setTexture(Sprite::create("ui_zhuye_jiaoseshengji_bg.png")->getTexture());
			  pet_select.at(pet_ID)->setVisible(false);
			  playAnimationByname(pet_back.at(pet_ID),"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","weixuanzhong_xiaoguo",true,false);
			}
			pet_ID=ID;
			PlayerInfoHelper::getHelper()->setDeafaultPet(ID);
			pet_back.at(ID)->setTexture(Sprite::create("ui_zhuye_jiaoseshengji_bg_select.png")->getTexture());
			pet_select.at(ID)->setVisible(true);
			playAnimationByname(pet_back.at(ID),"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","xuanzhongxiaoguo",false,false);

		}else{
			 
			 pet_seletid=ID;
			 BaseLayer* shop=BaseLayer::create();
	         shop->goShop(101,ID); //领取宠物
	         shop->Shieldtouched();
	         Director::getInstance()->getRunningScene()->addChild(shop);

	   }
		
	}else if(type==Gongju_list){ //魔法物
		

		int ID=(int)(long long)cell->getUserData();
		
		if(GoodsHelper::getHelper()->getOneItem(ID)!=nullptr){

			 //  
	       
			magic_back.at(ID)->stopAllActions();
			magic_back.at(ID)->removeChildByTag(100);
		
			
			if(ID!=magic_ID){
			magic_back.at(magic_ID)->setTexture(Sprite::create("ui_zhuye_jiaoseshengji_bg.png")->getTexture());
			magic_select.at(magic_ID)->setVisible(false);
			playAnimationByname(magic_back.at(magic_ID),"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","weixuanzhong_xiaoguo",true,false);
			}
			magic_ID=ID;
			PlayerInfoHelper::getHelper()->setDeafaultMagicItem(ID);


		
			magic_back.at(ID)->setTexture(Sprite::create("ui_zhuye_jiaoseshengji_bg_select.png")->getTexture());
			magic_select.at(ID)->setVisible(true);
			//magic_select.at(ID)->runAction(Sequence::create(ScaleTo::create(0.1f,0.5f),ScaleTo::create(0.1f,1.0f),nullptr));

			playAnimationByname(magic_back.at(ID),"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","xuanzhongxiaoguo",false,false);

		}else{
			
			 magic_selectd=ID;
			 BaseLayer* shop=BaseLayer::create();
	         shop->goShop(102,ID); //领取魔法物
	         shop->Shieldtouched();
	         Director::getInstance()->getRunningScene()->addChild(shop);
			
	   }
	}else if(type==Toll_list){

		/*
		int idx=(int)(long long)cell->getUserData();

	
		
	
		 //未解锁
		if(TollgateHelper::getHelper()->isCanStart(idx)==false){
		  
			 BaseLayer* layer=BaseLayer::create();
			 layer->Shieldtouched();
			 layer->goShop(103,0);
			 Director::getInstance()->getRunningScene()->addChild(layer);
		
		 //已解锁 未通关
		 }else if((TollgateHelper::getHelper()->getOneTollgate(idx)->unlockNeedAchiveID==0&&TollgateHelper::getHelper()->isCustoms(idx)==false)||(TollgateHelper::getHelper()->isCanStart(idx)==true&&TollgateHelper::getHelper()->isCustoms(idx)==false)){

			 if(GoodsHelper::getHelper()->getGoldNum()>=TollgateHelper::getHelper()->getOneTollgate(idx)->playNeedUnitCount){

				 TollgateHelper::getHelper()->goToTollgate(idx);
				 GameDirector::getDirector()->loadingToRunningScene(RunningMode::tollgate,idx);

			 }else{

				  BaseLayer* layer=BaseLayer::create();
			      layer->Shieldtouched();
			      layer->goShop(98,0);
			      Director::getInstance()->getRunningScene()->addChild(layer);

			 }
         
		 //已结锁 &&已通关 
		 }else if(TollgateHelper::getHelper()->isCustoms(idx)==true&&TollgateHelper::getHelper()->isCanStart(idx)==true){
			   TollgateHelper::getHelper()->goToTollgate(idx);
			   GameDirector::getDirector()->loadingToRunningScene(RunningMode::tollgate,idx);
		 }
		 */
	}

}

Size BaselisttableLayer::tableCellSizeForIndex(TableView *table, ssize_t idx){

	if(this->isVetical==false){
	  return Size(size.width+layerspace,size.height);
	}else{
	  return Size(size.width,size.height+layerspace);
	}

}


TableViewCell*  BaselisttableLayer::tableCellAtIndex(TableView *table, ssize_t idx){

	
	TableViewCell *cell = table->dequeueCell();
    if (cell != nullptr) 
	{
		cell->removeAllChildren();
	}
	else			  
	{
		cell = new TableViewCell();
		cell->autorelease();
	}

	//根据type生成不同界面
	addDataimg(cell,idx);
	return cell;

}


ssize_t BaselisttableLayer::numberOfCellsInTableView(TableView *table){

    //不同type不同 数量
	int num=0;

	if(type==Daoju_list){
		num=tools.size();
	}else if(type==Chengjiu_list){
		num=acives.size();
	}else if(type==Shangchen_list){
		num=sales.size();
	}else if(type==Juese_list){
		num=heros.size();
	}else if(type==Chongwu_list){
		num=pets.size();

	}else if(type==Gongju_list){ //魔法物
		num=magis.size();
	  
	}else if(type==Toll_list){

		//num=tollgate.size(); 
	}else if(type==Sports_list){
		num=netSports.size();
	}else if(type==Msgnet_list){
		num=eamils.size();
	}
	
	return num;
}


void BaselisttableLayer::setDataType(int type){

	
    this->type=type;

	if(type==Daoju_list){
		vector<SaleItem> tls=(vector<SaleItem>)*MarketHelper::getHelper()->getAllSaleItems();
		//只留道具
		int m_size=tls.size();
		for(int i=0;i<m_size;i++){
			const Unit *unitScale = GoodsHelper::getHelper()->getUnitByID(tls[i].unitSaleID);
			if(unitScale->type==unittype_Item||(unitScale->type==unittype_Special&&tls[i].unitSaleID==unitID_RandomItem)){
				tools.push_back(tls[i]);
			}
		}
		int m=tools.size();
		CCLOG("daoju price---->%d",m);
	}else if(type==Chengjiu_list){
		acives=(vector<Achieve>)*AchievementHelper::getHelper()->getAllAchievement();
	}else if(type==Shangchen_list){
		sales=(vector<SaleItem>)*MarketHelper::getHelper()->getAllSaleItems();
	}else if(type==Juese_list){
		//heros=ActorHelper::getHelper()->getAllHero();
		vector<SaleItem> tls=(vector<SaleItem>)*MarketHelper::getHelper()->getAllSaleItems();
		//只留道具
		int m_size=tls.size();
		for(int i=0;i<m_size;i++){
			const Unit *unitScale = GoodsHelper::getHelper()->getUnitByID(tls[i].unitSaleID);
			if(unitScale->type==unittype_Actor){
				heros.push_back(tls[i]);
			}
		}
	}else if(type==Chongwu_list){
		
		vector<SaleItem> tls=(vector<SaleItem>)*MarketHelper::getHelper()->getAllSaleItems();
		//只留魔法物
		int m_size=tls.size();
		for(int i=0;i<m_size;i++){
			const Unit *unitScale = GoodsHelper::getHelper()->getUnitByID(tls[i].unitSaleID);
			if(unitScale->type==unittype_Pet){
				pets.push_back(tls[i]);
			}
		}

	}else if(type==Gongju_list){ //魔法物
	
		vector<SaleItem> tls=(vector<SaleItem>)*MarketHelper::getHelper()->getAllSaleItems();
		//只留魔法物
		int m_size=tls.size();
		for(int i=0;i<m_size;i++){
			const Unit *unitScale = GoodsHelper::getHelper()->getUnitByID(tls[i].unitSaleID);
			if(unitScale->type==unittype_Magic){
				magis.push_back(tls[i]);
			}
		}
	}else if(type==Toll_list){

		//tollgate=*TollgateHelper::getHelper()->getAllTollgate();

	}else if(type==Sports_list){

		NetmsgManger* manager=NetmsgManger::getNetManager();

		this->netSports.clear();
        if(manager->netSports.size()>0){					 
				for(size_t i=0;i<manager->netSports.size();i++){
						this->netSports.push_back(manager->netSports.at(i));
				 }
		 }
	}else if(type==Msgnet_list){

		eamils=EmailHelper::getHeper()->getAllEmailList(30);
		CCLOG("------>%d",eamils.size());
	}


}

void BaselisttableLayer::RankMsg(){
	  
	  


}

void BaselisttableLayer::setMtabletype(int type,Size size,int itemnum,ScrollView::Direction direction,TableView::VerticalFillOrder order,int space){


	//list->setMtabletype(Daoju_list,Size(499,105),3,cocos2d::extension::ScrollView::Direction::VERTICAL,TableView::VerticalFillOrder::TOP_DOWN,0);

	this->size=size;
	this->itemnum=itemnum;
	this->layerspace=space;
	this->setDataType(type);
	
	Size layersize;
	if(direction==ScrollView::Direction::VERTICAL){
		this->isVetical=true;
		layersize.width=size.width;
		layersize.height=size.height*itemnum+space*itemnum;
	}else{
		this->isVetical=false;
		layersize.width=size.width*itemnum+space*itemnum;
		layersize.height=size.height;
	}
	this->setContentSize(layersize);
	
    mTableView = TableView::create(this,layersize);
	this->addChild(mTableView,2);
	mTableView->setDirection(direction);
	mTableView->setPosition(Vec2(0,getDelwithint(0,0)));
	mTableView->setDelegate(this);
	mTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	
	
	

}


void BaselisttableLayer::addDataimg(TableViewCell *cell, ssize_t idx){



	/*
	auto board =Sprite::create("shangdian_shangpin_bg.png");
	cell->addChild(board,0);
	board->setPosition(Vec2(board->getContentSize().width/2, board->getContentSize().height/2));
	*/
	if(type==Daoju_list){
    
		drow_Toolist(cell,idx);
	}else if(type==Chengjiu_list){
		drow_Acivelist(cell,idx);
	}else if(type==Shangchen_list){
		drow_Shoplist(cell,idx);
	}else if(type==Juese_list){
	    drow_rolelist(cell,idx);
	}else if(type==Chongwu_list){
        drow_chongwulist(cell,idx);
	}else if(type==Gongju_list){ //魔法物
	    drow_gongjugwulist(cell,idx);
	}else if(type==Toll_list){
		drow_tolllist(cell,idx);
	}else if(type==Sports_list){
		drow_Sportslist(cell,idx);
	}else if(type==Msgnet_list){
		drow_msg(cell,idx);
	}

}


//购买道具
void BaselisttableLayer::toolsale_Callback(Ref* sender){


	listentag=1;
	FX::Music::getInstance()->playEffectMusic("u_buy_pet");
	MenuItemImage* p=(MenuItemImage*)sender;
	p->runAction(Sequence::create(ScaleTo::create(0.05f,1.2f),ScaleTo::create(0.01f,1.0f),
		nullptr));
	int ID=(int)(long long)p->getUserData();
	cp_lable=p->getTag();
	sale_id=ID;
	MarketHelper::getHelper()->buyItem(ID);	
	
}

void BaselisttableLayer::spcall(Node* node,bool mr){
	 
	
}


void BaselisttableLayer::toolsdata(EventCustom* evt){



	MsgData* data=(MsgData*)evt->getUserData();
	int tag_id=data->value;

	if(data->errorCode==Rsp_Success){
		if(Director::getInstance()->getRunningScene()->getChildByTag(1000)!=nullptr)
		    Director::getInstance()->getRunningScene()->removeChildByTag(1000);
	}

	if(type==Daoju_list&&listentag==1){
			if(data->errorCode==Rsp_Success){
				 
				    
					 BagItem* tooldescriple=(BagItem*)GoodsHelper::getHelper()->getOneItem(cp_lable);
					 const SaleItem* item=MarketHelper::getHelper()->getSaleItemByID(sale_id);
					 int amount=0;
					 if(tooldescriple!=nullptr){
						 amount=tooldescriple->value;
					 }
					 string strnum=String::createWithFormat("%u",amount)->getCString();
					 if(p_label.at(cp_lable)!=nullptr){
						 if(amount==1){
							 p_labelsp.at(cp_lable)->setVisible(true);
						 }
						p_label.at(cp_lable)->setString(strnum.c_str());
						if(amount==1)
						   p_label.at(cp_lable)->setColor(Color3B(255,255,255));
					 }
					 string b_name=p_chagesp.at(cp_lable)->getName().c_str();
					 if(amount==1){
						 const Unit *unitScale = GoodsHelper::getHelper()->getUnitByID(item->unitSaleID);
						 auto text1=Label::createWithSystemFont(unitScale->descrigbe,"minijiankatong.ttf", 22);
						 text1->setColor(Color3B(114,9,20));
						 text1->setAnchorPoint(Vec2(0.0f,0.5f));
						 text1->setPosition(Vec2(p_chagesp.at(cp_lable)->getContentSize().width*0.25,p_chagesp.at(cp_lable)->getContentSize().height*0.35f-1));
						 p_chagesp.at(cp_lable)->addChild(text1);
						 p_chagesp.at(cp_lable)->setTexture(Sprite::create("UI_zhuye_shangdian_shangpin_bg_yiyou.png")->getTexture());
						 p_clabel.at(cp_lable)->setColor(Color3B(114,9,20));
					 }

					 if(cp_lable==unitID_RandomItem){
						  EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
					      EventCustom event(Randxiaoguo);
	                      MsgData data;
						  data.value =tag_id;
	                      event.setUserData(&data);
	                      eventDspc->dispatchEvent(&event);
					 }
				 //增加活跃值
				 ActiveValueHelper::getHelper()->addActiveByType(activeType_GouMaiDaoJu);
			}else if(data->errorCode==Rsp_Gold_UnEnough){
				 //调转到商店页面
				 BaseLayer* shop=BaseLayer::create();
				 shop->money_no(unitID_Gold);
	             Director::getInstance()->getRunningScene()->addChild(shop);
			}else if(data->errorCode==Rsp_Jewel_UnEnough){
				//调转到商店页面
				 BaseLayer* shop=BaseLayer::create();
				 shop->money_no(unitID_Diamond);
	             Director::getInstance()->getRunningScene()->addChild(shop);
			}else if(data->errorCode==Rsp_Need_UnEnough){

			}else if(data->errorCode==Rsp_DontHaveThing){
				//没有该物品
			}
		
		listentag=0;

	}else if(type==Chengjiu_list){
		
	}else if(type==Shangchen_list){
		
	}else if(type==Juese_list){

		if(data->errorCode==Rsp_Success){

			this->mTableView->reloadData();

	    }else if(data->errorCode==Rsp_Gold_UnEnough){
				//调转到商店页面

			     
				 BaseLayer* shop=BaseLayer::create();
	             shop->goShop(98);
	             shop->Shieldtouched();
	             Director::getInstance()->getRunningScene()->addChild(shop);
				 

			}else if(data->errorCode==Rsp_Jewel_UnEnough){
				//调转到商店页面

				 
				 BaseLayer* shop=BaseLayer::create();
	             shop->goShop(99);
	             shop->Shieldtouched();
	             Director::getInstance()->getRunningScene()->addChild(shop);
				 

			}else if(data->errorCode==Rsp_Need_UnEnough){


	    }else if(data->errorCode==Rsp_DontHaveThing){
				//没有该物品

		}

	}else if(type==Chongwu_list){

		if(data->errorCode==Rsp_Success){

			 int ID=pet_seletid; //(int)(long long)pet_button->getUserData();
			 int num=0;
			 vector<BagItem> bag=GoodsHelper::getHelper()->getAllGoods();
			 for(size_t i=0;i<bag.size();i++){
				 
				 if (bag.at(i).id >= ID_Pet_Start && bag.at(i).id <= ID_Pet_End)
				 {
					 num=num+1;
				 }
			 }
			 if(num==1){
				 pet_ID=ID;
				 PlayerInfoHelper::getHelper()->setDeafaultPet(ID);
				 pet_back.at(pet_ID)->setTexture(Sprite::create("ui_zhuye_jiaoseshengji_bg_select.png")->getTexture());
				 pet_select.at(pet_ID)->setVisible(true);
				 playAnimationByname(pet_back.at(pet_ID),"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","xuanzhongxiaoguo",false,false);
				// pet_select.at(ID)->runAction(Sequence::create(ScaleTo::create(0.1f,0.5f),ScaleTo::create(0.1f,1.0f),nullptr));
				 petmoney_back.at(ID)->setVisible(false);
				 pet_button=button_back.at(ID);

				 pet_button->removeChildByTag(10);
				
				 string isstr="ui_anniu_tongyong_zhuye.png";
	             string isstr1="ui_anniu_tongyong_zhuye.png";

				 pet_button->setVisible(true);
				 pet_button->setNormalImage(Sprite::create(isstr));
				 pet_button->setSelectedImage(Sprite::create(isstr1));
				 pet_button->setCallback( CC_CALLBACK_1(BaselisttableLayer::shengji_Callback,this));
				 auto buylabel=Label::createWithSystemFont(FX::StringsMap::getValue("level1"),"Arial",24);
				 buylabel->setPosition(Vec2(pet_button->getContentSize().width/2,pet_button->getContentSize().height/2));
				 pet_button->addChild(buylabel);
	         }else{

				 pet_button=button_back.at(ID);
				 petmoney_back.at(ID)->setVisible(false);
				 pet_button->setVisible(true);

				 pet_button->removeChildByTag(10);
				 string isstr="ui_anniu_tongyong_zhuye.png";
	             string isstr1="ui_anniu_tongyong_zhuye.png";
				 pet_button->setNormalImage(Sprite::create(isstr));
				 pet_button->setSelectedImage(Sprite::create(isstr1));
				 pet_button->setCallback( CC_CALLBACK_1(BaselisttableLayer::shengji_Callback,this));
				 auto buylabel=Label::createWithSystemFont(FX::StringsMap::getValue("level1"),"Arial",24);
				 buylabel->setPosition(Vec2(pet_button->getContentSize().width/2,pet_button->getContentSize().height/2));
				 pet_button->addChild(buylabel);
	         }
			 
	    }else if(data->errorCode==Rsp_Gold_UnEnough){
				//调转到商店页面
			     CCLOG("tianxia--->");
				 
				 BaseLayer* shop=BaseLayer::create();
	             shop->goShop(98);
	             shop->Shieldtouched();
	             Director::getInstance()->getRunningScene()->addChild(shop);
				 

		}else if(data->errorCode==Rsp_Jewel_UnEnough){
				//调转到商店页面
			 CCLOG("tianxia1--->");
			 
				 BaseLayer* shop=BaseLayer::create();
	             shop->goShop(99);
	             shop->Shieldtouched();
	             Director::getInstance()->getRunningScene()->addChild(shop);
				 
		}else if(data->errorCode==Rsp_Need_UnEnough){


	    }else if(data->errorCode==Rsp_DontHaveThing){
				//没有该物品

		}
       
	}else if(type==Gongju_list){ //魔法物
	      
		 if(data->errorCode==Rsp_Success){
				

			 int ID=magic_selectd; 
			 if(GoodsHelper::getHelper()->getAllMagicItems().size()==1){

		     int iCount = GoodsHelper::getHelper()->getAllMagicItems().size();
			 if(iCount==1){

				 
				 magic_ID=ID;
		         PlayerInfoHelper::getHelper()->setDeafaultMagicItem(ID);
				 magic_back.at(magic_ID)->setTexture(Sprite::create("ui_zhuye_jiaoseshengji_bg_select.png")->getTexture());
				 magic_select.at(magic_ID)->setVisible(true);
				 playAnimationByname(magic_back.at(magic_ID),"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","xuanzhongxiaoguo",false,false);
				 //magic_select.at(ID)->runAction(Sequence::create(ScaleTo::create(0.1f,0.5f),ScaleTo::create(0.1f,1.0f),nullptr));

	         }
			 pbutton=mg_back.at(ID);
			 pbutton->setVisible(false);
			 magicmoney_back.at(ID)->setVisible(false);

		 }else if(data->errorCode==Rsp_Gold_UnEnough){
				//调转到商店页面
				 BaseLayer* shop=BaseLayer::create();
	             shop->goShop(98);
	             shop->Shieldtouched();
	             Director::getInstance()->getRunningScene()->addChild(shop);


			}else if(data->errorCode==Rsp_Jewel_UnEnough){
				//调转到商店页面
				 BaseLayer* shop=BaseLayer::create();
	             shop->goShop(99);
	             shop->Shieldtouched();
	             Director::getInstance()->getRunningScene()->addChild(shop);
			}else if(data->errorCode==Rsp_Need_UnEnough){


		 }else if(data->errorCode==Rsp_DontHaveThing){
				//没有该物品

		}


	}
}

}

//成就
void BaselisttableLayer::achive_Callback(Ref* sender){



}

//商城
void BaselisttableLayer::shop_Callback(Ref* sender){



}



void BaselisttableLayer::juese_Callback(Ref* sender){

	

	FX::Music::getInstance()->playEffectMusic("u_level_up");
	MenuItemImage* p=(MenuItemImage*)sender;
	int id=(int)(long long)p->getUserData();

	EventCustom event(Layerchange);

	LayerData data;
	data.current=2;
	data.push=1;
	data.heroid=id;
    
	event.setUserData(&data);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	
}

//角色购买
void BaselisttableLayer::juese_Callback1(Ref* sender){

	MenuItemImage* p=(MenuItemImage*)sender;
	hero_button=p;
	int ID=(int)(long long)p->getUserData();

	MarketHelper::getHelper()->buyItem(ID);

}

void BaselisttableLayer::buy_Callback(Ref* sender){

	MenuItemImage* p=(MenuItemImage*)sender;
	pet_button=p;
	int ID=(int)(long long)p->getUserData();

	
	MarketHelper::getHelper()->buyItem(ID);


}

void BaselisttableLayer::shengji_Callback(Ref* sender){


	FX::Music::getInstance()->playEffectMusic("u_level_up");
	MenuItemImage* p=(MenuItemImage*)sender;
	int id=(int)(long long)p->getUserData();

	
	EventCustom event(Layerchange);

	LayerData data;
	data.current=4;
	data.push=3;
	data.petid=id;
    
	event.setUserData(&data);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);


}

void BaselisttableLayer::gongju_Callback(Ref* sender){


	FX::Music::getInstance()->playEffectMusic("u_buy_pet");
	MenuItemImage* p=(MenuItemImage*)sender;
	pbutton=p;
	int ID=(int)(long long)p->getUserData();

	
	MarketHelper::getHelper()->buyItem(ID);


}

//道具
void BaselisttableLayer::drow_Toolist(TableViewCell *cell, ssize_t idx){


     
	 
	 const Unit *unitScale = GoodsHelper::getHelper()->getUnitByID(tools.at(idx).unitSaleID);
	 const Unit *unitPay = GoodsHelper::getHelper()->getUnitByID(tools.at(idx).unitPayID);
	 BagItem* tooldescriple=(BagItem*)GoodsHelper::getHelper()->getOneItem(tools.at(idx).unitSaleID);
	 int amount=0;
	 if(tooldescriple!=nullptr){
	    amount=tooldescriple->value;
		if(amount>=100){
			amount=99;
		}
	 }

	 string tiao_back=daojuy_img;
	 string shuziw="UI_shuzi_02.png";
	 Color3B colornumber=Color3B(114,9,20);
	 
	 if(amount==0){
		 tiao_back=daojuw_img;
		 colornumber=Color3B(114,9,20);
		 
		 shuziw="UI_shuzi_01.png";
	 }

     auto board =Sprite::create(tiao_back);
	 cell->addChild(board,1);
	 board->setPosition(Vec2(size.width/2, size.height/2));


	 auto imgstr=ImageHelper::getUnitSpriteUIScene(tools.at(idx).unitSaleID);
	 string textstr=unitScale->descrigbe;
	 string typeimgstr=unitPay->imageFile;
	

	 int moneynum=tools.at(idx).payPrice;
	 //道具图片tools.at(idx);
	 auto img= imgstr;
	 img->setPosition(Vec2(img->getContentSize().width*0.5,board->getContentSize().height/2));
	 board->addChild(img);
	 //道具数量
	     auto amountimg=Sprite::create(daojushuzi_img);
		 amountimg->setPosition(Vec2(board->getContentSize().width*0.25,board->getContentSize().height-amountimg->getContentSize().height*0.3));
		 //img->addChild(amountimg);
		 board->addChild(amountimg);

		 
		 auto numberlabel=Label::createWithSystemFont(String::createWithFormat("%d",amount)->getCString(),"Arial",24);
		 numberlabel->setPosition(Vec2(amountimg->getContentSize().width/2,amountimg->getContentSize().height/2));
		 amountimg->addChild(numberlabel);

		 if(amount==0){
			 amountimg->setVisible(false);
		 }

		 int m=tools.at(idx).unitSaleID;
		 p_label.insert(m,numberlabel);
	     p_labelsp.insert(m,amountimg);
		 p_chagesp.insert(m,board);

	 //道具所需类型的数量
	 auto typetext=Label::createWithSystemFont(__String::createWithFormat("%d",moneynum)->getCString(), "minijiankatong.ttf",26);
	 typetext->setAnchorPoint(Vec2(0,0.5));
	 typetext->setPosition(Vec2(board->getContentSize().width*0.305,board->getContentSize().height*0.68));
	 typetext->setColor(colornumber);
	 board->addChild(typetext);

	 //道具所需类型图片
	 
	 if(tools[idx].unitPayID==UnitID::unitID_Gold){
		  typeimgstr=daoju_gold_img;
	 }else if(tools[idx].unitPayID==UnitID::unitID_Diamond){
		  typeimgstr=daoju_jiewel_img;
	 }

	 auto imgtype=BaseSprite::create(typeimgstr);
	 imgtype->setScale(0.7f);
	 imgtype->setPosition(Vec2(typetext->getPositionX()+typetext->getContentSize().width+imgtype->getContentSize().width/2,board->getContentSize().height*0.65));
	 board->addChild(imgtype);
	
	

     if(amount>0){

		 auto text1= Label::createWithSystemFont((textstr).c_str(), "minijiankatong.ttf",22);
		 text1->setColor(Color3B(184,69,13));
		 text1->setAnchorPoint(Vec2(0.0f,0.5f));
		 text1->setPosition(Vec2(board->getContentSize().width*0.25,board->getContentSize().height*0.35f-1));
		 board->addChild(text1,1025);
	 }
	 //道具说明
	 auto text=Label::createWithSystemFont((textstr).c_str(), "minijiankatong.ttf",22);
	 text->setColor(colornumber);
	 text->setAnchorPoint(Vec2(0.0f,0.5f));
	 text->setPosition(Vec2(board->getContentSize().width*0.25,board->getContentSize().height*0.35f));
	 board->addChild(text,1025);
	 p_clabel.insert(m,text);
	 
	 
	 auto button=MenuItemImage::create(daojubuy_img,daojubuy_img,CC_CALLBACK_1(BaselisttableLayer::toolsale_Callback,this));
	 button->setPosition(Vec2(board->getContentSize().width-button->getContentSize().width/2,board->getContentSize().height*0.5));
	 button->setUserData((void*)tools.at(idx).saleID);
	 button->setTag(tools.at(idx).unitSaleID);
     
	 auto menu = Menu::create(button,nullptr);
     menu->setPosition(Vec2::ZERO);
	 board->addChild(menu, 1);
	 
}

//成就
void BaselisttableLayer::drow_Acivelist(TableViewCell *cell, ssize_t idx){

	 
	 string iswc="ui_chengjiu_wancheng.png";

	 bool isfinish=false;
	
	 if(AchievementHelper::getHelper()->getAchieveByID(acives.at(idx).ID)->isOver)
	 {
		 isfinish=true;
		
	 }else{
		 iswc="ui_chengjiu_weiwancheng.png";
	 }



	 auto board =Sprite::create(iswc.c_str());
	 board->setPosition(Vec2(size.width/2, size.height/2));
	 cell->addChild(board);

	
	 

	      MenuItemImage* wc=MenuItemImage::create("ui_anniu_tongyong_zhuye.png","ui_anniu_tongyong_zhuye.png",CC_CALLBACK_1(BaselisttableLayer::linqu_Callback,this));
		  wc->setUserData((void*)acives.at(idx).ID);


		  Sprite* set=Sprite::create("ui_anniu_tongyong_zhuye.png");
		  set->setColor(Colorzdy);
		  wc->setSelectedImage(set);


		  //wc->setVisible(false);
		  wc->setLocalZOrder(100);
		  wc->setPosition(Vec2(board->getContentSize().width-wc->getContentSize().width/2-getDelwithint(15,1),board->getContentSize().height-board->getContentSize().height/2));
		  auto lq=Label::createWithSystemFont(FX::StringsMap::getValue("linqu"),"Arial",30);
		  lq->setPosition(wc->getContentSize().width/2,wc->getContentSize().height/2);
		  wc->addChild(lq);
	     
	

		  if( AchievementHelper::getHelper()->getAchieveByID(acives.at(idx).ID)->isOver&&AchievementHelper::getHelper()->getIsGotAchievement(acives.at(idx).ID)==false){


	        Menu* menu = Menu::create(wc,nullptr);
            menu->setPosition(Vec2::ZERO);
		    board->addChild(menu, 10);
		    ac_button.insert(acives.at(idx).ID,menu);
		    // wc->setVisible(true);
			 /*
			 auto wc_ts=Sprite::create("ui_chengjiu_wancheng_biaoshi.png"); 
			 wc_ts->setPosition(wc->getPosition());
			 board->addChild(wc_ts);
			 */
	      }else{

			  /*
			 auto wc_ts=Sprite::create("ui_chengjiu_weiwancheng_biaoshi.png"); 
			 wc_ts->setPosition(wc->getPosition());
			 board->addChild(wc_ts);
			 */
		  }

		  if( AchievementHelper::getHelper()->getAchieveByID(acives.at(idx).ID)->isOver){

             auto wc_ts=Sprite::create("ui_chengjiu_wancheng_biaoshi.png"); 
			 wc_ts->setPosition(wc->getPosition());
			 board->addChild(wc_ts,1);

		  }else{

			 auto wc_ts=Sprite::create("ui_chengjiu_weiwancheng_biaoshi.png"); 
			 wc_ts->setPosition(wc->getPosition());
			 board->addChild(wc_ts,1);
		  }



	 string name=acives.at(idx).name;
	 int awrad=acives.at(idx).award_unitID;

	// string awradstr="("+GoodsHelper::getHelper()->getUnitByID(acives.at(idx).ID)->descrigbe+")";
	 //成就描述
	 string dscrible=acives.at(idx).describe;

	
	 auto labelname=Label::createWithSystemFont(name,"Arial",24);
	// labelname->setColor(Color3B(172,65,0));
	 labelname->setPosition(Vec2(labelname->getContentSize().width/2+getDelwithint(25,1),board->getContentSize().height*0.725f));
	 board->addChild(labelname);

	 
	 auto dscriblable=Label::createWithSystemFont(dscrible,"Arial",24);
	 dscriblable->setColor(Color3B(172,65,0));
	 dscriblable->setPosition(Vec2(dscriblable->getContentSize().width/2+getDelwithint(20,1),board->getContentSize().height*0.2f));
	 board->addChild(dscriblable);

	


}
//商城
void BaselisttableLayer::drow_Shoplist(TableViewCell *cell, ssize_t idx){


}




//角色绘制
void BaselisttableLayer::drow_rolelist(TableViewCell *cell, ssize_t idx){


	   bool islock=ActorHelper::getHelper()->getIsUnlockItem(heros.at(idx).unitSaleID);
	  
	   string shuoming="ui_zhuye_jiaoseshengji_shuoming_bg.png"; 
	   int level=1;
	   if(GoodsHelper::getHelper()->getOneItem(heros.at(idx).unitSaleID)!=nullptr){
		   level= GoodsHelper::getHelper()->getOneItem(heros.at(idx).unitSaleID)->value;
	   }
	   

	   string levelstr=String::createWithFormat("%u",level)->getCString()+FX::StringsMap::getValue("level");
	   string herodscrible=GoodsHelper::getHelper()->getUnitByID(heros.at(idx).unitSaleID)->descrigbe;
	   //角色名字
	   auto  heroname=ImageHelper::getUnitSpriteUIScene(heros.at(idx).unitSaleID);  
	  


	   string isstr="ui_anniu_tongyong_zhuye.png";
	   string isstr1="ui_anniu_tongyong_zhuye.png";
	   string isgoumai=FX::StringsMap::getValue("level1");
	  
	   
	   if(islock==false){

		   isstr="ui_anniu_tongyong_goumai.png";
	       isstr1="ui_anniu_tongyong_goumai.png";
		   isgoumai=FX::StringsMap::getValue("buy");
	       
	   }

	   string back_select="ui_zhuye_jiaoseshengji_bg.png";//默认未选择

	   int defalut_id=PlayerInfoHelper::getHelper()->getDeafaultHero();
	   if(defalut_id==heros.at(idx).unitSaleID&&islock==true){

		   hero_ID=heros.at(idx).unitSaleID;
		   back_select="ui_zhuye_jiaoseshengji_bg_select.png";
		   //播放选折动画
	   }else{
		   //播放未选折动画

	   }
	   //pm_bg.jpg
	   auto board =Sprite::createWithTexture(Sprite::create(back_select)->getTexture());
	   cell->addChild(board,1);
	   board->setPosition(Vec2(board->getContentSize().width/2, board->getContentSize().height/2+100*(Actual_y/Design_hight)));
	   //  
	   if(PlayerInfoHelper::getHelper()->getDeafaultHero()==heros.at(idx).unitSaleID&&islock==true){

		  playAnimationByname(board,"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","xuanzhongxiaoguo",false,false);
		  
	    }else{
		   //播放未选折动画
		  playAnimationByname(board,"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","weixuanzhong_xiaoguo",true,false);
	    }

	   hero_back.insert(heros.at(idx).unitSaleID,board);

	   string isselect="ui_zhuye_jiaoseshengji_select.png";
	   auto selectimg=Sprite::create(isselect);
	   selectimg->setPosition(Vec2(board->getContentSize().width-selectimg->getContentSize().width/2,board->getContentSize().height-selectimg->getContentSize().height/2));
	   board->addChild(selectimg);
	   selectimg->setVisible(false);

	   if(PlayerInfoHelper::getHelper()->getDeafaultHero()==heros.at(idx).unitSaleID&&islock==true){
		   selectimg->setVisible(true);
	   }
	   hero_select.insert(heros.at(idx).unitSaleID,selectimg);


	   auto levelimg=Sprite::create("ui_zhuye_jiaoseshengji_dengji_bg.png");
	   levelimg->setPosition(Vec2(board->getContentSize().width/2,board->getContentSize().height-levelimg->getContentSize().height/2));
	   board->addChild(levelimg,200);

	   auto labell=Label::createWithSystemFont(levelstr,"Arial",24);
	   labell->setPosition(Vec2(levelimg->getContentSize().width/2,levelimg->getContentSize().height/2));
	   levelimg->addChild(labell);

	   auto smimg=Sprite::create(shuoming);
	   smimg->setPosition(Vec2(board->getContentSize().width/2,getDelwithint(41,1)));
	   board->addChild(smimg,200);

	   auto herosdlabel=Label::createWithSystemFont(herodscrible,"Arial",20,Size(260,0),TextHAlignment::CENTER);
	   herosdlabel->setPosition(Vec2(smimg->getContentSize().width/2+getDelwithint(5,0),(smimg->getContentSize().height)/2));
	   herosdlabel->setColor(Color3B(172,65,0));
	   smimg->addChild(herosdlabel);

	   auto dizuo=Sprite::create("ui_zhuye_dizuo.png");
	   dizuo->setPosition(Vec2(smimg->getPositionX(),smimg->getContentSize().height/2+smimg->getPositionY()+dizuo->getContentSize().height/2));
	   board->addChild(dizuo,1000);
	   
	   auto heroimg=heroname;
	   heroimg->setAnchorPoint(Vec2(0.5,0.0));
	   float yy=dizuo->getContentSize().height/4;
	   if(heros[idx].unitSaleID==203){
		   yy=0;
	   }
	   heroimg->setPosition(Vec2(dizuo->getContentSize().width/2,yy));
	   heroimg->setScale(0.6f);
	   dizuo->addChild(heroimg);
	   
	  

	   MenuItemImage* juese_button;
	   auto buylabel=Label::createWithSystemFont(isgoumai,"Arial",24);
	   
	   if(islock==false){//购买

	     juese_button= MenuItemImage::create(isstr,isstr1,
                         CC_CALLBACK_1(BaselisttableLayer::juese_Callback1,this));
	     juese_button->setPosition(Vec2(board->getContentSize().width/2,size.height-board->getContentSize().height-juese_button->getContentSize().height/2-10));
		 juese_button->setVisible(false);


		 string money_str="ui_zhuye_jiaoseshengji_jiage.png";
	     auto money1=Sprite::create(money_str);
		 money1->setVisible(false);
		 money1->setPosition(Vec2(board->getContentSize().width/2,size.height-board->getContentSize().height-juese_button->getContentSize().height/2-10));
		 cell->addChild(money1);

	     
		 Sprite* gold_sj;
		 if(heros[idx].unitPayID==UnitID::unitID_Gold){
			gold_sj=Sprite::create("ui_icon_jinbi.png");
		 }else if(heros[idx].unitPayID==UnitID::unitID_Diamond){
			gold_sj=Sprite::create("ui_icon_zuanshi.png");
		 }

		 gold_sj->setScale(0.38f);
		 gold_sj->setPosition(Vec2(getDelwithint(10,0),money1->getContentSize().height/2));
		 money1->addChild(gold_sj);

		 auto gold_lable_next=Label::createWithSystemFont(String::createWithFormat("%u",heros[idx].payPrice)->getCString(),"Arial",24);
		 gold_lable_next->setPosition(Vec2(money1->getContentSize().width/2,money1->getContentSize().height/2));
		 money1->addChild(gold_lable_next);





	   }else{ //升级
		  juese_button=MenuItemImage::create(isstr,isstr1,
                         CC_CALLBACK_1(BaselisttableLayer::juese_Callback,this));
	      juese_button->setPosition(Vec2(board->getContentSize().width/2,size.height-board->getContentSize().height-juese_button->getContentSize().height/2-10));
		  if(ActorHelper::getHelper()->getMaxLevel(heros[idx].unitSaleID)==ActorHelper::getHelper()->getOneHero(heros[idx].unitSaleID).level){
			  //juese_button->setVisible(false);

		  }

		  Sprite* set4=Sprite::create(isstr);
          set4->setColor(Colorzdy);
          juese_button->setSelectedImage(set4);
	   }

	   juese_button->setUserData((void *)heros.at(idx).unitSaleID);
	   buylabel->setPosition(Vec2(juese_button->getContentSize().width/2,juese_button->getContentSize().height/2));
	   juese_button->addChild(buylabel,10);

	   auto menu = Menu::create(juese_button,nullptr);
       menu->setPosition(Vec2::ZERO);
	   cell->addChild(menu, 1);
	   cell->setUserData((void*)heros.at(idx).unitSaleID);


}

//宠物绘制
void BaselisttableLayer::drow_chongwulist(TableViewCell *cell, ssize_t idx){


	  
	   bool islock=ActorHelper::getHelper()->getIsUnlockItem(pets.at(idx).unitSaleID);
	   string shuoming="ui_zhuye_jiaoseshengji_shuoming_bg.png";
	  
	  

	   int level=1;
	   if(GoodsHelper::getHelper()->getOneItem(pets.at(idx).unitSaleID)!=nullptr){
		   level=GoodsHelper::getHelper()->getOneItem(pets.at(idx).unitSaleID)->value;
	   }

	   string levelstr=String::createWithFormat("%u",level)->getCString()+FX::StringsMap::getValue("level");
	   string herodscrible=GoodsHelper::getHelper()->getUnitByID(pets.at(idx).unitSaleID)->descrigbe;
	   //角色名字
	   auto heroname=ImageHelper::getUnitSpriteUIScene(pets.at(idx).unitSaleID);  
		
	   string isstr="ui_anniu_tongyong_zhuye.png";
	   string isstr1="ui_anniu_tongyong_zhuye.png";

	   string isgoumai=FX::StringsMap::getValue("level1");
	  
	   
	   if(islock==false){

		   isstr="ui_anniu_tongyong_goumai.png";
	       isstr1="ui_anniu_tongyong_goumai.png";
		   isgoumai=FX::StringsMap::getValue("buy");
	       
	   }

	   string back_select="ui_zhuye_jiaoseshengji_bg.png";//默认未选择


	  
	   if(islock==true&&PlayerInfoHelper::getHelper()->getDeafaultPet()==pets.at(idx).unitSaleID){

		   
		   pet_ID=pets.at(idx).unitSaleID;
		   back_select="ui_zhuye_jiaoseshengji_bg_select.png";
		   
	   }else{
		  

	   }

	   
	   //背景
	   
	   auto board =Sprite::create(back_select);
	   cell->addChild(board,1);
	   board->setPosition(Vec2(board->getContentSize().width/2, board->getContentSize().height/2+100*(Actual_y/Design_hight)));

	    
	     //  
	   if(PlayerInfoHelper::getHelper()->getDeafaultPet()==pets.at(idx).unitSaleID&&islock==true){

		  playAnimationByname(board,"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","xuanzhongxiaoguo",false,false);
		  
	    }else{
		   //播放未选折动画
		  playAnimationByname(board,"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","weixuanzhong_xiaoguo",true,false);
	    }

	    pet_back.insert(pets.at(idx).unitSaleID,board);


		 // 选折图片
		 string isselect="ui_zhuye_jiaoseshengji_select.png";
	     auto selectimg=Sprite::create(isselect);
	     selectimg->setPosition(Vec2(board->getContentSize().width-selectimg->getContentSize().width/2,board->getContentSize().height-selectimg->getContentSize().height/2));
	     board->addChild(selectimg);
	     selectimg->setVisible(false);

		 if(PlayerInfoHelper::getHelper()->getDeafaultPet()==pets.at(idx).unitSaleID&&islock==true){
		    selectimg->setVisible(true);
		  
	     }
		 pet_select.insert(pets.at(idx).unitSaleID,selectimg);



		
		 //多少级

		  auto levelimg=Sprite::create("ui_zhuye_jiaoseshengji_dengji_bg.png");
	      levelimg->setPosition(Vec2(board->getContentSize().width/2,board->getContentSize().height-levelimg->getContentSize().height/2));
	      board->addChild(levelimg,200);

	      auto labell=Label::createWithSystemFont(levelstr,"Arial",24);
	      labell->setPosition(Vec2(levelimg->getContentSize().width/2,levelimg->getContentSize().height/2));
	      levelimg->addChild(labell);

		  //宠物图片
		   
		 
	     auto heroimg=heroname;
	     heroimg->setPosition(Vec2(board->getContentSize().width/2,board->getContentSize().height/2));
	     board->addChild(heroimg,1000);
	      

		  auto smimg=Sprite::create(shuoming);
	      smimg->setPosition(Vec2(board->getContentSize().width/2,getDelwithint(41,1)));
	      board->addChild(smimg,200);

	      auto herosdlabel=Label::createWithSystemFont(herodscrible,"Arial",20,Size(260,0),TextHAlignment::LEFT);
	      herosdlabel->setPosition(Vec2(smimg->getContentSize().width/2+getDelwithint(5,0),(smimg->getContentSize().height)/2));
	      herosdlabel->setColor(Color3B(172,65,0));
	      smimg->addChild(herosdlabel);


		   MenuItemImage* juese_button;
	       auto buylabel=Label::createWithSystemFont(isgoumai,"Arial",24);
	   
		   if(islock==false){//购买

			  juese_button= MenuItemImage::create(isstr,isstr1,
				 CC_CALLBACK_1(BaselisttableLayer::buy_Callback,this));
			  juese_button->setPosition(Vec2(board->getContentSize().width/2,size.height-board->getContentSize().height-juese_button->getContentSize().height/2-10));
			  juese_button->setVisible(false);


			 string money_str="ui_zhuye_jiaoseshengji_jiage.png";
	         auto money1=Sprite::create(money_str);
			 money1->setVisible(false);
			 petmoney_back.insert(pets.at(idx).unitSaleID,money1);
		     money1->setPosition(Vec2(board->getContentSize().width/2,size.height-board->getContentSize().height-juese_button->getContentSize().height/2-10));
		     cell->addChild(money1);

	     
			 Sprite* gold_sj;
			 if(pets[idx].unitPayID==UnitID::unitID_Gold){
				gold_sj=Sprite::create("ui_icon_jinbi.png");
			 }else if(pets[idx].unitPayID==UnitID::unitID_Diamond){
				gold_sj=Sprite::create("ui_icon_zuanshi.png");
			 }

			 gold_sj->setScale(0.38f);
			 gold_sj->setPosition(Vec2(getDelwithint(10,0),money1->getContentSize().height/2));
			 money1->addChild(gold_sj);

			 auto gold_lable_next=Label::createWithSystemFont(String::createWithFormat("%u",pets[idx].payPrice)->getCString(),"Arial",24);
			 gold_lable_next->setPosition(Vec2(money1->getContentSize().width/2,money1->getContentSize().height/2));
			 money1->addChild(gold_lable_next);


		    }else{ //升级
			  juese_button=MenuItemImage::create(isstr,isstr1,
				  CC_CALLBACK_1(BaselisttableLayer::shengji_Callback,this));
			  Sprite* set4=Sprite::create(isstr);
                set4->setColor(Colorzdy);
                juese_button->setSelectedImage(set4);

			  juese_button->setPosition(Vec2(board->getContentSize().width/2,size.height-board->getContentSize().height-juese_button->getContentSize().height/2-10));
			  if(ActorHelper::getHelper()->getMaxLevel(pets[idx].unitSaleID)==ActorHelper::getHelper()->getOnePet(pets[idx].unitSaleID).level){
			      //juese_button->setVisible(false);
		     }
		    }


		   button_back.insert(pets.at(idx).unitSaleID,juese_button);

		   juese_button->setUserData((void *)pets.at(idx).unitSaleID);
		   buylabel->setPosition(Vec2(juese_button->getContentSize().width/2,juese_button->getContentSize().height/2));
		   juese_button->addChild(buylabel,1,10);

		   auto menu = Menu::create(juese_button,nullptr);
		   menu->setPosition(Vec2::ZERO);
		   cell->addChild(menu, 1);
		   cell->setUserData((void*)pets.at(idx).unitSaleID);
		   
	 

}

//工具绘制
void BaselisttableLayer::drow_gongjugwulist(TableViewCell *cell, ssize_t idx){

	  
	bool islock=false;
	BagItem* bag= (BagItem*)GoodsHelper::getHelper()->getOneItem(magis.at(idx).unitSaleID);

	if(bag!=nullptr){
		islock=true;
	}

	string shuoming="ui_zhuye_jiaoseshengji_shuoming_bg.png";
	string back_select="ui_zhuye_jiaoseshengji_bg.png";//

	string isstr="ui_anniu_tongyong_goumai.png";
	string isstr1="ui_anniu_tongyong_goumai.png";
	string herodscrible=GoodsHelper::getHelper()->getUnitByID(magis.at(idx).unitSaleID)->descrigbe;

	if(islock==true&&PlayerInfoHelper::getHelper()->getDeafaultMagicItem()==magis.at(idx).unitSaleID){
           back_select="ui_zhuye_jiaoseshengji_bg_select.png";
		   magic_ID=magis.at(idx).unitSaleID;
	}



	auto board =Sprite::create(back_select);
	cell->addChild(board,1);
	board->setPosition(Vec2(board->getContentSize().width/2, board->getContentSize().height/2+100*(Actual_y/Design_hight)));

	magic_back.insert(magis.at(idx).unitSaleID,board);

	int idd=PlayerInfoHelper::getHelper()->getDeafaultMagicItem();
	  //  
	if(PlayerInfoHelper::getHelper()->getDeafaultMagicItem()==magis.at(idx).unitSaleID&&islock==true){

		 playAnimationByname(board,"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","xuanzhongxiaoguo",false,false);
		  
	 }else{
		   //播放未选折动画
		 playAnimationByname(board,"Run_zhuye_jiaosexuanze.ExportJson","Run_zhuye_jiaosexuanze","weixuanzhong_xiaoguo",true,false);
	}



	 // 选折图片
	string isselect="ui_zhuye_jiaoseshengji_select.png";
	auto selectimg=Sprite::create(isselect);
	selectimg->setPosition(Vec2(board->getContentSize().width-selectimg->getContentSize().width/2,board->getContentSize().height-selectimg->getContentSize().height/2));
	board->addChild(selectimg);
	selectimg->setVisible(false);

	
	if(islock==true&&PlayerInfoHelper::getHelper()->getDeafaultMagicItem()==magis.at(idx).unitSaleID){
		 selectimg->setVisible(true);
	 }
	magic_select.insert(magis.at(idx).unitSaleID,selectimg);

    
	auto smimg=Sprite::create(shuoming);
	smimg->setPosition(Vec2(board->getContentSize().width/2,getDelwithint(41,1)));
	board->addChild(smimg,200);

	auto herosdlabel=Label::createWithSystemFont(herodscrible,"Arial",24,Size(260,0),TextHAlignment::LEFT);
	herosdlabel->setPosition(Vec2(smimg->getContentSize().width/2+getDelwithint(5,0),(smimg->getContentSize().height)/2));
	herosdlabel->setColor(Color3B(172,65,0));
	smimg->addChild(herosdlabel);
	

	auto heroimg=ImageHelper::getUnitSpriteUIScene(magis.at(idx).unitSaleID);
	//heroimg->setScale(2.0f);
	heroimg->setPosition(Vec2(board->getContentSize().width/2,board->getContentSize().height/2));
	board->addChild(heroimg,1000);

	 /*
	 int level=ActorHelper::getHelper()->getOneMagic(magis[idx].unitSale->ID).level;
	 string levelstr=String::createWithFormat("%u",level)->getCString()+FX::StringsMap::getValue("level");


	 auto levelimg=Sprite::create("ui_zhuye_jiaoseshengji_dengji_bg.png");
	 levelimg->setPosition(Vec2(board->getContentSize().width/2,board->getContentSize().height-levelimg->getContentSize().height/2));
	 board->addChild(levelimg);

	 auto labell=Label::createWithSystemFont(levelstr,"Arial",24);
	 labell->setPosition(Vec2(levelimg->getContentSize().width/2,levelimg->getContentSize().height/2));
	 levelimg->addChild(labell);
	 */


	 MenuItemImage* juese_button;
	 string isgoumai=FX::StringsMap::getValue("buy");
	 auto buylabel=Label::createWithSystemFont(isgoumai,"Arial",24);
	   
     if(islock==false){//购买

		     
			 juese_button= MenuItemImage::create(isstr,isstr1,
				 CC_CALLBACK_1(BaselisttableLayer::gongju_Callback,this));
			 juese_button->setPosition(Vec2(board->getContentSize().width/2,size.height-board->getContentSize().height-juese_button->getContentSize().height/2-10));
			  juese_button->setUserData((void *)magis.at(idx).unitSaleID);
	  buylabel->setPosition(Vec2(juese_button->getContentSize().width/2,juese_button->getContentSize().height/2));
      juese_button->addChild(buylabel);
	  juese_button->setVisible(false);

	  string money_str="ui_zhuye_jiaoseshengji_jiage.png";
	  auto money1=Sprite::create(money_str);
	  money1->setVisible(false);
	  money1->setPosition(Vec2(juese_button->getPositionX(),juese_button->getPositionY()-juese_button->getContentSize().height/2-money1->getContentSize().height/2));
      cell->addChild(money1);
	  magicmoney_back.insert(magis[idx].unitSaleID,money1);
	     
	  Sprite* gold_sj;
	  if(magis[idx].unitPayID==UnitID::unitID_Gold){
			gold_sj=Sprite::create("ui_icon_jinbi.png");
	  }else if(magis[idx].unitPayID==UnitID::unitID_Diamond){
			gold_sj=Sprite::create("ui_icon_zuanshi.png");
	  }

	  gold_sj->setScale(0.38f);
	  gold_sj->setPosition(Vec2(getDelwithint(10,0),money1->getContentSize().height/2));
	  money1->addChild(gold_sj);

	  auto gold_lable_next=Label::createWithSystemFont(String::createWithFormat("%u",magis[idx].payPrice)->getCString(),"Arial",24);
	  gold_lable_next->setPosition(Vec2(money1->getContentSize().width/2,money1->getContentSize().height/2));
	  money1->addChild(gold_lable_next);

	  mg_back.insert(magis[idx].unitSaleID,juese_button);

	  auto menu = Menu::create(juese_button,nullptr);
	  menu->setPosition(Vec2::ZERO);
	  cell->addChild(menu, 1);
	  

      }else{ //升级

	  }

	 cell->setUserData((void*)magis.at(idx).unitSaleID);
	  


}


void BaselisttableLayer::updatetime(float time){


	//this->unschedule(schedule_selector(BaselisttableLayer::updatetime));



	Size layersize;
	ScrollView::Direction direction;

	if(this->isVetical==true){
		direction=ScrollView::Direction::VERTICAL;
		layersize.width=size.width;
		layersize.height=size.height*itemnum;//+this->layerspace*itemnum;
	}else{
		direction=ScrollView::Direction::HORIZONTAL;
		layersize.width=size.width*itemnum;//+this->layerspace*itemnum;
		layersize.height=size.height;
	}



	mTableView = TableView::create(this,layersize);
	addChild(mTableView,2);
	mTableView->setDirection(direction);
	mTableView->setPosition(Vec2(0,getDelwithint(0,0)));
	mTableView->setDelegate(this);
	mTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	

	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vec2(0,0));
	

}


float BaselisttableLayer::getDelwithint(int number,int iswidth){
   float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;

}




void BaselisttableLayer::playAnimationByname(Node *bs,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscal){

	auto armature =Armature::create(filename.c_str());
	if(isloop==true){
		armature->getAnimation()->play(animationName.c_str(),-1,1);
	}else{
	   armature->getAnimation()->play(animationName.c_str());
	   armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(BaselisttableLayer::callfunc_AnimateEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		//cocostudio::SEL_MovementEventCallFunc
	}
	armature->removeFromParentAndCleanup(true);
	if(isscal==true){
	   armature->setScale(1.25);

	}
	armature->setPosition(Vec2(bs->getContentSize().width/2,bs->getContentSize().height/2));
	armature->setTag(100);
	bs->addChild(armature);
	

}

void BaselisttableLayer::playAnimationBynamenode(Node *bs,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscal,Vec2 v2,int zorder){

	int m=0;
	auto armature =Armature::create(filename.c_str());
	if(isscal==true){
	   armature->setScale(1.25);
	}
	armature->setPosition(Vec2(v2));
	
	armature->setTag(100);
	armature->setLocalZOrder(zorder);
	bs->addChild(armature);
	
	if(isloop==true){
		armature->getAnimation()->play(animationName.c_str(),-1,1);
	}else{
	   armature->getAnimation()->play(animationName.c_str());
       armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(BaselisttableLayer::callfunc_AnimateEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	
}


void BaselisttableLayer::callfunc_AnimateEnd(Armature *ar, MovementEventType, const std::string&){

	ar->removeFromParent();
}

// void BaselisttableLayer::checkShouCangJiaAchievement()
// {
// 	Achieve*info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_FengKuangShouChangJia);
// 	if (info && !info->isOver)
// 	{
// 		int iCount = 0;
// 		vector<BagItem> Goods = GoodsHelper::getHelper()->getAllGoods();
// 		for (unsigned int i = 0; i < Goods.size(); ++i)
// 		{
// 			if ( (Goods.at(i).id >= ID_Actor_Start && Goods.at(i).id <= ID_Actor_End)
// 				||  (Goods.at(i).id >= ID_Pet_Start && Goods.at(i).id <= ID_Pet_End)
// 				||  (Goods.at(i).id >= ID_Magic_Start && Goods.at(i).id <= ID_Magic_End))
// 			{
// 				iCount++;
// 			}
// 		}
// 		int iTotal = ActorHelper::getHelper()->getAllHero().size()
// 			+ActorHelper::getHelper()->getAllMagic().size()
// 			+ActorHelper::getHelper()->getAllPet().size();
// 		if (iCount >= iTotal)
// 		{
// 			AchievementHelper::getHelper()->completeOneAchievement(achieveID_FengKuangShouChangJia, 1);
// 		}
// 	}
// }



void BaselisttableLayer::linqu_Callback(Ref* sender){
     
	MenuItemImage* img=(MenuItemImage*)(sender);
	

	int id=(int)(long long)img->getUserData();

	BaseLayer* layer=BaseLayer::create();
	layer->Shieldtouched();
	layer->Achievementpromptbox(id);
	Director::getInstance()->getRunningScene()->addChild(layer);


}



void BaselisttableLayer::drow_tolllist(TableViewCell *cell, ssize_t idx){

	/* 
	 string iswc="ui_chengjiu_wancheng.png";
	 auto board =Sprite::create(iswc.c_str());
	 board->setPosition(Vec2(size.width/2, size.height/2));
	 cell->addChild(board);


     
	 auto gk_name=Label::createWithSystemFont(tollgate.at(idx).name,"Arial",24);
	
	 gk_name->setPosition(Vec2(+getDelwithint(25,1)+gk_name->getContentSize().width/2,board->getContentSize().height*0.725f));
	 board->addChild(gk_name);
	 //TollgateHelper::getHelper()->isCustoms(tollgate.at(idx).ID);
	TollgateHelper::getHelper()->isCanStart(tollgate.at(idx).ID);
	 
	 //未解锁
	 if(TollgateHelper::getHelper()->isCanStart(tollgate.at(idx).ID)==false){

		 TollgateItem item=*TollgateHelper::getHelper()->getOneTollgate(tollgate.at(idx).ID);
		 Achieve acive=*AchievementHelper::getHelper()->getAchieveByID(item.unlockNeedAchiveID);

		 string acive_name=FX::StringsMap::getValue("daocheng")+acive.name+FX::StringsMap::getValue("Chengjiukaiqi");
		 auto need_acive=Label::createWithSystemFont(acive_name,"Arial",24);
		 need_acive->setColor(Color3B(172,65,0));
		 need_acive->setPosition(Vec2(need_acive->getContentSize().width/2+getDelwithint(25,1),board->getContentSize().height*0.20f));
		 board->addChild(need_acive);
		 //已解锁 未通关
	 }else if((TollgateHelper::getHelper()->getOneTollgate(tollgate.at(idx).ID)->unlockNeedAchiveID==0&&TollgateHelper::getHelper()->isCustoms(tollgate.at(idx).ID)==false)||(TollgateHelper::getHelper()->isCanStart(tollgate.at(idx).ID)==true&&TollgateHelper::getHelper()->isCustoms(tollgate.at(idx).ID)==false)){

		 TollgateItem item=*TollgateHelper::getHelper()->getOneTollgate(tollgate.at(idx).ID);
         

		 if(TollgateHelper::getHelper()->getOneTollgate(tollgate.at(idx).ID)->unlockNeedAchiveID!=0){
			 Achieve acive=*AchievementHelper::getHelper()->getAchieveByID(item.unlockNeedAchiveID);
			 string acive_name=FX::StringsMap::getValue("daocheng")+acive.name+FX::StringsMap::getValue("Chengjiukaiqi");
			 auto need_acive=Label::createWithSystemFont(acive_name,"Arial",24);
			 need_acive->setColor(Color3B(172,65,0));
			 need_acive->setPosition(Vec2(need_acive->getContentSize().width/2+getDelwithint(25,1),board->getContentSize().height*0.47f));
			 board->addChild(need_acive);
		 }

		 string need_gold=FX::StringsMap::getValue("Xuxianghan")+": "+__String::createWithFormat("%u",tollgate.at(idx).playNeedUnitCount)->getCString()+FX::StringsMap::getValue("Gold");
		 auto   lable_gold=Label::createWithSystemFont(need_gold,"Arial",24);
		 lable_gold->setColor(Color3B(172,65,0));
		 lable_gold->setPosition(Vec2(lable_gold->getContentSize().width/2+getDelwithint(25,1),board->getContentSize().height*0.20f));
		 board->addChild(lable_gold);


		 string gk_awrd=FX::StringsMap::getValue("tongguanawrd")+":";
		 auto a_lable=Label::create(gk_awrd,"Arial",24);
		 a_lable->setColor(Color3B(172,65,0));
		 a_lable->setPosition(Vec2(board->getContentSize().width-getDelwithint(25,1)-a_lable->getContentSize().width/2,board->getContentSize().height*0.725f));
		 board->addChild(a_lable);

		 auto gold_img=Sprite::create("ui_icon_jinbi.png");
		 gold_img->setScale(0.7f);
		 gold_img->setPosition(Vec2(a_lable->getPositionX()-gold_img->getContentSize().width/2,board->getContentSize().height*0.25f));
		 board->addChild(gold_img);

		
		 string gold_str=":";
		 string text=__String::createWithFormat("%u", tollgate.at(idx).awardUnitCount)->getCString();
		 gold_str+=text;


		 auto ard_label=Label::createWithSystemFont(gold_str,"Arial",24);
		 ard_label->setColor(Color3B(172,65,0));
		 ard_label->setPosition(Vec2(gold_img->getPositionX()+gold_img->getContentSize().width/2+ard_label->getContentSize().width/2-getDelwithint(10,1),gold_img->getPositionY()*0.9f));
		 board->addChild(ard_label);

		//已结锁 &&已通关 
	   }else if(TollgateHelper::getHelper()->isCustoms(tollgate.at(idx).ID)==true&&TollgateHelper::getHelper()->isCanStart(tollgate.at(idx).ID)==true){


		 auto Yitongg=Label::createWithSystemFont(FX::StringsMap::getValue("Yitongguan"),"Arial",24);
		 Yitongg->setColor(Color3B(172,65,0));
		 Yitongg->setPosition(Vec2(board->getContentSize().width/2,board->getContentSize().height/2));
		 board->addChild(Yitongg);

		 auto wC=Sprite::create("ui_chengjiu_wancheng_biaoshi.png");
		 wC->setPosition(Vec2( board->getContentSize().width-wC->getContentSize().width,board->getContentSize().height-wC->getContentSize().height/2-getDelwithint(5,0)));
		 board->addChild(wC);
	 }
	 cell->setUserData((void*)tollgate.at(idx).ID);
	 */
}


// ne_twork




void BaselisttableLayer::drow_Sportslist(TableViewCell *cell, ssize_t idx){


	 
	 auto board =Sprite::create("ui_chengjiu_wancheng.png");
	 cell->addChild(board,1);
	 board->setPosition(Vec2(size.width/2, size.height/2));


	 
	
     auto text=Label::createWithSystemFont(__String::createWithFormat("%d",netSports.at(idx).user_order)->getCString(),"Arial",50);
	 text->setPosition(Vec2(text->getContentSize().width/2+20,board->getContentSize().height/2));
	 text->setColor(Color3B(0,0,0));
	 board->addChild(text);



	  if(PlayerInfoHelper::getHelper()->getUserId()==netSports.at(idx).user_id){
		 myname=Label::createWithSystemFont(PlayerInfoHelper::getHelper()->getPlayerName(),"Arial",24);
		 myname->setPosition(Vec2(50+myname->getContentSize().width/2,board->getContentSize().height*0.8));
		 myname->setColor(Color3B(0,0,0));
		 board->addChild(myname);
	  }else{
         string name=netSports.at(idx).user_name;
		 auto nal=Label::createWithSystemFont(name,"Arial",24);
		 nal->setPosition(Vec2(50+nal->getContentSize().width/2,board->getContentSize().height*0.8));
		 nal->setColor(Color3B(0,0,0));
		 board->addChild(nal);
	  }
	 //昵称
	 //本周最高分
	 auto scl=Label::createWithSystemFont(__String::createWithFormat("%d",netSports.at(idx).score)->getCString(),"Arial",24);
	 scl->setPosition(Vec2(60+scl->getContentSize().width/2,board->getContentSize().height*0.2));
	 scl->setColor(Color3B(0,0,0));
	 board->addChild(scl);




	 
	 if(PlayerInfoHelper::getHelper()->getUserId()==netSports.at(idx).user_id&&netSports.at(idx).score==0){
          PlayerInfoHelper::getHelper()->setThisWeekMaxScore(0, 0);
	 }


	 //角色
	 /*
	 string  hn=netSports.at(idx).hero.type;
	 auto hl=Label::createWithSystemFont(hn,"Arial",24);
	 */
	 int a_type=netSports.at(idx).hero.type;
	 int a_level=netSports.at(idx).hero.level;
	 if(a_type==0||a_level==0){
		 a_level=1;
		 a_type=unitID_Actor1;
	 }

	 if(PlayerInfoHelper::getHelper()->getUserId()==netSports.at(idx).user_id){

		 Actor actor=ActorHelper::getHelper()->getOneHero(PlayerInfoHelper::getHelper()->getDeafaultHero());
		 a_type=actor.unit.ID;
		 a_level=actor.level;
	 }

	 auto hl=ImageHelper::getUnitSpriteRunningScene(a_type);

	 hl->setScale(0.1f);
	 hl->setPosition(Vec2(board->getContentSize().width*0.45,board->getContentSize().height*0.8));
	 board->addChild(hl);

	 auto hl_level=Label::createWithSystemFont(__String::createWithFormat("%d",a_level)->getCString(),"Arial",24);
	 hl_level->setColor(Color3B(0,0,0));
	 hl_level->setPosition(Vec2(hl->getPositionX(),board->getContentSize().height*0.2));
	 board->addChild(hl_level);

	  Actor actor=ActorHelper::getHelper()->getOnePet(PlayerInfoHelper::getHelper()->getDeafaultPet());
	 //宠物
	 /*
	 string  pets="gulagula";
	 auto petl=Label::createWithSystemFont(pets,"Arial",24);
	 */
	 if(netSports.at(idx).pet.level!=-1){
		 int p_type=netSports.at(idx).pet.type;
		 int p_level=netSports.at(idx).pet.level;
		 if(p_type==0||p_level==0){
			 p_level=1;
			 p_type=unitID_PiPiZhu;
		 }

		 if(PlayerInfoHelper::getHelper()->getUserId()==netSports.at(idx).user_id){

			 if(PlayerInfoHelper::getHelper()->getDeafaultPet()>0){
				 Actor actor=ActorHelper::getHelper()->getOnePet(PlayerInfoHelper::getHelper()->getDeafaultPet());
				 p_type=actor.unit.ID;
				 p_level=actor.level;
			 }
	     }
		 auto petl=ImageHelper::getUnitSpriteRunningScene(p_type);
		 petl->setPosition(Vec2(board->getContentSize().width*0.55,board->getContentSize().height*0.8));
		 petl->setScale(0.25f);
		 board->addChild(petl);
     
		 auto pet_level=Label::createWithSystemFont(__String::createWithFormat("%d",p_level)->getCString(),"Arial",24);
		 pet_level->setColor(Color3B(0,0,0));
		 pet_level->setPosition(Vec2(petl->getPositionX(),board->getContentSize().height*0.2));
		 board->addChild(pet_level);
	 }
	 //魔法物
	 if(netSports.at(idx).magic.level!=-1){

		 int m_type=netSports.at(idx).magic.type;
		 int m_level=netSports.at(idx).magic.level;
		 if(m_type==0||m_level==0){
			 m_level=1;
			 m_type=unitID_FaGuangWuXing;
		 }

		 if(PlayerInfoHelper::getHelper()->getUserId()==netSports.at(idx).user_id){

			 Actor actor=ActorHelper::getHelper()->getOneMagic(PlayerInfoHelper::getHelper()->getDeafaultMagicItem());
			 m_type=actor.unit.ID;
			 m_level=actor.level;			 
	     }

		 auto mpetl=ImageHelper::getUnitSpriteUIScene(unitID_FaGuangWuXing,m_level);
		 mpetl->setPosition(Vec2(board->getContentSize().width*0.65,board->getContentSize().height*0.8));
		 board->addChild(mpetl);
     
		 auto mpet_level=Label::createWithSystemFont(__String::createWithFormat("%d",m_level)->getCString(),"Arial",24);
		 mpet_level->setColor(Color3B(0,0,0));
		 mpet_level->setPosition(Vec2(mpetl->getPositionX(),board->getContentSize().height*0.2));
		 board->addChild(mpet_level);
	 }

	 auto modify_name=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(BaselisttableLayer::modify_callback,this));
	 modify_name->setPosition(Vec2(board->getContentSize().width*0.8,board->getContentSize().height*0.4));
	 auto menu=Menu::create(modify_name,nullptr);
	 menu->setPosition(Vec2::ZERO);
	 board->addChild(menu);
	 modify_name->setVisible(false);
	 this->setTextb(modify_name,"xgnc",30,Color3B(0,0,0));

	 if(PlayerInfoHelper::getHelper()->getUserId()==netSports.at(idx).user_id){
		 modify_name->setVisible(true);
	 }
	 
}

void BaselisttableLayer::setTextb(MenuItemImage* node,string str,int fontsize,Color3B b){

	auto label=Label::createWithSystemFont(FX::StringsMap::getValue(str),"Arial",fontsize);
	label->setPosition(Vec2(node->getContentSize().width/2,node->getContentSize().height/2));
	label->setColor(b);
	node->addChild(label);
}


void BaselisttableLayer::modify_callback(Ref* ref){
    

	if(GoodsHelper::getHelper()->getDiamondNum()>=50){
		auto layer=BaseLayer::create();
		layer->modify_nameBox();
		Director::getInstance()->getRunningScene()->addChild(layer);
		timeupname=0;
		schedule(schedule_selector(BaselisttableLayer::laName),1.0f);
		upname=-1;
		name_u=PlayerInfoHelper::getHelper()->getPlayerName();
	}else{
		 
		 BaseLayer* shop=BaseLayer::create();
		 shop->money_no(unitID_Diamond);
	     Director::getInstance()->getRunningScene()->addChild(shop);
	}
}


void BaselisttableLayer::laName(float dt){

	upname=1;
	
	string name=PlayerInfoHelper::getHelper()->getPlayerName();
	if(name.compare(name_u)!=0){
	  myname->setString(PlayerInfoHelper::getHelper()->getPlayerName());
	  unschedule(schedule_selector(BaselisttableLayer::laName));
	}
	
}

//消息 msg
void BaselisttableLayer::drow_msg(TableViewCell *cell, ssize_t idx){
	 
	 
	
	Email ifo=eamils.at(idx);

	if(ifo.type!=Email::ET_Invalid){
		string img_str="ui_chengjiu_weiwancheng.png";//  ui_chengjiu_wancheng1.png
		string type_txt=FX::StringsMap::getValue("Typemsg1");
		string opratefs="chakan";
		if(ifo.type==Email::ET_SystemBonus){
			img_str="ui_chengjiu_wancheng1.png";
			type_txt=FX::StringsMap::getValue("Typemsg1");
		}else if(ifo.type==Email::ET_SystemNotice){
			type_txt=FX::StringsMap::getValue("Typemsg2");
		}else if(ifo.type==Email::ET_FriendChat){
			type_txt=FX::StringsMap::getValue("Typemsg3");
		}
		auto board = Sprite::create(img_str);  //MenuItemImage::create(img_str,img_str,CC_CALLBACK_1(BaselisttableLayer::read_ward,this));
		board->setPosition(Vec2(size.width/2, size.height/2));
		board->setUserData((void*)idx);
		cell->addChild(board);

		//auto menu=Menu::create(board,nullptr);
		//menu->setPosition(Vec2::ZERO);
		//cell->addChild(menu);

		if(ifo.isAlreadyRead==true){

			auto yd_lable=Label::createWithSystemFont(FX::StringsMap::getValue("Yidu"),"Arial",24);
			yd_lable->setColor(Color3B(255,33,33));
			yd_lable->setRotation(-45.0f);
			yd_lable->setPosition(board->getContentSize().width*0.8,board->getContentSize().height/2);
			board->addChild(yd_lable);
		}else{
			
			auto ck_bt=MenuItemImage::create("ui_anniu_tongyong_zhuye_lanse.png","ui_anniu_tongyong_zhuye_lanse_select.png",CC_CALLBACK_1(BaselisttableLayer::ckWarld,this));
			setTextb(ck_bt,"chakan",30,Color3B(0,0,0));
			ck_bt->setPosition(Vec2(board->getContentSize().width-ck_bt->getContentSize().width/2,board->getContentSize().height/2));
			ck_bt->setUserData((&(eamils.at(idx))));

			auto menu=Menu::create(ck_bt,nullptr);
	        menu->setPosition(Vec2::ZERO);
			board->addChild(menu);

		}

		yidus.insert(idx,board);

		//msg type
		auto labelname=Label::createWithSystemFont(type_txt,"Arial",24);
	     // labelname->setColor(Color3B(172,65,0));
	    labelname->setPosition(Vec2(labelname->getContentSize().width/2+getDelwithint(25,1),board->getContentSize().height*0.725f));
	    board->addChild(labelname);

	  // msg content
		 auto dscriblable=Label::createWithSystemFont(ifo.content,"Arial",24);
	     dscriblable->setColor(Color3B(172,65,0));
	     dscriblable->setPosition(Vec2(dscriblable->getContentSize().width/2+getDelwithint(20,1),board->getContentSize().height*0.2f));
	     board->addChild(dscriblable);

	 //  ward type
         if(ifo.bonus_unitID>0){
		 auto m_img=ImageHelper::getUnitSpriteUIScene(ifo.bonus_unitID);
		 m_img->setPosition(Vec2(board->getContentSize().width*0.6,board->getContentSize().height*0.6));
		 board->addChild(m_img);
		
		 auto count_l=Label::createWithSystemFont(__String::createWithFormat("%d",ifo.bonusCount)->getCString(),"Arial",24);
	     count_l->setColor(Color3B(255,33,33));
		 count_l->setPosition(Vec2(m_img->getPositionX(),board->getContentSize().height*0.4f));
	     board->addChild(count_l);
          }         
	}else{

	  auto board =Sprite::create("ui_chengjiu_wancheng.png");
	  cell->addChild(board,1);
	  board->setPosition(Vec2(size.width/2, size.height/2));
		  auto info=Label::createWithSystemFont(FX::StringsMap::getValue("Wxxx"),"Arial",24);
		  info->setPosition(Vec2(board->getContentSize().width/2,board->getContentSize().height/2));
		  board->addChild(info);
	}
}

Email BaselisttableLayer::getEmailid(int id){

	  Email email;
	  vector<Email> eamils=EmailHelper::getHeper()->getAllEmailList(30);
	  for(size_t i=0;i<eamils.size();i++){

		  if(id==eamils.at(i).id){
			  email=eamils.at(i);
			  break;
		  }
	  }

	  return email;
}

void BaselisttableLayer::rmoveeamil(int id){


	Email pe=getEmailid(id);
	EmailHelper::getHeper()->setOneEmailAlreadyRead(id);

	
	if(pe.type==Email::ET_SystemNotice&&NetmsgManger::getNetManager()->chaochuSevenday(time(0), pe.recvTime)){
	  EmailHelper::getHeper()->removeEmail(id);
	  EmailHelper::getHeper()->save();
	  eamils=EmailHelper::getHeper()->getAllEmailList(30);
	  mTableView->reloadData();
	}else if(pe.type!=Email::ET_SystemNotice){
	  EmailHelper::getHeper()->removeEmail(id);
      EmailHelper::getHeper()->save();
	  eamils=EmailHelper::getHeper()->getAllEmailList(30);
	  mTableView->reloadData();
	}

    eamils=EmailHelper::getHeper()->getAllEmailList(30);
	EmailHelper::getHeper()->save();
	
	mTableView->reloadData();

	if(eamils.size()==0){

		std::string tt= FX::StringsMap::getValue("myyj");
	    auto text=Label::createWithSystemFont(tt, "Arial", 34);
	    text->setColor(Color3B(0,0,0));
	    text->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
	    addChild(text,100);
	}

}

void BaselisttableLayer::ckWarld(Ref* ref){

	 
	MenuItemImage* p=(MenuItemImage*)ref;
	Email* ifo=(Email*)p->getUserData();

	

	BasicLayer* layer=BasicLayer::create();
	layer->Chankanyoujian(ifo,this);
	Director::getInstance()->getRunningScene()->addChild(layer);


}


void BaselisttableLayer::setMails_list(vector<MailInfo_layer> &ms){

	/*
	   MailType_Invalid ,	//无效类型
	   MailType_Bonus 	,	//系统奖励
	   MailType_Notice ,	//系统通知 
	   MailType_Friend ,	//好友交互
	*/
	if(mails.size()>0){
		mails.clear();
	}
	for(size_t i=0;i<ms.size();i++){

		if((ms.at(i).type==MailType_Bonus||ms.at(i).type==MailType_Friend)&&ms.at(i).is_read==true){

		}else{
			mails.push_back(ms.at(i));
		}
	}


}


