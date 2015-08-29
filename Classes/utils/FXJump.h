//================================================
//* jump动画
//* 将上 下分开
//================================================

#pragma once

#include "cocos2d.h"

namespace FX{

using namespace cocos2d;

class ActionJumpUp : public ActionInterval
{
public:
	/** creates the action */
	static ActionJumpUp* create(float duration, float height);

	//
	// Overrides
	//
	virtual ActionJumpUp* clone() const override;
	virtual ActionJumpUp* reverse(void) const override;
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
	ActionJumpUp() {}
	virtual ~ActionJumpUp() {}

	/** initializes the action */
	bool initWithDuration(float duration, float height);

protected:
	Vec2           _startPosition;
	Vec2           _previousPos;
	float          _height;
	float          _userDuration;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(ActionJumpUp);
};



class ActionJumpDown : public ActionJumpUp
{
public:
	/** creates the action */ //height  < 0
	static ActionJumpDown* create(float duration, float height);
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;
};


}//namespace