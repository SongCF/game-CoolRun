//============================================================
//* 玩家信息帮助类
//* 逻辑层
//* 记录所有玩家操作记录：上次游戏选择的是什么角色、什么宠物、什么道具、玩家游戏的充值情况，数据统计等
//============================================================

#pragma once
#include <iostream>
#include "PlayerInfoDataManager.h"

#define History_AlreadyHasLogin				"Already_Has_Login"
#define History_AlreadyBoss_Guide			"Already_Boss_Guide"
//#define History_FirstLogin "history_first_login"

class PlayerInfoHelper
{
public:
	enum GameSetLock
	{
		MusicLock,
		MusciEffectLock,
		SpecialEffectLock,
	};
private:
	static PlayerInfoHelper* s_mPlayerInfoHelper;
	PlayerInfoHelper();
public:
	static PlayerInfoHelper* getHelper();

	//一般不要用
	void setOneInfo(string key, int value);
	int getOneInfo(string key);//没有就返回0


	//上次游戏选择的跑酷人物
	void setDeafaultHero(unsigned int unitID);
	unsigned int getDeafaultHero();

	//上次游戏选择的宠物
	void setDeafaultPet(unsigned int unitID);
	unsigned int getDeafaultPet();

	//魔法物
	void setDeafaultMagicItem(unsigned int unitID);
	unsigned int getDeafaultMagicItem();


	void setTeacher(unsigned int unitID);
	unsigned int getTeacher();
	//历史最高纪录
	void setHistoryMaxScore(unsigned int score, unsigned int meter);
	void getHistoryMaxScore(unsigned int & score, unsigned int & meter);
	//本周最高纪录
	void setThisWeekMaxScore(unsigned int score, unsigned int meter);
	void getThisWeekMaxScore(unsigned int & score, unsigned int & meter);

	//用户的uerid
	void setUserId(unsigned int id);
	unsigned int getUserId();

	void changeLoginTime();
	bool isFirstLogin();

	void setPlayerName(const std::string &name);
	const std::string getPlayerName();

	void addLoginDays();
	unsigned int getLoginDays();

	void setUpdate(UpdateTag tag);

	bool getIsUpdate(UpdateTag tag);

	void setGameLock(GameSetLock lockType,bool lock);
	bool getGameLock(GameSetLock lockType);

	void setMusicEnable(bool open);
	bool getMusicEnable();
};