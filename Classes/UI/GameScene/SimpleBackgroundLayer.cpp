#include "SimpleBackgroundLayer.h"
#include "ScrollForeverNode.h"
#include "RunningHelper.h"
#include "ImageHelper.h"
#include "PiaoFuWuNode.h"

using namespace std;

SimpleBackgroundLayer::SimpleBackgroundLayer()
	: m_pInfo(nullptr)
	, m_iRunTime(0)
	, m_refreshListener(nullptr)
{
	
}

SimpleBackgroundLayer::~SimpleBackgroundLayer()
{
	_eventDispatcher->removeEventListener(m_refreshListener);
	this->unschedule(schedule_selector(SimpleBackgroundLayer::update));
}

bool SimpleBackgroundLayer::init()
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

	m_refreshListener = _eventDispatcher->addCustomEventListener(MSG_RefreshBackground, 
		[&](EventCustom* pMsg){this->updateBackGround(pMsg);});
	this->scheduleUpdate();

	return true;
}

void SimpleBackgroundLayer::updateBackGround(EventCustom* pMsg)
{
	refreshBackGround(pMsg, false);
}

void SimpleBackgroundLayer::refreshBackGround(EventCustom* pMsg, bool bFisrt /*= false*/)
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
		CCASSERT(false, "error backgroundInfo!");
	}

	for (size_t i = 0; i < m_pInfo->backList.size(); i++)
	{
		ScrollForeverNode* pNode = (ScrollForeverNode*)this->getChildByTag(iTag_ScrollNode1+i);
		if (pNode)
		{
			pNode->refreshImagVec(m_pInfo->backList.at(i), pMsg);
		}
		else
		{
			pNode = ScrollForeverNode::createWithInfo(m_pInfo->backList.at(i));
			this->addChild(pNode, m_pInfo->backList.at(i).zOrder, iTag_ScrollNode1+i);
			pNode->setPosition(Vec2(m_Winsize.width/2 - 300, m_Winsize.height/2 + m_pInfo->backList.at(i).YOffset));
		}
	}

// 	ScrollForeverNode* pNode1 = (ScrollForeverNode*)this->getChildByTag(iTag_ScrollNode1);
// 	if (pNode1)
// 	{
// 		pNode1->refreshImagVec(m_pInfo->backList.at(0).mapList, m_pInfo->backList.at(0).speed, pMsg);
// 	}
// 	else
// 	{
// 		pNode1 = ScrollForeverNode::createWithImagVec(m_pInfo->backList.at(0).mapList, m_pInfo->backList.at(0).speed);
// 		this->addChild(pNode1, m_pInfo->backList.at(0).zOrder, iTag_ScrollNode1);
// 		pNode1->setPosition(m_Winsize/2);
// 	}
// 
// 	ScrollForeverNode* pZhongJingNode = (ScrollForeverNode*)this->getChildByTag(iTag_ScrollNode2);
// 	if (pZhongJingNode)
// 	{
// 		pZhongJingNode->refreshImagVec(m_pInfo->backList.at(1).mapList, m_pInfo->backList.at(1).speed, pMsg);
// 	}
// 	else
// 	{
// 		pZhongJingNode = ScrollForeverNode::createWithImagVec(m_pInfo->backList.at(1).mapList, m_pInfo->backList.at(1).speed);
// 		this->addChild(pZhongJingNode, m_pInfo->backList.at(1).zOrder, iTag_ScrollNode2);
// 		pZhongJingNode->setPosition(m_Winsize/2);
// 	}
// //	pZhongJingNode->setVisible(false);

// 	ScrollForeverNode* pJinJingNode = (ScrollForeverNode*)this->getChildByTag(iTag_ScrollNode3);
// 	if (pJinJingNode)
// 	{
// 		pJinJingNode->refreshImagVec(m_pInfo->backList.at(2).mapList, m_pInfo->backList.at(2).speed, pMsg);
// 	}
// 	else
// 	{
// 		pJinJingNode = ScrollForeverNode::createWithImagVec(m_pInfo->backList.at(2).mapList,m_pInfo->backList.at(2).speed);
// 		this->addChild(pJinJingNode, 0, iTag_ScrollNode3);
// 		pJinJingNode->setPosition(m_Winsize.width/2, m_Winsize.height/2-70);
// 	}
////	pJinJingNode->setVisible(false);

	PiaoFuWuNode* pNode = nullptr;
	for (size_t i = 0; i < m_pInfo->scene.layersList.size(); ++i)
	{
		pNode = PiaoFuWuNode::createWithLayerInfo(m_pInfo->scene.layersList.at(i));
		if (pNode)
		{
			this->addChild(pNode, m_pInfo->scene.layersList.at(i).zOrder, iTag_PiaoFuWuNodeStart+i);
		}
	}

}

void SimpleBackgroundLayer::setScrollEnable(bool e)
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

void SimpleBackgroundLayer::update(float dt)
{
 	++m_iRunTime;
}

ScrollForeverNode* SimpleBackgroundLayer::getBGLayer_JinJing()
{
	return (ScrollForeverNode*)this->getChildByTag(iTag_ScrollNode3);
}
ScrollForeverNode* SimpleBackgroundLayer::getBGLayer_ZhongJing()
{
	return (ScrollForeverNode*)this->getChildByTag(iTag_ScrollNode2);
}
ScrollForeverNode* SimpleBackgroundLayer::getBGLayer_YuanJing()
{
	return (ScrollForeverNode*)this->getChildByTag(iTag_ScrollNode1);
}