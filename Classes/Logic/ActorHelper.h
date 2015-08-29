//============================================================
//* 精灵单位帮助类
//* 逻辑层
//* 所有人物、宠物的操作、UI层调用，数据访问都是通过该类实现
//============================================================

#pragma once

#include "UnitDefine.h"
#include "LevelDataConfigManager.h"

struct Actor
{
	Unit unit;
	unsigned int level;
};

class ActorHelper
{
private:
	static ActorHelper* s_mActorHelper;
	ActorHelper();
public:
	static ActorHelper* getHelper();

	//从 PlayerInfoHelper获取
// 	//获取默认的角色
// 	Actor getDefaultactor();
// 	//获取默认的宠物
// 	Actor getDefaultpet();

	//所有角色列表 已解锁的 和 未解锁的
	const vector<Actor> getAllHero();

	//所有宠物物列表 已解锁的 和 未解锁的
	const vector<Actor> getAllPet();

	//所有宠物物列表 已解锁的 和 未解锁的
	const vector<Actor> getAllMagic();

	void levelUp(unsigned int heroOrPet_unitID);
	void fullLevelUp(unsigned int unitID); // 

	//使用碎片解锁角色（使用钻石或金币直接购买角色是在 商城里MarketHelper）
	void unlockHeroByFragment(unsigned int unitID);
	UnitID getActorUnlockNeedFragmentId(unsigned int unitID);
	//获取解锁某角色需要的 (碎片数量,金币数量)
	std::pair<int,int> getUnlockHeroNeedFragmentAndGold(unsigned int unitID);

	//喂养
	void feeding(unsigned int heroOrPet_unitID,unsigned int count = 1);

	unsigned int levelUpNeed(unsigned int heroOrPet_unitID);
	//void petLevelUp(unsigned int pet_unitID);

	bool isMaxLevel(unsigned int heroOrPet_unitID);

	Actor getOneHero(unsigned int hero_unitID);
	Actor getOnePet(unsigned int pet_unitID);
	Actor getOneMagic(unsigned int magic_unitID);

	//1. level参数默认为0，返回玩家已有的数据，如果返回等级为0，即玩家没有该actor。
	//2. level传入 1 --- maxLv，返回该level的配置数据。
	Actor getOneHeroWithLevelInfo(unsigned int hero_unitID, unsigned int hero_level = 0);
	Actor getOnePetWithLevelInfo(unsigned int pet_unitID, unsigned int pet_level = 0);
	Actor getOneMagicWithLevelInfo(unsigned int magic_unitID, unsigned int magic_level = 0);

	bool getIsUnlockItem(unsigned int id);

	int getMaxLevel(unsigned int id);
	const OneLevelInfo *getOneActorLevelInfo(unsigned int id,unsigned int level);
	vector<OneLevelInfo> getOneActorAllLevelInfo(unsigned int id,unsigned int level);

	UnitType getTypeFromUnit(unsigned int id);


};