
#pragma once

#include "ProductInfoHelper.h"
#include "cocos2d.h"
#include <string>
#include <map>
using namespace cocos2d;
using std::string;
using std::map;


//event
#define analyticsEVENT_Load			"Load"
#define analyticsEVENT_Unload		"UnLoad"
#define analyticsEVENT_LogIn		"LogIn"
#define analyticsEVENT_Pay			"Pay"
#define analyticsEVENT_Achive		"Achive"
#define analyticsEVENT_Role			"Role"
#define analyticsEVENT_Run			"Run"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#endif

#include "anysdk/AgentManager.h"
using namespace anysdk::framework;




class SdkManager : public PayResultListener, public ShareResultListener
{
private:
	SdkManager();
	static SdkManager* _sdkManager;
public:
	static SdkManager* getInstance();
	void destroy();

	//int sdk
	void initAnysdk();
	void destroyAnysdk();
	void initSharesdk();

	//pay
	void pay(ProductID productID);
	virtual void onPayResult(PayResultCode ret, const char* msg, TProductInfo info);

	//analytics
	void logEvent(string eventID, std::map<std::string, std::string>* eventMap = nullptr);

	//share
	void share(std::map<std::string, std::string>* shareInfo);
	virtual void onShareResult(ShareResultCode ret, const char* msg);

	//
	string getChannelID();

private:
	std::map<std::string , ProtocolIAP*>*  m_pluginsIAPMap;
};



