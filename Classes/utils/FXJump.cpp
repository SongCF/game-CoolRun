#include "FXJump.h"


namespace FX{

ActionJumpUp* ActionJumpUp::create(float duration, float height)
{
	ActionJumpUp *pAction = new ActionJumpUp();
	pAction->initWithDuration(duration, height);
	pAction->autorelease();

	return pAction;
}

bool ActionJumpUp::initWithDuration(float duration, float height)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_height = height;
		_userDuration = duration;

		return true;
	}

	return false;
}

ActionJumpUp* ActionJumpUp::clone() const
{
	// no copy constructor
	auto a = new ActionJumpUp();
	a->initWithDuration(_duration, _height);
	a->autorelease();
	return a;
}

void ActionJumpUp::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_previousPos = _startPosition = target->getPosition();
}

ActionJumpUp* ActionJumpUp::reverse() const
{
	return ActionJumpUp::create(_duration, _height);
}

void ActionJumpUp::update(float t)
{
	// parabolic jump (since v0.8.2)
	if (_target)
	{
		float frac = t * 0.5f;
		float y = _height * 4 * frac * (1 - frac);
		float x = 0;
//		CCLOG("%s --->dt=%f, frac=%.3f (y=%.2f)", __FUNCTION__, t, frac, y);

#if CC_ENABLE_STACKABLE_ACTIONS
		Vec2 currentPos = _target->getPosition();

		Vec2 diff = currentPos - _previousPos;
		_startPosition = diff + _startPosition;

		Vec2 newPos = _startPosition + Vec2(x,y);
		_target->setPosition(newPos);

		_previousPos = newPos;
#else
		_target->setPosition(_startPosition + Vec2(x,y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
	}
}




ActionJumpDown* ActionJumpDown::create(float duration, float height)
{
	ActionJumpDown *pAction = new ActionJumpDown();
	pAction->initWithDuration(duration, -height);
	pAction->autorelease();

	return pAction;
}
void ActionJumpDown::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_startPosition = target->getPosition() - Vec2(0, _height);
	_previousPos = target->getPosition();
}
void ActionJumpDown::update(float t)
{
	if (_target)
	{
		float frac = 0.5f + t * 0.5f;
		float y = _height * 4 * frac * (1 - frac);
		float x = 0;
//		CCLOG("%s --->dt=%f, frac=%.3f (y=%.2f)", __FUNCTION__, t, frac, y);

#if CC_ENABLE_STACKABLE_ACTIONS
		Vec2 currentPos = _target->getPosition();

		Vec2 diff = currentPos - _previousPos;
		_startPosition = _startPosition + diff;

		Vec2 newPos = _startPosition + Vec2(x,y);
		_target->setPosition(newPos);

		_previousPos = newPos;
#else
		_target->setPosition(_startPosition + Vec2(x,y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
	}
}



}