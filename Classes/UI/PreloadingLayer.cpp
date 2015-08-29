#include "PreloadingLayer.h"
#include "Preload.h"
#include "GameDirector.h"
#include "BaseSprite.h"
#include "StringsMap.h"
#include "cocostudio/CocoStudio.h"
#include "Font.h"
#include "Music.h"

#define WAITING_TIME 1.5f

static std::vector<string> g_Tips;

PreloadingLayer::PreloadingLayer()
{
	m_bWaitingEnd = false;
	m_loadVec.clear();
	m_hadLoadedCount = 0;

	if (0 == g_Tips.size())
	{
		int iCount = 0;
		char name[32] = {};
		std::string value;
		while (true)
		{
			sprintf(name, "tip%d", iCount+1);
			value = FX::StringsMap::getValue(name);
			if (value != "")
			{
				g_Tips.push_back(value);
				iCount++;
			}
			else
			{
				break;
			}
		}
	}

}

vector<string> PreloadingLayer::getTipsVector()
{
	return g_Tips;
}

Scene* PreloadingLayer::createScene(bool isRunningScene, RunningMode to, vector<int> sectionInfor)
{
	Scene *pScene = Scene::create();
	PreloadingLayer *pLayer = new PreloadingLayer();
	pLayer->init(isRunningScene, to, sectionInfor);
	pScene->addChild(pLayer);
	pLayer->release();

	return pScene;
}
bool PreloadingLayer::init(bool isRunningScene, RunningMode to, vector<int> sectionInfor)
{
	m_isRunningScene = isRunningScene;
	m_runningMode = to;
	m_sectionInfor = sectionInfor;

	Size winSize = Director::getInstance()->getWinSize();
	Layer::init();

	//tips
	Label *tipLabel = Label::createWithSystemFont(g_Tips.at(rand()%g_Tips.size()), "Arial", 30);
	this->addChild(tipLabel);
	tipLabel->setPosition(tipLabel->getContentSize().width/2+100, tipLabel->getContentSize().height);
	tipLabel->setColor(Font_Color_BaseColor);
	//
	Sprite *pS = Sprite::create("UI_zhuye_tishi_zuoshang.png");
	this->addChild(pS);
	pS->setPosition(Vec2(tipLabel->getPositionX()-tipLabel->getContentSize().width/2-pS->getContentSize().width/2, 
		tipLabel->getPositionY()+tipLabel->getContentSize().height));

	//
	Sprite *circle = Sprite::create("UI_biaoshi_zairu.png");
	this->addChild(circle);
	circle->setPosition(winSize/2);
	circle->runAction(RepeatForever::create(RotateBy::create(2,360)));

// 	//hero run
// 	BaseSprite* hero = BaseSprite::create("nanhai_pao_01.png");
// 	this->addChild(hero);
// 	hero->setScale(0.7f);
// 	hero->setPosition(winSize.width-hero->getContentSize().width*hero->getScaleX(), hero->getContentSize().height*hero->getScaleY());
// 	Animation *animation = Animation::create();
// 	for (int i=0; i<12; ++i)
// 	{
// 		animation->addSpriteFrameWithFile(StringUtils::format("nanhai_pao_%02d.png", i+1));
// 	}
// 	animation->setDelayPerUnit(0.0335f);
// 	RepeatForever *action = RepeatForever::create(Animate::create(animation));
// 	hero->runAction(action);
	//title
// 	Label* title = Label::createWithSystemFont(FX::StringsMap::getValue("WanMingJiaZai")+"...", "Arial", 24);
// 	this->addChild(title);
// 	title->setPosition(winSize.width-title->getContentSize().width, title->getContentSize().height);

	//
	this->scheduleOnce(schedule_selector(PreloadingLayer::startLoading), TranslationTime+0.1f);

	return true;
}

void PreloadingLayer::startLoading(float dt)
{
	//开始游戏时，重置音效
	FX::Music::getInstance()->stopBackgroundMusic();
	

	//1. purge texture
	Director::getInstance()->getTextureCache()->removeAllTextures();
	//2. purge armature
	//3. load all
	if (m_isRunningScene){
//		Preload::purgeUISceneRes();
		m_loadVec = Preload::loadRunningSceneRes();

		Preload::preloadMusicFile();
	}
	else {
//		Preload::purgeRunningSceneRes();
		m_loadVec = Preload::loadUISceneRes();
	}


	for (int i=0,count=m_loadVec.size(); i<count; ++i)
	{
		preloadAsync(m_loadVec.at(i));
	}

	this->scheduleOnce(schedule_selector(PreloadingLayer::waitingEnd), WAITING_TIME);
}

void PreloadingLayer::preloadAsync(string fileName)
{
	CCLOG("%s ---> %s", __FUNCTION__, fileName.c_str());

	int idx = fileName.find_last_of(".");
	string exName = fileName.substr(idx+1);
	if (exName == "jpg" || exName == "png"){
		Director::getInstance()->getTextureCache()->addImageAsync(fileName, [&](Texture2D* t){this->preloadAsyncEnd(0);});
	}
	else if (exName == "ExportJson"){
		//CCASSERT(false, "can not use cocostudio in this version!");
		cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(fileName, this, schedule_selector(PreloadingLayer::preloadAsyncEnd));
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
void PreloadingLayer::preloadAsyncEnd(float dt)
{
	++m_hadLoadedCount;

	if (m_hadLoadedCount == m_loadVec.size()){
		loadingEnd();
	}
}
void PreloadingLayer::loadingEnd()
{
	if (m_bWaitingEnd && m_hadLoadedCount == m_loadVec.size()){
		endToScene();
	}
}
void PreloadingLayer::waitingEnd(float dt)
{
	m_bWaitingEnd = true;

	if (m_bWaitingEnd && m_hadLoadedCount == m_loadVec.size()){
		endToScene();
	}
}
void PreloadingLayer::endToScene()
{
	CCLOG("%s ---> loading end", __FUNCTION__);
	if (m_isRunningScene){
		GameDirector::getDirector()->gotoRunningScene(m_runningMode, m_sectionInfor);
		//开始游戏时，重置音效
		FX::Music::getInstance()->playGameInBackgroundMusic();
	}
	else{
		GameDirector::getDirector()->gotoUIScene(m_runningMode);
		FX::Music::getInstance()->playMainBackgroundMusic();
	}

}