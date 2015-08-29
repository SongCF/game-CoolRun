//============================================================
//* 活跃度帮助类
//* 逻辑层
//============================================================

#pragma once
#include <vector>
#include "BagDataManager.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

#include "cocos2d.h"
using namespace cocos2d;

#define GetGiftNeedActiveNum 100

//player info 
#define HasActiveCount "HasActiveCount"

struct Active
{
	unsigned int id;
	string describe;
	int value;
};

enum ActiveID
{
	activeType_GouMaiDaoJu = 1,			//购买任意道具
	activeType_PoJiLu,					//打破记录
	activeType_LingQuDengLuJiangLi,		//领取每日登录奖励
	activeType_Pao1000Mi,				//开始一场游戏并跑到1000米
	activeType_MaiXinJueSe,				//购买新角色
	activeType_HuoDeXinChongWu,			//获得新宠物
	activeType_JueSeShengJi,			//升级角色
	activeType_ChongWuShengJi,			//升级宠物
	activeType_WanChengYiGeChengJiu,	//完成一个成就
};

//全是静态函数
class ActiveValueHelper
{
private:
	static ActiveValueHelper* s_ActiveValueHelper;
	ActiveValueHelper();
	bool getFileData();
	bool initData(Data dat);
	bool saveData();
public:
	static ActiveValueHelper* getHelper();

	bool canGetGift();

	BagItem getOneGift_useActiveNum();
	void addActiveByType(ActiveID type);

	const vector<Active>& getAllActive();

	//获取活跃值
	int getHasActiveCount();
protected:
	void setHasActiveCount(int count);
private:
	vector<Active> m_vecAllActive;
	string m_strVersionCode;
};