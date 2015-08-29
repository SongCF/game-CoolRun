#include "Netpet.h"
#include "ActiveValueHelper.h"
#include "StopLayer.h"
#include "BasicLayer.h"


#define pet_suo_bg         "UI_jiaose_qiehuanrenwu_bg.png"
#define pet_suokuan_bg     "UI_jiaose_qiehuanrenwu_suoding.png"
#define Pet_biaoti_img     "UI_chongwu_biaoti.png" 
#define Pet_back_img       "UI_jiaose_bg.png"
#define pet_close_img       "UI_jiaose_anniu_guanbi.png"
#define pet_shouji_img      "UI_chongwu_anniu_qushouji.png"
#define pet_weiyang_img     "UI_chongwu_anniu_weiyang.png"  
#define pet_jindu_kuan_img  "UI_chongwu_jindu_bg.png"
#define pet_jindu2_kuan_img "UI_chongwu_jindu.png"
#define pet_dengji_img      "UI_jiaose_shuxing_dengji_wenzi.png"

#define  Msgpet_id  "petchangid"


enum Pettag{

	 pet_return_button_tag,
	 pet_shouji_pet_tag,
	 pet_wy_pet_tag,
	 pet_list_tag,
	 pet_food_add_tag,
	 pet_wy_pet_tag_menu,
	 maoji_sprite_tag,
	 aramute_tag,

};

enum LayerState{

	nochange=0,
	change,

};


Netpet::Netpet(void)
{

}


Netpet::~Netpet(void)
{
}

bool Netpet::init(){

	if(!Layer::init()){
		return false;
	}
	x=0;
	l_is=false;
	initjiemian();
	return true;

}
void Netpet::onEnter(){

	auto callBack_id      =  [&](EventCustom* evt)  
                            {  
								MsgData* data=(MsgData*)evt->getUserData();
								cId=data->value;

								if(cId==pet.unit.ID){
									return;
								}

							
							    	
								for(auto mp:pets){
									if(mp.unit.ID==data->value){
										if(ActorHelper::getHelper()->getIsUnlockItem(mp.unit.ID)){
										   pet=ActorHelper::getHelper()->getOnePet(mp.unit.ID);
										   break;
										}else{
										   pet=mp;
										   break;
										}
									}
								}

								CCLOG("pet--descible-->%s",pet.unit.descrigbe.c_str());
								x=0;
								levelChange(change);
								
                            };  
	auto sj_id=EventListenerCustom::create(Msgpet_id,callBack_id);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(sj_id,1);  
	netpetsevents.push_back(sj_id);

	auto callBack       =  [&](EventCustom* evt)  
                            {  
								
								
							    if(netchange_sucess.laysers_mark==change&&netchange_sucess.layers.at(0)->isVisible()==true){
									int num=0;
									for(int i=0;i<3;i++){
										if(i==0){
											num=GoodsHelper::getHelper()->getDaogaiNum();
											netchange_sucess.num_food[0]->setString(__String::createWithFormat("%d",num)->getCString());
										}else if(i==1){
											num=GoodsHelper::getHelper()->getTangguoNum();
											netchange_sucess.num_food[1]->setString(__String::createWithFormat("%d",num)->getCString());
										}else if(i==2){
											num=GoodsHelper::getHelper()->getGuodongNum();
											netchange_sucess.num_food[2]->setString(__String::createWithFormat("%d",num)->getCString());
										}
									}
								}
                            };  
	auto Gold_changevent=EventListenerCustom::create(MSG_BagDataChanged,callBack);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(Gold_changevent,1);
	netpetsevents.push_back(Gold_changevent);


	auto callBack_sj       =  [&](EventCustom* evt)  
                            {  
								this->toolsdata(evt);
                            };  
	auto sj=EventListenerCustom::create(MSG_Feed_Rsp,callBack_sj);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(sj,1); 
	netpetsevents.push_back(sj);

	Layer::onEnter();
}
void Netpet::onExit(){

	for(auto m:netpetsevents){
		Director::getInstance()->getEventDispatcher()->removeEventListener(m);
	}
	Layer::onExit();
}

float Netpet::getDelwithint(float number,int iswidth){
    
	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/960);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;
}



void Netpet::initjiemian(){


	 Size visibleSize = Director::getInstance()->getWinSize();
	 Actual_x=visibleSize.width;
	 Actual_y=visibleSize.height;
	 this->setContentSize(Size(Actual_x,Actual_y));
	 inintData();
     topjiemian();	 
	 drawPetlist();
}


void Netpet::topjiemian(){


	netchange_sucess.back_hero=Sprite::create(Pet_back_img);
	auto back_biti=Sprite::create(Pet_biaoti_img);
	back_biti->setPosition(Vec2(Actual_x-netchange_sucess.back_hero->getContentSize().width/2,Actual_y-back_biti->getContentSize().height/2));
	back_biti->setLocalZOrder(100);
	addChild(back_biti);

	netchange_sucess.back_hero->setPosition(Vec2(Actual_x-netchange_sucess.back_hero->getContentSize().width/2,
		                        Actual_y-back_biti->getContentSize().height-netchange_sucess.back_hero->getContentSize().height/2+40));
	addChild(netchange_sucess.back_hero);
	
	levaDarw();

	if(isUnlockpet()==false&&pet.unit.ID==unitID_PiPiZhu){

		auto  b=MenuItemImage::create(pet_shouji_img,pet_shouji_img,CC_CALLBACK_1(Netpet::allcallback,this)); 
		b->setTag(pet_shouji_pet_tag);
		b->setPosition(Vec2(netchange_sucess.back_hero->getPositionX(),b->getContentSize().height/2+20));
		fillOuty(b,pet_shouji_img);

		auto menu=Menu::create(b,nullptr);
		menu->setPosition(Vec2(0,0));
		addChild(menu);
	}else if(isUnlockpet()&&isMaxlevel()==true){//满级

		drawMaojimenu();

	}else if(isUnlockpet()&&isMaxlevel()==false){//喂养

	   netchange_sucess.wy_mark=change;
	   auto  wy=MenuItemImage::create(pet_weiyang_img,pet_weiyang_img,CC_CALLBACK_1(Netpet::allcallback,this)); 
	   wy->setTag(pet_wy_pet_tag);
	   wy->setPosition(Vec2(netchange_sucess.back_hero->getPositionX(),wy->getContentSize().height/2+20));
	   fillOuty(wy,pet_weiyang_img);

	   auto menu=Menu::create(wy,nullptr);
	   menu->setPosition(Vec2(0,0));
	   menu->setTag(pet_wy_pet_tag_menu);
	   addChild(menu);
	}

	//画头像
	Layer* pet_l=Layer::create();
	pet_l->setPosition(Vec2(getContentSize().width*0.325,getContentSize().height*0.35));
	addChild(pet_l,1000);
	pet_l->setTag(aramute_tag);
	refreshPetImage(pet_l, pet.unit.ID);
}

void Netpet::refreshPetImage(Node* node, unsigned int petId)
{
	node->removeAllChildren();
	if(petId==unitID_PiPiZhu){
		playAnimationByname(node,"ppz_ng.ExportJson","pzz_ng","pzz_ng",true);
	}else if (petId==unitID_XiXiaoYang){
		playAnimationByname(node,"xxy_ng.ExportJson","xxy_ng","xxy_ng",true);
	}else if (petId==unitID_BuGuNiao){
		playAnimationByname(node,"bgn_ng.ExportJson","bgn_ng","bgn_ng",true);
	}else if (petId==unitID_ZhanHongLong){
		playAnimationByname(node,"zhl_ng.ExportJson","zhl_ng","zhl_ng",true);
	}
}


void Netpet::drawMaojimenu(){
	    
	    netchange_sucess.mj_mark=change;
	    auto pmj=Sprite::create("UI_jiaose_anniu_shengji_manji.png");
		pmj->setPosition(Vec2(netchange_sucess.back_hero->getPositionX(),pmj->getContentSize().height/2));
		pmj->setTag(maoji_sprite_tag);
		addChild(pmj);

		auto mj_m1=Label::createWithSystemFont(FX::StringsMap::getValue("petyimaoji"),"minijiankatong.ttf",24,Size(180,0),TextHAlignment::CENTER);
		mj_m1->setAnchorPoint(Vec2(0.5,0.5));
		mj_m1->setColor(Color3B(184,69,13));
		mj_m1->setPosition(Vec2(pmj->getContentSize().width/2,pmj->getContentSize().height*0.45));
		mj_m1->setLocalZOrder(100);
		pmj->addChild(mj_m1);

		auto mj2=Label::createWithSystemFont(FX::StringsMap::getValue("petyimaoji"),"minijiankatong.ttf",24,Size(180,0),TextHAlignment::CENTER);
		mj2->setAnchorPoint(Vec2(0.5,0.5));
		mj2->setPosition(Vec2(pmj->getContentSize().width/2,pmj->getContentSize().height*0.45-2));
		mj2->setLocalZOrder(90);
		pmj->addChild(mj2); 
}

void Netpet::levelChange(int tag){ 



	

	netchange_sucess.h_name->setString(pet.unit.name);

	if(isUnlockpet()==false){

		if(netchange_sucess.suo_mark==nochange){
			netchange_sucess.suo_mark=change;
			netchange_sucess.suo_img=Sprite::create(pet_suokuan_bg);
			netchange_sucess.suo_img->setScale(0.7f);
			netchange_sucess.suo_img->setPosition(Vec2(netchange_sucess.h_name->getPositionX()+netchange_sucess.h_name->getContentSize().width+netchange_sucess.suo_img->getContentSize().width*0.7f/2,
									  netchange_sucess.back_hero->getContentSize().height*0.825));
			netchange_sucess.back_hero->addChild(netchange_sucess.suo_img);
			//netchange_sucess.suo_img->setVisible(false);

		}else if(netchange_sucess.suo_mark==change){

			netchange_sucess.suo_img->setVisible(true);
		}

	}else if(isUnlockpet()==true){

		refreshPetImage(this->getChildByTag(aramute_tag), pet.unit.ID);

		if(netchange_sucess.suo_mark==change){
		   netchange_sucess.suo_img->setVisible(false);
		}

	}

	
	if(isMaxlevel()==false){
		Needunit need=getneedUnit();
		if(netchange_sucess.need_mark==nochange){

			netchange_sucess.need_mark=change;
			netchange_sucess.need_img=Sprite::create(need.imgfile);
			netchange_sucess.need_img->setPosition(Vec2(netchange_sucess.back_hero->getContentSize().width*0.55,
				netchange_sucess.back_hero->getContentSize().height*0.825));
			netchange_sucess.back_hero->addChild(netchange_sucess.need_img);
			netchange_sucess.kuang_img=Sprite::create(pet_jindu_kuan_img);
			netchange_sucess.kuang_img->setPosition(Vec2(netchange_sucess.need_img->getPositionX()+netchange_sucess.need_img->getContentSize().width/2+netchange_sucess.kuang_img->getContentSize().width/2,
				netchange_sucess.back_hero->getContentSize().height*0.825));
			netchange_sucess.back_hero->addChild(netchange_sucess.kuang_img);
		}else{
			netchange_sucess.need_img->setTexture(Sprite::create(need.imgfile)->getTexture());
			netchange_sucess.need_img->setVisible(true);

		}



		string str_pr;
		if(isUnlockpet()==true){
			str_pr=__String::createWithFormat("%d",getCujd())->getCString();
			str_pr+="/";
			str_pr+=__String::createWithFormat("%d",need.count)->getCString();
		}else{
			str_pr="? ? ?";
		}

		if(netchange_sucess.pecent_mark==nochange){
			netchange_sucess.pecent_mark=change;
			netchange_sucess.percent_label=Label::createWithSystemFont(str_pr,"minijiankatong.ttf",24);
			netchange_sucess.percent_label->setPosition(Vec2(netchange_sucess.kuang_img->getContentSize().width/2,netchange_sucess.kuang_img->getContentSize().height/2));
			netchange_sucess.kuang_img->addChild(netchange_sucess.percent_label,120);
		}
		netchange_sucess.percent_label->setVisible(true);
		netchange_sucess.percent_label->setString(str_pr);
		float x=getCujd()/need.count*100;

		if(netchange_sucess.kuang_mark==nochange){
			netchange_sucess.kuang_mark=change;
			netchange_sucess.kuang_img=Sprite::create(pet_jindu_kuan_img);
			netchange_sucess.kuang_img->setPosition(Vec2(netchange_sucess.need_img->getPositionX()+netchange_sucess.need_img->getContentSize().width/2+netchange_sucess.kuang_img->getContentSize().width/2,
				netchange_sucess.back_hero->getContentSize().height*0.825));
			netchange_sucess.back_hero->addChild(netchange_sucess.kuang_img);
		}else{

			netchange_sucess.back_hero->setVisible(true);
		}


		if(netchange_sucess.progress_mark==nochange){

			netchange_sucess.progress_mark=change;
			netchange_sucess.progress = ProgressTimer::create(Sprite::create(pet_jindu2_kuan_img));
			netchange_sucess.progress->setType(ProgressTimer::Type::BAR);
			netchange_sucess.progress->setAnchorPoint(Vec2(0.5,0.5));
			netchange_sucess.progress->setMidpoint(Vec2(0,0));
			netchange_sucess.progress->setBarChangeRate(Vec2(1, 0));
			netchange_sucess.progress->setPosition(Vec2(netchange_sucess.kuang_img->getContentSize().width/2,netchange_sucess.kuang_img->getContentSize().height/2+2));
			float x=getCujd()/need.count*100;
			netchange_sucess.progress->setPercentage(x);
			netchange_sucess.kuang_img->addChild( netchange_sucess.progress,100);
		}
		netchange_sucess.progress->setVisible(true);
		netchange_sucess.progress->setPercentage(x);
    
	}else if(isMaxlevel()==true){

			if(netchange_sucess.need_mark==change){
			   netchange_sucess.need_img->setVisible(false);
			}
			if(netchange_sucess.pecent_mark==change){
				netchange_sucess.percent_label->setVisible(false);
			}
			if(netchange_sucess.kuang_mark==change){
				netchange_sucess.kuang_img->setVisible(false);
			}
			if(netchange_sucess.progress_mark==change){
				 netchange_sucess.progress->setVisible(false);
			}

	}

	//netchange_sucess.dj_img->setTexture(Sprite::create(pet_dengji_img)->getTexture());

	int pet_level=pet.level==0?1:pet.level;
	int next_pet_level=pet.level+1;
	if(pet.level==ActorHelper::getHelper()->getMaxLevel(pet.unit.ID)){
		next_pet_level=pet.level;
	}

	for(int i=1;i<=ActorHelper::getHelper()->getMaxLevel(pet.unit.ID);i++){

		string p_img="UI_jiaose_shuxing_dengji_kuang.png";
		if(pet_level>=i)
				p_img="UI_jiaose_shuxing_dengji.png";
		netchange_sucess.sps.at(i-1)->setTexture(Sprite::create(p_img)->getTexture());
		if(i==pet_level){
			netchange_sucess.sps.at(i-1)->setTexture(Sprite::create("UI_jiaose_shuxing_dengji.png")->getTexture());
			netchange_sucess.sps.at(i-1)->stopAllActions();
			netchange_sucess.sps.at(i-1)->setOpacity(255);
		}

		if(isUnlockpet()==true){
		   if(i>pet_level&&i<=next_pet_level){
				netchange_sucess.sps.at(i-1)->setTexture(Sprite::create("UI_jiaose_shuxing_dengji.png")->getTexture());
				netchange_sucess.sps.at(i-1)->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr)));
			}
		}
	}

	if(isMaxlevel()==true){

		auto mjts=Sprite::create("UI_jiaose_shuxing_tishi_max.png");
		mjts->setScale(0.8f);
		float suoxiao=mjts->getContentSize().width*0.8f;
		mjts->setPosition(Vec2(netchange_sucess.dj_img->getPositionX()+netchange_sucess.dj_img->getContentSize().width+netchange_sucess.jg*5+5+mjts->getContentSize().width*0.8f/2,netchange_sucess.back_hero->getContentSize().height*0.75));
		netchange_sucess.back_hero->addChild(mjts);
		netchange_sucess.msjs.pushBack(mjts);

	}

	if(isUnlockpet()==false){

		for(auto m:netchange_sucess.msjs){
			m->removeFromParent();
		}

	}

	Addrdata mdats=getCurentaddrdata();
	Addrdata cdata=getCurentandnextdata();
	// 28 商店字体01 （184,69,13）   28  02(190,68,71)
	//分数 fenshu
	float ptx=netchange_sucess.back_hero->getContentSize().width*0.25;
	float pty=netchange_sucess.back_hero->getContentSize().height*(0.68-x*0.8);

	if(tag==1){

		for(auto m:netchange_sucess.labelss){
			m->removeFromParentAndCleanup(true);
		}
		for(int i=0;i<4;i++){
		   netchange_sucess.f_smark[i]=0;
		   netchange_sucess.cf_smark[i]=0;
		   netchange_sucess.jiaotou_mark[i]=0;
		   netchange_sucess.acf_smark[i]=0;
	    }

	}

	if(mdats.score>0){

		if(netchange_sucess.f_smark[0]==nochange){
			drawaddrPet(netchange_sucess.back_hero,mdats.score,cdata.score,"fenshu",0);
		}
		changesudrawaddrPet(mdats.score,cdata.score,"fenshu",0);
	}
	
	 //金币 jinbi
	if(mdats.gold>0){    
		if(netchange_sucess.f_smark[1]==nochange){
			drawaddrPet(netchange_sucess.back_hero,mdats.gold,cdata.gold,"jinbi",1);
		}
		changesudrawaddrPet(mdats.gold,cdata.gold,"jinbi",1);
	}
	 
	//攻击力 gongjili
	if(mdats.pwer>0){
		if(netchange_sucess.f_smark[2]==nochange){
			drawaddrPet(netchange_sucess.back_hero,mdats.pwer,cdata.pwer,"gongjili",2);
		}
		changesudrawaddrPet(mdats.pwer,cdata.pwer,"gongjili",2);		
	}
	 
	//磁铁 cidie
	if(mdats.maget>0){
		if(netchange_sucess.f_smark[3]==nochange){
			drawaddrPet(netchange_sucess.back_hero,mdats.maget,cdata.maget,"cidie",3);
		}
		changesudrawaddrPet(mdats.maget,cdata.maget,"cidie",3);		
	}

	//自带护盾 zidaihutun
	if(tag==change){
		if(mdats.protect==true){

			string str_fs=FX::StringsMap::getValue("zidaihutun");
			auto f_s=Label::createWithSystemFont(str_fs,"minijiankatong.ttf",28);
			f_s->setColor(Color3B(184,69,13));
			f_s->setAnchorPoint(Vec2(0,0.5));
			f_s->setPosition(Vec2(ptx,netchange_sucess.back_hero->getContentSize().height*(0.68-x*0.08)));
			netchange_sucess.back_hero->addChild(f_s); 
			netchange_sucess.labelss.pushBack(f_s);
			x+=1;
		}

		//自带冲刺 zidaihutun
		if(mdats.rush==true){
			string str_fs=FX::StringsMap::getValue("zidaichongci");
			auto f_s=Label::createWithSystemFont(str_fs,"minijiankatong.ttf",28);
			f_s->setColor(Color3B(184,69,13));
			f_s->setAnchorPoint(Vec2(0,0.5));
			f_s->setPosition(Vec2(ptx,netchange_sucess.back_hero->getContentSize().height*(0.68-x*0.08)));
			netchange_sucess.back_hero->addChild(f_s);  
			netchange_sucess.labelss.pushBack(f_s);
			x+=1;
		}
	}

	if(isMaxlevel()==true){

		    if(netchange_sucess.miaoshu_mark==nochange){
				netchange_sucess.miaoshu_mark=change;
				netchange_sucess.miaoshu_label=Label::createWithSystemFont(pet.unit.descrigbe,"minijiankatong.ttf",24,Size(400,0),TextHAlignment::CENTER);
				netchange_sucess.miaoshu_label->setAnchorPoint(Vec2(0.5,0.5));
				netchange_sucess.miaoshu_label->setPosition(Vec2(netchange_sucess.back_hero->getContentSize().width*0.5,
					netchange_sucess.back_hero->getContentSize().height*0.3));
				netchange_sucess.back_hero->addChild(netchange_sucess.miaoshu_label);
				netchange_sucess.miaoshu_label->setString(pet.unit.descrigbe);
			}else{
				netchange_sucess.miaoshu_label->setString(pet.unit.descrigbe);
				netchange_sucess.miaoshu_label->setVisible(true);
				
			}

			if(netchange_sucess.laysers_mark==change){
				for(int i=0;i<3;i++){
					netchange_sucess.layers.at(i)->setVisible(false);
				}
			}

			if(netchange_sucess.wy_mark==change){
				this->getChildByTag(pet_wy_pet_tag_menu)->setVisible(false);
			}
			if(netchange_sucess.mj_mark==nochange){
				drawMaojimenu();
				//pmj->setTag(maoji_sprite_tag);
				this->getChildByTag(maoji_sprite_tag)->setVisible(true);
			}else{
				this->getChildByTag(maoji_sprite_tag)->setVisible(true);
			}

	 }else if(isUnlockpet()==true&&isMaxlevel()==false){
            

		 if(netchange_sucess.laysers_mark==nochange){

			 netchange_sucess.laysers_mark=change;
			 for(int i=0;i<3;i++){
					Layer* p=Layer::create();
					string n_img;
					float x=0;
					float y=0;
					if(i==0){
						x=netchange_sucess.back_hero->getContentSize().width*0.32;
						y=netchange_sucess.back_hero->getContentSize().height*0.38;
						n_img="UI_chongwu_siliao_dangao.png";
					}else if(i==1){
						x=netchange_sucess.back_hero->getContentSize().width*0.67;
						y=netchange_sucess.back_hero->getContentSize().height*0.38;
						n_img="UI_chongwu_siliao_tiantianquan.png";
					}else if(i==2){
						x=netchange_sucess.back_hero->getContentSize().width*0.32;
						y=netchange_sucess.back_hero->getContentSize().height*0.22;
						n_img="UI_chongwu_siliao_tiantong.png";
					}
					p->setPosition(Vec2(x,y));
					drawfood(n_img,p);
					netchange_sucess.back_hero->addChild(p);
					netchange_sucess.layers.pushBack(p);
			   }


		 }else if( netchange_sucess.laysers_mark==change){

			    if( netchange_sucess.miaoshu_mark==change){
			        netchange_sucess.miaoshu_label->setVisible(false);
				}
			     
			    for(int i=0;i<3;i++){
						netchange_sucess.layers.at(i)->setVisible(true);
				}

				int num=0;
				for(int i=0;i<3;i++){
					if(i==0){
						num=GoodsHelper::getHelper()->getDaogaiNum();
						netchange_sucess.num_food[0]->setString(__String::createWithFormat("%d",num)->getCString());
					}else if(i==1){
						num=GoodsHelper::getHelper()->getTangguoNum();
						netchange_sucess.num_food[1]->setString(__String::createWithFormat("%d",num)->getCString());
					}else if(i==2){
						num=GoodsHelper::getHelper()->getGuodongNum();
						netchange_sucess.num_food[2]->setString(__String::createWithFormat("%d",num)->getCString());
					}
				}
		 }

		 if(netchange_sucess.wy_mark==nochange){

			 netchange_sucess.wy_mark=change;
			 auto  wy=MenuItemImage::create(pet_weiyang_img,pet_weiyang_img,CC_CALLBACK_1(Netpet::allcallback,this)); 
			 wy->setTag(pet_wy_pet_tag);
			 wy->setPosition(Vec2(netchange_sucess.back_hero->getPositionX(),wy->getContentSize().height/2+20));
			 fillOuty(wy,pet_weiyang_img);

			 auto menu=Menu::create(wy,nullptr);
			 menu->setPosition(Vec2(0,0));
			 menu->setTag(pet_wy_pet_tag_menu);
			 addChild(menu);

			 if(netchange_sucess.mj_mark==change){
				 this->getChildByTag(maoji_sprite_tag)->setVisible(false);
			 }
		 }else{

			 this->getChildByTag(pet_wy_pet_tag_menu)->setVisible(true);
			 if(netchange_sucess.mj_mark==change){
				 this->getChildByTag(maoji_sprite_tag)->setVisible(false);
			 }
		 }


	 }

	 if(isUnlockpet()==false){

		if(netchange_sucess.mj_mark==change){
				   this->getChildByTag(maoji_sprite_tag)->setVisible(false);
		}

		if(netchange_sucess.wy_mark==change){
			   this->getChildByTag(pet_wy_pet_tag_menu)->setVisible(false);
	    }

		if(netchange_sucess.miaoshu_mark==nochange){
			netchange_sucess.miaoshu_mark=change;
			netchange_sucess.miaoshu_label=Label::createWithSystemFont(pet.unit.descrigbe,"minijiankatong.ttf",24,Size(400,0),TextHAlignment::CENTER);
			netchange_sucess.miaoshu_label->setAnchorPoint(Vec2(0.5,0.5));
			netchange_sucess.miaoshu_label->setPosition(Vec2(netchange_sucess.back_hero->getContentSize().width*0.5,
				netchange_sucess.back_hero->getContentSize().height*0.3));
			netchange_sucess.back_hero->addChild(netchange_sucess.miaoshu_label);
			netchange_sucess.miaoshu_label->setString(pet.unit.descrigbe);

		}else{
			netchange_sucess.miaoshu_label->setString(pet.unit.descrigbe);
			netchange_sucess.miaoshu_label->setVisible(true);
		}
		if(netchange_sucess.laysers_mark==change){
			for(int i=0;i<3;i++){
				netchange_sucess.layers.at(i)->setVisible(false);
			}
		}

		if(tag==change){
			if(netchange_sucess.armate_mark==change){
				this->getChildByTag(aramute_tag)->setVisible(false);
			}
		}

	}

	//if(pet.unit.ID==unitID_PiPiZhu)
	{

		if(netchange_sucess.armate_mark==change){
			this->getChildByTag(aramute_tag)->setVisible(true);
		}
		refreshPetImage(this->getChildByTag(aramute_tag), pet.unit.ID);
	}
}


void Netpet::changedbyId(){

}

void Netpet::changesudrawaddrPet(float mdats,float cdata,string name,int mindex){

	
	 string str_fs=FX::StringsMap::getValue(name);
	 netchange_sucess.f_s[mindex]->setString(str_fs);

	 string cstr_fs;
	 if(mdats>0){
				//if(isLockpet()==false||(isLockpet()==true&&isMaxlevel()==true)){
					cstr_fs="+";
					cstr_fs+=__String::createWithFormat("%.0f",mdats*100)->getCString();
					cstr_fs+="%";
				//}
	 }else{
				cstr_fs="+0%";
	 }
	 netchange_sucess.cf_s[mindex]->setString(cstr_fs);
	 if(mdats>0&&isUnlockpet()==true&&isMaxlevel()==false){
	     
		 netchange_sucess.jiaotou[mindex]->setString("=>");
		 string addstr="+";
		 addstr+=__String::createWithFormat("%.0f",cdata*100)->getCString();
		 addstr+="%";
		 netchange_sucess.acf_s[mindex]->setString(addstr);


	 }

	 if(mdats>0&&isUnlockpet()==true&&isMaxlevel()==true){
		 if(netchange_sucess.jiaotou_mark[mindex]==change){
		    netchange_sucess.jiaotou[mindex]->setString("");
		 }
		 if(netchange_sucess.acf_smark[mindex]==change){
		    netchange_sucess.acf_s[mindex]->setString("");
		 }
	 }

	if(isUnlockpet()==true&&isMaxlevel()==true){
				 
				 auto maoji=Sprite::create("UI_jiaose_shuxing_tishi_max.png");
				 maoji->setScale(0.8f);
				 maoji->setPosition(Vec2(netchange_sucess.cf_s[mindex]->getPositionX()+netchange_sucess.cf_s[mindex]->getContentSize().width+5+maoji->getContentSize().width/2,
					                     netchange_sucess.f_s[mindex]->getPositionY()));
				 netchange_sucess.back_hero->addChild(maoji);
				 netchange_sucess.msjs.pushBack(maoji);

	}
	

	 
	


}


void Netpet::drawaddrPet(Sprite* p,float mdats,float cdata,string name,int mindex){

	        
	        float ptx=p->getContentSize().width*0.25;
	        float pty=p->getContentSize().height*0.68;
	        string str_fs=FX::StringsMap::getValue(name);

			netchange_sucess.f_smark[mindex]=change;
			netchange_sucess.f_s[mindex]=Label::createWithSystemFont(str_fs,"minijiankatong.ttf",28);
			netchange_sucess.f_s[mindex]->setColor(Color3B(184,69,13));
			netchange_sucess.f_s[mindex]->setAnchorPoint(Vec2(0,0.5));
			netchange_sucess.f_s[mindex]->setPosition(Vec2(ptx,p->getContentSize().height*(0.68-x*0.08)));
			p->addChild(netchange_sucess.f_s[mindex]);
			netchange_sucess.labelss.pushBack(netchange_sucess.f_s[mindex]);


			string cstr_fs;
			netchange_sucess.cf_smark[mindex]=change;
			netchange_sucess.cf_s[mindex]=Label::createWithSystemFont(str_fs,"minijiankatong.ttf",28);
			netchange_sucess.cf_s[mindex]->setColor(Color3B(190,68,71));
 			netchange_sucess.cf_s[mindex]->setAnchorPoint(Vec2(0,0.5));
			netchange_sucess.cf_s[mindex]->setPosition(Vec2(netchange_sucess.f_s[mindex]->getPositionX()+netchange_sucess.f_s[mindex]->getContentSize().width+5,netchange_sucess.f_s[mindex]->getPositionY()));
			p->addChild(netchange_sucess.cf_s[mindex]);
			netchange_sucess.labelss.pushBack(netchange_sucess.cf_s[mindex]);

			if(mdats>0){
				//if(isLockpet()==false||(isLockpet()==true&&isMaxlevel()==true)){
					cstr_fs="+";
					cstr_fs+=__String::createWithFormat("%.0f",mdats*100)->getCString();
					cstr_fs+="%";
				//}
			 }else{
				cstr_fs="+0%";
			 }
			 netchange_sucess.cf_s[mindex]->setString(cstr_fs);
			 if(mdats>0&&isUnlockpet()==true&&isMaxlevel()==false){
				     
				     netchange_sucess.jiaotou_mark[mindex]=change;
				     netchange_sucess.jiaotou[mindex]=Label::createWithSystemFont("=>","minijiankatong.ttf",28);
					 netchange_sucess.jiaotou[mindex]->setColor(Color3B(190,68,71));
					 netchange_sucess.jiaotou[mindex]->setAnchorPoint(Vec2(0,0.5));
					 netchange_sucess.jiaotou[mindex]->setPosition(Vec2(netchange_sucess.cf_s[mindex]->getPositionX()+netchange_sucess.cf_s[mindex]->getContentSize().width+5,netchange_sucess.f_s[mindex]->getPositionY()));
					 p->addChild(netchange_sucess.jiaotou[mindex]);
					 netchange_sucess.labelss.pushBack(netchange_sucess.jiaotou[mindex]);

					 string addstr="+";
					 addstr+=__String::createWithFormat("%.0f",cdata*100)->getCString();
					 addstr+="%";
					 netchange_sucess.acf_smark[mindex]=change;
					 netchange_sucess.acf_s[mindex]=Label::createWithSystemFont(addstr,"minijiankatong.ttf",28);
					 netchange_sucess.acf_s[mindex]->setColor(Color3B(190,68,71));
 					 netchange_sucess.acf_s[mindex]->setAnchorPoint(Vec2(0,0.5));
					 netchange_sucess.acf_s[mindex]->setPosition(Vec2(netchange_sucess.jiaotou[mindex]->getPositionX()+netchange_sucess.jiaotou[mindex]->getContentSize().width+5,netchange_sucess.jiaotou[mindex]->getPositionY()));
					 netchange_sucess.acf_s[mindex]->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr)));
					 p->addChild( netchange_sucess.acf_s[mindex]);
					 netchange_sucess.labelss.pushBack(netchange_sucess.acf_s[mindex]);
				}
			 x=x+1;
             
			if(isUnlockpet()==true&&isMaxlevel()==true){
				 
				 auto maoji=Sprite::create("UI_jiaose_shuxing_tishi_max.png");
				 maoji->setScale(0.8f);
				 maoji->setPosition(Vec2(netchange_sucess.cf_s[mindex]->getPositionX()+netchange_sucess.cf_s[mindex]->getContentSize().width+5+maoji->getContentSize().width/2,netchange_sucess.f_s[mindex]->getPositionY()));
				 p->addChild(maoji);
				 netchange_sucess.msjs.pushBack(maoji);
			}





}
void Netpet::levaDarw(){

	


	auto ret_img=MenuItemImage::create(pet_close_img,pet_close_img,CC_CALLBACK_1(Netpet::allcallback,this));
	ret_img->setTag(pet_return_button_tag);
	ret_img->setPosition(Vec2(netchange_sucess.back_hero->getContentSize().width-ret_img->getContentSize().width/2,
		                      netchange_sucess.back_hero->getContentSize().height-ret_img->getContentSize().height/4));
	fillOuty(ret_img,pet_close_img);

	auto remenu=Menu::create(ret_img,nullptr);
	remenu->setPosition(Vec2::ZERO);
	netchange_sucess.back_hero->addChild(remenu);



	//姓名
	netchange_sucess.h_name=Label::createWithSystemFont(pet.unit.name,"minijiankatong.ttf",30);
	netchange_sucess.h_name->setAnchorPoint(Vec2(0,0.5));
	netchange_sucess.h_name->setPosition(Vec2(netchange_sucess.back_hero->getContentSize().width*0.15,netchange_sucess.back_hero->getContentSize().height*0.825));
	netchange_sucess.h_name->setColor(Color3B(184,69,13));
	netchange_sucess.back_hero->addChild(netchange_sucess.h_name);



	if(isUnlockpet()==false){
		netchange_sucess.suo_mark=change;
		netchange_sucess.suo_img=Sprite::create(pet_suokuan_bg);
		netchange_sucess.suo_img->setScale(0.7f);
		netchange_sucess.suo_img->setPosition(Vec2(netchange_sucess.h_name->getPositionX()+netchange_sucess.h_name->getContentSize().width
			                                       +netchange_sucess.suo_img->getContentSize().width*0.7f/2,
			                                        netchange_sucess.back_hero->getContentSize().height*0.825));
		netchange_sucess.back_hero->addChild(netchange_sucess.suo_img);
	}
	//获得该等级所需的图片
	if(isMaxlevel()==false){
		Needunit need=getneedUnit();
		netchange_sucess.need_mark=change;
		netchange_sucess.need_img=Sprite::create(need.imgfile);
		netchange_sucess.need_img->setPosition(Vec2(netchange_sucess.back_hero->getContentSize().width*0.55,netchange_sucess.back_hero->getContentSize().height*0.825));
		netchange_sucess.back_hero->addChild(netchange_sucess.need_img);


		netchange_sucess.kuang_mark=change;
		netchange_sucess.kuang_img=Sprite::create(pet_jindu_kuan_img);
		netchange_sucess.kuang_img->setPosition(Vec2(netchange_sucess.need_img->getPositionX()+netchange_sucess.need_img->getContentSize().width/2+netchange_sucess.kuang_img->getContentSize().width/2,
									netchange_sucess.back_hero->getContentSize().height*0.825));
		netchange_sucess.back_hero->addChild(netchange_sucess.kuang_img);

	
			string str_pr;
			if(isUnlockpet()==true){
				str_pr=__String::createWithFormat("%d",getCujd())->getCString();
				str_pr+="/";
				str_pr+=__String::createWithFormat("%d",need.count)->getCString();
			}else{
				str_pr="? ? ?";
			}

			netchange_sucess.pecent_mark=change;
			netchange_sucess.percent_label=Label::createWithSystemFont(str_pr,"minijiankatong.ttf",24);
			netchange_sucess.percent_label->setPosition(Vec2(netchange_sucess.kuang_img->getContentSize().width/2,netchange_sucess.kuang_img->getContentSize().height/2));
			netchange_sucess.kuang_img->addChild(netchange_sucess.percent_label,120);


			if(isUnlockpet()==true){
				 
				 netchange_sucess.progress_mark=change;
				 netchange_sucess.progress = ProgressTimer::create(Sprite::create(pet_jindu2_kuan_img));
				 netchange_sucess.progress->setType(ProgressTimer::Type::BAR);
				 netchange_sucess.progress->setAnchorPoint(Vec2(0.5,0.5));
				 netchange_sucess.progress->setMidpoint(Vec2(0,0));
				 netchange_sucess.progress->setBarChangeRate(Vec2(1, 0));
				 netchange_sucess.progress->setPosition(Vec2(netchange_sucess.kuang_img->getContentSize().width/2,netchange_sucess.kuang_img->getContentSize().height/2+2));
				 float x=getCujd()/need.count*100;
				 netchange_sucess.progress->setPercentage(x);
				 netchange_sucess.kuang_img->addChild( netchange_sucess.progress,100);
			}
	}
	//等级
	netchange_sucess.dj_img=Sprite::create(pet_dengji_img);
	netchange_sucess.dj_img->setScale(0.8f);
	netchange_sucess.dj_img->setAnchorPoint(Vec2(0,0.5));
	netchange_sucess.dj_img->setPosition(Vec2(netchange_sucess.back_hero->getContentSize().width*0.15,netchange_sucess.back_hero->getContentSize().height*0.75));
	netchange_sucess.back_hero->addChild(netchange_sucess.dj_img);

	int pet_level=pet.level==0?1:pet.level;
	int next_pet_level=pet.level+1;
	if(pet.level==ActorHelper::getHelper()->getMaxLevel(pet.unit.ID)){
		next_pet_level=pet.level;
	}
	float jg=0;
	for(int i=1;i<=ActorHelper::getHelper()->getMaxLevel(pet.unit.ID);i++){
		string p_img="UI_jiaose_shuxing_dengji_kuang.png";
		if(pet_level>=i)
				p_img="UI_jiaose_shuxing_dengji.png";
		
		auto p=Sprite::create(p_img);
		float xp=p->getContentSize().width*0.6f;
		jg=xp;
		netchange_sucess.jg=xp;
		p->setScale(0.6f);
		p->setPosition(Vec2(netchange_sucess.dj_img->getPositionX()+netchange_sucess.dj_img->getContentSize().width+(i-1)*xp+xp/2,
			                netchange_sucess.back_hero->getContentSize().height*0.75));
		
		if(isUnlockpet()==true){
		   if(i>pet_level&&i<=next_pet_level){
				p->setTexture(Sprite::create("UI_jiaose_shuxing_dengji.png")->getTexture());
				Action* action=RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr));
				action->setTag(100);
				p->runAction(action);
			}
		}
		netchange_sucess.back_hero->addChild(p);
		netchange_sucess.sps.pushBack(p);

	}

	if(isMaxlevel()==true){

		auto mjts=Sprite::create("UI_jiaose_shuxing_tishi_max.png");
		mjts->setScale(0.8f);
		float suoxiao=mjts->getContentSize().width*0.8f;
		mjts->setPosition(Vec2(netchange_sucess.dj_img->getPositionX()+netchange_sucess.dj_img->getContentSize().width+jg*5+5+mjts->getContentSize().width*0.8f/2,netchange_sucess.back_hero->getContentSize().height*0.75));
		netchange_sucess.back_hero->addChild(mjts);
		netchange_sucess.msjs.pushBack(mjts);
	}
    
	Addrdata mdats=getCurentaddrdata();
	Addrdata cdata=getCurentandnextdata();
	// 28 商店字体01 （184,69,13）   28  02(190,68,71)
	//分数 fenshu
	 //
	 float ptx=netchange_sucess.back_hero->getContentSize().width*0.25;
	 float pty=netchange_sucess.back_hero->getContentSize().height*(0.68-x*0.8);

	 if(mdats.score>0){
		 drawaddrPet(netchange_sucess.back_hero,mdats.score,cdata.score,"fenshu",0);
	  }
	 
	 //金币 jinbi
	 if(mdats.gold>0){    
		  drawaddrPet(netchange_sucess.back_hero,mdats.gold,cdata.gold,"jinbi",1);
	 }
	 
	//攻击力 gongjili
	 if(mdats.pwer>0){
		  drawaddrPet(netchange_sucess.back_hero,mdats.pwer,cdata.pwer,"gongjili",2);		
	 }
	 
	//磁铁 cidie
	 
	 if(mdats.maget>0){
		  drawaddrPet(netchange_sucess.back_hero,mdats.maget,cdata.maget,"cidie",3);		
	 }
	

	//自带护盾 zidaihutun
	 
	 if(mdats.protect==true){
			    
				string str_fs=FX::StringsMap::getValue("zidaihutun");
				auto f_s=Label::createWithSystemFont(str_fs,"minijiankatong.ttf",28);
				f_s->setColor(Color3B(184,69,13));
				f_s->setAnchorPoint(Vec2(0,0.5));
				f_s->setPosition(Vec2(ptx,netchange_sucess.back_hero->getContentSize().height*(0.68-x*0.08)));
				netchange_sucess.back_hero->addChild(f_s); 
				netchange_sucess.labelss.pushBack(f_s);
				x+=1;
	  }
	
	 //自带冲刺 zidaihutun
	 if(mdats.rush==true){
					string str_fs=FX::StringsMap::getValue("zidaichongci");
					auto f_s=Label::createWithSystemFont(str_fs,"minijiankatong.ttf",28);
					f_s->setColor(Color3B(184,69,13));
					f_s->setAnchorPoint(Vec2(0,0.5));
					f_s->setPosition(Vec2(ptx,netchange_sucess.back_hero->getContentSize().height*(0.68-x*0.08)));
					netchange_sucess.back_hero->addChild(f_s);  
					netchange_sucess.labelss.pushBack(f_s);
					x+=1;
	 }

	 if(isMaxlevel()==true||isUnlockpet()==false){

		    netchange_sucess.miaoshu_mark=change;
			netchange_sucess.miaoshu_label=Label::createWithSystemFont(pet.unit.descrigbe,"minijiankatong.ttf",24,Size(400,0),TextHAlignment::LEFT);
			netchange_sucess.miaoshu_label->setAnchorPoint(Vec2(0.5,0.5));
			netchange_sucess.miaoshu_label->setPosition(Vec2(netchange_sucess.back_hero->getContentSize().width*0.5,netchange_sucess.back_hero->getContentSize().height*0.3));
			netchange_sucess.back_hero->addChild(netchange_sucess.miaoshu_label);


	 }else if(isUnlockpet()==true&&isMaxlevel()==false){
            
		     netchange_sucess.laysers_mark=change;
			 for(int i=0;i<3;i++){
					Layer* p=Layer::create();
					string n_img;
					float x=0;
					float y=0;
					if(i==0){
						x=netchange_sucess.back_hero->getContentSize().width*0.32;
						y=netchange_sucess.back_hero->getContentSize().height*0.38;
						n_img="UI_chongwu_siliao_dangao.png";
					}else if(i==1){
						x=netchange_sucess.back_hero->getContentSize().width*0.67;
						y=netchange_sucess.back_hero->getContentSize().height*0.38;
						n_img="UI_chongwu_siliao_tiantianquan.png";
					}else if(i==2){
						x=netchange_sucess.back_hero->getContentSize().width*0.32;
						y=netchange_sucess.back_hero->getContentSize().height*0.22;
						n_img="UI_chongwu_siliao_tiantong.png";
					}
					p->setPosition(Vec2(x,y));
					drawfood(n_img,p);
					netchange_sucess.back_hero->addChild(p);
					netchange_sucess.layers.pushBack(p);
			   }
	 }
}

void Netpet::drawfood(string imgfile,Layer* layer){

	//UI_chongwu_siliao_bg.png UI_chongwu_siliao_dangao.png UI_chongwu_siliao_tiantianquan.png UI_chongwu_siliao_tiantong.png
	auto back=MenuItemImage::create("UI_chongwu_siliao_bg.png","UI_chongwu_siliao_bg.png",CC_CALLBACK_1(Netpet::allcallback,this));
	back->setTag(pet_food_add_tag);

	auto dg_img=Sprite::create(imgfile);
	dg_img->setPosition(Vec2(0,back->getContentSize().height/2));
	back->addChild(dg_img);

	// 个数
	int num=0;
	int index;
	GoodsHelper::getHelper()->getTangguoNum();
	if(imgfile.compare("UI_chongwu_siliao_dangao.png")==0){
		num=GoodsHelper::getHelper()->getDaogaiNum();
		index=0;
	}else if(imgfile.compare("UI_chongwu_siliao_tiantianquan.png")==0){
		num=GoodsHelper::getHelper()->getTangguoNum();
		index=1;
	}else if(imgfile.compare("UI_chongwu_siliao_tiantong.png")==0){
		num=GoodsHelper::getHelper()->getGuodongNum();
		index=2;
	}


	netchange_sucess.num_food[index]=Label::createWithSystemFont(__String::createWithFormat("%d",num)->getCString(),"minijiankatong.ttf",36);
	netchange_sucess.num_food[index]->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height*0.6));
	back->addChild(netchange_sucess.num_food[index]);
    
	auto add_b=MenuItemImage::create("UI_zhuye_jinbilan_anniu_jia.png","UI_zhuye_jinbilan_anniu_jia.png",CC_CALLBACK_1(Netpet::allcallback,this));
	add_b->setTag(pet_food_add_tag);
	add_b->setPosition(Vec2(back->getContentSize().width-add_b->getContentSize().width,back->getContentSize().height/2-add_b->getContentSize().height/3));
	
	fillOuty(add_b,"UI_zhuye_jinbilan_anniu_jia.png");

	auto menu=Menu::create(back,add_b,nullptr);
	menu->setPosition(Vec2::ZERO);
	layer->addChild(menu);

	
	 

}





	

void Netpet::drawPetlist(){


    auto p=Petlist::create();
	p->setPosition(Vec2(getContentSize().width*0.06,0));
	p->setTag(pet_list_tag);
	addChild(p,100);
	

}

void Netpet::centerjiemian(){



}

void Netpet::allcallback(Ref* ref){


	MenuItemImage* p=(MenuItemImage*)ref;
	int tag=(int)p->getTag();
	current_oprate=tag;
	callsp();
	 
}
void Netpet::bAction(MenuItemImage* node){

	
}
void Netpet::callsp(){

	
	if(current_oprate==pet_return_button_tag){
		   
		   EventCustom event_m(Layerchange);
		   LayerData data_m;
		   data_m.current=103;
		   event_m.setUserData(&data_m);
		   Director::getInstance()->getEventDispatcher()->dispatchEvent(&event_m);
		   
	 }else if(current_oprate==pet_shouji_pet_tag){//收集


	 }else if(current_oprate==pet_wy_pet_tag){    //喂养
		 if(pet.level==ActorHelper::getHelper()->getMaxLevel(pet.unit.ID)){
			 return;
		 }
		 current=pet.level;
		 ActorHelper::getHelper()->feeding(pet.unit.ID);
	 }else if(current_oprate==pet_food_add_tag){
		 //去商店
		 auto shop=BaseLayer::create();
		 shop->shopsW();
	     Director::getInstance()->getRunningScene()->addChild(shop);
	 }

}


void  Netpet::wy_callback0(Ref* ref){

	 //去商店
		 auto shop=BaseLayer::create();
		 shop->shopsW();
	     Director::getInstance()->getRunningScene()->addChild(shop);
}
void  Netpet::wy_callback1(Ref* ref){

	   //去商店
		 auto shop=BaseLayer::create();
		 shop->shopsW();
	     Director::getInstance()->getRunningScene()->addChild(shop);
}

void  Netpet::wy_callback2(Ref* ref){
	   //去商店
		 auto shop=BaseLayer::create();
		 shop->shopsW();
	     Director::getInstance()->getRunningScene()->addChild(shop);
}

void Netpet::fh_buttonCallback(Ref* ref){

	EventCustom event(UI_Layerc);
	Layerst data;
	data.parent_number=1;
	data.son_number=0;
	event.setUserData(&data);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}


// 活跃值统计 升级角色 ActiveValueHelper::getHelper()->addActiveByType(activeType_JueSeShengJi);

void  Netpet::wy_callback(Ref* ref){
	  
	//喂养是否成功 MSG_LevelUp_Rsp
	ActorHelper::getHelper()->feeding(pet.unit.ID);
	//datacz();
	//topjiemian();
}

void  Netpet::hq_callback(Ref* ref){

	
	MenuItemImage* p=(MenuItemImage*)ref;
    
	if(getChildByTag(78)!=nullptr){

		removeChildByTag(78);
	}

	//获取方式
	Label* label=Label::createWithSystemFont(pet.unit.descrigbe,"Arial",30); 
	label->setTag(78);
	label->setColor(Color3B(233,33,33));
	label->setPosition(Vec2(p->getPositionX(),getContentSize().height*0.2));
	addChild(label);
	label->setScale(0.1f);
	auto action1 = Sequence::create(
		                ScaleTo::create(0.5,1.0f),
                        CallFunc::create(
                             // lambda
                             [&](){
                                  ;
                             }  ),
							nullptr);
	label->runAction(action1);
	
}


void Netpet::inintData(){
	 
	 
	 netchange_sucess.suo_mark=0;
	 netchange_sucess.need_mark=0;
	 netchange_sucess.pecent_mark=0;
	 netchange_sucess.progress_mark=0;
	 netchange_sucess.miaoshu_mark=0;
	 netchange_sucess.laysers_mark=0;
	 netchange_sucess.wy_mark=0;
	 netchange_sucess.mj_mark=0;
	 netchange_sucess.armate_mark=0;

	 for(int i=0;i<4;i++){
	   netchange_sucess.f_smark[i]=0;
	   netchange_sucess.cf_smark[i]=0;
	   netchange_sucess.jiaotou_mark[i]=0;
	   netchange_sucess.acf_smark[i]=0;
	 }

	 pets=ActorHelper::getHelper()->getAllPet();
	 if(PlayerInfoHelper::getHelper()->getDeafaultPet()==0){
		 pet=pets.at(0);
		 cId=pet.unit.ID;
	 }else{
		 
		 pet=ActorHelper::getHelper()->getOnePet(PlayerInfoHelper::getHelper()->getDeafaultPet());
	     cId=pet.unit.ID;
	 }
}


Addrdata Netpet::getCurentandnextdata(){

	 Addrdata datacurrent,datanext;
	 datacurrent=getCurentaddrdata();

	 int level=pet.level;
	 if(level==0){
		 level=1;
	 }

	 level=level+1;
	 if(pet.level==ActorHelper::getHelper()->getMaxLevel(pet.unit.ID)){
		 level=pet.level;
	 }
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(pet.unit.ID,level);
	 vector<Attribute> addrs=curent_info->attrVec; 
	 for(auto mr:addrs){

		 if(mr.attr.compare(Attr_AddMultScore)==0){
			 datanext.score=mr.value;//-datacurrent.score;
		 }
		 if(mr.attr.compare(Attr_AddMultGold)==0){
			 datanext.gold=mr.value;//-datacurrent.gold;
		 }
		 if(mr.attr.compare(Attr_AddMagnet)==0){
			 datanext.maget=mr.value;//-datacurrent.maget;
		 }
		 if(mr.attr.compare(Attr_AddPower)==0){
			 datanext.pwer=mr.value;//-datacurrent.pwer;
		 }
		 if(mr.attr.compare(Attr_Rush)==0){
			 datanext.rush=true;
		 }
		 if(mr.attr.compare(Attr_Protect)==0){
			 datanext.protect=true;
		 }
	 }
	 return datanext;

}

Addrdata Netpet::getCurentaddrdata(){


	 Addrdata data;
	 int level=pet.level;
	 if(level==0){
		 level=1;
	 }
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(pet.unit.ID,level);
	 vector<Attribute> addrs=curent_info->attrVec; 
	 for(auto mr:addrs){

		 if(mr.attr.compare(Attr_AddMultScore)==0){
			 data.score=mr.value;
			
		 }
		 if(mr.attr.compare(Attr_AddMultGold)==0){
			 data.gold=mr.value;
		 }
		 if(mr.attr.compare(Attr_AddMagnet)==0){
			 data.maget=mr.value;
		 }
		 if(mr.attr.compare(Attr_AddPower)==0){
			 data.pwer=mr.value;
		 }
		 if(mr.attr.compare(Attr_Rush)==0){
			  data.rush=true;
		 }
		 if(mr.attr.compare(Attr_Protect)==0){
			 data.protect=true;
		 }
	 }
	 return data;
}

Needunit Netpet::getneedUnit(){

	 Needunit need;
	 need.type=unitID_Gold;
	 need.count=0;
	 need.imgfile="UI_zhuye_shangdian_jinbi.png";

	 int level=pet.level;
	 if(level==0){
		 level=1;
	 }
	 int zlv=level;
	 if(zlv<ActorHelper::getHelper()->getMaxLevel(cId)){
		  zlv=level+1;
	 }else if(zlv==ActorHelper::getHelper()->getMaxLevel(cId)){
		  zlv=ActorHelper::getHelper()->getMaxLevel(cId);
	 }
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(pet.unit.ID,zlv);
	 need.type=curent_info->needUnit->ID;
	 need.count=curent_info->needCount;
	 need.imgfile=GoodsHelper::getHelper()->getUnitByID(need.type)->imageFile;
	 return need;
}

//是否已解锁
bool Netpet::isUnlockpet(){

	return  ActorHelper::getHelper()->getIsUnlockItem(pet.unit.ID);
}

bool Netpet::isMaxlevel(){

	return  pet.level==ActorHelper::getHelper()->getMaxLevel(pet.unit.ID);
}



int Netpet::getCujd(){
    
	const BagItem* item=GoodsHelper::getHelper()->getOneItem(pet.unit.ID);
	int max=0;
	if(item==nullptr){
		max=0;
	}else{
		max=item->value_2;
		
	}
	return max;
}
string Netpet::nextwyname(){

	int lev=pet.level;
	if(lev==0){
		lev=1;
	}
	if(isMaxlevel()){
		lev=pet.level;
	}else{
		lev+=1;
	}

	string name;
	const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(pet.unit.ID,lev);
	name=curent_info->needUnit->name;


	return name;

}

string Netpet::getAttr(int level){

	if(level==0){
		level=1;
	}

	const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(pet.unit.ID,level);
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

int Netpet::getCunum(){

	int lev=pet.level;
	if(lev==0){
		lev=1;
	}
	if(isMaxlevel()){
		lev=pet.level;
	}else{
		lev+=1;
	}

	const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(pet.unit.ID,lev);
	int type=curent_info->needUnit->ID;
	int need=curent_info->needCount;
	return need;

}


void Netpet::toolsdata(EventCustom* evt){


     MsgData* data=(MsgData*)evt->getUserData();
	 pet=ActorHelper::getHelper()->getOnePet(pet.unit.ID);
	 
	 if(data->errorCode==Rsp_Success){
		 if(isMaxlevel()==true){
				
				auto p=BasicLayer::create();
				p->maojitishi(103);
				Director::getInstance()->getRunningScene()->addChild(p,100);
		 }else{
			 if(pet.level-current==1){
				auto p=BasicLayer::create();
				p->buystr("sjcg","sjcgchongwumiaoshu");
				Director::getInstance()->getRunningScene()->addChild(p,100);
			 }
		 }
		 levelChange(0);


		 if(current_petlevel<pet.level) ActiveValueHelper::getHelper()->addActiveByType(activeType_ChongWuShengJi);
		 if (isMaxlevel())
		 {
			 //检测白富美成就
			 Achieve*info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_BaiFuMei);
			 if (info && !info->isOver)
			 {
				 AchievementHelper::getHelper()->completeOneAchievement(achieveID_BaiFuMei, 1);
			 }
		 }
		StopLayer::checkFightPowerAchievement();
	 }else{
		 //去商店
		BaseLayer* shop=BaseLayer::create();
		shop->money_no(unitID_DanGao);
	    Director::getInstance()->getRunningScene()->addChild(shop);

	 }

}

void Netpet::playAnimationByname(Node *bs,std::string jsonfile,std::string filename,std::string animationName,bool isloop){


	netchange_sucess.armate_mark=change;
	auto armature =Armature::create(filename.c_str());

	if(isloop==true){
		armature->getAnimation()->play(animationName.c_str(),-1,1);
	}
	armature->removeFromParentAndCleanup(true);
	//armature->setPosition(Vec2(bs->getContentSize().width/2,bs->getContentSize().height/2));
	armature->setTag(100);
	armature->setLocalZOrder(100);
	bs->addChild(armature);
	//xx_sly
	//if(animationName.compare("pzz_ng")==0)
	{
		auto yinying_img=Sprite::create("touying_UI.png"); //(hero_yingyin_img);
		yinying_img->setAnchorPoint(Vec2(0.5,0));
		yinying_img->setPosition(Vec2(0,-89));//-armature->getContentSize().height*0.7));
		armature->addChild(yinying_img);
	}
}

void Netpet::fillOuty(MenuItemImage* p,string imgfile){
	 
	 auto s=Sprite::create(imgfile);
	 s->setScale(1.05f);
	 p->setSelectedImage(s);
}


//petlist
//Petlistlayer

Petlist::Petlist(){

}

bool Petlist::init(){

	if(!Layer::init()){
		return false;
	}

	 pets.clear();
	
	pets=ActorHelper::getHelper()->getAllPet();

    size=Size(120,130);
	mTableView = TableView::create(this,Size(120*4,130));
	this->addChild(mTableView,2);
	mTableView->setDirection(ScrollView::Direction::HORIZONTAL);
	mTableView->setPosition(Vec2(0,0));
	mTableView->setDelegate(this);
	mTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	mTableView->setBounceable(false);

	return true;
}

void Petlist::onEnter(){
	
	Layer::onEnter();
}

void Petlist::onExit(){
   
	Layer::onExit();
}

void Petlist::tableCellTouched(TableView* table, TableViewCell* cell){

	current_oprate=cell->getTag();
	if(ActorHelper::getHelper()->getIsUnlockItem(current_oprate)==true){
		PlayerInfoHelper::getHelper()->setDeafaultPet(current_oprate);
	}
	bAc(cell->getChildByTag(current_oprate));  
	 
}

Size Petlist::tableCellSizeForIndex(TableView *table, ssize_t idx){

	 
	 return Size(120,130);

}

TableViewCell*  Petlist::tableCellAtIndex(TableView *table, ssize_t idx){

	
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
	drawpets(cell,idx);
	return cell;
}


ssize_t Petlist::numberOfCellsInTableView(TableView *table){

	return pets.size();
}


void Petlist::drawpets(TableViewCell *cell, ssize_t idx){


	if(ActorHelper::getHelper()->getIsUnlockItem(pets.at(idx).unit.ID)){
		 
		 auto back=Sprite::create(pet_suo_bg);
		 back->setTag(pets.at(idx).unit.ID);
		 back->setPosition(Vec2(size.width/2,size.height/2));
		 cell->setTag(pets.at(idx).unit.ID);
		 cell->addChild(back);

		 auto toux=Sprite::create(pets.at(idx).unit.imageFile);
		 toux->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height/2));
		 back->addChild(toux);
	 }else{
		 auto back=Sprite::create(pet_suo_bg);
		 back->setTag(pets.at(idx).unit.ID);
		 //back->setOpacity(80);
		 back->setPosition(Vec2(size.width/2,size.height/2));
		 cell->setTag(pets.at(idx).unit.ID);
		 cell->addChild(back);

		 auto toux=Sprite::create(pets.at(idx).unit.imageFile);
		 toux->setPosition(Vec2(back->getContentSize().width/2,back->getContentSize().height/2));
		 back->addChild(toux);

		 auto sd_img=Sprite::create(pet_suokuan_bg);
		 sd_img->setPosition(Vec2(back->getContentSize().width-sd_img->getContentSize().width/2,sd_img->getContentSize().height/2));
		 back->addChild(sd_img);

	 }
}



void Petlist::all_callback(Ref* ref){

	 //id 传出去
	FX::Music::getInstance()->buttonClicked();
	MenuItemImage* p=(MenuItemImage*)ref;
	current_oprate=p->getTag();
    bAc(p);
}

void Petlist::bAc(Node* node){

	node->runAction(	
		             Sequence::create(ScaleTo::create(0.1f,1.2f),CallFuncN::create( CC_CALLBACK_0(Petlist::callp,
							  this,node,true)),nullptr)
					);
	 
}

void Petlist::callp(Node* node,bool mr){

   node->setScale(1.0f);
   EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
   EventCustom event(Msgpet_id);
   MsgData data;
   data.value =current_oprate;
   event.setUserData(&data);
   eventDspc->dispatchEvent(&event);
   
}

Petlist::~Petlist(){

}
