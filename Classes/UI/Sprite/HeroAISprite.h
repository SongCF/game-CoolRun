//============================================================
//* HeroAISprite类
//* HeroAISprite
//============================================================

#pragma once

#include "UnitSprite.h"
#include <iostream>
#include "cocostudio/CocoStudio.h"
using namespace std;

enum CollisonType
{
	Collison_None,//没有碰撞
	Collison_Left,
	Collison_Right,
	Collison_Top,
	Collison_Bottom,
};
class HeroAISprite : public UnitSprite
{
	enum ChildeActionTag
	{
		//Running = 1,
		JumpingAction = 2,
	};
	enum ArmatureTag
	{
		Running = 1,
		Jumping = 2,
		Downing = 3,
	};
	
public:
	static HeroAISprite* create(unsigned int id,const Rect &rect);
	bool init(unsigned int id,const Rect &rect);
	HeroAISprite();
	virtual ~HeroAISprite();
	virtual void onEnter();
	virtual void onExit();
	virtual Rect getCollisonRectInWorldSpace();

	void setSpeed(float value);
	void playAnimation(const char* animationname);
private:
	void timer(float dt);

	bool updateHeroCollison();
	bool checkCollison(const Rect &heroRect,const Rect &obstacleRect);

	float getUpSide(float height);

	bool isDown(float width,float height);
	float getJumpHeight(float height);

	void jumpHighestPoint();//调到最高点了

	CollisonType getCollisonType(Rect hiterRect,Rect hitedRect);

protected:
	Rect				m_pPreRect;
	Rect				m_pPreHeroRect;
	Rect				m_rSelfRect;
	float				m_fSpeed;
	bool				m_bJumping;

	unsigned int		m_nHeroID;
	cocostudio::Armature			*m_pArmature;
	string				m_armatureFileName;
	ArmatureTag			m_tState;
	
};