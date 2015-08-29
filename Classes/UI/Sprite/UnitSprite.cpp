
#include "UnitSprite.h"
#include "RunningHelper.h"
#include "UnitConfigManager.h"
#include "VersionConfig.h"

#define USE_SPRITE_FRAME 0

bool UnitSprite::init(Unit *unit)
{

	m_unit = *unit;
	mWinSize = Director::getInstance()->getWinSize();
// 	Sprite* tempS = ImageHelper::getUnitSpriteRunningScene(unit->ID);
// 	this->initWithSpriteFrame(tempS->getSpriteFrame());
// 	this->setScale(tempS->getScale());

// 	struct timeval tBegin,tEnd;
// 	gettimeofday(&tBegin, nullptr);

	//有特殊皮肤的要替换
	if (unit->skinIDList.size() > 0 && RunningHelper::getHelper()->getCurRunningMode() == RunningMode::endless){
		//用 meter 不精确，特别是在common结束时 进入super，落地马上从common进入fight会容易解析皮肤错误
		//int phase = abs((int)RunningHelper::getHelper()->getMeter()-1)/g_uFightModeSwitchMeter;
		int phase = RunningHelper::getHelper()->getAlreadyFightPlayCount();
		if (RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Fight) phase -= 1; //当前fight num 属于第num-1 个阶段
		phase %= (unit->skinIDList[0].size()); // 0 - 5   size6
		m_unit.imageFile = UnitConfigManager::getManager()->getOneUnit(unit->skinIDList[0].at(phase))->imageFile;
	}
	else if (unit->skinIDList.size() > 1 && RunningHelper::getHelper()->getCurRunningMode() == RunningMode::tollgate){
		int seasonId = RunningHelper::getHelper()->getCurSeasonId();
		m_unit.imageFile = UnitConfigManager::getManager()->getOneUnit(unit->skinIDList[1][seasonId-1])->imageFile;
	}

#if USE_SPRITE_FRAME == 1
	this->initWithSpriteFrameFile(m_unit.imageFile);
#else
	this->initWithFile(m_unit.imageFile);
#endif
//	gettimeofday(&tEnd, nullptr);
// 	float t = (tEnd.tv_sec-tBegin.tv_sec)*1000 + (tEnd.tv_usec-tBegin.tv_usec)/1000.f;
// 	if (t > 0.1) CCLOG("%s ---> %fms", __FUNCTION__, t);

	//用action来实现，那么就不用每个sprite初始化的时候都判断了
	//附加外框
// 	UnitID typeId = (UnitID)m_unit.ID;
// 	if (typeId == unitID_CiTie || typeId == unitID_DianJin ||
// 		typeId == unitID_WuDi || typeId == unitID_ChongCi)
// 	{
// 		BaseSprite* board = BaseSprite::create("ui_kaishi_xiaoguo_liuguang.png");
// 		board->runAction(RepeatForever::create(RotateBy::create(5.5f, 360))),
// 			board->setScale(0.25f);
// 		this->addChild(board, -1);
// 		board->setPosition(this->getContentSize()/2);
// 	}

	return true;
}

const vector<Attribute>* UnitSprite::getSelfAttribute()
{
	return &m_unit.attribute;
}

const Unit* UnitSprite::getSelfUnit()
{
	return &m_unit;
}

Rect UnitSprite::getCollisonRectInWorldSpace()
{
	CCASSERT(false, "override me!"); 
	return Rect::ZERO;
}

void UnitSprite::onCollision(UnitSprite* pSprite, enumCollisionPos board)
{
	CCASSERT(false, "override me!");
}

