#include "Shake.h"


Shake::Shake(void)
{
}


Shake::~Shake(void)
{
}


Shake* Shake::create(float duration, float length)
{
    
	return createwithlength(duration,length,length);
}

Shake* Shake::createwithlength(float duration, float lengthx,float lengthy){

    Shake *ret = new Shake();
	ret->initWithDuration(duration, lengthx,lengthy);
    ret->autorelease();
    return ret;

}

bool Shake::initWithDuration(float duration,float lengthx,float lengthy){

	 if (ActionInterval::initWithDuration(duration))
    {
		this->time=duration;
		csh=false;
		m_strength_x=lengthx;
		m_strength_y=lengthy;
        return true;
    }

    return false;
}


Shake* Shake::clone() const
{
	// no copy constructor
	auto a = new Shake();
    a->initWithDuration(_duration, m_strength_x,m_strength_y);
	a->autorelease();
	return a;
}

void Shake::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _previousPosition = _startPosition = target->getPosition();
}

Shake* Shake::reverse() const
{
    return Shake::create(_duration, l_gth);
}


void Shake::update(float dt)
{

    if (_target)
    {
		//_target

		if(csh==false){
			m_initial_x=_target->getPosition().x;
			m_initial_y=_target->getPositionY();
			csh=true;
		}
		float randx = fgRangeRand( -m_strength_x, m_strength_x )*dt;
        float randy = fgRangeRand( -m_strength_y, m_strength_y )*dt;
		
		_target->setPosition( ccpAdd(Vec2(m_initial_x, m_initial_y),Vec2( randx, randy)));

		if(dt>=1.0f){
			_target->setPosition(Vec2(m_initial_x,m_initial_y));
		}
    }
}



float Shake::fgRangeRand( float min, float max )
{
    float rnd = CCRANDOM_0_1();
    return rnd*(max-min)+min;
}

