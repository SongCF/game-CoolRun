//============================================================
//* 角色管理类
//* 玩家角色的业务逻辑处理
//============================================================


#pragma once

#include "UnitDefine.h"
#include <map>
#include "XReadJson.h"

using std::map;

struct OneLevelInfo
{
	unsigned int level;
	vector<Attribute> attrVec;	//到达该等级所拥有的属性
	Unit *needUnit;				//升级到该等级所需要的 物品
	unsigned int needCount;
	string name;
	string imgFile;
	OneLevelInfo()
	{
		level = 0;
		attrVec.clear();
		needUnit= nullptr;
		needCount = 0;
		name = "";
		imgFile = "";
	}
	OneLevelInfo(const OneLevelInfo &_in)
	{
		this->attrVec = _in.attrVec;
		this->imgFile = _in.imgFile;
		this->level = _in.level;
		this->name = _in.name;
		this->needCount = _in.needCount;
		this->needUnit = _in.needUnit;
	}
	const OneLevelInfo &operator = (const OneLevelInfo &_in)
	{
		this->attrVec = _in.attrVec;
		this->imgFile = _in.imgFile;
		this->level = _in.level;
		this->name = _in.name;
		this->needCount = _in.needCount;
		this->needUnit = _in.needUnit;
		return *this;
	}
	
};

class LevelDataConfigManager
{
private:
	static LevelDataConfigManager* s_mLevelDataManager;
	LevelDataConfigManager();
	void initData();

	void initJsonData(const rapidjson::Value &arrayValue,const string& name);
public:
	static LevelDataConfigManager* getManager();

	const vector<OneLevelInfo>* getOneUnitAllLevelInfo(unsigned int actorOrPet_ID);
	const OneLevelInfo* getOneUnitLevelInfo(unsigned int actorOrPet_ID, unsigned int level);

	int getMaxLevel(unsigned int actor_ID);
protected:
	map<unsigned int, vector<OneLevelInfo> > m_levelInfoPool;
};