//============================================================
//* 成就帮助类
//* 逻辑层
//* 处理成绩的逻辑操作
//============================================================

#pragma once
#include "AchievementConfigManager.h"
#include <vector>
class AchievementHelper
{
private:
	static AchievementHelper* s_mAchievementHelper;
	AchievementHelper();
public:
	static AchievementHelper* getHelper();

	//获取所有成就 //配置文件里面的
	const vector<Achieve>* getAllAchievement();

	bool isAchieveSpecialAward(unsigned int id);

	const AchieveSpecialAward *getAchieveSpecialAwardInfo(unsigned int id);
	void getAchieveSpecialAward(unsigned int id);

	long getAchieveCompleteTime(unsigned int id);//获取完成成就的时间
	//用户操作
	//达成成就
	void completeOneAchievement(unsigned int achieveID,int value,long lTime = 0);
	//获取成就奖励
	void getAchievementAward(unsigned int achieveID);
	const Achieve *getAchieveByID(unsigned int id);

	void getNewAchievementList(vector<unsigned int>& list);

//	bool getAllAchievementOver();

	const vector<unsigned int>* getGotAchievement();//获取所有领取了奖励的成就

	bool getIsGotAchievement(unsigned int id);//获取是否领取了奖励

	void getAchievement(unsigned int id);//获取奖励
private:
	vector<unsigned int>				m_lNewAchievementList;
};