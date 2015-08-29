#include "NetactorLayer.h"
#include "ActiveValueHelper.h"
#include "StopLayer.h"
#include "BasicLayer.h"
#include "GuideLayer.h"
#include "GameDirector.h"
#include "Dialog.h"

#define    suo_img       "UI_jiaose_qiehuanrenwu_bg_weikaifang.png"
#define    suo_kuanimg   "UI_jiaose_qiehuanrenwu_suoding.png"
#define    suo_bg        "UI_jiaose_qiehuanrenwu_bg.png"
#define    Vone_hero_back "UI_jiaose_bg.png"
#define    Vone_hero_bt   "UI_jiaose_biaoti.png"
#define    Vone_gjl_img   "UI_jiaose_shuxing_gongjili.png"
#define    Vone_s_img     "UI_jiaose_shuxing_dengji.png"
#define    Vone_x_img     "UI_jiaose_shuxing_dengji_kuang.png"
#define    Vone_xues_img  "UI_jiaose_shuxing_shengming.png"
#define    Vone_xuex_img  "UI_jiaose_shuxing_shengming_kuang.png"
#define    Vone_sj_button_img "UI_jiaose_anniu_shengji.png"
#define    Vone_sj_img      "UI_jiaose_shengji_anniu_shengji.png"
#define    Vone_mj_img     "UI_jiaose_shengji_anniu_manji.png"
#define    Vone_qx_img     "UI_jiaose_shengji_anniu_quxiao.png"
#define    Vone_mjmark_img  "UI_jiaose_shuxing_tishi_max.png"
#define    Vone_mjtishi_img  "UI_jiaose_anniu_shengji_manji.png"
#define    Vone_jiesuo_img   "UI_jiaose_anniu_jiesuo.png"
#define    Vone_jiesuo_sp_img   "UI_jiaose_anniu_jiesuo_suipian.png"
#define    Vone_return_mainlayer_img  "UI_jiaose_anniu_guanbi.png"



#define    color_h1   Color3B(184,69,13)
#define    color_b1   Color3B(255,255,255)
#define    color_y1   Color3B(254,218,185)


#define   Msg_id  "listmsgid"
enum Herotag{
	 
	 herolist_tag,
	 vone_backimg_tag,
	 Vone_ggl_label_tag,
	 Vone_dj_label_tag,
	 Vone_x_label_tag,
	 Vone_nextggl_label_tag,
	 Vone_sj_button_tag,
	 Vone_sj0_button_tag,
	 Vone_msj0_button_tag,
	 Vone_qxsj0_button_tag,
	 Vone_msjmenu_tag,
	 Vone_sjmenu_tag,
	 Vone_return_button_tag,
	 Vone_zhuye_tag,
	 Vone_zhuye_ganme_tag,
	 shouzhi_tag,
	 Vone_suipian_label_tag,
	 Vone_suipian_item_tag,
};
//islevel 0 表示人物界面  1 表示升级界面 -1 表示已满级

enum Levelmark{

	 level_zero=0,
	 level_one,
	 leve_qone,

};


enum HeroIsteachmove{

	 nomarl=0,
     teacher,
};


NetactorLayer::NetactorLayer(void)
{
}


NetactorLayer::~NetactorLayer(void)
{
}


bool NetactorLayer::init(){

	if(!Layer::init()){
		return false;
	}
	
	//升级
	oprate_fanxiangtag=Vone_zhuye_tag;
	islevel=level_zero;
	auto callBack       =  [&](EventCustom* evt)  
                            {  
								this->callback_sj(evt);
                            };  
	auto sj=EventListenerCustom::create(MSG_LevelUp_Rsp,callBack);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(sj,1);  
	netactorEvents.push_back(sj);

	//选择
	auto callBack_id      =  [&](EventCustom* evt)  
                            {  
								MsgData* data=(MsgData*)evt->getUserData();
								if (isUnlockhero(data->value)){
									PlayerInfoHelper::getHelper()->setDeafaultHero(data->value);
								}
								dataInint(data->value);
                            };  
	auto sj_id=EventListenerCustom::create(Msg_id,callBack_id);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(sj_id,1);  
	netactorEvents.push_back(sj_id);

	//解锁
	auto callBack_unlock      =  [&](EventCustom* evt)  
	{
		MsgData* data=(MsgData*)evt->getUserData();
		if (data->errorCode == Rsp_Gold_UnEnough){
			MessageDialog::showDialog("gold unenough");
		}else if (data->errorCode == Rsp_Jewel_UnEnough){
			MessageDialog::showDialog("jewel unenough");
		}else if (data->errorCode == Rsp_Success){
			dataInint(cId);//刷新
		}else if (data->errorCode == Rsp_Suipian_UnEnough){
			MessageDialog::showDialog(FX::StringsMap::getValue("suipianbuzu"));
		}else {
			MessageDialog::showDialog("unhandle error");
		}
	};  
	auto unlockListener=EventListenerCustom::create(MSG_MarketBuyGoods_Rsp,callBack_unlock);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(unlockListener,1);  
	netactorEvents.push_back(unlockListener);


	Drawloadboundary();



	return true;
}

void NetactorLayer::setGameTag(){

	 oprate_fanxiangtag=Vone_zhuye_ganme_tag;
	 
}

void NetactorLayer::onEnter(){
	
	Layer::onEnter();
}
void NetactorLayer::onExit(){

    for(auto m:netactorEvents){

		Director::getInstance()->getEventDispatcher()->removeEventListener(m);
	}
	Layer::onExit();

}


float NetactorLayer::getDelwithint(float number,int iswidth){

	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	
	return dealnumber;

}
void NetactorLayer::dataInint(int id){


// 	 if(hero.unit.ID==unitID_Actor1){
// 		 xkongz.xx_action->stopAllActions();
// 	 }



	 cId=id;	
	 hero=ActorHelper::getHelper()->getOneHero(cId);
	 islevel=level_zero;
	 
     Drawequal();
}


void NetactorLayer::goshenji(){

	 current_oprate=Vone_sjmenu_tag;
	 islevel=level_one;
     Drawequal();
	 setGameTag();
}

//draw
void NetactorLayer::Drawloadboundary(){

	 Size visibleSize = Director::getInstance()->getWinSize();
	 Actual_x=visibleSize.width;
	 Actual_y=visibleSize.height;
	  //设置大小
	 this->setContentSize(Size(Actual_x,Actual_y-getDelwithint(Tophight,1)));

	 //当前id 数据初始化
	 cId=PlayerInfoHelper::getHelper()->getDeafaultHero();
	 currentselect=PlayerInfoHelper::getHelper()->getDeafaultHero();
	 hero=ActorHelper::getHelper()->getOneHero(cId);
	 getAllherosdata();
	 //界面初始化
	 Drawequal();

	 if(PlayerInfoHelper::getHelper()->getTeacher()==HeroIsteachmove::teacher){

		 	current_oprate=Vone_sj_button_tag;
			islevel=level_one;
			callsp();
	 }
}

void NetactorLayer::refreshHeroImage(Node* node, unsigned int heroId)
{
	node->removeAllChildren();
	if (heroId == unitID_Actor1){
		playAnimationByname(node,"xiaoxiao_NG.ExportJson","xiaoxiao_NG","xx_ng",true,false,Vec2(-500,50));
	}else if (heroId == unitID_Actor2){
		playAnimationByname(node,"xiaoxiao_NG.ExportJson","xiaoxiao_NG","ald_ng",true,false,Vec2(-500,50));
	}
}
void NetactorLayer::Drawequal(){

	this->removeAllChildren(); 
	//人物 是否已选折
	bool is_st=isSelect(cId);

	Layer* h_l=Layer::create();
	h_l->setPosition(Vec2(getContentSize().width*0.325,getContentSize().height*0.5));
	addChild(h_l,1000);	
	refreshHeroImage(h_l, cId);

	back_hero=Sprite::create(Vone_hero_back);
	auto back_biti=Sprite::create(Vone_hero_bt);
	back_biti->setPosition(Vec2(Actual_x-back_hero->getContentSize().width/2,Actual_y-back_biti->getContentSize().height/2));
	back_biti->setLocalZOrder(100);
	addChild(back_biti);

	back_hero->setPosition(Vec2(Actual_x-back_hero->getContentSize().width/2,
		                        Actual_y-back_biti->getContentSize().height-back_hero->getContentSize().height/2+40));
    back_hero->setTag(vone_backimg_tag);
	addChild(back_hero);


	

	//Vone_return_mainlayer_img
	levelDraw();
	levalChange();

	//Vone_sj_button_img
	if(isUnlockhero(cId)==true&&isMaxlevel()==false){
			if(islevel==level_zero){
				auto sj_button=MenuItemImage::create(Vone_sj_button_img,Vone_sj_button_img,CC_CALLBACK_1(NetactorLayer::allcallback,this));
				sj_button->setTag(Vone_sj_button_tag);
				sj_button->setPosition(Vec2(back_hero->getPositionX(),sj_button->getContentSize().height/2));
				fillOuty(sj_button,Vone_sj_button_img);

				auto menu=Menu::create(sj_button,nullptr);
				menu->setPosition(Vec2(0,0));
				menu->setTag(Vone_msjmenu_tag);
				addChild(menu);
				Drawnoequal();
			}else if(islevel==level_one){

				m_herodata.sj_p=MenuItemImage::create(Vone_sj_img,Vone_sj_img,CC_CALLBACK_1(NetactorLayer::allcallback,this));
				m_herodata.sj_p->setTag(Vone_sj0_button_tag);
				m_herodata.sj_p->setPosition(Vec2(back_hero->getPositionX()-m_herodata.sj_p->getContentSize().width,m_herodata.sj_p->getContentSize().height/2));
				fillOuty(m_herodata.sj_p,Vone_sj_img);
				Vec2 sjv=getItemoney();

				


                string sj_yuan;
				
				if(PlayerInfoHelper::getHelper()->getTeacher()==HeroIsteachmove::teacher){

					sj_yuan=FX::StringsMap::getValue("mianfeng");

					
						auto p=Sprite::create("shouzhi.png");
						p->setAnchorPoint(Vec2(0.5,0.5));
						p->setRotation(-90);
						p->setPosition(Vec2(m_herodata.sj_p->getPositionX()+m_herodata.sj_p->getContentSize().width/2,m_herodata.sj_p->getPositionY()+m_herodata.sj_p->getContentSize().height/4));
						addChild(p);
						p->setTag(shouzhi_tag);
						p->setGlobalZOrder(1000);
						p->runAction(RepeatForever::create(Sequence::create( FadeOut::create(0.5f),FadeIn::create(0.5f),nullptr)));

				}else{
					sj_yuan=getneedNum();
					sj_yuan+=FX::StringsMap::getValue("chinsey");
				}
				if(sjv.x==unitID_Gold||sjv.x==unitID_Diamond){

					if(PlayerInfoHelper::getHelper()->getTeacher()==HeroIsteachmove::nomarl)
					  sj_yuan=__String::createWithFormat("%.0f",sjv.y)->getCString();
				}
				auto sj_m=Label::createWithSystemFont(sj_yuan,"minijiankatong.ttf",30);
				sj_m->setColor(Color3B(254,218,185));
				if(sjv.x==unitID_Gold||sjv.x==unitID_Diamond){
			
					string qname="UI_zhuye_shangdian_jinbi.png";
					if(sjv.x==unitID_Diamond){
						 qname="UI_zhuye_shangdian_zuanshi.png";
					}


					if(PlayerInfoHelper::getHelper()->getTeacher()==HeroIsteachmove::nomarl){
						
                        auto m_img=Sprite::create(qname);
						sj_m->setAnchorPoint(Vec2(0,0.5));
						sj_m->setPosition(Vec2((m_herodata.sj_p->getContentSize().width-sj_m->getContentSize().width-m_img->getContentSize().width)/2,m_herodata.sj_p->getContentSize().height*0.2));
						m_img->setPosition(Vec2(sj_m->getPositionX()+sj_m->getContentSize().width+m_img->getContentSize().width/2,sj_m->getPositionY()));
						m_img->setTag(125);
						m_herodata.sj_p->addChild(m_img);
					}else{
						sj_m->setPosition(Vec2(m_herodata.sj_p->getContentSize().width/2,m_herodata.sj_p->getContentSize().height*0.2));
					}
				}else{
					sj_m->setPosition(Vec2(m_herodata.sj_p->getContentSize().width/2,m_herodata.sj_p->getContentSize().height*0.2));
				}
				sj_m->setLocalZOrder(100);
				sj_m->setTag(100);
				m_herodata.sj_p->addChild(sj_m);

		
				auto sj_m1=Label::createWithSystemFont(sj_yuan,"minijiankatong.ttf",30);
				sj_m1->setAnchorPoint(sj_m->getAnchorPoint());
				sj_m1->setPosition(Vec2(sj_m->getPositionX(),sj_m->getPositionY()-2));
				sj_m1->setLocalZOrder(90);
				sj_m1->setTag(200);
				m_herodata.sj_p->addChild(sj_m1);




				auto mj=MenuItemImage::create(Vone_mj_img,Vone_mj_img,CC_CALLBACK_1(NetactorLayer::allcallback,this));
				mj->setTag(Vone_msj0_button_tag);
				mj->setPosition(Vec2(back_hero->getPositionX(),mj->getContentSize().height/2));
				fillOuty(mj,Vone_mj_img);
				//getExsonneedNum();
				string mj_yuan="12"+FX::StringsMap::getValue("chinsey");
				auto mj_m=Label::createWithSystemFont(mj_yuan,"minijiankatong.ttf",30);
				mj_m->setColor(Color3B(254,218,185));
				mj_m->setPosition(Vec2(mj->getContentSize().width/2,mj->getContentSize().height*0.2));
				mj_m->setLocalZOrder(100);
				mj->addChild(mj_m);

				auto mj_m1=Label::createWithSystemFont(mj_yuan,"minijiankatong.ttf",30);
				mj_m1->setPosition(Vec2(mj->getContentSize().width/2,mj->getContentSize().height*0.2-2));
				mj_m1->setLocalZOrder(90);
				mj->addChild(mj_m1);


				auto qxsj=MenuItemImage::create(Vone_qx_img,Vone_qx_img,CC_CALLBACK_1(NetactorLayer::allcallback,this));
				qxsj->setTag(Vone_qxsj0_button_tag);
				qxsj->setPosition(Vec2(back_hero->getPositionX()+qxsj->getContentSize().width,qxsj->getContentSize().height/2));
				fillOuty(qxsj,Vone_qx_img);

				auto menu=Menu::create(m_herodata.sj_p,mj,qxsj,nullptr);
				menu->setPosition(Vec2(0,0));
				menu->setTag(Vone_sjmenu_tag);
				addChild(menu);
			}
	}else if(isUnlockhero(cId)==true&&isMaxlevel()==true){

		drawMaojitishi();

		Drawnoequal();
	}else if(isUnlockhero(cId)==false){
		//不提供功能
		Drawnoequal();

//		if(hero.unit.ID==unitID_Actor1)
		{
		//Vone_jiesuo_img
					

			Menu *menu = Menu::create();
			addChild(menu);
			menu->setPosition(Vec2::ZERO);
			auto p=MenuItemImage::create(Vone_jiesuo_img,Vone_jiesuo_img,CC_CALLBACK_1(NetactorLayer::unlockHeroCallback,this));
			menu->addChild(p);
			fillOuty(p,Vone_jiesuo_img);
			//auto p=Sprite::create(Vone_jiesuo_img);
			p->setPosition(Vec2(back_hero->getPositionX()*0.85,p->getContentSize().height/2));
			auto p_suipian_jiesuo =MenuItemImage::create(Vone_jiesuo_sp_img,Vone_jiesuo_sp_img,CC_CALLBACK_1(NetactorLayer::unlockHeroCallback,this));
			p_suipian_jiesuo->setTag(Vone_suipian_item_tag);
			menu->addChild(p_suipian_jiesuo);
			p_suipian_jiesuo->setAnchorPoint(Vec2(0,0.5));
			fillOuty(p_suipian_jiesuo,Vone_jiesuo_sp_img);
			//auto p=Sprite::create(Vone_jiesuo_img);
			p_suipian_jiesuo->setPosition(Vec2(back_hero->getPositionX()*1.05,p->getContentSize().height/2));

			int count=0;
			int type=unitID_Gold;
			int cout_suipian = 0;
			int type_suipian = unitID_Gold;
			int suipianType= unitID_Gold;
			for(auto s:heros){
				if(s.unitSaleID==cId){
					count=s.payPrice;
					type=s.unitPayID;
					p->setTag(s.saleID);
					break;
				}
			}

			string moeny_type="unitID_Gold";
			DrawButtonInfo(type,p,count);
			DrawButtonInfo(suipianType,p_suipian_jiesuo,gold_suipian.second);
		}
		

	}

}

void NetactorLayer::DrawButtonInfo(int type,MenuItemImage* p,int count){
	Sprite* p_img=nullptr;	
	if(type==unitID_Gold){
		p_img=Sprite::create("UI_zhuye_shangdian_jinbi.png");
		auto m_numlabel=Label::createWithSystemFont(__String::createWithFormat("%d",count)->getCString(),"minijiankatong.ttf",30);
		m_numlabel->setPosition(Vec2(p->getContentSize().width/2,m_numlabel->getContentSize().height*0.6));
		p->addChild(m_numlabel);
		p_img->setPosition(Vec2(m_numlabel->getPositionX()+m_numlabel->getContentSize().width/2+p_img->getContentSize().width/2,m_numlabel->getPositionY()));
		p->addChild(p_img);
	}else if(type==unitID_Diamond){
		p_img=Sprite::create("UI_zhuye_shangdian_zuanshi.png");
		auto m_numlabel=Label::createWithSystemFont(__String::createWithFormat("%d",count)->getCString(),"minijiankatong.ttf",30);
		m_numlabel->setPosition(Vec2(p->getContentSize().width/2,m_numlabel->getContentSize().height*0.6));
		p->addChild(m_numlabel);
		p_img->setPosition(Vec2(m_numlabel->getPositionX()+m_numlabel->getContentSize().width/2+p_img->getContentSize().width/2,m_numlabel->getPositionY()));
		p->addChild(p_img);

	}else if(type==unitID_RMB){
			
		string money=__String::createWithFormat("%d",count)->getCString();
		money+=": "+FX::StringsMap::getValue("yuan");
		auto m_numlabel=Label::createWithSystemFont(money,"minijiankatong.ttf",30);
		m_numlabel->setPosition(Vec2(p->getContentSize().width/2,m_numlabel->getContentSize().height/2));
		p->addChild(m_numlabel);
	}else if(type==unitID_Actor2Fragment){
			
		string money=__String::createWithFormat("%d",count)->getCString();
		auto m_numlabel=Label::createWithSystemFont(money,"minijiankatong.ttf",30);
		m_numlabel->setPosition(Vec2(p->getContentSize().width/2,m_numlabel->getContentSize().height/2));
		p->addChild(m_numlabel);
	}
}

void NetactorLayer::Drawnoequal(){
      

	auto p=Herolist::create();
	p->setPosition(Vec2(getContentSize().width*0.06,0));
	p->setTag(herolist_tag);
	addChild(p);
	
    

}


void NetactorLayer::datamanger(){

	  images.clear();
	  hero=ActorHelper::getHelper()->getOneHero(cId);
	  getAllherosdata();
	  Drawequal();

}

//caner

bool NetactorLayer::isMaxlevel(){

	 
	 return ActorHelper::getHelper()->getMaxLevel(cId)==hero.level;
}

int NetactorLayer::getchashenme(int id){

	  int isGoumai=unitID_Gold;
	  if(actorType2(id)==unitID_Gold){

		  if(GoodsHelper::getHelper()->getGoldNum()<getType2(id)){
			  isGoumai=unitID_Gold;
		  }
	  }else if(actorType2(id)==unitID_Diamond){

		  if(GoodsHelper::getHelper()->getDiamondNum()<getType2(id)){
			  isGoumai=unitID_Diamond;
		  }
	  }
	  return isGoumai;
}

int  NetactorLayer::getYuan(){
	 

	return 3;


}

Vec2 NetactorLayer::getItemoney(){

	 
	 Vec2 m;
	 int actorlevel=hero.level;
	 if(actorlevel==0){
		 actorlevel=1;
	 }
	 int zlv=actorlevel;
	 if(zlv<ActorHelper::getHelper()->getMaxLevel(cId)){
		  zlv=actorlevel+1;
	 }else if(zlv==ActorHelper::getHelper()->getMaxLevel(cId)){
		  zlv=ActorHelper::getHelper()->getMaxLevel(cId);
	 }
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit.ID,zlv);
	 m.x=curent_info->needUnit->ID;
	 m.y=curent_info->needCount;
	 return m;

}

bool  NetactorLayer::getmy_moneyislonh(int id){

	  bool isGoumai=false;
	  if(actorType2(id)==unitID_Gold){

		  if(GoodsHelper::getHelper()->getGoldNum()>=getType2(id)){
			  isGoumai=true;
		  }
	  }else if(actorType2(id)==unitID_Diamond){

		  if(GoodsHelper::getHelper()->getDiamondNum()>=getType2(id)){
			  isGoumai=true;
		  }
	  }

	  return isGoumai;
}


int  NetactorLayer::getHowtype(){

	 int type=0;
	
	for(size_t i=0;i<heros.size();i++){

		if(cId==heros.at(i).unitSaleID){
			type=heros.at(i).payPrice;
			break;
		}
	}
    return type;

}



int  NetactorLayer::getType2(int id){


	int type=0;
	for(size_t i=0;i<heros.size();i++){

		if(cId==heros.at(i).unitSaleID){
			type=heros.at(i).payPrice;
			break;
		}
	}
    return type;


}

int  NetactorLayer::actorType2(int id){

	int type=0;
	
	for(size_t i=0;i<heros.size();i++){

		if(id==heros.at(i).unitSaleID){
			type=heros.at(i).unitPayID;
			break;
		}
	}
    return type;

}

int  NetactorLayer::actorType(){

	int type=0;
	
	for(size_t i=0;i<heros.size();i++){

		if(cId==heros.at(i).unitSaleID){
			type=heros.at(i).unitPayID;
			break;
		}
	}
    return type;
}

bool NetactorLayer::isSelect(int id){
	  
	 
	 return ActorHelper::getHelper()->getIsUnlockItem(id);
}

int  NetactorLayer::getXixi(int id){
	  
	  int actorlevel=hero.level;
	 if(actorlevel==0){
		 actorlevel=1;
	 }
	 return actorlevel;	 
}

int  NetactorLayer::getSm(int id){

	 int actorlevel=hero.level;
	 if(actorlevel==0){
		 actorlevel=1;
	 }
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit.ID,actorlevel);
	 vector<Attribute> attrVec=curent_info->attrVec;
	 int sm=0;
	 for(size_t i=0;i<attrVec.size();i++){
		 if(attrVec.at(i).attr.compare(hp)==0){
			 sm=attrVec.at(i).value;
			 break;
		 } 
	 }
	 return sm;
}

int NetactorLayer::getNextsm(){

	 int actorlevel=hero.level;
	 if(actorlevel==0){
		 actorlevel=1;
	 }
	 int zlv=actorlevel;
	 if(zlv<ActorHelper::getHelper()->getMaxLevel(cId)){
		  zlv=actorlevel+1;
	 }else if(zlv==ActorHelper::getHelper()->getMaxLevel(cId)){
		  zlv=ActorHelper::getHelper()->getMaxLevel(cId);
	 }
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit.ID,zlv);
	 vector<Attribute> attrVec=curent_info->attrVec;
	 int sm=0;
	 for(size_t i=0;i<attrVec.size();i++){
		 if(attrVec.at(i).attr.compare(hp)==0){
			 sm=attrVec.at(i).value;
			 break;
		 } 
	 }
	 return sm;
}

int NetactorLayer::getMaxsm(){
	 
	 
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit.ID,ActorHelper::getHelper()->getMaxLevel(cId));
	 vector<Attribute> attrVec=curent_info->attrVec;
	 int sm=0;
	 for(size_t i=0;i<attrVec.size();i++){
		 if(attrVec.at(i).attr.compare(hp)==0){
			 sm=attrVec.at(i).value;
			 break;
		 } 
	 }
	 return sm;

}

int NetactorLayer::getSuipian(UnitID ID){
	GoodsHelper* help_info = GoodsHelper::getHelper();	
	int suipian =0;
	suipian = help_info ->getActorFragmentNum(ID);
	return suipian  ;
}

int NetactorLayer::getMaxSuipian(){
	return 0 ;
}

string NetactorLayer::hetheromisoshu(int id){
	 
	string heroms=hero.unit.descrigbe;
	return heroms;
	 
}

string NetactorLayer::hetheroTese(int id){

	string heroms=FX::StringsMap::getValue("tesemiaoshu");
	return heroms;
}

string NetactorLayer::getHeroGjl(int id){

	 int actorlevel=hero.level;
	 if(actorlevel==0){
		 actorlevel=1;
	 }
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit.ID,actorlevel);
	 vector<Attribute> attrVec=curent_info->attrVec;
	 int sm=0;
	 for(size_t i=0;i<attrVec.size();i++){
		 if(attrVec.at(i).attr.compare(power)==0){
			 sm=attrVec.at(i).value;
			 break;
		 }
	 }
	string Gkl=__String::createWithFormat("%d",sm)->getCString();
	return Gkl;
}


string NetactorLayer::getneedNum(){

	 int actorlevel=hero.level;
	 if(actorlevel==0){
		 actorlevel=1;
	 }

	 if(actorlevel==ActorHelper::getHelper()->getMaxLevel(hero.unit.ID)){

	 }else{
		 actorlevel+=1;
	 }
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit.ID,actorlevel);
	 vector<Attribute> attrVec=curent_info->attrVec;
	int sm=curent_info->needCount;
	string Gkl=__String::createWithFormat("%d",sm)->getCString();
	return Gkl;
	   

}

string NetactorLayer::getExsonneedNum(){

	  int num=0;

	  for(int i=1;i<=ActorHelper::getHelper()->getMaxLevel(hero.unit.ID);i++){

		  const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit.ID,i);
		  num+=curent_info->needCount;

	  }
	  return __String::createWithFormat("%d",num)->getCString();
}

string NetactorLayer::getHeronextGjl(int id){

	 int actorlevel=hero.level;
	 if(actorlevel==0){
		 actorlevel=1;
	 }
	 int zlv=actorlevel;
	 if(zlv<ActorHelper::getHelper()->getMaxLevel(cId)){
		  zlv=actorlevel+1;
	 }else if(zlv==ActorHelper::getHelper()->getMaxLevel(cId)){
		  zlv=ActorHelper::getHelper()->getMaxLevel(cId);
	 }
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit.ID,zlv);
	 vector<Attribute> attrVec=curent_info->attrVec;
	 int sm=0;
	 for(size_t i=0;i<attrVec.size();i++){

		 if(attrVec.at(i).attr.compare(power)==0){
			 sm=attrVec.at(i).value;
			 break;
		 }
	 }
	string Gkl=__String::createWithFormat("%d",sm)->getCString();
	return Gkl;

}

bool NetactorLayer::isUnlockhero(int id){

	return ActorHelper::getHelper()->getIsUnlockItem(id);

}


void  NetactorLayer::allcallback(Ref* ref){

	
		MenuItemImage* p=(MenuItemImage*)ref;
		int tag=(int)p->getTag();
		current_oprate=tag;

	if(current_oprate==Vone_sj_button_tag){
       islevel=level_one;
	}else{
	   FX::Music::getInstance()->buttonClicked();
	}

	if(PlayerInfoHelper::getHelper()->getTeacher()==HeroIsteachmove::teacher){
		if(current_oprate==Vone_sj0_button_tag){
	       callsp();
		}
	}else{
		callsp();
	}
	
}

void NetactorLayer::unlockHeroCallback(Ref* ref)
{
	MenuItemImage* p=(MenuItemImage*)ref;
	int id=(int)p->getTag();
	if (id == Vone_suipian_item_tag)
	{
		ActorHelper::getHelper()->unlockHeroByFragment(cId);
	} 
	else
	{
		if (isUnlockhero(id)) return;

		MarketHelper::getHelper()->buyItem(id);
	}
}

void NetactorLayer::bAction(MenuItemImage* node){
     
}

void NetactorLayer::callsp(){

	 
	
	 if(current_oprate==Vone_sj_button_tag)
	 {
		   //界面从绘
	      Drawequal();

	 }else if(current_oprate==Vone_sj0_button_tag){

         if(PlayerInfoHelper::getHelper()->getTeacher()==HeroIsteachmove::teacher){
			 this->getChildByTag(shouzhi_tag)->removeFromParent();
			 PlayerInfoHelper::getHelper()->setTeacher(-1);
			 
		 }
		 ActorHelper::getHelper()->levelUp(hero.unit.ID);

	 }else if(current_oprate==Vone_msj0_button_tag){

		 ActorHelper::getHelper()->fullLevelUp(hero.unit.ID);

	 }else if(current_oprate==Vone_qxsj0_button_tag){
		  islevel=false;
		  Drawequal();
	 }else if(current_oprate==Vone_return_button_tag){


		if(oprate_fanxiangtag==Vone_zhuye_tag){
			   EventCustom event_m(Layerchange);
			   LayerData data_m;
			   data_m.current=102;
			   event_m.setUserData(&data_m);
			   Director::getInstance()->getEventDispatcher()->dispatchEvent(&event_m);

		 }else if(oprate_fanxiangtag==Vone_zhuye_ganme_tag){

			 this->getParent()->removeFromParent();
		 }

	}

}

void  NetactorLayer::level_callback(Ref* ref){


	/*
	#define    Vone_sj_img      "UI_jiaose_shengji_anniu_shengji.png"
	#define    Vone_mj_img     "UI_jiaose_shengji_anniu_manji.png"
	#define    Vone_qx_img     "UI_jiaose_shengji_anniu_quxiao.png"
	EventCustom event(Layerchange);
	LayerData data;
	data.current=13;
	data.push=12;
	data.heroid=id;
	event.setUserData(&data);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
	*/
}

void  NetactorLayer::buy_callback(Ref* ref){
      
	 
	  MenuItemImage* p=(MenuItemImage*)ref;

	  int id=(int)(long long)p->getUserData();

	  if(getmy_moneyislonh(id)){

		  

		  MarketHelper::getHelper()->buyItem(id);
		  //活跃值统计 购买新角色

		   cId=id;
		   if(isUnlockhero(id)){
		      currentselect=id;
		      PlayerInfoHelper::getHelper()->setDeafaultHero(id);
	       }
		  ActiveValueHelper::getHelper()->addActiveByType(activeType_MaiXinJueSe);
		  datamanger();

		  //检测收藏I，收藏III成就
		  Achieve*info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ShouCangI);
		  Achieve*info1 = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ShouCangIII);
		  if ((info && !info->isOver) || (info1 && !info1->isOver))
		  {
			  int iCount = 0;
			  vector<BagItem> allGoods = GoodsHelper::getHelper()->getAllGoods();
			  for (unsigned int i = 0; i < allGoods.size(); ++i)
			  {
				  if (allGoods.at(i).id >= ID_Actor_Start && allGoods.at(i).id <= ID_Actor_End)
				  {
					  iCount++;
				  }
			  }
			  if (iCount == 2)
			  {
				  AchievementHelper::getHelper()->completeOneAchievement(achieveID_ShouCangI, 1);
			  }
			  else if (iCount == 3)
			  {
				  AchievementHelper::getHelper()->completeOneAchievement(achieveID_ShouCangIII, 1);
				  checkShouCangJiaAchievement();
			  }
		  }
		  StopLayer::checkFightPowerAchievement();
	  }else{

		  //int cha=getmy_moneyislonh(id);
		  auto layer=BaseLayer::create();
		  layer->money_no(actorType2(id));
		  Director::getInstance()->getRunningScene()->addChild(layer);
	  }
}

void  NetactorLayer::select_callback(Ref* ref){
	  
	  
	  MenuItemImage* p=(MenuItemImage*)ref;
	  int Id=(int)(long long)p->getUserData();
	  cId=Id;
	  if(isUnlockhero(Id)){
		  currentselect=Id;
		  PlayerInfoHelper::getHelper()->setDeafaultHero(Id);
	  }

	  images.clear();
	  hero=ActorHelper::getHelper()->getOneHero(cId);
	  getAllherosdata();
	  Drawequal();
	  
}


//数据初始化
void NetactorLayer::getAllherosdata(){
	 
 	 heros.clear();
	 vector<SaleItem> tls=(vector<SaleItem>)*MarketHelper::getHelper()->getAllSaleItems();
		int m_size=tls.size();
		for(int i=0;i<m_size;i++){
			if(GoodsHelper::getHelper()->getUnitByID(tls[i].unitSaleID)->type==unittype_Actor){
				heros.push_back(tls[i]);
			}
	 }
	
	// heros=ActorHelper::getHelper()->getAllHero();

}

//检测收藏家成就
void NetactorLayer::checkShouCangJiaAchievement()
{
	Achieve*info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ShouCangJia);
	if (info && !info->isOver)
	{
		int iCount = 0;
		vector<BagItem> Goods = GoodsHelper::getHelper()->getAllGoods();
		for (unsigned int i = 0; i < Goods.size(); ++i)
		{
			if ( (Goods.at(i).id >= ID_Actor_Start && Goods.at(i).id <= ID_Actor_End)
				||  (Goods.at(i).id >= ID_Pet_Start && Goods.at(i).id <= ID_Pet_End))
				//||  (Goods.at(i).id >= ID_Magic_Start && Goods.at(i).id <= ID_Magic_End))
			{
				iCount++;
			}
		}
		int iTotal = ActorHelper::getHelper()->getAllHero().size()
			//+ActorHelper::getHelper()->getAllMagic().size()
			+ActorHelper::getHelper()->getAllPet().size();
		if (iCount >= iTotal)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_ShouCangJia, 1);
		}
	}
}

//升级回调
void NetactorLayer::callback_sj(EventCustom* evt){

	  
	  GameDirector::getDirector()->hideWaiting();
	  MsgData* data=(MsgData*)evt->getUserData();
	  if(data->errorCode==Rsp_Success){
				int id_h=PlayerInfoHelper::getHelper()->getDeafaultHero();
				hero=ActorHelper::getHelper()->getOneHero(id_h);
				if(isMaxlevel()==true){
					islevel=level_zero;
					auto p=BasicLayer::create();
					p->maojitishi(102);
					Director::getInstance()->getRunningScene()->addChild(p,100);
				}else{
					auto p=BasicLayer::create();
					p->buystr("sjcg","sjcgrenwumiaoshu");
					Director::getInstance()->getRunningScene()->addChild(p,100);
				}

			 FX::Music::getInstance()->playEffectMusic("u_level_up");
			 levalScucss();
			 
		     ActiveValueHelper::getHelper()->addActiveByType(activeType_JueSeShengJi);
			 if (ActorHelper::getHelper()->getMaxLevel(hero.unit.ID) == hero.level)
			 {
				 //检测高富帅成就
				 Achieve*info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_GaoFuShuai);
				 if (info && !info->isOver)
				 {
					 AchievementHelper::getHelper()->completeOneAchievement(achieveID_GaoFuShuai, 1);
				 }
		      }
		   StopLayer::checkFightPowerAchievement();
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

    }else if(data->errorCode==Rsp_Need_UnEnough){


	}else if(data->errorCode==Rsp_DontHaveThing){
										//没有该物品
	}else if(data->errorCode==Rsp_NotSupport){
			 auto basic=BasicLayer::create();
			 basic->buystr("xjsdk");
			 Director::getInstance()->getRunningScene()->addChild(basic,2000);
    }
}

void NetactorLayer::playAnimationByname(Node *bs,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscal,Vec2 p){

	auto armature =Armature::create(filename.c_str());
	armature->getAnimation()->play(animationName,-1,1);
	armature->removeFromParentAndCleanup(true);
	armature->setPosition(Vec2(0,getDelwithint(-150,1)));
	armature->setAnchorPoint(Vec2(0.5,0));
	armature->setTag(100);
	armature->setLocalZOrder(100);
	bs->addChild(armature);
//	if( animationName.compare("xx_sly")==0)
	{
		auto yinying_img=Sprite::create("touying_UI.png"); 
		yinying_img->setAnchorPoint(Vec2(0.5,0));
		yinying_img->setScaleX(1.5f);
		yinying_img->setPosition(Vec2(armature->getPositionX(),-armature->getContentSize().height/2+50));
		bs->addChild(yinying_img);
// 		xkongz.xx_action=armature;
// 		xkongz.time=0;
// 		xkongz.xxtime[0]=20;
// 		xkongz.xxtime[1]=1;
// 		xkongz.change=false;
		
	}
}

void NetactorLayer::callfunc_AnimateEndGk(Armature *ar, MovementEventType movementType, const std::string& movementID){
/*
	//xkongz.time++;
	if(movementType==MovementEventType::COMPLETE){
        
	    ar->getAnimation()->play("xx_ng");
		ar->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(NetactorLayer::callfunc_AnimateEndGk, 
		this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		//xkongz.time=0;

	}else if(movementType==MovementEventType::LOOP_COMPLETE){

// 		if(xkongz.time<xkongz.xxtime[0]){
// 			ar->getAnimation()->play("xx_ng");
// 			ar->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(NetactorLayer::callfunc_AnimateEndGk, 
// 				this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
// 		}else if(xkongz.time>=xkongz.xxtime[0]){
// 			ar->getAnimation()->play("xx_sly");
// 			ar->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(NetactorLayer::callfunc_AnimateEndGk, 
// 				this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
// 		    xkongz.time=0;
// 		}
	}*/
}


void NetactorLayer::fillOuty(MenuItemImage* p,string imgfile){
	 
	auto s=Sprite::create(imgfile);
	s->setScale(1.05f);
	p->setSelectedImage(s);
}

void NetactorLayer::drawMaojitishi(){

	auto pmj=Sprite::create(Vone_mjtishi_img);
	pmj->setPosition(Vec2(back_hero->getPositionX(),pmj->getContentSize().height/2));
	addChild(pmj);

	auto mj_m1=Label::createWithSystemFont(FX::StringsMap::getValue("renwuyimaoji"),"minijiankatong.ttf",24,Size(180,0),TextHAlignment::CENTER);
	mj_m1->setAnchorPoint(Vec2(0.5,0.5));
	mj_m1->setColor(color_y1);
	mj_m1->setPosition(Vec2(pmj->getContentSize().width/2,pmj->getContentSize().height*0.45));
	mj_m1->setLocalZOrder(100);
	pmj->addChild(mj_m1);

	auto mj2=Label::createWithSystemFont(FX::StringsMap::getValue("renwuyimaoji"),"minijiankatong.ttf",24,Size(180,0),TextHAlignment::CENTER);
	mj2->setAnchorPoint(Vec2(0.5,0.5));
	mj2->setPosition(Vec2(pmj->getContentSize().width/2,pmj->getContentSize().height*0.45-2));
	mj2->setLocalZOrder(90);
	pmj->addChild(mj2);

}

void NetactorLayer::levalScucss(){

	if(/*hero.unit.ID==unitID_Actor1 && */isMaxlevel()==false)
	{

		Vec2 sjv=getItemoney();
		string sj_yuan="";
		
		if(sjv.x==unitID_Gold||sjv.x==unitID_Diamond){
		    sj_yuan=__String::createWithFormat("%.0f",sjv.y)->getCString();
		}else{
			sj_yuan=__String::createWithFormat("%.0f",sjv.y)->getCString()+FX::StringsMap::getValue("chinsey");
			if(m_herodata.sj_p->getChildByTag(125)!=nullptr){
				m_herodata.sj_p->getChildByTag(125)->setVisible(false);
			}
		}
		
		Label* p1= (Label*)m_herodata.sj_p->getChildByTag(100);
		p1->setString(sj_yuan);

		Label* p2= (Label*)m_herodata.sj_p->getChildByTag(200);
		p2->setString(sj_yuan);
	   

		if(sjv.x==unitID_Gold||sjv.x==unitID_Diamond){

			string qname="UI_zhuye_shangdian_jinbi.png";
			if(sjv.x==unitID_Diamond){
				qname="UI_zhuye_shangdian_zuanshi.png";
			}

			p1->setAnchorPoint(Vec2(0,0.5));
			p1->setPosition(Vec2((m_herodata.sj_p->getContentSize().width-41)/2,m_herodata.sj_p->getContentSize().height*0.2));
			p2->setAnchorPoint(Vec2(0,0.5));
			p2->setPosition(Vec2(p1->getPositionX(),p1->getPositionY()-2));

			if(m_herodata.sj_p->getChildByTag(125)==nullptr){
				auto m_img=Sprite::create(qname);
				m_img->setPosition(Vec2(p1->getPositionX()+p1->getContentSize().width+m_img->getContentSize().width/2,p1->getPositionY()));
				m_img->setTag(125);
				m_herodata.sj_p->addChild(m_img);
			}else{
				m_herodata.sj_p->getChildByTag(125)->setVisible(true);
			}
		
		}else{
			p1->setAnchorPoint(Vec2(0.5,0.5));
			p1->setPosition(Vec2(m_herodata.sj_p->getContentSize().width*0.5,m_herodata.sj_p->getContentSize().height*0.2));
			p2->setAnchorPoint(Vec2(0.5,0.5));
			p2->setPosition(Vec2(p1->getPositionX(),p1->getPositionY()-2));
		}

	}

	
	if(isMaxlevel()){

		this->getChildByTag(Vone_sjmenu_tag)->setVisible(false);
		drawMaojitishi();
		auto max_img=Sprite::create(Vone_mjmark_img);
		max_img->setPosition(Vec2(back_hero->getContentSize().width*0.84,back_hero->getContentSize().height*0.80));
		back_hero->addChild(max_img);
	}


	if(isMaxlevel()){

		auto max_img2=Sprite::create(Vone_mjmark_img);
		max_img2->setPosition(Vec2(back_hero->getContentSize().width*0.84,back_hero->getContentSize().height*0.54));
		back_hero->addChild(max_img2);
	}


	if(isMaxlevel()){

		auto max_img1=Sprite::create(Vone_mjmark_img);
		max_img1->setPosition(Vec2(back_hero->getContentSize().width*0.84,back_hero->getContentSize().height*0.67));
		back_hero->addChild(max_img1);
	}




	ggl_label->setString(getHeroGjl(cId));
    string add_ggl="+ ";
	int ggc=std::stoi(getHeronextGjl(cId))-std::stoi(getHeroGjl(cId));
	add_ggl+=__String::createWithFormat("%d",ggc)->getCString();
	next_ggl->setString(add_ggl);

    if(isMaxlevel()==true){
		next_ggl->setString("");
	}


	int leve_hero=getXixi(cId);
	int next_level=leve_hero+1;

	if(ActorHelper::getHelper()->getMaxLevel(cId)==leve_hero){
		next_level=leve_hero;
	}
	for(int i=1;i<=ActorHelper::getHelper()->getMaxLevel(cId);i++){
		string p_img=Vone_x_img;
	
		if(leve_hero>=i)
				p_img=Vone_s_img;

		if(islevel==level_one){
			if(i==leve_hero){//i>leve_hero&&i<=next_level&&isMaxlevel()==false){
				djsps.at(i-1)->setTexture(Sprite::create(Vone_s_img)->getTexture());
				//djsps.at(i-1)->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr)));
			}else if(i==leve_hero){

				//djsps.at(i-1)->stopAllActions();
				//djsps.at(i-1)->setOpacity(255);
				//djsps.at(i-1)->setTexture(Sprite::create(Vone_s_img)->getTexture());
			}
		}
	}

	

	int sm_n=getSm(cId);
	int next_sm=getNextsm();

	for(int i=1;i<=getMaxsm();i++){
		string p_img=Vone_xuex_img;
	
		if(sm_n>=i)
				p_img=Vone_xues_img;

		if(islevel==level_one){
			if(i==sm_n){//i>sm_n&&i<=next_sm&&isMaxlevel()==false){
				helps.at(i-1)->setTexture(Sprite::create(Vone_xues_img)->getTexture());
				//helps.at(i-1)->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr)));
			}else if(i==sm_n){

				//helps.at(i-1)->stopAllActions();
				//helps.at(i-1)->setOpacity(255);
				//djsps.at(i-1)->setTexture(Sprite::create(Vone_s_img)->getTexture());
			}
		}
	}

	if(isMaxlevel()){
		
		auto p1=Sprite::create(Vone_s_img);
		auto p2=Sprite::create(Vone_xues_img);
		for(int i=1;i<=ActorHelper::getHelper()->getMaxLevel(cId);i++){
			djsps.at(i-1)->setTexture(p1->getTexture());
			//djsps.at(i-1)->stopAllActions();
			djsps.at(i-1)->setOpacity(255);

			helps.at(i-1)->setTexture(p2->getTexture());
			//helps.at(i-1)->stopAllActions();
			helps.at(i-1)->setOpacity(255);
		}
	}



	
}

void NetactorLayer::levalChange(){

		//攻击力
	string moliText=FX::StringsMap::getValue("moli");
	moliText+=":";
	auto gongj_img=Label::createWithSystemFont(moliText,"minijiankatong.ttf",40);//Sprite::create(Vone_gjl_img);
	gongj_img->setPosition(Vec2(back_hero->getContentSize().width*0.65,back_hero->getContentSize().height*0.80));
	gongj_img->setColor(color_h1);
    back_hero->addChild(gongj_img);

	ggl_label=Label::createWithSystemFont(getHeroGjl(cId),"minijiankatong.ttf",40);
	ggl_label->setAnchorPoint(Vec2(0,0.5));
	ggl_label->setColor(color_h1);
	ggl_label->setPosition(Vec2(gongj_img->getPositionX()+gongj_img->getContentSize().width/2+5,back_hero->getContentSize().height*0.80));
	ggl_label->setTag(Vone_ggl_label_tag);
	back_hero->addChild(ggl_label);


	if(islevel==level_one){
		int ggc=std::stoi(getHeronextGjl(cId))-std::stoi(getHeroGjl(cId));
		string add_ggl="+ ";
		add_ggl+=__String::createWithFormat("%d",ggc)->getCString();
		if(ggc==0){
			add_ggl="";
		}

		next_ggl=Label::createWithSystemFont(add_ggl,"minijiankatong.ttf",40);
		next_ggl->setAnchorPoint(Vec2(0,0.5));
		next_ggl->setColor(color_h1);
		next_ggl->setPosition(Vec2(ggl_label->getPositionX()+ggl_label->getContentSize().width+5,back_hero->getContentSize().height*0.80));
		next_ggl->setTag(Vone_nextggl_label_tag);
		next_ggl->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr)));
		back_hero->addChild(next_ggl);
	}

	
   

    //等级
	auto dj_label=Sprite::create("UI_jiaose_shuxing_dengji_wenzi.png");
	dj_label->setAnchorPoint(Vec2(0,0.5));
	dj_label->setColor(color_b1);
	dj_label->setPosition(Vec2(back_hero->getContentSize().width*0.15,back_hero->getContentSize().height*0.70));
	dj_label->setTag(Vone_dj_label_tag);
	back_hero->addChild(dj_label);

	int leve_hero=getXixi(cId);
	int next_level=leve_hero+1;
	if(ActorHelper::getHelper()->getMaxLevel(cId)==leve_hero){
		next_level=leve_hero;
	}
	int maxk=ActorHelper::getHelper()->getMaxLevel(cId);
	if(djsps.size()>0){
		djsps.clear();
	}

	for(int i=1;i<=maxk;i++){
		string p_img=Vone_s_img;
		//Vone_x_img;
		if(i>leve_hero){//i>next_level){
			p_img=Vone_x_img;
		}

		//if(i==next_level&&islevel==level_zero&&isMaxlevel()==false){
		//	p_img=Vone_x_img;
		//}


		auto p=Sprite::create(p_img);
		float xp=p->getContentSize().width*0.8;
		p->setScale(0.8f);
		p->setPosition(Vec2(dj_label->getPositionX()+dj_label->getContentSize().width+(i-1)*xp+xp/2,
			                back_hero->getContentSize().height*0.70));

		if(islevel==level_one){
			if(i>leve_hero&&i<=next_level){
				//p->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr)));
			}
		
		}


		djsps.push_back(p);
		back_hero->addChild(p);


	}

	
	//生命 Vone_xues_img  Vone_xuex_img
	auto x_label=Sprite::create("UI_jiaose_shuxing_shengming_wenzi.png");//Label::createWithSystemFont(FX::StringsMap::getValue("xuel"),"minijiankatong.ttf",38);
	x_label->setAnchorPoint(Vec2(0,0.5));
	x_label->setColor(color_b1);
	x_label->setPosition(Vec2(back_hero->getContentSize().width*0.15,back_hero->getContentSize().height*0.60));
	x_label->setTag(Vone_x_label_tag);
	back_hero->addChild(x_label);
	if(helps.size()>0){
		helps.clear();
	}

    int sm_n=getSm(cId);
	int next_sm=getNextsm();
	for(int i=1;i<=getMaxsm();i++){
		string p_img=Vone_xues_img;
		if(i>sm_n){//>next_sm)
			p_img=Vone_xuex_img;
		}
		//if(i==next_sm&&islevel==level_zero&&isMaxlevel()==false){
			//p_img=Vone_xuex_img;
		//}
		auto p=Sprite::create(p_img);
		p->setScale(0.7f);
		float m=p->getContentSize().width*0.7;
		p->setPosition(Vec2(x_label->getPositionX()+x_label->getContentSize().width+(i-1)*m+m/2,
			                back_hero->getContentSize().height*0.60));
		if(islevel==level_one){
			if(next_sm>=i&&i>sm_n){
				//p->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr)));
			}
		}
		back_hero->addChild(p);
		helps.push_back(p);
	}
	if (isUnlockhero(cId)==false)
	{
		//碎片
		auto suipian_label=Sprite::create("UI_jiaose_shuxing_suipian.png");
		suipian_label->setAnchorPoint(Vec2(0,0.5));
		suipian_label->setColor(color_b1);
		suipian_label->setPosition(Vec2(back_hero->getContentSize().width*0.15,back_hero->getContentSize().height*0.52));
		suipian_label->setTag(Vone_suipian_label_tag);
		back_hero->addChild(suipian_label);
		gold_suipian =ActorHelper::getHelper()->getUnlockHeroNeedFragmentAndGold(cId); 
		int suipian_max = gold_suipian.first;
		int suipian_num = getSuipian((UnitID)hero.unit.ID);
		float percent_suipian = suipian_num/suipian_max;

		//进度条
		auto suipian_Boarder =Sprite::create("UI_chongwu_jindu_bg.png");
		suipian_Boarder->setAnchorPoint(Vec2(0,0.5));
		back_hero->addChild(suipian_Boarder);
		suipian_Boarder->setPosition(Vec2(suipian_label->getPositionX()+suipian_label->getContentSize().width,
								back_hero->getContentSize().height*0.52));
		suipian_progress = ProgressTimer::create(Sprite::create("UI_chongwu_jindu.png"));
		suipian_progress->setType(ProgressTimer::Type::BAR);
		suipian_progress->setMidpoint(Vec2(0,0));
		suipian_progress->setBarChangeRate(Vec2(1, 0));
		suipian_progress->setPosition(Vec2(suipian_Boarder->getContentSize().width/2, suipian_Boarder->getContentSize().height/2));
		suipian_Boarder->addChild(suipian_progress);
		suipian_progress->setPercentage(percent_suipian);

		//进度条上文字
		std::string  percent_string =  __String::createWithFormat("%d/%d",suipian_num,suipian_max)->getCString();
		auto suipian_percent_label=Label::createWithSystemFont(percent_string,"minijiankatong.ttf",24);
		suipian_percent_label->setPosition(Vec2(suipian_progress->getContentSize().width/2,suipian_progress->getContentSize().height/2));
		suipian_progress->addChild(suipian_percent_label);
		//suipian_Boarder->setVisible(false);
		//suipian_label->setVisible(false);
		//suipian_progress->setVisible(false);
	}
}


void NetactorLayer::levelDraw(){

	

	auto ret_img=MenuItemImage::create(Vone_return_mainlayer_img,Vone_return_mainlayer_img,CC_CALLBACK_1(NetactorLayer::allcallback,this));
	ret_img->setTag(Vone_return_button_tag);
	ret_img->setPosition(Vec2(back_hero->getContentSize().width-ret_img->getContentSize().width/2,back_hero->getContentSize().height-ret_img->getContentSize().height/4));
	fillOuty(ret_img,Vone_return_mainlayer_img);


	auto remenu=Menu::create(ret_img,nullptr);
	remenu->setPosition(Vec2::ZERO);
	back_hero->addChild(remenu);

	//姓名
	auto h_name=Label::createWithSystemFont(hero.unit.name,"minijiankatong.ttf",40);
	h_name->setAnchorPoint(Vec2(0,0.5));
	h_name->setPosition(Vec2(back_hero->getContentSize().width*0.15,back_hero->getContentSize().height*0.80));
	h_name->setColor(color_h1);
	back_hero->addChild(h_name);

	if(isUnlockhero(cId)==false){

		auto s_img=Sprite::create(suo_kuanimg);
		s_img->setPosition(Vec2(h_name->getPositionX()+h_name->getContentSize().width+s_img->getContentSize().width/2,
			                    h_name->getPositionY()));
		back_hero->addChild(s_img);
	}

    

    //描述
	string tobeCut = hetheromisoshu(cId);
	string cutStrin;
	int cPos = -1;
	cPos= tobeCut.find('[',0);
	if(cPos != -1){
		cutStrin = tobeCut.substr(cPos+1,tobeCut.size());
		tobeCut.erase(cPos,tobeCut.size());
	}
	auto miaoshu_label=Label::createWithSystemFont(tobeCut,"minijiankatong.ttf",24,Size(400,0),TextHAlignment::LEFT);
	miaoshu_label->setAnchorPoint(Vec2(0.5,0.5));
	miaoshu_label->setPosition(Vec2(back_hero->getContentSize().width*0.5,back_hero->getContentSize().height*0.3));
	back_hero->addChild(miaoshu_label);
	if (cutStrin.size() != 0 )
	{
		auto miaoshu2_label=Label::createWithSystemFont(cutStrin,"minijiankatong.ttf",24,Size(400,0),TextHAlignment::LEFT);
		miaoshu2_label->setAnchorPoint(Vec2(0.5,1));
		miaoshu2_label->setColor(Color3B(160,250,75));
		miaoshu2_label->setPosition(Vec2(back_hero->getContentSize().width*0.5,back_hero->getContentSize().height*0.3-miaoshu_label->getContentSize().height/2));
		back_hero->addChild(miaoshu2_label);
	}


	if(isMaxlevel()){

		auto max_img=Sprite::create(Vone_mjmark_img);
		max_img->setPosition(Vec2(back_hero->getContentSize().width*0.84,back_hero->getContentSize().height*0.80));
		back_hero->addChild(max_img);
	}


	if(isMaxlevel()){

		auto max_img2=Sprite::create(Vone_mjmark_img);
		max_img2->setPosition(Vec2(back_hero->getContentSize().width*0.84,back_hero->getContentSize().height*0.70));
		back_hero->addChild(max_img2);
	}


	if(isMaxlevel()){

		auto max_img1=Sprite::create(Vone_mjmark_img);
		max_img1->setPosition(Vec2(back_hero->getContentSize().width*0.84,back_hero->getContentSize().height*0.60));
		back_hero->addChild(max_img1);
	}

}



//herolistlayer
Herolist::Herolist(){

}

bool Herolist::init(){

	if(!Layer::init()){
		return false;
	}

	 heros.clear();
	 vector<SaleItem> tls=(vector<SaleItem>)*MarketHelper::getHelper()->getAllSaleItems();
		int m_size=tls.size();
		for(int i=0;i<m_size;i++){
			if(GoodsHelper::getHelper()->getUnitByID(tls[i].unitSaleID)->type==unittype_Actor){
				heros.push_back(tls[i]);
			}
	 }

    size=Size(120,130);
	mTableView = TableView::create(this,Size(120*(heros.size()),130));
	this->addChild(mTableView,2);
	mTableView->setDirection(ScrollView::Direction::HORIZONTAL);
	mTableView->setPosition(Vec2(0,0));
	mTableView->setDelegate(this);
	mTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);

	return true;
}

void Herolist::onEnter(){
	
	Layer::onEnter();
}

void Herolist::onExit(){
   
	Layer::onExit();
}

void Herolist::tableCellTouched(TableView* table, TableViewCell* cell){


	 
}

Size Herolist::tableCellSizeForIndex(TableView *table, ssize_t idx){

	 
	 return Size(120,130);

}

TableViewCell*  Herolist::tableCellAtIndex(TableView *table, ssize_t idx){

	
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
	drawheros(cell,idx);
	return cell;
}


ssize_t Herolist::numberOfCellsInTableView(TableView *table){

	return heros.size();
}


void Herolist::drawheros(TableViewCell *cell, ssize_t idx){

	Actor hero = ActorHelper::getHelper()->getOneHero(heros.at(idx).unitSaleID);

	if(ActorHelper::getHelper()->getIsUnlockItem(heros.at(idx).unitSaleID)){
		 auto back=MenuItemImage::create(suo_bg,suo_bg,CC_CALLBACK_1(Herolist::all_callback,this));
		 back->setTag(heros.at(idx).unitSaleID);
		 back->setPosition(Vec2(size.width/2,size.height/2));
		
		 auto toux=Sprite::create(hero.unit.imageFile);
		 toux->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height/2));
		 back->addChild(toux);
		 auto menu=Menu::create(back,nullptr);
		 menu->setPosition(Vec2::ZERO);
         cell->addChild(menu);
	 }else{
		 auto back=MenuItemImage::create(suo_bg,suo_bg,CC_CALLBACK_1(Herolist::all_callback,this));
		 back->setTag(heros.at(idx).unitSaleID);
		 //back->setOpacity(80);
		 back->setPosition(Vec2(size.width/2,size.height/2));

		 auto toux=Sprite::create(hero.unit.imageFile);
		 toux->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height/2));
		 back->addChild(toux);
		 auto sd_img=Sprite::create(suo_kuanimg);
		 sd_img->setPosition(Vec2(back->getContentSize().width-sd_img->getContentSize().width/2,sd_img->getContentSize().height/2));
		 back->addChild(sd_img);

         auto menu=Menu::create(back,nullptr);
		 menu->setPosition(Vec2::ZERO);
         cell->addChild(menu);
	 }
}

void Herolist::all_callback(Ref* ref){

	 //id 传出去
	FX::Music::getInstance()->buttonClicked();
	MenuItemImage* p=(MenuItemImage*)ref;
	current_oprate=p->getTag();
    bAc(p);
}

void Herolist::bAc(MenuItemImage* node){

	node->runAction(	
		             Sequence::create(ScaleTo::create(0.1f,1.2f),CallFuncN::create( CC_CALLBACK_0(Herolist::callp,
							  this,node,true)),nullptr)
					);
	 
}
void Herolist::callp(Node* node,bool mr){

   node->setScale(1.0f);

   EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
   EventCustom event(Msg_id);
   MsgData data;
   data.value =current_oprate;
   event.setUserData(&data);
   eventDspc->dispatchEvent(&event);
}

Herolist::~Herolist(){

}



