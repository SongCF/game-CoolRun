#include "RunningHelper.h"
#include "GameUnitLayer.h"
#include "SimpleBackgroundLayer.h"
#include "HeroSprite.h"
#include "UnitCollisionDetection.h"
#include "GameInTouchLayer.h"
#include "StateBar.h"
#include "AchievementHelper.h"
#include "TollgateHelper.h"
#include "PetSprite.h"
#include "VersionConfig.h"
#include "PiaoFuWuNode.h"
#include "PlayerInfoDataManager.h"
#include "SimpleFrontgroundLayer.h"
#include "StopLayer.h"
#include "TalkingLayer.h"
#include "ChatContentHelper.h"

#define  Armature_BOSS_ComeIn			"wushilaixi"

using namespace cocostudio;

RunningHelper* RunningHelper::m_runningHelper = nullptr;


RunningHelper* RunningHelper::getHelper()
{
	if (! m_runningHelper) m_runningHelper = new RunningHelper;
	return m_runningHelper;
}
RunningHelper::RunningHelper()
{
}

void RunningHelper::initStartGame(Scene* pRunningScene, RunningMode mode, vector<int> sectionInfor)
{
	m_pRunningScene = pRunningScene;
	setCurRunningMode(mode);
	m_curSectionInfor = sectionInfor;

	m_bIsGameIn = false;
	setCurrentScenePlayMode(mode_Common);
	m_uCommonMapCount = 0;
	m_uOtherMapCount = 0;
	m_uSuperPlayCount = 0;
	m_uFightPlayCount = 0;

	m_displayScore = 0;
	m_fTotalJewel = 0;
	m_displayMeter = 0;

	m_iGoldEffectCount = 0;
	m_bUserOldCommonScene = false;
	m_bEnableCalculateMeter = true;

	m_oneGameLog.clear();
	time_t curTime = time(0);
	struct tm *curTm = localtime(&curTime);//时区时间， 非时区时间使用struct tm *gmtime(const time_t *timep); 
	char * timeStr = asctime(curTm);
	m_oneGameLog.insert(std::make_pair("Time",String::createWithFormat("Begin[%s] ",timeStr)->getCString()));
}

void RunningHelper::gameOver()
{
	getGameUnitLayer()->stopScroll();
	getGameUnitLayer()->getHeroSprite()->stopAllActions();
	SimpleBackgroundLayer* pBackLayer = (SimpleBackgroundLayer*)m_pRunningScene->getChildByName("SimpleBackgroundLayer");
	if (pBackLayer) pBackLayer->setScrollEnable(false);
	SimpleFrontgroundLayer* pFrontLayer = (SimpleFrontgroundLayer*)m_pRunningScene->getChildByName("SimpleFrontgroundLayer");
	if (pFrontLayer) pFrontLayer->setScrollEnable(false);
	UnitCollisionDetection* pColliLayer = (UnitCollisionDetection*)m_pRunningScene->getChildByName("UnitCollisionDetection");
	if (pColliLayer) pColliLayer->setCheckCollisionType(UnitCollisionDetection::eCheckType_None);

	this->setGameIn(false);
}
void RunningHelper::gameWin()
{
	Achieve* info = nullptr;
	//检测初跑者成就
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChuPaoZhe);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChuPaoZhe, 1);
	}

	SimpleBackgroundLayer* pBackLayer = (SimpleBackgroundLayer*)m_pRunningScene->getChildByName("SimpleBackgroundLayer");
	if (pBackLayer) pBackLayer->setScrollEnable(false);
	SimpleFrontgroundLayer* pFrontLayer = (SimpleFrontgroundLayer*)m_pRunningScene->getChildByName("SimpleFrontgroundLayer");
	if (pFrontLayer) pFrontLayer->setScrollEnable(false);
	UnitCollisionDetection* pColliLayer = (UnitCollisionDetection*)m_pRunningScene->getChildByName("UnitCollisionDetection");
	if (pColliLayer) pColliLayer->setCheckCollisionType(UnitCollisionDetection::eCheckType_None);
	GameInTouchLayer *pTouchLayer = this->getGameInTouchLayer();
	if (pTouchLayer) pTouchLayer->hideMenu();
	getGameUnitLayer()->stopScroll();
	this->setGameIn(false);
	float width = Director::getInstance()->getWinSize().width;
	float time = width / (RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth()/Director::getInstance()->getAnimationInterval());

// 	getGameUnitLayer()->getHeroSprite()->runAction(Sequence::create(
// 		CallFunc::create([](){RunningHelper::getHelper()->getRunnerHero()->unscheduleUpdate();}),
// 		MoveBy::create(time, Vec2(width, 0)),
// 		RemoveSelf::create(),
// 		nullptr));
	getGameUnitLayer()->getHeroSprite()->playCongratulationAnimate();
}

void RunningHelper::gamePause()
{
	//教学才使用
// 	getGameUnitLayer()->stopScroll();
//	getGameUnitLayer()->getHeroSprite()->unscheduleUpdate();
	getGameUnitLayer()->getHeroSprite()->pause();
	getGameUnitLayer()->pause();
	getGameUnitLayer()->getHeroSprite()->pauseArmature();
	SimpleBackgroundLayer* pBackLayer = (SimpleBackgroundLayer*)m_pRunningScene->getChildByName("SimpleBackgroundLayer");
	if (pBackLayer) pBackLayer->setScrollEnable(false);
	SimpleFrontgroundLayer* pFrontLayer = (SimpleFrontgroundLayer*)m_pRunningScene->getChildByName("SimpleFrontgroundLayer");
	if (pFrontLayer) pFrontLayer->setScrollEnable(false);
	UnitCollisionDetection* pColliLayer = (UnitCollisionDetection*)m_pRunningScene->getChildByName("UnitCollisionDetection");
	if (pColliLayer) pColliLayer->setCheckCollisionType(UnitCollisionDetection::eCheckType_None);
}
void RunningHelper::gameResume()
{
	//教学才使用
// 	getGameUnitLayer()->startScroll();
//	getGameUnitLayer()->getHeroSprite()->scheduleUpdate();
	getGameUnitLayer()->getHeroSprite()->resume();
	getGameUnitLayer()->resume();
	getGameUnitLayer()->getHeroSprite()->resumeArmature();
	SimpleBackgroundLayer* pBackLayer = (SimpleBackgroundLayer*)m_pRunningScene->getChildByName("SimpleBackgroundLayer");
	if (pBackLayer) pBackLayer->setScrollEnable(true);
	SimpleFrontgroundLayer* pFrontLayer = (SimpleFrontgroundLayer*)m_pRunningScene->getChildByName("SimpleFrontgroundLayer");
	if (pFrontLayer) pFrontLayer->setScrollEnable(true);
	UnitCollisionDetection* pColliLayer = (UnitCollisionDetection*)m_pRunningScene->getChildByName("UnitCollisionDetection");
	if (pColliLayer) pColliLayer->setCheckCollisionType(UnitCollisionDetection::eCheckType_All);
}
void RunningHelper::gameReborn(bool bRocket /*= false*/)
{
	int iCount = PlayerInfoDataManager::getManager()->getOnePlayerInfo(key_GameRebornCount);
	PlayerInfoDataManager::getManager()->setOnePlayerInfo(key_GameRebornCount, iCount+1);


	//检测坚持相关成就
	Achieve* info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_JianChiI);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_JianChiI, 1);
	}
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_JianChiII);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_JianChiII, 1);
	}
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_JianChiIII);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_JianChiIII, 1);
	}
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_JianChiIV);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_JianChiIV, 1);
	}

	getGameUnitLayer()->startScroll();

	SimpleBackgroundLayer* pBackLayer = (SimpleBackgroundLayer*)m_pRunningScene->getChildByName("SimpleBackgroundLayer");
	if (pBackLayer) pBackLayer->setScrollEnable(true);
	SimpleFrontgroundLayer* pFrontLayer = (SimpleFrontgroundLayer*)m_pRunningScene->getChildByName("SimpleFrontgroundLayer");
	if (pFrontLayer) pFrontLayer->setScrollEnable(true);
	UnitCollisionDetection* pColliLayer = (UnitCollisionDetection*)m_pRunningScene->getChildByName("UnitCollisionDetection");
	if (pColliLayer) pColliLayer->setCheckCollisionType(UnitCollisionDetection::eCheckType_All);
	getRunnerHero()->reborn(bRocket);

	this->setGameIn(true);

	//若果有缩放
	if (RunningHelper::getHelper()->getCameraScale() < 1)
	{
		RunningHelper::getHelper()->scaleCamera(0.5f, 1);
	}

	//
// 	if (RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Common
// 		&& RunningHelper::getHelper()->getRunnerHero()->getWillSuperRun()){
// 		getCurrentStateBar()->setStartScoreControl();
// 		RunningHelper::getHelper()->getRunnerHero()->setWillSuperRun(false);
// 	}
	
}
UnitCollisionDetection *RunningHelper::getUnitCollsionDetection()
{
	return ((UnitCollisionDetection*)m_pRunningScene->getChildByName("UnitCollisionDetection"));
}
GameInTouchLayer *RunningHelper::getGameInTouchLayer()
{
	return ((GameInTouchLayer*)m_pRunningScene->getChildByName("GameInTouchLayer"));
}

SceneInfo RunningHelper::getOneSceneInfo(RunningMode mode, vector<int> sectionInforVec)
{
	//去掉getFirstBackground 放进去的scene info
	if (m_uCommonMapCount == 0)
		while (! m_curScene.empty()) m_curScene.pop();

	SceneInfo temp;
	if (mode == RunningMode::tollgate)
	{
		if (sectionInforVec.size() != 3){
			CCASSERT(false, "error tollgate mode info");
			return temp;
		}

		temp = GameSceneFactory::getFactory()->getTollgateMap(sectionInforVec[0],sectionInforVec[1],sectionInforVec[2]);
		m_curScene.push(temp);
	}
	else if (mode == RunningMode::endless)
	{
		if (getCurrentScenePlayMode() == mode_Super)
		{
			++m_uOtherMapCount;
			temp = GameSceneFactory::getFactory()->getEndlessMap(getCurrentScenePlayMode(),m_uSuperPlayCount,m_uOtherMapCount);
			m_curScene.push(temp);	
		}
		else if (getCurrentScenePlayMode() == mode_Fight)
		{
			++m_uOtherMapCount;
			temp = GameSceneFactory::getFactory()->getEndlessMap(getCurrentScenePlayMode(),m_uFightPlayCount,m_uOtherMapCount);
			m_curScene.push(temp);
		}
		else
		{
			if (m_bUserOldCommonScene)
			{
				m_bUserOldCommonScene = false;
				temp = m_preSuperScene;
			}
			else 
			{
				++m_uCommonMapCount;
				temp = GameSceneFactory::getFactory()->getEndlessMap(getCurrentScenePlayMode(),m_uCommonMapCount,1);
			}
			m_curScene.push(temp);
		}
		
	}
	else if (mode == RunningMode::teach)
	{
		temp = GameSceneFactory::getFactory()->getTeachMap();
		m_curScene.push(temp);
	}
	else 
	{
		CCASSERT(false, "");
	}

	if (m_curScene.size() > 1) {
		//pref: 切换背景只有在超跑的时候发生，所以在超跑云的帧时间中发送消息
//		unsigned int oldBgId = m_curScene.front().bgID;
		m_curScene.pop();
//		unsigned int newBgId = m_curScene.front().bgID;
// 		if (oldBgId != newBgId)
// 		{
// 			postRefreshBgMsg(100);  //背景不一样才切换
// 		}
		CCASSERT(m_curScene.size() == 1, "");
	}
	return temp;
}
//获取当前背景信息
const BackgroundList* RunningHelper::getFirstBackground()
{
	while (! m_curScene.empty()) m_curScene.pop();

	if (m_curRunningMode == RunningMode::endless){
		m_curScene.push(GameSceneFactory::getFactory()->getEndlessMap(Scene_PlayMode::mode_Common,1,1));
	}
	else if (m_curRunningMode == RunningMode::tollgate){
		m_curScene.push(GameSceneFactory::getFactory()->getTollgateMap(m_curSectionInfor[0],m_curSectionInfor[1],m_curSectionInfor[2]));
	}
	else if (m_curRunningMode == RunningMode::teach){
		m_curScene.push(GameSceneFactory::getFactory()->getTeachMap());
	}
	else {
		CCLOG("%s ---> error running mode[%d]", __FUNCTION__, m_curRunningMode);
		CCASSERT(false, "");
	}
	return m_curScene.front().sceneBG;
}
const BackgroundList* RunningHelper::getCurrentBackground()
{
	return m_curScene.front().sceneBG;
}

float RunningHelper::getRunnerMultitypeSpeed()
{
	//return getGameUnitLayer()->getScrollWidth() / g_fMapScrollDelta;
	return getGameUnitLayer()->getScrollMulti();
}

void RunningHelper::setRunnerMultitypeSpeed(float based_multi)
{
	getGameUnitLayer()->setScrollMultitypeWidth(based_multi);

// 	ScrollForeverNode* pScrollNode = getBackgroundLayer()->getBGLayer_JinJing();
// 	if (pScrollNode)
// 	{
// 		pScrollNode->setMoveSpeed(pScrollNode->getDefaultMoveSpeed()*based_multi);
// 	}
// 
// 	pScrollNode = getBackgroundLayer()->getBGLayer_ZhongJing();
// 	if (pScrollNode)
// 	{	
// 		pScrollNode->setMoveSpeed(pScrollNode->getDefaultMoveSpeed()*based_multi);
// 	}
	if (this->getBackgroundLayer())
	{
		int i = 0;
		while (ScrollForeverNode* pNode = dynamic_cast<ScrollForeverNode*>(getBackgroundLayer()->getChildByTag(SimpleBackgroundLayer::iTag_ScrollNode1+i)))
		{
			pNode->setMoveSpeed(pNode->getDefaultMoveSpeed()*based_multi);
			++i;
		}

		i = 0;
		while (PiaoFuWuNode* pNode = dynamic_cast<PiaoFuWuNode*>(getBackgroundLayer()->getChildByTag(SimpleBackgroundLayer::iTag_PiaoFuWuNodeStart+i)))
		{
			pNode->setMoveSpeed(pNode->getDefaultMoveSpeed()*based_multi);
			++i;
		}
	}
}
GameUnitLayer* RunningHelper::getGameUnitLayer()
{
	return (GameUnitLayer*)m_pRunningScene->getChildByName("GameUnitLayer");
}
HeroSprite* RunningHelper::getRunnerHero()
{
	GameUnitLayer* layer = getGameUnitLayer();
	if (layer)
		return layer->getHeroSprite();
	else 
		return nullptr;
}
//获取当前跑酷场景的所有障碍物和道具
cocos2d::Vector<ObstacleSprite*>* RunningHelper::getObstacles()
{
	GameUnitLayer* layer = getGameUnitLayer();
	if (layer) return layer->getObstacle();
	return nullptr;
}

void RunningHelper::postRefreshBgMsg(int time_ms)
{
	EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
	EventCustom event(MSG_RefreshBackground);
	event.setUserData((void*)time_ms); //切换背景渐变的毫秒数  ms ,  必须是整数
	eventDspc->dispatchEvent(&event);
}

//endlessMODE---> score       tollgateMODE---> 红宝石
void RunningHelper::addScore(float num, bool bGold /*=false*/)
{
	m_displayScore += num /** (1+RunningHelper::getHelper()->getHeroAttributeAddMultNum(Attr_AddMultGold))*/; //结算才加成

	if (bGold){
		StateBar* bar = getCurrentStateBar();
		if (bar) bar->addGoldValue(num);
	}
}
unsigned int RunningHelper::getScore()
{
	return (unsigned int)(m_displayScore);
}
void RunningHelper::addMeter(float num)
{
	if (! this->isCalculateMeter()) {
		return;
	}

	m_displayMeter += num;


	//达到 切换战斗地图的米数 马上切换
	int willFgihtCount = (int) m_displayMeter / g_uFightModeSwitchMeter;
	int alreadyFightCount = this->getAlreadyFightPlayCount();
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless
		&& willFgihtCount > alreadyFightCount 
		&& this->getCurrentScenePlayMode() == mode_Common 
		&& !this->getRunnerHero()->getWillSuperRun()) 
	{
		this->startFightPlay();
	}
}
unsigned int RunningHelper::getMeter()
{
	return (unsigned int)(m_displayMeter);
}
float RunningHelper::getHeroAttributeAddMultNum(string attr)
{
	float ret = 0;
	const vector<Attribute> *vec = getRunnerHero()->getGlobleAddAttribute();
	for (int i=0,count=vec->size(); i<count; ++i)
	{
		//可能会有多个  所以不要break
		if (vec->at(i).attr == attr){
			ret += vec->at(i).value;
		}
	}
	return ret;
}

void RunningHelper::superPlayStorageOldScene(vector<SceneItem>& vec, float curWidthInPixel)
{

	//1. 清除上次的 
	if (m_preSuperScene.sceneItems) {
		delete m_preSuperScene.sceneItems;
	}

	//2. 获取完整的
	//m_preSuperScene = GameSceneFactory::getFactory()->getSceneInfo(RunningMode::endless, m_uCommonMapCount, mode_Common);
	m_preSuperScene = GameSceneFactory::getFactory()->getEndlessMap(mode_Common, m_uCommonMapCount, 1);
	//3. 去掉已跑的
	m_preSuperScene.sceneItems = new vector<SceneItem>(vec);
//	int num = vec.size();
//	m_preSuperScene.sceneItems = new vector<SceneItem>(
//		num>m_preSuperScene.sceneItems->size()?m_preSuperScene.sceneItems->begin():m_preSuperScene.sceneItems->end()-num, m_preSuperScene.sceneItems->end());
	// 1）通过剩余的长度来计算 有多少个scene count
	float oldSceneCount = curWidthInPixel / g_fConfigOneScreenPixelWidth;
	float subScreen = m_preSuperScene.sceneItems->begin()->posX * g_fGridWidth / g_fConfigOneScreenPixelWidth;
	m_preSuperScene.screenCount = oldSceneCount - subScreen;

//	CCLOG("%s ---> oldSceneCount = %.1f, newSceneCount = %.1f, itemCount = %d", __FUNCTION__, oldSceneCount, m_preSuperScene.screenCount, m_preSuperScene.sceneItems->size());
	// 2）x值变换
	for (int i=0,count=m_preSuperScene.sceneItems->size(); i<count; ++i)
	{
		m_preSuperScene.sceneItems->at(i).posX -= (oldSceneCount-m_preSuperScene.screenCount)*g_fConfigOneScreenPixelWidth/g_fGridWidth;
	}
}
void RunningHelper::startSuperPlay()
{
	++m_uSuperPlayCount;
	this->enableCalculateMeter(false);
	setCurrentScenePlayMode(mode_Super);
	getGameUnitLayer()->startSuperMode();
	getGameInTouchLayer()->StartSuper();
	getCurrentStateBar()->setProtectMenuVisable(false);
}
void RunningHelper::startFightPlay()
{
	this->setFightEnd(false);
	this->enableCalculateMeter(false);
	//不立刻增加 会导致gameunit处 认为这次fight模式没有开启，还会调用，那么会出现多个boss
	++m_uFightPlayCount;
	CCASSERT (abs(m_displayMeter-m_uFightPlayCount * g_uFightModeSwitchMeter) < 100 , "差距太大，是不是出bug了。");
	//把meter 纠正。
	m_displayMeter = m_uFightPlayCount * g_uFightModeSwitchMeter;

	setCurrentScenePlayMode(mode_Fight);
	getGameUnitLayer()->startFightMode();

	Armature* pComein = Armature::create("Run_UI_youxizhong_zhandou");
	pComein->getAnimation()->play(Armature_BOSS_ComeIn);
	pComein->setPosition(Vec2(getGameUnitLayer()->getContentSize().width/2, getGameUnitLayer()->getContentSize().height - pComein->getContentSize().height - 50));
	Director::getInstance()->getRunningScene()->addChild(pComein, 100);
	pComein->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(RunningHelper::playAnimationEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	float x = 5;
	float time = 0.015f;
	Director::getInstance()->getRunningScene()->runAction(
		Sequence::create(
		Spawn::create(
		Sequence::create(
		DelayTime::create(0.1f),
		MoveBy::create(time,Vec2(x,0)),
		MoveBy::create(time,Vec2(-x,0)),
		MoveBy::create(time,Vec2(x,0)),
		MoveBy::create(time,Vec2(x,0)),
		MoveBy::create(time,Vec2(-x/2,0)),
		MoveTo::create(0.01f,Vec2(0,0)),
		nullptr),
		nullptr),
		DelayTime::create(0.2f),
		CallFunc::create([=](){
			getGameUnitLayer()->bossComeIn();
			getGameInTouchLayer()->startCommon();
		}), 
		nullptr));

}

void RunningHelper::endFightPlay()
{
	this->setFightEnd(true);
	m_uOtherMapCount = 0;

	HeroSprite *pHero = getRunnerHero();
	if (pHero)
	{
		//不能用changestate，它会调用willsuperrun，要保持现在还是fightmode
		pHero->setCurState(HeroSprite::state_Super);

		//要使用超跑的背景
		m_curScene.push(GameSceneFactory::getFactory()->getEndlessMap(mode_Super,1,1));
		m_curScene.pop();
		pHero->willStartSuperPlay(1);

		Sprite *pPet = getGameUnitLayer()->getPetSprite();
		if (pPet) pPet->setVisible(false);
		Node *pMagic = getGameUnitLayer()->getMagicSprite();
		if (pMagic) pMagic->setVisible(false);
	}
}
void RunningHelper::fightEndToCommon()
{
	getGameUnitLayer()->unscheduleUpdate();
	getGameUnitLayer()->getObstacle()->clear();
	getGameUnitLayer()->getObstacleContainerLayer()->removeAllChildren();

	StopLayer* pStopLayer = StopLayer::create();
	pStopLayer->showBlackLayer(StopLayer::Type_End, [=](Node* node){

		TalkingLayer *talkLayer = TalkingLayer::create();
		vector<string> vecChat = ChatContentHelper::getInstance()->getChatContent((ChatContentHelper::ChatIdx)(ChatContentHelper::eChatFightEnd1 + m_uFightPlayCount-1));
		talkLayer->setTalkingStrings(vecChat);
		talkLayer->setCallBack([=](int)
		{
			pStopLayer->showBlackLayerRemove(nullptr);

			Armature *yunAnim = Armature::create("chaopao_yun");
			RunningHelper::getHelper()->getGameUnitLayer()->addChild(yunAnim, getRunnerHero()->getLocalZOrder()-1);
			yunAnim->setPosition(Director::getInstance()->getWinSize()/2);
			yunAnim->getAnimation()->play("super_cloud_down");
			yunAnim->getAnimation()->setMovementEventCallFunc([=](Armature *armature, MovementEventType type, const std::string& movementID){
				if (type == COMPLETE) yunAnim->removeFromParent();
			});
			yunAnim->getAnimation()->setFrameEventCallFunc([=](Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex){
				if (frameEventName == "RefreshBackground") {
					RunningHelper::getHelper()->getGameInTouchLayer()->setCanBeTouched(true);
					getGameUnitLayer()->scheduleUpdate();
					startCommonPlay();
					postRefreshBgMsg(100);
				}
			});
		});
		talkLayer->createTalkingMessageBox(0);
		RunningHelper::getHelper()->getCurrentStateBar()->addChild(talkLayer, 101);


	}, true, false);
	pStopLayer->setLastScene(nullptr);
	RunningHelper::getHelper()->getCurrentStateBar()->addChild(pStopLayer, 100);
}

void RunningHelper::startCommonPlay()
{	
	//RunningHelper::getHelper()->enableCalculateMeter(true);//放在hero中更准确

	if (getCurrentScenePlayMode() == mode_Super){
		m_bUserOldCommonScene = true;
	}
	m_uOtherMapCount = 0;

	Scene_PlayMode mode = getCurrentScenePlayMode();
	setCurrentScenePlayMode(mode_Common);
	getGameUnitLayer()->startCommonMode(mode);
	getGameInTouchLayer()->startCommon();
	getCurrentStateBar()->setProtectMenuVisable(true);
}
bool RunningHelper::isTeachScene()
{
// 	if (m_curRunningMode == RunningMode::tollgate && m_curSectionInfor.size()>2 && m_curSectionInfor[0]==0 && m_curSectionInfor[1]==0) return true;
// 	else return false;
	if (m_curRunningMode == RunningMode::teach) return true;
	else return false;
}

void RunningHelper::teach(string attr)
{
	this->gamePause();

	if (attr == Attr_TeachJump){
		this->getGameInTouchLayer()->teachJump();
	}
	else if(attr == Attr_TeachDoubJump){
		this->getGameInTouchLayer()->teachDoubleJump();
	}
	else if (attr == Attr_TeachDown){
		this->getGameInTouchLayer()->teachDown();
	}
	else if (attr == Attr_TeachUp){
		this->getGameInTouchLayer()->teachUp();
	}
	else {
		CCASSERT(false, "error teach branch");
	}
}
StateBar* RunningHelper::getCurrentStateBar()
{
	return ((StateBar*)m_pRunningScene->getChildByName("StateBar"));
}
SimpleBackgroundLayer* RunningHelper::getBackgroundLayer()
{
	return ((SimpleBackgroundLayer*)m_pRunningScene->getChildByName("SimpleBackgroundLayer"));
}

SimpleFrontgroundLayer* RunningHelper::getFrontgroundLayer()
{
	return ((SimpleFrontgroundLayer*)m_pRunningScene->getChildByName("SimpleFrontgroundLayer"));
}

void RunningHelper::MoveCamera(float time, float deltaY, bool castSameTimeBack/*=false*/)
{
//	return;
	SimpleBackgroundLayer* bgLayer = getBackgroundLayer();

//	CCLOG("move camera hero y = %.1f, new y = %.1f", getGameUnitLayer()->getHeroSprite()->getCollisonRectInWorldSpace().getMidY(), getGameUnitLayer()->getHeroSprite()->getCollisonRectInWorldSpace().getMidY()+deltaY);

	if (time == 0)
	{

		//1. gameunit layer
		getGameUnitLayer()->moveLayer_byMoveCamera(time, deltaY);

		//2. background Layer
		if (! bgLayer) return;
// 		// 1) 近景
// 		ScrollForeverNode* pScrollNode = bgLayer->getBGLayer_JinJing();
// 		if (pScrollNode)
// 		{
// 			pScrollNode->setPositionY(pScrollNode->getPositionY()+deltaY * pScrollNode->getBackGroundInfo().CameraMoveFactor_Y);
// 		}
// 		// 2) 中景
// 		pScrollNode = bgLayer->getBGLayer_ZhongJing();
// 		if (pScrollNode)
// 		{	
// 			pScrollNode->setPositionY(pScrollNode->getPositionY()+deltaY * pScrollNode->getBackGroundInfo().CameraMoveFactor_Y);
// 		}

		int i = 0;
		while (PiaoFuWuNode* pNode = dynamic_cast<PiaoFuWuNode*>(bgLayer->getChildByTag(SimpleBackgroundLayer::iTag_PiaoFuWuNodeStart+i)))
		{
			pNode->setPositionY(pNode->getPositionY()+deltaY * pNode->getLayersBackgroundInfo().CameraMoveFactor_Y);
			++i;
		}
	}
	else 
		CCASSERT(false, "没有地方用 time > 0 的");
// 	else 
// 	{
// 		int actTag = 99999;
// 
// 		{
// 			Sequence *move1 = Sequence::create(
// 				MoveTo::create(time, getGameUnitLayer()->getObstacleContainerLayer()->getPosition()+Vec2(0, deltaY)),
// 				castSameTimeBack ? MoveBy::create(time, Vec2(0, -deltaY))->reverse() : nullptr,
// 				nullptr);
// 			move1->setTag(actTag);
// 			getGameUnitLayer()->getObstacleContainerLayer()->stopActionByTag(actTag);
// 			getGameUnitLayer()->getObstacleContainerLayer()->runAction(move1);
// 
// 			Sequence *move2 = Sequence::create(
// 				MoveTo::create(time, getGameUnitLayer()->getHeroSprite()->getPosition()+Vec2(0, deltaY)),
// 				castSameTimeBack ? MoveBy::create(time, Vec2(0, -deltaY))->reverse() : nullptr,
// 				nullptr);
// 			move2->setTag(actTag);
// 			getGameUnitLayer()->getHeroSprite()->stopActionByTag(actTag);
// 			getGameUnitLayer()->getHeroSprite()->runAction(move2);
// 		}
// 		
// 		ScrollForeverNode* pScrollNode = getBackgroundLayer()->getBGLayer_JinJing();
// 		if (pScrollNode)
// 		{
// 			Sequence *move = Sequence::create(
// 				MoveTo::create(time, pScrollNode->getPosition()+Vec2(0, deltaY * 0.3f)),
// 				castSameTimeBack ? MoveBy::create(time, Vec2(0, -deltaY * 0.3f))->reverse() : nullptr,
// 				nullptr);
// 			move->setTag(actTag);
// 			pScrollNode->stopActionByTag(actTag);
// 			pScrollNode->runAction(move);
// 		}
// 		pScrollNode = getBackgroundLayer()->getBGLayer_ZhongJing();
// 		if (pScrollNode)
// 		{	
// 			Sequence *move = Sequence::create(
// 				MoveTo::create(time, pScrollNode->getPosition()+Vec2(0, deltaY * 0.1f)),
// 				castSameTimeBack ? MoveBy::create(time, Vec2(0, -deltaY * 0.1f))->reverse() : nullptr,
// 				nullptr);
// 			move->setTag(actTag);
// 			pScrollNode->stopActionByTag(actTag);
// 			pScrollNode->runAction(move);
// 		}
// 	}
}

//scale == 1 就是放大回去
//其余都是缩小
void RunningHelper::scaleCamera(float time, float scale, float backTime/*=0*/)
{
	if (this->isTeachScene()) return; // 教学关卡不要缩放

	Size winSize = Director::getInstance()->getWinSize();
	float rate = 2.4f;

	int actTag = 99998;
	if (scale == 1)
	{
		//1) game unit layer
		Sequence *seq = Sequence::create(
			EaseIn::create(ScaleTo::create(time, 1), rate),
			nullptr);
		seq->setTag(actTag);
		getGameUnitLayer()->stopActionByTag(actTag);
		getGameUnitLayer()->runAction(seq);

		//2) back ground
		SimpleBackgroundLayer* bgLayer = getBackgroundLayer();
		if (bgLayer)
		{
// 			if (bgLayer->getBGLayer_JinJing() && scale <= 1)
// 			{
// 				Sequence *seq = Sequence::create(
// 					EaseIn::create(ScaleTo::create(time, 1), rate),
// 					nullptr);
// 				seq->setTag(actTag);
// 				bgLayer->getBGLayer_JinJing()->stopActionByTag(actTag);
// 				bgLayer->getBGLayer_JinJing()->runAction(seq);
// 			}
// 			if (bgLayer->getBGLayer_ZhongJing() && scale <= 1)
// 			{
// 				Sequence *seq = Sequence::create(
// 					EaseIn::create(ScaleTo::create(time, 1), rate),
// 					nullptr);
// 				seq->setTag(actTag);
// 				bgLayer->getBGLayer_ZhongJing()->stopActionByTag(actTag);
// 				bgLayer->getBGLayer_ZhongJing()->runAction(seq);
// 			}

			int i = 0;
			while (PiaoFuWuNode* pNode = dynamic_cast<PiaoFuWuNode*>(bgLayer->getChildByTag(SimpleBackgroundLayer::iTag_PiaoFuWuNodeStart+i)))
			{
				Sequence *seq = Sequence::create(
					EaseIn::create(ScaleTo::create(time, 1), rate),
					nullptr);
				seq->setTag(actTag);
				pNode->stopActionByTag(actTag);
				pNode->runAction(seq);
				++i;
			}
		}
	}
	else 
	{
		Sequence *seq = Sequence::create(
			EaseOut::create(ScaleTo::create(time, scale),rate),
			backTime>0 ? EaseIn::create(ScaleTo::create(backTime, 1),rate) : nullptr,
			nullptr);
		seq->setTag(actTag);
		getGameUnitLayer()->stopActionByTag(actTag);
		getGameUnitLayer()->runAction(seq);

		SimpleBackgroundLayer* bgLayer = getBackgroundLayer();
		if (bgLayer)
		{
// 			if (bgLayer->getBGLayer_JinJing() && scale <= 1)
// 			{
// 				//float fSca = 1-(1-scale)*0.9f;
// 				//if (fSca < 0.6f) fSca = 0.6f;
// 				float fSca = 1-(1-scale)*0.9f;
// 				if (fSca < 0.8f) fSca = 0.8f;
// 				Sequence *seq = Sequence::create(
// 					EaseOut::create(ScaleTo::create(time, fSca),rate),
// 					backTime>0 ? EaseIn::create(ScaleTo::create(backTime, 1),rate) : nullptr,
// 					nullptr);
// 				seq->setTag(actTag);
// 				bgLayer->getBGLayer_JinJing()->stopActionByTag(actTag);
// 				bgLayer->getBGLayer_JinJing()->runAction(seq);
// 			}
// 			if (bgLayer->getBGLayer_ZhongJing() && scale <= 1)
// 			{
// 				float fSca = 1-(1-scale)*0.9f;
// 				if (fSca < 0.8f) fSca = 0.8f;
// 				Sequence *seq = Sequence::create(
// 					EaseOut::create(ScaleTo::create(time, fSca),rate),
// 					backTime>0 ? EaseIn::create(ScaleTo::create(backTime, 1),rate) : nullptr,
// 					nullptr);
// 				seq->setTag(actTag);
// 				bgLayer->getBGLayer_ZhongJing()->stopActionByTag(actTag);
// 				bgLayer->getBGLayer_ZhongJing()->runAction(seq);
// 			}

			int i = 0;
			while (PiaoFuWuNode* pNode = dynamic_cast<PiaoFuWuNode*>(bgLayer->getChildByTag(SimpleBackgroundLayer::iTag_PiaoFuWuNodeStart+i)))
			{
				float fSca = 1-(1-scale)*0.9f;
				if (fSca < 0.8f) fSca = 0.8f;
				Sequence *seq = Sequence::create(
					EaseOut::create(ScaleTo::create(time, fSca),rate),
					backTime>0 ? EaseIn::create(ScaleTo::create(backTime, 1),rate) : nullptr,
					nullptr);
				seq->setTag(actTag);
				pNode->stopActionByTag(actTag);
				pNode->runAction(seq);
				++i;
			}
		}
	}
}
float RunningHelper::getCameraScale()
{
	return getGameUnitLayer()->getScale();
}

void RunningHelper::playAnimationEnd(Armature *armature, MovementEventType type, const std::string& movementID)
{
	if (type == COMPLETE)
	{
		if (movementID == Armature_BOSS_ComeIn)
			armature->removeFromParent();
	}
}