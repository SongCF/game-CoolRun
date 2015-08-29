#include "Magicup.h"
#include "StopLayer.h"
#include "BasicLayer.h"
#include "GameDirector.h"

#define  magic_biti_img "UI_modian_biaoti.png"
#define  magic_back_img "UI_jiaose_bg.png"
#define  magic_close_img "UI_jiaose_anniu_guanbi.png"
#define  magic_dj_img    "UI_jiaose_shuxing_dengji_wenzi.png"
#define  magic_ml_img    "UI_modian_shuxing_moli_wenzi.png"
#define  magic_shenj_img    "UI_jiaose_shengji_anniu_shengji.png"
#define  magic_maoji_img    "UI_jiaose_shengji_anniu_manji.png"
#define  magic_xinxins_img      "UI_jiaose_shuxing_dengji.png"
#define  magic_xinxinx_img      "UI_jiaose_shuxing_dengji_kuang.png"
#define  magic_next_dj_img    "UI_modian_xiayidengji.png"
#define  magic_armature_img "UI_armature_image"

#define  magic_l1_img      "csmagic_01.png"
#define  magic_l2_img      "csmagic_02.png"
#define  magic_l3_img      "csmagic_03.png"
#define  magic_l4_img      "csmagic_04.png"
#define  magic_l5_img      "csmagic_05.png"

enum  Magictag{

	  magic_sj_button_tag,
	  magic_mj_button_tag,
	  magic_return_button_tag,
	  magic_zhuye_tag,
	  magic_game_tag,
	  maoji_tag,
};

Magicup::Magicup(void)
{
}


Magicup::~Magicup(void)
{
}


bool Magicup::init(){

	if(!Layer::init()){
		return false;
	}
	layerGame=magic_zhuye_tag;
	auto callBack       =  [&](EventCustom* evt)  
                            {  
								this->callback_sj(evt);
                            };  
	sj=EventListenerCustom::create(MSG_LevelUp_Rsp,callBack);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(sj,1);  
	magicDescrible.add_mark=0;
	return true;
}

void Magicup::onEnter(){
	
	Layer::onEnter();
}

void Magicup::onExit(){

	this->stopAllActions();
	Director::getInstance()->getEventDispatcher()->removeEventListener(sj);
	Layer::onExit();

}

float Magicup::getDelwithint(float number,int iswidth){

	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/1136);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;

}


void Magicup::setGameFahui(){

	 layerGame=magic_game_tag;
}

void Magicup::setmagicId(int id){

	 

	 Size visibleSize = Director::getInstance()->getWinSize();
	 Actual_x=visibleSize.width;
	 Actual_y=visibleSize.height;
	  //设置大小
	 this->setContentSize(Size(Actual_x,Actual_y-getDelwithint(Tophight,1)));
	 this->magicid=id;
	 int magicid=PlayerInfoHelper::getHelper()->getDeafaultMagicItem();
	 magic=ActorHelper::getHelper()->getOneMagic(magicid);
	 inintJiemian();
	
}


void Magicup::inintJiemian(){

	 
	this->removeAllChildren();
	auto p_magic=Sprite::create(getCurrentleveimg());
	p_magic->setPosition(Vec2(Actual_x*0.25,Actual_y*0.5));
	//addChild(p_magic);

	playAnimationByname(this,"book_ui.ExportJson","book_ui", CCString::createWithFormat("b_huxi%d",magic.level)->getCString(),true);

	back_hero=Sprite::create(magic_back_img);
	auto back_biti=Sprite::create(magic_biti_img);
	back_biti->setPosition(Vec2(Actual_x-back_hero->getContentSize().width/2,Actual_y-back_biti->getContentSize().height/2));
	back_biti->setLocalZOrder(100);
	addChild(back_biti);

	
	back_hero->setPosition(Vec2(Actual_x-back_hero->getContentSize().width/2,
		                        Actual_y-back_biti->getContentSize().height-back_hero->getContentSize().height/2+40));
	addChild(back_hero);
	//Vone_return_mainlayer_img
	levelDraw();

	if(isMaxlevel()==false){

		       levael_str.sj_p =MenuItemImage::create(magic_shenj_img,magic_shenj_img,CC_CALLBACK_1(Magicup::allcallback,this));
				levael_str.sj_p->setTag(magic_sj_button_tag);
				levael_str.sj_p->setPosition(Vec2(back_hero->getPositionX()-levael_str.sj_p->getContentSize().width*0.8,levael_str.sj_p->getContentSize().height/2));
				fillOuty(levael_str.sj_p,magic_shenj_img);
				


				Vec2 sjv=getItemoney();
			    string sj_yuan=__String::createWithFormat("%.0f",sjv.y)->getCString()+FX::StringsMap::getValue("chinsey");
				if(sjv.x==unitID_Gold||sjv.x==unitID_Diamond){
					sj_yuan=__String::createWithFormat("%.0f",sjv.y)->getCString();
				}
				auto sj_m=Label::createWithSystemFont(sj_yuan,"minijiankatong.ttf",30);
				sj_m->setColor(Color3B(254,218,185));
				if(sjv.x==unitID_Gold||sjv.x==unitID_Diamond){
			
					string qname="UI_zhuye_shangdian_jinbi.png";
					if(sjv.x==unitID_Diamond){
						 qname="UI_zhuye_shangdian_zuanshi.png";
					}
					auto m_img=Sprite::create(qname);
					sj_m->setAnchorPoint(Vec2(0,0.5));
					sj_m->setPosition(Vec2((levael_str.sj_p->getContentSize().width-sj_m->getContentSize().width-m_img->getContentSize().width)/2,levael_str.sj_p->getContentSize().height*0.2));
					m_img->setPosition(Vec2(sj_m->getPositionX()+sj_m->getContentSize().width+m_img->getContentSize().width/2,sj_m->getPositionY()));
					m_img->setTag(125);
					levael_str.sj_p->addChild(m_img);

				}else{
					sj_m->setPosition(Vec2(levael_str.sj_p->getContentSize().width/2,levael_str.sj_p->getContentSize().height*0.2));
				}
				sj_m->setLocalZOrder(100);
				sj_m->setTag(100);
				levael_str.sj_p->addChild(sj_m);

		
				auto sj_m1=Label::createWithSystemFont(sj_yuan,"minijiankatong.ttf",30);
				sj_m1->setAnchorPoint(sj_m->getAnchorPoint());
				sj_m1->setPosition(Vec2(sj_m->getPositionX(),sj_m->getPositionY()-2));
				sj_m1->setLocalZOrder(90);
				sj_m1->setTag(200);
				levael_str.sj_p->addChild(sj_m1);

				auto mj=MenuItemImage::create(magic_maoji_img,magic_maoji_img,CC_CALLBACK_1(Magicup::allcallback,this));
				mj->setTag(magic_mj_button_tag);
				mj->setPosition(Vec2(back_hero->getPositionX()+mj->getContentSize().width*0.8,mj->getContentSize().height/2));
				fillOuty(mj,magic_maoji_img);

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

			auto menu=Menu::create(levael_str.sj_p,mj,nullptr);
			menu->setPosition(Vec2(0,0));
			menu->setTag(maoji_tag);
			addChild(menu);

	}else if(isMaxlevel()==true){

		drawMaojitishi();
	}
	
}


Vec2 Magicup::getItemoney(){

	  Vec2 m;
	 int actorlevel=magic.level;
	 if(actorlevel==0){
		 actorlevel=1;
	 }
	 int zlv=actorlevel;
	 if(zlv<ActorHelper::getHelper()->getMaxLevel(magic.unit.ID)){
		  zlv=actorlevel+1;
	 }else if(zlv==ActorHelper::getHelper()->getMaxLevel(magic.unit.ID)){
		  zlv=ActorHelper::getHelper()->getMaxLevel(magic.unit.ID);
	 }
	 const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(magic.unit.ID,zlv);
	 m.x=curent_info->needUnit->ID;
	 m.y=curent_info->needCount;
	 return m;

}

void Magicup::drawSx(Node* node,int dj){

	 
  
	 
     
}

void Magicup::allcallback(Ref* ref){

	 
	MenuItemImage* p=(MenuItemImage*)ref;
	int tag=(int)p->getTag();
	current_oprate=tag;
	callsp();
	 
}

void Magicup::bAction(MenuItemImage* node){


}

void Magicup::callsp(){


	 /*
	  magic_sj_button_tag,
	  magic_mj_button_tag,
	 */
	
	 if(current_oprate==magic_sj_button_tag)
	 {
		
		 ActorHelper::getHelper()->levelUp(magic.unit.ID);

	 }else if(current_oprate==magic_mj_button_tag){

		 ActorHelper::getHelper()->fullLevelUp(magic.unit.ID);
		  
	 }else if(current_oprate==magic_return_button_tag){
		  if(layerGame== magic_zhuye_tag){
			   EventCustom event_m(Layerchange);
			   LayerData data_m;
			   data_m.current=104;
			   event_m.setUserData(&data_m);
			   Director::getInstance()->getEventDispatcher()->dispatchEvent(&event_m);
		  }else if(layerGame== magic_game_tag){
			  this->getParent()->removeFromParent();
		  }
	 }

}

void Magicup::yjmjCallback(Ref* ref){

	//callback
	int culevel=magic.level;
	int max=ActorHelper::getHelper()->getMaxLevel(magic.unit.ID);

	for(int i=1;i<=max-culevel;i++){
	    ActorHelper::getHelper()->levelUp(magic.unit.ID);
	}
	int magicid=PlayerInfoHelper::getHelper()->getDeafaultMagicItem();
    magic=ActorHelper::getHelper()->getOneMagic(magicid);
	inintJiemian();
	
	StopLayer::checkFightPowerAchievement();
}

void Magicup::sjCallback(Ref* ref){

	ActorHelper::getHelper()->levelUp(magic.unit.ID);
}

int Magicup::getMjyuan(){

	int mj_money=12;
	return mj_money;
}

int Magicup::getsjyuan(){

	int sj_money=3;

	return sj_money;
}


int Magicup::getpj(int dj){

	return dj;
}

int Magicup::getNextpj(int dj){

	return dj;
}


float Magicup::getmfggl(){


	int l=magic.level;
	if(l==0) l=1;
	const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(magic.unit.ID,l);
	float gjl=0.0f;
	gjl=curent_info->attrVec.at(0).value;
	gjl=gjl*100;
	return gjl;
}


float Magicup::getnextmfggl(){

	int l=magic.level;
	if(l==0) l=1;
	int level=l+1;
	if(magic.level==ActorHelper::getHelper()->getMaxLevel(magic.unit.ID)){
		level=l;
	}

	const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(magic.unit.ID,level);
	float gjl=0.0f;
	gjl=curent_info->attrVec.at(0).value;
	gjl=gjl*100;
	return gjl;
	
}

string Magicup::getCurrentleveimg(){

	int l=magic.level;
	if(l==0) l=1;
    const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(magic.unit.ID,l);
	
	return curent_info->imgFile;
	
	
}
string Magicup::getNextleveimg(){

	int l=magic.level;
	if(l==0) l=1;
	int level=l+1;
	if(magic.level==ActorHelper::getHelper()->getMaxLevel(magic.unit.ID)){
		level=magic.level;
	}
	const OneLevelInfo* curent_info= ActorHelper::getHelper()->getOneActorLevelInfo(magic.unit.ID,level);
	return curent_info->imgFile;
	
}

bool Magicup::isMaxlevel(){

	 return magic.level==ActorHelper::getHelper()->getMaxLevel(magic.unit.ID);

}


void Magicup::callback_sj(EventCustom* evt){

	  GameDirector::getDirector()->hideWaiting();
	  
	  
	  MsgData* data=(MsgData*)evt->getUserData();
	  if(data->errorCode==Rsp_Success){
			 int magicid=PlayerInfoHelper::getHelper()->getDeafaultMagicItem();
			 magic=ActorHelper::getHelper()->getOneMagic(magicid);
			 if(isMaxlevel()==true){
				auto p=BasicLayer::create();
				p->maojitishi(104);
				Director::getInstance()->getRunningScene()->addChild(p,100);
			}else{
				auto p=BasicLayer::create();
				p->buystr("sjcg");
				Director::getInstance()->getRunningScene()->addChild(p,100);
		    }

			 levelChangSuccess();
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


void Magicup::playAnimationByname(Node *bs,std::string jsonfile,std::string filename,std::string animationName,bool isloop){

	bs->removeChildByName(magic_armature_img);
	  
	auto armature =Armature::create(filename.c_str());
	if(isloop==true){
		armature->getAnimation()->play(animationName.c_str(),-1,1);
	}
	armature->removeFromParentAndCleanup(true);
	armature->setTag(100);
	armature->setPosition(Vec2(Actual_x*0.35,Actual_y*0.5));
	bs->addChild(armature, 100, magic_armature_img);
	armature->setScale(0.75f);
}

void Magicup::fillOuty(MenuItemImage* p,string imgfile){
	 
	auto s=Sprite::create(imgfile);
	s->setScale(1.05f);
	p->setSelectedImage(s);
}

void Magicup::drawMaojitishi(){

	auto pmj=Sprite::create("UI_jiaose_anniu_shengji_manji.png");
	pmj->setPosition(Vec2(back_hero->getPositionX(),pmj->getContentSize().height/2));
	addChild(pmj);

	auto mj_m1=Label::createWithSystemFont(FX::StringsMap::getValue("magicyimaoji"),"minijiankatong.ttf",24,Size(180,0),TextHAlignment::CENTER);
	mj_m1->setAnchorPoint(Vec2(0.5,0.5));
	mj_m1->setColor(Color3B(184,69,13));
	mj_m1->setPosition(Vec2(pmj->getContentSize().width/2,pmj->getContentSize().height*0.45));
	mj_m1->setLocalZOrder(100);
	pmj->addChild(mj_m1);

	auto mj2=Label::createWithSystemFont(FX::StringsMap::getValue("magicyimaoji"),"minijiankatong.ttf",24,Size(180,0),TextHAlignment::CENTER);
	mj2->setAnchorPoint(Vec2(0.5,0.5));
	mj2->setPosition(Vec2(pmj->getContentSize().width/2,pmj->getContentSize().height*0.45-2));
	mj2->setLocalZOrder(90);
	pmj->addChild(mj2); 

}

void Magicup::levelChangSuccess(){


	if(isMaxlevel()==true){
		if(this->getChildByTag(maoji_tag)!=nullptr){
		   this->getChildByTag(maoji_tag)->setVisible(false);
		}
		drawMaojitishi();
	}
	if(isMaxlevel()==false){
		Vec2 sjv=getItemoney();
		string sj_yuan=__String::createWithFormat("%.0f",sjv.y)->getCString();
		if(sjv.x==unitID_Gold||sjv.x==unitID_Diamond){
			sj_yuan=__String::createWithFormat("%.0f",sjv.y)->getCString();
		}else{
			sj_yuan=__String::createWithFormat("%.0f",sjv.y)->getCString()+FX::StringsMap::getValue("chinsey");
			if(levael_str.sj_p->getChildByTag(125)!=nullptr){
               levael_str.sj_p->getChildByTag(125)->setVisible(false);
			}

		}
		Label* p1= (Label*)levael_str.sj_p->getChildByTag(100);
		
        Label* p2= (Label*)levael_str.sj_p->getChildByTag(200);

		if(p1!=nullptr&&p2!=nullptr){
				p2->setString(sj_yuan);	 
				p1->setString(sj_yuan);
				if(sjv.x==unitID_Gold||sjv.x==unitID_Diamond){

					string qname="UI_zhuye_shangdian_jinbi.png";
					if(sjv.x==unitID_Diamond){
						qname="UI_zhuye_shangdian_zuanshi.png";
					}
					p1->setAnchorPoint(Vec2(0,0.5));
					p1->setPosition(Vec2((levael_str.sj_p->getContentSize().width-61)/2,levael_str.sj_p->getContentSize().height*0.2));
					p2->setAnchorPoint(Vec2(0,0.5));
					p2->setPosition(Vec2(p1->getPositionX(),p1->getPositionY()-2));
					if(levael_str.sj_p->getChildByTag(125)==nullptr){
						auto m_img=Sprite::create(qname);
						m_img->setPosition(Vec2(p1->getPositionX()+p1->getContentSize().width+m_img->getContentSize().width/2,p1->getPositionY()));
						m_img->setTag(125);
						levael_str.sj_p->addChild(m_img);
					}else{
						levael_str.sj_p->getChildByTag(125)->setVisible(true);
					}

				}else{
					p1->setAnchorPoint(Vec2(0.5,0.5));
					p1->setPosition(Vec2(levael_str.sj_p->getContentSize().width*0.5,levael_str.sj_p->getContentSize().height*0.2));
					p2->setAnchorPoint(Vec2(0.5,0.5));
					p2->setPosition(Vec2(p1->getPositionX(),p1->getPositionY()-2));
				}
		}
	}
	magicDescrible.cr_djimg->setTexture(Sprite::create(getCurrentleveimg())->getTexture());
	magicDescrible.nextcr_djimg->setTexture(Sprite::create(getNextleveimg())->getTexture());
	int c_l=magic.level;
	int next_level=c_l+1;
	if(ActorHelper::getHelper()->getMaxLevel(magic.unit.ID)==magic.level){
		next_level=magic.level;
	}

	for(int i=1;i<=ActorHelper::getHelper()->getMaxLevel(magic.unit.ID);i++){

		if(i==c_l){//i>c_l&&i<=next_level&&isMaxlevel()==false){
			    magicDescrible.levelAction.at(i-1)->setTexture(Sprite::create(magic_xinxins_img)->getTexture());
				//magicDescrible.levelAction.at(i-1)->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr)));
		}
		if(i==c_l){
			//magicDescrible.levelAction.at(i-1)->setTexture(Sprite::create(magic_xinxins_img)->getTexture());
			//magicDescrible.levelAction.at(i-1)->stopAllActions();
			//magicDescrible.levelAction.at(i-1)->setOpacity(255);
		}
	}
    
	if(isMaxlevel()){

		for(int i=1;i<=ActorHelper::getHelper()->getMaxLevel(magic.unit.ID);i++){
			magicDescrible.levelAction.at(i-1)->setTexture(Sprite::create(magic_xinxins_img)->getTexture());
			//magicDescrible.levelAction.at(i-1)->stopAllActions();
			magicDescrible.levelAction.at(i-1)->setOpacity(255);
		}
	}

	if(isMaxlevel()==true){

		auto mjts=Sprite::create("UI_jiaose_shuxing_tishi_max.png");
		mjts->setScale(0.8f);
		float suoxiao=mjts->getContentSize().width*0.8f;
		mjts->setPosition(Vec2(magicDescrible.magic_djimg->getPositionX()+magicDescrible.magic_djimg->getContentSize().width/2+magicDescrible.jg*5+5+mjts->getContentSize().width*0.8f/2,back_hero->getContentSize().height*0.60));
		back_hero->addChild(mjts);

	}

	
	string ggl_str=__String::createWithFormat("%.0f",getmfggl())->getCString();
	ggl_str+="%";
	magicDescrible.current_mlabel->setString(ggl_str);
	if(isMaxlevel()==false){


		ggl_str+=" =>";
		magicDescrible.current_mlabel->setString(ggl_str);
		string add_gl="+ ";
		add_gl+=__String::createWithFormat("%.0f",getnextmfggl())->getCString();
		add_gl+="%";
		if(magicDescrible.add_mark==1){
		   magicDescrible.add_label->setString(add_gl);
		}
	}else{
		if(magicDescrible.add_mark==1){
		  magicDescrible.add_label->setString("");
		}
	}
	magicDescrible.miaoshu_label->setString(magic.unit.descrigbe);


	playAnimationByname(this,"book_ui.ExportJson","book_ui", CCString::createWithFormat("b_huxi%d",magic.level)->getCString(),true);
}

void Magicup::levelDraw(){

	
	auto ret_img=MenuItemImage::create(magic_close_img,magic_close_img,CC_CALLBACK_1(Magicup::allcallback,this));
	ret_img->setTag(magic_return_button_tag);
	ret_img->setPosition(Vec2(back_hero->getContentSize().width-ret_img->getContentSize().width/2,back_hero->getContentSize().height-ret_img->getContentSize().height/4));
	fillOuty(ret_img,magic_close_img);

	auto remenu=Menu::create(ret_img,nullptr);
	remenu->setPosition(Vec2::ZERO);
	back_hero->addChild(remenu);

	//magic_next_dj_img
	magicDescrible.cr_djimg=Sprite::create(getCurrentleveimg());
	magicDescrible.cr_djimg->setPosition(Vec2(back_hero->getContentSize().width*0.3,back_hero->getContentSize().height*0.75));
	back_hero->addChild(magicDescrible.cr_djimg);

	auto zhix_img=Sprite::create(magic_next_dj_img);
	zhix_img->setPosition(Vec2(back_hero->getContentSize().width*0.5,back_hero->getContentSize().height*0.75));
	back_hero->addChild(zhix_img);

	magicDescrible.nextcr_djimg=Sprite::create(getNextleveimg());
	magicDescrible.nextcr_djimg->setPosition(Vec2(back_hero->getContentSize().width*0.7,back_hero->getContentSize().height*0.75));
	back_hero->addChild(magicDescrible.nextcr_djimg);

	//等级
	magicDescrible.magic_djimg=Sprite::create(magic_dj_img);
	magicDescrible.magic_djimg->setPosition(Vec2(back_hero->getContentSize().width*0.20,back_hero->getContentSize().height*0.60));
	back_hero->addChild(magicDescrible.magic_djimg);

	int c_l=magic.level;
	int next_level=c_l+1;
	if(ActorHelper::getHelper()->getMaxLevel(magic.unit.ID)==magic.level){
		next_level=magic.level;
	}
	float jg=0;

	for(int i=1;i<=ActorHelper::getHelper()->getMaxLevel(magic.unit.ID);i++){
		string p_img=magic_xinxinx_img;
		if(c_l>=i)
				p_img=magic_xinxins_img;
		
		auto p=Sprite::create(p_img);
		float xp=p->getContentSize().width*0.6f;
		jg=xp;
		magicDescrible.jg=xp;
		p->setScale(0.6f);
		p->setPosition(Vec2(magicDescrible.magic_djimg->getPositionX()+magicDescrible.magic_djimg->getContentSize().width/2+(i-1)*xp+xp/2,
			                back_hero->getContentSize().height*0.6));
		
			if(i>c_l&&i<=next_level){
				//p->setTexture(Sprite::create(magic_xinxins_img)->getTexture());
				//p->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr)));
			}
		back_hero->addChild(p);
		magicDescrible.levelAction.pushBack(p);
	}

	if(isMaxlevel()==true){

		auto mjts=Sprite::create("UI_jiaose_shuxing_tishi_max.png");
		mjts->setScale(0.8f);
		float suoxiao=mjts->getContentSize().width*0.8f;
		mjts->setPosition(Vec2(magicDescrible.magic_djimg->getPositionX()+magicDescrible.magic_djimg->getContentSize().width/2+jg*5+5+mjts->getContentSize().width*0.8f/2,back_hero->getContentSize().height*0.60));
		back_hero->addChild(mjts);

	}

	//魔力
	magicDescrible.ml_djimg=Sprite::create(magic_ml_img);
	magicDescrible.ml_djimg->setPosition(Vec2(back_hero->getContentSize().width*0.20,back_hero->getContentSize().height*0.53));
	back_hero->addChild(magicDescrible.ml_djimg);

	string ggl_str=__String::createWithFormat("%.0f",getmfggl())->getCString();
	ggl_str+="%";
	magicDescrible.current_mlabel=Label::createWithSystemFont(ggl_str,"minijiankatong.ttf",24);
	magicDescrible.current_mlabel->setPosition(Vec2(magicDescrible.ml_djimg->getContentSize().width/2+magicDescrible.ml_djimg->getPositionX()+magicDescrible.current_mlabel->getContentSize().width/2+10,
		                             magicDescrible.ml_djimg->getPositionY()));
	magicDescrible.current_mlabel->setColor(Color3B(184,69,13));
	back_hero->addChild(magicDescrible.current_mlabel);


	if(isMaxlevel()==false){
		ggl_str+=" =>";
		magicDescrible.current_mlabel->setString(ggl_str);
		string add_gl="+ ";
		add_gl+=__String::createWithFormat("%.0f",getnextmfggl())->getCString();
		add_gl+="%";

		magicDescrible.add_mark=1;
		magicDescrible.add_label=Label::createWithSystemFont(add_gl,"minijiankatong.ttf",24);
		magicDescrible.add_label->setColor(Color3B(184,69,13));
		magicDescrible.add_label->setPosition(Vec2(magicDescrible.current_mlabel->getPositionX()+magicDescrible.current_mlabel->getContentSize().width/2+5+magicDescrible.add_label->getContentSize().width/2,
									magicDescrible.ml_djimg->getPositionY()));
		magicDescrible.add_label->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.5f),FadeOut::create(1.5f),nullptr)));
		back_hero->addChild(magicDescrible.add_label);
	}
	//描述
	magicDescrible.miaoshu_label=Label::createWithSystemFont(magic.unit.descrigbe,"minijiankatong.ttf",24,Size(400,0),TextHAlignment::CENTER);
	magicDescrible.miaoshu_label->setAnchorPoint(Vec2(0.5,0.5));
	magicDescrible.miaoshu_label->setPosition(Vec2(back_hero->getContentSize().width*0.5,back_hero->getContentSize().height*0.3));
	back_hero->addChild(magicDescrible.miaoshu_label);
}