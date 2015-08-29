
#include "StopLayer.h"
#include "StringsMap.h"
#include "StateBar.h"
#include "GameDirector.h"
#include "AchievementHelper.h"
#include "RunningHelper.h"
#include "GoodsHelper.h"
#include "extensions/cocos-ext.h"
#include "RunningHelper.h"
#include "HeroSprite.h"
#include "PlayerInfoHelper.h"
#include "AccountsLayer.h"
#include "ActiveValueHelper.h"
#include "Dialog.h"
#include "VersionConfig.h"
#include "MainuiLayer.h"
#include "GameUnitLayer.h"
#include "ShopLayer.h"
#include "ImageHelper.h"
//#include "sdk/SdkManager.h"

using namespace cocos2d::extension;

#define TagName_DeadLayer				"DeadLayer"
#define TagName_PauseLayer				"PauseLayer"

void StopLayer::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	Layer::onExit();
}

bool StopLayer::init()
{
	if( !Layer::init()) return false;
	m_winSize = Director::getInstance()->getWinSize();
	m_pVisitScene = nullptr;

	m_pVisitScene = Director::getInstance()->getRunningScene();
	this->scheduleUpdate();


	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch* pTouch, Event* pEvent)
	{
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void StopLayer::update(float dt)
{
	if (m_pVisitScene)
	{
		m_pVisitScene->visit();
	}
}

void StopLayer::setLastScene(Scene* pScene)
{
	m_pVisitScene = pScene;
}

void StopLayer::onMainScene(Ref* pObj)
{
	this->unscheduleUpdate();
	this->removeChildByName(TagName_PauseLayer);
	checkAchievement();
	GameDirector::getDirector()->loadingToUIScene();
}
void StopLayer::onResume(Ref* pObj)
{
	this->removeChildByName(TagName_PauseLayer);
	FX::Music::getInstance()->resumeBackgroundMusic();
	this->showDaoJiShiAnimation();
}

void StopLayer::showDaoJiShiAnimation()
{
	float framePreTime = Director::getInstance()->getAnimationInterval();
	Node* pActionNode = Node::create();

	Sprite* pBg = Sprite::create("UI_zanting_bg.png");
	pBg->setPosition(Point::ZERO);
	pActionNode->addChild(pBg);
	pBg->runAction(Sequence::create(
		DelayTime::create(175*framePreTime),
		FadeTo::create(5*framePreTime, 0),
		Hide::create(),
		CallFuncN::create([&](Node* pNode){
			this->reallyResumeGame();
			pNode->getParent()->removeFromParent();}), 
				nullptr));

			Sprite* pSaoMiao = Sprite::create("ui_sence_zanting_dumiao_xiaoguo.png");
			pSaoMiao->setPosition(Point::ZERO);
			pActionNode->addChild(pSaoMiao);
			pSaoMiao->setOpacity(0);
			pSaoMiao->runAction(Sequence::create(
				RotateBy::create(60*framePreTime, 360),
				RotateBy::create(60*framePreTime, 360),
				RotateBy::create(55*framePreTime, 360),
				FadeTo::create(5*framePreTime, 0),
				nullptr));

			LabelAtlas* pThree = LabelAtlas::create("3", "UI_shuzi_04.png", 35, 55, '0');
			pThree->setAnchorPoint(Vec2(0.5f, 0.5f));
			pThree->setPosition(Point::ZERO);
			pThree->setScale(2.0f);
			pThree->setString("3");
			pActionNode->addChild(pThree);
			pThree->runAction(Sequence::create(
				ScaleTo::create(10*framePreTime, 2.4f),
				Spawn::create(ScaleTo::create(50*framePreTime, 2.0f), FadeTo::create(50*framePreTime, 0), nullptr),
				RemoveSelf::create(),
				nullptr));


			LabelAtlas* pTwo = LabelAtlas::create("2", "UI_shuzi_04.png", 35, 55, '0');
			pTwo->setAnchorPoint(Vec2(0.5f, 0.5f));
			pTwo->setPosition(Point::ZERO);
			pTwo->setScale(2.0f);
			pTwo->setVisible(false);
			pTwo->setString("2");
			pActionNode->addChild(pTwo);
			pTwo->runAction(Sequence::create(
				DelayTime::create(60*framePreTime),
				Show::create(),
				ScaleTo::create(10*framePreTime, 2.4f),
				Spawn::create(ScaleTo::create(50*framePreTime, 2.0f), FadeTo::create(50*framePreTime, 0), nullptr),
				RemoveSelf::create(),
				nullptr));

			LabelAtlas* pOne = LabelAtlas::create("1", "UI_shuzi_04.png", 35, 55, '0');
			pOne->setAnchorPoint(Vec2(0.5f, 0.5f));
			pOne->setPosition(Point::ZERO);
			pOne->setScale(2.0f);
			pOne->setVisible(false);
			pOne->setString("1");
			pActionNode->addChild(pOne);
			pOne->runAction(Sequence::create(
				DelayTime::create(120*framePreTime),
				Show::create(),
				ScaleTo::create(10*framePreTime, 2.4f),
				Spawn::create(ScaleTo::create(50*framePreTime, 2.0f), FadeTo::create(50*framePreTime, 0), nullptr),
				RemoveSelf::create(),
				nullptr));

			this->addChild(pActionNode);
			pActionNode->setPosition(m_winSize/2);
}

void StopLayer::reallyResumeGame()
{
	if (RunningHelper::getHelper()->getRunnerHero()->getCurState() != HeroSprite::state_Dead)
	{
		RunningHelper::getHelper()->setGameIn(true);
		Director::getInstance()->popScene();
	}
}

void StopLayer::checkFightPowerAchievement()
{
	Achieve* info = nullptr;
	float fFightPower = PlayerInfoDataManager::getManager()->getMaxFightPower();

	if (fFightPower >= 24)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ZhanDouIV);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_ZhanDouIV, 1);
		}
	}
	if (fFightPower >= 16)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ZhanDouIII);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_ZhanDouIII, 1);
		}
	}
	if (fFightPower >= 12)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ZhanDouII);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_ZhanDouII, 1);
		}
	}
	if (fFightPower >= 6)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ZhanDouI);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_ZhanDouI, 1);
		}
	}	
}

void StopLayer::checkAchievement()
{
	Achieve* info = nullptr;
	//检测快跑相关成就
	if (RunningHelper::getHelper()->getMeter() >= 1000)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_KuaiPaoI);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_KuaiPaoI, 1);
		}
	}
	if (RunningHelper::getHelper()->getMeter() >= 2000)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_KuaiPaoII);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_KuaiPaoII, 1);
		}
	}
	if (RunningHelper::getHelper()->getMeter() >= 5000)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_KuaiPaoIII);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_KuaiPaoIII, 1);
		}
	}
	if (RunningHelper::getHelper()->getMeter() >= 10000)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_KuaiPaoIV);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_KuaiPaoIV, 1);
		}
	}
	//检测吃货相关成就
	if (RunningHelper::getHelper()->getScore() >= 10000)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiHuoI);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiHuoI, 1);
		}
	}
	if (RunningHelper::getHelper()->getScore() >= 20000)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiHuoII);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiHuoII, 1);
		}
	}
	if (RunningHelper::getHelper()->getScore() >= 30000)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiHuoIII);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiHuoIII, 1);
		}
	}
	if (RunningHelper::getHelper()->getScore() >= 50000)
	{
		info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiHuoIV);
		if (info && !info->isOver)
		{
			AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiHuoIV, 1);
		}
	}
}

void StopLayer::updateDeadLayer()
{
	this->removeChildByName(TagName_DeadLayer);

	int meter = 0;
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless)
	{
		meter = RunningHelper::getHelper()->getMeter()/g_uFightModeSwitchMeter;
		meter = (meter+1)*g_uFightModeSwitchMeter - RunningHelper::getHelper()->getMeter();
	}

	SelectDialog* pDeadDialog = SelectDialog::create(FX::StringsMap::getValue("youdead"),
		__String::createWithFormat(FX::StringsMap::getValue("deadbgTip").c_str(), meter)->getCString(),
		"UI_chongwu_touxiang_pipizhu.png");
	pDeadDialog->setCloseCallBack(std::bind(&StopLayer::showEndBlackLayer, this));
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless)
	{
		if (RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Common)
		{	
			pDeadDialog->setBtnInfo(FX::StringsMap::getValue("jueseshengji"),
				"",
				([&](Ref *pSender){
					MainuiLayer* pMain = MainuiLayer::create();
					pMain->joinHero();
					this->addChild(pMain, 100);
			}),
				SelectDialog::Type_Left);

			pDeadDialog->setBtnInfo(__String::createWithFormat(FX::StringsMap::getValue("fuhuo_duoshaoci").c_str(), g_iGameFuHuoCostDiamond)->getCString(),
				"UI_zhuye_shangdian_zuanshi.png", 
				std::bind(&StopLayer::onRebornMakeSure, this, std::placeholders::_1),
				SelectDialog::Type_Right);
		}
		else if (RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Fight)
		{
			pDeadDialog->setBgInfo(FX::StringsMap::getValue("modianTip"), "");
			pDeadDialog->setBtnInfo(FX::StringsMap::getValue("modianshengji"),
				"",
				([&](Ref *pSender){
					MainuiLayer* pMain = MainuiLayer::create();
					pMain->joinMagic();
					this->addChild(pMain, 100);
			}),
				SelectDialog::Type_Left);

			pDeadDialog->setBtnInfo(__String::createWithFormat(FX::StringsMap::getValue("fuhuo_duoshaoci").c_str(), g_iGameFuHuoCostDiamond)->getCString(),
				"UI_zhuye_shangdian_zuanshi.png", 
				std::bind(&StopLayer::onRebornMakeSure, this, std::placeholders::_1),
				SelectDialog::Type_Right);
		}
	}
	else if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::tollgate)
	{
		pDeadDialog->setBgInfo(FX::StringsMap::getValue("tg_failed"),
			FX::StringsMap::getValue("die_dlg_tip").c_str(),
			"UI_chongwu_touxiang_pipizhu.png");
		pDeadDialog->setDialogType(SelectDialog::DiglogType_OneBtn);
		pDeadDialog->setCloseCallBack([](Ref* pSender){
			GameDirector::getDirector()->loadingToUIScene(RunningMode::tollgate);});
		pDeadDialog->setBtnInfo(
			__String::createWithFormat(FX::StringsMap::getValue("fuhuo_duoshaoci").c_str(), g_iGameFuHuoCostDiamond)->getCString(),
			"UI_zhuye_shangdian_zuanshi.png", 
			std::bind(&StopLayer::onRebornMakeSure, this, std::placeholders::_1),
			SelectDialog::Type_Middle);
	}
	else 
	{
		CCASSERT(false, "其他模式没有失败处理逻辑");
	}

	this->addChild(pDeadDialog, 100, TagName_DeadLayer);
}

void StopLayer::showEndlessAccountLayer()
{
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless)
	{
		//保存本周最高纪录
		unsigned int curMeter=RunningHelper::getHelper()->getMeter();
		unsigned int curScore=RunningHelper::getHelper()->getScore();
		unsigned int thisWeekScore=0, thisWeekMeter=0, historyScore=0, historyMeter=0;
		PlayerInfoHelper::getHelper()->getThisWeekMaxScore(thisWeekScore, thisWeekMeter);
		PlayerInfoHelper::getHelper()->getHistoryMaxScore(historyScore, historyMeter);
		if (thisWeekScore < curScore)
		{

			PlayerInfoHelper::getHelper()->setThisWeekMaxScore(curScore, curMeter);
			//历史最高纪录
			if (historyScore < curScore)
			{
				PlayerInfoHelper::getHelper()->setHistoryMaxScore(curScore, curMeter);
				ActiveValueHelper::getHelper()->addActiveByType(activeType_PoJiLu);
				NetmsgManger::getNetManager()->updateUserinfo();
			}
		}

		AccountsLayer* layer = AccountsLayer::create();
		layer->inintAccoutsLayer(
			RunningHelper::getHelper()->getRunnerHero()->getSelfUnit()->ID, 
			historyScore,
			curScore,
			curMeter, 
			curScore/10.f*(1+RunningHelper::getHelper()->getHeroAttributeAddMultNum(Attr_AddMultGold)));
		//	this->unscheduleUpdate();
		this->removeChildByName(TagName_DeadLayer);


		this->addChild(layer, 200);
		layer->getChildByTag(color_tag)->getChildByTag(backtag)->setScale(0);

		layer->getChildByTag(color_tag)->getChildByTag(backtag)->runAction(Sequence::create(
			ScaleTo::create(0.2f, 1.2f), 
			ScaleTo::create(0.2f, 0.8f), 
			ScaleTo::create(0.2f, 1.0f), 
			nullptr));

		layer->getChildByTag(color_tag)->getChildByTag(menutag)->getChildByTag(next_ganme_tag)->runAction(Sequence::create(
			ScaleTo::create(0.2f, 1.2f), 
			ScaleTo::create(0.2f, 0.8f), 
			ScaleTo::create(0.2f, 1.0f), 
			nullptr));

		checkAchievement();

		//analytics
		{
			map<string,string>* pLog = RunningHelper::getHelper()->getGameLog();
			int rebornTimes = RunningHelper::getHelper()->getRunnerHero()->getRebornTimes();
			int protectTimes = RunningHelper::getHelper()->getCurrentStateBar()->getProtectTimes();
			int pauseTimes = RunningHelper::getHelper()->getCurrentStateBar()->getPauseTimes();
			pLog->insert(std::make_pair("Record",String::createWithFormat("score=%d,meter=%d",curScore,curMeter)->getCString()));
			pLog->insert(std::make_pair("rebornTimes",String::createWithFormat("%d",rebornTimes)->getCString()));
			pLog->insert(std::make_pair("protectTimes",String::createWithFormat("%d",protectTimes)->getCString()));
			pLog->insert(std::make_pair("pauseTimes",String::createWithFormat("%d",pauseTimes)->getCString()));
			time_t curTime = time(0);
			struct tm *curTm = localtime(&curTime);//时区时间， 非时区时间使用struct tm *gmtime(const time_t *timep); 
			char * timeStr = asctime(curTm);
			(*pLog)["Time"] += String::createWithFormat("End[%s]",timeStr)->getCString();
			//SdkManager::getInstance()->logEvent(analyticsEVENT_Run, pLog);
		}
	}
	
}

void StopLayer::showTollgateAccountLayer()
{
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::tollgate)
	{
		int seasonId = RunningHelper::getHelper()->getCurSeasonId();
		int sectionId = RunningHelper::getHelper()->getCurSectionId();
		Section::Difficulty dif = RunningHelper::getHelper()->getCurDifficutLevel();

		int jewel = RunningHelper::getHelper()->getScore();
		TollgateInfor::TollgatePassLevel passLv = StateBar::calculateTollgatePassLevel(jewel, RunningHelper::getHelper()->getTotalJewel());
		TollgateInfor::TollgatePassLevel oldPassLv = TollgateHelper::getHelper()->getOneSectionPassLevel(seasonId, sectionId, dif);
		//保存进度 (在结算界面之前保存，结算界面会调用tollgatehelper获取信息)
		TollgateHelper::getHelper()->completeTollgateSection(seasonId, sectionId, dif, passLv);

		TollgateAccountLayer* pLayer = TollgateAccountLayer::create(jewel, passLv, oldPassLv);
		this->addChild(pLayer, 200);
		pLayer->showStarAnimation();
	}
}

void StopLayer::showCommonPlayDeadLayer()
{
	RunningHelper::getHelper()->getCurrentStateBar()->removeChildByTag(StateBar::Tag_DeadLayer);
	this->removeChildByName(TagName_DeadLayer);
	this->removeChildByName(TagName_PauseLayer);
	if (RunningHelper::getHelper()->getRunnerHero()->getCurState() == HeroSprite::state_Dead
		&& RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Common)
	{
		this->updateDeadLayer();
	}
}

void StopLayer::showFightPlayDeadLayer()
{
	RunningHelper::getHelper()->getCurrentStateBar()->removeChildByTag(StateBar::Tag_DeadLayer);
	this->removeChildByName(TagName_DeadLayer);
	this->removeChildByName(TagName_PauseLayer);
	if (RunningHelper::getHelper()->getRunnerHero()->getCurState() == HeroSprite::state_Dead
		&& RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Fight)
	{
		this->updateDeadLayer();
	}
}

void StopLayer::showTollgateOverLayer(bool bSucceed)
{
	RunningHelper::getHelper()->getCurrentStateBar()->removeChildByTag(StateBar::Tag_DeadLayer);
	this->removeChildByName(TagName_DeadLayer);
	this->removeChildByName(TagName_PauseLayer);
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::tollgate)
	{
		if (bSucceed)
		{
			showEndBlackLayer();
		}
		else
		{
			this->updateDeadLayer();
		}
	}
}

void StopLayer::showPauseLayer()
{
	this->removeChildByName(TagName_DeadLayer);
	this->removeChildByName(TagName_PauseLayer);

	SelectDialog* pDeadDialog = SelectDialog::create(" ", FX::StringsMap::getValue("zantingzhong"), "");

	Sprite* pZanting = Sprite::create("UI_youxizhong_zanting_biaoshi.png");
	pDeadDialog->getBg()->addChild(pZanting, 1);
	pZanting->setPosition(pDeadDialog->getTitle()->getPosition());

	pDeadDialog->getBgInfo()->setPositionX(pDeadDialog->getBgInfo()->getPositionX()+170);

	pDeadDialog->setBtnInfo("", "", nullptr, SelectDialog::Type_Right);
	pDeadDialog->setBtnInfo("", "", nullptr, SelectDialog::Type_Left);

	pDeadDialog->getBtn(SelectDialog::Type_Close)->setVisible(false);

	MenuItemImage* pLeftBtn = pDeadDialog->getBtn(SelectDialog::Type_Left);
	pLeftBtn->setNormalImage(Sprite::create("UI_youxizhong_zanting_zanniu_zhuye.png"));
	pLeftBtn->setSelectedImage(Sprite::create("UI_youxizhong_zanting_zanniu_zhuye.png"));
	pLeftBtn->setCallback(std::bind(&StopLayer::onMainScene, this, std::placeholders::_1));
	pLeftBtn->setPositionY(pLeftBtn->getPositionY()+50);

	MenuItemImage* pRightBtn = pDeadDialog->getBtn(SelectDialog::Type_Right);
	pRightBtn->setNormalImage(Sprite::create("UI_youxizhong_zanting_zanniu_jixu.png"));
	pRightBtn->setSelectedImage(Sprite::create("UI_youxizhong_zanting_zanniu_jixu.png"));
	pRightBtn->setCallback(std::bind(&StopLayer::onResume, this, std::placeholders::_1));
	pRightBtn->setPositionY(pRightBtn->getPositionY()+50);

	this->addChild(pDeadDialog, 1, TagName_PauseLayer);
}

void StopLayer::showShopLayer()
{
	auto shop = BaseLayer::create();
	shop->goldbox();
	((ShopLayer*)shop->getChildByTag(game_returntag))->setCloseCallback(std::bind(&StopLayer::updateDeadLayer, this));
	this->addChild(shop, 200);
}

void StopLayer::showEndBlackLayer()
{
	this->removeChildByName(TagName_DeadLayer);
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::tollgate)
	{
		showTollgateAccountLayer();
	}
	else if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless)
	{
		showEndlessAccountLayer();
	}
	else 
	{
		CCASSERT(false, "");
	}
	showBlackLayer(Type_End, nullptr, true, true);
}

void StopLayer::showStartBlackLayer(bool bShowGoBtn)
{
	showBlackLayer(Type_Start, nullptr, false, false);

	if (!bShowGoBtn)
	{
		return;
	}

	Menu* pMenu = Menu::create();
	pMenu->setPosition(Vec2(0, 0));
	this->addChild(pMenu, 101);

	ccMenuCallback callBack = [&](Ref* pObj){
		showBlackLayer(Type_Start, std::bind(&StopLayer::rebegin, this, std::placeholders::_1), true, true);
		((Node*)pObj)->setVisible(false);
	};

	MenuItemImage* pBtn = MenuItemImage::create("UI_youxizhong_go.png", "UI_youxizhong_go.png",
		callBack);
	pMenu->addChild(pBtn);
 	pBtn->setPosition(Vec2(this->getContentSize().width - pBtn->getContentSize().width/2 - 10,
 		pBtn->getContentSize().height/2 + 10));
}

void StopLayer::rebegin(Ref* pNode)
{
	//RunningHelper::getHelper()->getRunnerHero()->rebegin(pNode);
	RunningHelper::getHelper()->getGameUnitLayer()->start();
}

void StopLayer::showBlackLayerRemove(const std::function<void(Node*)> &func)
{
	LayerColor* pUpLayer = (LayerColor*)this->getChildByTag(Tag_BlackLayer_Up);
	LayerColor* pDownLayer = (LayerColor*)this->getChildByTag(Tag_BlackLayer_Down);
	if (pUpLayer && pDownLayer)
	{
		Size winSize = Director::getInstance()->getWinSize();
		CallFunc* pRemove = CallFunc::create(std::bind(&StopLayer::removeSelf, this));
		CallFuncN * callback = nullptr;
		if (func) callback = CallFuncN::create(func);

		pUpLayer->runAction(Sequence::create(
			MoveTo::create(1.2f, Vec2(pUpLayer->getPositionX(), winSize.height+pUpLayer->getContentSize().height/2)), 
			nullptr));

		if (callback)
			pDownLayer->runAction(Sequence::create(
				MoveTo::create(1.2f, Vec2(pDownLayer->getPositionX(), -pDownLayer->getContentSize().height/2)),
				callback, pRemove, nullptr));
		else 
			pDownLayer->runAction(Sequence::create(
				MoveTo::create(1.2f, Vec2(pDownLayer->getPositionX(), -pDownLayer->getContentSize().height/2)),
				pRemove, nullptr));
	}
}
void StopLayer::showBlackLayer(BlackLayerType type, const std::function<void(Node*)> &func, bool bShowAni, bool bRemove)
{
	int yOffset = 15;
	LayerColor* pUpLayer = (LayerColor*)this->getChildByTag(Tag_BlackLayer_Up);
	if (!pUpLayer)
	{
		pUpLayer = LayerColor::create(Color4B(0, 0, 0, 255));
		pUpLayer->ignoreAnchorPointForPosition(false);
		pUpLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
		pUpLayer->setPosition(Vec2(this->getContentSize().width/2, pUpLayer->getContentSize().height*9/8 + yOffset));
		this->addChild(pUpLayer, 100, Tag_BlackLayer_Up);

 		Sprite* pBlack = Sprite::create("UI_juqing_heimu.png");
 		pUpLayer->addChild(pBlack);
 		pBlack->setPosition(Vec2(pUpLayer->getContentSize().width/2, pBlack->getContentSize().height/2 - yOffset));
 		pBlack->setFlippedY(true);
	}
	LayerColor* pDownLayer = (LayerColor*)this->getChildByTag(Tag_BlackLayer_Down);
	if (!pDownLayer)
	{
		pDownLayer = LayerColor::create(Color4B(0, 0, 0, 255));
		pDownLayer->ignoreAnchorPointForPosition(false);
		pDownLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
		pDownLayer->setPosition(Vec2(this->getContentSize().width/2, -pUpLayer->getContentSize().height*1/8 - yOffset));
		this->addChild(pDownLayer, 100, Tag_BlackLayer_Down);

		Sprite* pBlack = Sprite::create("UI_juqing_heimu.png");
		pDownLayer->addChild(pBlack);
		pBlack->setPosition(Vec2(pDownLayer->getContentSize().width/2, pDownLayer->getContentSize().height - pBlack->getContentSize().height/2 + yOffset));
	}

	if (type == Type_End)
	{	
		pUpLayer->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height + pUpLayer->getContentSize().height*5/8+yOffset));
		pDownLayer->setPosition(Vec2(this->getContentSize().width/2, -pDownLayer->getContentSize().height*5/8 - yOffset));
	}

	if (bShowAni)
	{
		CallFunc* pRemove = nullptr;
		CallFuncN* callFun = nullptr;
		if (bRemove)
		{
			pRemove = CallFunc::create(std::bind(&StopLayer::removeSelf, this));
		}
		if (func)
		{
			callFun = CallFuncN::create(func);
		}

		if (type == Type_End)
		{	
			pUpLayer->runAction(Sequence::create(
				MoveBy::create(1.2f, Vec2(0, -pUpLayer->getContentSize().height*3/8-yOffset)), 
				/*callFun, pRemove,*/ nullptr));

			pDownLayer->runAction(Sequence::create(
				MoveBy::create(1.2f, Vec2(0, pDownLayer->getContentSize().height*3/8+yOffset)),
				callFun, pRemove, nullptr));
		}
		else
		{
			pUpLayer->runAction(Sequence::create(
				MoveBy::create(1.2f, Vec2(0, pUpLayer->getContentSize().height*5/8+yOffset)), 
				/*callFun, pRemove,*/ nullptr));

			pDownLayer->runAction(Sequence::create(
				MoveBy::create(1.2f, Vec2(0, -pDownLayer->getContentSize().height*5/8-yOffset)),
				callFun, pRemove, nullptr));
		}
	}
}

void StopLayer::removeSelf()
{
	this->removeFromParent();
}

void StopLayer::onRebornMakeSure(Ref* obj)
{
	this->removeChildByName(TagName_DeadLayer);
	int iCount = GoodsHelper::getHelper()->getDiamondNum();
	if (iCount < g_iGameFuHuoCostDiamond)
	{
		SelectDialog* pDeadDialog = SelectDialog::create("",
			FX::StringsMap::getValue("zuanshibuzu"),
			"UI_chongwu_touxiang_pipizhu.png");
		pDeadDialog->getBtn(SelectDialog::Type_Close)->setVisible(false);
		pDeadDialog->setBtnInfo(FX::StringsMap::getValue("cacle"),
			"",
			std::bind(&StopLayer::updateDeadLayer, this),
			SelectDialog::Type_Left);

		pDeadDialog->setBtnInfo(FX::StringsMap::getValue("qungding"),
			"", 
			std::bind(&StopLayer::showShopLayer, this),
			SelectDialog::Type_Right);

		this->addChild(pDeadDialog, 100, TagName_DeadLayer);
	}
	else
	{
		this->removeFromParent();
		BagDataManager::getManager()->changeOneUnitCount(GoodsHelper::getHelper()->getOneItem(unitID_Diamond)->id,-g_iGameFuHuoCostDiamond);
		RunningHelper::getHelper()->gameReborn();
	}
}




//===================================================================================
//==============================WaitingLayer=========================================
//===================================================================================
WaitingLayer::WaitingLayer()
{
}

WaitingLayer::~WaitingLayer()
{
}

WaitingLayer* WaitingLayer::createWithType(WaitingLayer_Type type)
{
	WaitingLayer* pLayer = new WaitingLayer();
	if (pLayer && pLayer->initWithType(type))
	{

		return pLayer;
	}
	CC_SAFE_DELETE(pLayer);
	return nullptr;
}

bool WaitingLayer::initWithType(WaitingLayer_Type type)
{
	if (!Layer::init())
	{
		return false;
	}
	m_CurWaitingLayer_Type = type;

	Sprite* pSpr = nullptr;
	if (m_CurWaitingLayer_Type == WaitingLayer_Type_SiWangChongCi)
	{
		pSpr = ImageHelper::getUnitSpriteUIScene(unitID_SiWangChongCi);
	}

	if (!pSpr)
	{
		return false;
	}

	LayerColor* pLayerColor = LayerColor::create(Color4B(0.0f, 0.0f, 0.0f, 125.0f));
	this->addChild(pLayerColor, -1);

	this->addChild(pSpr, 3, iTag_MainSpr);
	pSpr->setPosition(this->getContentSize()/2);

	Sprite* pDaojuBg = Sprite::create("UI_youxizhong_daoju_bg.png");
	pSpr->addChild(pDaojuBg, -1);
	pDaojuBg->setPosition(pSpr->getContentSize()/2);
	pSpr->setScale(0.8f);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [&](Touch*, Event*){ return true;};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	Sprite* pProgressTimerBg = Sprite::create("UI_chengjiu_jindutiao_bg.png");
//	pProgressTimerBg->setScale(0.5f);
	this->addChild(pProgressTimerBg, 1, iTag_ProgressTimerBg);
//	pProgressTimerBg->setPosition(Vec2(pSpr->getPositionX(), pSpr->getPositionY() - pSpr->getContentSize().height/2 - pProgressTimerBg->getContentSize().height));
	pProgressTimerBg->setPosition(this->getContentSize()/2);
	pSpr->setPosition(Vec2(pProgressTimerBg->getPositionX()-pProgressTimerBg->getContentSize().width/2, pProgressTimerBg->getPositionY()));


	Menu* pMenu = Menu::create();
	pMenu->setPosition(Point::ZERO);
	this->addChild(pMenu, 2);

	ccMenuCallback callback = [&](Ref* pObj){
	GoodsHelper *goodsHelper = GoodsHelper::getHelper();
	const BagItem *item = goodsHelper->getOneItem(unitID_SiWangChongCi);
	if ((item && item->value > 0))
		{
			const vector<Attribute>& vec = goodsHelper->getUnitByID(item->id)->attribute;
			for (int i=0,count=vec.size(); i<count; ++i)
			{
				if (vec[i].attr == Attr_Rush){
					RunningHelper::getHelper()->getCurrentStateBar()->startEcffect(StateBar::Tag_DieSpeed);
					this->removeFromParent();
					RunningHelper::getHelper()->gameReborn();
					RunningHelper::getHelper()->getRunnerHero()->setSiWangChongCiTimes(RunningHelper::getHelper()->getRunnerHero()->getSiWangChongCiTimes()+1);
					//修改死亡冲刺后没效果的问题
					RunningHelper::getHelper()->getRunnerHero()->deathRush(vec[i].attr, vec[i].value);
					goodsHelper->useOneGoods(item->id);

					RunningHelper::getHelper()->getGameLog()->insert(std::make_pair("ItemEnd",String::createWithFormat("ID=%d",item->id)->getCString()));
				}
			}
		}
	};
	string img[1][3] = {{"UI_youxizhong_anniu_shiyong.png","UI_youxizhong_anniu_shiyong.png","UI_youxizhong_anniu_shiyong.png"}};

	MenuItemImage *itemImage = MenuItemImage::create(
		img[0][0],
		img[0][1],
		img[0][2],
		callback);
	pMenu->addChild(itemImage);
	itemImage->setPosition(Vec2(pProgressTimerBg->getPositionX(),  
		pProgressTimerBg->getPositionY()- pProgressTimerBg->getContentSize().height/2 - itemImage->getContentSize().height/2));


	ProgressTimer * pTimer = ProgressTimer::create(Sprite::create("UI_chengjiu_jindutiao.png"));
	pTimer->setType(ProgressTimer::Type::BAR);
	pTimer->setMidpoint(Vec2(0, 0.5f));
	pTimer->setBarChangeRate(Vec2(1,0));
	pTimer->setPercentage(100.f);
	pTimer->setAnchorPoint(Vec2(0,0.5f));
	pTimer->setPosition(pProgressTimerBg->getContentSize().width/2 - pTimer->getContentSize().width/2, pProgressTimerBg->getContentSize().height/2);
	pProgressTimerBg->addChild(pTimer, 1, iTag_ProgressTimer);

	pTimer->runAction(Sequence::create(ProgressFromTo::create(3.0f, 100, 0), 
		CallFuncN::create([&](Node* pNode){
			this->removeFromParent();
			RunningHelper::getHelper()->getRunnerHero()->showDeadLayer();
	}),
		nullptr));

	return true;
}