
#include "GameSceneFactory.h"
#include "GameSceneConfigManager.h"
#include "cocos2d.h"
using namespace cocos2d;
GameSceneFactory* GameSceneFactory::s_mGameSceneFactory = nullptr;

//const int MAX_Meter = 1140;

GameSceneFactory::GameSceneFactory()
	:m_nBackgroundID(0)
	,m_nScreenCount(0)
//	,m_nSuperCount(0)
{

}

GameSceneFactory* GameSceneFactory::getFactory()
{
	if( !s_mGameSceneFactory) s_mGameSceneFactory = new GameSceneFactory;
	return s_mGameSceneFactory;
}

#if 0
SceneInfo GameSceneFactory::getSceneInfo(RunningMode mode,int meter,Scene_PlayMode playMode,int count)
{
	GameSceneConfigManager* manager = GameSceneConfigManager::getManager();
	SceneInfo ret;

	if (mode == RunningMode::tollgate)
	{
		int id = CCRANDOM_0_1() * 10;

		int count = 0;
		const SceneItemInfo *info = manager->getSceneItems(meter,1,playMode,count,mode);
		ret.sceneItems = const_cast<vector<SceneItem> *>(&info->list);
		id = CCRANDOM_0_1() * 10;
		ret.bgID = info->backList[id % info->backList.size()];
		ret.sceneBG = manager->getBackground(ret.bgID);
		ret.screenCount = count ;
		m_nScreenCount = 0;
		return ret;
	}

	m_nMaxMeter = manager->getMaxMeter(playMode);

	switch (playMode)
	{
	case mode_Common:
		{
			if (meter > m_nMaxMeter)
			{
				meter = m_nMaxMeter;
			}
		}
		break;
	case mode_Super:
		{
			m_nSuperCount ++;
			if (m_nSuperCount > m_nMaxMeter)
			{
				m_nSuperCount = 1;
			}
			meter = m_nSuperCount;
		}
		break;
	case mode_Fight:
		{
			if (meter > m_nMaxMeter)
			{
				meter = m_nMaxMeter;
			}
		}
		break;
	default:
		break;
	}
	
	
	
	
	

	int id = CCRANDOM_0_1() * 10;
	int screen_count = 0;

	id = id % manager->getSceneCount(meter,playMode,count) + 1;
// 	for (auto iter = info->list.begin(); iter != info->list.end();iter ++)
// 	{
// 		ret.sceneItems->push_back(*iter);
// 	}
	const SceneItemInfo *info = manager->getSceneItems(meter,1,playMode,screen_count,mode,count);
	ret.sceneItems = const_cast<vector<SceneItem> *>(&info->list);
	id = CCRANDOM_0_1() * 10;
	ret.bgID = info->backList[id % info->backList.size()];
	ret.sceneBG = manager->getBackground(ret.bgID);
	ret.screenCount = screen_count ;
	m_nScreenCount += screen_count;

	return ret;


	/*
	if (m_nScreenCount < 10 && m_nBackgroundID != 0 && m_nBackgroundID != manager->getBackgroundCount())
	{
	ret.bgID = m_nBackgroundID;
	if (playMode == mode_Super)
	{
	ret.bgID = manager->getBackgroundCount();
	}
	m_nScreenCount = 0;

	}
	else
	{

	int id = CCRANDOM_0_1() * 10;

	int bgCount = manager->getBackgroundCount();
	ret.bgID = id % bgCount + 1;

	if (playMode == mode_Super)
	{
	ret.bgID = bgCount;
	}
	else
	{
	while (ret.bgID == bgCount)
			{
				id = CCRANDOM_0_1() * 10;
				ret.bgID = id % bgCount + 1;
			}
		}
		m_nScreenCount = 0;
	}
	m_nBackgroundID = ret.bgID;
	*/
}
#endif

SceneInfo GameSceneFactory::getTollgateMap(unsigned int seasonId, unsigned int sectionId, unsigned int difficultLevel)
{
	SceneInfo ret;
	GameSceneConfigManager* manager = GameSceneConfigManager::getManager();

	int screenCount = 0;
	const SceneItemInfo *info = manager->getSceneItems(seasonId,sectionId,difficultLevel,screenCount);
	if (!info){
		CCASSERT(false,"");
		CCLOG("%s ---> has not this map info {seasonId = %d, sectionId = %d}", __FUNCTION__, seasonId, sectionId);
		return ret;
	}

	ret.sceneItems = const_cast<vector<SceneItem> *>(&info->list);
	int bgId = CCRANDOM_0_1() * 10;
	ret.bgID = info->backList[bgId % info->backList.size()];
	ret.sceneBG = manager->getBackground(ret.bgID);
	ret.screenCount = screenCount;
//	m_nScreenCount = 0;

	return ret;
}

SceneInfo GameSceneFactory::getEndlessMap(Scene_PlayMode mode, unsigned int phaseId, unsigned int mapIdx)
{
	GameSceneConfigManager* manager = GameSceneConfigManager::getManager();
	SceneInfo ret;

	m_nMaxMeter = manager->getMaxMeter(mode);
	//
	if (phaseId > m_nMaxMeter)
	{
		if (mode == mode_Super){ //超跑循环取地图，所以只用传入超跑的次数即可
			phaseId = 1;
		}else {
			phaseId = m_nMaxMeter;
		}
	}
	//改阶段的地图由 onePhaseInfo->count个map文件组成，检测是否越界了，越界了则循环。
	const MapInfoList* onePhaseInfo = manager->getOnePhaseMapInfo(phaseId);
	if (onePhaseInfo->count < mapIdx) //mapidx 从1开始
	{
		mapIdx = 1;
	}

	int screen_count = 0;
	const SceneItemInfo *info = manager->getSceneItems(mode, phaseId, mapIdx, screen_count);
	if (!info){
		CCASSERT(false,"");
		CCLOG("%s ---> has not this map info {phaseId = %d, mapIdx = %d}", __FUNCTION__, phaseId, mapIdx);
		return ret;
	}

	ret.sceneItems = const_cast<vector<SceneItem> *>(&info->list);
	int bgId = CCRANDOM_0_1() * 10;
	ret.bgID = info->backList[bgId % info->backList.size()];
	ret.sceneBG = manager->getBackground(ret.bgID);
	ret.screenCount = screen_count ;
//	m_nScreenCount += screen_count;

	return ret;
}

SceneInfo GameSceneFactory::getTeachMap(int value)
{
	SceneInfo ret;
	GameSceneConfigManager* manager = GameSceneConfigManager::getManager();

	int screenCount = 0;
	const SceneItemInfo *info = manager->getTollgateSceneItems("teach_map.txt",screenCount);
	if (!info){
		CCASSERT(false,"");
		CCLOG("%s ---> has not teach map info}", __FUNCTION__);
		return ret;
	}

	ret.sceneItems = const_cast<vector<SceneItem> *>(&info->list);
	int bgId = CCRANDOM_0_1() * 10;
	ret.bgID = info->backList[bgId % info->backList.size()];
	ret.sceneBG = manager->getBackground(ret.bgID);
	ret.screenCount = screenCount;

	return ret;
}


unsigned int GameSceneFactory::getMaxMapCount(Scene_PlayMode playMode)
{
	return GameSceneConfigManager::getManager()->getMaxMeter(playMode);
}