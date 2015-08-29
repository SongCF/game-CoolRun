#include "ScrollForeverNode.h"
#include "ImageHelper.h"
#define X_OFFSET   600

ScrollForeverNode::ScrollForeverNode(void)
	: m_iImageVecCount(0)
	, m_pFirstSprite(nullptr)
	, m_pSecondSprite(nullptr)
	, m_pThirdSprite(nullptr)
	, m_fMoveSpeed(0.0f)
	, m_bScroll(true)
{
}


ScrollForeverNode::~ScrollForeverNode(void)
{
}

ScrollForeverNode* ScrollForeverNode::createWithInfo(const Background& info)
{
	ScrollForeverNode* pNode = new ScrollForeverNode();
	if (pNode && pNode->initWithInfo(info))
	{
		pNode->autorelease();
		return pNode;
	}
	CC_SAFE_DELETE(pNode);
	return pNode;
}

bool ScrollForeverNode::initWithInfo(const Background& info)
{
	if (!Node::init() || info.mapList.size() == 0)
	{
		return false;
	}
	m_fDefaultMoveSpeed = m_fMoveSpeed = info.speed;
	m_iImageVecCount = info.mapList.size();
	m_ImageVec = info.mapList;
	m_pFirstSprite = ImageHelper::getBackgroundRunningScene(info.mapList.at(0));
	m_pFirstSprite->setScale(2.0f);
	m_pFirstSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pFirstSprite->setPosition(Vec2(-m_pFirstSprite->getContentSize().width/2*m_pFirstSprite->getScaleX(), 0));
	m_pFirstSprite->setTag(0);

	this->addChild(m_pFirstSprite);

//	this->setContentSize(m_pFirstSprite->getContentSize());

	if (m_iImageVecCount == 1)
	{
		m_pSecondSprite = ImageHelper::getBackgroundRunningScene(info.mapList.at(0));
		m_pThirdSprite = ImageHelper::getBackgroundRunningScene(info.mapList.at(0));
	}
	else if (m_iImageVecCount == 2)
	{
		m_pSecondSprite = ImageHelper::getBackgroundRunningScene(info.mapList.at(1));
		m_pThirdSprite = ImageHelper::getBackgroundRunningScene(info.mapList.at(0));
	}
	else
	{
		m_pSecondSprite = ImageHelper::getBackgroundRunningScene(info.mapList.at(1));
		m_pThirdSprite = ImageHelper::getBackgroundRunningScene(info.mapList.at(2));
	}
	m_pSecondSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pSecondSprite->setPosition(Vec2(m_pFirstSprite->getPositionX()+m_pFirstSprite->getContentSize().width*m_pFirstSprite->getScaleX()-1, 0));
	m_pSecondSprite->setTag(1);
	m_pSecondSprite->setScale(2.0f);
	this->addChild(m_pSecondSprite);

	m_pThirdSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pThirdSprite->setPosition(Vec2(m_pSecondSprite->getPositionX()+m_pSecondSprite->getContentSize().width*m_pSecondSprite->getScaleX()-1, 0));
	m_pThirdSprite->setTag(2);
	m_pThirdSprite->setScale(2.0f);
	this->addChild(m_pThirdSprite);

	this->scheduleUpdate();

	return true;
}

void ScrollForeverNode::update(float dt)
{
	if (m_fMoveSpeed <= 0 || m_iImageVecCount <= 0 || !m_bScroll)
	{
		return;
	}

	if (m_pFirstSprite && m_pSecondSprite)
	{
		m_pFirstSprite->setPositionX(m_pFirstSprite->getPositionX()-m_fMoveSpeed);
		m_pSecondSprite->setPositionX(m_pSecondSprite->getPositionX()-m_fMoveSpeed);
		m_pThirdSprite->setPositionX(m_pThirdSprite->getPositionX()-m_fMoveSpeed);
		if (m_pFirstSprite->getPositionX() < -m_pFirstSprite->getContentSize().width*m_pFirstSprite->getScaleX()-Director::getInstance()->getWinSize().width/2 - X_OFFSET)
 		{
			changeDisplayFrame(m_pFirstSprite, m_pThirdSprite);
		}
		if (m_pSecondSprite->getPositionX() < -m_pSecondSprite->getContentSize().width*m_pSecondSprite->getScaleX()-Director::getInstance()->getWinSize().width/2 - X_OFFSET)
		{
			changeDisplayFrame(m_pSecondSprite, m_pFirstSprite);
		}
		if (m_pThirdSprite->getPositionX() < -m_pThirdSprite->getContentSize().width*m_pThirdSprite->getScaleX()-Director::getInstance()->getWinSize().width/2 - X_OFFSET)
		{
			changeDisplayFrame(m_pThirdSprite, m_pSecondSprite);
		}
	}
}

void ScrollForeverNode::refreshImagVec(const Background& info, EventCustom* pMsg)
{
	float dt = 0;
	if (pMsg)
	{
		dt = ((int)(long long)pMsg->getUserData())/1000.0f;
	}
	float dt1 = 0.0f;
	if (dt > 0)
	{
		dt1 = 0.001f;
	}
	m_ImageVec = info.mapList;
	m_iImageVecCount = m_ImageVec.size();
	m_fMoveSpeed = info.speed;
	m_pFirstSprite->runAction(Sequence::create(FadeOut::create(dt), CallFunc::create(CC_CALLBACK_0(ScrollForeverNode::updateTexture1, this)), FadeIn::create(dt), nullptr));
	m_pSecondSprite->runAction(Sequence::create(FadeOut::create(dt), DelayTime::create(dt1), CallFunc::create(CC_CALLBACK_0(ScrollForeverNode::updateTexture2, this)), FadeIn::create(dt), nullptr));
	m_pThirdSprite->runAction(Sequence::create(FadeOut::create(dt), DelayTime::create(dt1), CallFunc::create(CC_CALLBACK_0(ScrollForeverNode::updateTexture3, this)), FadeIn::create(dt), nullptr));
}

void ScrollForeverNode::updateTexture1()
{
	auto p = ImageHelper::getBackgroundRunningScene(m_ImageVec.at(0).c_str());
	m_pFirstSprite->setSpriteFrame(SpriteFrame::createWithTexture(p->getTexture(), p->getTextureRect()));
	m_pFirstSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pFirstSprite->setScaleX(2.0f);
	m_pFirstSprite->setTag(0);
	m_pFirstSprite->setPosition(Vec2(-m_pFirstSprite->getContentSize().width/2*m_pFirstSprite->getScaleX(), 0));
}

void ScrollForeverNode::updateTexture2()
{
	BaseSprite* p = nullptr;
	if (m_iImageVecCount == 1)
	{
		p = ImageHelper::getBackgroundRunningScene(m_ImageVec.at(0).c_str());
	}
	else 
	{
		p = ImageHelper::getBackgroundRunningScene(m_ImageVec.at(1).c_str());
	}
	m_pSecondSprite->setSpriteFrame(SpriteFrame::createWithTexture(p->getTexture(), p->getTextureRect()));
	m_pSecondSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pSecondSprite->setScaleX(2.0f);
	m_pSecondSprite->setTag(1);
	m_pSecondSprite->setPosition(Vec2(m_pFirstSprite->getPositionX()+m_pFirstSprite->getContentSize().width*m_pFirstSprite->getScaleX()-1, 0));
}

void ScrollForeverNode::updateTexture3()
{
	BaseSprite* p = nullptr;
	if (m_iImageVecCount == 1)
	{
		p = ImageHelper::getBackgroundRunningScene(m_ImageVec.at(0).c_str());
	}
	else if (m_iImageVecCount == 2)
	{
		p = ImageHelper::getBackgroundRunningScene(m_ImageVec.at(0).c_str());
	}
	else
	{
		p = ImageHelper::getBackgroundRunningScene(m_ImageVec.at(2).c_str());
	}
	m_pThirdSprite->setSpriteFrame(SpriteFrame::createWithTexture(p->getTexture(), p->getTextureRect()));
	m_pThirdSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pThirdSprite->setScaleX(2.0f);
	m_pThirdSprite->setTag(2);
	m_pThirdSprite->setPosition(Vec2(m_pSecondSprite->getPositionX()+m_pSecondSprite->getContentSize().width*m_pSecondSprite->getScaleX()-1, 0));
}

void ScrollForeverNode::changeDisplayFrame(Sprite* curSprite, Sprite* preSprite)
{
	if (!curSprite || ! preSprite)
	{
		return;
	}

	int index = 0;
	BaseSprite* p = nullptr;
	if (m_iImageVecCount == 1)
	{
		index = 0;
	}
	else
	{
		index = (preSprite->getTag()+1)%m_iImageVecCount;
	}
	p = ImageHelper::getBackgroundRunningScene(m_ImageVec.at(index).c_str());
	if (p)
	{
		curSprite->setSpriteFrame(SpriteFrame::createWithTexture(p->getTexture(), p->getTextureRect()));
		curSprite->setTag(index);
		log("----------cur png is %s-------------", m_ImageVec.at(index).c_str());
		curSprite->setPositionX(preSprite->getPositionX()+preSprite->getContentSize().width*preSprite->getScaleX()-1);
	}
	else
	{
		CCASSERT(false, "");
		curSprite->setPositionX(preSprite->getPositionX()+preSprite->getContentSize().width*preSprite->getScaleX()-1);
	}
}



