//============================================================
//* 游戏场景加载层
//* 每次切换场景（runningscene -> ui scene -> running scene）时，
//* gamedirector都需要先到loadinglayer 来加载接下来场景所需要的资源 释放上次场景所分配的资源
//============================================================

#pragma once

#include "GameDirector.h"

#include "cocos2d.h"
using namespace cocos2d;
#include <vector>
#include <string>

using std::vector;
using std::string;


class PreloadingLayer : public Layer
{
private:
	PreloadingLayer();
	bool init(bool isRunningScene, RunningMode to, vector<int> sectionInfo);
public:
	static Scene* createScene(bool isRunningScene, RunningMode to=RunningMode::endless, vector<int> sectionInfor=vector<int>(3,0) );
	static vector<string> getTipsVector();
protected:
	void startLoading(float dt);
	void preloadAsync(string fileName);
	void preloadAsyncEnd(float dt);

	void loadingEnd();
	void waitingEnd(float dt);
	void endToScene();
	
protected:
	bool m_isRunningScene;
	RunningMode m_runningMode;
// 	unsigned int m_seasonId;
// 	unsigned int m_sectionId;
// 	unsigned int m_difficultLevel;
	vector<int> m_sectionInfor;


	vector<string> m_loadVec;
	int m_hadLoadedCount;
	bool m_bWaitingEnd;
};