
#include "SdkManager.h"
#include "MsgDefine.h"
#include "GameDirector.h"

SdkManager* SdkManager::_sdkManager = nullptr;

SdkManager* SdkManager::getInstance()
{
	if (!_sdkManager) _sdkManager = new SdkManager();
	return _sdkManager;
}
void SdkManager::destroy()
{
    if (_sdkManager)
    {
        delete _sdkManager;
        _sdkManager = NULL;
    }
}

//init
SdkManager::SdkManager()
{
}
void SdkManager::initAnysdk()
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	std::string appKey = "EB5933F0-0095-2B08-D5A1-65B3874E088C";
	std::string appSecret = "e30eb63b8f138e20d53b0ed5713bccfe";
	std::string privateKey = "ADDA318E734BB175CF2EDECA3FDED8AE";
	std::string oauthLoginServer =
		"http://oauth.anysdk.com/api/OauthLoginDemo/Login.php";

	AgentManager::getInstance()->init(appKey, appSecret, privateKey, oauthLoginServer);
	AgentManager::getInstance()->loadALLPlugin();

	//对支付系统设置监听类
	CCLOG("Load plugins invoked");
	m_pluginsIAPMap = AgentManager::getInstance()->getIAPPlugin();
	std::map<std::string, ProtocolIAP*>::iterator iter;
	for (iter = m_pluginsIAPMap->begin(); iter != m_pluginsIAPMap->end(); iter++) {
		//(iter->second)->setDebugMode(true);
		(iter->second)->setResultListener(this);
	}

// 	//分享系统
// 	ProtocolShare* pSharePlugin = AgentManager::getInstance()->getSharePlugin();
// 	if (pSharePlugin) pSharePlugin->setResultListener(this);

	AgentManager::getInstance()->getAnalyticsPlugin()->setCaptureUncaughtException(true);
	AgentManager::getInstance()->getAnalyticsPlugin()->setSessionContinueMillis(15000);
	AgentManager::getInstance()->getAnalyticsPlugin()->logTimedEventBegin(analyticsEVENT_Load);
#endif
}
void SdkManager::destroyAnysdk()
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	CCLOG("Unload plugins invoked");
	AgentManager::getInstance()->getAnalyticsPlugin()->logTimedEventEnd(analyticsEVENT_Unload);
	AgentManager::getInstance()->unloadALLPlugin();
#endif
}

//pay
static int s_curPayProductID = 0;
void SdkManager::pay(ProductID productID)
{
	s_curPayProductID = productID;
	GameDirector::getDirector()->showWaiting();//

#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	std::map<std::string, std::string> productInfo;
	if(m_pluginsIAPMap)
	{
		const ProductInfo* pInfo = ProductInfoHelper::getHelper()->getOneProductInfo(productID);

		if (! pInfo){
			EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
			EventCustom event(MSG_PayResult);
			MsgData data;
			data.errorCode = Rsp_Error;
			data.pData = nullptr;
			event.setUserData(&data);
			eventDspc->dispatchEvent(&event);
			log("config Product.txt error!");
			return;
		}

		//此处map的key是固定的。pref: http://docs.anysdk.com/IAPSystem
		string productId = this->getChannelID();
		for (map<string,string>::const_iterator it=pInfo->channelProduct.begin(); it!=pInfo->channelProduct.end(); ++it)
		{
			if (productId == it->first){ //找到对应的channelID
				productId = it->second;
				break;
			}
		}
		productInfo["Product_Id"] = productId;//pInfo->channelProduct[channelId];
		productInfo["Product_Price"] = String::createWithFormat("%d", pInfo->price)->getCString();
		productInfo["Product_Name"] = pInfo->describe.c_str();
		productInfo["Server_Id"] = "1";//服务器id，若无填“1”
		productInfo["Product_Count"] = "1";//商品份数(除非游戏需要支持一次购买多份商品，否则传1即可)
		//注意：调用支付函数时需要传入的一些玩家信息参数(如角色名称，ID，等级)都是渠道强制需求(如UC,小米)，并非AnySDK收集所用，如果开发者不填或者填假数据都会导致渠道上架无法通过。
		productInfo["Role_Id"] = "1";
		productInfo["Role_Name"] = "1";

		std::map<std::string , ProtocolIAP*>::iterator it = m_pluginsIAPMap->begin();
		if(m_pluginsIAPMap->size() == 1)
		{
			//(it->second)->setDebugMode(true);
			(it->second)->payForProduct(productInfo);
		}
		else 
		{
			MessageBox("this package has more than one channel !", "error");
			return;
		}

	}
#endif
}
void SdkManager::onPayResult(PayResultCode ret, const char* msg, TProductInfo info)
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	log("%s ---> pay msg: %s", __FUNCTION__, msg);
	std::string temp = "fail";

	int errCode = Rsp_Error;
	string payRetInfo;
	switch(ret)
	{
	case kPaySuccess://支付成功回调
		temp = "Success";
		MessageBox(temp.c_str() , temp.c_str() );
		errCode = Rsp_Success;
		payRetInfo = "Success";
		break;
	case kPayFail://支付失败回调
		MessageBox(temp.c_str()  , temp.c_str() );
		errCode = Rsp_Error;
		payRetInfo = "Error";
		break;
	case kPayCancel://支付取消回调
		MessageBox(temp.c_str()  , "Cancel" );
		errCode = Rsp_Cancle;
		payRetInfo = "Cancel";
		break;
	case kPayNetworkError://支付网络出现错误
		MessageBox(temp.c_str()  , "NetworkError");
		errCode = Rsp_NetWorkError;
		payRetInfo = "NetworkError";
		break;
	case kPayProductionInforIncomplete://支付信息提供不完全
		MessageBox(temp.c_str()  , "ProductionInforIncomplete");
		errCode = Rsp_Error;
		payRetInfo = "ProductionInforIncomplete";
		break;
		/**
		* 新增加:正在进行中回调
		* 支付过程中若SDK没有回调结果，就认为支付正在进行中
		* 游戏开发商可让玩家去判断是否需要等待，若不等待则进行下一次的支付
		*/
	case kPayNowPaying:
		//ShowTipDialog();
		errCode = Rsp_TimeOut;
		payRetInfo = "Rsp_TimeOut";
		break;
	default:
		errCode = Rsp_Error;
		payRetInfo = "Rsp_Error";
		break;
	}

	time_t curTime = time(0);
	struct tm *curTm = localtime(&curTime);//时区时间， 非时区时间使用struct tm *gmtime(const time_t *timep); 
	char * timeStr = asctime(curTm);
	info.insert(std::make_pair("PayResult", payRetInfo.c_str()));
	info.insert(std::make_pair("PayTime", timeStr));
	this->logEvent(analyticsEVENT_Pay, &info);

	int proId = s_curPayProductID;
// 	Director::getInstance()->getRunningScene()->runAction(Sequence::create(
// 		DelayTime::create(1),
// 		CallFunc::create([=](){
			log("SdkManager ---> dispatch MSG_PayResult, errCode = %d", errCode);
			EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
			EventCustom event(MSG_PayResult);
			MsgData data;
			data.errorCode = errCode;
			data.value = proId;
			event.setUserData(&data);
			eventDspc->dispatchEvent(&event);
// 	}),
// 		nullptr));

#endif
}

//analytics
void SdkManager::logEvent(string eventID, std::map<std::string, std::string>* eventMap)
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	AgentManager::getInstance()->getAnalyticsPlugin()->logEvent(eventID.c_str(), eventMap);
#endif
}

string SdkManager::getChannelID()
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	return AgentManager::getInstance()->getChannelId();
#endif

	return "";
}

//share
void SdkManager::share(std::map<std::string, std::string>* shareInfo)
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
// 	if (shareInfo){
// 		//AgentManager::getInstance()->getSharePlugin()->setResultListener(this); --> 在init中初始化
// 		ProtocolShare* pSharePlugin = AgentManager::getInstance()->getSharePlugin();
// 		if (pSharePlugin) pSharePlugin->share(*shareInfo);
// 	}
#if 0
	Dictionary *content = Dictionary::create();
	//Dictionary可用的Key如下，如果需要用到其它字段，可自行参考Sample中的代码实现：
	// (并不是所有平台都有这些字段，需要参考文档http://wiki.mob.com/ 以下都是预定义的内容
	content -> setObject(String::create(FX::StringsMap::getValue("content")), "content"); //要分享的内容，注意在文档中content对应的是text字段 
	content -> setObject(String::create(FX::StringsMap::getValue("image")), "image"); //可以是本地路径（如：/sdcard/a.jpg）或是一个URL
	content -> setObject(String::create(FX::StringsMap::getValue("title")), "title");
	content -> setObject(String::create(FX::StringsMap::getValue("description")), "description");
	content -> setObject(String::create(FX::StringsMap::getValue("url")), "url");
	content -> setObject(String::createWithFormat("%d", C2DXContentTypeNews), "type");
	content -> setObject(String::create(FX::StringsMap::getValue("siteUrl")), "siteUrl");
	content -> setObject(String::create("ShareSDK"), "site");
	content -> setObject(String::create(FX::StringsMap::getValue("musicUrl")), "musicUrl");
	//content -> setObject(String::create("extInfo"), "extInfo"); //微信分享应用时传给应用的附加信息
	C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, [](C2DXResponseState state, C2DXPlatType platType, Dictionary *shareInfo, Dictionary *error){
		switch (state) {
		case C2DXResponseStateSuccess:
			C2DXShareSDK::toast(FX::StringsMap::getValue("shareCg").c_str);
			break;
		case C2DXResponseStateFail:
			C2DXShareSDK::toast(FX::StringsMap::getValue("shareSb").c_str);
			break;
		default:
			C2DXShareSDK::toast(FX::StringsMap::getValue("shareQx").c_str);
			break;
		}
	});
#endif
#endif
}

void SdkManager::onShareResult(ShareResultCode ret, const char* msg)
{
	switch(ret)
	{
	case kShareSuccess://分享成功回调
		MessageBox(msg, "kShareSuccess");
		break;
	case kShareFail://分享失败回调
		MessageBox(msg, "kShareFail");
		break;
	case kShareCancel://分享取消回调
		MessageBox(msg, "kShareCancel");
		break;
	case kShareNetworkError://分享网络出错回调
		MessageBox(msg, "kShareNetworkError");
		break;
	default:
		MessageBox(msg, "Error");
		break;
	}
}



void SdkManager::initSharesdk()
{
#if 0
	//初始化ShareSDK
	C2DXShareSDK::open("52192f5fcf20", false);
	//初始化社交平台信息

	//Facebook  注册应用名为 Coolrun 
	Dictionary *fbConfigDict = Dictionary::create();
	fbConfigDict -> setObject(String::create("399896833501594"), "api_key");
	fbConfigDict -> setObject(String::create("20afccb3037d51bf6781ded7799bb17c"), "app_secret");
	C2DXShareSDK::setPlatformConfig(C2DXPlatTypeFacebook, fbConfigDict);

	C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSMS, NULL);
	/*
	//Twitter//还未注册
	Dictionary *twConfigDict = Dictionary::create();
	twConfigDict -> setObject(String::create("mnTGqtXk0TYMXYTN7qUxg"), "consumer_key");
	twConfigDict -> setObject(String::create("ROkFqr8c3m1HXqS3rm3TJ0WkAJuwBOSaWhPbZ9Ojuc"), "consumer_secret");
	twConfigDict -> setObject(String::create("http://www.sharesdk.cn"), "redirect_uri");
	C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTwitter, twConfigDict);
	*/
#endif
}
