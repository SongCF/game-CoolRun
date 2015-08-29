//============================================================
//* 玩家信息类
//* 玩家上次游戏中响应操作留下的history
//* 主要记录玩家 上次选择的英雄、宠物、道具等等
//============================================================

#ifndef __PlayerInfoDataManager
#define __PlayerInfoDataManager



#include "UnitDefine.h"
#include <vector>
#include "WriteJson.h"
#include "XReadJson.h"
using std::vector;

#define	key_playGameCount					"key_playGameCount"
#define	key_ProtectItemUsedCount			"key_ProtectItemUsedCount"
#define	key_GameRebornCount					"key_GameRebornCount"

struct PlayerInfo
{
	string key;
	int value;
};
struct LoginTime
{
	int year;
	int month;
	int data;
	int hours;
};
enum UpdateTag
{
	Update_Score,
	
};
class PlayerInfoDataManager:WriteJson
{
private:
	static PlayerInfoDataManager* s_mPlayerInfoDataManager;
	PlayerInfoDataManager();
	void initData();
	void saveData();
	string WriteToJson();

	void copyFile();
public:
	static PlayerInfoDataManager* getManager();

	void setOnePlayerInfo(string key, int value);
	int getOnePlayerInfo(string key);

	//上次游戏选择的跑酷人物
	void setDeafaultHero(unsigned int unitID);
	unsigned int getDeafaultHero();

	//上次游戏选择的宠物
	void setDeafaultPet(unsigned int unitID);
	unsigned int getDeafaultPet();

	//魔法物
	void setDeafaultMagicItem(unsigned int unitID);
	unsigned int getDeafaultMagicItem();


	//教学
	void setPlayeriscomteacher(unsigned int unitID);
	unsigned int getPlayeriscomteacher();

	//void changeAchievement(unsigned int achieveID); 

	void setGotAchievement(unsigned int unitID);
	const vector<unsigned int> *getGotAchievementList();

// 	void setGotTollgate(unsigned int tollgateID);
// 	const vector<unsigned int>* getGotTollgate();//获取所有通关的关卡
// 
// 	void setGotTollgateAward(unsigned int tollgateID);
// 	const vector<unsigned int>* getGotTollgateAward();//获取领取过奖励的关卡

// 	//奔跑的历史最高纪录（距离）
// 	void setHistoryMaxDistance(unsigned int meter);
// 	unsigned int getHistoryMaxDistance();

	//用户的uerid
	void setUserId(unsigned int id);
	unsigned int getUserId();

	void changeLoginTime();
	const LoginTime *getLastLoginTime();

	void setPlayerName(const string &name)
	{
		m_sPlayerName = name;
		saveData();
	}
	string getPlayerName(int index = 1)
	{
		return m_sPlayerName;
	}

	void addLoginDays();
	unsigned int getLoginDays();

	bool getIsUpdate(UpdateTag tag);
	void setUpdata(UpdateTag tag);

	float getMaxFightPower();

private:
	vector<PlayerInfo>    m_lPlayerInfoList;

	vector<unsigned int>  m_lGotAchievement;//已经领取过奖励的成就

	vector<unsigned int>  m_lGotTollgate;//已经通关的关卡

	vector<unsigned int>  m_lGotTollgateAwardList;//已经领取过奖励的关卡

	LoginTime			  m_lLastLoginTime;

	string				  m_sPlayerName;

	bool				  m_bIsUpdate[4];
};
#endif // !__PlayerInfoDataManager