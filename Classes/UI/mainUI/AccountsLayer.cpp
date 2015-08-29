#include "AccountsLayer.h"
#include "GameDirector.h"
#include "BagDataManager.h"
#include "ActiveValueHelper.h"
#include "BasicLayer.h"
#include "StopLayer.h"
#include "Font.h"
#include "RunningHelper.h"
#include "TollgateLayer.h"

#include <vector>
#include <string>
#include <iostream> 
#include <random>


#define back_hese_img       "UI_juqing_heimu.png"
#define jiesuan_back_bg_img "UI_jiesuan_bg.png"
#define jiesuan_xiayiju_img "UI_jiesuan_anniu_xiayiju.png"
#define shuzi_lable  "UI_shuzi_04.png"
#define lisizuigao_zs_img "UI_jiesuan_zhuangshi_1.png"




AccountsLayer::AccountsLayer(void)
{
}


AccountsLayer::~AccountsLayer(void)
{
}



void AccountsLayer::onEnter(){

	Layer::onEnter();
}


void AccountsLayer::onExit(){

	Layer::onExit();
}

std::pair<int,vector<AcoundsBag>> AccountsLayer::getPair(int tag,vector<AcoundsBag> bag){

	  pair<int,vector<AcoundsBag>> p_qitem;
	  p_qitem.first=tag;
	  p_qitem.second=bag;
	 
	  return p_qitem;
}

AcoundsBag AccountsLayer::getAcounds(int id,int value,int weight){

	       AcoundsBag b;
		   b.id=id;
		   b.value=value;
		   b.weight=weight;
		   return b;
}

bool AccountsLayer::init(){

	if(!Layer::init()){
		return false;
	}
	counts=0;
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	wardId=0;
	isLinward=false;

	for(int i=5;i<=8;i++){
		if(i==5){
			vector<AcoundsBag> bags;
			bags.push_back(getAcounds(98,500,100));
			acounts.push_back(getPair(i,bags));
		}else if(i==6){
			vector<AcoundsBag> bags;
			bags.push_back(getAcounds(98,1000,50));
			bags.push_back(getAcounds(505,2,50));
			acounts.push_back(getPair(i,bags));
		}else if(i==7){
			vector<AcoundsBag> bags;
			bags.push_back(getAcounds(98,2000,50));
			bags.push_back(getAcounds(510,1,50));
			acounts.push_back(getPair(i,bags));
		}else if(i==8){
			vector<AcoundsBag> bags;
			bags.push_back(getAcounds(98,4000,50));
			bags.push_back(getAcounds(94,1,40));
			bags.push_back(getAcounds(95,1,10));

			acounts.push_back(getPair(i,bags));
		}

	}


	return true;
}





void AccountsLayer::inintAccoutsLayer(unsigned int heroid,unsigned int lsmax,unsigned int biaoxianfen,unsigned int distance,unsigned int thegold){
	 
	/*
	if(distance>=1000){
		ActiveValueHelper::getHelper()->addActiveByType(activeType_Pao1000Mi);
	}
    */


	this->lsmax=lsmax;
	this->biaoxianfen=biaoxianfen;
	this->distance=distance;
	this->thegold=thegold;
	//评分标准

	unsigned int diss[4]={500,1000,2000,3000};
	unsigned int dw[4]={1000,2000,3000,4000};

	if(distance>=dw[0]&&distance<dw[1]){
		wardId=5;
	}else if(distance>=dw[1]&&distance<dw[2]){
		wardId=6;
	}else if(distance>=dw[2]&&distance<dw[3]){
		wardId=7;
	}else if(distance>=dw[3]){
		wardId=8;
	}

	auto color =Color4B(0,0,0,100);
	auto c1 = LayerColor::create(color);
	c1->setContentSize(Size(Actual_x,Actual_y));
    
	//c1=Sprite::create("ui_zhuye_bg.jpg");
	//c1->setPosition(Vec2(Actual_x/2,Actual_y/2));
	c1->setTag(color_tag);
	addChild(c1,1);


	auto js_back=Sprite::create(jiesuan_back_bg_img);
	js_back->setPosition(Vec2(c1->getContentSize().width/2,c1->getContentSize().height*0.6));
	js_back->setTag(backtag);
	c1->addChild(js_back);

	// next_ganme_tag, diaowo_tag,
	auto next_game=MenuItemImage::create(jiesuan_xiayiju_img,jiesuan_xiayiju_img,CC_CALLBACK_1(AccountsLayer::allcallback,this));
	next_game->setTag(next_ganme_tag);
	next_game->setPosition(Vec2(c1->getContentSize().width/2,c1->getContentSize().height*0.15));

	fillOuty(next_game,jiesuan_xiayiju_img);


	auto menu=Menu::create(next_game,nullptr);
	menu->setPosition(Vec2::ZERO);
	menu->setTag(menutag);
	c1->addChild(menu);


	auto gold_num=Sprite::create("UI_zhuye_jinbilan_jinbi.png");
	gold_num->setPosition(Vec2(js_back->getContentSize().width*0.38,js_back->getContentSize().height*0.8));
	js_back->addChild(gold_num);

	auto gold_label=LabelAtlas::create(__String::createWithFormat("%d",thegold)->getCString(), shuzi_lable, 35, 55,'0');  
	gold_label->setAnchorPoint(Vec2(0.0,0.5));
	gold_label->setPosition(Vec2(gold_num->getPositionX()+gold_num->getContentSize().width/2,js_back->getContentSize().height*0.8));
	js_back->addChild(gold_label);


	string dsm_str=__String::createWithFormat("%d",distance)->getCString();
	dsm_str+=FX::StringsMap::getValue("Mi");

	auto dsm_label=Label::createWithSystemFont(dsm_str,"minijiankatong.ttf",30);
	dsm_label->setPosition(Vec2(js_back->getContentSize().width*0.38,js_back->getContentSize().height*0.7));
	js_back->addChild(dsm_label);


	auto dj_jb=ImageHelper::getUnitSpriteUIScene(unitID_JinBi);
	dj_jb->setPosition(Vec2(dsm_label->getPositionX()+dsm_label->getContentSize().width/2+dj_jb->getContentSize().width/2+gold_label->getContentSize().width/4,
		                    js_back->getContentSize().height*0.7));
	js_back->addChild(dj_jb);



	gold_one=Label::createWithSystemFont(__String::createWithFormat("%d",thegold)->getCString(),"minijiankatong.ttf",30);
	gold_one->setAnchorPoint(Vec2(0,0.5));
	gold_one->setPosition(Vec2(dj_jb->getPositionX()+dj_jb->getContentSize().width/2+5,js_back->getContentSize().height*0.7));
	js_back->addChild(gold_one);
	goldidx=0;
	
	BagDataManager::getManager()->changeOneUnitCount(unitID_Gold,thegold);
    
	auto jb2=Sprite::create("UI_zhuye_jinbilan_jinbi.png");
	jb2->setScale(0.65f);
	jb2->setPosition(Vec2(js_back->getContentSize().width*0.35,js_back->getContentSize().height*0.55));
	js_back->addChild(jb2);


	auto hty_label=Label::createWithSystemFont(FX::StringsMap::getValue("lsmaxfs"),"minijiankatong.ttf",30);
	hty_label->setAnchorPoint(Vec2(0,0.5));
	hty_label->setColor(Color3B(190,68,71));
	hty_label->setPosition(Vec2(jb2->getContentSize().width/2+jb2->getPositionX(),js_back->getContentSize().height*0.55));
	js_back->addChild(hty_label);


	auto hy_zs=Sprite::create(lisizuigao_zs_img);
	hy_zs->setAnchorPoint(Vec2(0,0.5));
	hy_zs->setPosition(Vec2(hty_label->getPositionX()+hty_label->getContentSize().width,js_back->getContentSize().height*0.55));
	js_back->addChild(hy_zs);

	auto history_mark=Label::createWithSystemFont(__String::createWithFormat("%d",lsmax)->getCString(),"minijiankatong.ttf",30);
	history_mark->setAnchorPoint(Vec2(0,0.5));
	history_mark->setColor(Color3B(184,69,13));
	history_mark->setPosition(Vec2(hy_zs->getPositionX()+hy_zs->getContentSize().width+5,js_back->getContentSize().height*0.55));
	js_back->addChild(history_mark);


	//descirble
	Actor actor=ActorHelper::getHelper()->getOneHero(heroid);
	auto history_descirble=Label::createWithSystemFont(FX::StringsMap::getValue("jiesuanmiaoshu"),"minijiankatong.ttf",28,Size(320,0),TextHAlignment::CENTER);
	history_descirble->setPosition(Vec2(js_back->getContentSize().width*0.38,js_back->getContentSize().height*0.25));
	history_descirble->setColor(Color3B(184,69,13));
	js_back->addChild(history_descirble);


	if(lsmax==biaoxianfen){

		auto history_mark_biaozi=Sprite::create("UI_jiesuan_lishizuigao.png");
		history_mark_biaozi->setAnchorPoint(Vec2(0,0.5));
		history_mark_biaozi->setPosition(Vec2(gold_label->getPositionX()+gold_label->getContentSize().width+10,js_back->getContentSize().height*0.78));
		js_back->addChild(history_mark_biaozi);
        
	}

	if(wardId==0){

		auto wu_warld=Sprite::create("UI_jiesuan_zhuangshi_3.png");
		wu_warld->setPosition(Vec2(js_back->getContentSize().width*0.7,js_back->getContentSize().height*0.25));
		js_back->addChild(wu_warld);

	}else if(wardId>0){

		auto wu_warld=Sprite::create("UI_jiesuan_zhuangshi_4.png");
		wu_warld->setPosition(Vec2(js_back->getContentSize().width*0.7,js_back->getContentSize().height*0.28));
		js_back->addChild(wu_warld);

		auto dao=MenuItemImage::create("UI_jiesuan_longdan.png","UI_jiesuan_longdan.png",CC_CALLBACK_1(AccountsLayer::allcallback,this));
		dao->setTag(diaowo_tag);
		dao->setPosition(Vec2(wu_warld->getContentSize().width/2,wu_warld->getContentSize().height/2));
		fillOuty(dao,"UI_jiesuan_longdan.png");

		//shouqutext
		

		auto menu=Menu::create(dao,nullptr);
		menu->setPosition(Vec2::ZERO);
		wu_warld->addChild(menu);


		auto diaowo=Sprite::create("UI_zhuye_shangdian_suijidaoju_kuang.png");
		diaowo->setAnchorPoint(Vec2(0.5,1));
		diaowo->setPosition(Vec2(wu_warld->getPositionX(),wu_warld->getPositionY()-wu_warld->getContentSize().height/2+20));
		js_back->addChild(diaowo);

		auto dw_l=Label::createWithSystemFont(FX::StringsMap::getValue("dw"),"minijiankatong.ttf",24);
		dw_l->setPosition(Vec2(diaowo->getContentSize().width/2,diaowo->getContentSize().height/2));
		dw_l->setLocalZOrder(90);
		diaowo->addChild(dw_l);

		auto dw_l2=Label::createWithSystemFont(FX::StringsMap::getValue("dw"),"minijiankatong.ttf",24);
		dw_l2->setPosition(Vec2(diaowo->getContentSize().width/2,diaowo->getContentSize().height/2-2));
		dw_l2->setColor(Color3B(184,69,13));
		dw_l2->setLocalZOrder(80);
		diaowo->addChild(dw_l2);
	}
	
	StopLayer::checkAchievement();
    
}


void AccountsLayer::allcallback(Ref* ref){


	 MenuItemImage* p=(MenuItemImage*)ref;
	 int tag=(int)p->getTag();
	 current_oprate=tag;
	 FX::Music::getInstance()->buttonClicked();
	 
	 p->setCallback(nullptr);
	 if(current_oprate==next_ganme_tag)
	 {
         
		 if(isLinward==false&&wardId>0){
			clearjinlin();
		 }else if(wardId==0){
		    GameDirector::getDirector()->gotoUIScene();
		 }

	 }else if(current_oprate==diaowo_tag){
		   clearjinlin();
	 }else if(current_oprate==shouqutag){
		   BagItem item=getBywarldId(wardId);
		   GoodsHelper::getHelper()->changeOneUnitCount(item.id,item.value);
		   GameDirector::getDirector()->gotoUIScene();
	 }


}


void AccountsLayer::clearjinlin(){

	//this->unscheduleAllSelectors();
	// menutag, backtag,

	if(this->getChildByTag(color_tag)!=nullptr){
	    this->getChildByTag(color_tag)->setVisible(false);
	}

	auto color =Color4B(0,0,0,100);
	auto c1 = LayerColor::create(color);
	c1->setContentSize(Size(Actual_x,Actual_y));
	addChild(c1,1);

 
	auto warld_back=Sprite::create("UI_tanchukuang.png");
	warld_back->setPosition(Vec2(c1->getContentSize().width/2,c1->getContentSize().height*0.7));
	c1->addChild(warld_back);

	auto shouqu=MenuItemImage::create("UI_youxizhong_fuhuo_anniu_bg.png","UI_youxizhong_fuhuo_anniu_bg.png",CC_CALLBACK_1(AccountsLayer::allcallback,this));
	shouqu->setTag(shouqutag);
	shouqu->setPosition(Vec2(c1->getContentSize().width/2,c1->getContentSize().height*0.3));
	fillOuty(shouqu,"UI_youxizhong_fuhuo_anniu_bg.png");

	auto menu=Menu::create(shouqu,nullptr);
	menu->setPosition(Vec2::ZERO);
	c1->addChild(menu);

	auto sq_l=Label::createWithSystemFont(FX::StringsMap::getValue("shouqutext"),"minijiankatong.ttf",30);
    sq_l->setPosition(Vec2(shouqu->getContentSize().width/2,shouqu->getContentSize().height/2));
	shouqu->addChild(sq_l);

	BagItem item=getBywarldId(wardId);
	const Unit* warldu=GoodsHelper::getHelper()->getUnitByID(item.id);

	auto h_name=Label::createWithSystemFont(FX::StringsMap::getValue("jinxi"),"minijiankatong.ttf",40);
	h_name->setColor(Color3B(254,218,185));
	h_name->setPosition(Vec2(warld_back->getContentSize().width/2,warld_back->getContentSize().height*0.8));
	warld_back->addChild(h_name);

	string m_warldText=FX::StringsMap::getValue("jiesuanwarlddaoju");
	auto warld_jl=Label::createWithSystemFont(m_warldText,"minijiankatong.ttf",26);
	warld_jl->setPosition(Vec2(warld_back->getContentSize().width/2,warld_back->getContentSize().height*0.5));
	warld_back->addChild(warld_jl);
    
    
	string img=warldu->imageFile;
	auto daoju_img=Sprite::create(img);
	daoju_img->setPosition(Vec2(warld_back->getContentSize().width/2-daoju_img->getContentSize().width/2,warld_back->getContentSize().height*0.25));
	warld_back->addChild(daoju_img);

	string name_value=" *";
	name_value+=__String::createWithFormat("%d",item.value)->getCString();
	auto er_hangl=Label::createWithSystemFont(name_value,"minijiankatong.ttf",30);
	er_hangl->setPosition(Vec2(daoju_img->getPositionX()+daoju_img->getContentSize().width/2+er_hangl->getContentSize().width/2+10,warld_back->getContentSize().height*0.25));
	warld_back->addChild(er_hangl);
}

BagItem AccountsLayer::getBywarldId(int id){

	if(counts==0){
		BagItem item;
		 vector<AcoundsBag> p;
		 for(auto m:acounts){
			 if(m.first==id){
				 p=m.second;
				 break;
			 }
		 }
		 int countswight=0;
		 for(auto m:p){
			 countswight+=m.weight;
		 }
		 std::mt19937 eng(time(nullptr));
		 std::uniform_int_distribution<int> dist(1,countswight);
		 int weight = dist(eng);
		 int total = 0;
		 for (int i=0,count=p.size(); i<count; ++i)
		 {
				total += p.at(i).weight;
				if (total >= weight){
					item.id = p.at(i).id;
					item.value = p.at(i).value;
					break;
				}
		 }
		counts++;
		pbag.id=item.id;
		pbag.value=item.value;
        return item;
	 }else{
		 return pbag;
	 }
}





void AccountsLayer::updateGold(float time){

	if(goldidx<thegold){
	    goldidx=goldidx+1;
		string lable_str=__String::createWithFormat("%d",goldidx)->getCString();
		gold_one->setString(lable_str);
	}else{
	    this->unschedule(schedule_selector(AccountsLayer::updateGold));
		BagDataManager::getManager()->changeOneUnitCount(unitID_Gold,thegold);
	}
}



void AccountsLayer::fillOuty(MenuItemImage* p,string imgfile){
	 
	auto s=Sprite::create(imgfile);
	s->setScale(1.05f);
	p->setSelectedImage(s);
}












//挑战关卡结算界面

TollgateAccountLayer::TollgateAccountLayer()
{
}

TollgateAccountLayer::~TollgateAccountLayer()
{
}


void TollgateAccountLayer::onEnter(){
	Layer::onEnter();

}


void TollgateAccountLayer::onExit(){

	Layer::onExit();
}

TollgateAccountLayer* TollgateAccountLayer::create(unsigned int jewel, TollgateInfor::TollgatePassLevel passLv, TollgateInfor::TollgatePassLevel oldPassLv)
{
	TollgateAccountLayer* pLayer = new TollgateAccountLayer();
	if (pLayer && pLayer->init(jewel, passLv, oldPassLv))
	{
		pLayer->autorelease();
		return pLayer;
	}
	CC_SAFE_DELETE(pLayer);
	return pLayer;
}

bool TollgateAccountLayer::init(unsigned int jewel, TollgateInfor::TollgatePassLevel passLv, TollgateInfor::TollgatePassLevel oldPassLv)
{
	if (!Layer::init())
	{
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch* pTouch, Event* pEvent)
	{
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	m_passLv = passLv;
	m_bUp = false;
	auto pBg = Sprite::create(jiesuan_back_bg_img);
	pBg->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height*0.6f));
	pBg->setTag(backtag);
	this->addChild(pBg);
	m_pUp = nullptr;
	//成绩提高了
	if (oldPassLv==TollgateInfor::eUnfinish?false:(passLv>oldPassLv))
	{
		m_bUp = true;
		m_pUp = Sprite::create("UI_jiesuan_guanqia_chengjitigaole.png");
		m_pUp->setPosition(Vec2(pBg->getContentSize().width-150, pBg->getContentSize().height - 102));
		pBg->addChild(m_pUp);
	}
	float xoffset = 10;
	float scale = 0.85f;
	string star_bg_filename;
	if (RunningHelper::getHelper()->getCurDifficutLevel() == Section::eNormal){
		star_bg_filename = "UI_jiesuan_star_bg.png";
	}
	else if (RunningHelper::getHelper()->getCurDifficutLevel() == Section::eDifficult){
		star_bg_filename = "UI_jiesuan_kunnan_star_bg.png";
	}
	else{ 
		CCASSERT(false, "");
	}
	for (size_t i = 0; i < 3; i++)
	{
		Sprite* pStarBg = Sprite::create(star_bg_filename);
		pStarBg->setTag(star_bg1_tag+i);
		pBg->addChild(pStarBg);

		if (i == 0)
		{
			pStarBg->setScale(scale);
			pStarBg->setPosition(Vec2(pBg->getContentSize().width/2-30 - pStarBg->getContentSize().width*pStarBg->getScale() - xoffset, pBg->getContentSize().height - pStarBg->getContentSize().height/2));
			pStarBg->setRotation(-12);
		}
		else if (i == 1)
		{
			pStarBg->setPosition(Vec2(pBg->getContentSize().width/2-30, pBg->getContentSize().height - pStarBg->getContentSize().height/2  + 25));
		}
		else if (i == 2)
		{
			pStarBg->setScale(scale);
			pStarBg->setPosition(Vec2(pBg->getContentSize().width/2-30 + pStarBg->getContentSize().width*pStarBg->getScale() + xoffset, pBg->getContentSize().height - pStarBg->getContentSize().height/2));
			pStarBg->setRotation(12);
		}
	}

	//显示奖励
	vector<std::pair<UnitID, unsigned int>> awardList;//奖励 （通过评级来计算）
	//计算奖励
	float fProb = CCRANDOM_0_1();
	const Section* pSectionInfor = TollgateHelper::getHelper()->getOneTollgateSection(RunningHelper::getHelper()->getCurSeasonId(), RunningHelper::getHelper()->getCurSectionId());
	const vector<Section::award>& pAward = pSectionInfor->levels[RunningHelper::getHelper()->getCurDifficutLevel()].awardList;
	for (size_t iAward=0; iAward<pAward.size(); ++iAward)
	{
		if (pAward[iAward].probability[passLv-1] >= fProb){
			awardList.push_back(std::make_pair((UnitID)pAward[iAward].awardUnitID, pAward[iAward].awardUnitCount));
		}
	}
	//加入绿宝石
	awardList.push_back(std::make_pair(unitID_RedJewel, jewel));
	//1. 第一次困难必出角色碎片
	if (RunningHelper::getHelper()->getCurDifficutLevel() == Section::eDifficult && oldPassLv == TollgateInfor::eUnfinish)
	{
		bool bFind = false;
		for (int i=0,count=awardList.size(); i<count; ++i){
			if (awardList[i].first == unitID_Actor2Fragment){
				bFind = true;
				break;
			}
		}
		if (!bFind){
			awardList.push_back(std::make_pair(unitID_Actor2Fragment, 1));
		}
	}
	if(PlayerInfoHelper::getHelper()->getOneInfo("isFirstTongGuanTangguo")== 0 && oldPassLv == TollgateInfor::eUnfinish)
	{
		PlayerInfoHelper::getHelper()->setOneInfo("isFirstTongGuanTangguo",1);
		awardList.push_back(std::make_pair(unitID_TangGuo, 1));
	}

	//2. 已有角色，或角色碎片满了，那么产出的碎片替换为食物
	if (ActorHelper::getHelper()->getIsUnlockItem(unitID_Actor2) || 
		(GoodsHelper::getHelper()->getActorFragmentNum(unitID_Actor2)>=ActorHelper::getHelper()->getUnlockHeroNeedFragmentAndGold(unitID_Actor2).first))
	{
		for (int i=0,count=awardList.size(); i<count; ++i){
			if (awardList[i].first == unitID_Actor2Fragment){
				int randNum = rand() % 4;
				if (randNum == 0) awardList[i].first = unitID_DanGao;
				else if (randNum == 1) awardList[i].first = unitID_GuoDong;
				else if (randNum == 2) awardList[i].first = unitID_TangGuo;
				else awardList[i].first = unitID_Diamond;
				break;
			}
		}
	}
	//3. 连续5次没获得任何奖励，则在第五次必出一个食物
	static int s_notHaveAwardTimes = 0;
	if (awardList.size() == 1){
		++s_notHaveAwardTimes;
	}else {
		s_notHaveAwardTimes = 0;
	}
	if (s_notHaveAwardTimes >= 5){
		s_notHaveAwardTimes = 0;
		UnitID temp;
		int randNum = rand() % 3;
		if (randNum == 0) temp = unitID_TangGuo;
		else if (randNum == 1) temp = unitID_DanGao;
		else temp = unitID_GuoDong;
		awardList.push_back(std::make_pair(temp, 1));
	}

// 	//test
// 	awardList.clear();
// 	awardList.push_back(std::make_pair(unitID_Actor2Fragment, 1));
// 	awardList.push_back(std::make_pair(unitID_DanGao, 1));
// 	awardList.push_back(std::make_pair(unitID_RedJewel, 1));
// 	awardList.push_back(std::make_pair(unitID_Diamond, 1));
	for (int i=0,count=awardList.size(); i<count; ++i)
	{
		//放入背包
		GoodsHelper::getHelper()->changeOneUnitCount(awardList[i].first, awardList[i].second);
		//UI
		const Unit* pUnit = GoodsHelper::getHelper()->getUnitByID(awardList[i].first);
		Sprite* pIcon = Sprite::create(pUnit->imageFile);
		pBg->addChild(pIcon);
		if (awardList[i].first == unitID_RedJewel) pIcon->setScale(scale);
 		//Sprite* pIcon1 = Sprite::create("UI_jiesuan_zhuangshi_1.png");
 		//pIcon1->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
 		//pBg->addChild(pIcon1);
		Label* pNumTTF = Label::createWithSystemFont(__String::createWithFormat("*%d", awardList[i].second)->getCString(), Font_TTF_KaTong, 30);
		pNumTTF->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
		pNumTTF->setColor(Font_Color_Store01);
		pBg->addChild(pNumTTF);
// 		pIcon->setPosition(Vec2(pBg->getContentSize().width/2 - (pIcon->getContentSize().width+/*pIcon1->getContentSize().width+*/pNumTTF->getContentSize().width)/2 , 230));
// 		//pIcon1->setPosition(Vec2(pIcon->getPositionX() + pIcon->getContentSize().width/2 + 10, pIcon->getPositionY()));
// 		pNumTTF->setPosition(Vec2(pIcon->getPositionX() + pIcon->getContentSize().width*pIcon->getScale() /*+ pIcon1->getContentSize().width*/, pIcon->getPositionY() + 5));
		pIcon->setPosition(pBg->getContentSize().width/2 + (i-(count-1)/2.0f)*120, 230);
		pNumTTF->setPosition(pIcon->getPositionX() + pIcon->getContentSize().width/2, pIcon->getPositionY() + 5);
	}

	Menu* pMenu = Menu::create();
	pBg->addChild(pMenu);
	pMenu->setPosition(Point::ZERO);
	MenuItemImage* pFacebook = MenuItemImage::create(
		"UI_jiesuan_guanqia_anniu_facebook.png",
		"UI_jiesuan_guanqia_anniu_facebook.png",
		"",
		CC_CALLBACK_1(TollgateAccountLayer::menuCallback, this));
	pFacebook->setTag(facebook_tag);
	pFacebook->setPosition(Vec2(pBg->getContentSize().width/2, 30 + pFacebook->getContentSize().height/2));
	pMenu->addChild(pFacebook);
	Label* pShareTTF = Label::createWithSystemFont(FX::StringsMap::getValue("sharedToFacebook"), Font_TTF_KaTong, 30);
	pBg->addChild(pShareTTF);
	pShareTTF->setPosition(Vec2(pFacebook->getPositionX(), pFacebook->getPositionY() + pFacebook->getContentSize().height/2 + pShareTTF->getContentSize().height/2));
	pShareTTF->setColor(Font_Color_Store01);
	
	pMenu = Menu::create();
	pMenu->setPosition(Point::ZERO);
	this->addChild(pMenu);
	xoffset = 50.0f;
	//选关
	MenuItemImage* pSelectLevel = MenuItemImage::create(
		"UI_jiesuan_guanqia_anniu_guanqia.png",
		"UI_jiesuan_guanqia_anniu_guanqia.png",
		"",
		CC_CALLBACK_1(TollgateAccountLayer::menuCallback, this));
	pSelectLevel->setTag(selectlevel_tag);
	pSelectLevel->setPosition(Vec2(pBg->getPositionX(), pBg->getPositionY() - pBg->getContentSize().height/2 - 30 - pSelectLevel->getContentSize().height/2));
	pMenu->addChild(pSelectLevel);
	//重玩
	MenuItemImage* pRestart = MenuItemImage::create(
		"UI_jiesuan_guanqia_anniu_restart.png",
		"UI_jiesuan_guanqia_anniu_restart.png",
		"",
		CC_CALLBACK_1(TollgateAccountLayer::menuCallback, this));
	pRestart->setTag(restart_tag);
	pRestart->setPosition(Vec2(pSelectLevel->getPositionX() - pSelectLevel->getContentSize().width/2 - pRestart->getContentSize().width/2 - xoffset, 
		pSelectLevel->getPositionY()));
	pMenu->addChild(pRestart);
	//下一关
	MenuItemImage* pNextLevel = MenuItemImage::create(
		"UI_jiesuan_guanqia_anniu_xiayiguan.png",
		"UI_jiesuan_guanqia_anniu_xiayiguan.png",
		"",
		CC_CALLBACK_1(TollgateAccountLayer::menuCallback, this));
	pNextLevel->setTag(nextlevel_tag);
	pNextLevel->setPosition(Vec2(pSelectLevel->getPositionX() + pSelectLevel->getContentSize().width/2 + pNextLevel->getContentSize().width/2 + xoffset,
		pSelectLevel->getPositionY()));
	pMenu->addChild(pNextLevel);

	return true;
}

void TollgateAccountLayer::showStarAnimation()
{
	float beginScale, endScale, rotate;

	for (int i=0,count=m_passLv; i<count; ++i)
	{
		if (i==1){
			beginScale = 1.5f;
			endScale = 1.f;
			rotate = 0;
		}else {
			beginScale = 1.5*0.85f;
			endScale = 0.85f;
			rotate = -12 + i*12;
		}

		Sprite* pStar = Sprite::create("UI_jiesuan_star.png");
		pStar->setScale(beginScale);
		this->getChildByTag(backtag)->addChild(pStar, 10);
		pStar->setPosition(pStar->getParent()->getChildByTag(star_bg1_tag+i)->getPosition());
		pStar->setRotation(rotate);
		pStar->setVisible(false);

		float time = 0.2f;
		pStar->runAction(Sequence::create(
			DelayTime::create(0.1f + i*0.2f),
			Show::create(),
			ScaleTo::create(time, endScale),
			CallFuncN::create(CC_CALLBACK_1(TollgateAccountLayer::showRandStars, this)),
			nullptr));
	}
}

void TollgateAccountLayer::showRandStars(Node* pParent)
{
	//设置随机种子
	srand((size_t)time(nullptr));
	int iStarCount = 11;//this->randStarCount();

	Point ppoint = pParent->getPosition();

	std::vector<int> angleVector;
	angleVector.push_back(20);
	angleVector.push_back(-45);
	angleVector.push_back(90);
	angleVector.push_back(-360);
	angleVector.push_back(123);
	angleVector.push_back(-164);
	angleVector.push_back(209);
	angleVector.push_back(294);
	angleVector.push_back(-279);
	angleVector.push_back(329);
	angleVector.push_back(340);

	for (int i = 0; i < iStarCount; ++i)
	{
		Sprite* pStar = Sprite::create("UI_jiesuan_star.png");
		pStar->setScale(this->randScale());
		pStar->setRotation(this->randRotation());
		pParent->getParent()->addChild(pStar, pParent->getLocalZOrder()-1);
		pStar->setPosition(Vec2(ppoint.x , ppoint.y));

		Point point;

		float angle = angleVector.at(i);//CC_RADIANS_TO_DEGREES(angleVector.at(i));
		float ds = 200;
		point.x = sin(angle)*ds;
		point.y = cos(angle)*ds;

		pStar->setOpacity(180);
		pStar->runAction(RepeatForever::create(RotateBy::create(0.035f, 180)));
		pStar->runAction(Sequence::create(
			Spawn::create(
			EaseIn::create(MoveBy::create(0.4f, point), 0.45f),
			nullptr),
			RemoveSelf::create(),
			nullptr));
	}

}

int TollgateAccountLayer::randStarCount()
{
	int iRet = 8;

	int max = 15;
	int min = iRet;
	iRet = rand()%(max-min) + min;

	return iRet;
}

float TollgateAccountLayer::randScale()
{
	float fRet = 0.5f;
	float maxScale = 0.4f;
	float minScale = 0.2f;
	fRet = CCRANDOM_0_1()*(maxScale-minScale) + minScale;

	return fRet;
}

float TollgateAccountLayer::randRotation()
{
	float fRet = 0.0f;
	float maxScale = 360.0f;
	float minScale = 0.0f;
	fRet = CCRANDOM_0_1()*(maxScale-minScale) + minScale;

	return fRet;
}

PointArray* TollgateAccountLayer::randStarPoint(int iCount)
{
	PointArray* pointRet = PointArray::create(iCount);

	pointRet->addControlPoint(Vec2(60, 210));
	pointRet->addControlPoint(Vec2(80, 330));
	pointRet->addControlPoint(Vec2(115, 450));
	pointRet->addControlPoint(Vec2(270, 570));
	pointRet->addControlPoint(Vec2(230, 100));
	pointRet->addControlPoint(Vec2(425, 550));
	pointRet->addControlPoint(Vec2(550, 410));
	pointRet->addControlPoint(Vec2(540, 180));

	return pointRet;
}

void TollgateAccountLayer::menuCallback(Ref* ref)
{
	MenuItemImage* p = (MenuItemImage*)ref;
	int tag = p->getTag();

	if (tag == facebook_tag)
	{
		CCLOG("facebook_btn_tag");
	}
	else if (tag == selectlevel_tag)
	{
		int curSeasonId = RunningHelper::getHelper()->getCurSeasonId();
		int curSectionId = RunningHelper::getHelper()->getCurSectionId();
		Section::Difficulty curLv = RunningHelper::getHelper()->getCurDifficutLevel();
		int count = TollgateHelper::getHelper()->getOneTollgateSeason(curSeasonId)->sectionList.size();
		if(curSectionId == count && Section::eNormal==curLv && !TollgateHelper::getHelper()->isGotAward(curSeasonId, curSectionId, Section::eNormal)){
			TollgateLayer::setAwardIllustration();
		}
		GameDirector::getDirector()->loadingToUIScene(RunningMode::tollgate);
	}
	else if (tag == nextlevel_tag)
	{
		int curSeasonId = RunningHelper::getHelper()->getCurSeasonId();
		int curSectionId = RunningHelper::getHelper()->getCurSectionId();
		Section::Difficulty curLv = RunningHelper::getHelper()->getCurDifficutLevel();
		int count = TollgateHelper::getHelper()->getOneTollgateSeason(curSeasonId)->sectionList.size();
		//不是该章节最后一关，且通关了
		if (curSectionId < count){
			GameDirector::getDirector()->loadingToRunningScene(RunningMode::tollgate, curSeasonId, curSectionId+1, curLv);
		}
		//该章节最后一关，且没获得插画  ---> 跳出去 显示章节插画
		else if(curSectionId == count && Section::eNormal==curLv && !TollgateHelper::getHelper()->isGotAward(curSeasonId, curSectionId, Section::eNormal)){
			TollgateLayer::setAwardIllustration();
			GameDirector::getDirector()->loadingToUIScene(RunningMode::tollgate);
		}
		//该章节最后一关，且已有插画, 且还有下一章
		else if (curSectionId == count && TollgateHelper::getHelper()->isGotAward(curSeasonId, curSectionId, Section::eNormal)
			&& curSeasonId < TollgateHelper::getHelper()->getAllTollgate()->size()){
			GameDirector::getDirector()->loadingToRunningScene(RunningMode::tollgate, curSeasonId+1, 1, curLv);
		}else {
			GameDirector::getDirector()->loadingToUIScene(RunningMode::tollgate);
		}
	}
	else if (tag == restart_tag)
	{
		GameDirector::getDirector()->loadingToRunningScene(RunningMode::tollgate, 
			RunningHelper::getHelper()->getCurSeasonId(),
			RunningHelper::getHelper()->getCurSectionId(),
			RunningHelper::getHelper()->getCurDifficutLevel());
	}
}