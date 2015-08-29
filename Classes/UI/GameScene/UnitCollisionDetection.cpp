#include "UnitCollisionDetection.h"
#include "GameDirector.h"
#include "GameUnitLayer.h"
#include "HeroSprite.h"
#include "RunningHelper.h"
#include "FXJump.h"

using namespace cocos2d;

UnitCollisionDetection::UnitCollisionDetection()
	: m_bCheckAllCollision(true)
	, m_bCheckLandCollision(true)
	, m_checkType(eCheckType_All)
	, m_preLandPoint(Point::ZERO)
	, m_heroLastTickState(HeroSprite::state_Run)
{

}

UnitCollisionDetection::~UnitCollisionDetection()
{

}

bool UnitCollisionDetection::init()
{
	if (!Node::init())
	{
		return false;
	}

//	this->scheduleUpdate();

	return true;
}
void UnitCollisionDetection::ReUpdata()
{

	if (RunningHelper::getHelper()->getRunnerHero()->getCurState() == HeroSprite::state_Dead)
	{
		return;
	}
	ObstacleSprite* pObstacleSprite = nullptr;
	Vector<ObstacleSprite*>* pObstacleSpriteArray = RunningHelper::getHelper()->getObstacles();
	map<ObstacleSprite*,unsigned int> pGunShiObstacleSpriteList;
	for (Vector<ObstacleSprite*>::iterator iter = pObstacleSpriteArray->begin();iter != pObstacleSpriteArray->end() ;iter++)
	{
		//没有滚石了，所以先屏蔽掉
//		if (((*iter)->getSelfUnit()->ID == unitID_GunShi_Da && (*iter)->getTriggerOpen()) || ((*iter)->getSelfUnit()->ID == unitID_GunShi_Xiao && (*iter)->getTriggerOpen()) )
//		{
//			pGunShiObstacleSpriteList.insert(map<ObstacleSprite*,unsigned int>::value_type(*iter,0));
//		}
	}
	ObstacleSprite* pLastObstacleSprite = nullptr;
	float scale = RunningHelper::getHelper()->getCameraScale();
	if (pObstacleSpriteArray)
	{

		bool bWillAutoUpOrDown = false;

		int iGunShiCollisionLandCount = 0;
		for (int i = 0; i < pObstacleSpriteArray->size(); ++i)
		{
			pObstacleSprite = (ObstacleSprite*)pObstacleSpriteArray->at(i);
			if (!pObstacleSprite->isVisible() && pObstacleSprite->getSelfUnit()->ID == unitID_JianXi)
			{
				continue;
			}	
			//没有滚石了，所以先屏蔽掉
// 			if (pObstacleSprite->getSelfUnit()->ID != unitID_GunShi_Da && pObstacleSprite->getSelfUnit()->ID != unitID_GunShi_Xiao)
// 			{
// 				for (map<ObstacleSprite*,unsigned int>::iterator iter = pGunShiObstacleSpriteList.begin();iter != pGunShiObstacleSpriteList.end() ;iter++)
// 				{
// 					ObstacleSprite *checkSpr = iter->first;
// 					if (!checkSpr->getParent() || !pObstacleSprite->getParent())
// 					{
// 						continue;
// 					}
// // 					if (checkSpr->getSelfUnit()->ID == unitID_GunShi_Xiao)
// // 					{
// // 						int x= 0;
// // 					}
// 					Rect gunShiRect = Rect::ZERO;
// 					Rect obstacleRect = Rect::ZERO;
// 					{
// 						Rect delta = Rect::ZERO;
// 						if (checkSpr->getSelfUnit()->rect.size() > 0)
// 						{
// 							delta = checkSpr->getSelfUnit()->rect[0];
// 						}
// 						float scaleX = checkSpr->getScaleX();
// 						float scaleY = checkSpr->getScaleY();
// 						Vec2 worldPos = checkSpr->getParent()->convertToWorldSpace(checkSpr->getPosition());
// 						delta.size.width = checkSpr->getContentSize().width;
// 						//worldPos.x += delta.getMinX();
// 						worldPos.y += delta.getMinY();
// 						Rect ret = Rect(worldPos.x-delta.size.width/2*scaleX * scale, worldPos.y-delta.size.height/2*scaleY* scale, delta.size.width*scaleX * scale, delta.size.height*scaleY * scale);
// 						gunShiRect = ret;
// 					}
// 					{
// 						Rect delta = Rect::ZERO;
// 						if (pObstacleSprite->getSelfUnit()->rect.size() > 0)
// 						{
// 							delta = pObstacleSprite->getSelfUnit()->rect[0];
// 						}
// 						float scaleX = pObstacleSprite->getScaleX();
// 						float scaleY = pObstacleSprite->getScaleY();
// 						Vec2 worldPos = pObstacleSprite->getParent()->convertToWorldSpace(pObstacleSprite->getPosition());
// 						delta.size.width = pObstacleSprite->getContentSize().width;
// 						//worldPos.x += delta.getMinX();
// 						worldPos.y += delta.getMinY();
// 						Rect ret = Rect(worldPos.x-delta.size.width/2*scaleX*scale, worldPos.y-delta.size.height/2*scaleY * scale, delta.size.width*scaleX* scale, delta.size.height*scaleY* scale);
// 						obstacleRect = ret;
// 					}
// 					
// 					enumCollisionPos obstacleCollisionPos = collideLEFT;
// 					//如果是藤条，检测碰撞
// 					//如果在上面跑
// 					//如果有一根藤条和人物碰撞就算碰撞，假如所有的藤条 都和精灵没有碰撞，那么就是 没有碰撞到
// 					if (pObstacleSprite->getSelfUnit()->type == unittype_Land)
// 					{
// 						if (!m_bCheckLandCollision)
// 						{
// 							//continue;
// 						}
// 
// 						//默认前面没有路面的阻挡物
// 						bool bHaveLandObstruct = false;
// 						//路面默认是上面被碰撞
// 						obstacleCollisionPos = collideUP;
// 						
// 						if (obstacleRect.intersectsRect(gunShiRect))
// 						{
// //							CCLOG("intersectsRect\n");
// 							if (gunShiRect.getMinY() >= obstacleRect.getMaxY())
// 							{
// 								float gun_x = gunShiRect.getMidX();
// 								float obstacle_x = obstacleRect.getMinX();
// 
// 
// 								if ( gun_x < obstacle_x )
// 								{
// 									pLastObstacleSprite = pObstacleSprite;
// 									continue;
// 								}
// 							}
// 							if (gunShiRect.getMinY() <= obstacleRect.getMaxY() && gunShiRect.getMidX() < obstacleRect.getMinX())
// 							{
// 								pLastObstacleSprite = pObstacleSprite;
// 								continue;
// 							}
// 							gunShiRect = checkSpr->getCollisonRectInWorldSpace();
// 							obstacleRect = pObstacleSprite->getCollisonRectInWorldSpace();
// 							enumCollisionPos heroCollisionPos = collideDOWN;
// 							if (gunShiRect.getMaxY() > obstacleRect.getMinY() && gunShiRect.getMinY() <= obstacleRect.getMinY())
// 							{
// 								float ds = obstacleRect.getMaxX() - gunShiRect.getMinX();
// //								CCLOG("ds == %f \n",ds);
// 								if (ds >= 0)
// 								{
// 									heroCollisionPos = collideRIGHT;
// 								}
// 							}	
// 							else if (gunShiRect.getMaxY() > obstacleRect.getMinY())
// 							{
// 								if (gunShiRect.getMinX() < obstacleRect.getMaxX() && gunShiRect.getMaxX() >obstacleRect.getMinX() )
// 								{
// 									heroCollisionPos = collideUP;
// 								}
// 							}
// 							if (gunShiRect.getMinY() < obstacleRect.getMaxY() && heroCollisionPos == collideUP)
// 							{
// 								float y = obstacleRect.getMaxY() - gunShiRect.getMinY();
// 								checkSpr->setPositionY(checkSpr->getPositionY() + y);
// 							}
// 							checkSpr->onCollision(pObstacleSprite, heroCollisionPos);
// 							iter->second ++;
// 							//pLastGunShiCollisionObstacleSprite = pObstacleSprite;
// 						}
// 
// 					}
// 					else
// 					{
// 						if (obstacleRect.intersectsRect(gunShiRect))
// 						{
// 							gunShiRect = checkSpr->getCollisonRectInWorldSpace();
// 							obstacleRect = pObstacleSprite->getCollisonRectInWorldSpace();
// 							//障碍物的上边被碰撞
// 							if (gunShiRect.getMidY() >= obstacleRect.getMaxY()
// 								&& gunShiRect.getMidX() < obstacleRect.getMaxX() && gunShiRect.getMidX() > obstacleRect.getMinX())
// 							{
// 								obstacleCollisionPos = collideUP;
// 							}	
// 							//障碍物的下边被碰撞
// 							else if (gunShiRect.getMaxY() <= obstacleRect.getMidY()
// 								&& gunShiRect.getMidX() < obstacleRect.getMaxX() && gunShiRect.getMidX() > obstacleRect.getMinX())
// 							{
// 								obstacleCollisionPos = collideDOWN;
// 							}
// 							//障碍物的右边被碰撞
// 							else// if (gunShiRect.getMidX() <= obstacleRect.getMidX())
// 							{
// 								obstacleCollisionPos = collideRIGHT;
// 							}	
// 
// 							enumCollisionPos type1 = collideRIGHT;
// 							if (obstacleCollisionPos == collideDOWN)
// 							{
// 								type1 = collideUP;
// 							}
// 							else if (obstacleCollisionPos == collideUP)
// 							{
// 								type1 = collideDOWN;
// 							}
// 							else if (obstacleCollisionPos == collideRIGHT)
// 							{
// 								type1 = collideLEFT;
// 							}
// 							checkSpr->onCollision(pObstacleSprite, type1);
// 							//					CCLOG("iCollisionZuJianCount ================= count is %d", iCollisionZuJianCount);
// 							//pObstacleSprite->onCollision(pHeroSprite, obstacleCollisionPos);
// 
// 						}
// 					}
// 				}
// 			}
		}
		for (map<ObstacleSprite*,unsigned int>::iterator iter = pGunShiObstacleSpriteList.begin();iter != pGunShiObstacleSpriteList.end() ;iter++)
		{
			if (iter->second == 0)
			{
				iter->first->noLandCollision(pLastObstacleSprite);
//				CCLOG("noLandCollision\n");
			}
		}

	}
}
void UnitCollisionDetection::newUpdate(float dt)
{
	//	ReUpdata();
	//不进行碰撞检测
	//if (!m_bCheckAllCollision) return;
	if (eCheckType_None == m_checkType) return;

	HeroSprite* pHeroSprite = RunningHelper::getHelper()->getRunnerHero();
	if (!pHeroSprite) return;

	Vector<ObstacleSprite*>* pObstacleSpriteArray = RunningHelper::getHelper()->getObstacles();
	if (! pObstacleSpriteArray) return;


	ObstacleSprite* pObstacleSprite = nullptr;
	ObstacleSprite* pLastCollisionObstacleSprite= nullptr;

	bool btreadLand = false;
	HeroSprite::HeroState state = pHeroSprite->getCurState();
	m_heroRect = pHeroSprite->getCollisonRectInWorldSpace();
	for (int i = 0; i < pObstacleSpriteArray->size(); ++i)
	{
		pObstacleSprite = (ObstacleSprite*)pObstacleSpriteArray->at(i);
		//物品不可见状态不检测碰撞
		if (!pObstacleSprite->isVisible()) continue;
		//不进行碰撞检测  (装饰物不做碰撞检测)
		UnitType obsType = pObstacleSprite->getSelfUnit()->type;
		if (obsType == unittype_Decorative || obsType == unittype_Skin) continue;
		//只检测item时
		if (eCheckType_Items == m_checkType && obsType != unittype_Item) continue;

		m_obstacleRect = pObstacleSprite->getCollisonRectInWorldSpace();
		if (checkCollison(m_heroRect, m_obstacleRect))
		{
			//1) 如果是item 直接优化处理
			if (obsType == unittype_Item)
			{
				pHeroSprite->onCollision(pObstacleSprite, collideUP);
				pObstacleSprite->onCollision(pHeroSprite, collideDOWN);
				continue;
			}

			//2）矩形区域 上下左
			enumCollisionPos obstacleCollisionPos;
			if (! getObstacleCollisionPos(obstacleCollisionPos, pObstacleSprite, state)) 
				continue;//有的接触不属于碰撞
			enumCollisionPos heroCollisionPos = getHeroCollisionPos(obstacleCollisionPos);
			//3）碰撞
			pHeroSprite->onCollision(pObstacleSprite, heroCollisionPos);
			pObstacleSprite->onCollision(pHeroSprite, obstacleCollisionPos);

			if (!btreadLand && heroCollisionPos == collideDOWN && pObstacleSprite->getSelfUnit()->haveAttrTread())
				btreadLand = true;
		}
	}// end for
	if (! btreadLand && eCheckType_Items != m_checkType)
		pHeroSprite->noLandCollision();

	m_heroLastTickState = state;//用于悬浮石柱自动下滑


	//最后清理这一帧碰撞后需要删除的 sprite
	for (int i=0,count=m_pWillToBeDeleteFromVec.size(); i<count; ++i)
	{
		UnitSprite* pObSpr = m_pWillToBeDeleteFromVec.at(i);
		pObstacleSpriteArray->eraseObject((ObstacleSprite*)m_pWillToBeDeleteFromVec.at(i));
	}
	m_pWillToBeDeleteFromVec.clear();

}

void UnitCollisionDetection::update(float dt)
{
//	ReUpdata();
	//不进行碰撞检测
	if (!m_bCheckAllCollision) return;

	HeroSprite* pHeroSprite = RunningHelper::getHelper()->getRunnerHero();
	if (!pHeroSprite) return;

	Vector<ObstacleSprite*>* pObstacleSpriteArray = RunningHelper::getHelper()->getObstacles();
	if (! pObstacleSpriteArray) return;


	ObstacleSprite* pObstacleSprite = nullptr;
	ObstacleSprite* pLastCollisionObstacleSprite= nullptr;
	int iCollisionLandCount = 0;
	int iLastCollisionIndex = -1;
	int upLand = 0;

	Vector<ObstacleSprite*> pCollisionSprites;
	HeroSprite::HeroState state;
	m_heroRect = pHeroSprite->getCollisonRectInWorldSpace();
	for (int i = 0; i < pObstacleSpriteArray->size(); ++i)
	{
		state = pHeroSprite->getCurState();

		//英雄死亡状态不进行碰撞检测
		if (!m_bCheckAllCollision || state == HeroSprite::state_Dead) return;

		pObstacleSprite = (ObstacleSprite*)pObstacleSpriteArray->at(i);
		//不进行碰撞检测  (装饰物不做碰撞检测)
		if (pObstacleSprite->getSelfUnit()->type == unittype_Decorative) continue;
		//物品不可见状态不检测碰撞
		if (!pObstacleSprite->isVisible()) continue;

		m_obstacleRect = pObstacleSprite->getCollisonRectInWorldSpace();
		//1. 先检测碰撞区域
		if (checkCollison(m_heroRect, m_obstacleRect))
		{
			//如果是item 直接优化处理
			if (pObstacleSprite->getSelfUnit()->type == unittype_Item)
			{
				pHeroSprite->onCollision(pObstacleSprite, collideUP);
				pObstacleSprite->onCollision(pHeroSprite, collideDOWN);
				continue;
			}

			//没有滚石了，所以先屏蔽掉
// 			//1）特殊形状 ---> 圆形
// 			if (pObstacleSprite->getSelfUnit()->ID == unitID_GunShi_Da || pObstacleSprite->getSelfUnit()->ID == unitID_GunShi_Xiao)
// 			{
// 				if( !roundInRect(m_heroRect,m_obstacleRect))
// 				{
// 					continue;
// 				}
// 			}
			//2）矩形区域 上下左
			enumCollisionPos obstacleCollisionPos = getObstacleCollisionPos();
			enumCollisionPos heroCollisionPos = getHeroCollisionPos(obstacleCollisionPos);
			//3）碰撞分类  type
			// --- 可踩踏
			if (obstacleCollisionPos == collideUP && pObstacleSprite->getSelfUnit()->haveAttribute(Attr_TreadNoneffect))
			{
				//如果不检测路面(包括彩虹桥)，则继续
				if (!m_bCheckLandCollision && (pObstacleSprite->getSelfUnit()->type == unittype_Land || pObstacleSprite->getSelfUnit()->ID == unitID_TaXian))
				{
					continue;
				}

				upLand++;
				//land的碰撞响应统一放到下面了
				if (pObstacleSprite->getSelfUnit()->type != unittype_Land && state != HeroSprite::state_ChongCi)
				{
					pHeroSprite->onCollision(pObstacleSprite, heroCollisionPos);
				}
			}
			// --- obstacle
			//解决速度为9.0的时候，在平路上会导致阻挡的bug。
			//这儿只处理非路面类型的东西，路面的东西会在下面统一处理
			//（原因：配置的路面实际上是高低不一的，所以碰撞检测是左边碰撞，就会阻挡）
			//else ===> else if (pObstacleSprite->getSelfUnit()->type != unittype_Land)
			else if (pObstacleSprite->getSelfUnit()->type != unittype_Land)
			{
				//由于游戏设定，障碍物的右边基本是不可能被碰撞的（但是为了扩展，判断方向的时候任然有右边碰撞），
				//所以，当障碍物的左边被碰撞时，转化为上和下的碰撞
				if (obstacleCollisionPos == collideRIGHT)
				{
					if (m_heroRect.getMidX() < m_obstacleRect.getMaxX())
					{
						if (m_heroRect.getMaxY() < m_obstacleRect.getMidY())
						{
							pObstacleSprite->onCollision(pHeroSprite, collideDOWN);
						}
						else
						{
							pObstacleSprite->onCollision(pHeroSprite, collideUP);
						}
					}
				}
				else
				{
					pObstacleSprite->onCollision(pHeroSprite, obstacleCollisionPos);
				}

				//当下滑时，上方还是阻碍物，放开下滑按钮能保持下滑状态直至没有阻碍物
				if (pObstacleSprite->getSelfUnit()->ID == unitID_XuanFuZhangAi 
					|| pObstacleSprite->getSelfUnit()->ID == unitID_XuanFuZhangAi_Down_Before || pObstacleSprite->getSelfUnit()->ID == unitID_XuanFuZhangAi_Down_After
					|| pObstacleSprite->getSelfUnit()->ID == unitID_XuanFuZhangAi_Up_Before || pObstacleSprite->getSelfUnit()->ID == unitID_XuanFuZhangAi_Up_After)
				{
					if (pHeroSprite->getPreState() == HeroSprite::state_DownGlide && m_heroRect.getMidY() < m_obstacleRect.getMinY())
					{
						pHeroSprite->onCollision(pObstacleSprite, collideUP);
					}
					else
					{
						pHeroSprite->onCollision(pObstacleSprite, heroCollisionPos);
					}
				}
				else
				{
					pHeroSprite->onCollision(pObstacleSprite, heroCollisionPos);
				}
			}

			//单独记录路面的最后一次碰撞的情况
			if (pObstacleSprite->getSelfUnit()->type == unittype_Land || pObstacleSprite->getSelfUnit()->ID == unitID_TaXian)
			{
				if (state == HeroSprite::state_DownGlide || state == HeroSprite::state_Run
					|| state == HeroSprite::state_Fall1 || state == HeroSprite::state_Fall2 || state == HeroSprite::state_Fall3
					|| state == HeroSprite::state_Jump1 || state == HeroSprite::state_Jump2 || state == HeroSprite::state_Jump3
					|| state == HeroSprite::state_ChongCi || state == HeroSprite::state_Bounce || state == HeroSprite::state_BounceBack)
				{
					pCollisionSprites.pushBack(pObstacleSprite);
					iLastCollisionIndex = i;
				}
			}
		}
	}// end for

	//默认前面没有路面的阻挡物
	bool bHaveLandObstruct = false;
	//检测到碰撞后进行的操作
	iCollisionLandCount = pCollisionSprites.size();
	if (iCollisionLandCount > 0)
	{		
		//遍历所有碰到的路面,然后处理
		for (int i = 0; i < iCollisionLandCount; ++i)
		{
			pObstacleSprite = pCollisionSprites.at(i);
			m_obstacleRect = pObstacleSprite->getCollisonRectInWorldSpace();
			//两个陆地有小的高低起伏、避免被阻挡
// 			if (i >= 2)
// 			{
// 				float ds = pCollisionSprites.at(i-1)->getCollisonRectInWorldSpace().getMaxY() - m_obstacleRect.getMaxY();
// 				if (fabs(ds) <= 12 && fabs(ds) > 0)
// 				{
// 					//解决在误差范围内情况下，下滑状态下会自动起来或者下落的问题
// 					if (pHeroSprite->getCurState() != HeroSprite::state_DownGlide)
// 					{
// 						ds = fabs(pCollisionSprites.at(i)->getCollisonRectInWorldSpace().getMinX() - m_heroRect.getMaxX());
// 						if (ds < 10 && ds > 0)
// 						{
// 							pHeroSprite->setPositionY(m_obstacleRect.getMaxY()-10+Y_offset_Hero+pHeroSprite->getHeroYOffset());
// 						}
// 
// 						ds =  fabs(m_heroRect.getMinX() - pCollisionSprites.at(0)->getCollisonRectInWorldSpace().getMaxX());
// 						if (ds < 10 && ds > 0)
// 						{
// 							pHeroSprite->setPositionY(m_obstacleRect.getMaxY()-10+Y_offset_Hero+pHeroSprite->getHeroYOffset());
// 						}
// 					}
// 				}
// 			}

			if (m_heroRect.getMinY() + 20 < m_obstacleRect.getMaxY() && m_heroRect.getMaxY() > m_obstacleRect.getMinY())
			{
				float ds = m_obstacleRect.getMinX() - m_heroRect.getMaxX();
				//路面的左边被碰撞
				if (m_heroRect.getMidX() < m_obstacleRect.getMaxX() && ds <= 0 && ds >= -m_heroRect.size.width)
				{
					bHaveLandObstruct = true;
				}
				//特殊情况下人会掉下去的问题。(大概图形如下)，||表示阻挡物，==表示路面，人撞到阻挡物不应该一直下掉，因为下面有路面
				//									 ||
				//								 人  ||
				// ================================= ||
				//									 ||
				//								  ==================================
				//路面的上面边被碰撞
				if (m_heroRect.getMaxY() <= m_obstacleRect.getMidY()
					&& m_heroRect.getMidX() < m_obstacleRect.getMaxX() && m_heroRect.getMidX() > m_obstacleRect.getMinX())
				{
					bHaveLandObstruct = false;
				}
			}
			if (bHaveLandObstruct)
			{
				if (state != HeroSprite::state_ChongCi)
				{
					pHeroSprite->onCollision(pObstacleSprite, collideRIGHT);
				}
				else if (state == HeroSprite::state_ChongCi && pHeroSprite->getPreState() == HeroSprite::state_Fall3) //如果冲刺需要自动上下，请在设置冲刺状态时，强制preState为fall3
				{
					pHeroSprite->setPositionY(m_obstacleRect.getMaxY() - OFFSET_Land_Hero + m_heroRect.size.height/2);
				}
			}
			else
			{
				enumCollisionPos heroPos = getHeroCollisionPos(getObstacleCollisionPos());
				if (state != HeroSprite::state_ChongCi)
				{
// 					if (heroPos == collideUP)
// 					{
// 						pHeroSprite->noLandCollision();
// 					}
// 					else 
						if (heroPos == collideLEFT)
					{
						//由于游戏设定，人物的左边基本是不可能被碰撞的（但是为了扩展，判断方向的时候任然有左边碰撞），
						//所以，当人物的左边被碰撞时，转化为上和下的碰撞
						if (m_heroRect.getMidX() < m_obstacleRect.getMaxX())
						{
							if (m_heroRect.getMaxY() < m_obstacleRect.getMidY())
							{
								//pHeroSprite->noLandCollision();
								pHeroSprite->onCollision(pObstacleSprite, collideUP);
							}
							else
							{
								pHeroSprite->onCollision(pObstacleSprite, collideDOWN);
							}
						}
					}
					else
					{
						pHeroSprite->onCollision(pObstacleSprite, heroPos);
					}
				}
			}
		}
	}

 	//冲刺状态下自动上下检测
//	autoRushUpDown(iLastCollisionIndex, bHaveLandObstruct);

	if (m_bCheckLandCollision)
	{
		HeroSprite::HeroState curstate = ((HeroSprite*)pHeroSprite)->getCurState();
		if (curstate == HeroSprite::state_Fall1 || curstate == HeroSprite::state_Run
			|| curstate == HeroSprite::state_Fall2 || curstate == HeroSprite::state_Fall3 || curstate == HeroSprite::state_DownGlide
			|| curstate == HeroSprite::state_ChongCi || curstate == HeroSprite::state_Bounce || curstate == HeroSprite::state_BounceBack)
		{
			if (iCollisionLandCount == 0)
			{
				if (upLand <= 0) pHeroSprite->noLandCollision();
			}
		}
	}


	m_preLandPoint = pHeroSprite->getPosition();

	//得到当前阻挡物，没有则为nullptr
	UnitSprite* pObstructSpr = pHeroSprite->getCurObstuctSprite();
	if (pObstructSpr)
	{
		if (pObstructSpr->getCollisonRectInWorldSpace().getMaxX() + 50 < m_heroRect.getMinX())
		{
			pHeroSprite->willResetX();
			pHeroSprite->setCurObstuctSprite(nullptr);
			pObstructSpr = nullptr;
		}
	}
	//最后清理这一帧碰撞后需要删除的 sprite
	for (int i=0,count=m_pWillToBeDeleteFromVec.size(); i<count; ++i)
	{
		UnitSprite* pObSpr = m_pWillToBeDeleteFromVec.at(i);
		pObstacleSpriteArray->eraseObject((ObstacleSprite*)m_pWillToBeDeleteFromVec.at(i));
	}
	m_pWillToBeDeleteFromVec.clear();

}

bool UnitCollisionDetection::getObstacleCollisionPos(enumCollisionPos& _out_boardPos, ObstacleSprite* pObs, int heroState)
{
	// pref: 优先级说明， 踩踏 >= 顶撞 > left  (没有right)
	HeroSprite::HeroState state = (HeroSprite::HeroState)heroState;
	//默认障碍物的左边边被碰撞
	bool bCollision = true;
	_out_boardPos = collideLEFT;

	switch (state)
	{
	case HeroSprite::state_Run:
	case HeroSprite::state_DownGlide:
	case HeroSprite::state_ChongCi:
	case HeroSprite::state_Super:
		{
			//横向  只能有: 3下滑变成跑顶到了悬浮石柱，2上面踩踏，1左侧阻挡伤害
			//有可踩踏属性的 响应踩踏
			if (pObs->getSelfUnit()->haveAttrTread()
				&& (m_obstacleRect.getMaxY()-m_heroRect.getMinY() <= 12) && m_heroRect.getMaxX()-m_obstacleRect.getMinX() >= 12)
			{
				_out_boardPos = collideUP;
			}
			else if ((pObs->getSelfUnit()->ID == unitID_XuanFuZhangAi || pObs->getSelfUnit()->ID == unitID_XuanFuZhangAi_Down_Before || pObs->getSelfUnit()->ID == unitID_XuanFuZhangAi_Down_After || pObs->getSelfUnit()->ID == unitID_XuanFuZhangAi_Up_Before || pObs->getSelfUnit()->ID == unitID_XuanFuZhangAi_Up_After)
				&& (m_heroLastTickState == HeroSprite::state_DownGlide && m_heroRect.getMidY() < m_obstacleRect.getMinY()))
			{
				_out_boardPos = collideDOWN;
			}
			else if (m_obstacleRect.getMidX() > m_heroRect.getMidX() && m_obstacleRect.getMaxY()-m_heroRect.getMinY() >= 12)
			{
				_out_boardPos = collideLEFT;
			}
			else 
			{
				bCollision = false;//下滑站起来那一下 可能碰到人物的左边！
			}
		}
		break;
	case HeroSprite::state_Jump1:
	case HeroSprite::state_Jump2:
	case HeroSprite::state_Jump3:
	case HeroSprite::state_Bounce:
		{
			//向上  只能有： 1左侧阻挡，2顶撞，3下塌离开(此类不应该响应碰撞 --- 用左侧阻挡代替)
			if (pObs->getSelfUnit()->haveAttrTopBump() && (m_heroRect.getMaxY() - m_obstacleRect.getMinY() <= 24))//人物向上顶撞
			{
				//有可顶撞属性的 响应顶撞
				_out_boardPos = collideDOWN;
			}
			else if (m_obstacleRect.getMidX() > m_heroRect.getMidX() && m_obstacleRect.getMaxY()-m_heroRect.getMinY() >= 24)
			{
				_out_boardPos = collideLEFT;
			}
			else
			{
				bCollision = false;
			}
		}
		break;
	case HeroSprite::state_Fall1:
	case HeroSprite::state_Fall2:
	case HeroSprite::state_Fall3:
	case HeroSprite::state_BounceBack:
		{
			// 1.下落时踩踏， 2.阻挡（少见！）
			if (pObs->getSelfUnit()->haveAttrTread() && m_heroRect.getMinY()>m_obstacleRect.getMinY() && m_heroRect.getMaxX()-m_obstacleRect.getMinX()>=12)
			{
				_out_boardPos = collideUP;
			}
			else if (m_obstacleRect.getMinX() > m_heroRect.getMidX() && m_heroRect.getMidY()<m_obstacleRect.getMaxY())
			{
				_out_boardPos = collideLEFT;
			}
			else
			{
				bCollision = false;
			}
		}
		break;
		//
	case HeroSprite::state_Dead:
	default:
		bCollision = false;
		CCASSERT(false, "error branch");
		break;
	}

	return bCollision;

}

enumCollisionPos UnitCollisionDetection::getObstacleCollisionPos()
{
	HeroSprite::HeroState state = RunningHelper::getHelper()->getRunnerHero()->getCurState();

	//默认障碍物的左边边被碰撞
	enumCollisionPos obstacleCollisionPos = collideLEFT;

	//障碍物的上边被碰撞
	//修改一段跳能跳过高木桩的bug
	//m_heroRect.getMidY() >= m_obstacleRect.getMaxY() ====> m_heroRect.getMinY() >= m_obstacleRect.getMaxY() - 25

	//修改向上过程中，会判断成上面碰撞的bug
	//增加状态判断（向上跳的过程肯定是不可能碰撞到上面的）
	//state != HeroSprite::state_Jump1 && state != HeroSprite::state_Jump2 && state != HeroSprite::state_Jump3
	if (state != HeroSprite::state_Jump1 && state != HeroSprite::state_Jump2 && state != HeroSprite::state_Jump3
		&& (m_heroRect.getMinY() >= m_obstacleRect.getMaxY() - 25)
		&& (m_heroRect.getMidX() <= m_obstacleRect.getMaxX() && m_heroRect.getMaxX() >= m_obstacleRect.getMinX()-5))

		//&& (m_heroRect.getMaxX() < m_obstacleRect.getMaxX() && m_heroRect.getMaxX() > m_obstacleRect.getMinX())
		//|| (m_heroRect.getMinX() < m_obstacleRect.getMaxX() && m_heroRect.getMinX() > m_obstacleRect.getMinX())))
	{
		obstacleCollisionPos = collideUP;
	}	
	//障碍物的下边被碰撞 
	//修改两个相邻的蘑菇，在中间跳起的时候有可能会导致从蘑菇中间穿过，一个蘑菇都碰撞不了的bug
	//m_heroRect.getMidX() < m_obstacleRect.getMaxX() ==> m_heroRect.getMinX() < m_obstacleRect.getMaxX()
	//增加状态判断（向下跳的过程肯定是不可能碰撞到下面的）
	//state != HeroSprite::state_Fall1 && state != HeroSprite::state_Fall2 && state != HeroSprite::state_Fall3
	else if (state != HeroSprite::state_Fall1 && state != HeroSprite::state_Fall2 && state != HeroSprite::state_Fall3
		&& m_heroRect.getMaxY() <= m_obstacleRect.getMidY()
		&& m_heroRect.getMinX() <= m_obstacleRect.getMaxX() && m_heroRect.getMidX() >= m_obstacleRect.getMinX())
	{
		obstacleCollisionPos = collideDOWN;
	}
	//障碍物的右边被碰撞
	// 					else if (m_heroRect.getMidX() > m_obstacleRect.getMidX())
	// 					{
	// 						obstacleCollisionPos = collideRIGHT;
	// 					}	
	else// if (m_heroRect.getMinY() + 35 < m_obstacleRect.getMaxY() && m_heroRect.getMaxY() > m_obstacleRect.getMinY())
	{
		float ds = m_obstacleRect.getMinX() - m_heroRect.getMaxX();
		//障碍物的左边被碰撞
		if (m_heroRect.getMidX() < m_obstacleRect.getMaxX() && ds <= 0 && ds >= -m_heroRect.size.width)
		{
			obstacleCollisionPos = collideLEFT;
		}
		//障碍物的右边被碰撞
		else //if (m_heroRect.getMidX() >= m_obstacleRect.getMaxX())
		{
			obstacleCollisionPos = collideRIGHT;
		}
	}

	return obstacleCollisionPos;
}

enumCollisionPos UnitCollisionDetection::getHeroCollisionPos(enumCollisionPos obstacleCollisionPos)
{

	enumCollisionPos heroCollisionPos = collideRIGHT;
	if (obstacleCollisionPos == collideDOWN)
	{
		heroCollisionPos = collideUP;
	}
	else if (obstacleCollisionPos == collideUP)
	{
		heroCollisionPos = collideDOWN;
	}
	else if (obstacleCollisionPos == collideRIGHT)
	{
		heroCollisionPos = collideLEFT;
	}
	return heroCollisionPos;
}

bool UnitCollisionDetection::checkIsOnLand(UnitSprite* pHeroSprite, UnitSprite* pObstacleSprite)
{
	//已经没有地方调用了  ---> by scf

//	bool bRet = false;
//	if (!pHeroSprite || !pObstacleSprite)
//	{
//		return bRet;
//	}
////	HeroSprite::HeroState state =  ((HeroSprite*)pHeroSprite)->getCurState();
////	Point tempPoint = pHeroSprite->getPosition();
////	
////	tempPoint = pHeroSprite->getParent()->convertToWorldSpace(tempPoint);
////
//////	if (state == HeroSprite::state_Run)
////	{
////		tempPoint = Vec2(m_heroRect.getMidX(), m_heroRect.getMidY()-m_heroRect.size.height/2);
////	}
////
////	if (m_obstacleRect.containsPoint(tempPoint))
//	if (m_obstacleRect.intersectsRect(m_heroRect))
//	{
//		return true;
//	}
//
	return false;
}

bool UnitCollisionDetection::checkCollison(Rect rect1, Rect rect2)
{
	bool bRet = false;

	if (rect1.intersectsRect(rect2))
	{
		bRet = true;
	}
	return bRet;
}

bool UnitCollisionDetection::roundInRect(Rect rect1,Rect rect2)
{
	bool bRet = false;
	if (( rect2.getMidX() - rect1.getMidX() ) * ( rect2.getMidX() - rect1.getMidX() ) + ( rect2.getMidY() - rect1.getMidY() ) * ( rect2.getMidY() - rect1.getMidY() ) <= ( rect1.size.width  + rect2.size.width ) * ( rect1.size.width  + rect2.size.width ) / 4)
	{
		bRet = true;
	}
	return bRet;
}

void UnitCollisionDetection::autoRushUpDown(int iLastCollisionIndex, bool bHaveLandObstruct)
{
	HeroSprite *pHeroSprite = RunningHelper::getHelper()->getRunnerHero();

	if ((pHeroSprite->getCurState() == HeroSprite::state_ChongCi || pHeroSprite->getWillSuperRun()) && !bHaveLandObstruct)
	{
		int index = iLastCollisionIndex;
		int upxOffset = 80;
		int downxoffset = 20;
		int icount = 0;

		if (pHeroSprite->getWillSuperRun())
		{
			upxOffset = 40;
		}

		auto pObstacleSpriteArray = RunningHelper::getHelper()->getGameUnitLayer()->getObstacle();
		if (index >= 0 && index < pObstacleSpriteArray->size()-1)
		{
			ObstacleSprite* pTemp = nullptr;
			for (int i = index+1; i < pObstacleSpriteArray->size(); ++i)
			{
				pTemp = pObstacleSpriteArray->at(i);
				if (pTemp->getSelfUnit()->type == unittype_Land || pTemp->getSelfUnit()->ID == unitID_TaXian)
				{
					//两个路面之间间隔太大的情况下不检测自动上下
					float dsY = fabs(pTemp->getCollisonRectInWorldSpace().getMaxY() - pObstacleSpriteArray->at(index)->getCollisonRectInWorldSpace().getMaxY());
					if (dsY > Land_Default_PosY+m_heroRect.size.height)
					{
						continue;
					}
					//冲刺状态自动上检测：下一个比当前面路面高，且距离小于等于80时，开始上跳
					if (pTemp->getCollisonRectInWorldSpace().getMaxY() > pObstacleSpriteArray->at(index)->getCollisonRectInWorldSpace().getMaxY())
					{
						float dsX = pTemp->getCollisonRectInWorldSpace().getMinX() - m_heroRect.getMaxX();
						if (dsX > 0 && dsX < upxOffset)
						{
							pHeroSprite->chongCiUp(pTemp);
							icount++;
							break;
						}
					}
					//冲刺状态自动上检测：下一个比当前面路面低，且距离小于等于20时，开始下跳
					else if (pTemp->getCollisonRectInWorldSpace().getMaxY() < pObstacleSpriteArray->at(index)->getCollisonRectInWorldSpace().getMaxY())
					{
						float dsX = pTemp->getCollisonRectInWorldSpace().getMinX() - m_heroRect.getMidX();
						if (dsX > -24 && dsX < downxoffset)
						{
							pHeroSprite->chongCiFall(pTemp);
							icount++;
							break;
						}
					}
				}
			}
		}
	}
}