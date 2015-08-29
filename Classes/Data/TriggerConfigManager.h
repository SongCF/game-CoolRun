//============================================================
//* 触发器配置类
//* 所有触发器的配置
//============================================================

#pragma once

#include "UnitDefine.h"
#include <string>
#include <vector>
#include "XReadJson.h"
using std::string;
using std::vector;

struct Trigger
{
	unsigned int ID;
	unsigned int actionID;
	int distance;

	string name;
	string describe;

	string musicFile;

	Trigger(){
		this->ID = 0;
		this->actionID = 0;
		this->distance = 0;
	}
	Trigger(const Trigger& in){
		this->ID = in.ID;
		this->name = in.name;
		this->describe = in.describe;
		this->actionID = in.actionID;
		this->musicFile = in.musicFile;
		this->distance = in.distance;
	}
	const Trigger& operator=(const Trigger& in){
		this->ID = in.ID;
		this->name = in.name;
		this->describe = in.describe;
		this->actionID = in.actionID;
		this->musicFile = in.musicFile;
		this->distance = in.distance;
		return *this;
	}
};

class TriggerConfigManager
{
private:
	static TriggerConfigManager* s_mTriggerConfigManager;
	TriggerConfigManager();
	virtual ~TriggerConfigManager();
	void initData();
public:
	static TriggerConfigManager* getManager();

	const vector<Trigger>* getAllTriggerItem();
	const Trigger* getTriggerItemByID(unsigned int triggerID);

protected:
	vector<Trigger> m_TriggerList;
};