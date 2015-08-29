
#pragma once
#include "cocos2d.h"
using namespace cocos2d;
//jni  callback
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>


//c++ to java


// java to c++
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_org_utils_JniWrapper_pauseGame(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_org_utils_JniWrapper_resumeGame(JNIEnv *env, jobject thiz);

JNIEXPORT void JNICALL Java_org_utils_JniWrapper_nativeInitPlugins(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_org_utils_JniWrapper_nativeUnLoadPlugins(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_org_utils_JniWrapper_nativeDestroy(JNIEnv *env, jobject thiz);

#ifdef __cplusplus
}
#endif



#endif  //#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
