#include "AppDelegate.h"
#include "Preload.h"
#include "FirstLayer.h"
#include "Music.h"
#include "RunningHelper.h"
#include "StateBar.h"
#include "VersionConfig.h"
//#include "sdk/SdkManager.h"
//#include "GameDirector.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#pragma comment(lib, "libprotobuf-lite")
#endif

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {

	CCLOG("%s ---> launching", __FUNCTION__);
	srand(time(0));

//	SdkManager::getInstance()->initSharesdk();
//	Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("The Smurfs Run");
		director->setOpenGLView(glview);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		glview->setFrameSize(1136, 640);
#endif
    }
//	director->getTextureCache()->setImageScale(1.f);
    glview->setDesignResolutionSize(960, 640, ResolutionPolicy::FIXED_HEIGHT);
    
    //search path
	{
		FileUtils::getInstance()->addSearchPath("ipad");
		FileUtils::getInstance()->addSearchPath("ipad/sence");
		FileUtils::getInstance()->addSearchPath("ipad/ui");
		FileUtils::getInstance()->addSearchPath("ipad/particle");
		FileUtils::getInstance()->addSearchPath("ipad/armature");
		FileUtils::getInstance()->addSearchPath("ipad/animation");

		FileUtils::getInstance()->addSearchPath("ipad/sence/scene1");
		FileUtils::getInstance()->addSearchPath("ipad/sence/scene2");
		FileUtils::getInstance()->addSearchPath("ipad/sence/scene3");
		FileUtils::getInstance()->addSearchPath("ipad/sence/scene4");
		FileUtils::getInstance()->addSearchPath("ipad/sence/zujian");
	}
	Preload::loadConfigData();

    // turn on display FPS
	if (1 == DEBUG_uGameInDebugInfo || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		director->setDisplayStats(true);
	else 
		director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene = FirstLayer::createScene();
    // run
    director->runWithScene(scene);
// 	auto scene = Scene::create();
// 	director->runWithScene(scene);
// 	director->getScheduler()->schedule([](float dt){GameDirector::getDirector()->loadingToRunningScene(RunningMode::endless);},
// 		scene, 0.1f, 0, 0, false, "aaaaa");
// 	

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

	if (RunningHelper::getHelper()->isGameIn())
	{
		if (RunningHelper::getHelper()->getCurrentStateBar())
		{
			RunningHelper::getHelper()->getCurrentStateBar()->onPause(nullptr);
		}
	}
    // if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	if (FX::Music::getInstance()->isMusicEnable())
		FX::Music::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	if (FX::Music::getInstance()->isMusicEnable())
		FX::Music::getInstance()->resumeBackgroundMusic();
}
