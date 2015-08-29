//============================================================
//* 战斗场景配置管理类
//* 配置工具配置的所有场景信息由该类来解析
//* 包含 1：远景近景图， 2：场景中所有物件
//============================================================


#pragma once

#include "UnitDefine.h"
#include <vector>
#include <map>
#include <string>
#include "XReadJson.h"
#include "GameDirector.h"
using std::string;
using std::map;
using std::vector;

class FightTotalMapManager
{
private:
	static FightTotalMapManager* s_mFightTotalMapManager;
	FightTotalMapManager();
	~FightTotalMapManager();
	void initData();
public:
	static FightTotalMapManager* getManager();

	unsigned int getMaxMeter();

	vector<string> *getTotalFightSceneItems();

protected:

	unsigned int					m_nMaxMeter;

	vector<string>					m_lTotalSceneItemList;

};