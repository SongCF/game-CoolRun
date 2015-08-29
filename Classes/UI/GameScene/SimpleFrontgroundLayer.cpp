#include "SimpleFrontgroundLayer.h"
#include "ScrollForeverNode.h"
#include "RunningHelper.h"
#include "ImageHelper.h"
#include "PiaoFuWuNode.h"

using namespace std;

SimpleFrontgroundLayer::SimpleFrontgroundLayer()
	: m_pInfo(nullptr)
	, m_iRunTime(0)
	, m_refreshListener(nullptr)
{
	
}

SimpleFrontgroundLayer::~SimpleFrontgroundLayer()
{
	_eventDispatcher->removeEventListener(m_refreshListener);
	this->unschedule(schedule_selector(SimpleFrontgroundLayer::update));
}

bool SimpleFrontgroundLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_iRunTime = 0;
	m_Winsize = Director::getInstance()->getWinSize();

	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vec2(0.5f,0.5f));
	this->setPosition(m_Winsize/2);

	refreshBackGround(nullptr, true);

	m_refreshListener = _eventDispatcher->addCustomEventListener(MSG_RefreshBackground, [&](EventCustom* pMsg){this->updateBackGround(pMsg);});
	this->scheduleUpdate();

	return true;
}

void SimpleFrontgroundLayer::updateBackGround(EventCustom* pMsg)
{
	refreshBackGround(pMsg, false);
}

void SimpleFrontgroundLayer::refreshBackGround(EventCustom* pMsg, bool bFisrt /*= false*/)
{
	this->removeAllChildren();
	if (bFisrt)
	{
		m_pInfo = const_cast<BackgroundList*>(RunningHelper::getHelper()->getFirstBackground());
	}
	else
	{
		m_pInfo = const_cast<BackgroundList*>(RunningHelper::getHelper()->getCurrentBackground());
	}

	if (!m_pInfo)
	{
		CCASSERT(false, "error frontgroundInfo!");
	}

	PiaoFuWuNode* pNode = nullptr;
	for (size_t i = 0; i < m_pInfo->table.layersList.size(); ++i)
	{
		pNode = PiaoFuWuNode::createWithLayerInfo(m_pInfo->table.layersList.at(i));
		if (pNode)
		{
			this->addChild(pNode, m_pInfo->table.layersList.at(i).zOrder, iTag_PiaoFuWuNodeStart+i);
		}
	}

}

void SimpleFrontgroundLayer::setScrollEnable(bool e)
{
	int i = 0;
	while (ScrollForeverNode* pNode = dynamic_cast<ScrollForeverNode*>(this->getChildByTag(iTag_ScrollNode1+i)))
	{
		pNode->setScrollEnable(e);
		++i;
	}

	i = 0;
	while (PiaoFuWuNode* pNode = dynamic_cast<PiaoFuWuNode*>(this->getChildByTag(iTag_PiaoFuWuNodeStart+i)))
	{
		pNode->setScrollEnable(e);
		++i;
	}
}

void SimpleFrontgroundLayer::update(float dt)
{
 	++m_iRunTime;
}

ScrollForeverNode* SimpleFrontgroundLayer::getBGLayer_JinJing()
{
	return (ScrollForeverNode*)this->getChildByTag(iTag_ScrollNode3);
}
ScrollForeverNode* SimpleFrontgroundLayer::getBGLayer_ZhongJing()
{
	return (ScrollForeverNode*)this->getChildByTag(iTag_ScrollNode2);
}
ScrollForeverNode* SimpleFrontgroundLayer::getBGLayer_YuanJing()
{
	return (ScrollForeverNode*)this->getChildByTag(iTag_ScrollNode1);
}