
#include "GameUnitLayer.h"
#include "StateBar.h"
#include "HeroSprite.h"
#include "PetSprite.h"
#include "BossSprite.h"
#include "ObstacleSprite.h"
#include "PlayerInfoHelper.h"
#include "ActorHelper.h"
#include "GoodsHelper.h"
#include "StringsMap.h"
#include "GameInTouchLayer.h"
#include "HeroAISprite.h"
#include "NetmsgManger.h"
#include "VersionConfig.h"
#include "UnitCollisionDetection.h"
#include "StopLayer.h"
#include "SimpleBackgroundLayer.h"
#include "SimpleFrontgroundLayer.h"
#include "TalkingLayer.h"
#include "Font.h"
#include "ChatContentHelper.h"

#include "Dialog.h"

GameUnitLayer* GameUnitLayer::create(RunningMode mode, vector<int> sectionInfor)
{
	GameUnitLayer* pLayer = new GameUnitLayer();
	if (pLayer && pLayer->init(mode, sectionInfor)){
		pLayer->autorelease();
	}else {
		CC_SAFE_DELETE(pLayer);
		pLayer = nullptr;
	}
	return pLayer;
}
GameUnitLayer::GameUnitLayer()
	:m_uFightSceneCount(0)
	,m_uCommonSceneCount(0)
	,m_lvUpListener(nullptr)
	,m_bChangeToSuper(false)
{
	m_hero = nullptr;
	m_pet = nullptr;
	m_magic = nullptr;
	m_pBlackLayer = nullptr;
	m_pMask = nullptr;
	m_fCurScale = m_fWillScaleTo = 5.0f;
// 	m_curScrollWidth = g_fMapScrollDelta;
// 	m_curSpeedMulti = 1.0;
	m_alreadyLoadSceneCount = 0;
	m_fCurSceneConfigWidth = 0.f;
	m_fSceneConfigTotalWidth = 0.f;
	m_obstacleContainer = nullptr;
	m_tempObstacleContainer = nullptr;
	m_obstacles.clear();

	m_runningMode = RunningMode::endless;
	m_runningSecionInfor.clear();

	mWinSize = Director::getInstance()->getWinSize();


	//教学特有的一些动画，在其他模式就不用加载了，避免浪费内存
	if (RunningHelper::getHelper()->isTeachScene())
	{
		cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("long.ExportJson");
	}
}
GameUnitLayer::~GameUnitLayer()
{
	if (m_hero) m_hero->release();
	if (m_pet) m_pet->release();
	if (m_magic) m_magic->release();
	if (m_obstacleContainer) m_obstacleContainer->release();
	CC_SAFE_RELEASE(m_pBlackLayer);
	CC_SAFE_RELEASE(m_pMask);
	m_obstacles.clear();
	if (m_lvUpListener) 
		_eventDispatcher->removeEventListener(m_lvUpListener);
	RunningHelper::getHelper()->setGameIn(false);
}
bool GameUnitLayer::init(RunningMode mode, vector<int> sectionInfor)
{
	if (! Layer::init()) {
		return false;
	}

	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vec2(0.5f,0.5f));
	this->setPosition(mWinSize.width/2, mWinSize.height/2);
//	this->setPosition(0, -20*GridWidth);

	m_runningMode = mode;
	m_runningSecionInfor = sectionInfor;

	m_curSpeedMulti = 1.0;
	if (m_runningMode == RunningMode::endless){
		m_curScrollWidth = m_curScrollBaseWidth = g_fMapScrollDelta;
	}
	else if (m_runningMode == RunningMode::tollgate){ 
		m_curScrollWidth = m_curScrollBaseWidth = TollgateHelper::getHelper()->getOneTollgateSection(sectionInfor[0], sectionInfor[1])->levels[sectionInfor[2]].speed;
	}
	else if (m_runningMode == RunningMode::teach){
		m_curScrollWidth = m_curScrollBaseWidth = g_fMapScrollDelta;
	}
	else {
		CCASSERT(false, "");
	}


	unsigned int heroID = PlayerInfoHelper::getHelper()->getDeafaultHero();
	unsigned int petID = PlayerInfoHelper::getHelper()->getDeafaultPet();
	unsigned int magicID = PlayerInfoHelper::getHelper()->getDeafaultMagicItem();
	//教学处理
	if (RunningHelper::getHelper()->isTeachScene())
	{
		petID = 0;
		magicID = 0;
	}

	string analyticsLog;
	//hero
	{
		Actor actor = ActorHelper::getHelper()->getOneHeroWithLevelInfo(heroID);
		m_hero = HeroSprite::create(&actor.unit);
		m_hero->retain();
		this->addChild(m_hero, 10);
		m_hero->setPosition(getHeroDefaultPosX(), 237+m_hero->getSelfUnit()->rect[0].size.height/2);

		analyticsLog = String::createWithFormat("Hero[ID=%d,Lv=%d] ",actor.unit.ID,actor.level)->getCString();
	}
	//pet
	if (petID != 0){
		Actor actor = ActorHelper::getHelper()->getOnePetWithLevelInfo(petID);
		m_hero->setPet(&actor.unit);
		m_pet = PetSprite::create(&actor.unit);
		m_pet->retain();
		this->addChild(m_pet, 11);
// 		m_pet->setPosition(mWinSize.width - 400, 200);
// 		m_pet->runAction(MoveTo::create(0.1f, Vec2(mWinSize.width - 400, actor.unit.ID==unitID_ZhanHongLong?440:200)));
		m_pet->setPosition(Vec2(mWinSize.width - 400, actor.unit.ID==unitID_ZhanHongLong?440:200));

		analyticsLog += String::createWithFormat("Pet[ID=%d,Lv=%d] ",actor.unit.ID,actor.level)->getCString();
	}
	//magic
	if (magicID != 0)
	{
		Actor actor = ActorHelper::getHelper()->getOneMagicWithLevelInfo(magicID);
		m_hero->setMagicGoods(&actor.unit);

		Armature* pMagicArmature = Armature::create("effect");
		this->addChild(pMagicArmature, 11);
		pMagicArmature->setPosition(m_hero->getPositionX()-80, m_hero->getPositionY() + 80);
		pMagicArmature->getAnimation()->play("e_modian");
//		pMagicArmature->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5f, Vec2(0,20)), MoveBy::create(0.5f, Vec2(0, -20)), nullptr)));
		m_magic = pMagicArmature;
		m_magic->retain();

		analyticsLog += String::createWithFormat("Magic[ID=%d,Lv=%d]",actor.unit.ID,actor.level)->getCString();
	}
	RunningHelper::getHelper()->getGameLog()->insert(std::make_pair("Runner",analyticsLog.c_str()));


	//随机道具
	//开场道具会在herosprite::beginattribute里面使用
	//statebar显示  --->< 在start回调中
	

	//obstacle
	{
		m_obstacleContainer = Layer::create();
		m_obstacleContainer->retain();
		m_obstacleContainer->setPosition(0,-20*g_fGridWidth);
		this->addChild(m_obstacleContainer, 1);
		loadConfigSceneInfo();
		preloadSceneItems();
	}

	this->scheduleOnce(schedule_selector(GameUnitLayer::start), 0.1f);


	//死亡时可以升级，然后回来继续复活  --- 奇葩！
	m_lvUpListener = _eventDispatcher->addCustomEventListener(MSG_LevelUp_Rsp, [=](EventCustom* pEvent){
		if (((MsgData*)pEvent->getUserData())->errorCode != Rsp_Success)
		{
			return;
		}
		//
		Actor actor;
		actor = ActorHelper::getHelper()->getOneHeroWithLevelInfo(heroID);
		m_hero->resetUnit(&actor.unit);
		//
		if (petID != 0)
		{
			actor = ActorHelper::getHelper()->getOnePetWithLevelInfo(petID);
			m_hero->setPet(&actor.unit);
		}
		//
		if (magicID != 0)
		{		
			actor = ActorHelper::getHelper()->getOneMagicWithLevelInfo(magicID);
			m_hero->setMagicGoods(&actor.unit);
		}
	});

	return true;
}

void GameUnitLayer::addMaskLayer(const Vec2& pos)
{
	return;
	removeMaskLayer();

	m_pBlackLayer = RenderTexture::create(this->getContentSize().width, this->getContentSize().height);
	m_pBlackLayer->retain();
	m_pBlackLayer->setPosition(this->getContentSize()/2);
	this->addChild(m_pBlackLayer, 17);

	m_fWillScaleTo = m_fCurScale = 5.0f;
	m_pMask = Sprite::create("mask.png");
	m_pMask->setPosition(pos);
	m_pMask->setScale(m_fCurScale);
	m_pMask->retain();

	BlendFunc blend;
	blend.src = GL_ZERO;
	blend.dst = GL_ONE_MINUS_SRC_ALPHA;
	m_pMask->setBlendFunc(blend);

//	this->schedule(schedule_selector(GameUnitLayer::updateMaskPos), 0.01f);
	this->schedule(schedule_selector(GameUnitLayer::scaleSmallMask), 5.0f);
}

void GameUnitLayer::removeMaskLayer()
{
	if (m_pBlackLayer)
	{
		m_pBlackLayer->removeFromParent();
	}
	if (m_pMask)
	{
		m_pMask->removeFromParent();
	}
	CC_SAFE_RELEASE_NULL(m_pBlackLayer);
	CC_SAFE_RELEASE_NULL(m_pMask);
//	this->unschedule(schedule_selector(GameUnitLayer::updateMaskPos));
	this->unschedule(schedule_selector(GameUnitLayer::scaleSmallMask));
	this->unschedule(schedule_selector(GameUnitLayer::scaleBigMask));
}

void GameUnitLayer::start(float dt)
{
	RunningHelper::getHelper()->getBackgroundLayer()->setScrollEnable(false);
	RunningHelper::getHelper()->getFrontgroundLayer()->setScrollEnable(false);


	StopLayer* pStopLayer = StopLayer::create();
	bool bShowGoMenu = true;
	if (RunningHelper::getHelper()->isTeachScene()){
		bShowGoMenu = false;

		TalkingLayer *talkLayer = TalkingLayer::create();

		vector<string> vecChat = ChatContentHelper::getInstance()->getChatContent(ChatContentHelper::eChatGuide);
		talkLayer->setTalkingStrings(vecChat);

		talkLayer->setCallBack([=](int){ pStopLayer->showBlackLayer(StopLayer::Type_Start, std::bind(&StopLayer::rebegin, pStopLayer, std::placeholders::_1), true, true); });

		talkLayer->createTalkingMessageBox(0);
		RunningHelper::getHelper()->getCurrentStateBar()->addChild(talkLayer, 101);
	}
	pStopLayer->showStartBlackLayer(bShowGoMenu);
	pStopLayer->setLastScene(nullptr);
	RunningHelper::getHelper()->getCurrentStateBar()->addChild(pStopLayer, 100);


	StateBar* pStateBar = RunningHelper::getHelper()->getCurrentStateBar();
	if (pStateBar)
	{
		pStateBar->showHadGoods();
	}

}

void GameUnitLayer::start()
{
	RunningHelper::getHelper()->getBackgroundLayer()->setScrollEnable(true);
	RunningHelper::getHelper()->getFrontgroundLayer()->setScrollEnable(true);
	RunningHelper::getHelper()->setGameIn(true);

	startScroll();
	m_hero->begin();

	//教学
	if (RunningHelper::getHelper()->isTeachScene())
	{
		for (int i=0,count=m_obstacles.size(); i<count; ++i)
		{
			if (m_obstacles.at(i)->getSelfUnit()->ID == unitID_JiaoxueLong)
			{
				Rect rect = m_obstacles.at(i)->getCollisonRectInWorldSpace();
				this->addFeiLong(rect.origin+rect.size/2);
				m_obstacles.at(i)->removeFromParent();
				m_obstacles.erase(i);
				break;
			}
		}
	}
}

void GameUnitLayer::startScroll()
{
	this->scheduleUpdate();
//	m_obstacleContainer->runAction(RepeatForever::create(MoveBy::create(1, Vec2(16*g_fGridWidth,0))));
}
void GameUnitLayer::stopScroll()
{
	this->unscheduleUpdate();
}

float GameUnitLayer::getCurModeBaseSpeed()
{
	return m_curScrollBaseWidth;
}
float GameUnitLayer::getScrollWidth()
{
	return m_curScrollWidth;//ScrollDelta;
}
float GameUnitLayer::getScrollMulti()
{
	return m_curSpeedMulti;
}
void GameUnitLayer::setScrollMultitypeWidth(float based_multi)
{
	m_curSpeedMulti = based_multi;
	m_curScrollWidth = m_curScrollBaseWidth * m_curSpeedMulti;
}

void GameUnitLayer::update(float dt)
{
//	if (dt > 0.03f) CCLOG("%s ---> frame delta time %.3f s", __FUNCTION__, dt);

	tick(dt);
}
void GameUnitLayer::tick(float dt)
{
	scrollLayer();
	clearCollisionVector();  //obstacle的update里面也会检测 超出屏幕左边的后自己移除

	RunningHelper::getHelper()->getUnitCollsionDetection()->newUpdate(dt);

	preloadSceneItems();
	updateRunMeter();

	if (m_pMask)
	{
		this->updateMaskPos(dt);
	}
}

void GameUnitLayer::scaleSmallMask(float dt)
{
	if (!m_pMask || !m_pBlackLayer)
	{
		return;
	}
	m_fCurScale -= 1.5f;
	if (m_fCurScale < 1.0f)
	{
		m_fCurScale = 1.0f;
	}
	m_pMask->setScale(m_fCurScale);
}

void GameUnitLayer::scaleBigMask(float dt)
{
	if (!m_pMask || !m_pBlackLayer)
	{
		return;
	}
	if (m_fCurScale < m_fWillScaleTo)
	{
		m_fCurScale += 0.02f;
	}
	else
	{
		this->unschedule(schedule_selector(GameUnitLayer::scaleBigMask));
	}
	m_pMask->setScale(m_fCurScale);
}

void GameUnitLayer::addMaskScale(float fscale /*=0.5f*/)
{
	if (!m_pMask || !m_pBlackLayer)
	{
		return;
	}
	m_fWillScaleTo = m_fCurScale + fscale;

	this->schedule(schedule_selector(GameUnitLayer::scaleBigMask), 0.01f);
}

void GameUnitLayer::updateMaskPos(float dt)
{
	if (m_pMask && m_pBlackLayer)
	{
		m_pBlackLayer->setScale(1/RunningHelper::getHelper()->getCameraScale());
		this->setMaskPos(m_hero->getPosition());
		m_pBlackLayer->clear(0,0,0,1.0f);
		m_pBlackLayer->begin();
		m_pMask->visit();
		m_pBlackLayer->end();
	}
}

void GameUnitLayer::setMaskPos(const Vec2& pos)
{
	if (m_pMask)
	{
		m_pMask->setPosition(pos);
	}
}


void GameUnitLayer::scrollLayer()
{
	//1. layer 移动
	m_obstacleContainer->setPositionX(m_obstacleContainer->getPositionX() - m_curScrollWidth);
	if (m_tempObstacleContainer){
		m_tempObstacleContainer->setPositionX(m_tempObstacleContainer->getPositionX() - m_curScrollWidth);
		//是时候改清除temp container了
		if (m_obstacleContainer->getPositionX() < -static_cast<int>(g_uPreloadMapWidthInPixel)){
			cleanTempObstacleLayer();
		}
	}
}

void GameUnitLayer::clearCollisionVector()
{
	Vector<ObstacleSprite*> deleteTemp;
	for(int i=0,count=m_obstacles.size(); i<count; ++i)
	{
		if (! m_obstacles.at(i)->getParent() || m_obstacles.at(i)->getCollisonRectInWorldSpace().getMaxX() < -100)
		{
			deleteTemp.pushBack(m_obstacles.at(i));
			break;
		}
	}

	for (int i=0,count=deleteTemp.size(); i<count; ++i)
	{
		ObstacleSprite* pObSpr = deleteTemp.at(i);
		recoveryHeroPos(pObSpr);
		m_obstacles.eraseObject(pObSpr);
		deleteTemp.at(i)->removeFromParent();
	}
	deleteTemp.clear();
}

const vector<SceneItem>* GameUnitLayer::loadConfigSceneInfo()
{
	SceneInfo temp = RunningHelper::getHelper()->getOneSceneInfo(m_runningMode, m_runningSecionInfor);
	++m_alreadyLoadSceneCount;
	m_fCurSceneConfigWidth = temp.screenCount * g_fConfigOneScreenPixelWidth;
	m_fSceneConfigTotalWidth += m_fCurSceneConfigWidth;

	m_sceneItemsVec = *temp.sceneItems;
	m_sceneItemsVecIdx = 0;

//	CCLOG("%s --->mode[%d] m_fSceneConfigTotalWidth = %.1f, m_fCurSceneConfigWidth = %.1f, screenCount = %.1f", 
//		__FUNCTION__, RunningHelper::getHelper()->getCurrentScenePlayMode(), m_fSceneConfigTotalWidth, m_fCurSceneConfigWidth, temp.screenCount);
	return temp.sceneItems;
}
void GameUnitLayer::preloadSceneItems(bool loadAtOnce/* = false*/)
{ 
	//1. init data 
	//***跑完一个配置文件 （1.是否会切换超跑、战斗等模式、  2.继续加载下一个）
	//***这里只做场景的预加载  具体的切换时间点是在running helper addmeter函数中
	if ((m_sceneItemsVec.empty() || m_sceneItemsVec.size() == m_sceneItemsVecIdx) &&
		m_obstacleContainer->getPositionX() < -m_fSceneConfigTotalWidth+mWinSize.width+g_uPreloadMapWidthInPixel)
	{
		RunningMode runningMode = RunningHelper::getHelper()->getCurRunningMode();
// 		if (runningMode == RunningMode::tollgate)
// 		{
// 			if (m_obstacleContainer->getPositionX() >= 0) //只加载一次
// 				loadConfigSceneInfo();
// 		}
		if (runningMode == RunningMode::endless)
		{
			Scene_PlayMode mode = RunningHelper::getHelper()->getCurrentScenePlayMode();
			if (mode == mode_Super)
			{
				RunningHelper::getHelper()->startCommonPlay();
			}
			else if (mode == mode_Fight)
			{
				bossLeave();
			}
			else if (mode == mode_Common) // to fight
			{
				if (!m_hero->getWillSuperRun() && RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless)
				{
					RunningHelper::getHelper()->startFightPlay();
				}
			}
			else
			{
				CCASSERT(false, "error branch");
			}
			return;
		}
	}

//	int count = 0;
// 	struct timeval tBegin,tEnd;
// 	gettimeofday(&tBegin, nullptr);
	//2. preload

	BossSprite *pBoss = getBoss();
	BossSprite::Boss_State curBossState = BossSprite::BS_Anger; // 默认让他们都显示出来
	if (pBoss) curBossState = pBoss->getBossState();

	SceneItem temp;
	while (m_sceneItemsVec.size() > m_sceneItemsVecIdx)
	{
		temp = m_sceneItemsVec.at(m_sceneItemsVecIdx);
		temp.posX *= g_fGridWidth;
		temp.posY *= g_fGridWidth;

		if (! loadAtOnce &&    //if or not  preload all 
			(m_obstacleContainer->convertToWorldSpace(Vec2(temp.posX+m_fSceneConfigTotalWidth-m_fCurSceneConfigWidth,temp.posY)).x >= mWinSize.width + g_uPreloadMapWidthInPixel) ) //太远的就不忙加载
			break;

		//出现概率
		int ratio;
		if (temp.chance < 100) 
			ratio = CCRANDOM_0_1() * 100;
		else 
			ratio = 100;

// 		//目前在这个道具状态，则不要出现这个道具了
// 		if (temp.unit->type == unittype_Item)
// 		{
// 			//vector<Attribute> vec = temp.unit->attribute;
// 			for (int attrI=0; attrI<temp.unit->attribute.size(); ++attrI)
// 			{
// 				//1. 火箭
// 				if ((temp.unit->attribute.at(attrI).attr == Attr_SpeedUp) && (m_hero->getIsAttrBuffer(Attr_SpeedUp))){
// 						temp.chance = 0;
// 				}
// 				//2. 无敌
// 				else if ((temp.unit->attribute.at(attrI).attr == Attr_Invincible) && m_hero->getIsAttrBuffer(Attr_Invincible)){
// 						temp.chance = 0;
// 				}
// 				//3. 磁铁
// 				else if ((temp.unit->attribute.at(attrI).attr == Attr_Magnet) && m_hero->getIsAttrBuffer(Attr_Magnet)){
// 						temp.chance = 0;
// 				}
// 				//4. 护盾
// 				else if (temp.unit->attribute.at(attrI).attr == Attr_Protect && m_hero->getIsAttrBuffer(Attr_Protect)){
// 					temp.chance = 0;
// 				}
// 				//5. 点金
// 				else if (temp.unit->attribute.at(attrI).attr == Attr_ChangeToGold && m_hero->getIsAttrBuffer(Attr_ChangeToGold)){
// 					temp.chance = 0;
// 				}
// 			}
// 		}
		
//		float bShowSkin = !(temp.unit->type == unittype_Skin && g_uUseSkinSwitch == CONFIG_OFF);
		if (temp.chance > 0 && ratio <= temp.chance/* && bShowSkin*/)
		{
			ObstacleSprite *s = ObstacleSprite::create(temp.unit, temp.actionID, temp.triggerID);

			//战斗模式  和  关卡模式boss关
			if (pBoss && (curBossState == BossSprite::BS_Normal || curBossState == BossSprite::BS_Rest))
			{
				//有bossmake触发器，且boss为normal状态
				for (int triIdx=0,triCount=temp.triggerID.size(); triIdx<triCount; ++triIdx)
				{
					if (temp.triggerID.at(triIdx) == 7)
					{
						s->setScale(0);
						s->setVisible(false);
						break;
					}
				}
			}

			s->setPosition(temp.posX+m_fSceneConfigTotalWidth-m_fCurSceneConfigWidth, temp.posY);
			s->setRotation(temp.rotation);
			s->setScale(temp.scale);
			s->setOpacity(temp.alpha);

			m_obstacleContainer->addChild(s, temp.zOrder);

			m_obstacles.pushBack(s);
			if (unitID_JianXi == temp.unit->ID)
				s->setVisible(false);

			//记录tollgate模式下宝石的总量
			if (temp.unit->ID == unitID_HongBaoShi)
			{
				RunningHelper::getHelper()->addJewel(1);
			}
		}
		++m_sceneItemsVecIdx;
//		++count;
	}
//	CCLOG("%s ---> count = %d", __FUNCTION__, count);
// 	gettimeofday(&tEnd, nullptr);
// 	float t = (tEnd.tv_sec-tBegin.tv_sec)*1000 + (tEnd.tv_usec-tBegin.tv_usec)/1000.f;
// 	log("%s ---> %d : %fms", __FUNCTION__, count, t);
}

void GameUnitLayer::recoveryHeroPos(ObstacleSprite* pObs)
{
	ObstacleSprite* pObstructSpr = (ObstacleSprite*)m_hero->getCurObstuctSprite();
	if (pObstructSpr && pObstructSpr == pObs)
	{
		//如果当前的阻挡物和将要删掉的一致，则调用准备恢复X坐标的方法
		m_hero->willResetX();
		m_hero->setCurObstuctSprite(nullptr);
	}
}

void GameUnitLayer::cleanTempObstacleLayer()
{
	if (m_tempObstacleContainer)
	{
		Vector<Node*> arr = m_tempObstacleContainer->getChildren();
		for (int i=0,count=arr.size(); i<count; ++i)
		{
			ObstacleSprite *p = dynamic_cast<ObstacleSprite*>(arr.at(i));
			if (p)  m_obstacles.eraseObject(p);
		}
		m_tempObstacleContainer->release();
		m_tempObstacleContainer->removeFromParent();
		m_tempObstacleContainer = nullptr;
	}
}
void GameUnitLayer::resetScrollLayer(bool loadScenInfo, Scene_PlayMode preMode)
{
	const vector<SceneItem> *tempVec = nullptr;
	//1. scene info
	cleanSceneItemQueue();
	// reset
	m_alreadyLoadSceneCount = 0;
	m_fCurSceneConfigWidth = 0.f;
	m_fSceneConfigTotalWidth = 0.f;
	if (loadScenInfo){
		tempVec = loadConfigSceneInfo();
	}
	else {
		CCASSERT(false, "");
	}

	//2. container layer
	cleanTempObstacleLayer();
	m_tempObstacleContainer = m_obstacleContainer;
	// 
	m_obstacleContainer = Layer::create();
	m_obstacleContainer->retain();
	this->addChild(m_obstacleContainer, 1);

	//
	//1. 根据第一个陆地的位置来放置它的初始位置。
	float y = 28;
	if (tempVec)
	{
		for (int i=0,count=tempVec->size(); i<count && i<20; ++i)
		{
			if (tempVec->at(i).unit->type == unittype_Land)
			{
				y = tempVec->at(i).posY;
				break;
			}
		}
	}
	m_obstacleContainer->setPosition(mWinSize.width+g_uPreloadMapWidthInPixel, (8-y)*g_fGridWidth);
	//2. 获取已加载的最后一个陆地 与 预加载的对齐
	if (RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Super)
	{
		m_bChangeToSuper = true;
	}
	else
	{
		Vec2 endLandWorldPos(0, 8*g_fGridWidth);
		Vector<Node*> &child =  m_tempObstacleContainer->getChildren();
		ObstacleSprite *p = nullptr;
		for (int i=child.size()-1; i>=0; --i)
		{
			p = dynamic_cast<ObstacleSprite*>(child.at(i));
			if (p && (p->getSelfUnit()->type == unittype_Land || p->getSelfUnit()->haveAttribute(Attr_TreadNoneffect)))
			{
				Rect rect = p->getCollisonRectInWorldSpace();
				Vec2 posP(rect.getMaxX(), rect.getMidY());
				if (posP.x > endLandWorldPos.x) endLandWorldPos = posP;
			}
		}
		if (preMode == mode_Super && RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Common){
			m_obstacleContainer->setPositionX(mWinSize.width*2);
		}
		else if (preMode == mode_Fight && RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Common){
			m_obstacleContainer->setPositionX(0);
		}
		else if (endLandWorldPos.x > mWinSize.width) {
			m_obstacleContainer->setPositionX(m_obstacleContainer->getParent()->convertToNodeSpace(endLandWorldPos).x);
		}
		float deltaY = m_obstacleContainer->convertToNodeSpace(endLandWorldPos).y - y*g_fGridWidth;
		m_obstacleContainer->setPositionY(m_obstacleContainer->getPositionY() + deltaY);
//		CCLOG("%s ---> pos (%.1f, %.1f)", __FUNCTION__, m_obstacleContainer->getPositionY(), m_obstacleContainer->getPositionY());
	}
}
void GameUnitLayer::startSuperMode()
{ 
	getHeroSprite()->startSuperPlay();

	//清除黑夜
	removeMaskLayer();
	removeRanklistHero();

	//进入超跑前，把剩下的场景信息保存，超跑结束后接着用
// 	vector<SceneItem> vec;
// 	while (! m_sceneItemsQueue.empty())
// 	{
// 		vec.push_back(m_sceneItemsQueue.front());
// 		m_sceneItemsQueue.pop();
// 	}
	vector<SceneItem> vec(m_sceneItemsVec.begin()+(m_sceneItemsVecIdx>100?m_sceneItemsVecIdx-100:0), m_sceneItemsVec.end());
	RunningHelper::getHelper()->superPlayStorageOldScene(vec, m_fCurSceneConfigWidth);
	resetScrollLayer(true);

	if (m_pet) m_pet->setVisible(false);
	if (m_magic) m_magic->setVisible(false);
}
void GameUnitLayer::startCommonMode(Scene_PlayMode preMode)
{
	if (preMode == mode_Super)
	{
		m_bChangeToSuper = false;
		resetScrollLayer(true, mode_Super);

		//从超跑最后一屏 到 预加载的陆地到达 人物脚下 需要一段时间
		m_hero->runAction(Sequence::create(
			CallFunc::create([=](){
				Armature *yunAnim = Armature::create("chaopao_yun");
				RunningHelper::getHelper()->getGameUnitLayer()->addChild(yunAnim, m_hero->getLocalZOrder()-1);
				yunAnim->setPosition(Director::getInstance()->getWinSize()/2);
				yunAnim->getAnimation()->play("super_cloud_down");
				yunAnim->getAnimation()->setMovementEventCallFunc([=](Armature *armature, MovementEventType type, const std::string& movementID){
					if (type == COMPLETE) yunAnim->removeFromParent();
				});
				yunAnim->getAnimation()->setFrameEventCallFunc([=](Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex){
					if (frameEventName == "RefreshBackground") {
						RunningHelper::getHelper()->postRefreshBgMsg(100);
						m_obstacleContainer->setPositionX(0);
					}
				});
		}),
			//MoveTo::create(2.0f, Vec2(getHeroDefaultPosX(), mWinSize.height/2)),
			DelayTime::create(2.0f),
			CallFunc::create([=](){ //当飞行人物 下面有路面时，才掉下来。
				this->schedule(SEL_SCHEDULE(&GameUnitLayer::superModeEnd), 0.1f);
		}),
			nullptr));

		m_hero->playAnimation(Armature_FeiLeiLe);
		
	}
	else if (preMode == mode_Fight)
	{
		//一场战斗场景完成  ---》 可以加速了
		++m_uFightSceneCount;

		resetScrollLayer(true, mode_Fight);

		m_hero->startCommonPlay();

		//白天?黑夜模式
		if (RunningHelper::getHelper()->getAlreadyFightPlayCount() % 2 == 1)
		{
			addMaskLayer(m_hero->getPosition());
		}
		//移除所有子弹
		Vector<ObstacleSprite*> delVec;
		for (int i=0,count=m_obstacles.size(); i<count; ++i)
		{
			if (m_obstacles.at(i)->getSelfUnit()->haveAttribute(Attr_Beat))
			{
				delVec.pushBack(m_obstacles.at(i));
			}
		}
		for (int i=0,count=delVec.size(); i<count; ++i)
		{
			delVec.at(i)->removeFromParent();
			m_obstacles.eraseObject(delVec.at(i));
		}
	}
	else 
	{
		CCASSERT(false, "error branch !");
	}
}

void GameUnitLayer::superModeEnd(float dt)
{
	//获取人物下面的路面 有则可以下落了。
	Rect landTempRect;
	std::pair<bool,Sprite*> landInfo = this->getRecentLandRect(landTempRect, false);
	if (! landInfo.first) return;

	this->unschedule(schedule_selector(GameUnitLayer::superModeEnd));

	getHeroSprite()->startCommonPlay();
	//超跑后 进度条要清零
	RunningHelper::getHelper()->getCurrentStateBar()->startGoldValue();
	//白天?黑夜模式
	if (RunningHelper::getHelper()->getAlreadyFightPlayCount() % 2 == 1)
	{
		addMaskLayer(m_hero->getPosition());
	}

	if (m_pet){
		m_pet->setVisible(true);
		m_pet->resetOnLand();
	}
	if (m_magic) {
		m_magic->setVisible(true);
	}
}

void GameUnitLayer::startFightMode()
{
	//开始一场战斗  那么就代表完成了一场 1000米的场景
	++m_uCommonSceneCount;

	cleanSceneItemQueue();

	//清除黑夜
	removeMaskLayer();
	removeRanklistHero();

	m_hero->startFightPlay();

	resetScrollLayer(true);
}
void GameUnitLayer::cleanSceneItemQueue()
{
	m_sceneItemsVec.clear();
}

void GameUnitLayer::updateRunMeter()
{
	int oldMeter = RunningHelper::getHelper()->getMeter();
	RunningHelper::getHelper()->addMeter(m_curScrollWidth / g_uOneMeterWidthInPixel);

	if (RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Super 
		|| m_hero->getCurState() == HeroSprite::state_ChongCi
		|| m_hero->getCurState() == HeroSprite::state_Bounce || m_hero->getCurState() == HeroSprite::state_BounceBack
		|| m_hero->getIsSpringBounce()//普通弹簧
		|| m_hero->isHaveAttrOnGoing(Attr_SpeedUp)
		) 
		return;


	RunningHelper::getHelper()->setRunnerMultitypeSpeed(getCurMeterShouldSpeed());
	int newMeter = RunningHelper::getHelper()->getMeter();
// 	if (newMeter > 3000){
// 		RunningHelper::getHelper()->setRunnerMultitypeSpeed(getCurMeterShouldSpeed());
// 	}
// 	else if (newMeter > 2000){
// 		RunningHelper::getHelper()->setRunnerMultitypeSpeed(getCurMeterShouldSpeed()); 
// 	}
// 	else if (newMeter > 1000){
// 		RunningHelper::getHelper()->setRunnerMultitypeSpeed(getCurMeterShouldSpeed());
// 	}
// 	else {
// 		RunningHelper::getHelper()->setRunnerMultitypeSpeed(getCurMeterShouldSpeed());
// 	}

	
	//关卡模式速度恒定，不会随米数增加
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless && oldMeter/1000 < newMeter/1000) 
	{
		Sprite *board = Sprite::create("UI_tanchukuang_shuoming.png");
//		board->setScale(0);
		RunningHelper::getHelper()->getCurrentStateBar()->addChild(board);
		board->setPosition(mWinSize.width+board->getContentSize().width/2, mWinSize.height-board->getContentSize().height);

		Label *label = Label::createWithSystemFont(StringUtils::format("%d%s", newMeter-newMeter%500,FX::StringsMap::getValue("Mi").c_str()).c_str(), "Arial", 40);
		board->addChild(label);
		label->setColor(Font_Color_BaseColor);
		label->setPosition(board->getContentSize().width/2, 68);

		board->runAction(Sequence::create(
//			ScaleTo::create(0.25f, 1),
			MoveTo::create(0.25f, Vec2(mWinSize.width-board->getContentSize().width/2, mWinSize.height-board->getContentSize().height)),
			DelayTime::create(1.5f),
//			ScaleTo::create(.25f, 0),
			MoveBy::create(0.25f, Vec2(board->getContentSize().width,0)),
			RemoveSelf::create(),
			nullptr));
	}
}

float GameUnitLayer::getCurMeterShouldSpeed(/*long meter*/)
{
// 	if (meter > 3000) return 1.5f;
// 	else if (meter > 2000) return 1.3f;
// 	else if (meter > 1000) return 1.1f;
// 	else return 1.f;
	if (m_uFightSceneCount >= 3) return 1.5f;
	else if (m_uFightSceneCount == 2) return 1.3f;
	else if (m_uFightSceneCount == 1) return 1.1f;
	else if (m_uFightSceneCount == 0) return 1.f;
	else CCASSERT(false, "error branch");
	return 1.f;
}

std::pair<bool,Sprite*> GameUnitLayer::getRecentLandRect(Rect& rect, bool herotread)
{
	rect = Rect(getHeroDefaultPosX()-2*g_fGridWidth, 192-2*g_fGridWidth, 2*g_fGridWidth, 2*g_fGridWidth);
	bool found = false;
	Sprite *land = nullptr;

	Rect heroRect = m_hero->getCollisonRectInWorldSpace();
	Rect landRect;

	Vector<Node*> curScreenItems =  m_obstacleContainer->getChildren();
	if (m_tempObstacleContainer){
		const Vector<Node*>& temp = m_tempObstacleContainer->getChildren();
		for (int i=0,count=temp.size(); i<count; ++i)
		{
			curScreenItems.pushBack(temp.at(i));
		}
	}

	ObstacleSprite *p = nullptr;
	for (int i=0; i<curScreenItems.size(); ++i)
	{
		p = dynamic_cast<ObstacleSprite*>(curScreenItems.at(i));
		if (p && (p->getSelfUnit()->type == unittype_Land || p->getSelfUnit()->haveAttribute(Attr_TreadNoneffect)))
		{
			landRect = p->getCollisonRectInWorldSpace();
			//1. 找到与人物在同一垂直面上的路面
			if (herotread 
				&& heroRect.getMidX() < landRect.getMaxX()
				&& heroRect.getMidX() > landRect.getMinX())
			{
				rect = landRect;
				found = true;
				land = p;
				break;
			}
			else if (! herotread 
				&& getHeroDefaultPosX() < landRect.getMaxX()
				&& getHeroDefaultPosX() > landRect.getMinX())
			{
				rect = landRect;
				found = true;
				land = p;
				break;
			}
		}
	}
	//2. 该处没有路面，是坑，  那么就找蘑菇，没有蘑菇才用默认的
	if (! found)
	{
		for (int i=0; i<curScreenItems.size(); ++i)
		{
			p = dynamic_cast<ObstacleSprite*>(curScreenItems.at(i));
			if (p && (p->getSelfUnit()->haveAttribute(Attr_TreadAddScore) || p->getSelfUnit()->haveAttribute(Attr_TreadApearItem)
				|| p->getSelfUnit()->haveAttribute(Attr_TreadDisapear) || p->getSelfUnit()->haveAttribute(Attr_TreadBounce)
				|| p->getSelfUnit()->haveAttribute(Attr_BounceFly)))
			{
				landRect = p->getCollisonRectInWorldSpace();
				if (herotread 
					&& heroRect.getMidX()< landRect.getMaxX()
					&& heroRect.getMidX() > landRect.getMinX())
				{
					rect = landRect;
					land = p;
					break;
				}
				else if (! herotread 
					&& getHeroDefaultPosX() < landRect.getMaxX()
					&& getHeroDefaultPosX() > landRect.getMinX())
				{
					rect = landRect;
					land = p;
					break;
				}
			}
		}
	}

	return std::pair<bool, Sprite*>(found,land);
}

Vec2 GameUnitLayer::getLastLandWorldPos()
{
	Vec2 ret = Vec2(getHeroDefaultPosX(), 192);

	Vector<Node*> &child =  m_obstacleContainer->getChildren();
	ObstacleSprite *p = nullptr;
	Rect rect;
	for (int i=0; i<child.size(); ++i)
	{
		p = dynamic_cast<ObstacleSprite*>(child.at(i));
		if (p && p->getSelfUnit()->type == unittype_Land)
		{
			rect = p->getCollisonRectInWorldSpace();
			//1. 找到与人物在同一垂直面上的路面
			if (rect.getMinX() <= mWinSize.width
				&& rect.getMaxX() > mWinSize.width)
			{
				ret = p->getParent()->convertToWorldSpace(p->getPosition());
				ret.y += rect.size.height/2;
				break;
			}
		}
	}

	return ret;
}

void GameUnitLayer::bossComeIn()
{
	int bossNum = RunningHelper::getHelper()->getAlreadyFightPlayCount();
	const Unit *uni = nullptr;
	if (bossNum % 2 == 1)
		uni = GoodsHelper::getHelper()->getUnitByID(unitID_Boss2);
	else 
		uni = GoodsHelper::getHelper()->getUnitByID(unitID_Boss1);
	BossSprite *boss = BossSprite::create(const_cast<Unit*>(uni));
	this->addChild(boss, 100, "boss");
	boss->setPosition(/*mWinSize.width+100*/0, mWinSize.height-150);
	boss->runAction(Sequence::create(
		MoveTo::create(2.0f, Vec2(mWinSize.width-170, mWinSize.height-150)),
		CallFuncN::create([&](Ref* node)
		{
			if (PlayerInfoHelper::getHelper()->getOneInfo(History_AlreadyBoss_Guide) == 0)
			{
				PlayerInfoHelper::getHelper()->setOneInfo(History_AlreadyBoss_Guide, 1);
				GuideDialog* pDialog = GuideDialog::showDialog(FX::StringsMap::getValue("Guide_touch_Boss"), FX::StringsMap::getValue("Guide_touch_continue"));
				pDialog->setBossGudie(true);
				RunningHelper::getHelper()->gamePause();
			}
		}),
		Repeat::create(Sequence::create( MoveBy::create(1.0f, Vec2(0, 20)), MoveBy::create(1.0f, Vec2(0, -20)), nullptr), -1),
		nullptr));
	boss->come();
}
void GameUnitLayer::bossComeIn(unsigned int bossUnitID)
{
	const Unit *uni = GoodsHelper::getHelper()->getUnitByID(bossUnitID);
	BossSprite *boss = BossSprite::create(const_cast<Unit*>(uni));
	this->addChild(boss, 100, "boss");
	boss->setPosition(/*mWinSize.width+100*/0, mWinSize.height-150);
	boss->runAction(Sequence::create(
		MoveTo::create(2.0f, Vec2(mWinSize.width-170, mWinSize.height-150)),
		Repeat::create(Sequence::create( MoveBy::create(1.0f, Vec2(0, 20)), MoveBy::create(1.0f, Vec2(0, -20)), nullptr), -1),
		nullptr));
	boss->come();
}

void GameUnitLayer::bossLeave()
{
	BossSprite *pBoss = getBoss();
	if (pBoss && !pBoss->isWillLeave())
	{
		pBoss->leave();
	}

	if (! RunningHelper::getHelper()->getFightEnd()) 
		RunningHelper::getHelper()->endFightPlay();
}

BossSprite *GameUnitLayer::getBoss()
{
	return dynamic_cast<BossSprite*>(this->getChildByName("boss"));
}

void GameUnitLayer::getPreloadSceneItemQueue(vector<SceneItem>& vec)
{
// 	queue<SceneItem> que = m_sceneItemsQueue;
// 	while (! que.empty())
// 	{
// 		vec.push_back(que.front());
// 		que.pop();
// 	}
	vec.insert(vec.begin(), m_sceneItemsVec.begin()+m_sceneItemsVecIdx, m_sceneItemsVec.end());
}
Rect GameUnitLayer::CalculateSceneItemRectInWorldSpace(const SceneItem& item)
{
	Rect ret;
	ObstacleSprite *s = ObstacleSprite::create(item.unit, item.actionID, item.triggerID);
	s->setPosition(item.posX*g_fGridWidth+m_fSceneConfigTotalWidth-m_fCurSceneConfigWidth, item.posY*g_fGridWidth);
	m_obstacleContainer->addChild(s);
	ret = s->getCollisonRectInWorldSpace();
	m_obstacleContainer->removeChild(s);
	return ret;
}

void GameUnitLayer::addRanklistHero()
{
	if (RunningHelper::getHelper()->getCurrentScenePlayMode() != mode_Common)
		return; 

	unsigned int unitID = 101;
	const vector<RankUser>& rankVec = NetmsgManger::getNetManager()->netSports;
	unsigned int meter = RunningHelper::getHelper()->getMeter();
	for (int i=0,count=rankVec.size(); i<count; ++i)
	{
		if (rankVec[i].distance >= meter && rankVec[i].distance - meter < 100)
		{
			unitID = rankVec[i].hero.type;
			break;
		}
	}

	if (unitID != 0)
	{
		const Unit *unit = GoodsHelper::getHelper()->getUnitByID(unitID);
		HeroAISprite * s = HeroAISprite::create(unit->ID, unit->rect.at(0));
		this->addChild(s, 1, "RanklistAiHero");
		s->setSpeed(-RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth()*20);//speed的速度单位是秒
		s->setPosition(s->getParent()->convertToNodeSpace(Vec2(mWinSize.width, mWinSize.height)));
		Vec2 pos = s->getParent()->convertToWorldSpace(s->getPosition());
//		CCLOG("add ranklist ai hero, world pos = (%.1f, %.1f)", pos.x, pos.y);
	}
}

void GameUnitLayer::removeRanklistHero()
{
	this->removeChildByName("RanklistAiHero");
}

void GameUnitLayer::moveLayer_byMoveCamera(float time, float delta)
{
	// 1) obstacle container
	if (! m_bChangeToSuper){
		m_obstacleContainer->setPositionY(m_obstacleContainer->getPositionY() + delta);
	}
	if (m_tempObstacleContainer){
		m_tempObstacleContainer->setPositionY(m_tempObstacleContainer->getPositionY() + delta);
	}

	// 2) hero sprite
	m_hero->setPositionY(getHeroSprite()->getPositionY() + delta);
	// 		// 3) rank list hero sprite
	// 		if (getGameUnitLayer()->getChildByName("RanklistAiHero"))
	// 		{
	// 			getGameUnitLayer()->getChildByName("RanklistAiHero")->setPositionY(getGameUnitLayer()->getChildByName("RanklistAiHero")->getPositionY() + deltaY);
	// 		}
	// 		// 4) pet sprite
	// 		if (getGameUnitLayer()->getPetSprite())
	// 		{
	// 			getGameUnitLayer()->getPetSprite()->setPositionY(getGameUnitLayer()->getPetSprite()->getPositionY() - deltaY);
	// 		}
	// 		// 5) magic sprite
	// 		if (getGameUnitLayer()->getMagicSprite())
	// 		{
	// 			getGameUnitLayer()->getMagicSprite()->setPositionY(getGameUnitLayer()->getMagicSprite()->getPositionY() - deltaY);
	// 			CCLOG("magic pos y = %.1f", getGameUnitLayer()->getMagicSprite()->getPositionY());
	// 		}
}

void GameUnitLayer::correctLandWillSuperRun()
{
	//特殊处理
	//1. 获取右边屏幕内最后一个陆地的顶边位置
	float posY = getLastLandWorldPos().y;
	//2. 铺平后面屏幕外的道路, 删除除道路外的所有物品
	Vector<ObstacleSprite*> delVec;
	Rect rect;
	for (int i=0, count=m_obstacles.size(); i<count; ++i)
	{
		rect = m_obstacles.at(i)->getCollisonRectInWorldSpace();
		if (rect.getMinX() > mWinSize.width)
		{
			if (m_obstacles.at(i)->getSelfUnit()->type == unittype_Land)
			{
				m_obstacles.at(i)->setPositionY(m_obstacles.at(i)->getParent()->convertToNodeSpace(Vec2(0,posY - rect.size.height/2)).y);
			}
			else
			{
				delVec.pushBack(m_obstacles.at(i));
			}
		}
	}
	for (int i=0; i<delVec.size(); ++i)
	{
		m_obstacles.eraseObject(delVec.at(i));
		delVec.at(i)->removeFromParent();
	}
	delVec.clear();
}

void GameUnitLayer::addFeiLong(Point pos)
{
	Armature* pArmature = Armature::create("long");
	this->addChild(pArmature);

	pArmature->setPosition(this->convertToNodeSpace(pos));
	pArmature->getAnimation()->play("m_feizou");

	MoveBy* pBy = MoveBy::create(1.2f, Vec2(0, 100));
	MoveBy* pBy1 = MoveBy::create(1.2f, Vec2(0, -150));
	MoveBy* pBy2 = MoveBy::create(1.2f, Vec2(0, -150));
	MoveBy* pBy3 = MoveBy::create(1.2f, Vec2(0, 150));
	MoveBy* pBy4 = MoveBy::create(1.2f, Vec2(0, 50));
	MoveBy* pBy5 = MoveBy::create(1.2f, Vec2(0, 100));
	MoveBy* pBy6 = MoveBy::create(1.2f, Vec2(0, -100));
	MoveBy* pBy7 = MoveBy::create(1.2f, Vec2(0, -200));
	MoveBy* pBy8 = MoveBy::create(1.2f, Vec2(0, 100));

	pArmature->runAction(RepeatForever::create(Sequence::create(pBy, pBy1, pBy2, pBy3, pBy4, pBy5, pBy6, pBy7, pBy8, nullptr)));
}