#include "NetworkWaitingLayer.h"


NetworkWaitingLayer* NetworkWaitingLayer::create(bool bShowWaitingUI)
{
	NetworkWaitingLayer* layer = new NetworkWaitingLayer;
	if (layer && layer->init(bShowWaitingUI))
	{
		layer->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(layer);
		layer = nullptr;
	}
	return layer;
}
bool NetworkWaitingLayer::init(bool bShowWaitingUI)
{
	if (! LayerColor::initWithColor(Color4B(0,0,0,80))) return false;

	if (bShowWaitingUI)
	{
		auto s = Sprite::create("UI_biaoshi_zairu.png");
		this->addChild(s);
		s->setPosition(Director::getInstance()->getWinSize()/2);
		s->runAction(RepeatForever::create(RotateBy::create(1, 360)));
	}

	this->setLocalZOrder(ZORDER_WaitingLayer);
	this->setName(TAGName_WaitingLayer);

	return true;
}

void NetworkWaitingLayer::onEnter()
{
	LayerColor::onEnter();
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = std::bind(&NetworkWaitingLayer::onTouchBegan, this, std::placeholders::_1,std::placeholders::_2);
	listener->onTouchMoved = std::bind(&NetworkWaitingLayer::onTouchMoved, this, std::placeholders::_1,std::placeholders::_2);
	listener->onTouchEnded = std::bind(&NetworkWaitingLayer::onTouchEnded, this, std::placeholders::_1,std::placeholders::_2);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
void NetworkWaitingLayer::onExit()
{
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	LayerColor::onExit();
}

bool NetworkWaitingLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void NetworkWaitingLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
}
void NetworkWaitingLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}