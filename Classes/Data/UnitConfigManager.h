//============================================================
//* 精灵信息类
//* 游戏中所有精灵（玩家角色、怪物、宠物）的配置信息都在这里获取
//* 通过解析配置文件来获得
//============================================================

#pragma once

#include "UnitDefine.h"
#include <vector>
#include "XReadJson.h"
using std::vector;

class UnitConfigManager
{
private:
	static UnitConfigManager* s_mUnitConfigManager;
	UnitConfigManager();
	void initData();

	void initUnitData(const rapidjson::Value& root,const string& name,UnitType type);
public:
	static UnitConfigManager* getManager();

	const Unit* getOneUnit(unsigned int id);
	const vector<Unit>* getAllUnit();


protected:
	vector<Unit>	m_lunitVec;
};