#include "FirstLayer.h"
#include "StartLayer.h"
#include "Preload.h"
#include "Music.h"
#include "cocostudio/CocoStudio.h"
#include "Veronestart.h"
//#include "sdk/SdkManager.h"

Scene* FirstLayer::createScene()
{
	Scene* pScene = Scene::create();
	FirstLayer* layer = FirstLayer::create();
	pScene->addChild(layer);
	return pScene;
}

bool FirstLayer::init()
{
	LayerColor::initWithColor(Color4B(0,0,0,255));
	m_winSize = Director::getInstance()->getWinSize();
	m_bDisplayLogoEnd = false;
	m_hadLoadedCount = 0;

// 	Sprite *logo1 = Sprite::create("logo_camellia.jpg");
// 	this->addChild(logo1);
// 	logo1->setPosition(m_winSize/2);

	m_loadVec = Preload::loadUISceneRes();
	for (int i=0,count=m_loadVec.size(); i<count; ++i)
	{
		preloadAsync(m_loadVec.at(i));
	}
	this->scheduleOnce(schedule_selector(FirstLayer::displayLogoEnd), .01f);

	return true;
}
void FirstLayer::preloadAsync(string fileName)
{
	CCLOG("%s ---> %s", __FUNCTION__, fileName.c_str());

	int idx = fileName.find_last_of(".");
	string exName = fileName.substr(idx+1);
	if (exName == "jpg" || exName == "png"){
		Director::getInstance()->getTextureCache()->addImageAsync(fileName, [&](Texture2D* t){this->preloadAsyncEnd(0);});
	}
	else if (exName == "ExportJson"){
		//CCASSERT(false, "can not use cocostudio in this version!");
		cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(fileName, this, schedule_selector(FirstLayer::preloadAsyncEnd));
	}
	else if (exName == "plist"){
		string textureName = fileName.substr(0, idx) + ".png";
		Director::getInstance()->getTextureCache()->addImageAsync(textureName, [=](Texture2D* t){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fileName, t);
			this->preloadAsyncEnd(0);
		});
	}
	else {
		CCASSERT(false, "error file extern name");
	}
}
void FirstLayer::preloadAsyncEnd(float dt)
{
	++m_hadLoadedCount;

	if (m_hadLoadedCount == m_loadVec.size()){
		loadingEnd();
	}
}

void FirstLayer::loadingEnd()
{
	if (m_bDisplayLogoEnd && m_hadLoadedCount == m_loadVec.size()){
		endToUIScene();
	}
}
void FirstLayer::displayLogoEnd(float dt)
{
	m_bDisplayLogoEnd = true;

	if (m_hadLoadedCount == m_loadVec.size() && m_bDisplayLogoEnd)
		endToUIScene();
}
void FirstLayer::endToUIScene()
{
	{
		GoodsHelper* pHelper = GoodsHelper::getHelper();
		map<string,string> info;
		time_t curTime = time(0);
		struct tm *curTm = localtime(&curTime);//时区时间， 非时区时间使用struct tm *gmtime(const time_t *timep); 
		char * timeStr = asctime(curTm);
		info.insert(std::make_pair("LogInTime", timeStr));
		info.insert(std::make_pair("GoldNum", String::createWithFormat("%u", pHelper->getGoldNum())->getCString()));
		info.insert(std::make_pair("DiamondNum", String::createWithFormat("%u", pHelper->getDiamondNum())->getCString()));
		info.insert(std::make_pair("TG_Num", String::createWithFormat("%u", pHelper->getTangguoNum())->getCString()));
		info.insert(std::make_pair("TT_Num", String::createWithFormat("%u", pHelper->getGuodongNum())->getCString()));
		info.insert(std::make_pair("DG_Num", String::createWithFormat("%u", pHelper->getDaogaiNum())->getCString()));
		//SdkManager::getInstance()->logEvent(analyticsEVENT_LogIn, &info);
	}


	Scene *pScene = Scene::create();
	//Veronestart pScene->addChild(StartLayer::create());
	pScene->addChild(Veronestart::create());
	Director::getInstance()->replaceScene(TransitionFade::create(0.05f, pScene));

	FX::Music::getInstance()->playMainBackgroundMusic();
}