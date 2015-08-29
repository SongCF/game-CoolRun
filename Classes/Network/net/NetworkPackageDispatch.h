#pragma once

#include "cocos2d.h"
#include "NetworkThreadProc.h"
using namespace cocos2d;

class NetworkPackageDispatch : public Node
{
private:
	static NetworkPackageDispatch* m_packageDispatch;
	NetworkPackageDispatch();
	void initNode();
public:
	static NetworkPackageDispatch* getInstance();
	~NetworkPackageDispatch();
	void setMsgProc(std::function<void(int, unsigned short, void*, unsigned int)> function);

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void update(float dt) override;
	void messageDispatch(RespondPackage* package);
	void onTimeOut(EventCustom* event);

private:
	std::function<void(int, unsigned short, void*, unsigned int)> m_msgProc;
};