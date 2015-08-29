//============================================================
//* 宠物类
//* 提供宠物的展示、游戏进行时的状态变迁、逻辑数据处理
//============================================================

#pragma once

#include "UnitSprite.h"
#include "cocostudio/CocoStudio.h"

class PetSprite : public UnitSprite
{
	enum ArmatureTag
	{
		Running = 1,
		Jumping = 2,
		Downing = 3,
	};
public:
	static PetSprite* create(Unit* unit);

	virtual void onEnter() override;
	virtual void onExit() override;

	void startRun();
	void jump();

	virtual Rect getCollisonRectInWorldSpace() override;
//	virtual void onCollision(UnitSprite* pSprite = nullptr){CCAssert(false, "pet has not cllision!");}
	virtual void update(float fDelta) override;
	void resetOnLand();
private:
	void playAnimation(const char* animationname);
	bool checkCollison(const Rect &heroRect,const Rect &obstacleRect);

	float getUpSide(float height);

	bool isDown(float width,float height);

	float getJumpHeight(float height);

private:
	cocostudio::Armature			*m_pArmature;
	string				m_armatureFileName;
	ArmatureTag			m_tState;
};