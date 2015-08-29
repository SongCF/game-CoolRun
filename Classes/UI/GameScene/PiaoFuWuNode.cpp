#include "PiaoFuWuNode.h"
#include "ImageHelper.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

static const int X_OFFSET = 300;

PiaoFuWuNode::PiaoFuWuNode(void)
	: m_fCurMoveSpeed(0.0f)
	, m_fPreMoveSpeed(0.0f)
	, m_fDefaultMoveSpeed(0.0f)
	, m_bScroll(true)
{
}


PiaoFuWuNode::~PiaoFuWuNode(void)
{
	this->unscheduleUpdate();
}

PiaoFuWuNode* PiaoFuWuNode::createWithLayerInfo(const LayersBackground& LayersBackgroundInfo)
{
	PiaoFuWuNode* pNode = new PiaoFuWuNode();
	if (pNode && pNode->initWithInfo(LayersBackgroundInfo))
	{
		pNode->autorelease();
		return pNode;
	}
	CC_SAFE_DELETE(pNode);
	return pNode;
}

bool PiaoFuWuNode::initWithInfo(const LayersBackground& LayersBackgroundInfo)
{
	if (!Node::init() || LayersBackgroundInfo.layerList.size() == 0)
	{
		return false;
	}

	m_fDefaultMoveSpeed = m_fPreMoveSpeed = m_fCurMoveSpeed = LayersBackgroundInfo.speed;
	m_LayersBackgroundInfo = LayersBackgroundInfo;

	for (size_t i = 0; i < m_LayersBackgroundInfo.layerList.size(); i++)
	{
		addPiaoFuWuSprite(i);
	}
	Size size = Director::getInstance()->getWinSize();
	this->scheduleUpdate();
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->setPosition(size/2);
	return true;
}

Node* PiaoFuWuNode::addPiaoFuWuSprite(int iIndex)
{
	Size size = Director::getInstance()->getWinSize();
	int max = m_LayersBackgroundInfo.maxRandomWidth;
	int min = m_LayersBackgroundInfo.minRandomWidth;

	Node * spr = nullptr;
	if (m_LayersBackgroundInfo.layerList.at(iIndex).armatureName.empty()){
		spr = Sprite::create(m_LayersBackgroundInfo.layerList.at(iIndex).fileName.c_str());
	}
	else {
		Armature* ar = Armature::create(m_LayersBackgroundInfo.layerList.at(iIndex).armatureName);
		ar->getAnimation()->play(m_LayersBackgroundInfo.layerList.at(iIndex).animateName);
		spr = ar;
	}

	if (!spr)
	{
		return nullptr;
	}
	spr->setScale(m_LayersBackgroundInfo.Scale);
	this->addChild(spr);
	if (m_vPiaoFuWuSprite.size() == 0)
	{
		if (max == min)
		{
			spr->setPosition(Vec2(spr->getContentSize().width*spr->getScaleX()-size.width/2 - 180, m_LayersBackgroundInfo.layerList.at(iIndex).YOffset));
		}
		else
		{
			spr->setPosition(Vec2(spr->getContentSize().width*spr->getScaleX()+(rand()%(max- min) + min)-size.width/2-180, m_LayersBackgroundInfo.layerList.at(iIndex).YOffset));
		}
	}
	else
	{
		if (max == min)
		{
			spr->setPosition(Vec2(m_vPiaoFuWuSprite.at(m_vPiaoFuWuSprite.size()-1)->getPositionX() + m_vPiaoFuWuSprite.at(m_vPiaoFuWuSprite.size()-1)->getContentSize().width/2*m_vPiaoFuWuSprite.at(m_vPiaoFuWuSprite.size()-1)->getScaleX() + spr->getContentSize().width/2*spr->getScaleX(), 
				m_LayersBackgroundInfo.layerList.at(iIndex).YOffset));
		}
		else
		{
			spr->setPosition(Vec2(m_vPiaoFuWuSprite.at(m_vPiaoFuWuSprite.size()-1)->getPositionX() + m_vPiaoFuWuSprite.at(m_vPiaoFuWuSprite.size()-1)->getContentSize().width/2*m_vPiaoFuWuSprite.at(m_vPiaoFuWuSprite.size()-1)->getScaleX() + spr->getContentSize().width/2*spr->getScaleX() + (rand()%(max- min) + min), 
				m_LayersBackgroundInfo.layerList.at(iIndex).YOffset));
		}
	}
	m_vPiaoFuWuSprite.push_back(spr);

// 	spr->setOpacity(0.0f);
// 	spr->runAction(FadeIn::create(0.3f));

	return spr;
}

void PiaoFuWuNode::update(float dt)
{
	if (m_fCurMoveSpeed <= 0 || m_LayersBackgroundInfo.layerList.size() <= 0 || !m_bScroll)
	{
		return;
	}

	for (size_t i = 0; i < m_vPiaoFuWuSprite.size(); ++i)
	{
		Node* pSpr = m_vPiaoFuWuSprite.at(i);
		pSpr->setPositionX(pSpr->getPositionX()-m_fCurMoveSpeed);
	}

//	this->setPositionX(this->getPositionX()-m_fCurMoveSpeed);

	removeUnSeeSpr();
}

void PiaoFuWuNode::removeUnSeeSpr()
{
	if (m_vPiaoFuWuSprite.size() == 0)
	{
		return;
	}

	for (size_t i = 0; i < m_vPiaoFuWuSprite.size(); ++i)
	{
		Node* pSpr = m_vPiaoFuWuSprite.at(i);
		if (pSpr)
		{
			float x = this->convertToWorldSpace(pSpr->getPosition()).x;
			if (x <= -X_OFFSET-(pSpr->getContentSize().width*pSpr->getScaleX()))
			{
				pSpr->removeFromParent();
				m_vPiaoFuWuSprite.erase(m_vPiaoFuWuSprite.begin()+i);

				int count = m_LayersBackgroundInfo.layerList.size();
				if (count > 0)
				{
					addPiaoFuWuSprite(rand()%count);
				}
			}
			break;
		}
	}
}

