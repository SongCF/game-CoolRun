#include "GameDirector.h"
#include "cocos2d.h"
#include "PreloadingLayer.h"

#include "RunningHelper.h"
#include "SimpleBackgroundLayer.h"
#include "SimpleFrontgroundLayer.h"
#include "EffectBackgroundLayer.h"
#include "GameUnitLayer.h"
#include "UnitCollisionDetection.h"
#include "StateBar.h"
#include "GameInTouchLayer.h"
#include "GameInKeyboardLayer.h"

#include "Uiscene.h"
#include "MainuiLayer.h"

#include "net/NetworkWaitingLayer.h"


using namespace cocos2d;


GameDirector* GameDirector::s_mGameDirector = nullptr;

GameDirector::GameDirector()
{

}

GameDirector* GameDirector::getDirector()
{
	if (! s_mGameDirector) s_mGameDirector = new GameDirector;
	return s_mGameDirector;
}

void GameDirector::loadingToUIScene(RunningMode posIdx)
{
	Director::getInstance()->popToSceneStackLevel(1);
	Scene *pScene = PreloadingLayer::createScene(false, posIdx);
	Director::getInstance()->replaceScene(TransitionFade::create(TranslationTime, pScene));
}
void GameDirector::loadingToRunningScene(RunningMode mode, unsigned int seasonId,unsigned int sectionId,unsigned int difficutLevel)
{
	vector<int> sectionInfo(3,1);
	sectionInfo[0] = seasonId;
	sectionInfo[1] = sectionId;
	sectionInfo[2] = difficutLevel;
	Scene *pScene = PreloadingLayer::createScene(true, mode, sectionInfo);
	Director::getInstance()->replaceScene(TransitionFade::create(TranslationTime, pScene));
}
void GameDirector::gotoUIScene(RunningMode posIdx)
{
	Scene *pScene = nullptr;
	if (posIdx == RunningMode::endless)
	{
		pScene = Uiscene::createScene();
	}
	else if (posIdx == RunningMode::tollgate)
	{
		pScene = Scene::create();
		MainuiLayer* layer = MainuiLayer::create();
		pScene->addChild(layer);
		layer->gotoTollgateLayer();
	}
	else if (posIdx == RunningMode::teach)
	{
		pScene = Uiscene::createScene();
	}
	else
	{
		CCASSERT(false, "");
	}
	Director::getInstance()->replaceScene(TransitionFade::create(TranslationTime, pScene));
}
void GameDirector::gotoRunningScene(RunningMode mode, vector<int> sectionInfo)
{
	// test guide 
// 	mode = RunningMode::tollgate;
// 	id = 1;

	Scene *pScene = Scene::create();

	RunningHelper::getHelper()->initStartGame(pScene, mode, sectionInfo);
	pScene->addChild(SimpleBackgroundLayer::create(), 1, "SimpleBackgroundLayer");
//	pScene->addChild(EffectBackgroundLayer::create(), 2, "EffectBackgroundLayer");
	pScene->addChild(GameUnitLayer::create(mode, sectionInfo), 3, "GameUnitLayer");
	pScene->addChild(SimpleFrontgroundLayer::create(), 4, "SimpleFrontgroundLayer");
	pScene->addChild(UnitCollisionDetection::create(), 5, "UnitCollisionDetection");
	pScene->addChild(StateBar::create(), 10, "StateBar");
	pScene->addChild(GameInTouchLayer::create(), 6, "GameInTouchLayer");
// #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	pScene->addChild(GameInKeyboardLayer::create(), 6, "GameInKeyboardLayer");
// #endif
// 	pScene->getChildByName("SimpleBackgroundLayer")->removeFromParent();//->setVisible(false);
// 	pScene->getChildByName("SimpleFrontgroundLayer")->removeFromParent();//->setVisible(false);
	Director::getInstance()->replaceScene(TransitionFade::create(TranslationTime, pScene));
}

void GameDirector::showWaiting(bool bShowWaitingUI)
{
	Director::getInstance()->getRunningScene()->addChild(NetworkWaitingLayer::create(bShowWaitingUI));
}

void GameDirector::hideWaiting()
{
	Director::getInstance()->getRunningScene()->removeChildByName(TAGName_WaitingLayer);
}