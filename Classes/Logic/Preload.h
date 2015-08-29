//============================================================
//* 预加载类
//* 处理配置文件加载、图片预加载等
//============================================================

#pragma once

#include "UnitConfigManager.h"
#include "AchievementConfigManager.h"
#include "GameSceneConfigManager.h"
#include "LevelDataConfigManager.h"
#include "MarketConfigManager.h"

#include "PlayerInfoDataManager.h"
#include "BagDataManager.h"
#include "ActionConfigManager.h"
#include "TriggerConfigManager.h"
#include "StringsMap.h"
#include "PreloadDataManager.h"
#include "TollgateDataManager.h"
#include "VersionConfig.h"
#include "Music.h"

#include "cocos2d.h"
using namespace cocos2d;
#include <vector>
#include <string>
using std::vector;
using std::string;


class Preload
{
public:
	static void loadConfigData(){
		VersionConfig::getConfig();

		FX::StringsMap::init();
		UnitConfigManager::getManager();
		AchievementConfigManager::getManager();
		GameSceneConfigManager::getManager();
		LevelDataConfigManager::getManager();
		MarketConfigManager::getManager();
		PlayerInfoDataManager::getManager();
		BagDataManager::getManager();
		ActionConfigManager::getManager();
		TriggerConfigManager::getManager();
		PreloadDataManager::getManager();
		TollgateDataManager::getManager();
	}
	static void preloadMusicFile(){
		const vector<string> *vec = PreloadDataManager::getManager()->getPreloadMusic();
		for (int i=0,count=vec->size(); i<count; ++i){
			FX::Music::getInstance()->loadEffectMusic(vec->at(i));
		}
	}

	static vector<string> loadUISceneRes(){
		return *PreloadDataManager::getManager()->getPreloadUIScene();
	}
	static void purgeUISceneRes(){

	}

	static vector<string> loadRunningSceneRes(){
		return *PreloadDataManager::getManager()->getPreloadRunScene();
	}

	static void purgeRunningSceneRes(){

	}
};