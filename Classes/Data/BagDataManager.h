//============================================================
//* 背包管理类
//* 玩家所获得的所有物品、角色、宠物等
//* 【背包】   ()
//  1. 金币（Unit_id——数量）  gold
// 	2. 钻石（Unit_id——数量）  jewel
// 	3. 角色（Unit_id——等级）  hero
// 	4. 宠物（Unit_id——等级）  pet
// 	5. 道具（Unit_id——数量）  item
// 	6. 成就（Achieve_ID——完成度）Achievement
//============================================================

#pragma once

#include "UnitDefine.h"
#include "WriteJson.h"
struct BagItem
{
	unsigned int id;
	unsigned int value; //物品就是个数、人物宠物就是等级
	unsigned long value_2; //物品就是个数、人物宠物就是等级

	BagItem(){
		this->id = 0;
		this->value = 0; 
		this->value_2 = 0; 
	}
	BagItem(const BagItem& _in){
		this->id = _in.id;
		this->value = _in.value;
		this->value_2 = _in.value_2;
	}
	const BagItem& operator=(const BagItem& _in){
		this->id = _in.id;
		this->value = _in.value;
		this->value_2 = _in.value_2;
		return *this;
	}
};
class BagDataManager:WriteJson
{
private:
	static BagDataManager* s_mBagManager;
	BagDataManager();
	void initData();
	void copyFile();
public:
	static BagDataManager* getManager();

	//获取数据
	const BagItem* getOneUnit(unsigned int id);
	const vector<BagItem>* getAllUnits();
	void saveData();

	// 增添、删除
	void changeOneUnitCount(unsigned int id, int count);

	// 人物、宠物升级
	void changeOneUnitLevel(unsigned int id, int addLv,int value);

	// 成就 完成度
	void changeOneAchievement(unsigned int achieveID, int data,long time);

private:
	string WriteToJson();
protected:
	vector<BagItem> m_BagItemVec;
};