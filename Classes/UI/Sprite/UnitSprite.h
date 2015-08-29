//============================================================
//* 精灵基类
//* 需要显示的人物、宠物、障碍物的基类
//* 提供一些公用的接口
//============================================================

#pragma once

#include "UnitDefine.h"
#include "BaseSprite.h"
#include "ImageHelper.h"

enum enumCollisionPos
{
	 collideUP,
	 collideDOWN,
	 collideLEFT,
	 collideRIGHT,
};

class UnitSprite : public BaseSprite
{
public:
	virtual bool init(Unit *unit);

	const vector<Attribute>* getSelfAttribute();

	const Unit* getSelfUnit();

	virtual Rect getCollisonRectInWorldSpace();

	virtual void onCollision(UnitSprite* pSprite = nullptr, enumCollisionPos board=collideLEFT);


protected:
	Unit m_unit;
	Size mWinSize;
};