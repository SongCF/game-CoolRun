#include "JniWrapper.h"
#include "SdkManager.h"
#include "RunningHelper.h"
#include "StateBar.h"
#include "StopLayer.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

//
JNIEXPORT void JNICALL Java_org_utils_JniWrapper_pauseGame(JNIEnv *env, jobject thiz)
{
	log("%s", __FUNCTION__);
	Director::getInstance()->stopAnimation();

	if (RunningHelper::getHelper()->isGameIn())
	{
		if (RunningHelper::getHelper()->getCurrentStateBar())
		{
			RunningHelper::getHelper()->getCurrentStateBar()->onPause(nullptr);
		}
	}
}
JNIEXPORT void JNICALL Java_org_utils_JniWrapper_resumeGame(JNIEnv *env, jobject thiz)
{
	log("%s", __FUNCTION__);
	Director::getInstance()->startAnimation();

	Node *pLayer = Director::getInstance()->getRunningScene()->getChildByName(TagName_StopLayer);
	if (pLayer)
	{
		StopLayer *pStop = dynamic_cast<StopLayer*>(pLayer);
		if (pStop)
		{
			pStop->onResume(nullptr);
		}
	}
}

//any sdk init
JNIEXPORT void JNICALL Java_org_utils_JniWrapper_nativeInitPlugins(JNIEnv *env, jobject thiz)
{
	log("%s", __FUNCTION__);
	SdkManager::getInstance()->initAnysdk();
}
JNIEXPORT void JNICALL Java_org_utils_JniWrapper_nativeUnLoadPlugins(JNIEnv *env, jobject thiz)
{
	log("%s", __FUNCTION__);
	SdkManager::getInstance()->destroyAnysdk();
}
JNIEXPORT void JNICALL Java_org_utils_JniWrapper_nativeDestroy(JNIEnv *env, jobject thiz)
{
	log("%s", __FUNCTION__);
	SdkManager::getInstance()->destroyAnysdk();
	SdkManager::getInstance()->destroy();
}


#endif  //#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
