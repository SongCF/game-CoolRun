//============================================================
//* 打开游戏的第一个界面
//============================================================

#pragma once

#include "GameDirector.h"

#include "cocos2d.h"
using namespace cocos2d;

#include <string>
#include <vector>
using std::string;
using std::vector;



class FirstLayer : public LayerColor
{
public:
	static Scene* createScene();
	CREATE_FUNC(FirstLayer);
	bool init();
	void preloadAsync(string fileName);
	void preloadAsyncEnd(float dt);

	void displayLogoEnd(float dt);
	void loadingEnd();
	void endToUIScene();
protected:
	Size m_winSize;

	vector<string> m_loadVec;
	int m_hadLoadedCount;
	bool m_bDisplayLogoEnd;
};