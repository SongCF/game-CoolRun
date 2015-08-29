//============================================================
//* 游戏场景工厂类
//* 功能 ：获取游戏场景、游戏内单元 位置 动画 等信息
//* 依赖 ：GameSceneConfigHelper
//============================================================

#pragma once

#include "GameSceneConfigManager.h"
#include "GameDirector.h"
#include <vector>
#include <queue>
using std::vector;


struct SceneInfo
{
	unsigned int bgID;
	BackgroundList* sceneBG;
	vector<SceneItem>* sceneItems; //不同的sceneID，会对应不同的资源位置(资源名相同，目录不同)
	float screenCount;  //会有不完成场景的情况。
	SceneInfo(){
		bgID = 0;
		screenCount = 0;
		sceneBG = nullptr;
		sceneItems = nullptr;
	}
	SceneInfo(const SceneInfo& temp){
		this->bgID = temp.bgID;
		this->sceneBG = temp.sceneBG;
		this->sceneItems = temp.sceneItems;
		this->screenCount = temp.screenCount;
	}
	const SceneInfo& operator=(const SceneInfo& temp){
		this->bgID = temp.bgID;
		this->sceneBG = temp.sceneBG;
		this->sceneItems = temp.sceneItems;
		this->screenCount = temp.screenCount;
		return *this;
	}
};

class GameSceneFactory
{
private:
	static GameSceneFactory* s_mGameSceneFactory;
	GameSceneFactory();
public:
	static GameSceneFactory* getFactory();

#if 0
	SceneInfo getSceneInfo(RunningMode mode,int meter,Scene_PlayMode playMode = mode_Common,int count = 1);
#endif
	//关卡模式 （大关id， 小关id， 难度）
	SceneInfo getTollgateMap(unsigned int seasonId, unsigned int sectionId, unsigned int difficultLevel);
	//无尽模式  
	//1. mode 模式(超跑、战斗、正常)
	//2. phaseId 阶段id(第一场，第二场)
	//3. mapIdx 阶段地图索引(cpp_map?_1，cpp_map?_2)
	//4. screencount _输出_ 屏数
	SceneInfo getEndlessMap(Scene_PlayMode mode, unsigned int phaseId, unsigned int mapIdx);
	//教学
	SceneInfo getTeachMap(int value=0);

	unsigned int getMaxMapCount(Scene_PlayMode playMode = mode_Common);
protected:
private:
	unsigned int					m_nMaxMeter;

	unsigned int					m_nBackgroundID;
	unsigned int					m_nScreenCount;

//	int								 m_nSuperCount;//超跑所跑地图的位置
};