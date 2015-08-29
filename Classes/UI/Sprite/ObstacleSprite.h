//============================================================
//* 人物类
//* 提供人物的展示、游戏进行时的状态变迁、逻辑数据处理
//============================================================

#pragma once

#include "UnitSprite.h"
#include <vector>


class ObstacleSprite : public UnitSprite
{
public:
	static ObstacleSprite* create(Unit* unit, unsigned int actId, vector<unsigned int> triggerId);
	void start(float dt);
	void reinitialize();
	ObstacleSprite();
	~ObstacleSprite();

	CC_SYNTHESIZE(unsigned int, m_actionId, ActionID);
	CC_SYNTHESIZE(vector<unsigned int>, m_triggerId, TriggerID);
	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void onCollision(UnitSprite* pSprite = nullptr, enumCollisionPos board=collideLEFT);
	virtual Rect getCollisonRectInWorldSpace() override;
	Rect getRectInObsContainerSpace();

	virtual void update(float fDelta) override; //update 仅用于触发器
	void JianXiShowAbleCheck(float dt);   //
	void tick_BeAbsorbed(float dt);
	void tick_StoneScroll(float dt);

	//碎裂
	//删除该精灵，（包括parent、collide vector）
	//所有会导致精灵消失的地方都要调用该方法消息，不能直接调用removeFromParent
	void damage();

	//英雄冲击波： 固定范围内的障碍物摧毁
	void heroShaken(Vec2 heroWorldPos); 

	//被吸收  ---> 由hero调用（消息），每调用一次 像hero移动固定的x坐标值，和y值偏移
	void beAbsorbed(Vec2 heroWorldPos);
	//点金  ---> 由hero调用（消息），每调用一次 像hero移动固定的x坐标值，和y值偏移
	void changeGold(Vec2 heroWorldPos);

	//被踩踏
	void beTreaded();

	Node* getOneGoldEffectNode();
	void showGoldAnimation();
	void showBaozhaAnimation();

	void noLandCollision(ObstacleSprite *spr);

public:
	//debug
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDrawDebugCollisonRect(const Mat4 &transform, uint32_t flags);
	

private:
	//所有成员变量 都需要在reinitializes中 设置默认值， 以便重利用的时候是初始化了的
	vector<EventListenerCustom*>				m_lListenerList;


	int m_iBeAbsorbed;

	//debug draw
	CustomCommand m_customCommand;
};