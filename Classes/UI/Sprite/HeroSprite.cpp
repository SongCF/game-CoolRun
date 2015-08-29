#include "HeroSprite.h"
#include "BossSprite.h"
#include "MsgDefine.h"
#include "FXJump.h"
#include "RunningHelper.h"
#include "GameUnitLayer.h"
#include "StateBar.h"
#include "PlayerInfoHelper.h"
#include "BagDataManager.h"
#include "ActorHelper.h"
//#include "GameDirector.h"
#include "RunningHelper.h"
#include "UnitCollisionDetection.h"
#include "AchievementHelper.h"
#include "GameInTouchLayer.h"
#include "GoodsHelper.h"
#include "UnitDefine.h"
#include "Music.h"
#include "AccountsLayer.h"
#include "StopLayer.h"
#include "VersionConfig.h"
#include "ActiveValueHelper.h"
#include "Font.h"
//debug
#include "VersionConfig.h"

//#define JUMP_Time        0.31f
//#define JUMP_Height1      (GridWidth*5.5f)
//#define JUMP_Height2      (GridWidth*3.5f)
//#define JUMP_Height3      (GridWidth*3.5f)


/*
//pref:
//人物的图片是land.png(1*1像素)，contentSize也是它，人物armature动画是放置在图片中心点的，动画的基准点是人物的脚踩在（0,0）点。
//所有整个人物是站在图片中心点上边的。
*/



#define SecondToM			1000

//计算弹簧速度应该为当前基础速度的多少倍  --->  关卡模式的基础速度 与 无尽模式的不一样，但是弹簧的速度需要恒定 7.7*2
#define BOUNCE_SPEED_MULTI(cur_base_speed) ((7.7f*2)/cur_base_speed)
//计算跳跃腾空时间，关卡模式基础速度 与 无尽模式不一样
#define JUMP_SPEED_MULTI(cur_multi_scrollWidth) (cur_multi_scrollWidth/g_fMapScrollDelta)

HeroSprite* HeroSprite::create(Unit* unit)
{
	HeroSprite* pSprite = new HeroSprite;
	if (pSprite && pSprite->init(unit)){
		pSprite->autorelease();
	}else {
		CC_SAFE_DELETE(pSprite);
		pSprite = nullptr;
	}
	return pSprite;
}

HeroSprite::HeroSprite()
	: m_curState(state_Run)
	, m_previouState(state_Run)
	, m_willChangetoState(-1)
	, m_fLandPosY(Land_Default_PosY)
	, m_fLandHeight(0)
	, m_startJumpPos(Point::ZERO)
	, m_pCurObstructSprite(nullptr)
	, m_bIsDieRockets(false)
	, m_pArmature(nullptr)
	, m_bWillSuperRun(false)
	, m_iHp(1)
	, m_iSiWangChongCiTimes(0)
	, m_iFuHuoTimes(0)
	, m_bIsSpringBounce(false)
	, m_bTreadLandRoll(false)
	, m_bArrivalTerminal(false)
{
}
HeroSprite::~HeroSprite()
{
	for (vector<EventListenerCustom*>::iterator iter = m_lListenerList.begin() ; iter != m_lListenerList.end() ; iter++)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(*iter);
	}
	m_lListenerList.clear();
}


void HeroSprite::onEnter()
{
	UnitSprite::onEnter();
}

void HeroSprite::onExit()
{
	UnitSprite::onExit();
}

bool HeroSprite::init(Unit *unit)
{
	if (!UnitSprite::init(unit)) return false;
	this->initWithFile("0none.png");//1*1透明图

	for (int i=0,count=unit->attribute.size(); i<count; ++i){
		m_globleAddAttribute.push_back(unit->attribute.at(i));
		//默认攻击力
		if (unit->attribute.at(i).attr == Attr_Power){
			m_fDefaultFightPower = unit->attribute.at(i).value;
		}
	}
	m_iHp = unit->getAttributeValue(Attr_HP);

	this->setIsShowCaiHongQiao(false);
	m_iZhongDuDaoJiShi = 0;
	this->setOpacity(0);

	this->unscheduleAllSelectors();
	this->scheduleUpdate();

	m_lListenerList.push_back(_eventDispatcher->addCustomEventListener(CustomButton_Jump, [&](EventCustom*){this->jump();}));
	m_lListenerList.push_back(_eventDispatcher->addCustomEventListener(CustomButton_Down, [&](EventCustom*){this->DownGlide();}));
//	m_lListenerList.push_back(_eventDispatcher->addCustomEventListener(CustomButton_Fight, [&](EventCustom*){this->fight();}));
	m_lListenerList.push_back(_eventDispatcher->addCustomEventListener(CustomButton_Super, [&](EventCustom* ev){this->super(*((int*)ev->getUserData()));}));


	//隔几帧后进行初始化加载，避免在切换场景的时候太卡
	Director::getInstance()->getScheduler()->schedule([=](float){
		this->addArmatureNode();
		m_pArmature->getAnimation()->play(Armature_HuXi);
	}, this, 0.1f, 0, 0.1f, false, "Preload_Hero_Armature");


	return true;
}
void HeroSprite::setPet(const Unit *pet)
{
	for (int iPet=0,count=pet->attribute.size(); iPet<count; ++iPet){
			m_globleAddAttribute.push_back(pet->attribute.at(iPet));
	}
}
void HeroSprite::setMagicGoods(const Unit* magic)
{
	for (int i=0,count=magic->attribute.size(); i<count; ++i){
		m_globleAddAttribute.push_back(magic->attribute.at(i));
	}
}
void HeroSprite::setItem(const Unit* item)
{
	for (int i=0,count=item->attribute.size(); i<count; ++i){
		m_globleAddAttribute.push_back(item->attribute.at(i));
	}
}

void HeroSprite::resetUnit(Unit *unit)
{
	m_globleAddAttribute.clear();

	m_unit = *unit;
	for (int i=0,count=m_unit.attribute.size(); i<count; ++i){
		m_globleAddAttribute.push_back(m_unit.attribute.at(i));
		//默认攻击力
		if (m_unit.attribute.at(i).attr == Attr_Power){
			m_fDefaultFightPower = m_unit.attribute.at(i).value;
		}
	}
	m_iHp = m_unit.getAttributeValue(Attr_HP);
	RunningHelper::getHelper()->getCurrentStateBar()->updateHPIconCount(m_iHp);
}

float HeroSprite::getFightPower()
{
	float ret = m_fDefaultFightPower;
	for (int i=0,count=m_globleAddAttribute.size(); i<count; ++i)
	{
		//可能会有多个  所以不要break
		if (m_globleAddAttribute.at(i).attr == Attr_AddPower)
		{
			ret += m_fDefaultFightPower * m_globleAddAttribute.at(i).value;
		}
	}

	return ret;
}

void HeroSprite::begin()
{
	startRun();

	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless){
		this->scheduleOnce(schedule_selector(HeroSprite::beginAttributeEffect), 1.f);
	}

// 	ParticleSystemQuad* par = ParticleSystemQuad::create("bianzi.plist");
// 	par->setAutoRemoveOnFinish(true);
// 	this->addChild(par,-1);
// 	Rect heroRect = this->getCollisonRectInWorldSpace();
// 	par->setPosition(Vec2(-14, heroRect.size.height/2-20));
}


void HeroSprite::addArmatureNode()
{
	pair<string, string> info =  getArmatureInfo(this->getSelfUnit()->ID);
	m_armatureFileName = info.first;
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo(m_armatureFileName);
	if (!m_pArmature)
	{
		m_pArmature = Armature::create(info.second.c_str());
		m_pArmature->setPosition(_contentSize/2);
		this->addChild(m_pArmature);
		m_pArmature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(HeroSprite::playAnimationEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
}

pair<string, string> HeroSprite::getArmatureInfo(unsigned int typeId)
{
	pair<string, string> info;

	if (typeId == unitID_Actor1)
	{
		info.first = "xiaoxiao.ExportJson";
		info.second = "xiaoxiao";
	}
	else if (typeId == unitID_Actor2)
	{
		info.first = "alading.ExportJson";
		info.second = "alading";
	}
	else 
	{
		CCASSERT(false, "error branch");
	}

	return info;
}


void HeroSprite::playAnimationEnd(Armature *armature, MovementEventType type, const std::string& movementID)
{
	if (type == COMPLETE)// || type == LOOP_COMPLETE)
	{
		if (movementID == Armature_Tiao_Guodu_1)
		{
			if (m_curState == state_Jump1){
				changeState(state_Fall1);
			}
		}
		else if (movementID == Armature_Tiao_Guodu_2)
		{
			if (m_curState == state_Jump2){
				changeState(state_Fall2);
			}
			else if (m_curState == state_Jump3){
				changeState(state_Fall3);
			}
		}
		else if (movementID == Armature_FanGun)
		{
			startRun();
		}
		else if (movementID == Armature_HuaXiangStart)
		{
			playAnimation(Armature_HuaXiangING);
			MotionStreak *streak = MotionStreak::create(1.75f, 1.0f, 50.0f, Color3B::WHITE, "fei_tiexiao4.png");
			BlendFunc blendf = {GL_SRC_ALPHA, GL_ONE};
			streak->setBlendFunc(blendf);
			RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->addChild(streak, 100, "Hero_BounceStreak");

			Rect heroRect = this->getCollisonRectInWorldSpace();
			streak->setPosition(RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->convertToNodeSpace(
				Vec2(heroRect.getMinX(), heroRect.getMidY())));

			this->schedule(SEL_SCHEDULE(&HeroSprite::showTreadBounceAnimation), 0.02f, -1, 0.02f);
		}
		else if (movementID == Armature_ShouShang)
		{
			willShock();
			ShowInvincibleAni();

			if (m_willChangetoState != -1){
				HeroState state = (HeroState)m_willChangetoState;
				m_willChangetoState = -1;
				changeState(state);
			}
			else{
				changeState(m_curState);
			}

			this->resume();
			RunningHelper::getHelper()->getGameUnitLayer()->startScroll();
			RunningHelper::getHelper()->getGameInTouchLayer()->setCanBeTouched(true);
			//GameDirector::getDirector()->hideWaiting();
		}
		else if (movementID == Armature_Congratulation)
		{
			this->runAction(Sequence::create(
				DelayTime::create(0.5f),
				CallFunc::create([](){
					EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
					EventCustom event(MSG_HeroWin);
					eventDspc->dispatchEvent(&event);
			}),
				nullptr));
		}
	}
}


//优先级：
//1. 随机道具
//2. 购买的开场道具
//3. 宠物、人物身上的属性
void HeroSprite::beginAttributeEffect(float dt)
{
	bool bHasRush = false;

	//1. 随机道具
	{
		unsigned int randomItemId = GoodsHelper::getHelper()->getRandomMarketItem();
		if (randomItemId != unitID_None)
		{
			const Unit* randomItem = GoodsHelper::getHelper()->getUnitByID(randomItemId);
			if (randomItem)
			{
				if (randomItem->haveAttribute(Attr_Rush))
				{
					bHasRush = true;
					rush(Attr_Rush, randomItem->getAttributeValue(Attr_Rush));
					RunningHelper::getHelper()->getCurrentStateBar()->startEcffect(StateBar::Tag_StartSpeed);
				}
				else 
				{
					this->setItem(randomItem);
				}
			}
			GoodsHelper::getHelper()->useRandomMarketItem();

			RunningHelper::getHelper()->getGameLog()->insert(std::make_pair("ItemBegin",String::createWithFormat("ID=%d,random",randomItemId)->getCString()));
		}
	}

	//2. 背包附加开场道具
	//开场冲刺
	if (! bHasRush)
	{
		GoodsHelper *goodsHelper = GoodsHelper::getHelper();
		const BagItem *item = goodsHelper->getOneItem(unitID_KaiChangChongCi);
		if (item && item->value > 0)
		{
			const Unit* unit = goodsHelper->getUnitByID(item->id);
			if (unit && unit->haveAttribute(Attr_Rush))
			{
				bHasRush = true;
				rush(Attr_Rush, unit->getAttributeValue(Attr_Rush));
				RunningHelper::getHelper()->getCurrentStateBar()->startEcffect(StateBar::Tag_StartSpeed);
				goodsHelper->useOneGoods(item->id);

				RunningHelper::getHelper()->getGameLog()->insert(std::make_pair("ItemBegin",String::createWithFormat("ID=%d",unit->ID)->getCString()));
			}
		}
	}

	//3. 人物属性
	Attribute temp;
	for(size_t i=0; i<m_globleAddAttribute.size(); )
	{
		temp = m_globleAddAttribute.at(i);
		if (Attr_Invincible == temp.attr){
			invincible(temp.attr, temp.value);
			m_globleAddAttribute.erase(m_globleAddAttribute.begin()+i);
		}
		else if (Attr_Protect == temp.attr){
			protect(temp.value);
			RunningHelper::getHelper()->getCurrentStateBar()->startEcffect(StateBar::Tag_StartProtect);
			m_globleAddAttribute.erase(m_globleAddAttribute.begin()+i);
		}
		else if (Attr_Magnet == temp.attr){
			magnet(temp.attr, temp.value);
			m_globleAddAttribute.erase(m_globleAddAttribute.begin()+i);
		}
		else if (Attr_ChangeToGold == temp.attr){
			changeGold(temp.value);
			m_globleAddAttribute.erase(m_globleAddAttribute.begin()+i);
		}
		else if (Attr_Rush == temp.attr && ! bHasRush){
			rush(temp.attr, temp.value);
			RunningHelper::getHelper()->getCurrentStateBar()->startEcffect(StateBar::Tag_StartSpeed);
			m_globleAddAttribute.erase(m_globleAddAttribute.begin()+i);
		}
		else {
			++i;
		}
	}

}

void HeroSprite::arrivalTerminal()
{
	GameInTouchLayer* pTouchLayer = RunningHelper::getHelper()->getGameInTouchLayer();
	if (pTouchLayer) pTouchLayer->hideMenu();
	m_bArrivalTerminal = true;
}
void HeroSprite::playCongratulationAnimate()
{
	m_pArmature->getAnimation()->play(Armature_Congratulation);
}

void HeroSprite::reborn(bool isRocket)
{
	this->stopAllActions();

	if (m_pArmature)
	{
		m_pArmature->stopAllActions();
	}

	if (m_curState == state_Dead)
	{
		//复活满血
		m_iHp = this->getSelfUnit()->getAttributeValue(Attr_HP);
		RunningHelper::getHelper()->getCurrentStateBar()->updateHPIconCount(m_iHp);

		willShock();

		m_iFuHuoTimes++;

	}

	if (isRocket)
	{
		//rockets(Attr_Rush, 100.0f);
		rush(Attr_Rush, 10.0f);

		//避免近超跑时死亡，复活后金币进度条不会再增长
		if (! RunningHelper::getHelper()->getCurrentStateBar()->getGoldValueEnable()){
			RunningHelper::getHelper()->getCurrentStateBar()->startGoldValue(false);
		}
	}
	else
	{
		changeState(state_Run);
		this->ShowInvincibleAni();
	}

	Rect rect;
	RunningHelper::getHelper()->getGameUnitLayer()->getRecentLandRect(rect, false);
	//correct camera
	RunningHelper::getHelper()->MoveCamera(0, Land_Default_PosY - rect.getMaxY());

	//需要把设置位置放到最后，不然会出现死亡后，矫正位置的时候人物掉到路面下的情况
	RunningHelper::getHelper()->getGameUnitLayer()->getRecentLandRect(rect, false);
	this->setPosition(RunningHelper::getHelper()->getGameUnitLayer()->getHeroDefaultPosX(),
		this->getParent()->convertToNodeSpace(Vec2(0, rect.getMaxY()+this->getCollisonRectInWorldSpace().size.height/2)).y);
	
}

void HeroSprite::ShowInvincibleAni()
{
	this->setIsShowCaiHongQiao(true);
//	this->scheduleOnce(schedule_selector(HeroSprite::hiedeCaiHongQiao), 3.0f);
	if (m_pArmature)
	{
		invincible(Attr_Invincible, 2.4f);
		m_pArmature->runAction(Repeat::create(Sequence::create(FadeOut::create(0.2f), FadeIn::create(0.2f), nullptr), 6));
	}
}

void HeroSprite::hiedeCaiHongQiao(float dt)
{
	this->setIsShowCaiHongQiao(false);
}

void HeroSprite::ObstuctHero()
{
	this->obstructTheAdvance();
}
Rect HeroSprite::getCollisonRectInWorldSpace()
{
	Rect delta = Rect::ZERO;

// 	if (m_unit.type != unittype_Actor) CCASSERT(false, "error");
// 	if (m_curState == HeroSprite::state_Run) delta = m_unit.rect[0];
// 	else if (m_curState == HeroSprite::state_Jump1 || m_curState == HeroState::state_Bounce || m_curState == HeroState::state_BounceBack) delta = m_unit.rect[1];
// 	else if (m_curState == HeroSprite::state_Jump2 || m_curState == HeroSprite::state_Jump3) delta = m_unit.rect[2];
// 	else if (m_curState == HeroSprite::state_Fall1 || m_curState == HeroSprite::state_Fall2 || m_curState == HeroSprite::state_Fall3) delta = m_unit.rect[3];
// 	else if (m_curState == HeroSprite::state_DownGlide) delta = m_unit.rect[4];
// 	else if (m_curState == HeroSprite::state_Super || m_curState == state_ChongCi) delta = m_unit.rect[5];
// 	else if (m_curState == HeroState::state_Dead) return delta;
// 	else CCASSERT(false, "error branch");

	switch (m_curState)
	{
	case HeroSprite::state_Run:
		delta = m_unit.rect[0];
		break;
	case HeroSprite::state_Jump1:
	case HeroSprite::state_Bounce:
	case HeroSprite::state_BounceBack:
		delta = m_unit.rect[1];
		break;
	case HeroSprite::state_Jump2:
	case HeroSprite::state_Jump3:
		delta = m_unit.rect[2];
		break;
	case HeroSprite::state_Fall1:
	case HeroSprite::state_Fall2:
	case HeroSprite::state_Fall3:
		delta = m_unit.rect[3];
		break;
	case HeroSprite::state_DownGlide:
		delta = m_unit.rect[4];
		break;
	case HeroSprite::state_ChongCi:
	case HeroSprite::state_Super:
		delta = m_unit.rect[5];
		break;
	case HeroSprite::state_Dead:
		//return delta; //为了看死亡时的矩形
		if (m_previouState == HeroSprite::state_Run) delta = m_unit.rect[0];
		else if (m_previouState == HeroSprite::state_Jump1 || m_previouState == HeroState::state_Bounce || m_previouState == HeroState::state_BounceBack) delta = m_unit.rect[1];
		else if (m_previouState == HeroSprite::state_Jump2 || m_previouState == HeroSprite::state_Jump3) delta = m_unit.rect[2];
		else if (m_previouState == HeroSprite::state_Fall1 || m_previouState == HeroSprite::state_Fall2 || m_previouState == HeroSprite::state_Fall3) delta = m_unit.rect[3];
		else if (m_previouState == HeroSprite::state_DownGlide) delta = m_unit.rect[4];
		else if (m_previouState == HeroSprite::state_Super || m_previouState == state_ChongCi) delta = m_unit.rect[5];
		break;
	default:
		CCASSERT(false, "error branch");
		break;
	}

	Rect ret;
	Vec2 worldPos = this->getParent()->convertToWorldSpace(this->getPosition());
	float scaleX = this->getScaleX() * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
	float scaleY = this->getScaleY() * RunningHelper::getHelper()->getGameUnitLayer()->getScaleY();
// 	if (delta.equals(Rect::ZERO))
// 	{
// 		CCASSERT(false, "error");
// 		Size size = this->getContentSize();
// 		size.setSize(size.width*scaleX, size.height*scaleY);
// 		ret = Rect(worldPos.x-size.width/2, worldPos.y-size.height/2, size.width, size.height);
// 	}else
	{
		worldPos.x += delta.origin.x;
		worldPos.y += delta.origin.y;
		ret = Rect(worldPos.x-delta.size.width/2*scaleX, worldPos.y-delta.size.height/2*scaleY, delta.size.width*scaleX, delta.size.height*scaleY);
	}
	return ret;
}

Rect HeroSprite::getRectInObsContainerSpace()
{
	Rect delta;
	if (m_curState == HeroSprite::state_Run) delta = m_unit.rect[0];
	else if (m_curState == HeroSprite::state_Jump1 || m_curState == HeroState::state_Bounce || m_curState == HeroState::state_BounceBack) delta = m_unit.rect[1];
	else if (m_curState == HeroSprite::state_Jump2 || m_curState == HeroSprite::state_Jump3) delta = m_unit.rect[2];
	else if (m_curState == HeroSprite::state_Fall1 || m_curState == HeroSprite::state_Fall2 || m_curState == HeroSprite::state_Fall3) delta = m_unit.rect[3];
	else if (m_curState == HeroSprite::state_DownGlide) delta = m_unit.rect[4];
	else if (m_curState == HeroSprite::state_Super || m_curState == state_ChongCi) delta = m_unit.rect[5];
	else if (m_curState == HeroState::state_Dead) return delta;
	else CCASSERT(false, "error branch");

	Vec2 pos = this->getParent()->convertToWorldSpace(_position);
	pos = RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->convertToNodeSpace(pos);
	// 	float scalX=_scaleX * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
	// 	float scalY=_scaleY * RunningHelper::getHelper()->getGameUnitLayer()->getScaleY();
	float w = delta.size.width * _scaleX;// * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX(); 
	float h = delta.size.height * _scaleY;// * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
	pos.x += delta.origin.x * _scaleX;// * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
	pos.y += delta.origin.y * _scaleY;// * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
	return Rect(pos.x - w/2, pos.y - h/2, w, h);
}

//call back
void HeroSprite::onCollision(UnitSprite* pObsSprite, enumCollisionPos board/*=collideLEFT*/)
{
	CCASSERT(pObsSprite != nullptr, "");

	const Unit *unit = pObsSprite->getSelfUnit();
	UnitID obsId = (UnitID)unit->ID;

	const vector<Attribute>* attrVec = pObsSprite->getSelfAttribute();
	string temp;
	//有的属性 一但起作用 就会break；
	for (int i=0,count=attrVec->size(); i<count; ++i)
	{
		temp = attrVec->at(i).attr;

		//1. 从上往下的。
		if(temp == Attr_TreadAddScore)				//踩踏加分
		{
			if (board == collideDOWN)
			{
				this->treadBounce(4 * g_fGridWidth);
				this->addScore(attrVec->at(i).value, pObsSprite);
				break;
			}
		}
// 		else if(temp == Attr_TreadApearItem)		//踩踏出道具
// 		{
// 			CCASSERT(false, "现在没有这种东西");
// 		}
		else if(temp == Attr_TreadDisapear)		//踩踏消失
		{
			if (board == collideDOWN)
			{
				this->treadBounce(4 * g_fGridWidth);
				break;
			}
		}
		else if (temp == Attr_TreadBounce)
		{
			if (board == collideDOWN)
			{
				this->treadBounce(attrVec->at(i).value * g_fGridWidth, pObsSprite);
				break;
			}
		}
		else if (temp == Attr_BounceFly)
		{
			if (board == collideDOWN)
			{
				Rect rect = pObsSprite->getCollisonRectInWorldSpace();
				this->BounceFly(attrVec->at(i).value * g_fGridWidth, Vec2(rect.getMidX(), rect.getMaxY()+50), pObsSprite);
				break;
			}
		}
		else if(temp == Attr_TreadNoneffect)		//踩踏在路面上者木桩上
		{
			if (board == collideDOWN)
			{
				this->treadLand(pObsSprite);
				break;
			}
		}
		else if(temp == Attr_TreadDamage)
		{
			if (board == collideDOWN)
			{
				this->injured(attrVec->at(i).value);
				break;
			}
		}


		//2. 从左往右的。
		else if(temp == Attr_Obstruct)		
		{
			if (board == collideRIGHT)
			{
				m_pCurObstructSprite = pObsSprite;
				this->obstructTheAdvance();
				break;
			}
			else if (board == collideUP)
			{
				//下滑时碰到悬浮物的下边，则继续保持下滑状态（目前还没自动恢复run的功能）
				if ((m_previouState == state_DownGlide && m_curState == state_Run)
					&& (obsId == unitID_XuanFuZhangAi || obsId == unitID_XuanFuZhangAi_Down_Before || obsId == unitID_XuanFuZhangAi_Down_After || obsId == unitID_XuanFuZhangAi_Up_Before || obsId == unitID_XuanFuZhangAi_Up_After))
				{
					changeState(state_DownGlide);
					break;
				}
				else if (obsId != unitID_MuZhuang_Ai && obsId != unitID_MuZhuang_Gao 
					&& (m_curState != state_Super && this->getWillSuperRun() && m_curState != state_ChongCi && m_curState != state_Bounce && m_curState != state_BounceBack))
				{
					changeState(state_Fall1);
					break;
				}
			}
		}
		else if (temp == Attr_LeftDamage)
		{
			if (board == collideRIGHT)
			{
				this->injured(attrVec->at(i).value);
				break;
			}
		}

		//3. 从右往左 ---  无
		//4. 从下往上
		else if (temp == Attr_TopBumpAddScore)
		{
			if (board == collideUP)
			{
				topBump();
				this->addScore(attrVec->at(i).value, pObsSprite);
				break;
			}
		}
// 		else if (temp == Attr_TopBumpApearItem)
// 		{
// 			CCASSERT(false, "现在没有这种东西");
// 		}
		else if (temp == Attr_TopBumpDisapear)
		{
			if (board == collideUP)
			{
				topBump();
				break;
			}
		}
		else if (temp == Attr_TopBumpBounce)
		{
			if (board == collideUP)
			{
				topBump();
				break;
			}
		}
		else if (temp == Attr_TopBumpDamage)
		{
			if (board == collideUP)
			{
				this->injured(attrVec->at(i).value);
				break;
			}
		}

		//5. 全方位的
		else if(temp == Attr_ScoreAdd)			
		{
			this->addScore(attrVec->at(i).value, pObsSprite);
		}
		else if(temp == Attr_Invincible)			
		{
			this->invincible(temp, attrVec->at(i).value);
		}
		else if(temp == Attr_Protect)
		{
			this->protect(attrVec->at(i).value);
		}else if(temp == Attr_Magnet)
		{
			this->magnet(temp, attrVec->at(i).value);
		}
		else if(temp == Attr_ChangeToGold)
		{
			this->showChongJiBoAnimation(false);
			this->changeGold(attrVec->at(i).value);
		}
		else if(temp == Attr_Rush)
		{
			this->rush(temp, attrVec->at(i).value);
		}
		else if (temp == Attr_SpeedUp)
		{
			this->speedUp(temp, attrVec->at(i).value);
		}
		else if (temp == Attr_Beat)
		{
			beatBoss(pObsSprite, attrVec->at(i).value);
		}
		else if (temp == Attr_ElectricSwitch)
		{

		}
		else if (temp == Attr_Poison)
		{
			this->addZhongDuState(temp, attrVec->at(i).value);
		}
		else if (temp == Attr_Detoxify)//解药
		{
			this->removeZhongDuState();
		}
		else if (temp == Attr_PushbackFog)//灯笼 --- 驱散迷雾
		{
			RunningHelper::getHelper()->getGameUnitLayer()->addMaskScale();
		}



		//6. 特殊点
		else if (temp == Attr_DownToUp)
		{
			if (!RunningHelper::getHelper()->getGameInTouchLayer()->getIsTouchingLeft())
			{
				resetRun();
				RunningHelper::getHelper()->getGameInTouchLayer()->removecheckIsTouchingLeft();
			}
			break;
		}
		else if(temp == Attr_GameEndPos)
		{
			RunningHelper *pHelper = RunningHelper::getHelper();
			if (pHelper->getCurRunningMode() == RunningMode::tollgate)
				this->arrivalTerminal();
			else if (pHelper->getCurRunningMode() == RunningMode::teach)
				this->arrivalTerminal();
			else if (pHelper->getCurrentScenePlayMode() == mode_Fight)
				pHelper->getGameUnitLayer()->bossLeave();
			else 
				CCASSERT(false, "");
			break;
		}
		else if(temp == Attr_TeachJump)
		{
			RunningHelper::getHelper()->teach(temp);
			break;
		}
		else if(temp == Attr_TeachDoubJump)
		{
			RunningHelper::getHelper()->teach(temp);
			break;
		}
		else if(temp == Attr_TeachDown)
		{
			RunningHelper::getHelper()->teach(temp);
			break;
		}
		else if(temp == Attr_TeachUp)
		{
			RunningHelper::getHelper()->teach(temp);
			break;
		}
		else if (temp == Attr_RestoreOriginPos)
		{
			RunningHelper::getHelper()->getGameInTouchLayer()->hideMenu();
			changeState(state_Run);
			break;
		}
		else if (temp == Attr_BossState)
		{
			BossSprite* pBoss = RunningHelper::getHelper()->getGameUnitLayer()->getBoss();
			if (pBoss) pBoss->changeState(attrVec->at(i).value);
			break;
		}
		else if (temp == Attr_BossCome)
		{
			RunningHelper::getHelper()->getGameUnitLayer()->bossComeIn(attrVec->at(i).value);
			break;
		}



		//属于 人物、宠物等特有的。
/*		else if(temp == Attr_FiveTimesScore)	
		{
			CCASSERT(false, "没有这种物品");
		}
		else if(temp == Attr_TreadDoubleScore)
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_ThreeJump)			
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_Reborn)			
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_HP)			
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_Power)			
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_AddMagnet)			
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_AddInvincible)		
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_AddProtect)			
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_AddSpeedUp)			
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_AddChangeGold)		
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_AddMultScore)
		{
			CCASSERT(false, "不应该出现这种道具");
		}
		else if(temp == Attr_AddMultGold)
		{
			CCASSERT(false, "不应该出现这种道具");
		}*/
		else 
		{
			CCLOG("%s ---> not surpport attribute = %s", __FUNCTION__, temp.c_str());
			CCASSERT(false, "error branch");
		}
	}
}

void HeroSprite::topBump()
{
	if (m_curState == state_Jump1)
	{
		changeState(state_Fall1);
	}
	else if (m_curState == state_Jump2)
	{
		changeState(state_Fall2);
	}
	else if (m_curState == state_Jump3)
	{
		changeState(state_Fall3);
	}
	else if (m_curState == state_Fall1 || m_curState == state_Fall2 || m_curState == state_Fall3)
	{
		// no thing
	}
	else 
	{
		CCASSERT(false, "error collision detection");
	}
}
void HeroSprite::noLandCollision()
{
	if (m_curState == state_Run || m_curState == state_DownGlide)
	{
		if (getCurPlayAnimationName() != Armature_Xialuo)
		{
			changeState(state_Fall1);
		}
	}
	else if (m_bFallNow)
	{
		Rect rect;
		std::pair<bool,Sprite*> landInfo = RunningHelper::getHelper()->getGameUnitLayer()->getRecentLandRect(rect, true);
		Rect heroRect = this->getCollisonRectInWorldSpace();
		if (landInfo.second && landInfo.second->isVisible() 
			&& m_startFallPos.y > rect.getMaxY() && heroRect.getMinY() < rect.getMinY())
		{
			this->setPositionY(this->getParent()->convertToNodeSpace(Vec2(0,rect.getMaxY()-OFFSET_Land_Hero+heroRect.size.height/2)).y);
			treadLand(dynamic_cast<UnitSprite*>(landInfo.second));
			return;
		}
	}
}
void HeroSprite::update(float fDelta)
{
//	CCLOG("%s ---> curState = %d", __FUNCTION__, m_curState);
	if (m_curState == state_Dead) return;


	//0. 矫正camera
	if (m_curState == state_ChongCi)
		correctChongCiHero(0);
	else if (m_curState == state_Super)
		correctChongCiHero(150);
	else 
		correctCamera();

	Rect heroRect = this->getCollisonRectInWorldSpace();

	//1. 死亡检测
	if (this->getPositionY() < RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->getPositionY())
	{
		if (!this->changeHp(Type_Minus, 1))
		{
			this->reborn(false);
		}
	}
	if (m_iHp <= 0)	this->die();


	//属性
	updateAttribute(fDelta);

	//阻挡
	//得到当前阻挡物，没有则为nullptr
	if (m_pCurObstructSprite)
	{
		if (m_pCurObstructSprite->getCollisonRectInWorldSpace().getMaxX() + 50 < heroRect.getMinX())
		{
			this->willResetX();
			m_pCurObstructSprite = nullptr;
		}
	}
}

void HeroSprite::correctCamera()
{
	Rect heroRect = this->getCollisonRectInWorldSpace();
	float heroYoffset = heroRect.size.height/2;
//	float heroWorldPosY = this->getParent()->convertToWorldSpace(this->getPosition()).y;
	float heroWorldPosY = heroRect.getMidY();
	Rect curLandWorldRect;
	std::pair<bool,Sprite*> landInfo = RunningHelper::getHelper()->getGameUnitLayer()->getRecentLandRect(curLandWorldRect, true);
	float delta = 0;
	float curScale = RunningHelper::getHelper()->getCameraScale();
	float multiSpeed = JUMP_SPEED_MULTI(RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth());//RunningHelper::getHelper()->getRunnerMultitypeSpeed();

	//1. 弹跳状态
	if (m_curState == state_Bounce)
	{
		if (heroWorldPosY > heroYoffset + Land_Default_PosY+g_fHeroJump1Height+g_fHeroJump2Height){
			delta = heroWorldPosY - (heroYoffset + Land_Default_PosY+g_fHeroJump1Height+g_fHeroJump2Height);
			RunningHelper::getHelper()->MoveCamera(0, -sqrt(abs(delta)/1) * multiSpeed);
		}
	}
	//2. 下落状态
	else if (state_BounceBack == m_curState)
	{
		if (heroWorldPosY < mWinSize.height*0.7f && curLandWorldRect.getMaxY() < 8*g_fGridWidth)
		{
			delta = min(mWinSize.height*0.7f-heroWorldPosY, 8*g_fGridWidth-curLandWorldRect.getMaxY());
			RunningHelper::getHelper()->MoveCamera(0, sqrt(abs(delta)/1) * multiSpeed);
		}
	}
	//3. 路面上跑的状态  矫正路面到默认位置
	else 
		if (m_curState == state_Fall1 || m_curState == state_Fall2 || m_curState == state_Fall3 || m_curState == state_Run || m_curState == state_ChongCi)
	{
		if (heroWorldPosY < Land_Default_PosY+heroYoffset-g_fGridWidth/2)
		{
			delta = heroWorldPosY - Land_Default_PosY - heroYoffset;
			RunningHelper::getHelper()->MoveCamera(0, sqrt(abs(delta))*1.2f * multiSpeed);
		}
		else if (heroWorldPosY > Land_Default_PosY+heroYoffset+g_fGridWidth/2 && curLandWorldRect.getMidY() > Land_Default_PosY && heroWorldPosY > curLandWorldRect.getMaxY())
		{
			delta = curLandWorldRect.getMidY() - Land_Default_PosY;
			RunningHelper::getHelper()->MoveCamera(0, -sqrt(abs(delta)) * multiSpeed);
		}
	}
}
void HeroSprite::correctChongCiHero(float dt)
{
	float heroWorldPosY = this->getParent()->convertToWorldSpace(this->getPosition()).y;
	Rect curLandWorldRect;
	std::pair<bool,Sprite*> landInfo = RunningHelper::getHelper()->getGameUnitLayer()->getRecentLandRect(curLandWorldRect, false);
	if (! landInfo.first) return;
	float delta = 3.f;

	//1. move camera
	if (curLandWorldRect.getMidY() < Land_Default_PosY-delta)
	{
		RunningHelper::getHelper()->MoveCamera(0, delta);
	}
	else if (curLandWorldRect.getMidY() > Land_Default_PosY)
	{
		RunningHelper::getHelper()->MoveCamera(0, -delta);
	}
	//2. move hero
	if (dt > 10) //super
	{
		if (heroWorldPosY < curLandWorldRect.getMaxY()+dt-6)
		{
			this->setPositionY(this->getPositionY()+0.8f*sqrt(curLandWorldRect.getMaxY()+dt-heroWorldPosY));
		}
		else if (heroWorldPosY > curLandWorldRect.getMaxY()+dt)
		{
			this->setPositionY(this->getPositionY()-6);
		}
	}
	else // rush
	{
		if (heroWorldPosY < curLandWorldRect.getMidY()+14*g_fGridWidth-delta)
		{
			this->setPositionY(this->getPositionY()+delta);
		}
		else if (heroWorldPosY > curLandWorldRect.getMidY()+14*g_fGridWidth)
		{
			this->setPositionY(this->getPositionY()-delta);
		}
	}
}

void HeroSprite::chongCiUp(UnitSprite* pObstacleSprite)
{
// 	return;  //by scf  ---> 新的冲刺时骑鸟，不是在陆地上自动上下了。
// 
// 	if (this->getActionByTag(Tag_ChongCiUpAction))
// 	{
// 		return;
// 	}
// 	this->stopActionByTag(Tag_ChongCiUpAction);
// 	//*this->getParent()->getScale() 是为了防止还没有缩放到正常比例时会出现在路面中间冲刺的现象
// 	m_fLandPosY = pObstacleSprite->getCollisonRectInWorldSpace().getMaxY()-10;//pObstacleSprite->getParent()->convertToNodeSpace(Vec2(0, pObstacleSprite->getCollisonRectInWorldSpace().getMaxY())).y - 10;
// 	m_fLandHeight = pObstacleSprite->getCollisonRectInWorldSpace().size.height/2;
// 	Sequence* pAction = Sequence::create(
// 	MoveTo::create(0.1*(2-RunningHelper::getHelper()->getRunnerMultitypeSpeed()), Vec2(this->getPositionX(), (m_fLandPosY + Y_offset_Hero + getHeroYOffset())*this->getParent()->getScale())),
// //		CallFunc::create(std::bind(&HeroSprite::chongCiUpDone,this)),
// 		nullptr);
// 	pAction->setTag(Tag_ChongCiUpAction);
// // 	if (!m_bWillSuperRun)
// // 	{
// // 		playAnimation(Armature_Tiao_1);
// // 	}
// 	this->runAction(pAction);
}

void HeroSprite::chongCiUpDone()
{
	if (!m_bWillSuperRun)
	{
		playAnimation(Armature_ChongCi);
	}
//	float time = 0.35f;
//	RunningHelper::getHelper()->MoveCamera(time, -(m_fLandPosY-Land_Default_PosY)-RunningHelper::getHelper()->getGameUnitLayer()->getPositionY());
}

void HeroSprite::chongCiFall(UnitSprite* pObstacleSprite)
{
// 	return;  //by scf  ---> 新的冲刺时骑鸟，不是在陆地上自动上下了。
// 
// 	if (this->getActionByTag(Tag_ChongCiFallAction))
// 	{
// 		return;
// 	}
// 	m_fLandPosY = pObstacleSprite->getCollisonRectInWorldSpace().getMaxY()-10;//pObstacleSprite->getParent()->convertToNodeSpace(Vec2(0, pObstacleSprite->getCollisonRectInWorldSpace().getMaxY())).y - 10;
// 	m_fLandHeight = pObstacleSprite->getCollisonRectInWorldSpace().size.height/2;
// 	Sequence* pAction = Sequence::create(
// 	MoveTo::create(0.1*(2-RunningHelper::getHelper()->getRunnerMultitypeSpeed()), Vec2(this->getPositionX(), m_fLandPosY + Y_offset_Hero + getHeroYOffset())),
// //		CallFunc::create(std::bind(&HeroSprite::chongCiFallDone,this)),
// 		nullptr);
// 	pAction->setTag(Tag_ChongCiFallAction);
// // 	if (!m_bWillSuperRun)
// // 	{
// // 		playAnimation(Armature_Xialuo);
// // 	}
// 	this->runAction(pAction);
}

void HeroSprite::chongCiFallDone()
{
	if (!m_bWillSuperRun)
	{
		playAnimation(Armature_ChongCi);
	}
//	float time = 0.35f;
//	RunningHelper::getHelper()->MoveCamera(time, -(m_fLandPosY-Land_Default_PosY)-RunningHelper::getHelper()->getGameUnitLayer()->getPositionY());
}

void HeroSprite::setWillChangeToState(HeroState state)
{
	m_willChangetoState = state;
}
void HeroSprite::setWillSuperRun(bool b)
{
	m_bWillSuperRun = b;
	this->setIsShowCaiHongQiao(b);

	//将要超跑时就不要计算meter，    如果common地图结束时踩的弹簧，在空中吃满金币到落地这个过程 willsuper is true，but calculate meter，超跑结束后进入common会导致地图皮肤解析错误。  
	if (b) RunningHelper::getHelper()->enableCalculateMeter(false);
}
void HeroSprite::changeState(HeroState state)
{
	if (Armature_ShouShang == m_pArmature->getAnimation()->getCurrentMovementID())
	{
		setWillChangeToState(state);
		return;
	}

// 	CCLOG("%s ---> old state = %d, new state = %d", __FUNCTION__, m_curState, state);

	//1.修改状态前的处理
	this->stopAllActions();
	m_pArmature->getAnimation()->stop();
	//1.
	if (m_curState == state_Bounce || m_curState == state_BounceBack || m_curState == state_Dead){
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(HeroSprite::showTreadBounceAnimation), this);
		RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->removeChildByName("Hero_BounceStreak");
		if (RunningHelper::getHelper()->getCameraScale() < 1)
		{
			RunningHelper::getHelper()->scaleCamera(1.5f, 1);
		}
	}

	//2.
	this->setCurState(state);

	switch (m_curState)
	{
	case HeroSprite::state_Run:
		{
			startRun();
			this->setPositionY(this->getParent()->convertToNodeSpace(Vec2(0,m_fLandPosY+this->getCollisonRectInWorldSpace().size.height/2)).y);
		}
		break;
	case HeroSprite::state_DownGlide:
		{
			playAnimation(Armature_hua);
			this->setPositionY(this->getParent()->convertToNodeSpace(Vec2(0, m_fLandPosY+this->getCollisonRectInWorldSpace().size.height/2)).y);
		}
		break;
	case HeroSprite::state_Jump1:
		{
			m_startJumpPos = this->getPosition();
			stopRun();
			playAnimation(Armature_Tiao_1);
			Sequence* action = Sequence::create(
				//FX::ActionJumpUp::create(g_fJump1NeedTime / RunningHelper::getHelper()->getRunnerMultitypeSpeed(), g_fHeroJump1Height),
				FX::ActionJumpUp::create(g_fJump1NeedTime / JUMP_SPEED_MULTI(RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth()), g_fHeroJump1Height),
				CallFunc::create(std::bind(&HeroSprite::jumpHighestPoint,this)), 
				nullptr);
			this->runAction(action);

			FX::Music::getInstance()->playEffectMusic("r_jump");
		}
		break;
	case HeroSprite::state_Jump2:
	case HeroState::state_Jump3:
		{
			playAnimation(Armature_Tiao_2);
			this->setRotation(0);
			Sequence* action = Sequence::create(
				//Spawn::create(FX::ActionJumpUp::create((g_fJump1NeedTime-0.1f) / RunningHelper::getHelper()->getRunnerMultitypeSpeed(), g_fHeroJump2Height), nullptr),
				Spawn::create(FX::ActionJumpUp::create((g_fJump1NeedTime-0.1f) / JUMP_SPEED_MULTI(RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth()), g_fHeroJump2Height), nullptr),
				CallFunc::create(std::bind(&HeroSprite::jumpHighestPoint,this)), 
				nullptr);
			action->setTag(Tag_JumpAction_2);
			this->runAction(action);

			//music
			UnitID typeId = (UnitID)this->getSelfUnit()->ID;
			if (typeId == unitID_Actor1)
				FX::Music::getInstance()->playEffectMusic("r_second_jump_big_girl");
			else if (typeId == unitID_Actor2)
				FX::Music::getInstance()->playEffectMusic("r_second_jump_boy");
			else 
				CCASSERT(false, "error branch");
		}
		break;
	case HeroSprite::state_Fall1:
	case HeroSprite::state_Fall2:
	case HeroSprite::state_Fall3:
		{
			fall();
		}
		break;
	case HeroSprite::state_ChongCi:
		{
			playAnimation(Armature_ZuoQi);
			this->setIsShowCaiHongQiao(true);
			removeChongCiAnimation(); // 如果有就会移除加速的黄光。
		}
		break;
	case HeroSprite::state_Super:
		{
			setWillSuperRun(false);
			playAnimation(Armature_ZuoQi);
			removeChongCiAnimation(); // 如果有就会移除加速的黄光。
		}
		break;
	case state_Bounce:
		{
			playAnimation(Armature_HuaXiangStart);
		}
		break;
	case HeroSprite::state_Dead:
		{
		}
		break;
	default:
		CCASSERT(false, "error branch");
		break;
	}
}
void HeroSprite::showTreadBounceAnimation(float dt)
{
	static int scheduleNum = 0;
	scheduleNum = 0;
	MotionStreak *s = dynamic_cast<MotionStreak*>(RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->getChildByName("Hero_BounceStreak"));
	if (!s) return ;
	++scheduleNum;

	Rect heroRect = this->getCollisonRectInWorldSpace();
	Vec2 pos = Vec2(heroRect.getMinX(), heroRect.getMidY()); 
	// to maxX minY
	pos.x += 3;
	pos.y -= 2;
	if (pos.x > heroRect.getMaxX()) pos.x = heroRect.getMaxX();
	if (pos.y < heroRect.getMinY()) pos.y = heroRect.getMinY();
	s->setPosition(RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->convertToNodeSpace(pos));
}

void HeroSprite::showChongCiAnimation()
{
	removeChongCiAnimation();
	this->schedule(schedule_selector(HeroSprite::updateChongCiAnimation), 0.01f, kRepeatForever, 0.0f);
}

void HeroSprite::updateChongCiAnimation(float dt)
{
	Sprite* s = Sprite::create("tiexiao_chongci_01.png");
	Layer* obstacleContainer = RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer();
	obstacleContainer->addChild(s, 100);
	Point point = obstacleContainer->convertToNodeSpace(this->getParent()->convertToWorldSpace(this->getPosition()));
	s->setPosition(Vec2(point.x - 30, point.y + 10));
//	s->setColor(Color3B::RED);
	s->runAction(Sequence::create(
		Spawn::create(FadeTo::create(0.35f, 0), nullptr),
		RemoveSelf::create(),
		nullptr));
}

void HeroSprite::removeChongCiAnimation()
{
//	Director::getInstance()->getScheduler()->unschedule("heromotionstreak", this);
	this->unschedule(schedule_selector(HeroSprite::updateChongCiAnimation));
}

void HeroSprite::willResetX()
{
	this->stopResetX();
	this->schedule(schedule_selector(HeroSprite::resetX), 0.01f);
}

void HeroSprite::stopResetX()
{
	this->unschedule(schedule_selector(HeroSprite::resetX));
}

void HeroSprite::resetX(float dt)
{
	if (m_pCurObstructSprite)
	{
		return;
	}
	if (this->getPositionX() <= GameUnitLayer::getHeroDefaultPosX())
	{
		this->setPositionX(this->getPositionX()+3.0f);
	}
	else
	{
		stopResetX();
		this->setPositionX( GameUnitLayer::getHeroDefaultPosX());
	}
}

//Action
void HeroSprite::resumeAction()
{
	changeState(m_curState);
}

void HeroSprite::resetRun()
{
	if (m_curState == state_DownGlide)
	{
		changeState(state_Run);
	}
}

void HeroSprite::startRun()
{
	this->setRotation(0);
// 	if (getCurPlayAnimationName() == Armature_Pao)
// 	{
// 		return;
// 	}

	if (this->isHaveAttrOnGoing(Attr_SpeedUp))
		playAnimation(Armature_ChongCi);
	else 
		playAnimation(Armature_Pao);
}
void HeroSprite::stopRun()
{
	this->setRotation(0);
	this->stopActionByTag(Tag_RunAction);
}
void HeroSprite::injured(int subHp)
{
	//碰到电闸就死，不管有没有其他状态
	bool bWillDead = false;
	if (subHp > 1)
	{
		bWillDead = true;
	}

	bool protectAttr = isHaveAttrOnGoing(Attr_Protect);
	bool invincibleAttr = isHaveAttrOnGoing(Attr_Invincible);
	//受伤
	if ((protectAttr || invincibleAttr) && !bWillDead)
	{
		if (!invincibleAttr)
		{
			endAttribute(Attr_Protect);
			RunningHelper::getHelper()->getCurrentStateBar()->setProtectMenuEnable(true);
			Node* p = this->getChildByTag(Tag_Protect);
			if (p)
			{
				willShock();
				p->removeFromParent();
			}
		}
	}
	else
	{
		this->changeHp(Type_Minus, subHp);

		if (m_iHp > 0)
		{
			StateBar *pStateBar = RunningHelper::getHelper()->getCurrentStateBar();
			if (pStateBar)
			{
				Sprite *pHongKuang = Sprite::create("hongjing.png");
				pStateBar->getParent()->addChild(pHongKuang, pStateBar->getLocalZOrder()-1, "EFFECT_ShouShang_HongKuang");
				pHongKuang->setPosition(mWinSize/2);
				pHongKuang->setScaleX(mWinSize.width/pHongKuang->getContentSize().width);
				pHongKuang->setScaleY(mWinSize.height/pHongKuang->getContentSize().height);
				pHongKuang->runAction(Sequence::create(
					Repeat::create(Sequence::create(FadeOut::create(0.125f),FadeIn::create(0.125f),nullptr), 2),
					RemoveSelf::create(),
					nullptr));
			}

			playAnimation(Armature_ShouShang);
			this->pause();
			RunningHelper::getHelper()->getGameUnitLayer()->stopScroll();
			RunningHelper::getHelper()->getGameInTouchLayer()->setCanBeTouched(false);
			//GameDirector::getDirector()->showWaiting(false);
		}
	}
}
void HeroSprite::die()
{
	if (m_curState == state_Dead)
	{
		return;
	}
	
	changeState(state_Dead);
	RunningHelper::getHelper()->gameOver();
	this->stopResetX();

	//死亡移除中毒属性
	endAttribute(Attr_Poison);
	//死亡移除加速属性，不然会导致冲刺状态下死亡复活不能点击上跳按钮
	endAttribute(Attr_Rush);
	removeChongCiAnimation();
	removeZhongDuState();
//	this->setWillSuperRun(false);
//	this->setWillBeChongCi(false);
	this->setIsShowCaiHongQiao(false);

	RunningHelper::getHelper()->getGameInTouchLayer()->setCanBeTouched(true);

	skill_screenShake();
	//死亡人物动画
	Rect curLandWorldRect;
	std::pair<bool,Sprite*> landInfo = RunningHelper::getHelper()->getGameUnitLayer()->getRecentLandRect(curLandWorldRect, true);
	float delta = this->getCollisonRectInWorldSpace().getMinY() - curLandWorldRect.getMaxY();
	if (landInfo.first && abs(delta) < 24)
	{
		this->setPositionY(this->getParent()->convertToNodeSpace(Vec2(0, curLandWorldRect.getMaxY())).y);
		playAnimation(Armature_SiWang);
	}

	FX::Music::getInstance()->playEffectMusic("r_ch_die");
}

void HeroSprite::skill_screenShake()
{
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
			CallFunc::create(std::bind(&HeroSprite::showDieDlg,this)), 
		nullptr));
}

void HeroSprite::showDieDlg()
{
	m_pArmature->getAnimation()->stop();

	//1. 复活属性
	int rebornNum = 0;
	for (size_t i=0; i<m_globleAddAttribute.size(); )
	{
		if (m_globleAddAttribute[i].attr == Attr_Reborn) {
			++rebornNum;
			m_globleAddAttribute.erase(m_globleAddAttribute.begin()+i);
		}else {
			++i;
		}
	}
	//人物复活属性也不能在关卡模式用
	if (rebornNum == 2 && RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless){
		RunningHelper::getHelper()->gameReborn();
		return;
	}

	//2. 开场购买的死亡冲刺道具 (无尽模式才能用)
	if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless
		&& RunningHelper::getHelper()->getCurrentScenePlayMode() != mode_Fight)
	{
		GoodsHelper *goodsHelper = GoodsHelper::getHelper();
		const BagItem *item = goodsHelper->getOneItem(unitID_SiWangChongCi);
		if ((item && item->value > 0) && m_iSiWangChongCiTimes < g_fMaxSiWangChongCiTimes)
		{
			WaitingLayer* pWaitingLayer = WaitingLayer::createWithType(WaitingLayer::WaitingLayer_Type_SiWangChongCi);
			Director::getInstance()->getRunningScene()->addChild(pWaitingLayer, 10);
// 			const vector<Attribute>& vec = goodsHelper->getUnitByID(item->id)->attribute;
// 			for (int i=0,count=vec.size(); i<count; ++i)
// 			{
// 				if (vec[i].attr == Attr_SpeedUp){
// 					RunningHelper::getHelper()->getCurrentStateBar()->startEcffect(StateBar::Tag_DieSpeed);
// // 					this->removeAllChildren();
//  					this->stopAllActions();
// 					//修改死亡冲刺后没效果的问题gameReborn(false)-->gameReborn(true);
// 					RunningHelper::getHelper()->gameReborn(true);
// 					rockets(vec[i].attr, vec[i].value, true);
// 					goodsHelper->useOneGoods(item->id);
//			m_iSiWangChongCiTimes++;
 			return;
// 				}
// 			}
		}
	}
	this->showDeadLayer();
}

void HeroSprite::showDeadLayer()
{
	if (RunningHelper::getHelper()->getCurrentScenePlayMode() != mode_Fight
		&& (m_iFuHuoTimes-m_iSiWangChongCiTimes < g_fMaxZuanShiFuHuoTimes))
	{
		//3. game over
		{
			EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
			EventCustom event(MSG_HeroDead);
			eventDspc->dispatchEvent(&event);
			if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless)
			{
				//手动单独调用死亡界面（因为有时候会出现界面弹不出的情况）
				StopLayer* pDieStopLayer = StopLayer::create();
				pDieStopLayer->showCommonPlayDeadLayer();
				RunningHelper::getHelper()->getCurrentStateBar()->addChild(pDieStopLayer, 100, StateBar::Tag_DeadLayer);
			}
		}
	}
	else
	{
		if (RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless)
		{
			StopLayer* pEndLayer = StopLayer::create();
			pEndLayer->showFightPlayDeadLayer();
			RunningHelper::getHelper()->getCurrentStateBar()->addChild(pEndLayer, 100, StateBar::Tag_DeadLayer);
		}
	}

}

void HeroSprite::DownGlide()
{
	if (!RunningHelper::getHelper()->getGameInTouchLayer()->getCanBeTouched() )//|| m_bWillSuperRun)
	{
		return;
	}
	if (m_curState == state_Run) 
	{
		changeState(state_DownGlide);
	}

}

void HeroSprite::jump()
{
	if (!RunningHelper::getHelper()->getGameInTouchLayer()->getCanBeTouched())
		return;

	switch (m_curState)
	{
	case HeroSprite::state_Run:
		changeState(state_Jump1);
		break;
	case HeroSprite::state_DownGlide:
		changeState(state_Run);
		break;
	case HeroSprite::state_Jump1:
	case HeroSprite::state_Fall1:
		changeState(state_Jump2);
		break;
	case HeroSprite::state_Jump2:
	case HeroSprite::state_Fall2:
		for(const auto &temp : m_globleAddAttribute) {
			if (Attr_ThreeJump == temp.attr){
				changeState(state_Jump3);
				break;
			}
		}
		break;
	case HeroSprite::state_Jump3:
	case HeroSprite::state_Fall3:
		break;
	case HeroSprite::state_ChongCi:
	case HeroSprite::state_Super:
		break;
	case HeroSprite::state_Bounce:
	case HeroSprite::state_BounceBack:
		if (m_bIsSpringBounce)
		{

		}
		break;
	case HeroSprite::state_Dead:
		break;
	default:
		break;
	}
}

void HeroSprite::jumpHighestPoint()
{
	if (m_curState == state_Jump1){
		playAnimation(Armature_Tiao_Guodu_1);
	}
	else if (m_curState == state_Jump2){
		playAnimation(Armature_Tiao_Guodu_2);
	}
	else if (m_curState == state_Jump3){
		playAnimation(Armature_Tiao_Guodu_2);
	}
	else if (m_curState == state_Bounce){ //普通弹簧
		playAnimation(Armature_Tiao_Guodu_1);
	}
	else if (m_curState == state_Super){
	}
	else CCASSERT(false, "error branch");
}


void HeroSprite::fall()
{
	if (m_curState == state_Dead || m_curState == state_Super)
	{
		return;
	}
	if (m_curState != state_Fall1 && m_curState != state_Fall2 && m_curState != state_Fall3)
		CCASSERT(false, "error state, 调用fall之前，请确保把状态更为下落");
	

	playAnimation(Armature_Xialuo);

	//落地翻滚
	if (m_curState == state_Fall2 || m_curState == state_Fall3 || m_bIsSpringBounce){
		m_bTreadLandRoll = true;
	}

	float fallHeight = RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->getPositionY()-mWinSize.height*2;
	//时间随速度变化
	float fallTime = sqrt(2*abs(fallHeight)/10.f)/21.5f;
	if (!m_bIsSpringBounce 
		&& (m_curState == state_Fall1 || m_curState == state_Fall2 || m_curState == state_Fall3)
		&& (m_previouState == state_Jump1 || m_curState == state_Jump2 || m_curState == state_Jump3)){
		fallTime /= JUMP_SPEED_MULTI(RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth());//RunningHelper::getHelper()->getRunnerMultitypeSpeed();
	}

	Sequence *action = Sequence::create(
			FX::ActionJumpDown::create(fallTime, fallHeight),
//			CallFunc::create(std::bind(&HeroSprite::fallDone,this)),  //由碰撞检测处理
			nullptr);
	action->setTag(Tag_JumpFallAction);
	this->runAction(action);

	recordFallInfoEnable(true);
}


void HeroSprite::fallDone()
{
	if (m_pArmature->getAnimation()->getCurrentMovementID() == Armature_Pao)
		return;

	changeState(state_Run);

	if (m_bTreadLandRoll)
	{
		m_bTreadLandRoll = false;
		playAnimation(Armature_FanGun);
	}
}

void HeroSprite::fight()
{

}

void HeroSprite::treadLand(UnitSprite* pSprite)
{
	recordFallInfoEnable(false);
	//普通弹簧
	if (m_bIsSpringBounce) m_bIsSpringBounce = false;

	if (m_curState == state_Super || m_curState == state_ChongCi || m_curState == state_Bounce)
	{
		return;
	}
	if (m_bWillSuperRun && RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Common)
	{
		this->willStartSuperPlay();
		return;
	}


	Rect landRect = pSprite->getCollisonRectInWorldSpace();
	m_fLandPosY = landRect.getMaxY() - OFFSET_Land_Hero;
	m_fLandHeight = landRect.size.height/2;


	if (state_Fall1 == m_curState || state_Fall2 == m_curState || state_Fall3 == m_curState || state_BounceBack == m_curState)
	{
		m_startJumpPos.y = m_fLandPosY;
		fallDone();
	}

	//关卡模式到达终点，   落在路面上时
	if (m_bArrivalTerminal){
		RunningHelper::getHelper()->gameWin();
	}
}

void HeroSprite::treadBounce(float height, UnitSprite* pObsSprite)
{
	recordFallInfoEnable(false);
	//普通弹簧
	if (m_bIsSpringBounce) m_bIsSpringBounce = false;

	if (m_curState == state_Jump1 || m_curState == state_ChongCi || m_curState == state_Super) 
		return;

	m_startJumpPos = this->getPosition();

	this->stopAllActions();
	if (height <= 5*g_fGridWidth) 
	{
		changeState(state_Jump1);
		this->stopAllActions();
		Sequence* action = Sequence::create(
			//EaseExponentialOut::create(MoveBy::create(height / 250 * JUMP_Time, Vec2(0,height))),
			//FX::ActionJumpUp::create((height / 200 * g_fJump1NeedTime) / RunningHelper::getHelper()->getRunnerMultitypeSpeed(), height),
			FX::ActionJumpUp::create((height / 200 * g_fJump1NeedTime) / JUMP_SPEED_MULTI(RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth()), height),
			CallFunc::create(std::bind(&HeroSprite::jumpHighestPoint,this)), 
			nullptr);
		this->runAction(action);
	}
	else
	{
		if (pObsSprite)
		{
			pObsSprite->initWithFile("0none.png");
			Armature* pArm = Armature::create("tanhuang");
			pObsSprite->addChild(pArm);
			pArm->getAnimation()->play("tanhuang2");
		}

		m_bIsSpringBounce = true;//为了在普通弹簧能够加速
		float jumpTime = height / 190 * g_fJump1NeedTime + 0.0835f; //0.0835 ->5帧的时间，因为以前弹簧起跳是jump1的动画，而不是转圈，jump1动画完之后会接一个5帧的跳1过度动画。现在用转圈则没有了这个过度动画。

		this->setCurState(state_Jump1);
		RunningHelper::getHelper()->setRunnerMultitypeSpeed(BOUNCE_SPEED_MULTI(RunningHelper::getHelper()->getGameUnitLayer()->getCurModeBaseSpeed()));
// 		m_pArmature->getAnimation()->setSpeedScale(0.2333/jumpTime);
// 		m_pArmature->getAnimation()->play(Armature_Tiao_1);
		m_pArmature->getAnimation()->setSpeedScale(0.45/jumpTime);
		m_pArmature->getAnimation()->play(Armature_ZhuanQuan);

		Sequence* action = Sequence::create(
			//FX::ActionJumpUp::create(jumpTime, height),
			EaseExponentialOut::create(MoveBy::create(jumpTime, Vec2(0,height))),
			//CallFunc::create([=](){ playAnimation(Armature_Tiao_Guodu_1); }),
			CallFunc::create([=](){ changeState(state_Fall1); }),
			nullptr);
		this->runAction(action);

		//music
		FX::Music::getInstance()->playEffectMusic("r_monster_break");
	}
}

void HeroSprite::BounceFly(float height, Vec2 startWorldPos, UnitSprite* pObsSprite)         //踩踏 反弹动画
{
	recordFallInfoEnable(false);
	//普通弹簧
	if (m_bIsSpringBounce) m_bIsSpringBounce = false;

	//1. prepare
	changeState(state_Bounce);
	RunningHelper::getHelper()->setRunnerMultitypeSpeed(2.0f);
	this->setPosition(this->getParent()->convertToNodeSpace(startWorldPos));//定点起跳

	//2. cacl
#define DeltaY(deltaTime, Height) (4*(deltaTime)*(Height)-4*(deltaTime)*(deltaTime)*(Height))
	float jumpY = height;
	float jumpTime = 2.0f;
	float fallHeight = RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer()->getPositionY()-mWinSize.height;
	float fallTime = sqrt(abs(fallHeight)/100);
	Rect heroRect = this->getCollisonRectInWorldSpace();
	Rect obsRect;
	Vec2 originHeroWorldPos(heroRect.getMidX(), heroRect.getMinY());//抛物线起跳点
	Vec2 topHeroWorldPos = originHeroWorldPos + Vec2(jumpTime/Director::getInstance()->getAnimationInterval()*RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth(), jumpY);//抛物线最高点
	Vec2 landPos;
	vector<SceneItem> obsVec;
	RunningHelper::getHelper()->getGameUnitLayer()->getPreloadSceneItemQueue(obsVec);

	//1)遍历之后每一个路面，计算出人物运动到该路面最右端所用的时间
	//2)通过时间计算人物当前的y， land maxY > hero minY 则与该路面相交（即着陆点）
	for (size_t obsNum=0; obsNum<obsVec.size(); ++obsNum)
	{
		if (obsVec.at(obsNum).unit->type != unittype_Land) continue;

		obsRect = RunningHelper::getHelper()->getGameUnitLayer()->CalculateSceneItemRectInWorldSpace(obsVec.at(obsNum));		
		landPos.x = obsRect.getMaxX();
		landPos.y = obsRect.getMaxY();
		//			CCLOG("(%.1f, %.1f)", landPos.x, landPos.y);
		//只计算下落部分(在抛物线最高点右侧)
		float deltaX = landPos.x - topHeroWorldPos.x;
		if (deltaX < 0)
			continue;
		float useTime = deltaX / (RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth()/Director::getInstance()->getAnimationInterval());
		float heroY = topHeroWorldPos.y-(-fallHeight) + DeltaY(useTime/fallTime*0.5f+0.5f, -fallHeight);

		if (heroY <= landPos.y)
		{
			landPos = Vec2(obsRect.getMidX(), obsRect.getMaxY());
			//obsTemp->setColor(Color3B::RED);
			//				CCLOG("answer land pos = %.1f, %.1f", landPos.x, landPos.y);
			break;
		}
		//没找到、预加载不够、那么就取最后一个吧
		if (obsNum == obsVec.size()-1)
		{
			landPos = Vec2(obsRect.getMidX(), obsRect.getMaxY());
		}
	}

	//1. 缩放
	float fScale = 0.7f;
	if (originHeroWorldPos.y-landPos.y > 0)
		fScale -= (originHeroWorldPos.y-landPos.y)/72*0.03125f;
	//2. 放大时间
	float scaleBackTime = (landPos.x - topHeroWorldPos.x) / (RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth()/Director::getInstance()->getAnimationInterval());
	if (scaleBackTime < 0.15f) scaleBackTime = 1.0f;
	else if (scaleBackTime < 0.3f) scaleBackTime = 1.25f;
	else if (scaleBackTime < 0.55f) scaleBackTime = 1.5f;
	else if (scaleBackTime < 0.85f) scaleBackTime = 2.0f;

	Sequence* action = Sequence::create(
		Spawn::create(
		FX::ActionJumpUp::create(jumpTime, height),
		CallFunc::create([=](){RunningHelper::getHelper()->scaleCamera(jumpTime, fScale);}),
		nullptr), 
		CallFunc::create([=](){
			this->stopActionByTag(Tag_BounceAction);
			//this->stopAllActions();
			this->setCurState(state_BounceBack);
			Spawn *action = Spawn::create(
				CallFunc::create([=](){RunningHelper::getHelper()->scaleCamera(scaleBackTime, 1);}),
				FX::ActionJumpDown::create(fallTime, fallHeight),
				nullptr);
			action->setTag(Tag_JumpFallAction);
			this->runAction(action);
			recordFallInfoEnable(true);
	}),
		nullptr);
	action->setTag(Tag_BounceAction);
	this->runAction(action);


	Layer *obsLayer = RunningHelper::getHelper()->getGameUnitLayer()->getObstacleContainerLayer();
	Armature* pEffect = Armature::create("effect");
	obsLayer->addChild(pEffect,10);
	pEffect->getAnimation()->setMovementEventCallFunc([pEffect](Armature* pAr,MovementEventType moveType,string moveId){
		if (moveType == COMPLETE && moveId == "e_tiao")
		{
			pEffect->removeFromParent();
		}
	});
	pEffect->setPosition(obsLayer->convertToNodeSpace(this->getParent()->convertToWorldSpace(this->getPosition())));
	pEffect->getAnimation()->play("e_tiao");

	//tanhuang
	if (pObsSprite)
	{
		pObsSprite->initWithFile("0none.png");
		Armature* pArm = Armature::create("tanhuang");
		pObsSprite->addChild(pArm);
		pArm->getAnimation()->play("tanhuang1");
	}
	//music
	FX::Music::getInstance()->playEffectMusic("r_monster_break");
}

void HeroSprite::addScore(int score, UnitSprite* pSprite)
{
	unsigned int unit_id = pSprite->getSelfUnit()->ID;
	RunningHelper::getHelper()->addScore(score, (unit_id == unitID_JinBi || unit_id == unitID_JinDan));

//	if (score >= 10) displayAddScoreLabel(score);
}
void HeroSprite::displayAddScoreLabel(int score)
{

}
void HeroSprite::obstructTheAdvance()
{
	if (m_curState == state_Bounce || m_curState == state_Super || m_curState == state_ChongCi)
	{
		return;
	}

	//无敌状态时，阻挡无效，需要从检测数组里面移除
	if (isHaveAttrOnGoing(Attr_Rush) && m_pCurObstructSprite)
	{
		if (m_pCurObstructSprite->getSelfUnit()->type != unittype_Land && m_pCurObstructSprite->getSelfUnit()->ID != unitID_JianXi)
		{
			RunningHelper::getHelper()->getUnitCollsionDetection()->addWillBeRemoveFromVec((ObstacleSprite *)m_pCurObstructSprite);
		}
		return;
	}


	//如果当前是下落，那么可能是下落太快，卡在障碍物或者陆地中间
	//那么如果有tread相关属性，都应该把hero往上提，置为tread状态，而不是被阻挡。
	Rect rect = m_pCurObstructSprite->getCollisonRectInWorldSpace();
	Rect heroRect = this->getCollisonRectInWorldSpace();
	if (m_startFallPos.y > rect.getMaxY()
		&& (m_curState == state_Fall1 || m_curState == state_Fall2 || m_curState == state_Fall3 || m_curState == state_BounceBack)
		&& m_pCurObstructSprite->getSelfUnit()->haveAttrTread()
		&& heroRect.getMidX() > rect.getMinX())
	{
		this->setPositionY(this->getParent()->convertToNodeSpace(Vec2(0,rect.getMaxY()-OFFSET_Land_Hero+heroRect.size.height/2)).y);
		treadLand(m_pCurObstructSprite);
		return;
	}



	float speed = RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth();
	this->setPositionX(this->getPositionX()-speed);
	if (this->getCollisonRectInWorldSpace().getMidX() <= 0)
	{
		//die();
		if (!this->changeHp(Type_Minus, 1))
		{
			this->reborn(false);
		}
	}
}

void HeroSprite::invincible(string attr, float fTime)
{
	if (attr != Attr_Invincible) return;

	float protectTime = fTime;
	//加成   可能有多个
	for (auto iter = m_globleAddAttribute.begin() ; iter != m_globleAddAttribute.end() ; iter++)
	{
		if (iter->attr == Attr_AddInvincible)
		{
			protectTime += fTime * iter->value;
		}
	}

	auto& temp = m_lHeroAttributeList[Attr_Invincible];
	if (temp.onGoing && temp.endTime-temp.curTime > protectTime) 
		return;

	temp.onGoing = true;
	temp.curTime = 0;
	temp.endTime = protectTime;

	// no action
// 	// begin
// 	if (! this->getActionByTag(Tag_InvincibleAction))
// 	{
// 		this->setScale(1.5f);
// 		if (m_curState == state_Run || m_curState == state_DownGlide)
// 		{
// 			float deltaY = getHeroYOffset();// * 2 * (1.5-1);  放大了0.5倍  整体向上移动了半个人的位置
// 			this->setPositionY(this->getPositionY() + deltaY);
// 		}
// 		Action *action = Repeat::create(Sequence::create(
// 			TintTo::create(0.1f, 255,255,255),
// 			DelayTime::create(1),
// 			TintTo::create(0.1f, 255,100,100),
// 			DelayTime::create(1),
// 			nullptr), -1);
// 		action->setTag(Tag_InvincibleAction);
// 		this->runAction(action);
// 	}

	FX::Music::getInstance()->playEffectMusic("i_invincible");
}
void HeroSprite::protect(float fTime)
{
	//加成  可能有多个
	float protectTime = fTime;
	for (vector<Attribute>::const_iterator iter = m_globleAddAttribute.begin() ; iter != m_globleAddAttribute.end() ; iter++)
	{
		if (iter->attr == Attr_AddProtect)
		{
			protectTime += iter->value * fTime;
		}
	}

	auto& temp = m_lHeroAttributeList[Attr_Protect];
	if (temp.onGoing && temp.endTime-temp.curTime > protectTime) 
		return;

	temp.onGoing = true;
	temp.curTime = 0;
	temp.endTime = protectTime;


	this->removeChildByTag(Tag_Protect); 
	Sprite *spr = Sprite::create("daoju_paopao.png");
	this->addChild(spr,0,Tag_Protect);
	spr->setPosition(this->getContentSize() / 2);
	spr->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.15f,0.95f),ScaleTo::create(0.15f,1.0f),nullptr)));
}
void HeroSprite::magnet(string attr, float fTime)
{
	float magnetTime = fTime;
	//加成 可能有多个
	for (vector<Attribute>::const_iterator iter = m_globleAddAttribute.begin() ; iter != m_globleAddAttribute.end() ; iter++)
	{
		if (iter->attr == Attr_AddMagnet)
		{
			magnetTime += iter->value * fTime;
		}
	}

	auto& temp = m_lHeroAttributeList[Attr_Magnet];
	if (temp.onGoing && temp.endTime-temp.curTime > magnetTime) 
		return;

	temp.onGoing = true;
	temp.curTime = 0;
	temp.endTime = magnetTime;

	FX::Music::getInstance()->playEffectMusic("i_magnet");
}
void HeroSprite::changeGold(float fTime)
{
	float changeGoldTime = fTime;
	//加成 可能有多个
	for (vector<Attribute>::const_iterator iter = m_globleAddAttribute.begin() ; iter != m_globleAddAttribute.end() ; iter++)
	{
		if (iter->attr == Attr_AddChangeGold)
		{
			changeGoldTime += iter->value * fTime;
		}
	}

	auto& temp = m_lHeroAttributeList[Attr_ChangeToGold];
	if (temp.onGoing && temp.endTime-temp.curTime > changeGoldTime) 
		return;

	temp.onGoing = true;
	temp.curTime = 0;
	temp.endTime = changeGoldTime;
}

void HeroSprite::showChongJiBoAnimation(bool bBig)
{
	float scale = 4.3f;
	if (!bBig)
	{
		scale = 2.3f;
	}
	//music
	FX::Music::getInstance()->playEffectMusic("r_screen_clear");

	Armature* eAr = Armature::create("effect");
	eAr->getAnimation()->setMovementEventCallFunc([](Armature *armature, MovementEventType movementType, const std::string& movementID){
		if (movementType == COMPLETE) armature->removeFromParent();
	});
	eAr->getAnimation()->play("e_chongjibo");
	this->addChild(eAr);
	eAr->setPosition(this->getContentSize()/2);

/*
	float framePreTime = Director::getInstance()->getAnimationInterval();
	Sprite* pActionNode = Sprite::create("daoju_chongjibo_01.png");
	pActionNode->setScale(scale-0.3f);
	char name[64] = {};
	Animation* pAnimation = Animation::create();
	for (unsigned int i = 0; i < 5; ++i)
	{
		sprintf(name, "daoju_chongjibo_%02d.png", i+1);
		pAnimation->addSpriteFrameWithFile(name);
	}
	pAnimation->setDelayPerUnit(2*framePreTime);
	pAnimation->setLoops(1);
	pActionNode->runAction(Sequence::create(Animate::create(pAnimation), 
			Spawn::create(
				SkewTo::create(9*framePreTime, -25.0f, 25.0f),
				ScaleTo::create(9*framePreTime, scale-0.1f, scale),
				RotateTo::create(9*framePreTime, -25),
				FadeTo::create(9*framePreTime, 10),
			nullptr),
			CallFuncN::create(CC_CALLBACK_1(HeroSprite::movementCallback, this)),
		nullptr));

	this->addChild(pActionNode);
	pActionNode->setName("daoju_chongjibo");
	pActionNode->setPosition(this->getContentSize()/2);*/
}

void HeroSprite::speedUp(string attr, float fValue)
{
	float speedUpTime = fValue;
	//加成  可能有多个
	for (vector<Attribute>::const_iterator iter = m_globleAddAttribute.begin() ; iter != m_globleAddAttribute.end() ; iter++)
	{
		if (iter->attr == Attr_AddSpeedUp)
		{
			speedUpTime += iter->value * fValue;
		}
	}

	auto& temp = m_lHeroAttributeList[Attr_SpeedUp];
	if (temp.onGoing && temp.endTime-temp.curTime > speedUpTime) 
		return;
	
	temp.onGoing = true;
	temp.curTime = 0;
	temp.endTime = speedUpTime;


	//效果
	//在update中改速度
	if (m_curState == state_Run)
	{
		playAnimation(Armature_ChongCi);
	}
	if (m_curState != state_Super && m_curState != state_ChongCi) 
	{
		//this->showChongCiAnimation();
		FX::Music::getInstance()->playEffectMusic("i_rush");
	}
}

// pref:
// 冲刺 一共是两个属性： magnet 和 rush（rush自带无敌）
// 所以unit中配的rush 不用再配 magnet ， 否则会导致两个属性的时间可能不一致
//
void HeroSprite::rush(string attr, float fValue)
{
	if (Attr_Rush != attr) return;
	if (m_curState == state_Super) return;

	//data  (冲刺 自带 磁铁效果)
	float rushTime = fValue;
	for (vector<Attribute>::const_iterator iter = m_globleAddAttribute.begin() ; iter != m_globleAddAttribute.end() ; iter++)
	{
		if (iter->attr == Attr_AddSpeedUp)
		{
			rushTime += iter->value * fValue;
		}
	}
	auto& temp = m_lHeroAttributeList[Attr_Rush];
	if (temp.onGoing && temp.endTime-temp.curTime > rushTime) 
		return;

	temp.onGoing = true;
	temp.curTime = 0;
	temp.endTime = rushTime;
	auto& magnet = m_lHeroAttributeList[Attr_Magnet];
	magnet.onGoing = true;
	magnet.curTime = 0;
	magnet.endTime = rushTime;
	auto& invicible = m_lHeroAttributeList[Attr_Invincible];
	invicible.onGoing = true;
	invicible.curTime = 0;
	invicible.endTime = rushTime;
	

	// prepare
	this->setScale(1.0f);
	this->setColor(Color3B(255,255,255));
	this->setRotation(0);
	if (this->getPositionX() < GameUnitLayer::getHeroDefaultPosX())
	{
		this->setPositionX(GameUnitLayer::getHeroDefaultPosX());
	}
	if (m_curState != state_Super && !m_bWillSuperRun && m_curState != state_ChongCi/* && !m_bWillBeChongCi*/)
	{
//		this->setWillBeChongCi(true);  //如果是WillBeChongCi 会在falldone里正式进入冲刺
		changeState(state_ChongCi);
	}
	//效果
	RunningHelper::getHelper()->setRunnerMultitypeSpeed(g_fRushSpeedMulti);
	RunningHelper::getHelper()->scaleCamera(1.5f, 0.6f);


	FX::Music::getInstance()->playEffectMusic("i_rush");
}

void HeroSprite::deathRush(string attr,float fValue)
{
	m_bIsDieRockets = true;


	Rect curLandWorldRect;
	std::pair<bool,Sprite*> landInfo = RunningHelper::getHelper()->getGameUnitLayer()->getRecentLandRect(curLandWorldRect, true);
	float curLandPosY = curLandWorldRect.getMaxY();
	if (curLandPosY > Land_Default_PosY+6)
	{
		float delta = Land_Default_PosY - curLandPosY;
		RunningHelper::getHelper()->MoveCamera(0, delta);
	}
	this->setPositionY(600.0f);


	rush(attr,fValue);
}


void HeroSprite::speedDown(string attr, float fTime)
{

}

void HeroSprite::updateAttribute(float dt)
{
	for (auto iter = m_lHeroAttributeList.begin(); iter != m_lHeroAttributeList.end() ; ++iter)
	{
		//0.
		if (! iter->second.onGoing) 
			continue;
		iter->second.curTime += dt;

		//1.
		if (iter->first == Attr_Invincible)
		{
			if (iter->second.curTime >= iter->second.endTime)
			{
				iter->second.onGoing = false;

				// current has not action
// 				if (m_curState == state_Run)
// 				{
// 					float deltaY = getHeroYOffset();// * 2 * (1.5-1);  放大了0.5倍  整体向上移动了半个人的位置
// 					this->setPositionY(this->getPositionY() - deltaY);
// 				}
// 				this->setScale(1);
// 				this->setColor(Color3B::WHITE);
//				willShock();
			}
		}
		else if (iter->first == Attr_Protect)
		{
			if (iter->second.curTime >= iter->second.endTime)
			{
				iter->second.onGoing = false;

				this->removeChildByTag(Tag_Protect);
				willShock();
				RunningHelper::getHelper()->getCurrentStateBar()->setProtectMenuEnable(true);
			}
		}
		else if (iter->first == Attr_Magnet)
		{
			if (iter->second.curTime >= iter->second.endTime)
			{
				iter->second.onGoing = false;
			}
			else
			{
				//CCLOG("send Magnet LOG!\n")
				EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
				EventCustom event(MSG_Absorbe);
				Rect rect = this->getCollisonRectInWorldSpace();
				Vec2 vec = RunningHelper::getHelper()->getGameUnitLayer()->convertToWorldSpace(Point(rect.getMidX(), rect.getMidY()));
				event.setUserData(&vec);
				eventDspc->dispatchEvent(&event);
			}
		}
		else if (iter->first == Attr_ChangeToGold)
		{
			if (iter->second.curTime >= iter->second.endTime)
			{
				iter->second.onGoing = false;
			}
			else
			{
				EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
				EventCustom event(MSG_ChangeGold);
				Rect rect = this->getCollisonRectInWorldSpace();
				Vec2 vec = RunningHelper::getHelper()->getGameUnitLayer()->convertToWorldSpace(Point(rect.getMidX(), rect.getMidY()));
				event.setUserData(&vec);
				eventDspc->dispatchEvent(&event);
			}
		}
		else if (iter->first == Attr_Rush)
		{
			if (iter->second.curTime >= iter->second.endTime)
			{
				iter->second.onGoing = false;

				//--死亡冲刺
				if (m_bIsDieRockets)
				{
					m_bIsDieRockets = false;
					die();
					return;
				}

				//--其他冲刺
				willShock();
				RunningHelper::getHelper()->scaleCamera(1.0f, 1);
				if (m_curState == state_ChongCi)//如果进入冲刺状态后，又改为其它状态 就不要fall了。
					changeState(state_Fall3);
				this->setIsShowCaiHongQiao(false);
			}
		}
		else if (iter->first == Attr_SpeedUp)
		{
			if (iter->second.curTime >= iter->second.endTime)
			{
				iter->second.onGoing = false;
				//willShock();
				removeChongCiAnimation();
				if (m_curState == state_Run) playAnimation(Armature_Pao); //加速的时候是疾跑动画
			}
			else
			{
				float speed = g_fItemSpeedUpMultiple * RunningHelper::getHelper()->getGameUnitLayer()->getCurMeterShouldSpeed();
				if (RunningHelper::getHelper()->getRunnerMultitypeSpeed() < speed)
				{
					RunningHelper::getHelper()->setRunnerMultitypeSpeed(speed);
				}
			}
		}
		//中毒
		else if (iter->first == Attr_Poison)
		{
			if (iter->second.curTime >= iter->second.endTime)
			{
				iter->second.onGoing = false;
				die();
			}
		}
		else {
			CCLOG("%s ---> error attribute : %s", __FUNCTION__, iter->first.c_str());
			CCASSERT(false, "error branch");
		}
	}

}
bool HeroSprite::isHaveAttrOnGoing(string attr)
{
// 	if (attr == Attr_Invincible && m_bIsProtect)
// 	{
// 		return true;
// 	}

	auto & temp = m_lHeroAttributeList[attr];
	return (temp.onGoing && temp.endTime > temp.curTime);
}

void HeroSprite::willShock()
{
//	m_bIsProtect = true;
	this->showChongJiBoAnimation();
//	m_bIsProtect = false;

	EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
	EventCustom event(MSG_ShockWave);
	Rect rect = this->getCollisonRectInWorldSpace();
	Vec2 vec = rect.size / 2;
	event.setUserData(&vec);
	eventDspc->dispatchEvent(&event);
}

 void HeroSprite::movementCallback(Node* pNode)  
 {  
	pNode->removeFromParent();
 }  
void HeroSprite::super(int y)
{
	float height = Director::getInstance()->getWinSize().height;

	float end_y = this->getPositionY() + y;

		if (end_y >= height - this->getContentSize().height / 2)
		{
			end_y = height - this->getContentSize().height / 2;
		}
	
		if (end_y <= this->getContentSize().height / 2)
		{
			end_y = this->getContentSize().height / 2;
		}
	
	this->setPositionY(end_y);
}
void HeroSprite::startSuperPlay()
{
	RunningHelper::getHelper()->setGoldEffectCount(0);
	if (m_curState == state_Dead)
		return;

	RunningHelper::getHelper()->setRunnerMultitypeSpeed(g_fSuperSpeedMulti);

//	showChaoJiJiangLiAnimation();

	//超跑模式下，不清除有益状态（吸铁石，冲刺，护盾等等。。。）,清除有害属性
	endAttribute(Attr_Poison);

//	m_lHeroAttributeList.clear();
	removeChongCiAnimation();
	removeZhongDuState();

//	this->getParent()->setPositionY(0);
	this->setRotation(0);
//	this->setPositionY(Director::getInstance()->getWinSize().height/2);
	this->setScale(1.0f);
	this->setColor(Color3B(255,255,255));
	changeState(state_Super);

	FX::Music::getInstance()->playEffectMusic("i_rush");
}
void HeroSprite::startCommonPlay()
{
	if (m_curState == state_Dead)
		return;

	RunningHelper::getHelper()->enableCalculateMeter(true);
	RunningHelper::getHelper()->getUnitCollsionDetection()->setCheckCollisionType(UnitCollisionDetection::eCheckType_All);

//	RunningHelper::getHelper()->getGameInTouchLayer()->setCanBeTouched(true);
	RunningHelper::getHelper()->setGoldEffectCount(0);
	
	//不清除有益状态（吸铁石，冲刺，护盾等等。。。）,清除有害属性
	endAttribute(Attr_Poison);

	this->setRotation(0);
	this->setScale(1.0f);
	this->setColor(Color3B(255,255,255));

	if (isHaveAttrOnGoing(Attr_Protect))
	{
		Sprite *spr = Sprite::create("daoju_paopao.png");
		this->addChild(spr,0,Tag_Protect);
		spr->setPosition(this->getContentSize() / 2);
		spr->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.15f,0.95f),ScaleTo::create(0.15f,1.0f),nullptr)));
	}

	removeChongCiAnimation();
	willShock();
	if (m_curState == state_Super)
		changeState(state_Fall3);
}
void HeroSprite::startFightPlay()
{
	if (m_curState == state_Dead)
		return;

	RunningHelper::getHelper()->getGameInTouchLayer()->setCanBeTouched(true);

	endAttribute(Attr_Poison);

	removeChongCiAnimation();
	removeZhongDuState();
}
float HeroSprite::getHeroYOffset()
{
	return 	this->getScaleY() * this->getContentSize().height/2 - this->getContentSize().height/2;
}
// void HeroSprite::rockets(string attr,float fValue,bool isDie)
// {
// 	m_bIsDieRockets = isDie;
// 	if (attr == Attr_Rush)
// 	{
// 		float time = fValue;
// 		magnet(Attr_Magnet, time);
// 		invincible(Attr_Invincible,time);
// 		rush(Attr_Rush,time);
// 	}
// }


void HeroSprite::beatBoss(UnitSprite *pSprite, float value)
{
// 	if (RunningHelper::getHelper()->getCurrentScenePlayMode() != mode_Fight) 
// 		return;
	BossSprite* boss = RunningHelper::getHelper()->getGameUnitLayer()->getBoss();
	if (! boss || boss->getBossHP()<= 0) 
		return;
	//
	float subNum = value * getFightPower();

	Armature *ar = Armature::create("effect");
	ar->getAnimation()->setMovementEventCallFunc([=](Armature *armature, MovementEventType movementType, const std::string& movementID){
		if (movementType == COMPLETE && movementID == "e_zidanfeixing"){
			ar->removeFromParent();
		}
	});

	Vec2 startPos = this->getPosition();
	Vec2 endPos = boss->getPosition();

	this->getParent()->addChild(ar);
	ar->setPosition(startPos);
	ar->setRotation(-atan2(endPos.y-startPos.y,endPos.x-startPos.x)*45);
	ar->getAnimation()->play("e_zidanfeixing");

	ar->runAction(Sequence::create(
		MoveTo::create(15*Director::getInstance()->getAnimationInterval(), endPos),
		CallFunc::create([subNum](){
			BossSprite* boss = RunningHelper::getHelper()->getGameUnitLayer()->getBoss();
			if (boss && RunningHelper::getHelper()->getRunnerHero()->getCurState() != state_Dead) 
				boss->beBeaten(subNum);
	}),
		RemoveSelf::create(),
		nullptr));

// 	Sprite *beat = Sprite::createWithSpriteFrame(pSprite->getSpriteFrame());
// 	boss->getParent()->addChild(beat);
// 	beat->setPosition(this->getPosition());
// 	beat->runAction(Sequence::create(
// 		MoveTo::create(0.5f, boss->getPosition()),
// 		CallFunc::create([subNum](){
// 			BossSprite* boss = RunningHelper::getHelper()->getGameUnitLayer()->getBoss();
// 			if (boss && RunningHelper::getHelper()->getRunnerHero()->getCurState() != state_Dead) boss->beBeaten(subNum);}),
// 		RemoveSelf::create(),
// 		nullptr));
}

void HeroSprite::addZhongDuState(string &attr, float value)
{
	//有中毒状态就不用再加了
	if (isHaveAttrOnGoing(attr))
	{
		return;
	}
	//中毒??秒后没吃到解药就直接死亡
	m_iZhongDuDaoJiShi = value;
	auto& temp = m_lHeroAttributeList[attr];
	temp.onGoing = true;
	temp.curTime = 0;
	temp.endTime = value;

	showZhongDuAnimaiton(true);
	this->showZhongDuDaoJiShi(true);
}

void HeroSprite::removeZhongDuState()
{
	if (isHaveAttrOnGoing(Attr_Poison))
	{
		//effect
		Armature *ar = Armature::create("effect");
		this->addChild(ar, 1, "JieDuEffect");
		ar->getAnimation()->setMovementEventCallFunc([](Armature *armature, MovementEventType movementType, const std::string& movementID){
			if (movementType == LOOP_COMPLETE || movementType == COMPLETE){
				armature->removeFromParent();
			}
		});
		ar->getAnimation()->play("e_jiedu");
		//music
		FX::Music::getInstance()->playEffectMusic("i_detoxify");
	}

	m_iZhongDuDaoJiShi = 0;
	endAttribute(Attr_Poison);
	showZhongDuAnimaiton(false);
	this->showZhongDuDaoJiShi(false);
}

void HeroSprite::showZhongDuDaoJiShi(bool e)
{
	this->removeChildByTag(Tag_TimerLabelAction);
	this->removeChildByTag(Tag_TimerIconlAction);
	if (e)
	{
		LabelAtlas* pTimerLabel = LabelAtlas::create("0", "UI_shuzi_04.png", 35, 55, '0');
		pTimerLabel->setScale(0.8f);
//		pTimerLabel->setColor(Color3B(230, 110, 10));
		this->addChild(pTimerLabel, 1, Tag_TimerLabelAction);
		pTimerLabel->setPosition(Vec2(this->getContentSize().width/2 + 30, this->getContentSize().height + pTimerLabel->getContentSize().height/2 + 40));

		//移除中毒图标（中毒动画已自带该图标）
//		Sprite* pIcon = Sprite::create("UI_biaoshi_zhongdu.png");
//		pIcon->setScale(0.8f);
//		this->addChild(pIcon, 1, Tag_TimerIconlAction);
//		pIcon->setPosition(Vec2(this->getContentSize().width/2 - 10, pTimerLabel->getPositionY() + 30*pTimerLabel->getScale()));

		pTimerLabel->runAction(Repeat::create(
			Sequence::create(
			CallFuncN::create([&](Node* pNode){
				if (m_iZhongDuDaoJiShi <= 0)
				{
					removeZhongDuState();
					return;
				}
				((LabelAtlas*)pNode)->setString(__String::createWithFormat("%d", m_iZhongDuDaoJiShi)->getCString());
				m_iZhongDuDaoJiShi--;
		}), 
			DelayTime::create(1.0f),
			nullptr), 
			10));
	}
	else
	{
		m_iZhongDuDaoJiShi = 0;
	}
}

void HeroSprite::showZhongDuAnimaiton(bool e)
{
	if (e)
	{
 		TintTo* tintto1 = TintTo::create(0.2f, 0, 255, 0);
 		TintTo* tintto2 = TintTo::create(0.2f, 0, 0, 0);
 		auto action = RepeatForever::create(Sequence::create(tintto1, tintto2, nullptr));
 		action->setTag(Tag_ZhongDuAction);
 
 		m_pArmature->runAction(action);
		Armature* pEffect = Armature::create("effect");
		this->addChild(pEffect, 1, Tag_ZhongDuAction);
		pEffect->setPosition(this->getContentSize()/2);
		pEffect->getAnimation()->play("e_zhongdu");

		//music
		FX::Music::getInstance()->playEffectMusic("i_poison");
	}
	else
	{
		this->removeChildByTag(Tag_ZhongDuAction);
		m_pArmature->stopActionByTag(Tag_ZhongDuAction);
		m_pArmature->setColor(Color3B::WHITE);
	}
}

void HeroSprite::playAnimation(const char* animationname)
{
	if (animationname == Armature_Tiao_1)
		m_pArmature->getAnimation()->setSpeedScale(/*(14/60.f)*/0.2333/g_fJump1NeedTime);
	else 
		m_pArmature->getAnimation()->setSpeedScale(1);
	m_pArmature->getAnimation()->play(animationname);
}

void HeroSprite::playAnimationByState(HeroState state)
{
	switch (m_curState)
	{
	case HeroSprite::state_Run:
	case HeroSprite::state_DownGlide:
		if (isHaveAttrOnGoing(Attr_SpeedUp))
			playAnimation(Armature_ChongCi);
		else 
			playAnimation(Armature_Pao);
		break;
	case HeroSprite::state_Jump1:
		playAnimation(Armature_Tiao_1);
		break;
	case HeroSprite::state_Fall1:
	case HeroSprite::state_Fall2:
	case HeroSprite::state_Fall3:
		playAnimation(Armature_Xialuo);
		break;
	case HeroSprite::state_Jump2:
	case HeroSprite::state_Jump3:
		playAnimation(Armature_Tiao_2);
		break;
	case HeroSprite::state_ChongCi:
	case HeroSprite::state_Super:
	case HeroSprite::state_Bounce:
	case HeroSprite::state_BounceBack:
	case HeroSprite::state_Dead:
	default:
		{
			__String temp;
			temp.initWithFormat("this state[%d] should not be injured! please tell us!", m_curState);
			MessageBox(temp.getCString(), "error");
		}
		break;
	}
}

string HeroSprite::getCurPlayAnimationName()
{
	string sRet = m_pArmature->getAnimation()->getCurrentMovementID();
	return sRet;
}

void HeroSprite::pauseArmature()
{
	if (m_pArmature)
	{
		m_pArmature->getAnimation()->pause();
	}
}

void HeroSprite::resumeArmature()
{
	if (m_pArmature)
	{
		m_pArmature->getAnimation()->resume();
	}
}

void HeroSprite::willStartSuperPlay(int status)
{
	this->stopAllActions();
	this->showChongJiBoAnimation();
	RunningHelper::getHelper()->getUnitCollsionDetection()->setCheckCollisionType(UnitCollisionDetection::eCheckType_Items);
	RunningHelper::getHelper()->getGameInTouchLayer()->setCanBeTouched(false);
	Size size = Director::getInstance()->getWinSize();
	playAnimation(Armature_ZuoQi);
	this->runAction(Sequence::create(
		//MoveTo::create(.25f, Vec2(RunningHelper::getHelper()->getGameUnitLayer()->getHeroDefaultPosX()+50, Director::getInstance()->getWinSize().height/2 + 50)), 
		MoveTo::create(.75f, Vec2(RunningHelper::getHelper()->getGameUnitLayer()->getHeroDefaultPosX()+100, Director::getInstance()->getWinSize().height/2)), 
		CallFunc::create([=](){
			Armature *yunAnim = Armature::create("chaopao_yun");
			RunningHelper::getHelper()->getGameUnitLayer()->addChild(yunAnim, this->getLocalZOrder()-1);
			yunAnim->setPosition(Director::getInstance()->getWinSize()/2);
			yunAnim->getAnimation()->play("super_cloud_up");
			yunAnim->getAnimation()->setMovementEventCallFunc([=](Armature *armature, MovementEventType type, const std::string& movementID){
				if (type == COMPLETE) yunAnim->removeFromParent();
			});
			yunAnim->getAnimation()->setFrameEventCallFunc([=](Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex){
				if (frameEventName == "RefreshBackground") 
				{
					RunningHelper::getHelper()->postRefreshBgMsg(100);
					RunningHelper::getHelper()->getGameUnitLayer()->cleanTempObstacleLayer();
					if (status == 1){
						RunningHelper::getHelper()->fightEndToCommon();
					}
				}
			});


			if (status == 0){
				RunningHelper::getHelper()->startSuperPlay();
				RunningHelper::getHelper()->getGameInTouchLayer()->setCanBeTouched(true);
			}
			this->runAction(Sequence::create(
				MoveTo::create(.75f, Vec2(RunningHelper::getHelper()->getGameUnitLayer()->getHeroDefaultPosX(), Director::getInstance()->getWinSize().height/2)),
				nullptr));
		}),
		nullptr));
}

void HeroSprite::endAttribute(const string& attr)
{
	auto& temp = m_lHeroAttributeList[attr];
	temp.onGoing = false;
}

bool HeroSprite::changeHp(HP_ChangType changType, int iNum)
{
	bool bDie = false;
	if (changType == Type_Add)
	{
		m_iHp += iNum;
	}
	else if (changType == Type_Minus)
	{
		m_iHp -= iNum;
	}

	if (m_iHp <= 0)
	{
		m_iHp = 0;
		bDie = true;
	}

	RunningHelper::getHelper()->getCurrentStateBar()->updateHPIconCount(m_iHp);

	return bDie;
}

void HeroSprite::startAttribute(Attribute attr)
{
	if (Attr_Rush == attr.attr) {
		//this->invincible(Attr_Invincible, 5);
		this->rush(attr.attr, attr.value);
	}
	else if (Attr_SpeedUp == attr.attr)
	{
		this->speedUp(attr.attr, attr.value);
	}
	else if (Attr_Magnet == attr.attr) {
		this->magnet(attr.attr, attr.value);
	}
	else if (Attr_Protect == attr.attr) {
		this->protect(attr.value);
	}
	else if (Attr_ChangeToGold == attr.attr){
		this->changeGold(attr.value);
	}
	else {
		CCASSERT(false, "not surpport");
	}
}

bool HeroSprite::isNotInjured()
{
	if (m_curState == state_Super || m_curState == state_ChongCi || m_curState == state_Bounce || m_curState == state_BounceBack)
		return true;

	//不用改包含 protect状态， 该状态只是在受到伤害时有用
	if (isHaveAttrOnGoing(Attr_Invincible)/* || isHaveAttrOnGoing(Attr_Protect)*/ || isHaveAttrOnGoing(Attr_Rush))
		return true;

	return false;
}

void HeroSprite::removeDianZha()
{
	ObstacleSprite* pObstacleSprite= nullptr;
	Vector<ObstacleSprite*>* pObstacleSpriteArray = RunningHelper::getHelper()->getObstacles();
	if (pObstacleSpriteArray)
	{
		for (int i = 0; i < pObstacleSpriteArray->size(); ++i)
		{
			pObstacleSprite = pObstacleSpriteArray->at(i);
			if (pObstacleSprite->getSelfUnit()->ID == unitID_DianZha)
			{
				UnitCollisionDetection *unitCollisionDetection = RunningHelper::getHelper()->getUnitCollsionDetection();
				unitCollisionDetection->addWillBeRemoveFromVec(pObstacleSprite);
				pObstacleSprite->removeFromParent();
				break;
			}
		}
	}
}

void HeroSprite::recordFallInfoEnable(bool b)
{
	if (b)
	{
		m_bFallNow = true;

		Rect rect = this->getCollisonRectInWorldSpace();
		m_startFallPos.x = rect.getMidX();
		m_startFallPos.y = rect.getMinY();
	}
	else
	{
		m_bFallNow = false;
	}
}


void HeroSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	UnitSprite::draw(renderer, transform, flags);

	//debug
	if (1 == DEBUG_uGameInCollisionRect)
	{
		m_customCommand.init(_globalZOrder+1);
		m_customCommand.func = CC_CALLBACK_0(HeroSprite::onDrawDebugCollisonRect, this, transform, flags);
		renderer->addCommand(&m_customCommand);
	}

}
void HeroSprite::onDrawDebugCollisonRect(const Mat4 &transform, uint32_t flags)
{
	glLineWidth(2);
	DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
	Rect rect = this->getCollisonRectInWorldSpace();
	DrawPrimitives::drawRect(rect.origin, Vec2(rect.getMaxX(), rect.getMaxY()));
}
