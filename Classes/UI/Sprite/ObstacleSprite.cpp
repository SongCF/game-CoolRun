#include "ObstacleSprite.h"
#include "HeroSprite.h"
#include "BossSprite.h"
#include "RunningHelper.h"
#include "GameUnitLayer.h"
#include "GoodsHelper.h"
#include "UnitSpriteCache.h"
#include "RunningHelper.h"
#include "UnitCollisionDetection.h"
#include "UnitActionHelper.h"
#include "UnitTriggerHelper.h"
#include "Music.h"
#include "FXJump.h"
#include "ActionConfigManager.h"
#include "VersionConfig.h"

#define USE_SPRITE_CACHE 1

#define AniName_Daoju_Texiao_Baozha			"daoju_texiao_baozha"


static int g_goldMusic = 0; //一帧只播放一个金币的音效

enum ChildTag
{
	Tag_TreadDown = 100,
	Tag_JumpFallAction,
};
ObstacleSprite* ObstacleSprite::create(Unit* unit, unsigned int actId, vector<unsigned int> triggerId)
{
#if USE_SPRITE_CACHE == 1
	ObstacleSprite* pSprite = UnitSpriteCache::getInstance()->getSpriteByCache(unit);
#else
	ObstacleSprite* pSprite = new ObstacleSprite;
#endif

	if (pSprite && pSprite->init(unit))
	{
#if USE_SPRITE_CACHE != 1
		pSprite->autorelease();
#endif
		pSprite->setActionID(actId);
		pSprite->setTriggerID(triggerId);
		pSprite->scheduleOnce(schedule_selector(ObstacleSprite::start), 0.01f);

		//
		if (unit->ID == 504 || unit->ID == 508 || unit->ID == 509 || unit->ID == 510 || unit->ID == 545 || unit->ID == 560){
			Armature *pAr = Armature::create("effect");
			pSprite->addChild(pAr,-1);
			pAr->setPosition(pSprite->getContentSize()/2);
			pAr->getAnimation()->play("e_daoju");
		}

	}else 
	{
		CC_SAFE_DELETE(pSprite);
		pSprite = nullptr;
	}
	return pSprite;
}

ObstacleSprite::ObstacleSprite()
	:m_iBeAbsorbed(0)
	,m_actionId(0)
//	,m_triggerId(0) //vector
{
}

void ObstacleSprite::reinitialize()
{
	//0. node
	this->setVisible(true);
	this->setOpacity(255);
	this->setScale(1);
	this->setRotation(0);

	//1. super
	m_unit.ID = 0;
	m_unit.type = unittype_None;

	//2. this
	for (int i=0,count=m_lListenerList.size(); i<count; ++i) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_lListenerList[i]);
	}
	m_lListenerList.clear();
	this->stopAllActions();
	this->unscheduleAllSelectors();

	m_iBeAbsorbed = 0;
	m_actionId = 0;
	m_triggerId.clear();
}

ObstacleSprite::~ObstacleSprite()
{
}

void ObstacleSprite::onEnter()
{
	UnitSprite::onEnter();

	//1.震荡波 （极速状态完的那一瞬间）
	m_lListenerList.push_back(_eventDispatcher->addCustomEventListener(MSG_ShockWave, 
		[&](EventCustom* ev){this->heroShaken(Vec2(*(Vec2*)ev->getUserData()));}));
	//2.吸收 （磁铁效果）
	m_lListenerList.push_back(Director::getInstance()->getEventDispatcher()->addCustomEventListener(MSG_Absorbe, 
		[&](EventCustom* ev){this->beAbsorbed(Vec2(*(Vec2*)ev->getUserData()));}));
	//3.点金
	m_lListenerList.push_back(_eventDispatcher->addCustomEventListener(MSG_ChangeGold, 
		[&](EventCustom* ev){this->changeGold(Vec2(*(Vec2*)ev->getUserData()));}));
}
void ObstacleSprite::onExit() 
{
	for (vector<EventListenerCustom*>::iterator iter = m_lListenerList.begin() ; iter != m_lListenerList.end() ; iter++)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(*iter);
	}
	m_lListenerList.clear();

// 	this->stopAllActions();
// 	this->unscheduleAllSelectors();
	UnitSprite::onExit();
}

void ObstacleSprite::start(float dt)
{
	//间隙 ---  彩虹桥
	if (m_unit.ID == unitID_JianXi)
	{
		this->unschedule(schedule_selector(ObstacleSprite::JianXiShowAbleCheck));
		this->schedule(schedule_selector(ObstacleSprite::JianXiShowAbleCheck), 0.15f);
	}
	else if (m_unit.ID == unitID_ZhongDuJieYao)
	{//没有中毒就清掉解药
		HeroSprite *pHero = RunningHelper::getHelper()->getRunnerHero();
		if (!pHero || !pHero->isHaveAttrOnGoing(Attr_Poison)){
			this->runAction(Sequence::create(RemoveSelf::create(true), nullptr));
			return;
		}
	}
	else if (m_unit.type == unittype_Land) //如果加载到了陆地 不是彩虹桥 那么证明彩虹桥加载完了。
	{
		RunningHelper::getHelper()->getRunnerHero()->setIsShowCaiHongQiao(false);
	}

	//配置的action
	if (m_actionId != 0) 
	{
		UnitActionHelper::runActionByActionId(this, m_actionId);
	}

	//配置的Trigger
	if (! m_triggerId.empty())
	{
		this->scheduleUpdate();
	}
}

void ObstacleSprite::JianXiShowAbleCheck(float dt)
{
	if (RunningHelper::getHelper()->getRunnerHero()->getIsShowCaiHongQiao()) 
		this->setVisible(true);
}

Rect ObstacleSprite::getCollisonRectInWorldSpace()
{
	if (! this->getParent()) return Rect::ZERO; //避免从父类删除后、在碰撞数组中没有删

	Rect delta = Rect::ZERO;

	if (m_unit.rect.size() > 1) {
		CCASSERT(false, "error");
	}
	else if (m_unit.rect.size() == 1) {
		delta = m_unit.rect[0];
	}
	else {
		delta = Rect::ZERO;
	}


	Rect ret;
	Vec2 worldPos = this->getParent()->convertToWorldSpace(this->getPosition());
	float scaleX = this->getScaleX() * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
	float scaleY = this->getScaleY() * RunningHelper::getHelper()->getGameUnitLayer()->getScaleY();
	if (delta.equals(Rect::ZERO))
	{
		Size size = this->getContentSize();
		size.setSize(size.width*scaleX, size.height*scaleY);
		ret = Rect(worldPos.x-size.width/2, worldPos.y-size.height/2, size.width, size.height);
	}else
	{
		worldPos.x += delta.getMinX()*scaleX;
		worldPos.y += delta.getMinY()*scaleY;
		ret = Rect(worldPos.x-delta.size.width/2*scaleX, worldPos.y-delta.size.height/2*scaleY, delta.size.width*scaleX, delta.size.height*scaleY);
	}
	
	return ret;
}

Rect ObstacleSprite::getRectInObsContainerSpace()
{
	if (! this->getParent()) return Rect::ZERO; //避免从父类删除后、在碰撞数组中没有删

	// 	float scalX = _scaleX * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
	// 	float scalY = _scaleY * RunningHelper::getHelper()->getGameUnitLayer()->getScaleY();
	float w,h;
	if (m_unit.rect.size() == 1) {
		w = m_unit.rect[0].size.width * _scaleX;// * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
		h = m_unit.rect[0].size.height * _scaleY;// * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
		return Rect(_position.x + m_unit.rect[0].origin.x * _scaleX - w/2, _position.y + m_unit.rect[0].origin.y * _scaleY - h/2, w, h);
	}
	else if (m_unit.rect.size() == 0) {
		w = _contentSize.width * _scaleX;// * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
		h = _contentSize.height * _scaleY;// * RunningHelper::getHelper()->getGameUnitLayer()->getScaleY();
		return Rect(_position.x-w/2, _position.y-h/2, w, h);
	}
	else {
		CCASSERT(false, "error");
		return Rect::ZERO;
	}
}

void ObstacleSprite::onCollision(UnitSprite* pSprite/* = nullptr*/, enumCollisionPos board/*=collideLEFT*/)
{
	CCASSERT(pSprite != nullptr, "");
	HeroSprite *pHeroSprite = (HeroSprite*)pSprite;


	//1. 特殊id
	bool bDefault = false;//是否没有经历switch处理
	UnitID obsId = (UnitID)m_unit.ID;
	switch (obsId)
	{
	case unitID_JinBi:
	case unitID_JinDan:
		{
			this->showGoldAnimation();
			this->damage();
			//music
			if (0 >= g_goldMusic){
				++g_goldMusic;
				FX::Music::getInstance()->playEffectMusic("r_get_star_01");
				Director::getInstance()->getScheduler()->schedule([&](float dt){
					--g_goldMusic;
				}, RunningHelper::getHelper(), 0, 0, 0, false, "RESET_GOLD_MUSIC_NUM"); //用runninghelper做target是因为this可能在这一帧结束就删除了
			}
		}
		break;
		//红宝石
	case unitID_HongBaoShi:
		{
			RunningHelper::getHelper()->addScore(1, true);
			this->damage();
		}
		break;
	case unitID_TanHuang:
		{
			//避免弹簧多次碰撞
			UnitCollisionDetection *unitCollisionDetection = RunningHelper::getHelper()->getUnitCollsionDetection();
			if (unitCollisionDetection) unitCollisionDetection->addWillBeRemoveFromVec(const_cast<ObstacleSprite*>(this));
		}
		break;
		//子弹
	case unitID_ZiDanXiao:
	case unitID_ZiDanDa:
	case unitID_ZiDanChaoDa:
		{
			Rect obsRect = this->getCollisonRectInWorldSpace();
			this->damage();
			BossSprite *pBoss = RunningHelper::getHelper()->getGameUnitLayer()->getBoss();
			if (pBoss){
				Armature *destroyAr = Armature::create("moping");
				destroyAr->getAnimation()->setMovementEventCallFunc([=](Armature *armature, MovementEventType movementType, const std::string& movementID){
					if (movementType == COMPLETE && movementID == "polie"){
						destroyAr->removeFromParent();
					}
				});
				pBoss->getParent()->addChild(destroyAr);
				destroyAr->setPosition(obsRect.getMidX(),obsRect.getMidY());
				destroyAr->getAnimation()->play("polie");
			}
		}
		break;
	default:
		bDefault = true;
		break;
	}
	if (! bDefault) 
		return;

	//2. 特殊类型
	if (m_unit.type == unittype_Item)
	{
		this->damage();
		return;
	}

	//3. 特殊属性
	if (board == collideDOWN && (m_unit.haveAttribute(Attr_TopBumpAddScore) || m_unit.haveAttribute(Attr_TopBumpApearItem) || m_unit.haveAttribute(Attr_TopBumpDisapear))
	 || board == collideUP && (m_unit.haveAttribute(Attr_TreadAddScore) || m_unit.haveAttribute(Attr_TreadApearItem) || m_unit.haveAttribute(Attr_TreadDisapear)))
	{
		UnitCollisionDetection *unitCollisionDetection = RunningHelper::getHelper()->getUnitCollsionDetection();
		unitCollisionDetection->addWillBeRemoveFromVec(const_cast<ObstacleSprite*>(this));
		this->setOpacity(0);
		//				this->damage(); //直接damage会没有动画
		this->showBaozhaAnimation();
		//music
		FX::Music::getInstance()->playEffectMusic("r_monster_break");
		return;
	}

	//4. 特殊英雄状态
	if (pHeroSprite->isNotInjured())
	{
		if (m_unit.type == unittype_Obstacle)
		{
			this->damage();
			return;
		}
	}
}

/*
void ObstacleSprite::onCollision(UnitSprite* pSprite, enumCollisionPos board)
{
	HeroSprite* hero = dynamic_cast<HeroSprite*>(pSprite);
	HeroSprite::HeroState state = hero->getCurState();
	//预处理1：无敌状态特殊处理
	if (m_unit.type == unittype_Obstacle){
		//添加hero->getWillBeChongCi()判断，在进入冲刺状态下落的时刻也相当于无敌
		if (hero && (hero->isHaveAttrOnGoing(Attr_Invincible) || hero->isHaveAttrOnGoing(Attr_Rush)
			|| state == HeroSprite::state_Bounce || state == HeroSprite::state_BounceBack
			|| state == HeroSprite::state_Super || hero->getWillSuperRun()
			|| state == HeroSprite::state_ChongCi || hero->getWillBeChongCi()))
		{
			if (this->getSelfUnit()->ID != unitID_DianZha)
			{
				//无敌状态和冲刺状态时，要先添加到移除数组，防止出现被阻挡一下的情况
				UnitCollisionDetection *unitCollisionDetection = RunningHelper::getHelper()->getUnitCollsionDetection();
				unitCollisionDetection->addWillBeRemoveFromVec(const_cast<ObstacleSprite*>(this));

				this->runAction(Sequence::create(FadeTo::create(0.07f,125),CallFunc::create(std::bind(&ObstacleSprite::damage,this)),nullptr));
				return;
			}
		}
	}


	UnitID uTypeId = (UnitID)m_unit.ID;
	switch (uTypeId)
	{
	case unitID_TongBi:				//坚果
	case unitID_YinBi:			//草莓果
	case unitID_JinBi:			//蓝莓果
		{
			this->showGoldAnimation();
			this->damage();
			//music
			if (0 >= g_goldMusic){
				++g_goldMusic;
				FX::Music::getInstance()->playEffectMusic("r_get_star_01");
				Director::getInstance()->getScheduler()->schedule([&](float dt){
					--g_goldMusic;
				}, RunningHelper::getHelper(), 0, 0, 0, false, "RESET_GOLD_MUSIC_NUM"); //用runninghelper做target是因为this可能在这一帧结束就删除了
			}
		}
		break;
	case unitID_ChongCi:			//火箭5m
		{
			this->damage();
		}
		break;
	case unitID_KaiChangChongCi:			//极速飞行
		{
			this->damage();
		}
		break;
	case unitID_SiWangChongCi:			//意念疾飞
		{
			this->damage();
		}
		break;
	case unitID_WuDi:		//无敌火龙果
		{
			this->damage();
		}
		break;
	case unitID_CiTie:				//磁铁
		{
			this->damage();
		}
		break;
	case unitID_DianJin:			//魔法棒
		{
			this->damage();
		}
		break;
	case unitID_HuDun:			//五彩气泡
		{
			this->damage();
		}
		break;
	case unitID_Gold:				//金币
		{
			CCASSERT(false, "不支持");
		}
		break;
	case unitID_Diamond:				//钻石
		{
			CCASSERT(false, "不支持");
		}
		break;
	case unitID_MuZhuang_Ai:			//木桩（矮）
		{
			
		}
		break;
	case unitID_MuZhuang_Gao:		//木桩（高）
		{

		}
		break;
	case unitID_JianXi:			//彩虹桥
		{

		}
		break;
	//可踩踏
	case unitID_JingTaiGuaiWu_Xiao:					
	case unitID_JingTaiGuaiWu_Da:
	case unitID_CaiTa_Xiao:
	case unitID_CaiTa_Da:
	case unitID_KeCaiTa:		
		{
			if (board == collideDOWN || collideUP == board)
			{
				UnitCollisionDetection *unitCollisionDetection = RunningHelper::getHelper()->getUnitCollsionDetection();
				unitCollisionDetection->addWillBeRemoveFromVec(const_cast<ObstacleSprite*>(this));
				this->setOpacity(0);
//				this->damage(); //直接damage会没有动画
				this->showBaozhaAnimation();
				//music
				FX::Music::getInstance()->playEffectMusic("r_monster_break");
			}
		}
		break;
	case unitID_DianZha_KaiGuan:		//电闸-开关
		{
			this->damage();
			removeDianZha();
		}
		break;
	case unitID_LuMian_1:				//藤条1
		{

		}
		break;
	case unitID_LuMian_2:				//藤条2
		{

		}
		break;
	case unitID_LuMian_3:				//藤条3
		{

		}
		break;
	case unitID_MiFeng:					//蜜蜂
		{

		}
		break;
		
	//没有滚石了，所以先屏蔽掉
// 	case unitID_GunShi_Da:
// 		{
// 			const vector<Attribute>* attrVec = pSprite->getSelfAttribute();
// 			string temp;
// 			for (int i=0,count=attrVec->size(); i<count; ++i)
// 			{
// 				temp = attrVec->at(i).attr;
// 				if(temp == Attr_Obstruct)				//阻挡
// 				{
// 					if (board == collideRIGHT)
// 					{
// 						this->stopAllActions();
// 					}
// 				}
// 				if(pSprite->getSelfUnit()->type == unittype_Actor)	
// 				{
// 					this->stopAllActions();
// 				}
// 			}
// 		}
// 		break;
// 	case  unitID_GunShi_Xiao:
// 		{
// 			const vector<Attribute>* attrVec = pSprite->getSelfAttribute();
// 			string temp;
// 			for (int i=0,count=attrVec->size(); i<count; ++i)
// 			{
// 				temp = attrVec->at(i).attr;
// 				if(temp == Attr_Obstruct)				//阻挡
// 				{
// 					if (board == collideRIGHT)
// 					{
// 						this->stopAllActions();
// 					}
// 				}
// 				if(pSprite->getSelfUnit()->type == unittype_Actor)	
// 				{
// 					this->stopAllActions();
// 				}
// 			}
// 		}
// 		break;
	case unitID_TanHuang:
		{
			//避免弹簧多次碰撞
			UnitCollisionDetection *unitCollisionDetection = RunningHelper::getHelper()->getUnitCollsionDetection();
			if (unitCollisionDetection) unitCollisionDetection->addWillBeRemoveFromVec(const_cast<ObstacleSprite*>(this));
		}
		break;
// 	case unitID_Land_stone_zuo:
// 		{
// 			//no thing
// 			if (((HeroSprite*)pSprite)->getCurState() == HeroSprite::state_ChongCi)
// 			{
// 				this->damage();
// 			}
// 		}
// 		break;
// 	case unitID_Land_stone_you:
// 		{
// 			//no thing
// 			if (((HeroSprite*)pSprite)->getCurState() == HeroSprite::state_ChongCi)
// 			{
// 				this->damage();
// 			}
// 		}
// 		break;
	case unitID_ZhongDuJieYao:
		{
			this->damage();
		}
		break;

	case unitID_ZhongDu:		//中毒
		{
			this->damage();
		}
		break;

	case unitID_XuanFuZhangAi_DownToUp:		//
		{
			this->damage();
		}
		break;
	default:
		if (m_unit.type == unittype_Item) 
			this->damage();

		break;
	}
}
*/

Node* ObstacleSprite::getOneGoldEffectNode()
{
	static int s_cacheCount = 0;
	static int s_cacheIndex = 0;
	static Vector<Node*> s_cachePool;

	//已经创建到了最大个数了
	if (s_cacheCount >= g_fMaxGoldEffectCount)
	{
		if (s_cacheIndex >= s_cacheCount)
			s_cacheIndex = 0;
		return s_cachePool.at(s_cacheIndex++);
	}
	//需要创建一个新的
	else
	{
		Node* pActionNode = Node::create();
		//
		Sprite* pQuan = Sprite::create("daoju_chijinbi_quan.png");
		pActionNode->addChild(pQuan, 1, "GoldEffect_Quan");
		//
		for (unsigned int i = 0; i < 5; ++i)
		{
			Sprite* pStar = Sprite::create("daoju_chijinbi_xing.png");
			pActionNode->addChild(pStar, 1, "");
		}

		s_cachePool.pushBack(pActionNode);
		++s_cacheCount;
		++s_cacheIndex;

		return pActionNode;
	}
}

void ObstacleSprite::showGoldAnimation()
{
	if (CONFIG_ON != g_uGameInEffectSwitch) return;

	int iGoldEffectCount = RunningHelper::getHelper()->getGoldEffectCount();
	if (iGoldEffectCount >= g_fMaxGoldEffectCount || ! this->getParent())
	{
		//CCLOG("%s ---> return", __FUNCTION__);
		return;
	}
	RunningHelper::getHelper()->setGoldEffectCount(RunningHelper::getHelper()->getGoldEffectCount()+1);

	///////////////////////////////
	float framePreTime = Director::getInstance()->getAnimationInterval();
	Node* pActionNode  = getOneGoldEffectNode();
	pActionNode->removeFromParent(); //
	this->getParent()->addChild(pActionNode);
	Point pos = this->getPosition();
	pos.x -= 70;
	pos.y += CCRANDOM_MINUS1_1() * this->getContentSize().height;
	pActionNode->setPosition(pos);
	pActionNode->runAction(Sequence::create(
		DelayTime::create(16*framePreTime),
		CallFunc::create([=](){
			int iCount = RunningHelper::getHelper()->getGoldEffectCount();
			if (iCount > 0) RunningHelper::getHelper()->setGoldEffectCount(--iCount);
			pActionNode->removeFromParent();
	}),
		nullptr));
	//
	Sprite* pQuan = (Sprite*)pActionNode->getChildByName("GoldEffect_Quan");
	if (pQuan)
	{
		pQuan->setPosition(Vec2(0, 0));
		pQuan->setOpacity(125);
		pQuan->setScale(1);
		pQuan->runAction(Sequence::create(
			Spawn::create( 
				MoveTo::create(framePreTime, Vec2(1, 0)),
				ScaleTo::create(framePreTime, 1.2f),
				FadeTo::create(framePreTime, 50),
				nullptr),
			nullptr)
			);
	}
	//
	std::vector<Point> points;
	points.push_back(Vec2(-30, 22));
	points.push_back(Vec2(35, 21));
	points.push_back(Vec2(3, 30));
	points.push_back(Vec2(-22, -14));
	points.push_back(Vec2(25, -12));
	points.push_back(Vec2(-14, 0));
	std::vector<Point> points1;
	points1.push_back(Vec2(-54, 29));
	points1.push_back(Vec2(53, 43));
	points1.push_back(Vec2(1, 50));
	points1.push_back(Vec2(-45, -60));
	points1.push_back(Vec2(38, -52));
	points1.push_back(Vec2(-45, 2));

	Vector<Node*>& childrenArr = pActionNode->getChildren();
	for (int i=0,count=childrenArr.size(); i<count; ++i)
	{
		Sprite* pStar = (Sprite*)childrenArr.at(i);
		if (! pStar || pStar == pQuan) continue;

		pStar->setScale(3.0f);
		pStar->setOpacity(125);
		pStar->setVisible(false);
		pStar->setPosition(points.at(i));

		pStar->runAction(
			Sequence::create(
				DelayTime::create(framePreTime),
				Show::create(),
				Spawn::create(
					ScaleTo::create(4*framePreTime, 1.4f),
					FadeTo::create(4*framePreTime, 255),
					nullptr),
				Spawn::create(
					ScaleTo::create(10*framePreTime, 0.5f),
					FadeTo::create(10*framePreTime, 0),
					MoveTo::create(10*framePreTime, points1.at(i)),
					nullptr),
				Hide::create(),
			nullptr)
			);
	}
	/////////////////////////////////

/*
return;

	float framePreTime = Director::getInstance()->getAnimationInterval();
	Node* pActionNode = Node::create();

	Sprite* pQuan = Sprite::create("daoju_chijinbi_quan.png");
	pQuan->setPosition(Vec2(0, 0));
	pQuan->setOpacity(125);
	pActionNode->addChild(pQuan);
	pQuan->runAction(Sequence::create(
			Spawn::create( 
				MoveTo::create(framePreTime, Vec2(1, 0)),
				ScaleTo::create(framePreTime, 1.2f),
				FadeTo::create(framePreTime, 50),
				nullptr),
			RemoveSelf::create(),
			nullptr)
		);

	std::vector<Point> points;
	points.push_back(Vec2(-30, 22));
	points.push_back(Vec2(35, 21));
	points.push_back(Vec2(3, 30));
	points.push_back(Vec2(-22, -14));
	points.push_back(Vec2(25, -12));
	points.push_back(Vec2(-14, 0));

	std::vector<Point> points1;
	points1.push_back(Vec2(-54, 29));
	points1.push_back(Vec2(53, 43));
	points1.push_back(Vec2(1, 50));
	points1.push_back(Vec2(-45, -60));
	points1.push_back(Vec2(38, -52));
	points1.push_back(Vec2(-45, 2));

	int iCount = 6;
	for (unsigned int i = 0; i < iCount; ++i)
	{
		Sprite* pStar = Sprite::create("daoju_chijinbi_xing.png");
		pStar->setScale(3.0f);
		pStar->setOpacity(125);
		pStar->setVisible(false);
		pStar->setPosition(points.at(i));

		pStar->runAction(
			Sequence::create(
			DelayTime::create(framePreTime),
			Show::create(),
			Spawn::create(
			ScaleTo::create(4*framePreTime, 1.4f),
			FadeTo::create(4*framePreTime, 255),
			nullptr),
			Spawn::create(
			ScaleTo::create(10*framePreTime, 0.5f),
			FadeTo::create(10*framePreTime, 0),
			MoveTo::create(10*framePreTime, points1.at(i)),
			nullptr),
			RemoveSelf::create(),
			nullptr)
			);
		pActionNode->addChild(pStar);
	}
	
	if (this->getParent()) this->getParent()->addChild(pActionNode);
	Point pos = this->getPosition();
	pos.x -= 70;
	pos.y += CCRANDOM_MINUS1_1() * this->getContentSize().height;
	pActionNode->setPosition(pos);
	pActionNode->runAction(Sequence::create(
		DelayTime::create(16*framePreTime),
		CallFunc::create([=](){
			int iCount = RunningHelper::getHelper()->getGoldEffectCount();
			if (iCount > 0) RunningHelper::getHelper()->setGoldEffectCount(--iCount);
			pActionNode->removeFromParent();
			}),
		nullptr));
*/
}

void ObstacleSprite::showBaozhaAnimation()
{
	Animate *animate = nullptr;
	Sprite* pActionNode = Sprite::create("daoju_tiexiao_baozha_01.png");
	{
		float framePreTime = Director::getInstance()->getAnimationInterval();
		char filename[64] = {};
		Animation* pAnimation = Animation::create();
		pAnimation->setDelayPerUnit(0.017f);
		for (unsigned int i = 0; i < 7; ++i)
		{
			sprintf(filename, "daoju_tiexiao_baozha_%02d.png", i+1);
			pAnimation->addSpriteFrameWithFile(filename);
		}
		animate = Animate::create(pAnimation);
		pAnimation->setDelayPerUnit(framePreTime);
		pAnimation->setLoops(1);
	}
	pActionNode->runAction(Sequence::create(
		animate,
		CallFunc::create([=](){this->damage();}),
		RemoveSelf::create(),
		nullptr));
	pActionNode->setName(AniName_Daoju_Texiao_Baozha);
	pActionNode->setTag(Tag_TreadDown);

	this->addChild(pActionNode);
	pActionNode->setPosition(this->getContentSize()/2);
}

//#define ShockWaveWidth 400
void ObstacleSprite::heroShaken(Vec2 heroWorldPos)
{
	float posX = this->getCollisonRectInWorldSpace().getMidX();
	if (posX < mWinSize.width * 2)
	{
		if (m_unit.type == unittype_Obstacle){
			this->damage();
		} else {
			if (unitID_JianXi == getSelfUnit()->ID)
			{	
				this->setVisible(true);
				RunningHelper::getHelper()->getGameUnitLayer()->getObstacle()->pushBack(this);
			}
		}
	}
}

void ObstacleSprite::damage()
{
	UnitCollisionDetection *unitCollisionDetection = RunningHelper::getHelper()->getUnitCollsionDetection();
	if (unitCollisionDetection) unitCollisionDetection->addWillBeRemoveFromVec(this);
	this->removeFromParent();

	this->stopAllActions();
	this->unscheduleAllSelectors();
	//不能再检测碰撞的过程中删除碰撞数组里面的东西
	//	RunningHelper::getHelper()->getGameUnitLayer()->getObstacle()->eraseObject(this);
}

//#define AbsorbedMoveWidth 6
void ObstacleSprite::beAbsorbed(Vec2 heroWorldPos)
{
	if (m_iBeAbsorbed==0 && m_unit.type == unittype_Item)
	{
		float posX = this->getCollisonRectInWorldSpace().getMinX();
		if (posX < mWinSize.width && posX > heroWorldPos.x)  m_iBeAbsorbed = 1;

		// 处于被吸收状态
		this->unschedule(schedule_selector(ObstacleSprite::tick_BeAbsorbed));
		this->schedule(schedule_selector(ObstacleSprite::tick_BeAbsorbed));
	}
}

void ObstacleSprite::tick_BeAbsorbed(float dt)
{
	if (m_iBeAbsorbed > 0){
		++m_iBeAbsorbed;

		HeroSprite * hero = RunningHelper::getHelper()->getRunnerHero();
		Rect heroRect = hero->getCollisonRectInWorldSpace();
		Rect obsRect = this->getCollisonRectInWorldSpace();
		Vec2 heroWorldPos,selfWorldPos;
		heroWorldPos.x = heroRect.getMidX();
		heroWorldPos.y = heroRect.getMidY();
		selfWorldPos.x = obsRect.getMidX();
		selfWorldPos.y = obsRect.getMidY();

		if(selfWorldPos.x < heroWorldPos.x + 30){
			this->setPosition(this->getParent()->convertToNodeSpace(heroWorldPos + Vec2(15,0)));
		}else {
			this->setPosition(this->getParent()->convertToNodeSpace(selfWorldPos - Vec2(30,(selfWorldPos.y-heroWorldPos.y)*m_iBeAbsorbed/16.0f)));
		}
	}
}

void ObstacleSprite::changeGold(Vec2 heroWorldPos)
{
	float deltaX = this->getCollisonRectInWorldSpace().getMinX() - heroWorldPos.x-20;
	if (abs(deltaX) < 15*g_fGridWidth)
	{
		UnitID type = (UnitID)m_unit.ID;
		if (type == unitID_JinBi)
		{
			this->init(const_cast<Unit *>(GoodsHelper::getHelper()->getUnitByID(unitID_JinDan)));
		}
	}
}

void ObstacleSprite::update(float fDelta)
{
	//有trigger 才会进入update

	HeroSprite* hero = RunningHelper::getHelper()->getRunnerHero();
	if (!hero || hero->getCurState() == HeroSprite::state_Dead) return;


	//触发器
	if (m_triggerId.empty())
	{
		this->unscheduleUpdate();
		return;
	}
	else 
	{
		if (m_triggerId[0] == 0)//避免0的情况
		{
			m_triggerId.erase(m_triggerId.begin());
		}
		else
		{

			Rect heroRect = hero->getCollisonRectInWorldSpace();
			Rect obsRect = this->getCollisonRectInWorldSpace();
			Vec2 heroWorldPos,selfWorldPos;
			heroWorldPos.x = heroRect.getMidX();
			heroWorldPos.y = heroRect.getMidY();
			selfWorldPos.x = obsRect.getMidX();
			selfWorldPos.y = obsRect.getMidY();


			const Trigger* trig = UnitTriggerHelper::getOneTrigger(m_triggerId[0]);
			if (trig  && (action_BossMake != trig->actionID) && (trig->distance*g_fGridWidth >= selfWorldPos.x - heroWorldPos.x)){
				UnitTriggerHelper::runTriggerActionById(this, m_triggerId[0]);
				m_triggerId.erase(m_triggerId.begin());
			}
			else if (trig && (action_BossMake == trig->actionID))
			{
				BossSprite* pSprite = RunningHelper::getHelper()->getGameUnitLayer()->getBoss();
				if (pSprite)
				{
					Point pos = pSprite->getParent()->convertToWorldSpace(pSprite->getPosition());
					if (pos.x - selfWorldPos.x >= trig->distance*g_fGridWidth)
					{
						UnitTriggerHelper::runTriggerActionById(this, m_triggerId[0]);
						m_triggerId.erase(m_triggerId.begin());
					}
				}
			}
			else if (trig && selfWorldPos.x < 0){
				m_triggerId.erase(m_triggerId.begin());
			}
		}
	}

}
void ObstacleSprite::noLandCollision(ObstacleSprite *sprite)
{
	float fallHeight = -12.0f;
	float scale = RunningHelper::getHelper()->getCameraScale();
	if (sprite)
	{

		Rect gunShiRect = Rect::ZERO;
		Rect obstacleRect = Rect::ZERO;
		{
			Rect delta = Rect::ZERO;
			if (this->getSelfUnit()->rect.size() > 0)
			{
				delta = this->getSelfUnit()->rect[0];
			}
			float scaleX = this->getScaleX();
			float scaleY = this->getScaleY();
			Vec2 worldPos = this->getParent()->convertToWorldSpace(this->getPosition());
			delta.size.width = this->getContentSize().width;
			//worldPos.x += delta.getMinX();
			worldPos.y += delta.getMinY();
			Rect ret = Rect(worldPos.x-delta.size.width/2*scaleX*scale, worldPos.y-delta.size.height/2*scaleY*scale, delta.size.width*scaleX*scale, delta.size.height*scaleY*scale);
			gunShiRect = ret;
		}
		{
			Rect delta = Rect::ZERO;
			if (sprite->getSelfUnit()->rect.size() > 0)
			{
				delta = sprite->getSelfUnit()->rect[0];
			}
			float scaleX = sprite->getScaleX();
			float scaleY = sprite->getScaleY();
			Vec2 worldPos = sprite->getParent()->convertToWorldSpace(sprite->getPosition());
			delta.size.width = sprite->getContentSize().width;
			//worldPos.x += delta.getMinX();
			worldPos.y += delta.getMinY();
			Rect ret = Rect(worldPos.x-delta.size.width/2*scaleX*scale, worldPos.y-delta.size.height/2*scaleY*scale, delta.size.width*scaleX*scale, delta.size.height*scaleY*scale);
			obstacleRect = ret;
		}

		float x = gunShiRect.getMidX() - obstacleRect.getMinX();
		float r = gunShiRect.size.width / 2;
		float v = r - sqrt(r * r - x * x);
		this->setPositionY(this->getPositionY() - v);

		//CCLOG();
	}
	else
	{
		this->runAction(FX::ActionJumpDown::create(sqrt(2*abs(fallHeight)/10.f)/21.5f, fallHeight));
	}
	
	//action->setTag(Tag_JumpFallAction);
	
}

void ObstacleSprite::tick_StoneScroll(float dt)
{
	HeroSprite* pHero = RunningHelper::getHelper()->getRunnerHero();
	if (! pHero) 
		return;
	if (pHero->getCurState() == HeroSprite::state_Dead)
	{
// 		this->stopActionByTag(action_GunDong);
// 		this->unschedule(schedule_selector(ObstacleSprite::tick_StoneScroll));
		return;
	}

#define OFFSET 10
	//land
	bool bOnLand = false;
	Rect stoneRect = this->getCollisonRectInWorldSpace();
	float stoneR = stoneRect.size.height/2;
	Rect landRect;
	Vector<ObstacleSprite*>* pObstacleSpriteArray = RunningHelper::getHelper()->getObstacles();
	for (Vector<ObstacleSprite*>::iterator iter = pObstacleSpriteArray->begin(); iter != pObstacleSpriteArray->end() ; iter++)
	{
		ObstacleSprite *pObstacleSprite = *iter;
		if (pObstacleSprite->getSelfUnit()->haveAttribute(Attr_TreadNoneffect) && pObstacleSprite->isVisible())
		{
			landRect = pObstacleSprite->getCollisonRectInWorldSpace();
			//石头位于陆地中上
			if (landRect.getMaxY()-OFFSET<=stoneRect.getMidY() && landRect.getMaxY()-OFFSET>=stoneRect.getMinY()
				&& stoneRect.getMidX()-stoneR*0.1<=landRect.getMaxX() && stoneRect.getMidX()+stoneR*0.4>=landRect.getMinX())
			{
				float offset = landRect.getMaxY()-stoneRect.getMinY()-OFFSET;
				this->setPositionY(this->getPositionY()+offset);
				bOnLand = true;
				break;
			}
		}
	}
	//下落
	if (!bOnLand)
	{
		this->setPositionY(this->getPositionY()-8.0f);
	}

}




void ObstacleSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	UnitSprite::draw(renderer, transform, flags);

	//debug
	if (1 == DEBUG_uGameInCollisionRect)
	{
		UnitType type = getSelfUnit()->type;
		if (type == unittype_Item || type == unittype_Land || type == unittype_Obstacle || type == unittype_Tool)
		{
			//debug
			m_customCommand.init(_globalZOrder+1);
			m_customCommand.func = CC_CALLBACK_0(ObstacleSprite::onDrawDebugCollisonRect, this, transform, flags);
			renderer->addCommand(&m_customCommand);
		}
	}

}
void ObstacleSprite::onDrawDebugCollisonRect(const Mat4 &transform, uint32_t flags)
{
	glLineWidth(2);
	DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
	Rect rect = this->getCollisonRectInWorldSpace();
	DrawPrimitives::drawRect(rect.origin, Vec2(rect.getMaxX(), rect.getMaxY()));
}