//============================================================
//* 成就配置类
//* 所有成就的配置，获得相应成就的条件、即奖励
//============================================================

#pragma once

#include "UnitDefine.h"
#include <string>
#include <vector>
#include "XReadJson.h"
using std::string;
using std::vector;
struct AchieveSpecialAward
{
	unsigned int ID;
	unsigned int award_unitID;
	unsigned int award_value;//如果是道具 那么为个数，如果是人物  那么是等级
	unsigned long ex_value;//扩展字段
};
struct Achieve
{
	unsigned int ID;
	string name;
	string describe;
	string imgFile;
	unsigned int award_unitID;
	unsigned int count;

	int process; // 完成度

	bool isOver;//是否完成
	Achieve(){
		this->ID = 0;
		this->award_unitID = 0;
		this->count = 0;
		this->process = 0;
		this->isOver = false;
	}
	Achieve(const Achieve& in){
		this->ID = in.ID;
		this->name = in.name;
		this->describe = in.describe;
		this->imgFile = in.imgFile;
		this->award_unitID = in.award_unitID;
		this->count = in.count;
		this->process = in.process;
		this->isOver = in.isOver;
	}
	const Achieve& operator=(const Achieve& in){
		this->ID = in.ID;
		this->name = in.name;
		this->describe = in.describe;
		this->imgFile = in.imgFile;
		this->award_unitID = in.award_unitID;
		this->count = in.count;
		this->process = in.process;
		this->isOver = in.isOver;
		return *this;
	}
};

class AchievementConfigManager
{
private:
	static AchievementConfigManager* s_mAchievementConfigManager;
	AchievementConfigManager();
	virtual ~AchievementConfigManager();
	void initData();
public:
	static AchievementConfigManager* getManager();

	const vector<Achieve>* getAllAchievement();
	const Achieve* getAchievementByID(unsigned int achieveID);

	const vector<AchieveSpecialAward>* getAllAchieveSpecialAwardVec();
protected:
	vector<Achieve> m_lAchieveVec;
	vector<AchieveSpecialAward> m_lAchieveSpecialAwardVec;
};