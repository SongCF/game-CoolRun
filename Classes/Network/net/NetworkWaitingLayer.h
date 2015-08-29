#pragma once

#include "cocos2d.h"

using namespace cocos2d;

#define TAGName_WaitingLayer "network_waiting_layer"
#define ZORDER_WaitingLayer 3069

class NetworkWaitingLayer : public LayerColor
{
public:
	static NetworkWaitingLayer* create(bool bShowWaitingUI=true);
	bool init(bool bShowWaitingUI);

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
	virtual void onTouchMoved(Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
};