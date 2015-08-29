//============================================================
//* boss类
//* boss sprite
//============================================================

#pragma once

#include "UnitSprite.h"
#include <string>
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

class BossSprite : public UnitSprite
{
public:
	enum _Tag
	{
		Tag_BossControlBgSpr = 6000,
		Tag_BossXingSpr,
		Tag_BossHPString,
	};

	enum Boss_State
	{
		BS_Normal = 1,
		BS_Rest,
		BS_Anger,
	};
	CC_SYNTHESIZE(Boss_State, m_bossState, BossState);
public:
	static BossSprite* create(Unit* unit);
	bool init(Unit* unit);
	std::pair<string, string> addArmatureInfo(unsigned int bossId);

	void initBloodBar();
	void updateBloodBar();
	void removeBloodBar();

//	virtual void onEnter() override;
//	virtual Rect getCollisonRectInWorldSpace() override;
//	virtual void onCollision(UnitSprite* pSprite = nullptr){CCAssert(false, "pet has not cllision!");}
//	virtual void update(float fDelta){}

	void changeState(unsigned int num);
	void come();
	bool hit();
	void beBeaten(float subBlood); // 被打
	int getBossHP(){return m_fCurHp;}
	void die();
	void leave(); //离开
	bool isWillLeave(){return m_bWillLeave;}

protected:
	void playBossAnimation(string animateName);
	void playAnimationEnd(Armature *armature, MovementEventType type, const std::string& movementID);
protected:
	float m_iDefaultHp;
	float m_fCurHp;
	float m_fPreHp;
	ProgressTimer	*m_controlSprite;
	Armature*		m_pArmature;

	bool m_bWillLeave; //将要离开
};