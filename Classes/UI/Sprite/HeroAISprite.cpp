#include "HeroAISprite.h"
#include "RunningHelper.h"
#include "GameUnitLayer.h"
#include "ObstacleSprite.h"
#include "HeroSprite.h"
#include "VersionConfig.h"
#include "FXJump.h"
#include "UnitCollisionDetection.h"

#include "HeroSprite.h"

#define JUMPUP(__Height__) Sequence::create( \
FX::ActionJumpUp::create(g_fJump1NeedTime / RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth()/g_fMapScrollDelta, __Height__), \
	CallFunc::create(std::bind(&HeroAISprite::jumpHighestPoint,this)), \
	nullptr)
const int JumpSpeed = 22 * 24;
HeroAISprite* HeroAISprite::create(unsigned int id,const Rect &rect)
{
	HeroAISprite* pSprite = new HeroAISprite;
	if (pSprite && pSprite->init(id,rect)){
		pSprite->autorelease();
	}else {
		CC_SAFE_DELETE(pSprite);
		pSprite = nullptr;
	}
	return pSprite;
}
HeroAISprite::HeroAISprite()
	: m_rSelfRect(Rect::ZERO)
	, m_fSpeed(0.0f)
	, m_bJumping(false)
	, m_pPreHeroRect(Rect::ZERO)
	, m_pPreRect(Rect::ZERO)
	, m_pArmature(nullptr)
{

}
HeroAISprite::~HeroAISprite()
{

}
bool HeroAISprite::init(unsigned int id,const Rect &rect)
{
	if (! BaseSprite::init()) 
	{
		return false;
	}
	m_nHeroID = id;
	/*Sprite *spr = Sprite::create();
	spr->setPosition(this->getContentSize());
	addChild(spr);*/
	m_rSelfRect = rect;

	return true;
}
void HeroAISprite::onEnter()
{
	BaseSprite::onEnter();

	pair<string, string> info =  HeroSprite::getArmatureInfo(m_nHeroID);
	m_armatureFileName = info.first;

//	ArmatureDataManager::getInstance()->addArmatureFileInfo(m_armatureFileName.c_str());
	if (!m_pArmature)
	{
		m_pArmature = Armature::create(info.second.c_str());
		m_pArmature->setPositionX(this->getContentSize().width/2);
		this->addChild(m_pArmature);
		//m_pArmature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(HeroSprite::playAnimationEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	playAnimation(Armature_Pao);
	this->runAction(RepeatForever::create(MoveBy::create(1.0f,Vec2(m_fSpeed,0))));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(HeroAISprite::timer),this,0,false);
}
void HeroAISprite::playAnimation(const char* animationname)
{
	if (animationname == Armature_Pao && m_tState == Running)
	{
		return;
	}
	else if (animationname == Armature_Tiao_1 && m_tState == Jumping)
	{
		return;
	}
	else if (animationname == Armature_Xialuo && m_tState == Downing)
	{
		return;
	}
	m_pArmature->getAnimation()->play(animationname);
	if (animationname == Armature_Pao)
	{
		m_tState = Running;
	}
	else if (animationname == Armature_Tiao_1)
	{
		m_tState = Jumping;
	}
	else if (animationname == Armature_Xialuo)
	{
		m_tState = Downing;
	}
}
void HeroAISprite::onExit()
{
	BaseSprite::onExit();
//	ArmatureDataManager::getInstance()->removeArmatureFileInfo(m_armatureFileName.c_str());
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(HeroAISprite::timer),this);
}
Rect HeroAISprite::getCollisonRectInWorldSpace()
{
	Rect ret = m_rSelfRect;
	if (!this->getParent())
	{
		return ret;
	}
	Vec2 worldPos = RunningHelper::getHelper()->getGameUnitLayer()->convertToWorldSpace(this->getPosition());
	float scaleX = this->getScaleX() * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
	float scaleY = this->getScaleY() * RunningHelper::getHelper()->getGameUnitLayer()->getScaleY();
	if (m_rSelfRect.equals(Rect::ZERO))
	{
		CCASSERT(false, "error");
		Size size = this->getContentSize();
		size.setSize(size.width*scaleX, size.height*scaleY);
		ret = Rect(worldPos.x-size.width/2, worldPos.y-size.height/2, size.width, size.height);
	}
	else
	{
		worldPos.x += m_rSelfRect.getMinX() * scaleX;
		worldPos.y += m_rSelfRect.getMinY() * scaleY;
		ret = Rect(worldPos.x/*-m_rSelfRect.size.width/2*scaleX*/, worldPos.y/*-m_rSelfRect.size.height/2*scaleY*/, m_rSelfRect.size.width*scaleX, m_rSelfRect.size.height*scaleY);
	}
	return ret;
}
bool HeroAISprite::checkCollison(const Rect &heroRect,const Rect &obstacleRect)
{
	bool bRet = false;
	if (obstacleRect.intersectsRect(heroRect))
	{
		bRet = true;
	}
	return bRet;
}
void HeroAISprite::timer(float dt)
{
	if (updateHeroCollison())
	{
		return;
	}
	if (this->getCollisonRectInWorldSpace().getMaxX() < -10.0f)
	{
		this->removeFromParent();
		return;
	}
	
	float scale = RunningHelper::getHelper()->getCameraScale();
	Rect _rect = this->getCollisonRectInWorldSpace();
	bool bFirstLand = true;
	bool bFirstJump = true;
	Vector<ObstacleSprite*>* pObstacleSpriteArray = RunningHelper::getHelper()->getObstacles();
	ObstacleSprite *nextLandObstacleSprite = nullptr;
	ObstacleSprite *firstJumpObstacleSprite = nullptr;
	ObstacleSprite *footDownLandSprite = nullptr;
	for (Vector<ObstacleSprite*>::iterator iter = pObstacleSpriteArray->begin(); iter != pObstacleSpriteArray->end() ; iter++)
	{
		ObstacleSprite *pObstacleSprite = *iter;
		if ((pObstacleSprite->getSelfUnit()->type == unittype_Land && pObstacleSprite->getSelfUnit()->ID != unitID_JianXi || pObstacleSprite->getSelfUnit()->ID == unitID_TaXian || pObstacleSprite->getSelfUnit()->ID == 1006))
		{
			Rect rect = pObstacleSprite->getCollisonRectInWorldSpace();
			
			if (checkCollison(_rect, rect))
			{
				this->stopActionByTag(JumpingAction);
				playAnimation(Armature_Pao);
				
				float y = _rect.getMinY() - rect.getMaxY();
				if (y < 0)
				{
					this->setPositionY(this->getPositionY() + rect.getMaxY() - _rect.getMinY());
					_rect = this->getCollisonRectInWorldSpace();
				}
				footDownLandSprite = pObstacleSprite;
				//bFirstLand = true;
			}
			else
			{
				if (rect.getMaxX() > _rect.getMaxX() && rect.getMinX() < _rect.getMinX())
				{
					footDownLandSprite = pObstacleSprite;
				}
				if (bFirstLand && rect.getMaxX() > _rect.getMaxX())
				{
					nextLandObstacleSprite = pObstacleSprite;
					bFirstLand = false;
				}
				if (bFirstJump && rect.getMaxX() > _rect.getMaxX())// && _rect.getMinY() < rect.getMaxY())
				{
					if (footDownLandSprite)
					{
						Rect rect2 = footDownLandSprite->getCollisonRectInWorldSpace();
						float rect_y = rect.getMaxY();
						float rect2_y = rect2.getMaxY();
						if (rect2_y < rect_y)
						{
							firstJumpObstacleSprite = pObstacleSprite;
							bFirstJump = false;
						}
					}
					else
					{
						firstJumpObstacleSprite = pObstacleSprite;
						bFirstJump = false;
					}
					
				}
			}
		}
	}
	if (m_bJumping)
	{
		//return;
	}
	if (footDownLandSprite)
	{
		if (firstJumpObstacleSprite)
		{
			Rect rect = firstJumpObstacleSprite->getCollisonRectInWorldSpace();
			float side_x = rect.getMinX() - _rect.getMaxX();
			float jumpHeight = getJumpHeight(rect.getMaxY() - _rect.getMinY());
			float width = getUpSide(jumpHeight);
			if (side_x <= width)
			{
				/*Action *action = JUMPUP(jumpHeight);
				action->setTag(JumpingAction);
				this->runAction(action);*/
				//this->runAction(JUMPUP(jumpHeight));
				//this->runAction(FX::ActionJumpUp::create(JUMP_Time, jumpHeight));
				this->setPositionY(this->getPositionY() + dt * JumpSpeed);
				playAnimation(Armature_Tiao_1);
			}
			else
			{
				Rect rect_2 = footDownLandSprite->getCollisonRectInWorldSpace();
				if (rect_2.getMaxY() < _rect.getMinY())
				{
					float y = dt * JumpSpeed;
					float side_y = _rect.getMinY() - rect_2.getMaxY();
					if (side_y < y)
					{
						this->setPositionY(this->getPositionY() - side_y);
						playAnimation(Armature_Pao);
					}
					else
					{
						this->setPositionY(this->getPositionY() - y);
						playAnimation(Armature_Xialuo);
					}

					

				}
			}
		}
		else
		{
			Rect rect = footDownLandSprite->getCollisonRectInWorldSpace();
			if (_rect.getMinY() > rect.getMaxY())
			{
				float y = dt * JumpSpeed;
				float side_y = _rect.getMinY() - rect.getMaxY();
				if (_rect.getMinY() - rect.getMaxY() < y)
				{
					this->setPositionY(this->getPositionY() - side_y);
					playAnimation(Armature_Pao);
				}
				else
				{
					this->setPositionY(this->getPositionY() - y);
					playAnimation(Armature_Xialuo);
				}
				
			}
		}
	}
	else
	{
		Rect rect = Rect::ZERO;
		Rect rect2 = Rect::ZERO;
		 if (nextLandObstacleSprite)
		 {
			 rect = nextLandObstacleSprite->getCollisonRectInWorldSpace();
		 }
		 if (firstJumpObstacleSprite)
		 {
			 rect2 = firstJumpObstacleSprite->getCollisonRectInWorldSpace();
		 }
		if (nextLandObstacleSprite != firstJumpObstacleSprite)
		{
			float jumpHeight = getJumpHeight(rect.getMaxY() - _rect.getMinY());
			if (_rect.getMinY() < rect.getMaxY())
			{
				/*Action *action = JUMPUP(jumpHeight);
				action->setTag(JumpingAction);
				this->runAction(action);*/
				//this->runAction(JUMPUP(jumpHeight));
				//this->runAction(FX::ActionJumpUp::create(JUMP_Time, jumpHeight));
				this->setPositionY(this->getPositionY() + dt * JumpSpeed);
				playAnimation(Armature_Tiao_1);
			}
			else
			{
				if (isDown(rect2.getMinX() - _rect.getMidX() - getUpSide(rect2.getMaxY() - rect.getMaxY()),_rect.getMinY() - rect.getMaxY()))
				{
					/*float jumpHeight2 = getJumpHeight(rect2.getMaxY() - _rect.getMinY());
					Action *action = JUMPUP(jumpHeight2);
					action->setTag(JumpingAction);
					this->runAction(action);*/
					//this->runAction(JUMPUP(jumpHeight2));
					//this->runAction(FX::ActionJumpUp::create(JUMP_Time, jumpHeight2));
					this->setPositionY(this->getPositionY() + dt * JumpSpeed);
					playAnimation(Armature_Tiao_1);
				}
				else
				{
					float y = dt * JumpSpeed;
					float side_y = _rect.getMinY() - rect.getMaxY();
					if (side_y < y)
					{
						this->setPositionY(this->getPositionY() - side_y);
						playAnimation(Armature_Pao);
					}
					else
					{
						this->setPositionY(this->getPositionY() - y);
						playAnimation(Armature_Xialuo);
					}
					
					//this->setPositionY(this->getPositionY() - dt * JumpSpeed);
				}
			}
			
		}
		else
		{
			float jumpHeight = getJumpHeight(rect.getMaxY() - _rect.getMinY());
			if (_rect.getMinY() < rect.getMaxY())
			{
				/*Action *action = JUMPUP(jumpHeight);
				action->setTag(JumpingAction);
				this->runAction(action);*/
				//this->runAction(JUMPUP(jumpHeight));
				//this->runAction(FX::ActionJumpUp::create(JUMP_Time, jumpHeight));
				this->setPositionY(this->getPositionY() + dt * JumpSpeed);
				playAnimation(Armature_Tiao_1);
			}
			else
			{
				if (isDown(rect.getMinX() - _rect.getMidX(),_rect.getMinY() - rect.getMaxY()))
				{
					float y = dt * JumpSpeed;
					float side_y = _rect.getMinY() - rect.getMaxY();
					if (side_y < y)
					{
						this->setPositionY(this->getPositionY() - side_y);
						playAnimation(Armature_Pao);
					}
					else
					{
						this->setPositionY(this->getPositionY() - y);
						playAnimation(Armature_Xialuo);
					}
					
				}
				else
				{
					/*Action *action = JUMPUP(JUMP_Height1);
					action->setTag(JumpingAction);
					this->runAction(action);*/
					this->setPositionY(this->getPositionY() + dt * JumpSpeed);
					playAnimation(Armature_Tiao_1);
	
					//this->runAction(FX::ActionJumpUp::create(JUMP_Time, JUMP_Height1));
				}
			}
		}
	}
	/*
	else if (nextLandObstacleSprite)
	{
		Rect rect = nextLandObstacleSprite->getCollisonRectInWorldSpace();
		if (firstJumpObstacleSprite)
		{
			Rect rect2 = firstJumpObstacleSprite->getCollisonRectInWorldSpace();
			if (nextLandObstacleSprite == firstJumpObstacleSprite)
			{
				if (_rect.getMinY() < rect.getMaxY())
				{
					this->setPositionY(this->getPositionY() + dt * JumpSpeed);
					CCLOG("setPositionY \n");
				}
				else if (_rect.getMinY() > rect.getMaxY())
				{
					if (isDown(rect.getMinX() - _rect.getMaxX(),_rect.getMinY() - rect.getMaxY()))
					{
						this->setPositionY(this->getPositionY() - dt * JumpSpeed);
					}
					else
					{
						//CCLOG("2\n");
						this->setPositionY(this->getPositionY() + dt * JumpSpeed);
					}
				}
			}
			else
			{
				if (rect2.getMinX() - _rect.getMaxX() <= getUpSide(rect2.getMaxY() - _rect.getMinY()))
				{
					this->setPositionY(this->getPositionY() + dt * JumpSpeed);
				}
				else
				{
					if (isDown(rect2.getMinX() - _rect.getMaxX() - getUpSide(rect2.getMaxY() - rect.getMaxY()),_rect.getMinY() - rect.getMaxY()))
					{
						this->setPositionY(this->getPositionY() + dt * JumpSpeed);
					}
					else
					{
						//CCLOG("3\n");
						if (_rect.getMinY() > rect.getMaxY())
						{
							this->setPositionY(this->getPositionY() - dt * JumpSpeed);
						}
						
					}
				}
			}
		}
		else
		{
			if (_rect.getMinY() > rect.getMaxY())
			{
				if (isDown(rect.getMinX() - _rect.getMaxX(),_rect.getMinY() - rect.getMaxY()))
				{
					float y = dt * JumpSpeed;
					float side_y = _rect.getMinY() - rect.getMaxY();
					if (_rect.getMinY() - rect.getMaxY() < y)
					{
						this->setPositionY(this->getPositionY() - side_y);
					}
					else
					{
						this->setPositionY(this->getPositionY() - y);
					}
				}
				else
				{
					this->setPositionY(this->getPositionY() + dt * JumpSpeed);
				}
			}
			else
			{
				if (!footDownLandSprite)
				{
					this->setPositionY(this->getPositionY() + dt * JumpSpeed);
				}
			}
			
		}

	}
	else
	{
		int x = 0;
	}
	*/

	//this->get
}
float HeroAISprite::getUpSide(float height)
{
	float t = height * 1.0f / JumpSpeed;
	float side = t * (RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth() * 60 + m_fSpeed);
	return side;
}
bool HeroAISprite::isDown(float width,float height)
{
	bool ret = false;
	float downSide = height / JumpSpeed * (RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth() * 60);
	if (downSide >= width)
	{
		ret = true;
	}
	return ret;
}
float HeroAISprite::getJumpHeight(float height)
{
	/*
	if (height < JUMP_Height1)
	{
		return JUMP_Height1;
	}
	else if (height < JUMP_Height2)
	{
		return JUMP_Height2;
	}
	else if (height < JUMP_Height3)
	{
		return JUMP_Height3;
	}
	return JUMP_Height3;*/
	return height;
}
void HeroAISprite::jumpHighestPoint()
{
	m_bJumping = false;
	this->stopActionByTag(JumpingAction);
	float fallHeight = -this->getPositionY();
	Action *action = FX::ActionJumpDown::create(sqrt(2*abs(fallHeight)/10.f)/21.5f, fallHeight);
	action->setTag(JumpingAction);
	this->runAction(action);
}
bool HeroAISprite::updateHeroCollison()
{
	HeroSprite *sprite = RunningHelper::getHelper()->getRunnerHero();
	Rect _rect = this->getCollisonRectInWorldSpace();
	Rect rect = sprite->getCollisonRectInWorldSpace();

	if (sprite->getCurState() == HeroSprite::state_Dead)
	{
		if (m_fSpeed != 0)
		{
			setSpeed(0);
			if (m_pArmature)
			{
				m_pArmature->getAnimation()->pause();
			}
		}
		return true;
	}
	else
	{
		if (m_fSpeed == 0)
		{
			setSpeed(-RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth()*20);
			if (m_pArmature)
			{
				m_pArmature->getAnimation()->resume();
			}
		}
	}

	if (checkCollison(rect,_rect))
	{
		switch (getCollisonType(m_pPreHeroRect,m_pPreRect))
		{
		case Collison_None:
			{
				return false;
			}
			break;
		case Collison_Left:
			{
				if (sprite->getCurState() == HeroSprite::state_ChongCi || sprite->getCurState() == HeroSprite::state_DownGlide)
				{
					this->removeFromParent();
				}
				else
				{
					RunningHelper::getHelper()->getUnitCollsionDetection()->addWillBeRemoveFromVec((UnitSprite *)this);
					sprite->ObstuctHero();
					sprite->setCurObstuctSprite((UnitSprite*)this);
				}
				
				//this->removeFromParent();
	//			CCLOG("HeroAISprite::updateHeroCollison --------> left");
				return true;
			}
			break;
		case Collison_Top://踩踏双倍加分
			{
	//			CCLOG("HeroAISprite::updateHeroCollison --------> top");
				sprite->treadBounce(4 * g_fGridWidth);
				this->removeFromParent();
				return true;
			}
			break;
		default:
			break;
		}
	}
	m_pPreHeroRect = rect;
	m_pPreRect = _rect;

	return false;

}
CollisonType HeroAISprite::getCollisonType(Rect hiterPreRect,Rect hitedPreRect)
{
	CollisonType type = Collison_None;
	if (hiterPreRect.getMinY() > hitedPreRect.getMaxY() && hiterPreRect.getMinX() < hitedPreRect.getMaxX())
	{
		type = Collison_Top;
	}
	else if (hiterPreRect.getMinY() > hitedPreRect.getMaxY() && hiterPreRect.getMinX() >= hitedPreRect.getMaxX())
	{
		type = Collison_Right;
	}
	else if (hiterPreRect.getMinY() <= hitedPreRect.getMaxY() && hiterPreRect.getMaxX() < hitedPreRect.getMinX())
	{
		type = Collison_Left;
	}
	else if (hiterPreRect.getMinY() <= hitedPreRect.getMaxY() && hiterPreRect.getMaxX() >= hitedPreRect.getMinX())
	{
		type = Collison_Bottom;
	}
	return type;
}
void HeroAISprite::setSpeed(float value)
{
	m_fSpeed = value;
	this->stopAllActions();
	this->runAction(RepeatForever::create(MoveBy::create(1.0f,Vec2(m_fSpeed,0))));
}