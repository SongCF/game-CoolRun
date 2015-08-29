#include "StateBar.h"
#include "RunningHelper.h"
#include "BaseSprite.h"
#include "RunningHelper.h"
#include "GameDirector.h"
#include "StringsMap.h"
#include "MsgDefine.h"
#include "HeroSprite.h"
#include "AccountsLayer.h"
#include "PlayerInfoHelper.h"
#include "cocos2d.h"
#include "TollgateHelper.h"
#include "GoodsHelper.h"
#include "extensions/cocos-ext.h"
#include "StringsMap.h"
#include "GameUnitLayer.h"
#include "StopLayer.h"
#include "Dialog.h"
#include "VersionConfig.h"
#include "Font.h"
#include "TalkingLayer.h"

//USING_NS_CC_EXT;
using namespace FX;

bool StateBar::init()
{
	if( !Layer::init()) return false;
	m_lMenuItemList.clear();
	m_bStartScoreControl = true;
	m_fGoldScoreValue = 0;
	m_iUseProtectTimes = 0;
	m_iPauseTimes = 0;
	m_winSize = Director::getInstance()->getWinSize();

	Menu *menu = Menu::create();
	this->addChild(menu);
	menu->setPosition(Vec2::ZERO);

	MenuItemImage *item = MenuItemImage::create(
		"UI_youxizhong_zanting_zanniu_zanting.png",
		"UI_youxizhong_zanting_zanniu_zanting.png",
		"UI_youxizhong_zanting_zanniu_zanting.png",
		std::bind(&StateBar::onPause, this, std::placeholders::_1));
	menu->addChild(item);
	item->setPosition(m_winSize.width-item->getContentSize().width/2 -20, m_winSize.height-item->getContentSize().height/2-20);
	m_lMenuItemList.push_back(item);

	Node* pHPNode = Node::create();
	this->addChild(pHPNode, 0, Tag_HPNode);
	pHPNode->setAnchorPoint(Vec2(0.0f, 0.5f));
	pHPNode->setContentSize(Size(0, 64));
	pHPNode->setPosition(Vec2(0, m_winSize.height - pHPNode->getContentSize().height/2));

	updateHPIconCount(RunningHelper::getHelper()->getHelper()->getRunnerHero()->getCurHP());

	Sprite* pDaoJu_Hudun = createDaoJu(Tag_DaoJu_Protect);
	if (pDaoJu_Hudun)
	{
		pDaoJu_Hudun->setPosition(Vec2(pDaoJu_Hudun->getContentSize().width/2, pHPNode->getPositionY()-pHPNode->getContentSize().height/2 - pDaoJu_Hudun->getContentSize().height/2 - 10));
	}

	Sprite *sScore;
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::tollgate)
		sScore = Sprite::create("UI_guanqia_baoshilan_lvbaoshi.png");
	else if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless)
		sScore = Sprite::create("UI_youxizhong_jinbi.png");
	else if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::teach)
		sScore = Sprite::create("UI_youxizhong_jinbi.png");
	else 
		CCASSERT(false, "");

	this->addChild(sScore);
	sScore->setPosition(m_winSize.width/2 - sScore->getContentSize().width/2 - 5, m_winSize.height -sScore->getContentSize().height/2 - 10);
	m_scoreLabel = LabelAtlas::create("0", "UI_shuzi_04.png", 35, 55, '0');
	sScore->addChild(m_scoreLabel);
	m_scoreLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_scoreLabel->setPosition(sScore->getContentSize().width+10, sScore->getContentSize().height/2);
	m_scoreLabel->setString("0");
//	m_scoreLabel->setScale(0.45f);

	//不显示米数
// 	BaseSprite *sMeter = BaseSprite::create("ui_sence_dinglan_juli.png");
// 	this->addChild(sMeter);
// 	sMeter->setPosition(m_winSize.width/2, m_winSize.height - sMeter->getContentSize().height/2-10);
// 	m_meterLabel = LabelAtlas::create("0", "ui_shuzi_huangse.png", 42, 70, '0');
// 	sMeter->addChild(m_meterLabel);
// 	m_meterLabel->setAnchorPoint(Vec2(1, 0.5f));
// 	m_meterLabel->setPosition(sMeter->getContentSize().width-45, sMeter->getContentSize().height/2);
// 	m_meterLabel->setString("0");
// 	m_meterLabel->setScale(0.45f);

	Sprite *controlBgSpr = Sprite::create("UI_youxizhong_chaojijiangli_bg.png");
	controlBgSpr->setPosition(this->getContentSize().width / 2 - 30,45);
	this->addChild(controlBgSpr, 1,Tag_ControlBgSpr);

// 	Sprite *xingSpr = Sprite::create("ui_sence_chaojijiangli_jindutiao_star.png");
// 	xingSpr->setPosition(controlBgSpr->getPositionX() - controlBgSpr->getContentSize().width / 2 + 35,45);
// 	this->addChild(xingSpr,2,Tag_XingSpr);
// 	controlBgSpr->setPositionX(controlBgSpr->getPositionX() + 63);

	ProgressTimer * hpTimer = ProgressTimer::create(Sprite::create("UI_youxizhong_chaojijiangli_jindutiao.png"));
	hpTimer->setType(ProgressTimer::Type::BAR);
	hpTimer->setMidpoint(Vec2(0, 0.5f));
	hpTimer->setBarChangeRate(Vec2(1,0));
	hpTimer->setPercentage(100.f);
	hpTimer->setAnchorPoint(Vec2(0,0.5f));
	hpTimer->setPosition(79, hpTimer->getContentSize().height+10);
	controlBgSpr->addChild(hpTimer, 1,Tag_ControlBgSpr);
	m_controlSprite = hpTimer;
// 	Label *label = Label::createWithSystemFont(CCString::create("0%")->getCString(),"Arial",24.0f);
// 	label->setPosition(controlBgSpr->getContentSize() / 2);
// 	controlBgSpr->addChild(label,1,Tag_ScoreValue);

// 	Label *label1 = Label::createWithSystemFont(CCString::createWithFormat("(0/%d)", g_uSuperModeSwitchScore)->getCString(),"Arial",24.0f);
// 	label1->setPosition(Vec2(label->getPositionX(), label->getPositionY() - 30));
// 	controlBgSpr->addChild(label1,1,Tag_ScoreValue1);

	if (RunningHelper::getHelper()->isTeachScene())
	{
		menu->setVisible(false);
		for (vector<MenuItem*>::iterator iter = m_lMenuItemList.begin() ; iter != m_lMenuItemList.end() ; iter ++)
		{
			(*iter)->setVisible(false);
		}

		setDaoJuMenuVisable(Tag_DaoJu_Protect, false);
	}
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::tollgate
		|| RunningHelper::getHelper()->getCurrentScenePlayMode() == Scene_PlayMode::mode_Fight)
	{
		this->showScoreBar(false);
	}


	if (1 == DEBUG_uGameInAttrMenu) initDebugMenuBar();
	if (1 == DEBUG_uGameInDebugInfo) initDebugInfo();

	return true;
}
void StateBar::onEnter()
{
	Layer::onEnter();

	//
	this->scheduleUpdate();
	//
	EventDispatcher* pDisp = Director::getInstance()->getEventDispatcher();
	{
		EventListenerCustom *listener = EventListenerCustom::create(MSG_HeroDead, std::bind(&StateBar::onMsgHeroDead, this, std::placeholders::_1));
		pDisp->addEventListenerWithSceneGraphPriority(listener, this);
		m_lListenerList.push_back(listener);
	}
	{
		EventListenerCustom *listener = EventListenerCustom::create(MSG_HeroWin, std::bind(&StateBar::onMsgHeroWin, this, std::placeholders::_1));
		pDisp->addEventListenerWithSceneGraphPriority(listener, this);
		m_lListenerList.push_back(listener);
	}
	{
		auto callBack = [&](EventCustom* evt)  
		{
			if (Node* pNode = this->getChildByTag(Tag_DaoJu_Protect))
			{
				LabelAtlas *label = (LabelAtlas*)pNode->getChildByTag(Tag_DaoJu_Count_Bg)->getChildByTag(Tag_DaoJu_Label);
				const BagItem *bagItem = GoodsHelper::getHelper()->getOneItem(unitID_HuDun);
				int number = 0;
				if (bagItem)
				{
					number = bagItem->value;
					if (number < 1 )
					{
						pNode->removeFromParent();
					}
					else
					{
						label->setString(String::createWithFormat("%d",number)->getCString());
					}
				}
				else
				{
					pNode->removeFromParent();
				}
			}
		};  
		auto Gold_changevent=EventListenerCustom::create(MSG_BagDataChanged,callBack);
		m_lListenerList.push_back(Gold_changevent);
		pDisp->addEventListenerWithFixedPriority(Gold_changevent,1);  
	}
	//keyboard
	{
		EventListenerKeyboard *listener = EventListenerKeyboard::create();
		listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
			if (EventKeyboard::KeyCode::KEY_BACK == keyCode && RunningHelper::getHelper()->isGameIn()){
				this->onPause(nullptr);
			}
		};
		pDisp->addEventListenerWithSceneGraphPriority(listener, this);
	}
}

void StateBar::onExit()
{
	//
	this->unscheduleUpdate();
	for (vector<EventListenerCustom*>::iterator iter = m_lListenerList.begin() ; iter != m_lListenerList.end() ; iter++)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(*iter);
	}
	m_lListenerList.clear();

	Layer::onExit();
}
void StateBar::update(float dt)
{
	string temp;

	//score label
	temp = StringUtils::format("%d", RunningHelper::getHelper()->getScore());
	m_scoreLabel->setString(temp);
	//score bar
	if (m_bStartScoreControl)
	{
		updateGoldBarValue();
	}

	//meter label
// 	temp = StringUtils::format("%d", RunningHelper::getHelper()->getMeter());
// 	m_meterLabel->setString(temp);

}
void StateBar::updateGoldBarValue()
{
	float value = m_fGoldScoreValue / g_uSuperModeSwitchScore;
	if (value >= 1.0f)
	{
		if (RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Common)
		{
			HeroSprite* hero = RunningHelper::getHelper()->getRunnerHero();
			hero->setWillSuperRun(true);
// 			if (hero->getCurState() != HeroSprite::state_ChongCi)
// 				hero->willStartSuperPlay();

			value = 0;
			m_bStartScoreControl = false;
		}
	}
	m_controlSprite->setPercentage(value * 100.0f);
	//m_controlSprite->setTextureRect(Rect(0,0,m_controlSprite->getParent()->getContentSize().width * value,m_controlSprite->getContentSize().height));
// 	Label *label = (Label*)this->getChildByTag(Tag_ControlBgSpr)->getChildByTag(Tag_ScoreValue);
// 	label->setString(CCString::createWithFormat("%.0f%%",value * 100)->getCString());

// 	Label *label1 = (Label*)this->getChildByTag(Tag_ControlBgSpr)->getChildByTag(Tag_ScoreValue1);
// 	label1->setString(CCString::createWithFormat("(%s/%d)", m_scoreLabel->getString().c_str(), g_uSuperModeSwitchScore)->getCString());
// 
}
void StateBar::startGoldValue(bool refreshValue)
{
	m_bStartScoreControl = true;
	if (refreshValue){
		m_fGoldScoreValue = 0;
		//m_uStartScore = RunningHelper::getHelper()->getScore();
	}
}

void StateBar::onPause(Ref* obj)
{
	if (!RunningHelper::getHelper()->isGameIn() || RunningHelper::getHelper()->getRunnerHero()->getCurState() == HeroSprite::state_Dead)
	{
		return;
	}

	++m_iPauseTimes;
	if (FX::Music::getInstance()->isMusicEnable())
		FX::Music::getInstance()->pauseBackgroundMusic();

	Scene *lastScene = Director::getInstance()->getRunningScene();

	Scene *pScene = Scene::create();
	StopLayer *layer = StopLayer::create();
	layer->showPauseLayer();
	pScene->addChild(layer, 10, TagName_StopLayer);
	layer->setLastScene(lastScene);
	Director::getInstance()->pushScene(pScene);

	RunningHelper::getHelper()->setGameIn(false);
}
void StateBar::onProtect(Ref* obj)
{
	MenuItem *item = (MenuItem *)obj;
	if (RunningHelper::getHelper()->getRunnerHero()->isHaveAttrOnGoing(Attr_Protect))
	{
		return;
	}
	const BagItem *bagItem = GoodsHelper::getHelper()->getOneItem(unitID_HuDun);
	if (bagItem)
	{
		if (bagItem->value >= 1 )
		{
			++m_iUseProtectTimes;
			setProtectMenuEnable(false);
			GoodsHelper::getHelper()->useOneGoods(unitID_HuDun);
			RunningHelper::getHelper()->getRunnerHero()->startAttribute(Attribute(Attr_Protect, 20.0f));

			//检测防御相关成就
			Achieve* info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_FangYuI);
			if (info && !info->isOver)
			{
				AchievementHelper::getHelper()->completeOneAchievement(achieveID_FangYuI, 1);
			}
			info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_FangYuII);
			if (info && !info->isOver)
			{
				AchievementHelper::getHelper()->completeOneAchievement(achieveID_FangYuII, 1);
			}
			info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_FangYuIII);
			if (info && !info->isOver)
			{
				AchievementHelper::getHelper()->completeOneAchievement(achieveID_FangYuIII, 1);
			}
			info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_FangYuIV);
			if (info && !info->isOver)
			{
				AchievementHelper::getHelper()->completeOneAchievement(achieveID_FangYuIV, 1);
			}
		}
	}
}

void StateBar::onMsgHeroDead(EventCustom* msg)
{
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless){
//		StopLayer* pDieStopLayer = StopLayer::create();
//		pDieStopLayer->setLastScene(Director::getInstance()->getRunningScene());
//		Scene* pStopScene = Scene::create();
//		pStopScene->addChild(pDieStopLayer);
//		Director::getInstance()->pushScene(pStopScene);
//		this->addChild(pDieStopLayer);
//		this->updateDeadLayer();
	}
	else if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::tollgate){
		this->tollgateModeEnd(false);
	}
	else if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::teach){
		CCASSERT(false, "why teach failed?!!!");
	}
	else {
		CCASSERT(false, "error branch");
	}
}
void StateBar::onMsgHeroWin(EventCustom* msg)
{
	//教学
	if (RunningHelper::getHelper()->isTeachScene()){

		PlayerInfoHelper::getHelper()->setOneInfo(History_AlreadyHasLogin, 1);
		if (PlayerInfoHelper::getHelper()->getTeacher() == 0) 
			PlayerInfoHelper::getHelper()->setTeacher(1);

		GuideDialog::showDialog(FX::StringsMap::getValue("Guide_begin_game"), "", FX::StringsMap::getValue("startgame"),
			[](){GameDirector::getDirector()->loadingToUIScene();});
	}
	//其他关卡
	else {
		this->tollgateModeEnd(true);
	}
}

//只有非教学关卡会进入
void StateBar::tollgateModeEnd(bool arrivalEndPos)
{
	StopLayer* pEndLayer = StopLayer::create();
	pEndLayer->showTollgateOverLayer(arrivalEndPos);
	this->addChild(pEndLayer, 100, StateBar::Tag_DeadLayer);
}

TollgateInfor::TollgatePassLevel StateBar::calculateTollgatePassLevel(int gotJewel, int totalJewel)
{
	float val = gotJewel / (float)totalJewel;
	if (val >= 0.9) return TollgateInfor::ePerfect;
	else if (val >= 0.6) return TollgateInfor::eNice;
	else return TollgateInfor::eGood;
}

void StateBar::showScoreBar(bool isShow)
{
	if (m_controlSprite)
	{
		m_controlSprite->getParent()->setVisible(isShow);
// 		Label *label = (Label*)this->getChildByTag(Tag_ControlBgSpr)->getChildByTag(Tag_ScoreValue);
// 		if (label)
// 		{
// 			label->setVisible(isShow);
// 		}
// 		Sprite *spr = (Sprite*)this->getChildByTag(Tag_XingSpr);
// 		if (spr)
// 		{
// 			spr->setVisible(isShow);
// 		}
	}
}
void StateBar::showHadGoods()
{
	Point pos(m_winSize.width / 2,72);
	GoodsHelper *goodsHelper = GoodsHelper::getHelper();
	HeroSprite* hero = RunningHelper::getHelper()->getRunnerHero();
	int start_x = 100;

	//1.开场冲刺
	bool bHasRush = false;
	// --- 先使用随机道具、没有再使用背包、没有才使用人物自身属性
	{
		unsigned int randomItemId = goodsHelper->getRandomMarketItem();
		if (randomItemId != unitID_None)
		{
			const Unit* randomItem = goodsHelper->getUnitByID(randomItemId);
			if (randomItem)
			{
				if (randomItem->haveAttribute(Attr_Rush))
				{
					bHasRush = true;
					m_speedUnit = *randomItem;
				}
			}
		}
	}
	if (! bHasRush)
	{
		const BagItem *item = goodsHelper->getOneItem(unitID_KaiChangChongCi);
		if (item && item->value > 0)
		{
			const Unit* unit = goodsHelper->getUnitByID(item->id);
			if (unit && unit->haveAttribute(Attr_Rush))
			{
				bHasRush = true;
				m_speedUnit = *unit;
			}
		}
	}
	if(! bHasRush && hero)
	{
		const vector<Attribute>* attrVec = hero->getGlobleAddAttribute();
		for (int i=0,count=attrVec->size(); i<count; ++i)
		{
			if (attrVec->at(i).attr == Attr_Rush)
			{
				bHasRush = true;
				m_speedUnit = *hero->getSelfUnit();
				break;
			}
		}
	}
	if (bHasRush)
	{
		BaseSprite *spr = ImageHelper::getUnitSpriteUIScene(unitID_KaiChangChongCi);
		spr->setPosition(m_winSize.width / 2 + spr->getContentSize().width/2 + start_x, m_winSize.height - spr->getContentSize().height / 2 - 10);
		this->addChild(spr,2,Tag_StartSpeed);
	}

	//2. 死亡冲刺
	{
		const BagItem *item = goodsHelper->getOneItem(unitID_SiWangChongCi);
		if ((item && item->value > 0))
		{
			const Unit* unit = goodsHelper->getUnitByID(item->id);
			if (unit && unit->haveAttribute(Attr_Rush))
			{
				m_dieSpeedUnit = *unit;
			}
			BaseSprite *spr = ImageHelper::getUnitSpriteUIScene(unitID_SiWangChongCi);
			spr->setPosition(m_winSize.width / 2 + spr->getContentSize().width / 2 * 5 + start_x, m_winSize.height - spr->getContentSize().height / 2 - 10);
			this->addChild(spr,2,Tag_DieSpeed);
		}
	}

	//3. 死亡复活
	if (hero)
	{
		const vector<Attribute>* attrVec = hero->getGlobleAddAttribute();
		for (int i=0,count=attrVec->size(); i<count; ++i)
		{
			if (attrVec->at(i).attr == Attr_Reborn)
			{
				BaseSprite *spr = ImageHelper::getUnitSpriteUIScene(unitID_FuHuoDaoJu);
				spr->setPosition(m_winSize.width / 2 + spr->getContentSize().width / 2 * 7 + start_x, m_winSize.height - spr->getContentSize().height / 2 - 10);
				this->addChild(spr,2,Tag_Reborn);
				break;
			}
		}
	}

/*
	const vector<Attribute> *attrList = RunningHelper::getHelper()->getRunnerHero()->getGlobleAddAttribute();

	int start_x = 100;
	int speedValue = 0;
	int protectValue = 0;
	int rebornValue = 0;
	for (vector<Attribute>::const_iterator iter = attrList->begin() ; iter != attrList->end() ; iter++)
	{
		if (iter->attr == Attr_Rush)
		{
			speedValue = iter->value;
		}
		else if (iter->attr == Attr_Protect)
		{
			protectValue = iter->value;
		}
		else if (iter->attr == Attr_Reborn)
		{
			rebornValue = iter->value;
		}
	}
	{
		
		const BagItem *item = goodsHelper->getOneItem(unitID_KaiChangChongCi);
		if ((item && item->value > 0) || (item && speedValue >= 1))
		{
			const Unit* unit = goodsHelper->getUnitByID(item->id);
			float value = 0;
			if (unit)
			{
				for (vector<Attribute>::const_iterator iter = unit->attribute.begin(); iter != unit->attribute.end();iter ++)
				{
					if (iter->attr == Attr_Rush)
					{
						value = iter->value;
					}
				}
			}
			
			if (value > speedValue)
			{
				speedValue = value;
			}
			BaseSprite *spr = ImageHelper::getUnitSpriteUIScene(unitID_KaiChangChongCi);
			spr->setPosition(m_winSize.width / 2 + spr->getContentSize().width/2 + start_x, m_winSize.height - spr->getContentSize().height / 2 - 10);
			this->addChild(spr,2,Tag_StartSpeed);
		}
	}
// 	{
// 		
// 		const BagItem *item = goodsHelper->getOneItem(unitID_HuDun);
// 		if ((item && item->value > 0) || (item && protectValue >= 1))
// 		{
// 			const Unit* unit = goodsHelper->getUnitByID(item->id);
// 			float value = 0;
// 			if (unit)
// 			{
// 				for (vector<Attribute>::const_iterator iter = unit->attribute.begin(); iter != unit->attribute.end();iter ++)
// 				{
// 					if (iter->attr == Attr_Protect)
// 					{
// 						value = iter->value;
// 					}
// 				}
// 			}
// 
// 			if (value > protectValue)
// 			{
// 				protectValue = value;
// 			}
// 			BaseSprite *spr = ImageHelper::getUnitSpriteUIScene(unitID_HuDun);
// 			spr->setPosition(m_winSize.width / 2 + spr->getContentSize().width / 2 * 3 + start_x, m_winSize.height - spr->getContentSize().height / 2 - 10);
// 			this->addChild(spr,2,Tag_StartProtect);
// 		}
// 		
// 	}
	{
		const BagItem *item = goodsHelper->getOneItem(unitID_SiWangChongCi);
		if ((item && item->value > 0))
		{
			const Unit* unit = goodsHelper->getUnitByID(item->id);
			float value = 0;
			if (unit)
			{
				for (vector<Attribute>::const_iterator iter = unit->attribute.begin(); iter != unit->attribute.end();iter ++)
				{
					if (iter->attr == Attr_Rush)
					{
						value = iter->value;
					}
				}
			}
			m_nDieSpeedValue = value;
			BaseSprite *spr = ImageHelper::getUnitSpriteUIScene(unitID_SiWangChongCi);
			spr->setPosition(m_winSize.width / 2 + spr->getContentSize().width / 2 * 5 + start_x, m_winSize.height - spr->getContentSize().height / 2 - 10);
			this->addChild(spr,2,Tag_DieSpeed);
		}
	}
	{
		if (rebornValue >= 1)
		{
			BaseSprite *spr = ImageHelper::getUnitSpriteUIScene(unitID_FuHuoDaoJu);
			spr->setPosition(m_winSize.width / 2 + spr->getContentSize().width / 2 * 7 + start_x, m_winSize.height - spr->getContentSize().height / 2 - 10);
			this->addChild(spr,2,Tag_Reborn);
		}
	}
	m_nSpeedValue = speedValue;*/
	
}
void StateBar::startEcffect(Attr_Tag tag)
{
	string attr ;
	BaseSprite *imgSpr = nullptr;
	if (tag == Tag_StartSpeed)
	{
//		imgSpr = ImageHelper::getUnitSpriteUIScene(unitID_KaiChangChongCi);
		attr = StringUtils::format("%s", m_speedUnit.descrigbe.c_str());//"%s%d%s",StringsMap::getValue("StartSpeed").c_str(),m_nSpeedValue,StringsMap::getValue("meter").c_str());
	}
	else if (tag == Tag_DieSpeed)
	{
//		imgSpr = ImageHelper::getUnitSpriteUIScene(unitID_SiWangChongCi);
		attr = StringUtils::format("%s", m_dieSpeedUnit.descrigbe.c_str());//"%s%d%s",StringsMap::getValue("DieSpeed").c_str(),m_nDieSpeedValue,StringsMap::getValue("meter").c_str());
	}
	else if (tag == Tag_Reborn)
	{
//		imgSpr = ImageHelper::getUnitSpriteUIScene(unitID_FuHuoDaoJu);
		attr = StringsMap::getValue("Reborn");
	}
	else
	{
		this->removeChildByTag(tag);
		return;
	}

	Scale9Sprite *spr = Scale9Sprite::create("UI_jiaose_anniu_shengji_manji.png");
	
	this->addChild(spr);

	Label *label = Label::createWithSystemFont(attr,Font_TTF_KaTong, 30.0f, Size(130, 0), TextHAlignment::LEFT);
	label->setColor(Font_Color_BaseColor);
	spr->addChild(label,3);
//	spr->setContentSize(Size(label->getContentSize().width,spr->getContentSize().height));
	spr->setPosition(m_winSize.width + spr->getContentSize().width / 2,m_winSize.height / 2);
	label->setPosition((spr->getContentSize().width) / 2 + 15 ,spr->getContentSize().height / 2 - 8);
	if (!imgSpr)
	{
		imgSpr = BaseSprite::create();
	}
	imgSpr->setAnchorPoint(Vec2(0.0f,0.5f));
	imgSpr->setPosition(0,spr->getContentSize().height / 2);
	spr->addChild(imgSpr,2);
	
	spr->runAction(Sequence::create(MoveBy::create(1.0f,Vec2(- spr->getContentSize().width - imgSpr->getContentSize().width ,0)),DelayTime::create(8.0f),MoveBy::create(1.0f,Vec2(spr->getContentSize().width  + imgSpr->getContentSize().width,0)),RemoveSelf::create(),nullptr));

	this->removeChildByTag(tag);
	
}

void StateBar::setProtectMenuEnable(bool e)
{
	setDaoJuMenuEnable(Tag_DaoJu_Protect, e);
}

void StateBar::setProtectMenuVisable(bool isVisable)
{
	setDaoJuMenuVisable(Tag_DaoJu_Protect, isVisable);
}

void StateBar::setDaoJuMenuEnable(int iTag, bool e)
{
	if (Node* pNode = this->getChildByTag(iTag))
	{
		Menu* pMenu = (Menu*)pNode->getChildByTag(Tag_DaoJu_Menu);
		pMenu->setEnabled(e);
	}
}

void StateBar::setDaoJuMenuVisable(int iTag, bool isVisable)
{
	if (Node* pNode = this->getChildByTag(iTag))
	{
		pNode->setVisible(isVisable);
	}
}


void StateBar::updateHPIconCount(int iHP)
{
	if (iHP < 0)
	{
		iHP = 0;
	}

	Node* pHPNode = this->getChildByTag(Tag_HPNode);
	
	CCASSERT(pHPNode, "pHPNode is should not be null");

	string name = "UI_youxizhong_shuxing_shengming.png";
	int maxHp = RunningHelper::getHelper()->getRunnerHero()->getSelfUnit()->getAttributeValue(Attr_HP);

	if (pHPNode->getChildrenCount() < maxHp)
	{
		pHPNode->removeAllChildren();
		for (int i = 0; i < maxHp; ++i)
		{
			Sprite* pHpIcon = Sprite::create(name.c_str());
			pHPNode->addChild(pHpIcon, 1, i+1);
			pHpIcon->setPosition(Vec2(pHpIcon->getContentSize().width/2+pHpIcon->getContentSize().width*i, pHpIcon->getContentSize().height/2));
		}
			
		pHPNode->setPositionY(m_winSize.height - ((Node*)pHPNode->getChildren().at(0))->getContentSize().height/2);
	}
	else
	{
		Vector<Node*> chilren = pHPNode->getChildren();
		for (int i = 0; i < chilren.size(); ++i)
		{
			if (i < iHP)
			{
				name = "UI_youxizhong_shuxing_shengming.png";
			}
			else
			{
				name = "UI_youxizhong_shuxing_kuang.png";
			}
			((Sprite*)chilren.at(i))->setTexture(name.c_str());
		}
	}
	
}

void StateBar::initDebugMenuBar()
{
	auto menu = Menu::create();
	this->addChild(menu, 1025, "DEBUG_TEST_BAR");
	menu->setPosition(m_winSize.width-100, m_winSize.height/2);

	//1. attrubite
	vector<Attribute> vec;
	vec.push_back(Attribute(Attr_Rush,8));
	vec.push_back(Attribute(Attr_SpeedUp,10));
	vec.push_back(Attribute(Attr_Magnet,6));
//	vec.push_back(Attribute(Attr_ChangeToGold,15));
	vec.push_back(Attribute(Attr_Protect,99999999));
	for (auto &temp : vec)
	{
		auto item = MenuItemFont::create(StringUtils::format("%s", temp.attr.c_str()) ,[=](Ref* obj){
			RunningHelper::getHelper()->getRunnerHero()->startAttribute(temp);});
		menu->addChild(item);
		item->setColor(Color3B::RED);
	}

	// control menu
	MenuItemToggle *colliRect = MenuItemToggle::createWithCallback([](Ref* obj){
		if (1 == DEBUG_uGameInCollisionRect) DEBUG_uGameInCollisionRect = 0;
		else DEBUG_uGameInCollisionRect = 1;
	}, MenuItemFont::create("ColliRectOn"), MenuItemFont::create("ColliRectOff"),nullptr,nullptr);
	menu->addChild(colliRect);
	MenuItemToggle *musicSwitch = MenuItemToggle::createWithCallback([](Ref* obj){
		if (FX::Music::getInstance()->isMusicEnable()) FX::Music::getInstance()->setMusicEnable(false);
		else FX::Music::getInstance()->setMusicEnable(true);
	}, MenuItemFont::create("MusicOn"), MenuItemFont::create("MusicOff"),nullptr,nullptr);
	menu->addChild(musicSwitch);

	menu->alignItemsVerticallyWithPadding(20);
}

static float s_fDebugFrameTime;
static float s_fDebugRenderTime;
static float s_fDebugScheduleTime;
static float s_fDebugVisitTime;
static float s_fDebugAutopoolTime;//
extern void getDebugTime(float& frame, float& render, float& sche, float& visit, float& disp);
void StateBar::initDebugInfo()
{
	Layer *base = Layer::create();
	this->addChild(base, 1025, "DEBUG_INFO_CONTAINER");
	base->setPosition(m_winSize.width-220, 0);
	float posY = m_winSize.height - 30;
	float GAP = 30;

	//game unit layer
	Label *unitScollSpeed = Label::createWithSystemFont("scroll Speed: 1.0", "Arial", 24);
	base->addChild(unitScollSpeed);
	unitScollSpeed->setPosition(0, posY);
	posY -= GAP;
	Label *obstacleCount = Label::createWithSystemFont("obstacle Count: 0", "Arial", 24);
	base->addChild(obstacleCount);
	obstacleCount->setPosition(0, posY);
	posY -= GAP;
	Label *unitLayerChilds = Label::createWithSystemFont("unitLayer Childs: 0", "Arial", 24);
	base->addChild(unitLayerChilds);
	unitLayerChilds->setPosition(0, posY);
	posY -= GAP;
	Label *unitLayerPos = Label::createWithSystemFont("unitLayer Pos: 0", "Arial", 24);
	base->addChild(unitLayerPos);
	unitLayerPos->setPosition(0, posY);
	posY -= GAP;
	//time
// 	Label *frameTime = Label::createWithSystemFont("DrawTime: 0", "Arial", 24);
// 	base->addChild(frameTime);
// 	frameTime->setPosition(0, posY);
// 	posY -= GAP;
// 	Label *scheduleTime = Label::createWithSystemFont("ScheduleTime: 0", "Arial", 24);
// 	base->addChild(scheduleTime);
// 	scheduleTime->setPosition(0, posY);
// 	posY -= GAP;
// 	Label *renderTime = Label::createWithSystemFont("RenderTime: 0", "Arial", 24);
// 	base->addChild(renderTime);
// 	renderTime->setPosition(0, posY);
// 	posY -= GAP;
// 	Label *visitTime = Label::createWithSystemFont("VisitTime: 0", "Arial", 24);
// 	base->addChild(visitTime);
// 	visitTime->setPosition(0, posY);
// 	posY -= GAP;
// 	Label *autoPool = Label::createWithSystemFont("AutoPool: 0", "Arial", 24);
// 	base->addChild(autoPool);
// 	autoPool->setPosition(0, posY);
// 	posY -= GAP;

	Director::getInstance()->getScheduler()->schedule([=](float dt){
		unitScollSpeed->setString(StringUtils::format("Scroll width: %.1f", RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth()));
		obstacleCount->setString(StringUtils::format("obstacle Count: %ld", RunningHelper::getHelper()->getGameUnitLayer()->getObstacle()->size()));
		unitLayerChilds->setString(StringUtils::format("unitLayer Childs: %ld", RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->getChildren().size()));
		unitLayerPos->setString(StringUtils::format("ObsLayer LocalPos (%.1f, %.1f)", RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->getPositionX(), RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->getPositionY()));
//		cocos2d::getDebugTime(s_fDebugFrameTime,s_fDebugRenderTime,s_fDebugScheduleTime,s_fDebugVisitTime,s_fDebugAutopoolTime);
// 		frameTime->setString(StringUtils::format("DrawTime: %f", s_fDebugFrameTime));
// 		scheduleTime->setString(StringUtils::format("ScheduleTime: %f", s_fDebugScheduleTime));
// 		renderTime->setString(StringUtils::format("RenderTime: %f", s_fDebugRenderTime));
// 		visitTime->setString(StringUtils::format("VisitTime: %f", s_fDebugVisitTime));
// 		autoPool->setString(StringUtils::format("AutoPool: %f", s_fDebugAutopoolTime));
	},this, 0.1f, false, "DEBUG_INFO");
}

Sprite* StateBar::createDaoJu(int iTag)
{
	int unitID = -1;
	if (iTag == Tag_DaoJu_Protect)
	{
		unitID = unitID_HuDun;
	}
	if (unitID < 0)
	{
		return nullptr;
	}

	const BagItem *bagItem = GoodsHelper::getHelper()->getOneItem(unitID);
	int number = 0;
	if (bagItem)
	{
		number = bagItem->value;
		if (number < 1)
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
	Sprite* pDaojuBg = (Sprite*)this->getChildByTag(iTag);
	if (pDaojuBg)
	{
		pDaojuBg->removeFromParent();
	}

	pDaojuBg = Sprite::create("UI_youxizhong_daoju_bg.png");
	this->addChild(pDaojuBg, 0, iTag);
	pDaojuBg->setScale(0.9f);

	BaseSprite* pIcon = ImageHelper::getUnitSpriteUIScene(unitID);
	pDaojuBg->addChild(pIcon, 1);
	pIcon->setPosition(Vec2(pDaojuBg->getContentSize().width/2, pDaojuBg->getContentSize().height/2+5));

	Sprite* pCountBg = Sprite::create("UI_zhuye_tishi_shangpinshuliang.png");
	pDaojuBg->addChild(pCountBg, 1, Tag_DaoJu_Count_Bg);
	pCountBg->setPosition(Vec2(pDaojuBg->getContentSize().width - 10, pDaojuBg->getContentSize().height - 10));

	Menu* pMenu = Menu::create();
	pMenu->setPosition(Point::ZERO);
	pDaojuBg->addChild(pMenu, 2, Tag_DaoJu_Menu);

	ccMenuCallback callback[1] = {std::bind(&StateBar::onProtect, this, std::placeholders::_1)};
	string img[1][3] = {{"UI_youxizhong_anniu_shiyong.png","UI_youxizhong_anniu_shiyong.png","UI_youxizhong_anniu_shiyong.png"}};

	MenuItemImage *item1 = MenuItemImage::create(
		img[0][0],
		img[0][1],
		img[0][2],
		callback[0]);
	pMenu->addChild(item1);
	item1->setPosition(pDaojuBg->getContentSize().width/2, 0);

	LabelAtlas *label = LabelAtlas::create("0", "UI_shuzi_02.png", 15, 20, '0');
	label->setAnchorPoint(Vec2(0.5f, 0.5f));
//	Label::createWithSystemFont(String::createWithFormat("%d",number)->getCString(),"",24.0f);
	label->setPosition(Vec2(pCountBg->getContentSize().width/2, pCountBg->getContentSize().height/2));
	label->setTag(Tag_DaoJu_Label);
	label->setString(String::createWithFormat("%d",number)->getCString());
	pCountBg->addChild(label);

	return pDaojuBg;
}