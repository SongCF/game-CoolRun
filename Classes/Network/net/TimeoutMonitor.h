#pragma once
#include "cocos2d.h"
#include "NetworkThreadProc.h"

using namespace cocos2d;

// 会发送这个消息通知已经超时了
#define  MSG_WAIT_TIME_OUT	"WAIT_TIME_OUT"

// 最多等20秒钟
#define		MAX_TIME_WAIT	20.0f

class TimeoutMonitor;
static TimeoutMonitor* mTimeMonitor = nullptr;
class TimeoutMonitor : public Node
{
private:
	TimeoutMonitor()
	{
		this->init();
		this->onEnter();
		this->onEnterTransitionDidFinish();
	}
public:
	static TimeoutMonitor* getInstance()
	{
		if (mTimeMonitor == nullptr)
		{
			//MessageBox("new time monitor", "");
			mTimeMonitor = new TimeoutMonitor();
		}
		return mTimeMonitor;
	}

	// 开始监视，超时之后自动断连接
	void startMonitor(float time_wait = MAX_TIME_WAIT)
	{
		CCLOG("%s:time_wait = %f",__FUNCTION__,time_wait);
		unschedule(schedule_selector(TimeoutMonitor::timeOutCallback));
		schedule(schedule_selector(TimeoutMonitor::timeOutCallback),time_wait);
	}

	void cancleMonitor()
	{
		CCLOG("%s",__FUNCTION__);
		unschedule(schedule_selector(TimeoutMonitor::timeOutCallback));
	}

	void timeOutCallback(float t)
	{
		CCLOG("TimeoutMonitor::---------------TimeOut-----------");
		unschedule(schedule_selector(TimeoutMonitor::timeOutCallback));
		NetworkThreadProc::getInstance()->closeNetwork();

		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(MSG_WAIT_TIME_OUT, nullptr);
//		NotificationCenter::sharedNotificationCenter()->postNotification(MSG_WAIT_TIME_OUT,NULL);
	}
};
