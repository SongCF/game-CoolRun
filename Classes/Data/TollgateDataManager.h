//============================================================
//关卡解析
//============================================================

#pragma once

#include "UnitDefine.h"
#include "XReadJson.h"


//1.配置信息
//大关卡中的一系列小关
struct Section
{
	//通关条件：1碰到结束点，2得分，3路程",
	enum PassConditionType
	{
		PassType_End = 1,
		PassType_Score,
		PassType_Meter,
	};
	//难度类型
	enum Difficulty
	{
		eNormal = 0, //用作vector的下标,所以不要改这里的值
		eDifficult,
	};
	//奖励
	typedef struct{
		unsigned int awardUnitID;
		unsigned int awardUnitCount;
		vector<float> probability; //每一关会有多个奖励，没种奖励都有自己的出现概率（通关评级越高概率越高）
	}award;
	//不同难度
	typedef struct{
		string lvDescribe;// 简单 、 困难
		float speed;  //地图滚动速度
		vector<award> awardList;
	}levelInfor;


	unsigned int sectionId;
	string name;
	string mapName;
	PassConditionType passType;
	vector<levelInfor> levels;  //不同难度的奖励：   普通、困难


	Section(){
		this->sectionId = 0; 
		this->passType = PassType_End;
	}
	Section(const Section& _in){
		this->sectionId = _in.sectionId;
		this->name = _in.name; 
		this->mapName = _in.mapName; 
		this->passType = _in.passType;
		this->levels = _in.levels;
	}
	const Section& operator=(const Section& _in){
		this->sectionId = _in.sectionId;
		this->name = _in.name; 
		this->mapName = _in.mapName; 
		this->passType = _in.passType;
		this->levels = _in.levels;
		return *this;
	}
};
//大关卡
struct Season
{
	unsigned int seasonId;
	int unlockCondition;   //解锁需要面前的评价总和达到多少
	vector<Section> sectionList;

	Season(){
		seasonId = 0;
		unlockCondition = 0xffffff;
	}
	Season(const Season& _in){
		this->seasonId = _in.seasonId;
		this->unlockCondition = _in.unlockCondition;
		this->sectionList = _in.sectionList;
	}
	const Season& operator=(const Season& _in){
		this->seasonId = _in.seasonId;
		this->unlockCondition = _in.unlockCondition;
		this->sectionList = _in.sectionList;
		return *this;
	}
};

//2. 用户信息
struct TollgateInfor
{
	enum TollgatePassLevel
	{
		eUnfinish = 0, //未完成
		eGood = 1,
		eNice,
		ePerfect,
	};
	unsigned int seasonId;
	unsigned int sectionId;
	vector<std::pair<int,int>> passLevel_gotRewards;  //不同难度通关评级 //是否领取了奖励
	TollgateInfor(){
		seasonId = 0;
		sectionId = 0;
	}
	TollgateInfor(const TollgateInfor& _in){
		this->seasonId = _in.seasonId;
		this->sectionId = _in.sectionId;
		this->passLevel_gotRewards = _in.passLevel_gotRewards;
	}
	const TollgateInfor& operator=(const TollgateInfor& _in){
		this->seasonId = _in.seasonId;
		this->sectionId = _in.sectionId;
		this->passLevel_gotRewards = _in.passLevel_gotRewards;
		return *this;
	}
};

//3.
class TollgateDataManager
{
private:
	static TollgateDataManager* s_mTollgateManager;
	TollgateDataManager();
	void initData();
	void initMapConfigFile();
	void initUserDataInfor();

	void saveData();
public:
	static TollgateDataManager* getManager();

	//config map
	const vector<Season>* getAllTollgate();
	const Season* getOneSeason(unsigned int seasonId);
	const Section* getOneSection(unsigned int seasonId, unsigned int sectionId);

	//infor
	const vector<TollgateInfor>* getTollgateInforList();
	//因为没玩的关卡，一个season的用户信息是不全的，所以不提供获取整个season的info
	//vector<TollgateInfor> getTollgateInfor(unsigned int seasonId);
	const TollgateInfor* getTollgateInfor(unsigned int seasonId, unsigned int sectionId);
	void setTollgateInfor(const TollgateInfor* newInfor);

private:
	/*
	//pref: 
	// 所有的get均为顺序遍历查找，不能使用下标直接访问。（有的关卡可能没有相应的配置信息，直接访问可能会照成数组越界）
	*/
	vector<Season>				mTollgateMapList;
	vector<TollgateInfor>		mTollgateInforList;
};