//============================================================
//* 游戏场景内（一共五层结构）
//* 第四层 ：碰撞逻辑检测层
//* 功能 ：检测第三层中玩家角色与所有单元的碰撞
//============================================================


#pragma once

#include "UnitSprite.h"
#include "cocos2d.h"
#include "ObstacleSprite.h"
#include "RunningHelper.h"
using namespace cocos2d;

class UnitCollisionDetection : public Node
{
public:
	enum CheckType
	{
		eCheckType_None,    //不检测
		eCheckType_All,     //检测所有
//		eCheckType_ExceptLand, // 不检测land
		eCheckType_Items,   //只检测item
	};

	UnitCollisionDetection();
	~UnitCollisionDetection();

	virtual bool init();

	CREATE_FUNC(UnitCollisionDetection);

public:
	bool checkCollison(Rect rect1, Rect rect2);

	//set check type?
protected:	
	CheckType m_checkType;
	bool m_bCheckLandCollision; // new update not use
	bool m_bCheckAllCollision; // new update not use
public: 
	void setCheckCollisionType(UnitCollisionDetection::CheckType type)
	{
		m_checkType = type;
	}
	void setCheckCollisonEnable(bool e)
	{
		m_bCheckAllCollision = e;
		if (e)
		{
			this->unscheduleUpdate();
			this->scheduleUpdate();
		}
		else
		{
			this->unscheduleUpdate();
		}
	};
	void setCheckLandCollisonEnable(bool e)
	{
		m_bCheckLandCollision = e;
	}


	inline void addWillBeRemoveFromVec(UnitSprite* pSprite){
		if (!pSprite)
		{
			return;
		}
		m_pWillToBeDeleteFromVec.pushBack(pSprite);
	};

	void update(float dt);
	void newUpdate(float dt);
private:
	void autoRushUpDown(int iLastCollisionIndex, bool bHaveLandObstruct);  //冲刺时  自动上下

	void ReUpdata();
	bool roundInRect(Rect rect1,Rect rect2);
	bool checkIsOnLand(UnitSprite* pHeroSprite, UnitSprite* pObstacleSprite);
	enumCollisionPos getHeroCollisionPos(enumCollisionPos obstacleCollisionPos);
	enumCollisionPos getObstacleCollisionPos();
	bool getObstacleCollisionPos(enumCollisionPos& colBoard, ObstacleSprite* pObs, int heroState);

	unsigned int					m_heroLastTickState;
	Rect							m_heroRect;
	Rect							m_obstacleRect;
	Point							m_preLandPoint;
	Vector<UnitSprite*>			m_pWillToBeDeleteFromVec;
};