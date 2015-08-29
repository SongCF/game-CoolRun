#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Shake  : public ActionInterval
{
public:
	Shake(void);
	~Shake(void);

	static Shake* create(float duration, float length);
	static Shake* createwithlength(float duration, float lengthx,float lengthy);

	bool initWithDuration(float duration, float lengthx,float lengthy);

	float fgRangeRand( float min, float m );
    
	virtual Shake* clone() const override;
	virtual Shake* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
   
    CC_CONSTRUCTOR_ACCESS:

protected:
    Vec2 _positionDelta;
    Vec2 _startPosition;
    Vec2 _previousPosition;

	bool csh;
	float l_gth;
	float time;
	 // Initial position of the shaked node
    float m_initial_x, m_initial_y;
    // Strength of the action
    float m_strength_x, m_strength_y;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(Shake);
};

