#include "Netactorup.h"
#include "ActiveValueHelper.h"
#include "StopLayer.h"

Netactorup::Netactorup(void)
{
}


Netactorup::~Netactorup(void)
{
}


//gouzhanqu


bool Netactorup::init(){

	if(!Layer::init()){
		return false;
	}

	 
	return true;

}
void Netactorup::onEnter(){

    auto callBack       =  [&](EventCustom* evt)  
                            {  
								this->callback_sj(evt);
                            };  
	sj=EventListenerCustom::create(MSG_LevelUp_Rsp,callBack);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(sj,1); 
	Layer::onEnter();

}
void Netactorup::onExit(){

	Director::getInstance()->getEventDispatcher()->removeEventListener(sj);
	Layer::onExit();
}

float Netactorup::getDelwithint(float number,int iswidth){
    
	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;
}


void Netactorup::setactorId(int id){
	 
	 
	 Size visibleSize = Director::getInstance()->getWinSize();
	 Actual_x=visibleSize.width;
	 Actual_y=visibleSize.height;
	  //设置大小
	 this->setContentSize(Size(Actual_x,Actual_y-getDelwithint(Tophight,1)));
	 actorid=id;
	 hero=ActorHelper::getHelper()->getOneHero(id);
	 current_herolevel=hero.level;
	 inintjiemian();



}


void Netactorup::inintjiemian(){
	 
	 
	this->removeAllChildren();

	//left
	auto scal9_l=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9_l->setContentSize(Size(499,395));
	scal9_l->setPosition(Vec2(getContentSize().width*0.025+scal9_l->getContentSize().width/2,getContentSize().height*0.6));
	addChild(scal9_l,1);

	auto ts_l=Sprite::create("ui_sence_zanting_bg_tiao.png");
	ts_l->setPosition(Vec2(scal9_l->getContentSize().width/2,scal9_l->getContentSize().height));
	scal9_l->addChild(ts_l,100);

	auto l_la=Label::createWithSystemFont(FX::StringsMap::getValue("dqsx"),"Arial",24);
	l_la->setPosition(Vec2(ts_l->getContentSize().width/2,ts_l->getContentSize().height/2));
	ts_l->addChild(l_la);

	drawSx(scal9_l,hero.level);


	//center
	auto c_img=Sprite::create("ui_zhuye_guanqiamoshi_guanqia_anniu_fanye.png");
	c_img->setPosition(Vec2(scal9_l->getPositionX()+scal9_l->getContentSize().width/2+c_img->getContentSize().width,scal9_l->getPositionY()));
	addChild(c_img);

	//right
	auto scal9_r=Scale9Sprite::create("ui_zhuye_daojukuang.png");
	scal9_r->setContentSize(Size(499,395));
	scal9_r->setPosition(Vec2(scal9_l->getContentSize().width/2+scal9_l->getPositionX()+90+scal9_r->getContentSize().width/2,getContentSize().height*0.6));
	addChild(scal9_r,1);

	auto ts_r=Sprite::create("ui_sence_zanting_bg_tiao.png");
	ts_r->setPosition(Vec2(scal9_r->getContentSize().width/2,scal9_r->getContentSize().height));
	scal9_r->addChild(ts_r,100);

	auto r_la=Label::createWithSystemFont(FX::StringsMap::getValue("sjhsx"),"Arial",24);
	r_la->setPosition(Vec2(ts_r->getContentSize().width/2,ts_r->getContentSize().height/2));
	ts_r->addChild(r_la);
	int dj=hero.level;
	if(ActorHelper::getHelper()->getMaxLevel(hero.unit.ID)>dj){
		dj=dj+1;
	}
	drawSx(scal9_r,dj);


	//yijianmaoji
	auto yjmj_b=MenuItemImage::create("ui_anniu_tongyong_goumai.png","ui_anniu_tongyong_goumai_select.png",CC_CALLBACK_1(Netactorup::yjmjCallback,this));
	yjmj_b->setPosition(Vec2(scal9_l->getPositionX(),getContentSize().height*0.1));

	auto mj_l=Label::createWithSystemFont(FX::StringsMap::getValue("yijiaomj2"),"Arial",24);
	mj_l->setPosition(Vec2(yjmj_b->getContentSize().width/2,yjmj_b->getContentSize().height-mj_l->getContentSize().height/2-5));
	yjmj_b->addChild(mj_l);

	string str_y=__String::createWithFormat("%d",getMjyuan())->getCString()+FX::StringsMap::getValue("yuan");
	auto y_label=Label::createWithSystemFont(str_y,"Arial",24);
	y_label->setPosition(Vec2(yjmj_b->getContentSize().width/2,5+y_label->getContentSize().height/2));
	yjmj_b->addChild(y_label);

	//shenji
	auto yjmj_s=MenuItemImage::create("ui_anniu_tongyong_goumai.png","ui_anniu_tongyong_goumai_select.png",CC_CALLBACK_1(Netactorup::sjCallback,this));
	yjmj_s->setPosition(Vec2(scal9_r->getPositionX(),getContentSize().height*0.1));

	auto sj_l=Label::createWithSystemFont(FX::StringsMap::getValue("sj2"),"Arial",24);
	sj_l->setPosition(Vec2(yjmj_s->getContentSize().width/2,yjmj_s->getContentSize().height-sj_l->getContentSize().height/2-5));
	yjmj_s->addChild(sj_l);

	string str_ys=__String::createWithFormat("%d",getsjyuan())->getCString()+FX::StringsMap::getValue("yuan");
	auto s_label=Label::createWithSystemFont(str_ys,"Arial",24);
	s_label->setPosition(Vec2(yjmj_s->getContentSize().width/2,5+s_label->getContentSize().height/2));
	yjmj_s->addChild(s_label);


	auto menu=Menu::create(yjmj_b,yjmj_s,nullptr);
	menu->setPosition(Vec2(0,0));
	addChild(menu);


}

void Netactorup::drawSx(Node* node,int dj){

	 
	for(int i=0;i<3;i++){

		
		auto img_bk=Sprite::create("ui_chengjiu_weiwancheng.png");
		float x=node->getContentSize().width/2;
		float y=node->getContentSize().height-img_bk->getContentSize().height/2-20;
		img_bk->setPosition(Vec2(x,y-(getDelwithint(123,1))*i));
		node->addChild(img_bk,100);

		string name="xj";
		string bx_imgstr="xixi.png";
		
		
		

		if(i==0){
			name="xj";
			bx_imgstr="xixi.png";
			int xj_num=0;
			xj_num=getxj(dj);
			
			for(int j=0;j<xj_num;j++){
				auto img=Sprite::create(bx_imgstr);
				float x=img_bk->getContentSize().width*0.2+img->getContentSize().width/2;
				float y=img_bk->getContentSize().height*0.4;
				img->setPosition(Vec2(x+50*j,y));
				img_bk->addChild(img,100);
			}
		}else if(i==1){
			name="sm";
			bx_imgstr="xixi.png";
			int sm_num=0;
			sm_num=getsm(dj);
			
			for(int j=0;j<sm_num;j++){
				auto img=Sprite::create(bx_imgstr);
				float x=img_bk->getContentSize().width*0.2+img->getContentSize().width/2;
				float y=img_bk->getContentSize().height*0.4;
				img->setPosition(Vec2(x+50*j,y));
				img_bk->addChild(img,100);
			}
		}else if(i==2){
			name="jc";
			bx_imgstr="xixi.png";
			string jc_num;
			jc_num=getjc(dj);

			/*
			for(int j=0;j<jc_num;j++){
				auto img=Sprite::create(bx_imgstr);
				float x=img_bk->getContentSize().width*0.2+img->getContentSize().width/2;
				float y=img_bk->getContentSize().height*0.4;
				img->setPosition(Vec2(x+50*j,y));
				img_bk->addChild(img,100);
			}
			*/

			auto label=Label::createWithSystemFont(jc_num,"Arial",24,Size(img_bk->getContentSize().width-30,0),TextHAlignment::CENTER);
			label->setColor(Color3B(255,33,33));
			label->setPosition(Vec2(img_bk->getContentSize().width/2,img_bk->getContentSize().height*0.4));
			img_bk->addChild(label,100);
		}
		auto su_l=Label::createWithSystemFont(FX::StringsMap::getValue(name),"Arial",24);
		su_l->setPosition(Vec2(su_l->getContentSize().width/2+25,img_bk->getContentSize().height*0.725));
		img_bk->addChild(su_l);
	}
	 
}



void Netactorup::yjmjCallback(Ref* ref){
	 
	 
	int culevel=hero.level;
	int max=ActorHelper::getHelper()->getMaxLevel(hero.unit.ID);

	for(int i=1;i<=max-culevel;i++){
	    ActorHelper::getHelper()->levelUp(hero.unit.ID);
	}
	hero=ActorHelper::getHelper()->getOneHero(PlayerInfoHelper::getHelper()->getDeafaultHero());
    inintjiemian();	
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


	 
}

void Netactorup::sjCallback(Ref* ref){
	 



	ActorHelper::getHelper()->levelUp(hero.unit.ID);
	/*
	const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit->ID,hero.level);
	
	int type=curent_info->needUnit->ID;
	int need=curent_info->needCount;
    
	bool iss=false;
	
	if(type==unitID_Gold){

		if(GoodsHelper::getHelper()->getGoldNum()<need){
			BaseLayer* shop=BaseLayer::create();
			shop->money_no(unitID_Gold);
	        Director::getInstance()->getRunningScene()->addChild(shop);
		}else{
			 ActorHelper::getHelper()->levelUp(hero.unit->ID);
			 hero=ActorHelper::getHelper()->getOneHero(PlayerInfoHelper::getHelper()->getDeafaultHero());
			 iss=true;
             inintjiemian();	 
		}
	}else{
		if(GoodsHelper::getHelper()->getJewelNum()<need){
			     BaseLayer* shop=BaseLayer::create();
				 shop->money_no(unitID_Diamond);
	             Director::getInstance()->getRunningScene()->addChild(shop);
		}else{
			ActorHelper::getHelper()->levelUp(hero.unit->ID);
			int id_h=PlayerInfoHelper::getHelper()->getDeafaultHero();
			hero=ActorHelper::getHelper()->getOneHero(id_h);
			iss=true;
			inintjiemian();
		}
	}
	*/

	 
	 
}


int Netactorup::getMjyuan(){

	int mj_money=12;

	return mj_money;
}

int Netactorup::getsjyuan(){

	int sj_money=3;

	return sj_money;
}


int Netactorup::getxj(int dj){

	 return dj;
}

int Netactorup::getsm(int dj){

	 
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit.ID,dj);
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

string Netactorup::getjc(int dj){


	const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(hero.unit.ID,dj);
	 vector<Attribute> attrVec=curent_info->attrVec;
	 float value=0;
	 string attr="";
	 string allstr="";
	 for(size_t i=0;i<attrVec.size();i++){

		 if(attrVec.at(i).attr.compare(hp)!=0&&attrVec.at(i).attr.compare(power)!=0){
			 value=attrVec.at(i).value;
			 attr=attrVec.at(i).attr;
			 allstr+=attr+":"+__String::createWithFormat("%.0f",value*100)->getCString()+"%"+" ";
		 } 
	 }
	return allstr;
}

void Netactorup::callback_sj(EventCustom* evt){

	  MsgData* data=(MsgData*)evt->getUserData();
	  if(data->errorCode==Rsp_Success){

		     
			int id_h=PlayerInfoHelper::getHelper()->getDeafaultHero();
			hero=ActorHelper::getHelper()->getOneHero(id_h);
			inintjiemian();  

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
	  }else{
		CCLOG("--->shop");
	 }

}