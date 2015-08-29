//============================================================
//* 排行榜帮助类
//* 逻辑层
//* 提供所有排行榜的逻辑操作
//============================================================

#pragma once

#include <string>
#include <vector>

using std::vector;
using std::string;

struct RankingData
{
	//一个排名奖励 是通过 组groupId 和 rankId来确定的。
	unsigned int groupId;
	unsigned int rankId;

	unsigned int bonus_unitId;
	unsigned int bonusCount;
	string describe;

	RankingData()
	{
		groupId = 0;
		rankId = 0;
		bonus_unitId = 0;
		bonusCount = 0;
	}
	const RankingData& operator=(const RankingData& in)
	{
		groupId = in.groupId;
		rankId = in.rankId;
		bonus_unitId = in.bonus_unitId;
		bonusCount = in.bonusCount;
		describe = in.describe;
		return *this;
	}
};

class RankingListHelper
{
private:
	static RankingListHelper* m_sRankinglistHelper;
	RankingListHelper();
	bool initData();
public:
	~RankingListHelper();
	static RankingListHelper* getHelper();

	RankingData getOneRankingData(unsigned int groupId, unsigned int rankId);
	vector<RankingData> getOneGroupRankingList(unsigned int groupId);
	const vector<RankingData>& getAllRankingList();

private:
	vector<RankingData> m_vecRankingList;
};