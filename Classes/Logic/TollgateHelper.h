//============================================================
//* 管理关卡类
//============================================================

#pragma once

#include "UnitDefine.h"
#include <vector>
#include "TollgateDataManager.h"
class TollgateHelper
{
private:
	static TollgateHelper* s_mTollgateHelper;
	TollgateHelper();
public:
	static TollgateHelper* getHelper();

	//*********************************************
	//获取相关的配置信息
	//*********************************************
	const vector<Season>* getAllTollgate();
	const Season* getOneTollgateSeason(unsigned int seasonId);//一个大关卡
	const Section* getOneTollgateSection(unsigned int seasonId, unsigned int sectionId);//大关中的一个小关卡


	//*********************************************
	//获取相关的用户信息
	//获取通关星级
	//*********************************************
	TollgateInfor::TollgatePassLevel getOneSectionPassLevel(unsigned int seasonId, unsigned int sectionId, Section::Difficulty lv);
	//设置已领取通关奖励（当前版本是直接在结算界面颁发奖励，所以关卡奖励不会使用到该接口）
	//该接口用于每章第十关通关后 奖励的插画信息。
	void setGotAward(unsigned int seasonId, unsigned int sectionId, Section::Difficulty lv);
	bool isGotAward(unsigned int seasonId, unsigned int sectionId, Section::Difficulty lv);//获取是否已经领取奖励	
	//pref : 完成一个关卡，保存数据
	//seasonId大关卡id
	//sectionId小关卡id
	//lv 难度
	//star 完成评级
	void completeTollgateSection(unsigned int seasonId, unsigned int sectionId, Section::Difficulty lv, TollgateInfor::TollgatePassLevel star=TollgateInfor::eGood);
	bool isPassed(unsigned int seasonId, Section::Difficulty lv); //整个大关卡某难度是否通关
	bool isPassed(unsigned int seasonId, unsigned int sectionId, Section::Difficulty lv);//某难度是否已经通关
	bool isCanStart(unsigned int seasonId, unsigned int sectionId, Section::Difficulty lv);//某难度是否可以开启
	//获取当前进度  season section
	//返回当前难度可打的最后一关（如：刚打过1-1 那么返回1-2）
	vector<int> getProgress(Section::Difficulty lv);
	//获取当前章节可打过得最后一关
	int getSeasonProgress(int seasonId, Section::Difficulty lv);
};