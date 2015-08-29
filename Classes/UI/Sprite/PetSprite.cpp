#include "PetSprite.h"
#include "RunningHelper.h"
#include "ObstacleSprite.h"
#include "HeroSprite.h"
#include "GameUnitLayer.h"
const int JumpSpeed = 22 * 24;
const int JumpSide = 5;

#define OFFSET_Y 10
#define JUMP_Height 96
#define UP_Height 48
#define DOWN_Height 12

PetSprite* PetSprite::create(Unit* unit)
{
	PetSprite* pSprite = new PetSprite;
	if (pSprite && pSprite->init(unit)){
		pSprite->autorelease();
		pSprite->startRun();
	}else {
		CC_SAFE_DELETE(pSprite);
		pSprite = nullptr;
	}
	return pSprite;
}

Rect PetSprite::getCollisonRectInWorldSpace()
{
	Rect ret = Rect::ZERO;
	if (!this->getParent())
	{
		return ret;
	}
	Vec2 worldPos = RunningHelper::getHelper()->getGameUnitLayer()->convertToWorldSpace(this->getPosition());
	float scaleX = this->getScaleX() * RunningHelper::getHelper()->getGameUnitLayer()->getScaleX();
	float scaleY = this->getScaleY() * RunningHelper::getHelper()->getGameUnitLayer()->getScaleY();


	Size size;
	if (m_unit.rect.empty())
		size = this->getContentSize();
	else 
		size = m_unit.rect[0].size/2;
	size.setSize(size.width*scaleX, size.height*scaleY);
	ret = Rect(worldPos.x-size.width/2*scaleX, worldPos.y-size.height/2*scaleY, size.width*scaleX, size.height*scaleY);
	return ret;
}

void PetSprite::onEnter()
{
	UnitSprite::onEnter();

	this->scheduleUpdate();
}

void PetSprite::onExit()
{
	UnitSprite::onExit();
	this->unscheduleUpdate();
}



void PetSprite::startRun()
{
	this->initWithFile("0none.png");//1*1透明图
	if (m_unit.ID == unitID_PiPiZhu)
	{
		cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("pizizhu.ExportJson");
		m_armatureFileName = "pizizhu";
	}
	else if (m_unit.ID == unitID_XiXiaoYang)
	{
		cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("xixiaoyang.ExportJson");
		m_armatureFileName = "xixiaoyang";
	}
	else if (m_unit.ID == unitID_BuGuNiao)
	{
		cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("buguniao.ExportJson");
		m_armatureFileName = "buguniao";
	}
	else if (m_unit.ID == unitID_ZhanHongLong)
	{
		cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("zhanhonglong.ExportJson");
		m_armatureFileName = "zhanhonglong";
	}
	else
	{
		CCASSERT(false, "暂不支持");
	}

	m_pArmature = Armature::create(m_armatureFileName);
	this->addChild(m_pArmature);
	m_pArmature->getAnimation()->play("paobu");
	m_tState = Running;
}
bool PetSprite::checkCollison(const Rect &heroRect,const Rect &obstacleRect)
{
	bool bRet = false;
	if (obstacleRect.intersectsRect(heroRect))
	{
		bRet = true;
	}
	return bRet;
}

#if 0
void PetSprite::update(float dt)
{
	if (Running != m_tState)
		return;
	HeroSprite* pHero = RunningHelper::getHelper()->getRunnerHero();
	if (!pHero || pHero->getCurState() == HeroSprite::state_Dead)
		return;

	Rect _rect = this->getCollisonRectInWorldSpace();
	Vector<ObstacleSprite*>* pObstacleSpriteArray = RunningHelper::getHelper()->getObstacles();
	bool willJump = false;
	ObstacleSprite *pTreadLand = nullptr;
	Rect jumpDownLandRect, treadLandRect;
	for (Vector<ObstacleSprite*>::iterator iter = pObstacleSpriteArray->begin(); iter != pObstacleSpriteArray->end() ; iter++)
	{
		ObstacleSprite *pObstacleSprite = *iter;
		if (pObstacleSprite->getSelfUnit()->haveAttribute(Attr_TreadNoneffect) && pObstacleSprite->isVisible())
		{
			Rect rect = pObstacleSprite->getCollisonRectInWorldSpace();

			//矫正位置，判断是否要自动跳跃了
			if (pTreadLand==nullptr && checkCollison(_rect, rect))
			{
				pTreadLand = pObstacleSprite;
				treadLandRect = rect;
				float y = _rect.getMinY() - (rect.getMaxY() - OFFSET_Y);
				if (fabs(y) > 2)
				{
					this->setPositionY(this->getPositionY() - y);
					_rect = this->getCollisonRectInWorldSpace();
				}

				//willjump?
				if (_rect.getMaxX() > rect.getMaxX()){
					willJump = true;
				}else{
					break;
				}
			}
			//寻找落地点  nextland
			// pTreadLand的下一个land就是落地点（obsArr是按x排序的）
			if (willJump && pObstacleSprite != pTreadLand)
			{
				if (rect.getMinX() > _rect.getMaxX()//确实有间隙
					|| fabs(rect.getMaxY() - treadLandRect.getMaxY()) > 24) //是高低坎
				{
					jumpDownLandRect = rect;
				}else
				{ //其实是下一个连接的路面
					willJump = false;
				}
				break;
			}
		}
	}//end obs for

	if (willJump)
	{
		//计算距离、高度差
		float distance = jumpDownLandRect.getMinX() - treadLandRect.getMaxX();
		float y_offset = jumpDownLandRect.getMaxY() - treadLandRect.getMaxY();

		float scrollSpeed = RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth();
		float time = distance / scrollSpeed;
		if (time < 0.1f) time *= 2;
		float upTime, downTime;
		float total, upY, downY; // 都使用正数

		float actionRate = 2.4f;
		if (y_offset > 24){ //上坎
			upY = UP_Height+y_offset;
			downY = UP_Height;
			total = upY + downY;
			upTime = time * (upY/total);
			downTime = time * (downY/total);
		}
		else if (y_offset < -24){//下落
			upY = DOWN_Height;
			downY = DOWN_Height-y_offset;
			total = upY + downY;
			upTime = time * (upY/total);
			downTime = time * (downY/total);
		}
		else{ //间隙
			upY = JUMP_Height+y_offset;
			downY = JUMP_Height - y_offset;
			total = upY + downY;
			upTime = time * (upY/total);
			downTime = time * (downY/total);
		}

		m_tState = Jumping;
		this->runAction(Sequence::create(
			EaseOut::create(MoveBy::create(upTime,Vec2(0,upY)), actionRate),
			EaseOut::create(MoveBy::create(upTime,Vec2(0,-downY)), actionRate),
			CallFunc::create([&](){m_tState = Running;}),
			nullptr));
	}
}
#endif

#if 1
void PetSprite::update(float dt)
{
	HeroSprite* pHero = RunningHelper::getHelper()->getRunnerHero();
	if (!pHero || pHero->getCurState() == HeroSprite::state_Dead)
		return;


	if (m_unit.ID == unitID_ZhanHongLong)
	{
		//this->setPositionY();
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
		if (pObstacleSprite->getSelfUnit()->haveAttribute(Attr_TreadNoneffect) && pObstacleSprite->isVisible())
		{
			Rect rect = pObstacleSprite->getCollisonRectInWorldSpace();
			
			if (checkCollison(_rect, rect))
			{
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
	if (footDownLandSprite)
	{
		if (firstJumpObstacleSprite)
		{
			Rect rect = firstJumpObstacleSprite->getCollisonRectInWorldSpace();
			float side_x = rect.getMinX() - _rect.getMaxX();
			float jumpHeight = getJumpHeight(rect.getMaxY() - _rect.getMinY());
			float width = getUpSide(jumpHeight);
			if (side_x <= width && JumpSide < jumpHeight)
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
			if (_rect.getMinY() < rect.getMaxY() && JumpSide < jumpHeight)
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
}
#endif

void PetSprite::resetOnLand()
{
	if (m_unit.ID == unitID_ZhanHongLong){
		return;
	}
	Rect petRect = this->getCollisonRectInWorldSpace();
	Vector<ObstacleSprite*>* pObstacleSpriteArray = RunningHelper::getHelper()->getObstacles();
	for (Vector<ObstacleSprite*>::iterator iter = pObstacleSpriteArray->begin(); iter != pObstacleSpriteArray->end() ; iter++)
	{
		ObstacleSprite *pObstacleSprite = *iter;
		if (pObstacleSprite->getSelfUnit()->haveAttribute(Attr_TreadNoneffect) && pObstacleSprite->isVisible())
		{
			Rect rect = pObstacleSprite->getCollisonRectInWorldSpace();

			if (rect.getMinX()<=petRect.getMaxX() && rect.getMaxX()>=petRect.getMinX())
			{
				playAnimation(Armature_Pao);
				this->setPositionY(this->getPositionY() + rect.getMaxY() - petRect.getMinY());
				break;
			}
		}
	}
}

float PetSprite::getUpSide(float height)
{
	float t = height * 1.0f / JumpSpeed;
	float side = t * (RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth() * 60);
	return side;
}
bool PetSprite::isDown(float width,float height)
{
	bool ret = false;
	float downSide = height / JumpSpeed * (RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth() * 60);
	if (downSide >= width)
	{
		ret = true;
	}
	return ret;
}

void PetSprite::playAnimation(const char* animationname)
{
// 	if (animationname == Armature_Pao && m_tState == Running)
// 	{
// 		return;
// 	}
// 	else if (animationname == Armature_Tiao_1 && m_tState == Jumping)
// 	{
// 		return;
// 	}
// 	else if (animationname == Armature_Xialuo && m_tState == Downing)
// 	{
// 		return;
// 	}
// 	//m_pArmature->getAnimation()->play(animationname);
// 	if (animationname == Armature_Pao)
// 	{
// 		m_tState = Running;
// 	}
// 	else if (animationname == Armature_Tiao_1)
// 	{
// 		m_tState = Jumping;
// 	}
// 	else if (animationname == Armature_Xialuo)
// 	{
// 		m_tState = Downing;
// 	}
}

float PetSprite::getJumpHeight(float height)
{
	return height;
}